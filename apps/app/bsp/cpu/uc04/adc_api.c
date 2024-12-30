#include "typedef.h"
#include "clock.h"
#include "adc_api.h"
#include "gpio.h"
#include "init.h"
/* #include "efuse.h" */
#include "irq.h"
#include "p33_interface.h"
#include "jiffies.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[saradc]"
#include "log.h"

u8 cur_ch = ADC_MAX_CH;  //adc采集队列当前编号
u8 adc_clk_div = 0; //adc时钟分频系数

struct adc_info_t adc_queue[ADC_MAX_CH + ENABLE_PREEMPTIVE_MODE];  //采集队列声明

static u8 adc_ch_io_table[16] = {  //gpio->adc_ch 表
    IO_PORTA_00,
    IO_PORTA_01,
    IO_PORTA_02,
    IO_PORTA_03,
    IO_PORTA_04,
    IO_PORTA_05,
    IO_PORTA_06,
    IO_PORTA_07,
    IO_PORTA_08,
    IO_PORTA_09,
    IO_PORTA_10,
    IO_PORTA_11,
    IO_PORTA_12,
    IO_PORTA_13,
    IO_PORT_DP,
    IO_PORT_DM,
};

static void clock_critical_enter(void)
{
}
static void adc_adjust_div(void)
{
    const u8 adc_div_table[] = {1, 6, 12, 24, 48, 72, 96, 128};
    const u32 lsb_clk = clk_get("lsb");
    adc_clk_div = 7;
    for (int i = 0; i < ARRAY_SIZE(adc_div_table); i++) {
        if (lsb_clk / adc_div_table[i] <= 1000000) {
            adc_clk_div = i;
            break;
        }
    }

}

u32 adc_io2ch(int gpio)   //根据传入的GPIO，返回对应的ADC_CH
{
    for (u8 i = 0; i < ARRAY_SIZE(adc_ch_io_table); i++) {
        if (adc_ch_io_table[i] == gpio) {
            return (ADC_CH_TYPE_IO | i);
        }
    }
    log_info("add_adc_ch io error!!! change other io_port!!!\n");
    return 0xffffffff; //未找到支持ADC的IO
}

void adc_io_ch_set(enum AD_CH ch, u32 mode) //adc io通道 模式设置
{

}

void adc_internal_signal_to_io(u32 analog_ch, u8 adc_io) //将内部通道信号，接到IO口上，输出
{
    gpio_set_mode(adc_io, PORT_HIGHZ);

    adc_sample(analog_ch, 0);
    u32 ch = adc_io2ch(adc_io);
    u16 adc_ch_sel = ch & ADC_CH_MASK_CH_SEL;
    SFR(JL_ADC->CON, 8, 4, adc_ch_sel);
    SFR(JL_ADC->CON, 21, 3, 0b111);
}

static void _adc_pmu_vbg_enable()
{
    p33_or_1byte(P3_PMU_ADC0, (u8)BIT(4));
    udelay(11);
    udelay(11);
    udelay(11);
    udelay(11);
    udelay(11);
    p33_or_1byte(P3_PMU_ADC0, (u8)BIT(5));
}
static void _adc_pmu_vbg_diable()
{
    p33_and_1byte(P3_PMU_ADC0, (u8)~BIT(5));
    udelay(11);
    udelay(11);
    udelay(11);
    udelay(11);
    udelay(11);
    p33_and_1byte(P3_PMU_ADC0, (u8)~BIT(4));
}

static void _adc_pmu_ch_select(u16 ch)
{
    if (ch == 0) {
        P33_CON_SET(P3_PMU_ADC0, 6, 2, 0b01);//固定采MBG08
    } else {
    }
    P33_CON_SET(P3_PMU_ADC0, 0, 4, ch);
    P33_CON_SET(P3_PMU_ADC1, 0, 1, 1);
    P33_CON_SET(P3_PMU_ADC1, 1, 1, 1);
}

