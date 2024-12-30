#include "asm/power_interface.h"
#include "app_config.h"
/* #include "audio.h" */
#include "gpio.h"
#include "audio_dac_api.h"
#include "audio_dac.h"
#include "audio_adc.h"
#include "audio.h"
#include "clock.h"
#include "adc_api.h"
#include "key.h"

#define LOG_TAG_CONST       PMU
#define LOG_TAG             "[PMU]"
#include "log.h"

#define P33_IO_WKUP_IOMAP			KEY_WAKEUP_IO
#define P33_IO_WKUP_EDGE			KEY_WAKEUP_EDGE
#define P33_IO_PULLUP_DOWN_ENABLE	KEY_PULLUP_DOWN_ENABLE
#define P33_IO_FILTER_ENABLE 		KEY_FILTER_ENABLE

#define P33_IO_LONG_PRESS_RESET		//IO_PORTB_01
#define VDDIOM_VOL_CONFIG			TCFG_VDDIOM_LEVEL

//充电唤醒--SDK中没有这个充电唤醒流程，只提供对应的充电唤醒IO,用户需根据对应充电IC进行开发,这里只其demo的作用
#define P33_CHARGE_WKUP_EDGE			CHARGE_WAKEUP_EDGE
#define P33_CHARGE_WKUP_IOMAP			CHARGE_WAKEUP_IO
#define P33_CHARGE_PULLUP_DOWN_ENABLE	CHARGE_PULLUP_DOWN_ENABLE
#define P33_CHARGE_FILTER_ENABLE		CHARGE_FILTER_ENABLE

/**************************************************** power_param *********************************************************/
const struct low_power_param power_param = {
    //sniff时芯片是否进入低功耗
    .config         = 1,

    //外接晶振频率
    .btosc_hz       = 24000000,

    .vddiom_lev     = VDDIOM_VOL_CONFIG,

    .vddiow_lev     = 0,

    .osc_type       = OSC_TYPE_LRC,

};

/**************************************************** p33_io_wkup_param *********************************************************/
#if KEY_WAKEUP_ENABLE
//按键唤醒
const struct p33_io_wkup_config port0 = {
    .pullup_down_enable = P33_IO_PULLUP_DOWN_ENABLE,//配置I/O 内部上下拉是否使能
    .filter_enable = P33_IO_FILTER_ENABLE,			//是否需要滤波
    .edge       = P33_IO_WKUP_EDGE,                 //唤醒方式选择,可选：上升沿\下降沿
    .iomap      = P33_IO_WKUP_IOMAP,                //唤醒口选择
};
#endif

#if CHARGE_WAKEUP_ENABLE
//充电唤醒--SDK中没有这个充电唤醒流程，只提供对应的充电唤醒IO,用户需根据对应充电IC进行开发,这里只其demo的作用
const struct p33_io_wkup_config port1 = {
    .pullup_down_enable = P33_CHARGE_PULLUP_DOWN_ENABLE,//配置I/O 内部上下拉是否使能
    .filter_enable = P33_CHARGE_FILTER_ENABLE,			//是否需要滤波
    .edge       = P33_CHARGE_WKUP_EDGE,                 //唤醒方式选择,可选：上升沿\下降沿
    .iomap      = P33_CHARGE_WKUP_IOMAP,                //唤醒口选择
};
#endif

const struct p33_io_wkup_param wkup_param = {
    .filter = PORT_FLT_4ms,//滤波时间,结合上面的filter_enable变量
#if KEY_WAKEUP_ENABLE
    .port[1] = &port0,
#endif

#if CHARGE_WAKEUP_ENABLE
    .port[2] = &port1,
#endif
};

static void audio_close_all(void)
{
    // audio analog close
    SFR(JL_ADDA->DAA_CON2, 19, 1, 0);/*SDDAC_RPABUF_EN [19]*/
    SFR(JL_ADDA->DAA_CON2, 18, 1, 0);/*SDDAC_RPA_EN [18]*/
    SFR(JL_ADDA->DAA_CON2, 14, 1, 0);/*SDDAC_LPABUF_EN [14]*/
    SFR(JL_ADDA->DAA_CON2, 13, 1, 0);/*SDDAC_LPA_EN [13]*/
    SFR(JL_ADDA->DAA_CON2,  0, 1, 0);/*SDDAC_VCMOPA_EN [0]*/

    udelay(10);

    JL_ADDA->DAA_CON2 = BIT(15) | BIT(20);
    JL_ADDA->ADDA_CON0 = 0;
    JL_ADDA->DAA_CON0 = 0;
    JL_ADDA->DAA_CON1 = 0;
    JL_ADDA->DAA_CON3 = 0;

    JL_ADDA->ADA_CON0 = 0;

    JL_ADDA->ADA_CON1 = 0;
    JL_ADDA->ADA_CON2 = 0;
    JL_ADDA->ADA_CON3 = 0;
    JL_ADDA->ADA_CON4 = 0;

    // dma close
    JL_AUDIO->DAC_LEN = 0;
    JL_AUDIO->ADC_LEN = 0;

    // module disenable
    JL_AUDIO->DAC_CON &= ~BIT(4);
    JL_AUDIO->ADC_CON &= ~BIT(4);
    JL_ASS->CLK_CON &= ~(BIT(0) | BIT(1));
    JL_CLOCK->CLK_CON2 &= ~(BIT(0) | BIT(1));
}

