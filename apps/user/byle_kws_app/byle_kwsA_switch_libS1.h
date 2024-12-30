#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED    (-1.47)  //  (-1.98)			//BZ
#endif
#define KWS_SLEEP_SCORE_THRED    (-0.6)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED      (-0.81)  // (-1.83)    //BZ
#endif
#define KWS_HASHTABLE_SIZE      (13)//   (52)

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
};

#define NLU_CONTENT_SIZE 7
#define NLU_CONTENT_MAP_SIZE 14

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];


#define EN_SCORE_THRED_FOR_EVERY_CMD
static const float g_nlu_score_thred[]=
	{
	KWS_WAKEUP_SCORE_THRED  ,//eCMD_wakeup_uni
	#ifdef OPEN_CLOSE_KWS_CMD_SCORE_THRED_HIGH			//BZ
	 (KWS_CMD_SCORE_THRED-4),// KWS_CMD_SCORE_THRED,//		eCMD_default_open,
	 (KWS_CMD_SCORE_THRED-4),//KWS_CMD_SCORE_THRED-0.5,//eCMD_default_close,					//BZ  灵敏度中版本 -2    灵敏度高高版本 -4  
	#else
	 (KWS_CMD_SCORE_THRED-2),// KWS_CMD_SCORE_THRED,//		eCMD_default_open,
	 (KWS_CMD_SCORE_THRED-2),//KWS_CMD_SCORE_THRED-0.5,//eCMD_default_close,					//BZ  灵敏度中版本 -2    灵敏度高高版本 -4  
	#endif
	KWS_CMD_SCORE_THRED,//eCMD_changecolor,
	 KWS_CMD_SCORE_THRED,//	eCMD_open0,
	 KWS_CMD_SCORE_THRED,//eCMD_close0,
	KWS_CMD_SCORE_THRED  ,//cmd_no1,

		
	};

//
#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0
#define DIASBLE_eCMD_setlight

#define USER_WY_COLOR_REPEAT


/*
kwsA_switch_libS1
唤醒词（灵敏度：中）：小爱小爱、你好小爱
命令词（灵敏度：中）：
default_open=开灯
default_close=关灯
changecolor=变颜色
open0=打开电灯
close0=关闭电灯
cmd_no1=开|关|灯|开开|关关|颜色|灯灯



*/
static const u16 Byle_kws_msg_table[]=
	{
	KEY_NULL  ,//eCMD_wakeup_uni
	 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
	 MSG_LED_OFF,//eCMD_default_close,
	MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor,
	 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
	 MSG_LED_OFF,//eCMD_close0,
	KEY_NULL  ,//cmd_no1,

		
	};




#if defined(LED_W_PORT)||defined(LED_Y_PORT)



static const u16 music_led_wy_msg_table[]=
	{
		KEY_NULL  ,//eCMD_wakeup_uni
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		MSG_LED_VOICE_COLOR_TEMP,//eCMD_changecolor,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		KEY_NULL  ,//cmd_no1,
	
			
};



static const u16 music_led_w_msg_table[]=
	{
		KEY_NULL  ,//eCMD_wakeup_uni
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		KEY_NULL,//eCMD_changecolor,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		KEY_NULL  ,//cmd_no1,
	
			
};
static const u16 music_led_wyrgb_msg_table[]=
	{
		KEY_NULL  ,//eCMD_wakeup_uni
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		MSG_LED_COLOR_SEL_WITH_WY,//eCMD_changecolor,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		KEY_NULL  ,//cmd_no1,
	
			
};

static const u16 music_led_wrgb_msg_table[]=
	{
		KEY_NULL  ,//eCMD_wakeup_uni
		 MSG_LED_USB_NIGHT_ON,//		eCMD_default_open,
		 MSG_LED_OFF,//eCMD_default_close,
		MSG_LED_COLOR_SEL_WITH_W,//eCMD_changecolor,
		 MSG_LED_USB_NIGHT_ON,//	eCMD_open0,
		 MSG_LED_OFF,//eCMD_close0,
		KEY_NULL  ,//cmd_no1,
	
			
};

#endif



#endif

