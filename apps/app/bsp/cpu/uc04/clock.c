#include "clock.h"
#include "config.h"
#include "efuse.h"
#include "common.h"
#include "app_config.h"
#include "asm/power_interface.h"
#include "gpio.h"
#include "jiffies.h"

const int  clock_sys_src_use_lrc_hw = 1;

/* extern void otp_init(u32 mode); */
extern void wait_cache_idle();

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[clock]"
#include "log.h"

#define     SPI_TSHSL       30
static u32 LSB_MAX_CLK = 80 * MHz_UNIT;
static u32 SPI_MAX_CLK = 96 * MHz_UNIT;

static u32 sys_clock = 16000000;
const u32 pll_clock_tab0[3][6] = {
    //0  d3p5        d2p5        d2p0        d1p5        d1p0
    {0,  82300000L, 115200000L, 144000000L, 192000000L, 288000000L},
    {0, 109700000L, 153600000L, 192000000L, 256000000L, 384000000L},
    {0, 137100000L, 192000000L, 240000000L, 320000000L, 480000000L},
};
#define     MAX_RANGE   2
const u32 pll_max_clock_tab[MAX_RANGE][4] = {
    //dvdd                sys max     lsb max     otp max
    {DVDD_VOL_SEL_102V, 128000000L, 64000000L,  21333334L},//1.20v
    {DVDD_VOL_SEL_105V, 160000000L, 80000000L,  20000000L} //1.32v
};

const u8 div_taba[] = {
    1, 3, 5, 7
};

const u8 div_tabb[] = {
    1, 2, 4, 8
};

__attribute__((always_inline))
int get_sys_pll_sel()
{
    return JL_CLOCK->CLK_CON1 & 0x7;
}

__attribute__((always_inline))
u32 sys_clock_get(void)
{
    return sys_clock;
}

AT(.clock.text.cache.L1)
void delay(u32 t)
{
    while (t--) {
        asm("nop");
    }
}

AT(.clock.text.cache.L1)
void udelay(u32 us)
{
    u32 mips = us * (sys_clock / 1000000 / 3);

    while (mips--) {
        asm("nop");
    }
}

AT(.clock.text.cache.L1)
void mdelay(u32 ms)
{
    for (int i = 0; i < ms; i++) {
        udelay(1000);
    }
}

void udelay_margin(u32 usec)
{
    udelay(usec * 3);
}

static u32 pll_target_clk = 480000000;
static u8 pll_dp[] = {7, 5, 4, 3, 2};
__attribute__((always_inline))
u32 get_sys_pll_clk()
{
    u32 t_sel;
    u32 clock = 0;
    t_sel = get_sys_pll_sel();
    clock = pll_target_clk * 2 / pll_dp[t_sel - 1];
    if (0 == clock) {
        log_info(" sys clock info err\n");
        return 0;
    }
    u32 t_diva = div_taba[(JL_CLOCK->CLK_CON1 >> 4) & 0x3];
    u32 t_divb = div_tabb[(JL_CLOCK->CLK_CON1 >> 6) & 0x3];
    return clock / (t_diva * t_divb);
}
__attribute__((always_inline))
u32 lrc_clk_get(void)
{
    return pll_target_clk / (JL_PLL0->PLL_NR & 0xfff);
}

__attribute__((always_inline))
u32 lsb_clk_get(void)
{
    u32 div42 = ((JL_CLOCK->SYS_DIV >> 8) & 0xf) + 1;
    u32 lsb_clk = sys_clock_get() / (div42);

    return lsb_clk;
}
__attribute__((always_inline))
u32 sys_clock_peration(void)
{
    u32 hsb_div = (JL_CLOCK->SYS_DIV & 0xff) + 1;
    u32 pll_sys_clk = get_sys_pll_clk();
    u32 hsb_clk = pll_sys_clk / (hsb_div);

    /* log_info(" sys clock %ld\n", hsb_clk); */
    return hsb_clk;
}