void sleep_exit_callback()
{
    adc_exit_power_down_update();

    audio_init();
#if TCFG_DAC_ENABLE
    dac_init_api(8000);
#endif
    audio_adc_analog_open();
    audio_adc_digital_open(16000, 2);

#if TCFG_MIC_CAPLESS
    audio_adc_trim();
#endif

    putchar('>');
}

void sleep_enter_callback()
{
    audio_close_all();
    putchar('<');
}



static void __mask_io_cfg()
{
    struct boot_soft_flag_t boot_soft_flag = {0};

    boot_soft_flag.flag0.boot_ctrl.sfc_fast_boot = 0;
    boot_soft_flag.flag0.boot_ctrl.skip_flash_reset = 0;
    boot_soft_flag.flag0.boot_ctrl.flash_power_keep = 0;
    boot_soft_flag.flag0.boot_ctrl.sfc_flash_stable_delay_sel = 0; //0: 0.5mS; 1: 1mS
    boot_soft_flag.flag0.boot_ctrl.flash_stable_delay_sel = 0;   //0: 0mS;   1: 4mS

    boot_soft_flag.flag2.boot_ctrl.flash_spi_baud = 0;

    boot_soft_flag.flag1.boot_ctrl.usbdm = SOFTFLAG_PD10K;//SOFTFLAG_HIGH_RESISTANCE;
    boot_soft_flag.flag1.boot_ctrl.usbdp = SOFTFLAG_PD10K;//SOFTFLAG_HIGH_RESISTANCE;
    boot_soft_flag.flag3.boot_ctrl.pa12 = SOFTFLAG_OUT0;
    boot_soft_flag.flag3.boot_ctrl.pa13 = SOFTFLAG_OUT1;

    mask_softflag_config(&boot_soft_flag);
}

void board_set_soft_poweroff()
{
    /* audio_off(); */

    u32 gpio_config[4] = {0xffff, 0xffff, 0xffff, 0xffff};
    u32 usbio_config = 0x3;
#define PORT_PROTECT(gpio)	gpio_config[gpio/IO_GROUP_NUM] &= ~BIT(gpio%IO_GROUP_NUM)
#define USB_PROTECT(gpio)   usbio_config &= ~BIT(gpio-IO_PORT_DP)

    PORT_PROTECT(P33_IO_WKUP_IOMAP);

    if (is_mclr_en()) {
        PORT_PROTECT(MCLR_PORT);
    }

    //长按复位
    if (is_pinr_en()) {
        u8 port_sel = get_pinr_port();
        if ((port_sel >= PA0_IN) && (port_sel <= PA14_IN)) {
            PORT_PROTECT(IO_PORTA_00 + (port_sel - PA0_IN));
        } else if (port_sel == USBDP_IN) {
            USB_PROTECT(IO_PORT_DP);
        } else if (port_sel == USBDM_IN) {
            USB_PROTECT(IO_PORT_DM);
        }
    }

    //FLASH
    u32 read_mode = get_sfc_bit_mode();
    PORT_PROTECT(SPI0_CS_A);
    PORT_PROTECT(SPI0_CLK_A);
    PORT_PROTECT(SPI0_DO_D0_A);
    if (read_mode != 0) {
        PORT_PROTECT(SPI0_DI_D1_A);
        if (read_mode == 4) {
            PORT_PROTECT(SPI0_WP_D2_A);
            PORT_PROTECT(SPI0_HOLD_D3_A);
        }
    }

    __mask_io_cfg();

    gpio_close(JL_PORTA, gpio_config[0]);
    gpio_close(JL_PORTF, gpio_config[3]);
    if (usbio_config & BIT(0)) {
        gpio_set_direction(IO_PORT_DP, 1);
        gpio_set_die(IO_PORT_DP, 0);
        gpio_set_dieh(IO_PORT_DP, 0);
        gpio_set_pull_up(IO_PORT_DP, 0);
        gpio_set_pull_down(IO_PORT_DP, 0);
    }

    if (usbio_config & BIT(1)) {
        gpio_set_direction(IO_PORT_DM, 1);
        gpio_set_die(IO_PORT_DM, 0);
        gpio_set_dieh(IO_PORT_DM, 0);
        gpio_set_pull_up(IO_PORT_DM, 0);
        gpio_set_pull_down(IO_PORT_DM, 0);
    }
}

void sys_power_init()
{
    power_config_vdc13_cap(0);
    power_config_flash_pg_keep(0);
    power_config_sf_vddio_keep(VDDIO_KEEP_TYPE_NORMAL);
    power_config_pd_vddio_keep(VDDIO_KEEP_TYPE_NORMAL);

    power_init(&power_param);

    power_wakeup_init(&wkup_param);

    soff_latch_release();

    /*
    while (1) {
        sys_power_down(3000000);
    }
    */
    /* printf("\nSOFTOFF\n"); */
    /* power_set_soft_poweroff(); */
}
