#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED  (-1.47)// (-1.47)  //  (-0.47) 	// (-1.47) // (-1.83)				//BZ		整体提高灵敏度-2           还原 默认 (-1.47)
#endif
#define KWS_SLEEP_SCORE_THRED    (-0.6)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED  (-0.81)// (-2.81)	//   (-1.81)	//(-0.81)	// (-1.83)			//BZ	整体提高灵敏度-2           还原 默认 (-1.47)
#endif
#define KWS_HASHTABLE_SIZE        (52)

/*---MIC config---*/
#define ACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR
#define MIC_ARRY_NUM          1

#define LOCAL_TONE_MAX_VOLUME      "101"
#define LOCAL_TONE_MIN_VOLUME      "102"
#define DEFAULT_PCM_WAKEUP        "103"
#define DEFAULT_PCM_ASR_TIMEOUT   "501"
#define DEFAULT_PCM_SLEEP         "106"
#define DEFAULT_PCM_WELCOME       " "

typedef struct {
  unsigned int  key_word_hash_code; /* 存放识别词汇对应的hashcode */
  unsigned char nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char          *hash_collision_orginal_str; /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */
} uni_nlu_content_mapping_t;




#define NLU_CONTENT_SIZE 28
#define NLU_CONTENT_MAP_SIZE 32

enum {
	eCMD_wakeup_uni,
	eCMD_default_open,
	eCMD_open0,
	eCMD_open2,
	eCMD_default_close,
	eCMD_close0,
	eCMD_close2,
	eCMD_coolmode,
	eCMD_warmmode,
	eCMD_sunmode,
	eCMD_all_on,
	eCMD_nightlight,
	eCMD_brightthelight,
	eCMD_brightthelight1,
	eCMD_dimthelight,
	eCMD_dimthelight1,
	eCMD_MaximumBright,
	eCMD_MinBright,
	eCMD_MinimumBrightn,
	eCMD_changecolor,
	eCMD_opennight,
	eCMD_closenight,
    eCMD_sleep10min,
	eCMD_sleep30min,
	eCMD_cancelsleepoff,
	eCMD_open3,
	eCMD_close3,
	eCMD_changecolor1,

	eCMD_cmd_no1,
	eCMD_setlight,
    eCMD_setcolor,
  eCMD_sleep30s,
};
#define NLU_CONTENT_SIZE 28
#define NLU_CONTENT_MAP_SIZE 32



extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];




#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0
#define USER_open1  eCMD_open2

#define LIGHT_TIMER_EN

#define USER_WY_COLOR_REPEAT


//#define  EN_SCORE_THRED_FOR_EVERY_CMD




