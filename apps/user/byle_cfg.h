  /*



  语音算法：


                                  */

  #define kwsA  0
  #define kwsB  1
  #define kwsB2  2
  #define kwsC 3
  #define kwsC1 4

  #define kwsD 5								 

  #define lib_irA  0
  #define lib_irB  1

  #define kwsB_AEC  2
  #define kwsB_ES  3
  #define kwsB_AEC_ES  4

  #define  KWSA_SDK_lib   0  //可以自由定制库
  #define  KWSA_light_libC1 1//固定库
  #define  KWSC_light_libC1 2
  #define  KWSC_light_libD1 3
  #define  KWSC_switch_libC1 5
  #define  KWSC_light_libE1 6
  #define  KWSA_switch_libS1 7
  #define  KWSA_light_libD1  8
  #define  KWSA_MIN_Q   9//定制库
  #define  KWSC1_light_libF1 10
  #define  KWSA_light_libK4 11
  #define  KWSA_ac_libK1 12
  #define  KWSA_ac_libK2 13     //BZ
  #define  KWSA_ac_libK3 14     //BZ
  #define KWSA_user_libB1   15  //BZ
  #define  KwsA_FAN_LIGHT_F6_180K 20
  #define  KwsA_FAN_LIGHT_F6_100K 21
  #define  kwsA_wy_light_libC2 22

  #define  KWSA_switch_libS2 23//17

  #define  KWSA_light_libC2 24//18

  #define  KWSA_switch_libS3 25//19

  #define  KWSA_light_libC3 26//21

  //#define US320B4_kwsA_F6_demo
  //#define US320A8_kwsA_libC1_WY_V5
  //#define US320A8_kwsA_libC1_WY_V7
  //#define US320A8_kwsC_libE1_WY_V5
  //#define US320A8_kwsC_libC1_RGB_V3
  //#define US320A8_kwsC_libC1_WY_V7
  //#define US320A8_kwsC_libC1_RGB_V4
  //#define US320A8_kwsC_libC1_RGB_V3
  //#define US320A8_kwsA_libC1_RGB_V3
  //#define  US320A8_kwsC_libF1_WY_V5
  //#define US320A8_kwsA_libK4_fan_lamp
  //#define US320_irA_kwsA_K1lib_ch_RGB_V3
  //#define US320_irB_kwsA_K1lib_ch_RGB_V3

  //#define US320A8_kwsA_libC1_AC_zero_port


  //#define  BY59_KWSB_AEC_V1
  //#define BYLE_KWS_TO_UART_DEMO//


  #define    KWSA_SDK_DEMO
  //#define    US320A8_kwsA_MINIQ

  //#define US320A8_kwsA_libC3_RGB_V3
  // #define US320A8_BYLE_KWSA_switch_libS3

  #define KWS_USE_ID   "userMjAyMzA0MTExNDE5NDMV2zv0e7KXOvY"//"方案需要修改为自己的ID,才可以跑， 对应方案商唯一ID"//




  #define  ENGINE_KWS_MODE_WAKEUP  0// 需要唤醒 ENGINE_KWS_MODE_WAKEUP  
  #define  ENGINE_KWS_MODE_CMD 1//不需要唤醒ENGINE_KWS_MODE_CMD
  #define   ENGINE_KWS_MODE_INVALID 2

  #define  Byle_Authorization_keyI  0X11 //
  #define  Byle_Authorization_keyH  0X10 //

  #undef  TCFG_UART_TX_PORT  	
  #define  BYLE_TIMER_APP
  #define  USER_MCU_TIMER   JL_TIMER2
  #define  USER_IRQ_TIME_IDX                    IRQ_TIME2_IDX

  #ifdef US320A8_BYLE_KWSA_switch_libS3
  #define Byle_Authorization_file  Byle_Authorization_keyH//I
  #define  Byle_Kws_Lib_Type  KWSA_switch_libS3//KWSA_switch_libS2//KWSA_switch_libS1 
  #define  KWS_TYPE  kwsA
  #define  US320A8_BYLE_KWS_SWITCH
  #endif

  #ifdef  US320A8_kwsA_libC3_RGB_V3
  #define Byle_Authorization_file  Byle_Authorization_keyH//I
  #ifndef Byle_Kws_Lib_Type
  #define  Byle_Kws_Lib_Type  KWSA_light_libC3//KWSA_light_libC2//KWSA_light_libC1 
  #define  KWS_TYPE  kwsA
  #endif
  #define US320A8_RGB_V3
  #define US320A_kws_led
  #endif

  #ifdef  US320_irB_kwsA_K1lib_ch_RGB_V3
  #define  IR_LIB_TYPE lib_irB
  #define  US320_irA_kwsA_K1lib_ch_RGB_V3

  #endif
  #ifdef  US320B4_kwsA_F6_demo
  #define BYLE_KWS_ASR
  #define  KWS_TYPE  kwsA
  #define  Byle_Kws_Lib_Type  KwsA_FAN_LIGHT_F6_100K 
  #define Byle_Authorization_file  Byle_Authorization_keyI
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_WAKEUP  
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #define flash_SIZE 0x40000
  #endif

  #ifdef  US320_irA_kwsA_K1lib_ch_RGB_V3
  #define  Byle_Kws_Lib_Type  KWSA_ac_libK1
  #define KWA_customizable_lib
  #define  KWS_TYPE  kwsA
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_WAKEUP// 	

  #define Byle_Authorization_file  Byle_Authorization_keyI//Byle_Authorization_keyI
  #define BYLE_KWS_ASR
  #define BYLE_kws_music_led
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #ifndef IR_LIB_TYPE
  #define  IR_LIB_TYPE lib_irA
  #endif
  #define flash_SIZE 0x80000  //ad128B4
  //#define DISABLE_MIC_BIAS
  #define COM_APP_TIMER_MAX  7
  //#undef TCFG_MIC_CAPLESS
  //#define TCFG_MIC_CAPLESS                 1 //免电容

  #define  IR_EMITTER_PORT  IO_PORTA_08
  #define  IR_EMITTER_PWM_TIMER   JL_TIMER0
  #define  IR_EMITTER_PWM_OUTPUT_CHANNEL   0
  #define  IR_EMITTER_PORT_USE_MIC_BIAS
  #define  IR_EMITTER_PWM_LOW_EN 1


  #define IR_EMITTER_TIMER  JL_TIMER1
  #define IR_EMITTER_IRQ_IDX  IRQ_TIME1_IDX

  //#define US320A8_WY_V5
  #define US320A_kws_led
  #define LED_W_PORT IO_PORTA_02

  //#define US320A8_RGB_V3
  //#define US320A_kws_led
  #endif

  #ifdef US320A8_kwsA_libC1_AC_zero_port
  #define  Byle_Kws_Lib_Type  KWSA_light_libC1 
  #define  KWS_TYPE  kwsA
  #define LED_W_PORT IO_PORTA_02
  #define AC_zero_port  IO_PORTA_14
  #define Byle_Authorization_file  Byle_Authorization_keyI//Byle_Authorization_keyI
  #define BYLE_KWS_ASR
  #define BYLE_kws_music_led

  #ifndef KWS_CMD_MODE
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD// 	
  #endif
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #endif




  #ifdef US320A8_kwsA_libK4_fan_lamp
  #define BYLE_UATR_ENABLE
  #define BYLE_UATR_TX_PIN   IO_PORTA_05
  #define BYLE_UATR_RX_PIN   IO_PORTA_14
  #define BYLE_UART_BAUD 1200
  #define BYLE_UATR_FRAME_LEN 16
  #define BYLE_UATR_TIME_OUT  50 //ms
  #define  Byle_Kws_Lib_Type   KWSA_light_libK4 
  #define KWA_customizable_lib
  #define Byle_Authorization_file  Byle_Authorization_keyI
  #define  KWS_TYPE  kwsA
  #define BYLE_KWS_ASR
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_WAKEUP  
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置

  #endif

  #ifdef US320A8_kwsA_MINIQ
  #define  MOTOR_A_PORT   IO_PORTA_01
  #define  MOTOR_B_PORT   IO_PORTA_02
  #define  MOTOR_A_H()  { gpio_set_die(MOTOR_A_PORT,0);gpio_set_direction(MOTOR_A_PORT,0);gpio_direction_output(MOTOR_A_PORT,1); }
  #define  MOTOR_A_L()  { gpio_set_die(MOTOR_A_PORT,0);gpio_set_direction(MOTOR_A_PORT,0);gpio_direction_output(MOTOR_A_PORT,0); }
  #define  MOTOR_B_H()  { gpio_set_die(MOTOR_B_PORT,0);gpio_set_direction(MOTOR_B_PORT,0);gpio_direction_output(MOTOR_B_PORT,1); }
  #define  MOTOR_B_L()  { gpio_set_die(MOTOR_B_PORT,0);gpio_set_direction(MOTOR_B_PORT,0);gpio_direction_output(MOTOR_B_PORT,0); }
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD  
  #define  MOTOR_EN   IO_PORTA_01
  #define BYLE_KWS_ASR
  #define  KWS_TYPE  kwsA
  #define  Byle_Kws_Lib_Type   KWSA_MIN_Q 
  #define Byle_Authorization_file  Byle_Authorization_keyI
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #define KWA_customizable_lib

  #endif


  #ifdef  KWSA_SDK_DEMO
  #define BYLE_KWS_ASR
  #define  KWS_TYPE  kwsA
  //#define  KWS_TONE_EN  1
  #define  Byle_Kws_Lib_Type   KWSA_SDK_lib 
  #define KWA_customizable_lib
  #define Byle_Authorization_file  Byle_Authorization_keyH//I
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD  
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #define flash_SIZE 0x80000
  #endif


  #ifdef  BY59_KWSB_AEC_V1
    
  //#define AEC_UATR_ENABLE

  #ifdef AEC_UATR_ENABLE
  #define BYLE_UART_AEC
  #define AEC_UATR_TX_PIN   IO_PORTA_05//
  #define AEC_UATR_RX_PIN   IO_PORTA_14//
  #define AEC_UART_BAUD 2000000
  #define  AEC_UATR_TX_FRAME_LEN 72//260//72
  #define  AEC_UATR_RX_FRAME_LEN 268// 140
  #define  AEC_UART_DATA_LEN    1024// 512
  #define  AEC_UART_RX_CBUF_LEN      1024
  #define AEC_UATR_TIME_OUT  5//ms
  #define BYLE_LIB_CMD_UART_EN

  #endif
  #define MIC_AEC_TO_DAC

  #define MIC_ADC_ENABLE  
  #define EN_MIC_BIAS_IO_PORT IO_PORTA_01

  #ifdef MIC_AEC_TO_DAC
  #define DAC_SR 16000
  #undef  TCFG_DAC_ENABLE 
  #define TCFG_DAC_ENABLE 1
  #define TCFG_MIC_BIAS_ENANLE 0
  #endif

  #define Byle_Authorization_file    Byle_Authorization_keyI	
  #define KWS_AEC_EN     kwsB_AEC_ES
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//IO_PORTB_07//IO_PORT_DM//串口发送脚配置
  #define  MIC_GAIN 8
  #endif


  #ifdef  BYLE_KWS_TO_UART_DEMO
  #define BYLE_UATR_ENABLE
  #define BYLE_UATR_TX_PIN   IO_PORTA_05
  #define BYLE_UATR_RX_PIN   IO_PORTA_14
  #define BYLE_UART_BAUD 1200
  #define BYLE_UATR_FRAME_LEN 16
  #define BYLE_UATR_TIME_OUT  50 //ms
  #define  Byle_Kws_Lib_Type   KWSA_light_libC1 
  #define Byle_Authorization_file  Byle_Authorization_keyI
  #define BYLE_KWS_ASR
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD  
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #endif



  #ifdef US320A8_BYLE_KWSA_switch_libS1
  #define  Byle_Kws_Lib_Type  KWSA_switch_libS1 
  #define  KWS_TYPE  kwsA
  #define  US320A8_BYLE_KWS_SWITCH
  #endif

  #ifdef US320A8_BYLE_KWSC_switch_libC1
  #define  Byle_Kws_Lib_Type  KWSC_switch_libC1 
  #define  KWS_TYPE  kwsC
  #define  US320A8_BYLE_KWS_SWITCH
  #endif




  #ifdef US320A8_BYLE_KWS_SWITCH
  #define  BYLE_KWS_SWITCH
  #ifndef BYLE_KWS_SWITCH_IO
  #define  BYLE_KWS_SWITCH_IO IO_PORTA_00
  #endif
  #ifndef BYLE_KWS_SWITCH_IO1
  #define  BYLE_KWS_SWITCH_IO1 IO_PORTA_14
  #endif
  #define  Byle_Authorization_file  Byle_Authorization_keyI//Byle_Authorization_keyI
  #define  BYLE_KWS_ASR
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD// 
  #define  TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #endif


  #ifdef US320A8_kwsA_libC1_WY_V7     
  #define  Byle_Kws_Lib_Type  KWSA_light_libC1
  #define LIGHT_TIMER_EN    
  #define US320A_kws_led
  #define  KWS_TYPE  kwsA
  #define  US320A8_WY_V7
  #define USE_TIMER_FOR_PWM

  #endif



  #ifdef US320A8_kwsC_libD1_WY_V7
  #define  Byle_Kws_Lib_Type  KWSC_light_libD1    
  #define US320A_kws_led
  #define  KWS_TYPE  kwsC
  #define  US320A8_WY_V7
  #define USE_TIMER_FOR_PWM

  #endif


  #ifdef US320A8_kwsC_libC1_WY_V7
  #define  Byle_Kws_Lib_Type  KWSC_light_libC1 
  #define US320A8_WY_V7
  #define US320A_kws_led
  #define  KWS_TYPE  kwsC
  #endif




  #ifdef US320A8_kwsA_libC1_WY_V5
  #define  Byle_Kws_Lib_Type  KWSA_light_libC1 
  #define  KWS_TYPE  kwsA
  #define US320A8_WY_V5
  #define US320A_kws_led
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置
  #endif

  #ifdef US320A8_kwsC_libF1_WY_V5
  #define  Byle_Kws_Lib_Type  KWSC1_light_libF1 
  #define US320A8_WY_V5
  #define US320A_kws_led
  #define  KWS_TYPE  kwsC1
  #undef  TCFG_UART_TX_PORT 
  #define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置

  #endif

  #ifdef US320A8_kwsC_libE1_WY_V5
  #define  Byle_Kws_Lib_Type  KWSC_light_libE1 
  #define US320A8_WY_V5
  #define US320A_kws_led
  #define  KWS_TYPE  kwsC
  //#undef  TCFG_UART_TX_PORT 
  //#define TCFG_UART_TX_PORT  				IO_PORT_DP//串口发送脚配置

  #endif

  #ifdef  US320A8_kwsA_libC1_RGB_V3_abc
  #define  US320A8_kwsA_libC1_RGB_V3
  #define  Byle_Kws_Lib_Type  KWSA_light_libD1
  #define  KWS_TYPE  kwsA
  #endif

  #ifdef  US320A8_kwsA_libC1_RGB_V3
  #ifndef Byle_Kws_Lib_Type
  #define  Byle_Kws_Lib_Type  KWSA_light_libC1 
  #define  KWS_TYPE  kwsA
  #endif
  #define US320A8_RGB_V3
  #define US320A_kws_led
  #endif


  #ifdef US320A_kws_led
  #ifndef Byle_Authorization_file
  #define Byle_Authorization_file  Byle_Authorization_keyI//Byle_Authorization_keyI
  #endif
  #define BYLE_KWS_ASR
  #ifndef KWS_CMD_MODE
  #define  KWS_CMD_MODE  ENGINE_KWS_MODE_CMD// 	
  #endif
  #define MUSIC_LED_CODE_LESS 
  #include "music_led_board_cfg.h"

  //#define EN_MIC_BIAS_IO_PORT IO_PORTA_08
  #endif





  #ifdef EN_MIC_BIAS_IO_PORT
  #define MIC_BIAS_POWER(x) {gpio_set_die(EN_MIC_BIAS_IO_PORT,0);gpio_set_direction(EN_MIC_BIAS_IO_PORT,0); gpio_set_pull_up(EN_MIC_BIAS_IO_PORT,0);gpio_set_pull_down(EN_MIC_BIAS_IO_PORT,0);gpio_write(EN_MIC_BIAS_IO_PORT,x);}
  #endif

  #ifdef BYLE_KWS_ASR
  #define MIC_ADC_ENABLE  
  #define  DISABLE_DAC_OUT
  #ifndef KWS_TONE_EN
  #define KWS_TONE_EN 0
  #endif
  #endif




  #define USER_SW_IRQ_IDX  IRQ_SOFT3_IDX
  #ifndef COM_APP_TIMER_MAX
  #define COM_APP_TIMER_MAX  5
  #endif
  #ifndef Byle_Authorization_file
  //#define Byle_Authorization_file  Byle_Authorization_keyI
  #define Byle_Authorization_file  Byle_Authorization_keyH
  #endif
  #ifndef KWS_CMD_MODE
  #define KWS_CMD_MODE  ENGINE_KWS_MODE_WAKEUP
  #endif

  #undef ICACHE_RAM_TO_RAM_ENABLE
  #define ICACHE_RAM_TO_RAM_ENABLE			1//ICACHE RAM用作普通RAM使能位



  #ifndef TCFG_MIC_BIAS_ENANLE
  #define TCFG_MIC_BIAS_ENANLE 0
  #endif



  #if TCFG_MIC_BIAS_ENANLE
  #undef AMM_RS_INSIDE_ENABLE
  #define AMM_RS_INSIDE_ENABLE               1

  #undef AMM_RS_OUTSIDE_ENABLE
  #define AMM_RS_OUTSIDE_ENABLE            0
  #endif



  #ifndef TCFG_UART_TX_PORT
  #define  TCFG_UART_TX_PORT NO_CONFIG_PORT
  #endif


  #if   KWS_TONE_EN
  #undef  TCFG_TONE_PLAY_ENABLE 
  #define TCFG_TONE_PLAY_ENABLE               1//ENABLE                 //是否使能提示音播放        CQL_2101			//BZ
  #define TCFG_DAC_ENABLE                     TCFG_TONE_PLAY_ENABLE   //是否需要DAC播放
  #define TCFG_DAC_ANA_VOL                    AUDAC_0db               //DAC模拟增益配置
  #if TCFG_TONE_PLAY_ENABLE
  #define TCFG_DEC_WTG_ENABLE                 1//ENABLE                  //使能wtg解码
  #endif

  #define USER_TONE_FILE  dir_wtg//dir_wtg

  #endif


  #ifndef KWS_TYPE
  #define  KWS_TYPE  kwsA
  #endif


  #ifndef MIC_GAIN
  #define MIC_GAIN             8//db
  #endif




  #ifndef  PWM_HIGH_ON
  #define  PWM_HIGH_ON 0  // 共阳极灯带: 0=反相输出
  #endif

  #define LOBYTE(w)           ((u8)(w))
  #define HIBYTE(w)           ((u8)(((u16)(w) >> 8) & 0xFF))
  #define BYTE0(val)			(((u8*)(&val))[0])
  #define BYTE1(val)			(((u8*)(&val))[1])
  #define BYTE2(val)			(((u8*)(&val))[2])
  #define BYTE3(val)			(((u8*)(&val))[3])


  #ifndef MIC_NO_AUDIO_VAULE
  #if MIC_GAIN>=8
  #define  MIC_NO_AUDIO_VAULE 1500
  #else
  #define  MIC_NO_AUDIO_VAULE 1200
  #endif
  
  #endif

  #ifdef MUTE_PORT      

  #ifdef ENABLE_HIGH_MUTE     
  #define PA_UNMUTE()    { gpio_set_die(MUTE_PORT,0);gpio_set_direction(MUTE_PORT,0); gpio_write(MUTE_PORT,0);}
    #define PA_MUTE()    { gpio_set_die(MUTE_PORT,1);gpio_set_direction(MUTE_PORT,1);  gpio_set_pull_up(MUTE_PORT, 0);gpio_set_pull_down(MUTE_PORT, 0);}
  #else
  #define PA_MUTE()    { gpio_set_die(MUTE_PORT,0);gpio_set_direction(MUTE_PORT,0);gpio_write(MUTE_PORT,0);}
    #define PA_UNMUTE()    { gpio_set_die(MUTE_PORT,0);gpio_set_direction(MUTE_PORT,0); gpio_write(MUTE_PORT,1);}
  #endif
  #else

  #define PA_UNMUTE() 
  #define PA_MUTE()	

  #endif
