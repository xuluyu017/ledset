#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED    (-2.86)
#endif
#define KWS_SLEEP_SCORE_THRED    (-0.6)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED       (-3.75)
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


enum {
	eCMD_default_open,
	eCMD_open0,
	eCMD_default_close,
	eCMD_close0,
	eCMD_open1,
	eCMD_close1,
	eCMD_close2,
	eCMD_changecolor,
	eCMD_changecolor1,
	eCMD_changecolor2,
	eCMD_brightthelight,
	eCMD_dimthelight,
	eCMD_coolmode,
	eCMD_warmmode,
	eCMD_cancelsleepoff,
	eCMD_sleep30min,
	eCMD_setlight,
	eCMD_wakeup_uni,
	eCMD_sleep10min,
	eCMD_musicmode,

	
eCMD_setcolor,
  eCMD_changemode,
  eCMD_ColorfulMode,
  eCMD_Breathingmode,
  eCMD_yellow,
  eCMD_green,
  eCMD_blue,
  eCMD_red,
  eCMD_purple,
  eCMD_cyan,
  eCMD_white,
  eCMD_sunmode,
  eCMD_nightlight,
  eCMD_sleep30s,
  eCMD_cmd_no1,
  eCMD_dimthelight1,
  eCMD_brightthelight1,
  eCMD_open2,

};

/*关键词阈值调整*/
static const  float confidence[20] = {
	0.45,//打开电灯
	0.55,//开灯
	0.12,//关闭电灯
	0.65,//关灯
	0.44,//我回来了
	0.27,//我睡觉了
	0.42,//我出去了
	0.42,//变颜色
	0.42,//换颜色
	0.42,//改颜色
	0.35,//调亮一点
	0.4,//调暗一点
	0.3,//白色光
	0.15,//黄色光
	0.35,//取消定时
	0.3,//三十分钟后关灯
	0.4,//设置定时
	0.4,//小爱小爱---唤醒词
	0.3,//十分钟后关灯
	0.4,//音乐律动
};




//#define NLU_CONTENT_SIZE 48
//#define NLU_CONTENT_MAP_SIZE 51

//#define  RE_DEFINE_MAPPING 3081464298U
//#define  RE_DEFINE_MAPPING_index  eCMD_brightthelight


//extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
//extern const byle_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];
//
#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0

#define USER_open2   eCMD_open1


#define DIASBLE_eCMD_setlight


#define USER_CHANGE_COLOR2  eCMD_changecolor1
#define USER_CHANGE_COLOR3  eCMD_changecolor2


#define USER_WY_COLOR_REPEAT





/*
default_open=打开电灯 
open0=开灯
default_close=关闭电灯
close0=关灯
open2=我回来了
close1=我睡觉了
close1=我出去了
changecolor=变颜色
changecolor1=改颜色
changecolor2=换颜色
brightthelight=调亮一点
dimthelight=调暗一点








*/
#ifdef BYLE_UATR_ENABLE
#define  BYLE_UART_SEND_USER_CMD_TABLE  3
#define CMD_DATA 0XAA
#define CMD_DATA_END 0X55

#define CMD_WAKEUP_DATA 0X81
#define SET_TIME_OUT   20 //s
#define  SEND_USER_CMD_POWER    NLU_CONTENT_SIZE
#define  SEND_USER_CMD_EXIT         NLU_CONTENT_SIZE+1