/*

小艾小艾
小智小智
小美小美
小杜小杜
小亿小亿

命令词：（灵敏度：中）
cmd01=开灯
cmd02=打开灯
cmd03=打开灯光
cmd04=关灯
cmd05=关闭灯
cmd06=关闭灯光
cmd07=白光
cmd08=黄光
cmd09=中性光
cmd10=全亮
cmd11=小夜灯
cmd12=亮一点
cmd13=调亮一点
cmd14=暗一点
cmd15=调暗一点
cmd16=最大亮度
cmd17=中等亮度
cmd18=最小亮度
cmd19=变颜色
cmd20=打开辅助光
cmd21=关闭辅助光
cmd22=定时十分钟
cmd23=定时半小时
cmd24=取消定时
cmd25=打开电灯
cmd26=关闭电灯
cmd27=切换颜色



*/

	static const u16 Byle_kws_msg_table[]=
	{
      KEY_NULL,//  eCMD_wakeup_uni,
      MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
      MSG_LED_OFF,// eCMD_default_close,
      MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//  eCMD_changecolor,
      MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
      MSG_LED_OFF,//  eCMD_close0,
      KEY_NULL,// eCMD_cmd_no1,
      MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
      MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
	  MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//	eCMD_changecolor1,
	  MSG_LED_USB_NIGHT_ON,//  eCMD_open2,
	  MSG_LED_OFF,//  eCMD_close2,
	  MSG_LED_OFF,//  eCMD_close3,	  
	  KEY_NULL,// eCMD_setlight,
      #ifdef LIGHT_TIMER_EN 
      MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
      MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
      MSG_LED_SLEEP_1HOUR_OFF,//eCMD_sleep1hour,
      MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
      #else
      KEY_NULL,//eCMD_sleep10min,
      KEY_NULL,//eCMD_sleep30min,
      KEY_NULL,//eCMD_sleep1hour,
      KEY_NULL,//eCMD_cancelsleepoff,
      #endif
	  MSG_LED_VOICE_MAX_BRIGHT,//  eCMD_MaximumBright,
      MSG_LED_VOICE_MIN_BRIGHT,//  eCMD_MinimumBrightn,
      MSG_LED_MID_BRIGHT,// eCMD_MinBright,
      MSG_LED_COLOR_FULL_CHANGE,//  eCMD_ColorfulMode,
      MSG_LED_COLOR_CHAGE_FAD,//   eCMD_Breathingmode,
      MSG_LED_YELLOW,// eCMD_yellow,
      MSG_LED_GREEG,//  eCMD_green,
      MSG_LED_BLUE,// eCMD_blue,
      MSG_LED_REG,// eCMD_red,
      MSG_LED_PURPLE,// eCMD_purple,
      MSG_LED_cyan,//  eCMD_cyan,
      MSG_LED_WHITE,//  eCMD_white,
      MSG_LED_SENOR_MODE,//eCMD_musicmode,
      MSG_LED_SENOR_MODE,//eCMD_musicmode1,
	  MSG_LED_USB_NIGHT_ON,//	opennight,
	  MSG_LED_OFF,//closenight,	
KEY_NULL,//eCMD_coolmode,
KEY_NULL,//eCMD_warmmode,
KEY_NULL,//eCMD_sunmode,
KEY_NULL,//eCMD_sunmode1,
KEY_NULL,//eCMD_nightlight,
KEY_NULL,//eCMD_setlight1,
KEY_NULL,//eCMD_sleep30s,
KEY_NULL,//eCMD_dimthelight1,
KEY_NULL,//eCMD_brightthelight1,
KEY_NULL,//eCMD_open1,
KEY_NULL,//eCMD_close1,
KEY_NULL,//eCMD_changecolor2,
KEY_NULL,//eCMD_changecolor3,
	};





static const u16 music_led_wy_msg_table[]=
	{
      KEY_NULL,//  eCMD_wakeup_uni,
      MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
      MSG_LED_OFF,// eCMD_default_close,
      MSG_LED_VOICE_COLOR_TEMP,//  eCMD_changecolor,
      MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
      MSG_LED_OFF,//  eCMD_close0,
      KEY_NULL,// eCMD_cmd_no1,
      MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
      MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
	  MSG_LED_VOICE_COLOR_TEMP,//	eCMD_changecolor1,
	  MSG_LED_USB_NIGHT_ON,//  eCMD_open2,
	  MSG_LED_OFF,//  eCMD_close2,
	  MSG_LED_OFF,//  eCMD_close3,	  
	  KEY_NULL,// eCMD_setlight,
      #ifdef LIGHT_TIMER_EN 
      MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
      MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
      MSG_LED_SLEEP_1HOUR_OFF,//eCMD_sleep1hour,
      MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
      #else
      KEY_NULL,//eCMD_sleep10min,
      KEY_NULL,//eCMD_sleep30min,
      KEY_NULL,//eCMD_sleep1hour,
      KEY_NULL,//eCMD_cancelsleepoff,
      #endif
	  MSG_LED_VOICE_MAX_BRIGHT,//  eCMD_MaximumBright,
      MSG_LED_VOICE_MIN_BRIGHT,//  eCMD_MinimumBrightn,
      MSG_LED_MID_BRIGHT,// eCMD_MinBright,
      KEY_NULL,//  eCMD_ColorfulMode,
      KEY_NULL,//   eCMD_Breathingmode,
      KEY_NULL,// eCMD_yellow,
      KEY_NULL,//  eCMD_green,
      KEY_NULL,// eCMD_blue,
      KEY_NULL,// eCMD_red,
      KEY_NULL,// eCMD_purple,
      KEY_NULL,//  eCMD_cyan,
      KEY_NULL,//  eCMD_white,
      KEY_NULL,//eCMD_musicmode,
      KEY_NULL,//eCMD_musicmode1,
	  MSG_LED_USB_NIGHT_ON,//	opennight,
	  MSG_LED_OFF,//closenight,	
KEY_NULL,//eCMD_coolmode,
KEY_NULL,//eCMD_warmmode,
KEY_NULL,//eCMD_sunmode,
KEY_NULL,//eCMD_sunmode1,
KEY_NULL,//eCMD_nightlight,
KEY_NULL,//eCMD_setlight1,
KEY_NULL,//eCMD_sleep30s,
KEY_NULL,//eCMD_dimthelight1,
KEY_NULL,//eCMD_brightthelight1,
KEY_NULL,//eCMD_open1,
KEY_NULL,//eCMD_close1,
KEY_NULL,//eCMD_changecolor2,
KEY_NULL,//eCMD_changecolor3,
	};


