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
#define KWS_HASHTABLE_SIZE        (59)

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





enum {
  eCMD_wakeup_uni,
  eCMD_open1,
  eCMD_close1,
  eCMD_open3,
  eCMD_close3,
  eCMD_open0,
  eCMD_close0,
  eCMD_changecolor,
  eCMD_changecolor1,
  eCMD_musicmode,
  eCMD_musicmode1,
  eCMD_ColorfulMode,
  eCMD_Breathingmode,
  eCMD_coolmode,
  eCMD_warmmode,
  eCMD_sunmode,
  eCMD_nightlighton,
  eCMD_nightlightoff,
  eCMD_brightthelight,
  eCMD_dimthelight,
  eCMD_brightthelight1,
  eCMD_dimthelight2,
  eCMD_MaximumBright,
  eCMD_MinimumBrightn,
  eCMD_MinBright,
  eCMD_FAN_ON,
  eCMD_FAN_OFF,
  eCMD_SWING_ON,
  eCMD_SWING_OFF,
  eCMD_Windspeed1,
  eCMD_Windspeed2,
  eCMD_Windspeed3,
  eCMD_Windspeed4,
  eCMD_Windspeed5,
  eCMD_Windspeed6,
  eCMD_Windspeed7,
  eCMD_WindNatural,
  eCMD_WindNor,
  eCMD_WindSleep,
  eCMD_MaxSpeed,
  eCMD_MinSpeed,
  eCMD_WindUp,
  eCMD_WindDowm,
  eCMD_Timer1Hour,
  eCMD_Timer2Hour,
  eCMD_TimerHour2,
  eCMD_Timer3Hour,
  eCMD_Timer4Hour,
  eCMD_Timer5Hour,
  eCMD_Timer6Hour,
  eCMD_Timer7Hour,
  eCMD_Timer8Hour,
  eCMD_Timer9Hour,
  eCMD_Timer10Hour,
  eCMD_Timer11Hour,
  eCMD_Timer12Hour,
  eCMD_TimerCancel,
  eCMD_TimerOff,
};

#define NLU_CONTENT_SIZE 58
#define NLU_CONTENT_MAP_SIZE 59

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];




#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0
#define USER_open1  eCMD_open2

#define LIGHT_TIMER_EN

#define USER_WY_COLOR_REPEAT


//#define  EN_SCORE_THRED_FOR_EVERY_CMD




/*

唤醒词（灵敏度：中）：你好小艾、小艾小艾
命令词（灵敏度：中）：
open1=打开灯光
close1=关闭灯光
open3=打开灯
close3=关闭灯
open0=开灯
close0=关灯
changecolor=变颜色
changecolor1=改变颜色
musicmode=音乐模式
musicmode1=音乐律动
ColorfulMode=七彩变色
Breathingmode=呼吸变色
coolmode=白色光
warmmode=黄色光
sunmode=中性光
nightlighton=打开夜灯
nightlightoff=关闭夜灯
brightthelight=亮一点
dimthelight=暗一点
brightthelight1=调亮一点
dimthelight2=调暗一点
MaximumBright=最大亮度
MinimumBrightn=最小亮度
MinBright=中等亮度
FAN_ON=打开风扇
FAN_OFF=关闭风扇
SWING_ON=打开摇头
SWING_OFF=关闭摇头
Windspeed1=一档风
Windspeed2=二档风
Windspeed3=两档风
Windspeed4=三档风
Windspeed5=四档风
Windspeed6=五档风
Windspeed7=六档风
WindNatural=自然风
WindNor=正常风
WindSleep=睡眠风
MaxSpeed=最大风
MinSpeed=最小风
WindUp=增大风速
WindDowm=减小风速
Timer1Hour=定时一小时
Timer2Hour=定时二小时
TimerHour2=定时两小时
Timer3Hour=定时三小时
Timer4Hour=定时四小时
Timer5Hour=定时五小时
Timer6Hour=定时六小时
Timer7Hour=定时七小时
Timer8Hour=定时八小时
Timer9Hour=定时九小时
Timer10Hour=定时十小时
Timer11Hour=定时十一小时
Timer12Hour=定时十二小时
TimerCancel=取消定时
TimerOff=关闭定时


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