void IcuWaitIdle(void);
AT(.clock.text.cache.L1)
void sfc_suspend(u32 enable_spi)
{
    //wait cache idle
    IcuWaitIdle();
    //wait sfc idle
    while (JL_SFC->CON & BIT(31));
    //disable sfc
    JL_SFC->CON &= ~BIT(0);
    if (enable_spi) {
        JL_SPI0->CON |= BIT(0);
    }
}


AT(.clock.text.cache.L1)
void sfc_resume(u32 disable_spi)
{
    if (disable_spi) {
        JL_SPI0->CON &= ~BIT(0);
    }
    JL_SFC->CON |= BIT(0);
}

#define FREQ_BY_DIV(freq, div)      (freq / (div + 1))
AT(.clock.text.cache.L2)
static u8 clk_limit(u32 frequency, u32 clk_limit, u8 div_max)
{
    u8 div;

    for (div = 0; div <= div_max; div++) {
        if (FREQ_BY_DIV(frequency, div) <= clk_limit) {
            break;
        }
    }

    return div;
}

static u32 sfc_clk ;
AT(.clock.text.cache.L2)
void sfc_baud_set(u32 baud)
{
    /* local_irq_disable(); */
    sfc_suspend(0);
    const u32 tshsl = SPI_TSHSL * (sfc_clk / 1000000) / 1000 + 1;
    // see https://gitee.com/Jieli-Tech/fw-AD15N/issues/I41WDD
    /* const u32 tshsl = 0x7; */
    SFR(JL_SFC->CON, 20, 4, tshsl);
    JL_SFC->QUCNT = 0x1f;
    JL_SFC->BAUD = baud;
    sfc_resume(0);
    /* local_irq_enable(); */
}
AT(.clock.text.cache.L2)
u32 otp_baud_adjust()
{
    /* u32 otp_div = clk_limit(sys_clock, OTP_MAX_CLK, 255); */
    /* wait_cache_idle(); */

    /* JL_OTP->WREN = 0xe6; */
    /* SFR(JL_OTP->CON0, 4, 8, otp_div); */
    /* JL_OTP->WREN = 0xe6; */
    /* return sys_clock / (otp_div + 1); */
    return 0;
}

__attribute__((always_inline))
void main_clock_sel(u32 sys_clk_src)
{
    if (sys_clk_src == SYS_SRC_RC16M) {
        sys_clock = 16000000;
        /* } else if (sys_clk_src == SYS_SRC_RC1M) { */
        /*     sys_clock = 1000000; */
    }
    SFR(JL_CLOCK->CLK_CON1, 8, 3, sys_clk_src);
    asm("csync");
    asm("nop");
}

__attribute__((always_inline))
void hsb_clk_div(u32 div)
{
    SFR(JL_CLOCK->SYS_DIV, 0, 8, div);
}
__attribute__((always_inline))
void lsb_clk_div(u32 div)
{
    SFR(JL_CLOCK->SYS_DIV, 8, 4, div);
}

AT(.clock.text.cache.L2)
static u32 sfc_baud_adjust()
{
    sfc_clk = get_sys_pll_clk();
    u32 sfc_div = clk_limit(sfc_clk, SPI_MAX_CLK, 255);
    sfc_baud_set(sfc_div);
    return sfc_clk / (sfc_div + 1);
}


/***********************************************************************************/
#define lrc_con0_init \
		/*UNUSED          1bit */ (0<<7) | \
		/*RC20OK_CAPS     3bit */ (2<<4) | \
		/*UNUSED          2bit */ (0<<2) | \
		/*RC20OK_RN_TRIM  1bit */ (1<<1) | \
		/*RC20OK_EN       1bit */ (1<<0)
#define lrc_con1_init \
		/*UNUSED          2bit */ (0<<6) | \
		/*RC20OK_RPPS     2bit */ (3<<4) | \
		/*UNUSED          2bit */ (0<<2) | \
		/*RC20OK_RNPS     2bit */ (0<<0)