static const u16 music_led_w_msg_table[]=
	{
      KEY_NULL,//  eCMD_wakeup_uni,
      MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
      MSG_LED_OFF,// eCMD_default_close,
      KEY_NULL,//  eCMD_changecolor,
      MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
      MSG_LED_OFF,//  eCMD_close0,
      KEY_NULL,// eCMD_cmd_no1,
      MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
      MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
	  KEY_NULL,//	eCMD_changecolor1,
	  MSG_LED_USB_NIGHT_ON,//  eCMD_open2,
	  MSG_LED_OFF,//  eCMD_close2,
	  MSG_LED_OFF,//  eCMD_close3,	  
	  KEY_NULL,// eCMD_setlight,
      #ifdef LIGHT_TIMER_EN 
      MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
      MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
      MSG_LED_SLEEP_1HOUR_OFF,//eCMD_sleep1hour,
      MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
      #else
      KEY_NULL,//eCMD_sleep10min,
      KEY_NULL,//eCMD_sleep30min,
      KEY_NULL,//eCMD_sleep1hour,
      KEY_NULL,//eCMD_cancelsleepoff,
      #endif
	  MSG_LED_VOICE_MAX_BRIGHT,//  eCMD_MaximumBright,
      MSG_LED_VOICE_MIN_BRIGHT,//  eCMD_MinimumBrightn,
      MSG_LED_MID_BRIGHT,// eCMD_MinBright,
      KEY_NULL,//  eCMD_ColorfulMode,
      KEY_NULL,//   eCMD_Breathingmode,
      KEY_NULL,// eCMD_yellow,
      KEY_NULL,//  eCMD_green,
      KEY_NULL,// eCMD_blue,
      KEY_NULL,// eCMD_red,
      KEY_NULL,// eCMD_purple,
      KEY_NULL,//  eCMD_cyan,
      KEY_NULL,//  eCMD_white,
      KEY_NULL,//eCMD_musicmode,
      KEY_NULL,//eCMD_musicmode1,
	  MSG_LED_USB_NIGHT_ON,//	opennight,
	  MSG_LED_OFF,//closenight,	
KEY_NULL,//eCMD_coolmode,
KEY_NULL,//eCMD_warmmode,
KEY_NULL,//eCMD_sunmode,
KEY_NULL,//eCMD_sunmode1,
KEY_NULL,//eCMD_nightlight,
KEY_NULL,//eCMD_setlight1,
KEY_NULL,//eCMD_sleep30s,
KEY_NULL,//eCMD_dimthelight1,
KEY_NULL,//eCMD_brightthelight1,
KEY_NULL,//eCMD_open1,
KEY_NULL,//eCMD_close1,
KEY_NULL,//eCMD_changecolor2,
KEY_NULL,//eCMD_changecolor3,	
	};