static void adc_audio_ch_select(u16 ch)
{
    static u8 ch_bit;
    if ((ch & ADC_AUDIO_CAL) == ADC_AUDIO_SUB) {
        JL_ADDA->ADDA_CON0 &= ~BIT(7);
        ch_bit = ch & 0xf;
        JL_ADDA->ADDA_CON0 |= BIT(ch_bit);
    } else {
        SFR(JL_ADDA->ADDA_CON0, 0, 8, BIT(7));
        JL_ADDA->ADDA_CON0 |= BIT(7);
        SFR(JL_ADDA->ADDA_CON0, 8, 3, (ch & 0x07));
    }
}

u32 adc_add_sample_ch(enum AD_CH ch)   //添加一个指定的通道到采集队列
{
    u32 adc_type_sel = ch & ADC_CH_MASK_TYPE_SEL;
    u16 adc_ch_sel = ch & ADC_CH_MASK_CH_SEL;
    log_info("type = %x,ch = %x\n", adc_type_sel, adc_ch_sel);
    u32 i = 0;
    for (i = 0; i < ADC_MAX_CH; i++) {
        if (adc_queue[i].ch == ch) {
            break;
        } else if (adc_queue[i].ch == -1) {
            adc_queue[i].ch = ch;
            adc_queue[i].v.value = (u16) - 1;
            adc_queue[i].sample_period = 0;
#if AD_CH_IO_VBAT_PORT
            if (ch == AD_CH_IO_VBAT_PORT) {
                adc_queue[i].adc_voltage_mode = 1;
                continue;
            }
#endif
            switch (ch) {
            case AD_CH_LDOREF:
                adc_queue[i].adc_voltage_mode = 0;
                break;
            case AD_CH_PMU_VBAT:
                adc_queue[i].adc_voltage_mode = 1;
                break;
            case AD_CH_PMU_VTEMP:
                adc_queue[i].adc_voltage_mode = 1;
                break;
            default:
                adc_queue[i].adc_voltage_mode = 0;
                break;
            }
            break;
        }
    }
    return i;
}

u32 adc_delete_ch(enum AD_CH ch)    //将一个指定的通道从采集队列中删除
{
    u32 i = 0;
    for (i = 0; i < ADC_MAX_CH; i++) {
        if (adc_queue[i].ch == ch) {
            adc_queue[i].ch = -1;
            break;
        }
    }
    return i;
}

void adc_sample(enum AD_CH ch, u32 ie) //启动一次cpu模式的adc采样
{
    u32 adc_con = 0;
    SFR(adc_con, 0, 3, adc_clk_div);//adc_clk 分频
    adc_con |= (0xf << 12); //启动延时控制，实际启动延时为此数值*8个ADC时钟
    adc_con |= BIT(3) | BIT(30); //ana en
    if (ie) {
        adc_con |= BIT(29);//ie
    }
    adc_con |= BIT(17);//clk en

    u32 adc_type_sel = ch & ADC_CH_MASK_TYPE_SEL;
    u16 adc_ch_sel = ch & ADC_CH_MASK_CH_SEL;
    SFR(adc_con, 21, 3, 0b010);//cpu adc test sel en
    SFR(adc_con, 18, 3, adc_type_sel >> 16);    //test sel
    switch (adc_type_sel) {
    case ADC_CH_TYPE_PMU:
        _adc_pmu_ch_select(adc_ch_sel);
        break;
    case ADC_CH_TYPE_AUDIO:
        adc_audio_ch_select(adc_ch_sel);
        break;
    case ADC_CH_TYPE_PLL:
        break;
    default:
        SFR(adc_con, 21, 3, 0b001); //cpu adc io sel en
        SFR(adc_con, 8, 4, adc_ch_sel);
        break;
    }
    JL_ADC->CON = adc_con;
    JL_ADC->CON |= BIT(4);//en
    JL_ADC->CON |= BIT(6);//kistart
}