AT(.clock.text.cache.L2)
static void lrc_init(void)
{
    if (p33_rx_1byte(P3_LRC_CON0) & BIT(0)) {
        return;
    }
    p33_tx_1byte(P3_LRC_CON1, lrc_con1_init);
    p33_tx_1byte(P3_LRC_CON0, lrc_con0_init);
    mdelay(1);
}

static void hw_pll_enable()
{
    //reset
    /* log_info("HAL - pll_enable"); */
    JL_PLL0->PLL_CON0 |= BIT(0);
    //rc16m
    udelay(100);//>10us
    JL_PLL0->PLL_CON0 |= BIT(1);
    udelay(600);//>300us//200k mode:1ms,2m mode:200us
}

static void hw_pll_disable(void)
{
    /* log_info("HAL - pll_disable"); */
    SFR(JL_PLL0->PLL_CON1, 11, 5, 0); //pll en
    JL_PLL0->PLL_CON0 &= ~BIT(0);
    JL_PLL0->PLL_CON0 &= ~BIT(1);
}

#define PLL_REF_SEL0(x)				SFR(pll_con0_data, 2, 2, x)
#define PLL_MODE(x)					SFR(pll_con0_data, 5, 1, x)
#define PLL_PFD(x)					SFR(pll_con0_data, 6, 2, x)
#define PLL_ICP(x)					SFR(pll_con0_data, 8, 3, x)
#define PLL_LPFR2(x)				SFR(pll_con0_data, 11, 3, x)
#define PLL_IVCO(x)					SFR(pll_con0_data, 14, 3, x)
#define PLL_LDO12A(x)				SFR(pll_con0_data, 17, 3, x)
#define PLL_LDO12D(x)				SFR(pll_con0_data, 20, 3, x)
#define PLL_LDO_BYPASS(x)			SFR(pll_con0_data, 23, 1, x)
#define PLL_REFDS(x)				SFR(pll_con1_data, 0, 7, x)
#define PLL_REFDSEN(x)				SFR(pll_con1_data, 7, 2, x)
#define PLL_200K_EN(x)
static void pll_init(_PLL_REF_SEL pll_ref, u32 input_freq, u32 pll_vco)
{
    /* log_info("--func = %s", __FUNCTION__); */
    /* log_info("PLL in freq : 0x%d", input_freq); */
    u32 pll_con0_data = 0;
    u32 pll_con1_data = 0;
    u32 pll_nr_data = 0;
    u8 pll_ivco = 0b011;//??????????
    u32 pll_refds = 0;

    PLL_MODE(0b0);
    PLL_PFD(0b01);
    PLL_LDO12A(0b100);
    PLL_LDO12D(0b100);
    PLL_LDO_BYPASS(0b0);

    PLL_IVCO(pll_ivco);

    pll_refds = input_freq / 2000000 - 2;

    if (pll_ref == PLL_REF_SEL_LRC_200K) {		//unavailable
        /* __hw_lrc_init(); */
        lrc_init();//++
        PLL_REF_SEL0(0b00);
        PLL_REFDSEN(0b01);
        PLL_ICP(0b000);
        PLL_LPFR2(0b111);
        PLL_200K_EN(0b1);
        u32 lrc_clk = 480000000 / (JL_PLL0->PLL_NR & 0xfff);
        /* lrc_clk = 229360; */
        pll_nr_data = pll_vco / lrc_clk;//lrc校正
        pll_refds = 0;
    } else if (pll_ref == PLL_REF_SEL_RC_16M) {//???????//available only!!!
        PLL_REF_SEL0(0b10);
        PLL_REFDSEN(0b10);
        PLL_ICP(0b100);//????????
        PLL_LPFR2(0b101);//????????
        PLL_200K_EN(0b0);
        /* } else if (pll_ref == PLL_REF_SEL_BTOSC_24M) {	//available only!!! */
        /*     btosc_init(); */
        /*     PLL_REF_SEL0(0b01); */
        /*     PLL_REF_SEL1(0b1); */
        /*     PLL_REFDSEN(0b10); */
        /*     PLL_ICP(0b100); */
        /*     PLL_LPFR2(0b101); */
        /*     PLL_200K_EN(0b0); */
    }
    PLL_REFDS(pll_refds);

    hw_pll_disable();

    JL_PLL0->PLL_CON0 = pll_con0_data;
    JL_PLL0->PLL_CON1 = pll_con1_data;
    JL_PLL0->PLL_NR   = pll_nr_data;

    hw_pll_enable();
}