static const u16 music_led_wyrgb_msg_table[]=
	{
      KEY_NULL,//  eCMD_wakeup_uni,
      MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
      MSG_LED_OFF,// eCMD_default_close,
      MSG_LED_COLOR_DISP_MODE_WITH_WY,//  eCMD_changecolor,
      MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
      MSG_LED_OFF,//  eCMD_close0,
      KEY_NULL,// eCMD_cmd_no1,
      MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
      MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
	  MSG_LED_COLOR_DISP_MODE_WITH_WY,//	eCMD_changecolor1,
	  MSG_LED_USB_NIGHT_ON,//  eCMD_open2,
	  MSG_LED_OFF,//  eCMD_close2,
	  MSG_LED_OFF,//  eCMD_close3,	  
	  KEY_NULL,// eCMD_setlight,
      #ifdef LIGHT_TIMER_EN 
      MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
      MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
      MSG_LED_SLEEP_1HOUR_OFF,//eCMD_sleep1hour,
      MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
      #else
      KEY_NULL,//eCMD_sleep10min,
      KEY_NULL,//eCMD_sleep30min,
      KEY_NULL,//eCMD_sleep1hour,
      KEY_NULL,//eCMD_cancelsleepoff,
      #endif
	  MSG_LED_VOICE_MAX_BRIGHT,//  eCMD_MaximumBright,
      MSG_LED_VOICE_MIN_BRIGHT,//  eCMD_MinimumBrightn,
      MSG_LED_MID_BRIGHT,// eCMD_MinBright,
      MSG_LED_COLOR_FULL_CHANGE,//  eCMD_ColorfulMode,
      MSG_LED_COLOR_CHAGE_FAD,//   eCMD_Breathingmode,
      MSG_LED_VOICE_COLOR_TEMP_YELLOW,// eCMD_yellow,
      MSG_LED_GREEG,//  eCMD_green,
      MSG_LED_BLUE,// eCMD_blue,
      MSG_LED_REG,// eCMD_red,
      MSG_LED_PURPLE,// eCMD_purple,
      MSG_LED_cyan,//  eCMD_cyan,
      MSG_LED_VOICE_COLOR_TEMP_WHITE,//  eCMD_white,
      MSG_LED_SENOR_MODE,//eCMD_musicmode,
      MSG_LED_SENOR_MODE,//eCMD_musicmode1,
	  MSG_LED_USB_NIGHT_ON,//	opennight,
	  MSG_LED_OFF,//closenight,	
KEY_NULL,//eCMD_coolmode,
KEY_NULL,//eCMD_warmmode,
KEY_NULL,//eCMD_sunmode,
KEY_NULL,//eCMD_sunmode1,
KEY_NULL,//eCMD_nightlight,
KEY_NULL,//eCMD_setlight1,
KEY_NULL,//eCMD_sleep30s,
KEY_NULL,//eCMD_dimthelight1,
KEY_NULL,//eCMD_brightthelight1,
KEY_NULL,//eCMD_open1,
KEY_NULL,//eCMD_close1,
KEY_NULL,//eCMD_changecolor2,
KEY_NULL,//eCMD_changecolor3,	
	};

