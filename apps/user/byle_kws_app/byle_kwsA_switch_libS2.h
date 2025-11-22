#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED    (0.73)
#endif
#define KWS_SLEEP_SCORE_THRED    (1.69)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED       (-1.9)
#endif
#define KWS_HASHTABLE_SIZE     (32)// (13)//   (52)

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
  eCMD_default_open,// eCMD_cmd01,
  eCMD_cmd02,
  eCMD_cmd03,
  eCMD_default_close,//eCMD_cmd04,
  eCMD_cmd05,
  eCMD_cmd06,
  eCMD_cmd07,
  eCMD_cmd08,
  eCMD_cmd09,
  eCMD_cmd10,
  eCMD_cmd11,
  eCMD_cmd12,
  eCMD_cmd13,
  eCMD_cmd14,
  eCMD_cmd15,
  eCMD_cmd16,
  eCMD_cmd17,
  eCMD_cmd18,
  eCMD_changecolor,//eCMD_cmd19,
  eCMD_cmd20,
  eCMD_cmd21,
  eCMD_cmd22,
  eCMD_cmd23,
  eCMD_cmd24,
  eCMD_open0,//eCMD_cmd25,
  eCMD_close0,//eCMD_cmd26,
  eCMD_cmd27,
  
  
  //eCMD_wakeup_uni,
//   eCMD_default_open,
//   eCMD_default_close,
//   eCMD_changecolor,
//   eCMD_open0,
//   eCMD_close0,
  eCMD_cmd_no1,
};

#define NLU_CONTENT_SIZE 28
#define NLU_CONTENT_MAP_SIZE 32

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];


#define EN_SCORE_THRED_FOR_EVERY_CMD
static const float g_nlu_score_thred[]=
	{


	KWS_WAKEUP_SCORE_THRED  ,//eCMD_wakeup_uni,
	KWS_CMD_SCORE_THRED,//eCMD_default_open,// eCMD_cmd01,
	KWS_CMD_SCORE_THRED,//eCMD_cmd02,
	KWS_CMD_SCORE_THRED,//eCMD_cmd03,
	KWS_CMD_SCORE_THRED,//eCMD_default_close,//eCMD_cmd04,
	KWS_CMD_SCORE_THRED,//eCMD_cmd05,
	KWS_CMD_SCORE_THRED,//eCMD_cmd06,
	KWS_CMD_SCORE_THRED,//eCMD_cmd07,
	KWS_CMD_SCORE_THRED,//eCMD_cmd08,
	KWS_CMD_SCORE_THRED,//eCMD_cmd09,
	KWS_CMD_SCORE_THRED,//eCMD_cmd10,
	KWS_CMD_SCORE_THRED,//eCMD_cmd11,
	KWS_CMD_SCORE_THRED,//eCMD_cmd12,
	KWS_CMD_SCORE_THRED,//eCMD_cmd13,
	KWS_CMD_SCORE_THRED,//eCMD_cmd14,
	KWS_CMD_SCORE_THRED,//eCMD_cmd15,
	KWS_CMD_SCORE_THRED,//eCMD_cmd16,
	KWS_CMD_SCORE_THRED,//eCMD_cmd17,
	KWS_CMD_SCORE_THRED,//eCMD_cmd18,
	KWS_CMD_SCORE_THRED,//eCMD_changecolor,//eCMD_cmd19,
	KWS_CMD_SCORE_THRED,//eCMD_cmd20,
	KWS_CMD_SCORE_THRED,//eCMD_cmd21,
	KWS_CMD_SCORE_THRED,//eCMD_cmd22,
	KWS_CMD_SCORE_THRED,//eCMD_cmd23,
	KWS_CMD_SCORE_THRED,//eCMD_cmd24,
	KWS_CMD_SCORE_THRED,//eCMD_open0,//eCMD_cmd25,
	KWS_CMD_SCORE_THRED,//eCMD_close0,//eCMD_cmd26,
	KWS_CMD_SCORE_THRED,//eCMD_cmd27,		
	};

//
#define SET_TIME_OUT   15 //s
#define USER_open0  eCMD_open0
#define DIASBLE_eCMD_setlight

#define USER_WY_COLOR_REPEAT


/*
kwsA_switch_libS2



唤醒词：（灵敏度：中）
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



	KEY_NULL  ,//eCMD_wakeup_uni,
	MSG_LED_USB_NIGHT_ON,//eCMD_default_open,// eCMD_cmd01,
	KEY_NULL  ,//eCMD_cmd02,
	KEY_NULL  ,//eCMD_cmd03,
	MSG_LED_OFF,//eCMD_default_close,//eCMD_cmd04,
	KEY_NULL  ,//eCMD_cmd05,
	KEY_NULL  ,//eCMD_cmd06,
	KEY_NULL  ,//eCMD_cmd07,
	KEY_NULL  ,//eCMD_cmd08,
	KEY_NULL  ,//eCMD_cmd09,
	KEY_NULL  ,//eCMD_cmd10,
	KEY_NULL  ,//eCMD_cmd11,
	KEY_NULL  ,//eCMD_cmd12,
	KEY_NULL  ,//eCMD_cmd13,
	KEY_NULL  ,//eCMD_cmd14,
	KEY_NULL  ,//eCMD_cmd15,
	KEY_NULL  ,//eCMD_cmd16,
	KEY_NULL  ,//eCMD_cmd17,
	KEY_NULL  ,//eCMD_cmd18,
	MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor,//eCMD_cmd19,
	KEY_NULL  ,//eCMD_cmd20,
	KEY_NULL  ,//eCMD_cmd21,
	KEY_NULL  ,//eCMD_cmd22,
	KEY_NULL  ,//eCMD_cmd23,
	KEY_NULL  ,//eCMD_cmd24,
	MSG_LED_USB_NIGHT_ON,//eCMD_open0,//eCMD_cmd25,
	MSG_LED_OFF,//eCMD_close0,//eCMD_cmd26,
	KEY_NULL  ,//eCMD_cmd27,
	


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