#if 0/*{{{*/
#define PLL_REF_SEL(x)         SFR(pll_con0_data,  3,  2,  (x))
#define PLL_PFDS(x)            SFR(pll_con0_data,  5,  2,  (x))
#define PLL_ICPS(x)            SFR(pll_con0_data,  7,  2,  (x))
#define PLL_LPFR2(x)           SFR(pll_con0_data,  9,  2,  (x))
#define PLL_IVCOS(x)           SFR(pll_con0_data, 11,  3,  (x))
#define PLL_LDO12A_S(x)          SFR(pll_con0_data, 14,  2,  (x))
#define PLL_LDO12D_S(x)          SFR(pll_con0_data, 16,  2,  (x))
#define PLL_LDO_BYPASS(x)      SFR(pll_con0_data, 18,  1,  (x))
#define PLL_TEST_SEL(x)        SFR(pll_con0_data,  19, 2,  x)
#define PLL_TEST_EN(x)         SFR(pll_con0_data,  21, 1,  x)
#define PLL_VCTRL_OE(x)        SFR(pll_con0_data,  22, 1,  x)
#define PLL_VBG_TRIM(x)          SFR(pll_con0_data, 23,  4,  (x))
#define PLL_REFDS(x)           SFR(pll_con1_data, 0,  5,  (x))
#define PLL_REFDSEN(x)         SFR(pll_con1_data, 5,  2,  (x))
#define PLL_200K_EN(x)         SFR(pll_con1_data, 7,  1,  (x))
AT(.clock.text.cache.L2)
static void pll_init(_PLL_REF_SEL pll_ref, u32 input_clk, _PLL_VCO_SEL pll_vco)//XOSC:input_clk=(1~33)*2M (1MHz,2MHz~66MHz)
{
    u32 pll_nr_data = 0;
    u32 pll_src_ref = 2000000;
    u8 pll_ivcos = 0;
    u8 _pll_refds = 0;
    u32 pll_con0_data = 0;
    u32 pll_con1_data = 0;

    PLL_PFDS(0b10);
    PLL_LDO12A_S(0b01);
    PLL_LDO12D_S(0b01);
    PLL_VBG_TRIM(0b0111);



    if (pll_ref == PLL_REF_SEL_LRC_200K) {
        lrc_init();
        PLL_REF_SEL(0b00);
        PLL_REFDSEN(0b01);
        PLL_REFDS(0b00000);
        PLL_ICPS(0b01);
        PLL_LPFR2(0b11);
        PLL_200K_EN(0b1);
        pll_nr_data = efuse_get_lrc_pll_nr();
        pll_src_ref = 192000000 / pll_nr_data;
    } else if (pll_ref == PLL_REF_SEL_XOSC) {
        xosc_12m_init();
        PLL_REF_SEL(0b01);
        if (input_clk == 1000000) {
            PLL_REFDSEN(0b00);
            PLL_REFDS(0);//n
        } else if (input_clk == 2000000) {
            PLL_REFDSEN(0b01);
            PLL_REFDS(0);//n
        } else if (input_clk >= 4000000) {
            _pll_refds = input_clk / 2000000;
            _pll_refds -= 2;
            PLL_REFDSEN(0b10);
            PLL_REFDS(_pll_refds);//n
        }
        PLL_ICPS(0b01);
        PLL_LPFR2(0b10);
        PLL_200K_EN(0b0);
    }
    if (pll_vco == PLL_VCO_SEL_96M) {
        pll_ivcos  = 0;
        pll_nr_data = 96000000 / pll_src_ref;
    } else if (pll_vco == PLL_VCO_SEL_144M) {
        pll_ivcos =  1;
        pll_nr_data = 144000000 / pll_src_ref;
    } else if (pll_vco == PLL_VCO_SEL_192M) {
        pll_ivcos =  5;
        pll_nr_data = 192000000 / pll_src_ref;
    } else if (pll_vco == PLL_VCO_SEL_240M) {
        pll_ivcos =  7;
        pll_nr_data = 240000000 / pll_src_ref;
    }
    PLL_IVCOS(pll_ivcos);

    JL_PLL0->PLL_CON0 = pll_con0_data;
    JL_PLL0->PLL_CON1 = pll_con1_data;
    JL_PLL0->PLL_NR   = pll_nr_data;

    PLL_EN(1);
    udelay(10);//10us
    PLL_RST(1);
    udelay(80);//80us
}
#endif/*}}}*/