static const u16 music_led_wrgb_msg_table[]=
	{
      KEY_NULL,//  eCMD_wakeup_uni,
      MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
      MSG_LED_OFF,// eCMD_default_close,
      MSG_LED_COLOR_DISP_MODE_WITH_W,//  eCMD_changecolor,
      MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
      MSG_LED_OFF,//  eCMD_close0,
      KEY_NULL,// eCMD_cmd_no1,
      MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
      MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
	  MSG_LED_COLOR_DISP_MODE_WITH_W,//	eCMD_changecolor1,
	  MSG_LED_USB_NIGHT_ON,//  eCMD_open2,
	  MSG_LED_OFF,//  eCMD_close2,
	  MSG_LED_OFF,//  eCMD_close3,	  
	  KEY_NULL,// eCMD_setlight,
      #ifdef LIGHT_TIMER_EN 
      MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
      MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
      MSG_LED_SLEEP_1HOUR_OFF,//eCMD_sleep1hour,
      MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
      #else
      KEY_NULL,//eCMD_sleep10min,
      KEY_NULL,//eCMD_sleep30min,
      KEY_NULL,//eCMD_sleep1hour,
      KEY_NULL,//eCMD_cancelsleepoff,
      #endif
	  MSG_LED_VOICE_MAX_BRIGHT,//  eCMD_MaximumBright,
      MSG_LED_VOICE_MIN_BRIGHT,//  eCMD_MinimumBrightn,
      MSG_LED_MID_BRIGHT,// eCMD_MinBright,
      MSG_LED_COLOR_FULL_CHANGE,//  eCMD_ColorfulMode,
      MSG_LED_COLOR_CHAGE_FAD,//   eCMD_Breathingmode,
      MSG_LED_YELLOW,// eCMD_yellow,
      MSG_LED_GREEG,//  eCMD_green,
      MSG_LED_BLUE,// eCMD_blue,
      MSG_LED_REG,// eCMD_red,
      MSG_LED_PURPLE,// eCMD_purple,
      MSG_LED_cyan,//  eCMD_cyan,
      MSG_LED_VOICE_COLOR_TEMP_WHITE,//  eCMD_white,
      MSG_LED_SENOR_MODE,//eCMD_musicmode,
      MSG_LED_SENOR_MODE,//eCMD_musicmode1,
	  MSG_LED_USB_NIGHT_ON,//	opennight,
	  MSG_LED_OFF,//closenight,	
KEY_NULL,//eCMD_coolmode,
KEY_NULL,//eCMD_warmmode,
KEY_NULL,//eCMD_sunmode,
KEY_NULL,//eCMD_sunmode1,
KEY_NULL,//eCMD_nightlight,
KEY_NULL,//eCMD_setlight1,
KEY_NULL,//eCMD_sleep30s,
KEY_NULL,//eCMD_dimthelight1,
KEY_NULL,//eCMD_brightthelight1,
KEY_NULL,//eCMD_open1,
KEY_NULL,//eCMD_close1,
KEY_NULL,//eCMD_changecolor2,
KEY_NULL,//eCMD_changecolor3,	
	};



static const u16 music_led_spi_rgb_msg_table[]=

	{
		
             KEY_NULL,//  eCMD_wakeup_uni,
             MSG_LED_VOICE_BRIGHT_UP,//  eCMD_dimthelight,
             MSG_LED_VOICE_BRIGHT_DOWM	,// eCMD_brightthelight,
             MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
             MSG_LED_OFF,// eCMD_default_close,
             MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
             MSG_LED_OFF,//  eCMD_close0,
               MSG_SPILEDS_COLOR_RHYTHM,//eCMD_musicmode,
      
             MSG_SPILEDS_USERMODE_LOOP,//  eCMD_changemode,
             MSG_SPILEDS_MIXCOLOR_WAVE_FLOWING_FADE,//  eCMD_ColorfulMode,
             MSG_SPILEDS_RAINBALL_DOUBLE,//   eCMD_Breathingmode,
             MSG_SPILED_YELLOW,// eCMD_yellow,
             MSG_SPILED_GREEN,//	eCMD_green,
             MSG_SPILED_BLUE,// eCMD_blue,
             MSG_SPILED_RED,// eCMD_red,
             MSG_SPILED_PURPLE,// eCMD_purple,
             MSG_SPILED_CYAN,//  eCMD_cyan,
             MSG_SPILED_MIX_WHITE,//	eCMD_white,
             KEY_NULL,// eCMD_setlight,
             MSG_SPILED_COLOR_SWITCH,//  eCMD_changecolor,
             MSG_SPILED_MIX_WHITE,// eCMD_coolmode,
             MSG_SPILED_YELLOW ,//  eCMD_warmmode,
             KEY_NULL,// eCMD_sunmode,
             KEY_NULL,//  eCMD_nightlight,
             #ifdef LIGHT_TIMER_EN 
             MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
             MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
             MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
             #else
             KEY_NULL,//eCMD_sleep10min,
             KEY_NULL,//eCMD_sleep30min,
             KEY_NULL,//eCMD_cancelsleepoff,
             #endif
             eCMD_cmd_no1,
	
		
	};





