#include "includes.h"
#include "app_config.h"
#include "audio_adc.h"
//中断优先级
//系统使用到的
const int IRQ_AUDIO_IP   = 5;
//系统还未使用到的
const int IRQ_IRTMR_IP   = 6;
const int IRQ_DECODER_IP = 2;
const int IRQ_WFILE_IP   = 1;
const int IRQ_ADC_IP     = 1;
const int IRQ_ENCODER_IP = 0;
const int IRQ_TICKTMR_IP = 4;
const int IRQ_USB_IP	 = 3;
const int IRQ_SD_IP		 = 3;
const int IRQ_PPM_IP     = 0;
const int IRQ_PPS_IP     = 0;
const int IRQ_STREAM_IP  = 4;
const int IRQ_AEC_IP	 = 0;
const int IRQ_UART0_IP   = 0;
const int IRQ_UART1_IP   = 0;
const int IRQ_ALINK0_IP  = 3;

//内存管理malloc内部断言
const char MM_ASSERT    = TRUE;

const u8 config_use_flash_system = 1;
const u8 config_spi_code_user_cache = 1;//1:sfc放code区 0:sfc放ram

//内核异常打印
const u8 config_asser = TCFG_DEBUG_ENABLE;
const u8 config_exception_enable = 1;
const u8 config_exception_unit_lite_lite_enable  = 1;
const u8 config_exception_unit_lite_enable       = 1;
const u8 config_exception_dev_lite_enable        = 1;

/*************audio dac analog config************************************/
const bool config_vcm_cap_addon = 1;// 0 :vcm不会外挂电容；1：vcm会外挂电容；

/*************audio adc analog config************************************/
u16 const mic_bias_rs =  TCFG_MIC_BIAS_RS_SEL;
u16 const mic_gain = TCFG_MIC_GAIN;
u16 const micldo_level = TCFG_MICLDO_LEVEL;
u16 const mic_input_mode = TCFG_MIC_INPUT_MODE;
float const mic_dig_gain = TCFG_MIC_DIG_GAIN;
/****************audio config end************************************************/

const u8 config_have_dac = TCFG_DAC_ENABLE;

const u8 have_tick_timer = 1;


/**
 * @brief Bluetooth Controller Log
 */
/*-----------------------------------------------------------*/
#if TCFG_DEBUG_ENABLE
const char libs_debug AT(.LOG_TAG_CONST) = TRUE; //打印总开关
#else
const char libs_debug AT(.LOG_TAG_CONST) = FALSE; //打印总开关
#endif

#define  CONFIG_DEBUG_LIBS(X)   (X & libs_debug)

const char log_tag_const_i_MIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_MIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_MIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_MAIN AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_MAIN AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_MAIN AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_PP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_PP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_PP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_KEYM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_KEYM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_KEYM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_MUGRD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_MUGRD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_MUGRD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_PWRA AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_PWRA AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_PWRA AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_LP_TIMER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_LP_TIMER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_LP_TIMER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);


const char log_tag_const_i_P33 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_P33 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_P33 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);


const char log_tag_const_i_LRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_LRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_LRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_PMU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_PMU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_PMU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_WKUP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_WKUP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_WKUP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

/* const char log_tag_const_i_SOFI AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */
/* const char log_tag_const_d_SOFI AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */
/* const char log_tag_const_e_SOFI AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */


/* const char log_tag_const_i_UTD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1); */
/* const char log_tag_const_d_UTD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */
/* const char log_tag_const_e_UTD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */
/* const char log_tag_const_c_UTD AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1); */


/* const char log_tag_const_i_LBUF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1); */
/* const char log_tag_const_d_LBUF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */
/* const char log_tag_const_e_LBUF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0); */

const char log_tag_const_i_FAT AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_FAT AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_FAT AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_c_FAT AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_NORM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_NORM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_e_NORM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_NORM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);


const char log_tag_const_i_DEBUG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_DEBUG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_e_DEBUG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_DEBUG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_FLASH AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_FLASH AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_FLASH AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_FLASH AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_SPI1 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_SPI1 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_SPI1 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_SPI1 AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_SPI1_TEST AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_SPI1_TEST AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_SPI1_TEST AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_EEPROM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_EEPROM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_EEPROM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_EEPROM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_IIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_IIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_IIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_IIC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_USB AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_USB AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_USB AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_USB AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);


const char log_tag_const_i_HEAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_HEAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_HEAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_c_HEAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_VM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_VM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_VM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_VM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_CPU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_CPU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_CPU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_c_CPU AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);


const char log_tag_const_i_OFF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_d_OFF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_OFF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_c_OFF AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

const char log_tag_const_i_CHARGE AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_CHARGE AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_e_CHARGE AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_CHARGE AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_EARDET AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_EARDET AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_EARDET AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_EARDET AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);

const char log_tag_const_i_IAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_d_IAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);
const char log_tag_const_e_IAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(1);
const char log_tag_const_c_IAP AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIBS(0);