static void adc_wait_idle_timeout()
{
    u32 time_out_us = 1000;
    while (time_out_us--) {
        asm("csync");
        if (JL_ADC->CON & BIT(31)) {
            return;
        }
        if ((JL_ADC->CON & BIT(4)) == 0) {
            return;
        }
        udelay(1);
    }
}

void adc_wait_enter_idle() //等待adc进入空闲状态，才可进行阻塞式采集
{
    if (JL_ADC->CON & BIT(4)) {
        adc_wait_idle_timeout();
        adc_close();
    } else {
        return ;
    }
}
void adc_set_enter_idle() //设置 adc cpu模式为空闲状态
{
    /* JL_ADC->CON &= ~BIT(4); */
}
u16 adc_wait_pnd()   //cpu采集等待pnd
{
    adc_wait_idle_timeout();
    u32 adc_res = JL_ADC->RES;
    adc_close();
    return adc_res;
}

void adc_close()     //adc close
{
    JL_ADC->CON = BIT(17);//clock_en
    JL_ADC->CON = BIT(17) | BIT(30);
    JL_ADC->CON = BIT(30);
}

___interrupt
static void adc_isr()   //中断函数
{
    if (adc_queue[ADC_MAX_CH].ch != -1) {
        adc_close();
        return;
    }
    if (JL_ADC->CON & BIT(31)) {
        u32 adc_value = JL_ADC->RES;

        JL_ADC->CON |= BIT(30);	//清pend
        SFR(JL_ADDA->ADDA_CON0,  0, 8, 0);
        P33_CON_SET(P3_PMU_ADC1, 0, 1, 0);
        P33_CON_SET(P3_PMU_ADC1, 1, 1, 0);//关闭所有的通道

        if (adc_cpu_mode_process(adc_value)) {
            return;
        }
        cur_ch++;
        cur_ch = adc_get_next_ch();
        if (cur_ch == ADC_MAX_CH) {
            adc_close();
        } else {
            adc_sample(adc_queue[cur_ch].ch, 1);
        }
    }

}

void adc_api_scan()  //定时函数，每 x ms启动一轮cpu模式采集
{
    if (adc_queue[ADC_MAX_CH].ch != -1) {
        return;
    }
    if (JL_ADC->CON & BIT(4)) {
        return;
    }
    cur_ch = adc_get_next_ch();
    if (cur_ch == ADC_MAX_CH) {
        return;
    }
    adc_sample(adc_queue[cur_ch].ch, 1);
}

void adc_hw_init(void)    //adc初始化子函数
{
    memset(adc_queue, 0xff, sizeof(adc_queue));

    u32 voltages;
    adc_close();

    adc_adjust_div();//ADC 时钟分频

    adc_add_sample_ch(AD_CH_LDOREF);
    adc_set_sample_period(AD_CH_LDOREF, PMU_CH_SAMPLE_PERIOD);

    adc_add_sample_ch(AD_CH_PMU_VBAT);
    adc_set_sample_period(AD_CH_PMU_VBAT, PMU_CH_SAMPLE_PERIOD);

#if AD_CH_IO_VBAT_PORT
    adc_add_sample_ch(adc_io2ch(AD_CH_IO_VBAT_PORT));
    adc_set_sample_period(adc_io2ch(AD_CH_IO_VBAT_PORT), PMU_CH_SAMPLE_PERIOD);
#endif

    u32 vbg_adc_value = 0;
    u32 vbg_min_value = -1;
    u32 vbg_max_value = 0;

    for (int i = 0; i < AD_CH_PMU_VBG_TRIM_NUM; i++) {
        u32 adc_value = adc_get_value_blocking(AD_CH_LDOREF);
        if (adc_value > vbg_max_value) {
            vbg_max_value = adc_value;
        } else if (adc_value < vbg_min_value) {
            vbg_min_value = adc_value;
        }
        vbg_adc_value += adc_value;
    }
    vbg_adc_value -= vbg_max_value;
    vbg_adc_value -= vbg_min_value;

    vbg_adc_value /= (AD_CH_PMU_VBG_TRIM_NUM - 2);
    adc_queue[0].v.value = vbg_adc_value;
    log_info("LDOREF = %d\n", vbg_adc_value);

    u32 voltage = adc_get_voltage_blocking(AD_CH_PMU_VBAT);
    adc_queue[1].v.voltage = voltage;
    log_info(" voltage = %d \n ", voltage);
    log_info("vbat = %d mv\n", adc_get_voltage(AD_CH_PMU_VBAT) * 4);

#if AD_CH_IO_VBAT_PORT
    voltage = adc_get_voltage_blocking(adc_io2ch(AD_CH_IO_VBAT_PORT));
    adc_queue[2].v.voltage = voltage;
    log_info("io_vbat = %d mv\n", voltage);
#endif

    request_irq(IRQ_ADC_IDX, IRQ_ADC_IP, adc_isr, 0);//注册中断函数
}