static const u16 music_led_spi_rgb_wy_msg_table[]=
	{
		
             KEY_NULL,//  eCMD_wakeup_uni,
             MSG_LED_VOICE_BRIGHT_UP,//  eCMD_dimthelight,
             MSG_LED_VOICE_BRIGHT_DOWM	,// eCMD_brightthelight,
             MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
             MSG_LED_OFF,// eCMD_default_close,
             MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
             MSG_LED_OFF,//  eCMD_close0,
               MSG_SPILEDS_COLOR_RHYTHM,//eCMD_musicmode,
         
             MSG_SPILEDS_USERMODE_LOOP,//  eCMD_changemode,
             MSG_SPILEDS_MIXCOLOR_WAVE_FLOWING_FADE,//  eCMD_ColorfulMode,
             MSG_SPILEDS_RAINBALL_DOUBLE,//   eCMD_Breathingmode,
             MSG_SPILED_YELLOW,// eCMD_yellow,
             MSG_SPILED_GREEN,//	eCMD_green,
             MSG_SPILED_BLUE,// eCMD_blue,
             MSG_SPILED_RED,// eCMD_red,
             MSG_SPILED_PURPLE,// eCMD_purple,
             MSG_SPILED_CYAN,//  eCMD_cyan,
             MSG_SPILED_MIX_WHITE,//	eCMD_white,
             KEY_NULL,// eCMD_setlight,
             MSG_SPILED_COLOR_SWITCH_WY,//  eCMD_changecolor,
             MSG_SPILEDS_TEMP_WHITE,// eCMD_coolmode,
             MSG_SPILEDS_TEMP_YELLOW ,//  eCMD_warmmode,
             MSG_SPILEDS_TEMP_NATURAL,// eCMD_sunmode,
             MSG_SPILEDS_TEMP_NIGHT,//  eCMD_nightlight,
             #ifdef LIGHT_TIMER_EN 
             MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
             MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
             MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
             #else
             KEY_NULL,//eCMD_sleep10min,
             KEY_NULL,//eCMD_sleep30min,
             KEY_NULL,//eCMD_cancelsleepoff,
             #endif
             eCMD_cmd_no1,
	
		
	};
static const u16 music_led_spi_wy_msg_table[]=
	{
		
             KEY_NULL,//  eCMD_wakeup_uni,
             MSG_LED_VOICE_BRIGHT_UP,//  eCMD_dimthelight,
             MSG_LED_VOICE_BRIGHT_DOWM	,// eCMD_brightthelight,
             MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
             MSG_LED_OFF,// eCMD_default_close,
             MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
             MSG_LED_OFF,//  eCMD_close0,
               KEY_NULL,//eCMD_musicmode,
             MSG_SPILEDS_TEMP_SWITCH,//  eCMD_changemode,
             KEY_NULL,//  eCMD_ColorfulMode,
             KEY_NULL,//   eCMD_Breathingmode,
             KEY_NULL,// eCMD_yellow,
             KEY_NULL,//	eCMD_green,
             KEY_NULL,// eCMD_blue,
             KEY_NULL,// eCMD_red,
             KEY_NULL,// eCMD_purple,
             KEY_NULL,//  eCMD_cyan,
             KEY_NULL,//	eCMD_white,
             MSG_SPILEDS_TEMP_NIGHT,// eCMD_setlight,
             MSG_SPILEDS_TEMP_SWITCH,//  eCMD_changecolor,
             KEY_NULL,// eCMD_coolmode,
             KEY_NULL ,//  eCMD_warmmode,
             KEY_NULL,// eCMD_sunmode,
             MSG_SPILEDS_TEMP_NIGHT,//  eCMD_nightlight,
             #ifdef LIGHT_TIMER_EN 
             MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
             MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
             MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
             #else
             KEY_NULL,//eCMD_sleep10min,
             KEY_NULL,//eCMD_sleep30min,
             KEY_NULL,//eCMD_cancelsleepoff,
             #endif
             eCMD_cmd_no1,
	
		
	};