static const u8 cmd_table[][BYLE_UART_SEND_USER_CMD_TABLE]=
{
 {CMD_DATA,0X70,CMD_DATA_END},//    wakeup=小爱小爱（免唤醒命令词）
 {CMD_DATA,0X71,CMD_DATA_END},//	wakeup1=小丽小丽 （免唤醒命令词）
 {CMD_DATA,0X01,CMD_DATA_END},//	dimthelight=调亮一点
 {CMD_DATA,0X02,CMD_DATA_END},//		brightthelight=调暗一点
 {CMD_DATA,0X03,CMD_DATA_END},//		MaximumBright=最大亮度
 {CMD_DATA,0X04,CMD_DATA_END},//		MinimumBrightn=最小亮度
 {CMD_DATA,0X05,CMD_DATA_END},//		MinBright=中等亮度
 {CMD_DATA,0X06,CMD_DATA_END},//		default_open=打开电灯		  
 {CMD_DATA,0X07,CMD_DATA_END},//		default_close=关闭电灯
 {CMD_DATA,0X08,CMD_DATA_END},//		open0=开灯
 {CMD_DATA,0X09,CMD_DATA_END},//		close0=关灯
 {CMD_DATA,0X0a,CMD_DATA_END},//		open1=打开灯光
 {CMD_DATA,0X0b,CMD_DATA_END},//		close1=关闭灯光
 {CMD_DATA,0X0c,CMD_DATA_END},//		musicmode=音乐模式
 {CMD_DATA,0X0d,CMD_DATA_END},//		changemode=切换模式
 {CMD_DATA,0X0e,CMD_DATA_END},//		ColorfulMode=七彩变色
 {CMD_DATA,0X0f,CMD_DATA_END},//		Breathingmode=呼吸变色
 {CMD_DATA,0X10,CMD_DATA_END},//		yellow=黄色
 {CMD_DATA,0X11,CMD_DATA_END},//	green=绿色
 {CMD_DATA,0X12,CMD_DATA_END},//	blue=蓝色
 {CMD_DATA,0X13,CMD_DATA_END},//	red=红色
 {CMD_DATA,0X14,CMD_DATA_END},//	purple=紫色
 {CMD_DATA,0X15,CMD_DATA_END},//	cyan=青色
 {CMD_DATA,0X16,CMD_DATA_END},//	white=白色
 {CMD_DATA,0X17,CMD_DATA_END},//	setlight=设置定时
 {CMD_DATA,0X18,CMD_DATA_END},//	changecolor=换颜色
 {CMD_DATA,0X19,CMD_DATA_END},//	changecolor2=变颜色
 {CMD_DATA,0X1a,CMD_DATA_END},//	coolmode=白色光
 {CMD_DATA,0X1b,CMD_DATA_END},//	warmmode=黄色光
 {CMD_DATA,0X1c,CMD_DATA_END},//	sunmode=中性光
 {CMD_DATA,0X1d,CMD_DATA_END},//	nightlight=夜灯模式
 {CMD_DATA,0X1c,CMD_DATA_END},//	sleep30s=三十秒后关灯
 {CMD_DATA,0X1d,CMD_DATA_END},//	sleep10min=十分钟后关灯
 {CMD_DATA,0X1e,CMD_DATA_END},//	sleep30min=三十分钟后关灯
 {CMD_DATA,0X1f,CMD_DATA_END},//	cancelsleepoff=取消定时
 {CMD_DATA,0X20,CMD_DATA_END},//	cmd_no1=开|关|灯|色|颜色|回来|出去|睡觉|来了|去了
 {CMD_DATA,0X21,CMD_DATA_END},//	dimthelight1=亮一点
 {CMD_DATA,0X22,CMD_DATA_END},//	brightthelight1=暗一点
 {CMD_DATA,0X23,CMD_DATA_END},//open2=回来了
 {CMD_DATA,0X24,CMD_DATA_END},//	close2=睡觉了
 {CMD_DATA,0X25,CMD_DATA_END},//close3=出去了
 {CMD_DATA,0X26,CMD_DATA_END},//	musicmode1=律动模式
 {CMD_DATA,0X27,CMD_DATA_END},//	musicmode2=音乐律动
 {CMD_DATA,0X28,CMD_DATA_END},//	changemode1=切换灯光
 {CMD_DATA,0X29,CMD_DATA_END},//	changemode2=改变模式
 {CMD_DATA,0X2a,CMD_DATA_END},//	setlight1=设置灯光
 {CMD_DATA,0X2b,CMD_DATA_END},//	changecolor1=改颜色
 {CMD_DATA,0X2c,CMD_DATA_END},//	sunmode1=自然光
  {CMD_DATA,0X80,CMD_DATA_END},//	POWER ON
{CMD_DATA,0X81,CMD_DATA_END},//   _EXIT ON
};
#endif
static const u16 Byle_kws_msg_table[]=
	{
	 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
	 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
	 MSG_LED_OFF,//eCMD_default_close,
	 MSG_LED_OFF,//eCMD_close0,
	 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
	 MSG_LED_OFF,//eCMD_close1,
	 MSG_LED_OFF,//eCMD_close2,
	 MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor,
	 MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor1,
	MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//MSG_LED_SENOR_MODE,// MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor2,			//BZ
	 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
	MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,

		
	};



typedef struct tone_map_s {
	uint16_t idx;
	uint16_t name;
} tone_map_t;


enum {
    BYLE_KWS_IDEX_TONE_NUM_500,
    BYLE_KWS_IDEX_TONE_NUM_501,
    BYLE_KWS_IDEX_TONE_NUM_502,
    BYLE_KWS_IDEX_TONE_NUM_503,
    BYLE_KWS_IDEX_TONE_NUM_504,
    BYLE_KWS_IDEX_TONE_NUM_505,
    BYLE_KWS_IDEX_TONE_NUM_506,
    BYLE_KWS_IDEX_TONE_NUM_507,
    BYLE_KWS_IDEX_TONE_NUM_508,
    BYLE_KWS_IDEX_TONE_NUM_509,
    BYLE_KWS_IDEX_TONE_NUM_510,
    BYLE_KWS_IDEX_TONE_NUM_511,
    BYLE_KWS_IDEX_TONE_NUM_512,
    BYLE_KWS_IDEX_TONE_NUM_513,
    BYLE_KWS_IDEX_TONE_NUM_514,
    BYLE_KWS_IDEX_TONE_NUM_515,
    BYLE_KWS_IDEX_TONE_NUM_516,
    BYLE_KWS_IDEX_TONE_NUM_517,
    BYLE_KWS_IDEX_TONE_NUM_518,
    BYLE_KWS_IDEX_TONE_NUM_519,

	
    BYLE_KWS_IDEX_TONE_NONE ,
};