static void adc_test_demo()  //adc测试函数，根据需求搭建
{
    /* log_info("\n\n%s() CHIP_ID :%x\n", __func__, get_chip_version()); */
    /* log_info("%s() VBG:%d\n", __func__, adc_get_value(AD_CH_LDOREF)); */
    log_info("%s() VBAT:%d mv\n", __func__, adc_get_voltage(AD_CH_PMU_VBAT) * 4);
    log_info("%s() PA3:%d\n", __func__, adc_get_value(adc_io2ch(IO_PORTA_03)));
    log_info("%s() PA3:%d mv\n", __func__, adc_get_voltage(adc_io2ch(IO_PORTA_03)));
    log_info("%s() PA4:%d\n", __func__, adc_get_value_blocking(adc_io2ch(IO_PORTA_04)));
    log_info("%s() PA4:%d mv\n", __func__, adc_get_voltage_blocking(adc_io2ch(IO_PORTA_04)));
    /* log_info("%s() DTEMP:%d\n", __func__, adc_get_voltage(AD_CH_PMU_VTEMP)); */
    /* log_info("%s() PA2:%d\n", __func__, adc_get_value(AD_CH_IO_PA2)); */
    /* log_info("%s() PA2:%dmv\n", __func__, adc_get_voltage_blocking(AD_CH_IO_PA2)); */
    /* log_info("%s() PA10:%dmv\n", __func__, adc_get_voltage_blocking(AD_CH_IO_PA10)); */
    /* log_info("%s() PA6:%dmv\n", __func__, adc_get_voltage_blocking(AD_CH_IO_PA6)); */
    /* log_info("%s() PB1:%d\n", __func__, adc_get_value(AD_CH_IO_PB1)); */
    /* log_info("%s() PB1:%dmv\n", __func__, adc_get_voltage(AD_CH_IO_PB1)); */
}
float adc_value_update(enum AD_CH ch, float adc_value_old, float adc_value_new) //vbat, vbg 值更新
{
    float ret_value;
#if AD_CH_IO_VBAT_PORT
    if (ch == AD_CH_IO_VBAT_PORT) {
        return ret_value = (adc_value_old * 0.8f + adc_value_new * 0.2f); //这里可以调用自己的滤波算法
    }
#endif
    return ret_value = (adc_value_old * 0.8f + adc_value_new * 0.2f) + 0.5;
}

void adc_api_init(void) //adc初始化
{
    _adc_pmu_vbg_enable();
    adc_close();
    adc_hw_init();
}

u32 adc_get_next_ch()    //获取采集队列中下一个通道的队列编号
{
    if (cur_ch == ADC_MAX_CH) {
        cur_ch = 0;
    }
    for (int i = cur_ch; i < ADC_MAX_CH; i++) {
        if (adc_queue[i].ch != -1) {
            if (adc_queue[i].sample_period) {
                if (time_before(adc_queue[i].jiffies, jiffies)) {
                    adc_queue[i].jiffies += adc_queue[i].sample_period;
                } else {
                    continue;
                }
            }

            return i;
        }
    }
    return ADC_MAX_CH;
}