static const u16 music_led_spi_w_msg_table[]=
	{
		
             KEY_NULL,//  eCMD_wakeup_uni,
             MSG_LED_VOICE_BRIGHT_UP,//  eCMD_dimthelight,
             MSG_LED_VOICE_BRIGHT_DOWM	,// eCMD_brightthelight,
             MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
             MSG_LED_OFF,// eCMD_default_close,
             MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
             MSG_LED_OFF,//  eCMD_close0,
               KEY_NULL,//eCMD_musicmode,
             KEY_NULL,//  eCMD_changemode,
             KEY_NULL,//  eCMD_ColorfulMode,
             KEY_NULL,//   eCMD_Breathingmode,
             KEY_NULL,// eCMD_yellow,
             KEY_NULL,//	eCMD_green,
             KEY_NULL,// eCMD_blue,
             KEY_NULL,// eCMD_red,
             KEY_NULL,// eCMD_purple,
             KEY_NULL,//  eCMD_cyan,
             KEY_NULL,//	eCMD_white,
             MSG_SPILEDS_TEMP_NIGHT,// eCMD_setlight,
             KEY_NULL,//  eCMD_changecolor,
             KEY_NULL,// eCMD_coolmode,
             KEY_NULL ,//  eCMD_warmmode,
             KEY_NULL,// eCMD_sunmode,
             MSG_SPILEDS_TEMP_NIGHT,//  eCMD_nightlight,
             #ifdef LIGHT_TIMER_EN 
             MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
             MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
             MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
             #else
             KEY_NULL,//eCMD_sleep10min,
             KEY_NULL,//eCMD_sleep30min,
             KEY_NULL,//eCMD_cancelsleepoff,
             #endif
             eCMD_cmd_no1,
	
		
	};



static const u16 music_led_spi_wrgb_msg_table[]=
	{
		
             KEY_NULL,//  eCMD_wakeup_uni,
             MSG_LED_VOICE_BRIGHT_UP,//  eCMD_dimthelight,
             MSG_LED_VOICE_BRIGHT_DOWM	,// eCMD_brightthelight,
             MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
             MSG_LED_OFF,// eCMD_default_close,
             MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
             MSG_LED_OFF,//  eCMD_close0,
               MSG_SPILEDS_COLOR_RHYTHM,//eCMD_musicmode,
   
             MSG_SPILEDS_USERMODE_LOOP,//  eCMD_changemode,
             MSG_SPILEDS_MIXCOLOR_WAVE_FLOWING_FADE,//  eCMD_ColorfulMode,
             MSG_SPILEDS_RAINBALL_DOUBLE,//   eCMD_Breathingmode,
             MSG_SPILED_YELLOW,// eCMD_yellow,
             MSG_SPILED_GREEN,//	eCMD_green,
             MSG_SPILED_BLUE,// eCMD_blue,
             MSG_SPILED_RED,// eCMD_red,
             MSG_SPILED_PURPLE,// eCMD_purple,
             MSG_SPILED_CYAN,//  eCMD_cyan,
             MSG_SPILED_MIX_WHITE,//	eCMD_white,
             KEY_NULL,// eCMD_setlight,
             MSG_SPILED_COLOR_SWITCH,//  eCMD_changecolor,
             MSG_SPILEDS_TEMP_WHITE,// eCMD_coolmode,
             KEY_NULL ,//  eCMD_warmmode,
             KEY_NULL,// eCMD_sunmode,
             KEY_NULL,//  eCMD_nightlight,
             #ifdef LIGHT_TIMER_EN 
             MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
             MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
             MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
             #else
             KEY_NULL,//eCMD_sleep10min,
             KEY_NULL,//eCMD_sleep30min,
             KEY_NULL,//eCMD_cancelsleepoff,
             #endif
             eCMD_cmd_no1,
	
		
	};






typedef struct tone_map_s {
	uint16_t idx;
	uint16_t name;
} tone_map_t;

enum {
    UNISOUMD_IDEX_TONE_NUM_101,


	
    UNISOUMD_IDEX_TONE_NONE ,
};



/*
101.pcm    已是最大音量
102.pcm    已是最小音量
103.pcm    我在
104.pcm    有需要再叫我
105.pcm    欢迎使用



*/
	static const u8 tone_cmd_table[]=
	{
	0x01,// 103: 主人，您好

	
	};

static const char *const music_led_tone_index[] = {



	
};

static const tone_map_t tone_mapping[] = {
    {UNISOUMD_IDEX_TONE_NUM_101, 101},


};






#endif