#define  POWER_ON_TONE  BYLE_KWS_IDEX_TONE_NUM_500


/*
500.mp3	欢迎使用智能语音灯，你可以叫小爱小爱唤醒我

501.mp3	谢谢使用

502.mp3	我在

503.mp3	灯光已打开

504.mp3	灯光已关闭

505.mp3	已调亮

506.mp3	已调暗

507.mp3	黄色

508.mp3	绿色

509.mp3	蓝色

510.mp3	红色

511.mp3	紫色

512.mp3	青色

513.mp3	白色

514.mp3	音乐模式

515.mp3	已变颜色

516.mp3	三十秒后关灯

517.mp3	十分钟后关灯

518.mp3	三十分钟后关灯

519.mp3	取消定时




*/
	
	static const char* g_nlu_content_str_bye[][2] = {
	[eCMD_default_open] = {"default_open", "503"},
	/*[eCMD_open0] = {"open", "503"},
	[eCMD_close0] = {"close", "504"},
	[eCMD_close1] = {"close", "504"},
	[eCMD_open1] = {"open", "503"},
	[eCMD_close1] = {"open", "504"},
	 [eCMD_close2] = {"close", "504"},
	[eCMD_changecolor] = {"changecolor", "515"},
	[eCMD_changecolor1] = {"changecolor", "515"},
	[eCMD_changecolor2] = {"changecolor", "515"},
	[eCMD_brightthelight] = {"brightthelight", "506"},
	[eCMD_dimthelight] = {"dimthelight", "505"},*/



	};

	static const u8 tone_cmd_table[]=
	{
	0x01,// 103: 主人，您好
	0xf1,// 104: 谢谢使用

	
	};

static const char *const Byle_kws_tone_index[] = {


	
};

static const tone_map_t tone_mapping[] = {
    {BYLE_KWS_IDEX_TONE_NUM_500, 500},

	
};



#if defined(LED_W_PORT)||defined(LED_Y_PORT)



static const u16 music_led_wy_msg_table[]=

	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor1,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor2,
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
                MSG_LED_VOICE_COLOR_TEMP_WHITE,//eCMD_coolmode,
                MSG_LED_VOICE_COLOR_TEMP_YELLOW,//eCMD_warmmode,
                MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
                MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
                KEY_NULL,//eCMD_setlight,
                KEY_NULL,//eCMD_wakeup_uni,
                MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
                MSG_LED_SENOR_MODE,//eCMD_musicmode,
		};

	;


static const u16 music_led_w_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 KEY_NULL,//eCMD_changecolor,
		 KEY_NULL,//eCMD_changecolor1,
		 KEY_NULL,//eCMD_changecolor2,
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
		
	        MSG_LED_VOICE_COLOR_TEMP_WHITE,//eCMD_coolmode,
                KEY_NULL,//eCMD_warmmode,
                MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
                MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
                KEY_NULL,//eCMD_setlight,
                KEY_NULL,//eCMD_wakeup_uni,
                MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
                KEY_NULL,//eCMD_musicmode,
			
		};
static const u16 music_led_wyrgb_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor1,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor2,
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
               MSG_LED_VOICE_COLOR_TEMP_WHITE,//eCMD_coolmode,
                MSG_LED_VOICE_COLOR_TEMP_YELLOW,//eCMD_warmmode,
                MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
                MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
                KEY_NULL,//eCMD_setlight,
                KEY_NULL,//eCMD_wakeup_uni,
                MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
                MSG_LED_SENOR_MODE,//eCMD_musicmode,			
		};

static const u16 music_led_wrgb_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor1,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor2,
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	        MSG_LED_VOICE_COLOR_TEMP_WHITE,//eCMD_coolmode,
                KEY_NULL,//eCMD_warmmode,
                MSG_LED_SLEEP_CANSEL,//eCMD_cancelsleepoff,
                MSG_LED_SLEEP_30MIN_OFF,//eCMD_sleep30min,
                KEY_NULL,//eCMD_setlight,
                KEY_NULL,//eCMD_wakeup_uni,
                MSG_LED_SLEEP_10MIN_OFF,//eCMD_sleep10min,
                MSG_LED_SENOR_MODE,//eCMD_musicmode,
			
		};

#endif



#endif