u32 adc_set_sample_period(enum AD_CH ch, u32 ms) //设置一个指定通道的采样周期
{
    u32 i;
    for (i = 0; i < ADC_MAX_CH; i++) {
        if (adc_queue[i].ch == ch) {
            adc_queue[i].sample_period = ms / 10;
            adc_queue[i].jiffies = 0 + jiffies;
            break;
        }
    }
    return i;
}

void adc_update_vbg_value_restart(u8 cur_miovdd_level, u8 new_miovdd_level)
{
    log_info("cur = %d, new = %d\n", cur_miovdd_level, new_miovdd_level);
    u32 cur_miovdd_vol = 2100 + cur_miovdd_level * 100;
    u32 new_miovdd_vol = 2100 + new_miovdd_level * 100;
    adc_queue[0].v.value = adc_queue[0].v.value * cur_miovdd_vol / new_miovdd_vol;
}

u32 adc_get_vbg_voltage()
{
    if (ADC_VBG_DATA_WIDTH == 0) {
        return ADC_VBG_CENTER;
    }
    int data = efuse_get_gpadc_vbg_trim();
    int sign = (data >> ADC_VBG_DATA_WIDTH) & 0x01;
    data = data & ((1 << ADC_VBG_DATA_WIDTH) - 1);
    if (sign == 1) {
        return  ADC_VBG_CENTER - data * ADC_VBG_TRIM_STEP;
    } else {
        return  ADC_VBG_CENTER + data * ADC_VBG_TRIM_STEP;
    }
}
//adc_value-->voltage   用传入的 vbg 计算
u32 adc_value_to_voltage(u32 adc_vbg, u32 adc_value)
{
    u32 tmp = adc_get_vbg_voltage();
    if (adc_vbg == 0) {
        adc_vbg = 1;        //防止div0异常
    }
    u32 adc_res = adc_value * tmp / adc_vbg;
    return adc_res;
}

//adc_value-->voltage   用 vbg_value_array 数组均值计算
u32 adc_value_to_voltage_filter(u32 adc_value)
{
    u32 adc_vbg = adc_get_value(AD_CH_LDOREF);
    return adc_value_to_voltage(adc_vbg, adc_value);
}

u32 adc_get_value(enum AD_CH ch)   //获取一个指定通道的原始值，从队列中获取
{
    for (int i = 0; i < ADC_MAX_CH; i++) {
        if (adc_queue[i].ch == ch) {
            return adc_queue[i].v.value;
        }
    }
    return 1;
}

u32 adc_get_voltage(enum AD_CH ch) //获取一个指定通道的电压值，从队列中获取
{
    for (int i = 0; i < ADC_MAX_CH + ENABLE_PREEMPTIVE_MODE; i++) {
        if (ch == adc_queue[i].ch && adc_queue[i].adc_voltage_mode == 1) {
            return adc_queue[i].v.voltage;
        }
    }

    u32 adc_vbg = adc_get_value(AD_CH_LDOREF);
    u32 adc_res;
    if (ch == AD_CH_IOVDD) {
        adc_res = 1023;
    } else {
        adc_res = adc_get_value(ch);
    }
    return adc_value_to_voltage(adc_vbg, adc_res);
}

u32 adc_get_value_blocking(enum AD_CH ch)    //阻塞式采集一个指定通道的adc原始值
{
    adc_queue[ADC_MAX_CH].ch = ch;

    adc_wait_enter_idle();

    local_irq_disable();

    adc_sample(adc_queue[ADC_MAX_CH].ch, 0);

    adc_queue[ADC_MAX_CH].v.value = adc_wait_pnd();

    adc_queue[ADC_MAX_CH].ch = -1;

    adc_set_enter_idle();

    local_irq_enable();

    return adc_queue[ADC_MAX_CH].v.value;
}