AT(.clock.text.cache.L2)
static void pll_sel_phy(u32 pll_clock, _PLL_DIV pll_div, _HSB_CLK_DIV hsb_div)
{
    u32 pll_vco = PLL_VCO_SEL_480M;
    u8 pll_96m = 2;
__change_clk:
    if (pll_clock <= PLL_D1p0_288M) {
        pll_vco = PLL_VCO_SEL_288M;
        pll_96m = 4;
    } else if (pll_clock <= PLL_D1p0_384M) {
        pll_vco = PLL_VCO_SEL_384M;
        pll_clock >>= 4;
        pll_96m = 3;
    } else if (pll_clock <= PLL_D1p0_480M) {
        pll_vco = PLL_VCO_SEL_480M;
        pll_clock >>= 8;
        pll_96m = 2;
    }

    local_irq_disable();
    main_clock_sel(SYS_SRC_RC16M);
    hsb_clk_div(0);

    pll_target_clk = pll_vco;

    if (clock_sys_src_use_lrc_hw) {
        pll_init(PLL_REF_SEL_LRC_200K, 0, pll_vco);
    } else {
        pll_init(PLL_REF_SEL_RC_16M, 16000000, pll_vco);
    }


    // enable all oe
    /* SFR(JL_PLL0->PLL_CON1, 11, 5, 0b11111); */
    JL_PLL0->PLL_CON1 |= (0b11111 << 11);

    /* SFR(JL_CLOCK->CLK_CON1, 0, 3, pll_clock); */
    /* SFR(JL_CLOCK->CLK_CON1, 4, 4, pll_div); */
    JL_CLOCK->CLK_CON1 &= ~((7 << 0) | (0xf << 4));
    JL_CLOCK->CLK_CON1 |= ((pll_clock << 0) | (pll_div << 4));

    hsb_clk_div(hsb_div);
    lsb_clk_div(0);

    //dvdd
#if 0
    u8 clk_err_flag = 0;
    u32 pll_sys_clk = get_sys_pll_clk();
    u8 dvdd_range = 0;
    for (dvdd_range = 0; dvdd_range < MAX_RANGE; dvdd_range++) {
        if (pll_sys_clk <= pll_max_clock_tab[dvdd_range][1]) {
            break;
        }
    }
    if (dvdd_range == MAX_RANGE) { //>160M error :改为160M
        clk_err_flag = 1;
        if (pll_vco == PLL_VCO_SEL_192M) {
            pll_clock = PLL_D1p5_160M;
            local_irq_enable();
            goto __change_clk;//
        } else {
            pll_sys_clk = 160000000;
            pll_clock = 4;
            JL_CLOCK->CLK_CON1 &= ~((7 << 0) | (0xf << 4));
            JL_CLOCK->CLK_CON1 |= ((pll_clock << 0) | (pll_div << 4));
            dvdd_range = MAX_RANGE - 1;
        }
    }
    dvdd_vol_sel(pll_max_clock_tab[dvdd_range][0]);//1.32v default:1.11v RV:1.2v
    LSB_MAX_CLK = pll_max_clock_tab[dvdd_range][2];
    /* OTP_MAX_CLK = pll_max_clock_tab[dvdd_range][3]; */
#endif
    sys_clock = sys_clock_peration();

    u32 lsb_div = clk_limit(sys_clock, LSB_MAX_CLK, 15);
    lsb_clk_div(lsb_div);

    main_clock_sel(SYS_SRC_PLL_SYS_CLK);
    udelay(10);

    u32 spi_clk = sfc_baud_adjust();
    /* u32 otp_clk = otp_baud_adjust(); */

    PLL_96M_SEL(pll_96m);//std_clk config pll
    STD_48M_SEL(0);//sel pll_96 div2  -> std48_clk
    STD_12M_SEL(0);//std48 div2 -> std12m

    //sel ass clk(audio clk) STD48M
    SFR(JL_CLOCK->CLK_CON2, 0, 2, 1);

    //std48---> usb
    SFR(JL_CLOCK->CLK_CON3, 6, 2, 0b01);
    //std48---> uart
    SFR(JL_CLOCK->CLK_CON3, 9, 2, 0b01);
    //p33 clk div
    /* SFR(JL_CLOCK->CLK_CON0, 10, 3, 0b001); */

    u32 spi0_div = clk_limit(lsb_clk_get(), 24000000L, 0xff);
    JL_SPI0->BAUD = spi0_div;

    local_irq_enable();
    /* if (clk_err_flag) { //>160M error */
    /*     log_error("pll sys clock must be no larger than 160M. The clock is forcibly set to %d.", pll_sys_clk); */
    /*     while (1) { */
    /*         asm("idle"); */
    /*     } */
    /* } */
}

