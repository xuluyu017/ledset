#ifndef __SYSCFG_EFUSE_H__
#define __SYSCFG_EFUSE_H__

#include "typedef.h"

//efuse物理上未烧写时为0, 烧写后为1，注意使用默认值
#define SYS_EFUSE_SIZE      128 //1Kbit
#define EFUSE_DEFAULT       0x00

//eq参数转换系数
#define gain_fix    (24)
#define gain_fix2   (10)
#define q_fix       (100)
#define freq_fix    (1)

//for pll_auto_trim
enum pll_trim_mode {
    PLL_AUTO_TRIM_WITH_EFUSE_NR_98_PERCENT,
    PLL_AUTO_TRIM_WITH_EFUSE_NR,
    PLL_MANUAL_TRIM_WITH_TIMER_TRIM0,
    PLL_MANUAL_TRIM_WITH_TIMER_TRIM1,
};

//for hid_report_type
enum {
    HID_REPORT_TYPE_WINDOWS = 0,    //关闭主机类型识别时使用hid描述符类型为WINDOWS
    HID_REPORT_TYPE_ANDROID,        //关闭主机类型识别时使用hid描述符类型为Android
    HID_REPORT_TYPE_IOS,            //关闭主机类型识别时使用hid描述符类型为IOS
};

/*校准数据结构*/
struct efuse_trim {
    u32 rdac_trim;                  //rdac_trim校准左右声道电压
} _GNU_PACKED_;

/*EQ参数结构*/
struct efuse_eq {
    u16 eq_seg0_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg0_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg0_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg0_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg1_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg1_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg1_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg1_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg2_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg2_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg2_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg2_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg3_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg3_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg3_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg3_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg4_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg4_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg4_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg4_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg5_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg5_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg5_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg5_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg6_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg6_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg6_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg6_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg7_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg7_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg7_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg7_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg8_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg8_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg8_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg8_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 eq_seg9_freq        : 15;   //(20~22000),中心截止频率工具界面值（20~22000）Hz,保存值（20~22000)
    u16 eq_seg9_gain        : 9;    //(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_seg9_q           : 12;   //(0~3000),Q值工具界面值(0.0~30),保存值（0~3000）
    u16 eq_seg9_iir_type    : 3;    //EQ_IIR_TYPE(0~4),滤波器类型工具界面值(0~4),保存值（0~4）

    u16 global_gain         : 9;    //eq总增益,(0~480),增益工具界面值（-24.0~24.0）db,保存值（0~480）
    u16 eq_enable           : 1;    //eq总使能 0:disable 1:enable (default=0)
    //(15 + 3 + 12 + 9) * 10 + 9 + 1 = 400, 400 / 8 = 50byte
} _GNU_PACKED_;

/*AUDIO参数结构*/
// 回声消除配置 - mic配置 - dac配置
struct efuse_audio {
    u16 wn_gain;                    //舒适噪声增益 16384对应0db
    s8  aec_refengthr;              //进入回音消除参考值, default: -70.0f(-90 ~ -60 dB)
    s8  es_aggress_factor;          //回音前级动态压制(nlp_enable使能后有效),越小越强,default: -1.0f(-1 ~ -5)
    u8  es_min_suppress;	        //回音后级静态压制(nlp_enable使能后有效),越大越强,default: 6.f(0 ~ 10)
    u8  aec_simplex_tcnt;           //回声消除单工工作时间(等待回声消除算法收敛时间) time= (128 / 16k) * aec_simplex_tcnt * 4(S)
    u8  mic_digital_gain;           //mic的数字增益幅值 0~255 -> 0.0 ~ 25.5, dB和幅值的转换 dB = 20log10(Mag) Mag = 10^(dB/20)

    u8  aec_enable          : 1;    //回声消除使能
    u8  wn_en               : 1;    //舒适噪声使能(aec_enable=1 & nlp_enable=0 时有效)
    u8  nlp_enable          : 1;    //回声压制使能(回声消除使能时有效)
    u8  fade_enable         : 1;    //音量调节淡入淡出总使能
    u8  mic_bias_rsel       : 4;    //MIC偏置电阻选择(0-15)