u32 adc_get_value_blocking_filter(enum AD_CH ch, u32 sample_times)
{
    u32 ch_adc_value = 0;
    u32 ch_min_value = 0xffff;
    u32 ch_max_value = 0;

    if (sample_times <= 2) {
        sample_times = 3;
    }
    for (int i = 0; i < sample_times; i++) {
        if (ch == AD_CH_IOVDD) {
            break;
        }

        u32 adc_value = adc_get_value_blocking(ch);

        if (adc_value > ch_max_value) {
            ch_max_value = adc_value;
        }
        if (adc_value < ch_min_value) {
            ch_min_value = adc_value;
        }
        ch_adc_value += adc_value;
    }

    if (ch == AD_CH_IOVDD) {
        ch_adc_value = 1023;
    } else {
        ch_adc_value -= ch_max_value;
        ch_adc_value -= ch_min_value;

        ch_adc_value /= sample_times - 2;
    }

    return ch_adc_value;
}

u32 adc_get_voltage_blocking(enum AD_CH ch)  //阻塞式采集一个指定通道的电压值（经过均值滤波处理）
{
    u32 vbg_adc_value = 0;
    u32 vbg_min_value = 0xffff;
    u32 vbg_max_value = 0;

    u32 ch_adc_value = 0;
    u32 ch_min_value = 0xffff;
    u32 ch_max_value = 0;

    for (int i = 0; i < 12; i++) {

        u32 adc_value = adc_get_value_blocking(AD_CH_LDOREF);
        if (adc_value > vbg_max_value) {
            vbg_max_value = adc_value;
        }
        if (adc_value < vbg_min_value) {
            vbg_min_value = adc_value;
        }

        vbg_adc_value += adc_value;

        if (ch == AD_CH_IOVDD) {
            continue;
        }

        adc_value = adc_get_value_blocking(ch);

        if (adc_value > ch_max_value) {
            ch_max_value = adc_value;
        }
        if (adc_value < ch_min_value) {
            ch_min_value = adc_value;
        }
        ch_adc_value += adc_value;
    }

    vbg_adc_value -= vbg_max_value;
    vbg_adc_value -= vbg_min_value;

    vbg_adc_value /= 10;


    if (ch == AD_CH_IOVDD) {
        ch_adc_value = 1023;
    } else {
        ch_adc_value -= ch_max_value;
        ch_adc_value -= ch_min_value;

        ch_adc_value /= 10;
    }

    return adc_value_to_voltage(vbg_adc_value, ch_adc_value);
}

static u32 abs_value(u32 a, u32 b)
{
    if (a >= b) {
        return a - b;
    } else {
        return b - a;
    }
}
u32 adc_cpu_mode_process(u32 adc_value)    //adc_isr 中断中，cpu模式的公共处理函数
{
    if (adc_queue[cur_ch].adc_voltage_mode == 1) {
        u32 vbg_value = adc_queue[0].v.value;
        u32 voltage = adc_value_to_voltage(vbg_value, adc_value);
        adc_queue[cur_ch].v.voltage = adc_value_update(adc_queue[cur_ch].ch, adc_queue[cur_ch].v.voltage, voltage);
        /* log_info("%d ad[%x]: %d %d\n", cur_ch, adc_queue[cur_ch].ch, adc_value, adc_queue[cur_ch].v.voltage); */
    } else  if (adc_queue[cur_ch].ch == AD_CH_LDOREF) {
        adc_queue[0].v.value = adc_value_update(AD_CH_LDOREF, adc_queue[0].v.value, adc_value);
    } else {
        adc_queue[cur_ch].v.value = adc_value;
    }
    /* log_info("%d ad[%x]: %d %d", cur_ch, adc_queue[cur_ch].ch, adc_value, adc_queue[cur_ch].v.voltage); */
    if (adc_queue[ADC_MAX_CH].ch != -1) {
        adc_close();
        return 1;
    }
    return 0;
}

u32 adc_check_vbat_lowpower()
{
    return 0;
}

void adc_hw_uninit(void)
{
    local_irq_disable();
    memset(adc_queue, 0xff, sizeof(adc_queue));
    JL_ADC->CON = 0;
    local_irq_enable();
}


void adc_exit_power_down_update()
{
    _adc_pmu_vbg_enable();

}
