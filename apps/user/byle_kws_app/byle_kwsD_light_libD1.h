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
	eCMD_open0,
	eCMD_close0,
	eCMD_default_open,
	eCMD_default_close,
	eCMD_open1,
	eCMD_close1,
	eCMD_close2,
	eCMD_changecolor,
	eCMD_changecolor1,
	eCMD_changecolor2,
	 eCMD_musicmode,   //BZ
	eCMD_brightthelight,
	eCMD_dimthelight,

	eCMD_wakeup_uni,  
eCMD_setcolor,
//  eCMD_musicmode,    //BZ
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
  eCMD_setlight,
  eCMD_coolmode,
  eCMD_warmmode,
  eCMD_sunmode,
  eCMD_nightlight,
  eCMD_sleep30s,
  eCMD_sleep10min,
  eCMD_sleep30min,
  eCMD_cancelsleepoff,
  eCMD_cmd_no1,
  eCMD_dimthelight1,
  eCMD_brightthelight1,
  eCMD_open2,

};

#define NLU_CONTENT_SIZE eCMD_open2+1

#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0
#define USER_open2   eCMD_open1
#define DIASBLE_eCMD_setlight
#define USER_CHANGE_COLOR2  eCMD_changecolor1
#define USER_CHANGE_COLOR3  eCMD_changecolor2
#define USER_WY_COLOR_REPEAT

// ########## Copy the following to main.c ###########
static  const  char*   g_nlu_content_str_bye[]= {
    "kai deng",            /*** 0 ***/
    "guan deng",           /*** 1 ***/
    "da kai dian deng",    /*** 2 ***/
    "guan bi dian deng",   /*** 3 ***/
    "hui lai le",          /*** 4 ***/
    "chu qu le",           /*** 5 ***/
    "shui jiao le",        /*** 6 ***/
    "bian yan se",         /*** 7 ***/
    "bian se",             /*** 8 ***/
    "huan yan se",         /*** 9 ***/
    "gai yan se",          /*** 10 ***/
    "liang yi dian",       /*** 11 ***/
    "an yi dian",          /*** 12 ***/
};

//offset to scores of each keyword, will be added to final score
//Therefore, positive will make the kw more likely, negative will make kw ess likely.
// const u8 kw_offset[] = {-2, -8, 0, 0, -11, -4, -3, 0, -5, 0, 0, -11, -5};
// const u8 kw_offset[] = {3, -3, 5, 5, -6, 1, 2, 5, 0, 5, 5, -6, 0};        //V1     +5                                           micgain=14
// const u8 kw_offset[] = {3, -3, 5, 5, -6, 1, 2, 5, 0, 10, 5, -6, 0};       //V2     换颜色不应  huan yan se +5                    micgain=14
// const u8 kw_offset[] = {3, -3, 5, 5, -6, 1, 2, 5, 0, 10, 5, -6, -5};      //V3     喊换颜色误识别成暗一点，故"an yi dian" 不变     micgain=14
// const u8 kw_offset[] = {3, -3, 5, 5, -6, 1, 2, 5, 0, 10, 5, -6, -8};      //V4     喊亮一点误识别成暗一点  暗一点调低              micgain=14
// const u8 kw_offset[] = {0, -3, 5, 5, -6, 1, 2, 5, 0, 10, 5, -6, -8};      //V5     嘈杂环境 误识别成关灯   关灯调低                micgain=14
#define  byle_kw_offset {0, -3, 5, 5, -3, 4, 5, 5, 0, 10, 5, -6, -8}    //V6     出去了，睡觉了，回来了   +3                    micgain=7
//To compensate for model's bias, need to use bi-gram LM
//structure: first token, second token, score offset
//Note that this score is only the score of one phone instead of the whole word
#define  byle_NUM_BIGRAMS   0;
#define  byle_bigram_list   {}

#define  byle_KW_TRIGGER_SCORE -75
#define  byle_KW_OUT_SCORE  -75

//during sleep, kw output threshold will be WAKE_SCORE, threshold returns to KW_OUT_SCORE after 1 word is triggered.
//If no word is detected for WAKE_STEPS steps, will be back to sleep.
//one step is 32ms
#define  byle_WAKE_STEPS  600
#define  byle_WAKE_SCORE  -50//Seems that now score distribution is similar to before, higher than -50 is very rare
  #define  byle_USE_KW_PRE_SUPPRESS 0
#define  byle_KW_PRE_SUPPRESS_MAX_STEPS 30//max steps before a kw that will suppress the kw


 static const u16 Byle_kws_msg_table[]=
	{
	 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
	 MSG_LED_OFF,//eCMD_default_close,
	MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
	 MSG_LED_OFF,//eCMD_close0,
	 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
	 MSG_LED_OFF,//eCMD_close1,
	 MSG_LED_OFF,//eCMD_close2,
	 MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor,
	 MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor1,
	  MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor1,          //BZ
	MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//MSG_LED_SENOR_MODE,// MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor2,        //BZ  律动功能   改为 变颜色功能
	 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
	MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,

		
	};





static const u16 music_led_wy_msg_table[]=

	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor1,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor2,
		 MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor2,           	 //BZ
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
			
		};

	// ;    BZ


static const u16 music_led_w_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 KEY_NULL,//eCMD_changecolor,
		 KEY_NULL,//eCMD_changecolor1,
		 KEY_NULL,//eCMD_changecolor2,
		 KEY_NULL,//eCMD_changecolor2,               //BZ
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
			
		};
static const u16 music_led_wyrgb_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor1,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor2,
		 MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor2,   //BZ
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
			
		};

static const u16 music_led_wrgb_msg_table[]=
	{
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 
		 MSG_LED_OFF,//eCMD_close0,
		 MSG_LED_USB_NIGHT_ON,//eCMD_open1,
		 MSG_LED_OFF,//eCMD_close1,
		 MSG_LED_OFF,//eCMD_close2,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor1,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor2,
		 MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor2,       //BZ
		 MSG_LED_VOICE_BRIGHT_UP,//eCMD_brightthelight,
		MSG_LED_VOICE_BRIGHT_DOWM  ,//eCMD_dimthelight,
	
			
		};




#endif