    u8  fade_step           : 4;    //淡入淡出步进,默认值(1)
    u8  fade_slow           : 4;    //淡入淡出分频,默认值(0)

    u8  mic_ldo_vs          : 3;    //000:2.3v 001:2.5v 010:2.7v 011:2.9v 100:3.0v 101:3.1v 110:3.2v 111:3.3v
    u8  mic_gain            : 3;    //MIC增益选择(0~4) 000:0dB, 001:3dB, 010:9dB, 011:15dB, 1xx:21dB
    u8  mic_input_mode      : 2;    //00:单端用PA7 01:单端用PA6 10:差分PA7_PA6

    u8  vcm_capless         : 1;    //VCM省电容配置 1:省电容,0:有电容
    u8  dac_ana_vol         : 3;    //111:0db 110:-2db 101:-4db 100:-6db 011:-8db 010:-10db 001:-12db 000:-14db
    u8  vcm_voltage         : 2;    //00:1.2v 01:1.3v 10:1.4v 11:1.5v
    u8  reserved            : 2;    //预留对齐
} _GNU_PACKED_;

/*系统参数结构*/
struct efuse_system {
    u8  boot_mode           : 1;    //0: 从flash启动, 1:从rom启动
    u8  pll192m_en          : 1;    //PLL192使能, 0:160M 1:192M
    u8  dvdd_offset         : 2;    //dvdd档位offset [160M]:00->1.11v, 01->1.14v, 10->1.17v; [192M]:00->1.23v 01->1.26v 10->1.29v;
    u8  pll_auto_trim       : 2;    //PLL校准模式
    u8  reserved0           : 2;    //预留给启动配置

    u8  earphone_type       : 2;    //00:4线耳机, 01:5线耳机, 10:四线转接头(美标)
    u8  key_enable          : 1;    //按键使能
    u8  host_type_check_en  : 1;    //主机类型检测使能(同时也是双击使能开关)
    u8  eq_debug_en         : 1;    //eq调试使能
    u8  auto_mute_en        : 1;    //自动mute使能
    u8  reserved1           : 2;    //预留对齐

    u8  digital_vol         : 5;    //数字音量初始值
    u8  reserved2           : 3;    //预留对齐

    u8  up_key_res          : 4;    //+按键电阻 res = (up_key_res + 1) * 50;
    u8  down_key_res        : 4;    //-按键电阻 res = (down_key_res + 1) * 50;

    u8  auto_mute_energy;           //自动mute的能量阈值(1~255)
    u8  auto_mute_time;             //自动mute的检测时间(1~255)*20ms
    u8  auto_unmute_energy;         //自动解mute的能量阈值(1~255)
    u8  auto_unmute_time;           //自动解mute的检测时间1~255 ms
} _GNU_PACKED_;

/*USB参数结构*/
struct efuse_desc_cfg {
    u8 device_class_config  : 4;    //每一个Bit开启一个class, BIT(0):spk_class BIT(1):mic_class BIT(2):hid_class BIT(3):cdc_class
    u8 bcdUSB               : 1;    //Usb版本 0:1.1, 1:2.0
    u8 bcdDevice            : 1;    //Device版本 0:1.1, 1:2.0
    u8 bmAttributesRemoteWakup: 1;  //使能RemoteWakup 0:关闭, 1:使能
    u8 bmAttributesSelfPowered: 1;	//供电方式 0:自供电, 1:总线供电

    u16 vid;                        //usb_vid
    u16 pid;                        //usb_pid
    u8 iManufacturer        : 4;    //厂商字符串描述
    u8 iSerialNumber        : 4;    //序列号字符串描述

    u8 spk_control          : 4;    //Bit0:mute, Bit1:volume
    u8 mic_control          : 4;    //Bit0:mute, Bit1:volume

    u8 spk_samp;                    //spk采样率 每一个Bit使能一种采样率 Bit7~0:96000, 64000, 48000,	44100, 32000, 24000, 16000, 8000
    u8 mic_samp;	                //mic采样率 每一个Bit使能一种采样率 Bit7~0:48000, 44100, 32000, 24000, 22050, 16000, 11025, 8000

