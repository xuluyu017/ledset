#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED    (-1.98)
#endif
#define KWS_SLEEP_SCORE_THRED    (-0.6)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED       (-1.83)
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



enum {
  eCMD_wakeup_uni,
  eCMD_default_open,
  eCMD_default_close,
  eCMD_changecolor,
  eCMD_open0,
  eCMD_close0,
  eCMD_cmd_no1,
  eCMD_brightthelight,
  eCMD_dimthelight,
  eCMD_ColorfulMode,
  eCMD_Breathingmode,
  eCMD_yellow,
  eCMD_green,
  eCMD_blue,
  eCMD_red,
  eCMD_purple,
  eCMD_cyan,
  eCMD_white,
  eCMD_musicmode,
  eCMD_musicmode1,

  


    eCMD_setcolor,
  eCMD_sleep30s,
};


#define NLU_CONTENT_SIZE 20
#define NLU_CONTENT_MAP_SIZE 27


extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];



#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0




//#define  EN_SCORE_THRED_FOR_EVERY_CMD




/*

KWSA_light_libD1:
唤醒词（灵敏度：中）：小爱小爱、你好小爱
命令词（灵敏度：中）：
default_open=开灯
default_close=关灯
changecolor=变颜色
open0=打开电灯
close0=关闭电灯
cmd_no1=开|关|灯|开开|关关|颜色|色
dimthelight=调亮一点
brightthelight=调暗一点
ColorfulMode=七彩变色
Breathingmode=呼吸变色
yellow=黄色
green=绿色
blue=蓝色
red=红色
purple=紫色
cyan=青色
white=白色
musicmode=音乐模式
musicmode1=音乐律动

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
		
	};



#if defined(AUTO_DET_RGB_IO)||defined(LED_W_PORT)||defined(LED_Y_PORT)

#ifdef LED_Y_PORT

static const u16 music_led_wy_msg_table[]=
	{
		
		  KEY_NULL,//  eCMD_wakeup_uni,
		  MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
		  MSG_LED_OFF,// eCMD_default_close,
		  MSG_LED_VOICE_COLOR_TEMP,//	eCMD_changecolor,
		  MSG_LED_USB_NIGHT_ON,//  eCMD_open0,
		  MSG_LED_OFF,//  eCMD_close0,
		  KEY_NULL,// eCMD_cmd_no1,
		  MSG_LED_VOICE_BRIGHT_UP,//  eCMD_brightthelight,
		  MSG_LED_VOICE_BRIGHT_DOWM  ,// eCMD_dimthelight,
		  KEY_NULL,//	eCMD_ColorfulMode,
		  KEY_NULL,//   eCMD_Breathingmode,
		  MSG_LED_VOICE_COLOR_TEMP_YELLOW,// eCMD_yellow,
		  KEY_NULL,//	eCMD_green,
		  KEY_NULL,// eCMD_blue,
		  KEY_NULL,// eCMD_red,
		  KEY_NULL,// eCMD_purple,
		  KEY_NULL,//  eCMD_cyan,
		  MSG_LED_VOICE_COLOR_TEMP_WHITE,//	eCMD_white,
		  KEY_NULL,//eCMD_musicmode,
		  KEY_NULL,//eCMD_musicmode1,
			
		};

#endif


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
		
	};

#ifndef DISABLE_RGB_LED

#ifdef LED_Y_PORT

static const u16 music_led_wyrgb_msg_table[]=
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
		
	};
#endif

static const u16 music_led_wrgb_msg_table[]=
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
		
	};
#endif

#endif


#ifdef USE_SPI_DRIVE_RGBLED

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
#endif






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