#define CACHE_LINE_COUNT            (32)
extern u32 _CLK_CODE_START;
extern u32 _CLK_CODE_END;
AT(.clock.text.cache.L1)
static void load_clk_code2cache(void)
{
    u32 ali_start = ((u32)&_CLK_CODE_START & ~(CACHE_LINE_COUNT - 1));
    u32 ali_end = (u32)&_CLK_CODE_END + (8 * CACHE_LINE_COUNT);
    IcuPfetchRegion((u32 *)ali_start, ali_end - ali_start);
}

AT(.clock.text.cache.L1)
void pll_sel(u32 pll_clock, _PLL_DIV pll_div, _HSB_CLK_DIV hsb_div)
{
    local_irq_disable();
    load_clk_code2cache();
    pll_sel_phy(pll_clock, pll_div, hsb_div);
    local_irq_enable();
}

void dump_clock_info()
{
    u32 sfc_clk = get_sys_pll_clk();
    /* u32 otp_clk = sys_clock / (((JL_OTP->CON0 >> 4) & 0xf) + 1); */
    log_info("---SFC CLK : %d", sfc_clk);
    log_info("---SPI CLK : %d", sfc_clk / (JL_SFC->BAUD + 1));
    log_info("---HSB CLK : %d", sys_clock);
    /* log_info("---OTP CLK : %d", otp_clk); */
    log_info("---LSB CLK : %d", clk_get("lsb"));
    /* log_info("---LRC CLK : %d", 192000000 / efuse_get_lrc_pll_nr()); */
    log_info("---LRC CLK : %d", pll_target_clk / (JL_PLL0->PLL_NR & 0xfff));
}
__attribute__((always_inline_when_const_args))
int clk_get(const char *name)
{
    if (!strcmp(name, "sys")) {
        return sys_clock_get();
    }
    if (!strcmp(name, "uart")) {
        return (48 * 1000000);
    }
    if (!strcmp(name, "timer")) {
        return lsb_clk_get();//(48 * 1000000);
    }
    if (!strcmp(name, "lsb")) {
        return lsb_clk_get();
    }
    if (!strcmp(name, "spi")) {
        return lsb_clk_get();
    }
    if (!strcmp(name, "adc")) {
        return lsb_clk_get();
    }
    if (!strcmp(name, "mcpwm")) {
        return lsb_clk_get();
    }
    if (!strcmp(name, "lrc")) {
        return lrc_clk_get();
    }
    return 0;
}

void update_vdd_table(u8 val)
{

}