    u8 spk_terminal_type    : 4;    //0x0~0x7:0x0300~0x0307, 0x08:0x0100, 0x09:0x0101, 0x0a:0x01ff
    u8 mic_terminal_type    : 4;    //0x0~0x7:0x0200~0x0207, 0x08:0x0100, 0x09:0x0101, 0x0a:0x01ff

    u8 spk_ch               : 1;    //spk通道 0:单通道, 1:双通道
    u8 mic_ch               : 1;    //mic通道 0:单通道, 1:双通道
    u8 spk_res              : 1;    //spk位宽 0:16Bit, 1:24Bit
    u8 mic_res              : 1;    //mic位宽 0:16Bit, 1:24Bit
    u8 bMaxPower            : 2;    //0b00:50mA, 0b01:100mA, 0b10:200mA, 0b11:400mA
    u8 mic_feature_desc     : 1;    //mic_feature_desc 使能
    u8 fusb_pll_trim_en     : 1;    //full_usb_pll_trim 使能位

    u8 hid_report_type      : 2;    //host_type_check_en=0时有效，写0:windows，写1:安卓，写2:ios
    u8 spk_as_cfg           : 1;    //spk_audio_streaming配置 0:单配置  1:双配置,用来支持普通电话
    u8 reserved             : 5;    //预留对齐

    u8 iProduct[17];		        //产品字符串描述
} _GNU_PACKED_;

/* --------------------------------------------------------------------------*/
/**
 * @brief EFUSE结构体
 */
/* ----------------------------------------------------------------------------*/
struct efuse_config {
    struct efuse_trim trim_cfg;     //电压校准
    struct efuse_eq eq_tab;         //eq
    struct efuse_audio audio_cfg;   //audio
    struct efuse_system sys_cfg;    //system
    struct efuse_desc_cfg usb_cfg;  //usb
} _GNU_PACKED_;

extern u32 g_efuse_config[SYS_EFUSE_SIZE / 4];
void efuse_config_init(void);

//trim
u32 efuse_get_rdac_trim(void);

//eq
u8 efuse_get_eq_enable(void);
struct efuse_eq *efuse_get_eq_table(void);

//usb
struct efuse_desc_cfg *usb_get_efuse_desc_cfg(void);
void efuse_set_usb_pid_vid(u16 pid, u16 vid);
u8 efuse_get_spk_res(void);
u8 efuse_get_mic_res(void);
u8 efuse_get_spk_ch(void);
u8 efuse_get_mic_ch(void);
u8 efuse_get_hid_report_type(void);

//audio
u8 efuse_get_vcm_capless(void);
u8 efuse_get_dac_ana_vol(void);
u8 efuse_get_fade_enable(void);
u8 efuse_get_fade_step(void);
u8 efuse_get_fade_slow(void);
u8 efuse_get_vcm_voltage(void);
u8 efuse_get_aec_enable(void);
u32 efuse_get_aec_simplex_tcnt(void);
s8 efuse_get_aec_refengthr(void);
u8 efuse_get_wn_en(void);
u16 efuse_get_wn_gain(void);
u8 efuse_get_nlp_enable(void);
s8 efuse_get_es_aggress_factor(void);
u8 efuse_get_es_min_suppress(void);
u8 efuse_get_mic_ldo_vs(void);
u8 efuse_get_mic_gain(void);
float efuse_get_mic_digital_gain(void);
u8 efuse_get_mic_bias_rsel(void);
u8 efuse_get_mic_input_mode(void);

//system
u8 efuse_get_boot_mode(void);
u8 efuse_get_pll192m_en(void);
u8 efuse_get_dvdd_offset(void);
u8 efuse_get_pll_auto_trim(void);
u8 efuse_get_earphone_type(void);
u8 efuse_get_key_enable(void);
u8 efuse_get_key_double_click_en(void);
u8 efuse_get_host_type_check_en(void);
u8 efuse_get_eq_debug_en(void);
u8 efuse_get_auto_mute_en(void);
u8 efuse_get_auto_mute_energy(void);
u32 efuse_get_auto_mute_time(void);
u8 efuse_get_auto_unmute_energy(void);
u32 efuse_get_auto_unmute_time(void);
u8 efuse_get_digital_vol(void);
u16 efuse_get_up_key_res(void);
u16 efuse_get_down_key_res(void);

#endif

