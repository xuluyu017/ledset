#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_msg.h"

#define BYLE_ASR_TIMEOUT           15    //ASR timeout Xs

#define MIC_ARRY_TYPE_LINER   0
#define MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1
#ifndef KWS_WAKEUP_SCORE_THRED
#define KWS_WAKEUP_SCORE_THRED  (-1.47)// (-1.47)  //  (-0.47) 	// (-1.47) // (-1.83)				 		整体提高灵敏度-2           还原 默认 (-1.47)
#endif
#define KWS_SLEEP_SCORE_THRED    (-0.6)
#ifndef KWS_CMD_SCORE_THRED
#define KWS_CMD_SCORE_THRED   (-1.31)//(-0.81)// (-2.81)	//   (-1.81)	//(-0.81)	// (-1.83)			 	整体提高灵敏度-2           还原 默认 (-1.47)
#endif
#define KWS_HASHTABLE_SIZE   (18)//(44) //(40)  //V6:  (36) // V5:  (37)  // V4: (40)  // V2：  (41)	// (52)		 

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
eCMD_changecolor,
eCMD_changecolor1,
eCMD_default_open,
eCMD_default_close,
eCMD_musicmode,
eCMD_Breathingmode,
eCMD_ColorfulMode,
eCMD_yellow,
eCMD_white,
eCMD_green,
eCMD_blue,
eCMD_red,
eCMD_purple,
eCMD_pink,


  eCMD_open0,
  eCMD_close0,
  eCMD_cmd_no1,
  eCMD_dimthelight,
  eCMD_brightthelight,
  eCMD_open2,
  eCMD_close2,
  eCMD_close3,
  eCMD_setlight,
  eCMD_sleep10min,
  eCMD_sleep30min,
  eCMD_sleep1hour,
  eCMD_cancelsleepoff,
  eCMD_MaximumBright,
  eCMD_MinimumBrightn,
  eCMD_MinBright,
  eCMD_cyan,
  eCMD_musicmode1,
  eCMD_opennight,
  eCMD_closenight,
  eCMD_coolmode,// 		    eCMD_setcolor,
  eCMD_warmmode,		 
  eCMD_sunmode,		 
  eCMD_sunmode1,		 
  eCMD_nightlight,		 
  eCMD_setlight1,			 
  eCMD_sleep30s,
  eCMD_dimthelight1,		 
  eCMD_brightthelight1,		 
  eCMD_open1,		 
  eCMD_close1,		 
  eCMD_changecolor2,		 
  eCMD_changecolor3,		 

};

#define NLU_CONTENT_SIZE 15//34
#define NLU_CONTENT_MAP_SIZE  18//44 //40  //V6: 36 //V5: 37 // V4: 40  //V2：  41	 

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];



#define SET_TIME_OUT   15 //s
// #define USER_open0  eCMD_open0

// #define USER_open2   eCMD_open1   

// #define USER_open3   eCMD_open2      

// #define USER_open4   eCMD_opennight      
#ifndef  ENABLE_eCMD_setlight
#define DIASBLE_eCMD_setlight   
#endif


#ifndef KWS_CMD_MODE_AS_WAKEUP				 
#define USER_WY_COLOR_REPEAT
#endif			 




#define  EN_SCORE_THRED_FOR_EVERY_CMD


static const float  g_nlu_score_thred[]= {

KWS_WAKEUP_SCORE_THRED,//eCMD_wakeup_uni,
KWS_CMD_SCORE_THRED,//eCMD_changecolor,
KWS_CMD_SCORE_THRED,//eCMD_changecolor1,
KWS_CMD_SCORE_THRED,//eCMD_default_open,
KWS_CMD_SCORE_THRED,//eCMD_default_close,
KWS_CMD_SCORE_THRED,//eCMD_musicmode,
KWS_CMD_SCORE_THRED,//eCMD_Breathingmode,
KWS_CMD_SCORE_THRED,//eCMD_ColorfulMode,
KWS_CMD_SCORE_THRED,//eCMD_yellow,
KWS_CMD_SCORE_THRED,//eCMD_white,
KWS_CMD_SCORE_THRED,//eCMD_green,
KWS_CMD_SCORE_THRED,//eCMD_blue,
KWS_CMD_SCORE_THRED,//eCMD_red,
KWS_CMD_SCORE_THRED,//eCMD_purple,
KWS_CMD_SCORE_THRED,//eCMD_pink,



};

  #ifdef VOID_WAKEUP_TABLE
  #define VOID_WAKEUP_TABLE_SIZE  4
static const char* void_wake_table[VOID_WAKEUP_TABLE_SIZE]=
{
#ifndef Hey_Sloth_WAKEUP_EN
"hey sloth",
#endif
#ifndef Hey_Panda_WAKEUP_EN
"hey panda",
#endif
#ifndef Hey_Cappy_WAKEUP_EN
"hey cappy",
#endif
#ifndef Hey_night_light_WAKEUP_EN
"hey night light",
#endif
};
  #endif
/*

唤醒词：
Hey Sloth
Hey Panda
Hey Cappy
hey night light
命令词：
changecolor=Change color
changecolor1=color changing
defaultopen=Turn on the light
defaultclose=Turn off the light
musicmode=Music mode
Breathingmode=steady on
ColorfulMode=slow pulse
yellow=yellow Light
white=white Light
green=green Light
blue=blue Light
red=red Light
purple=purple Light
pink=pink Light




*/

	static const u16 Byle_kws_msg_table[]=
	{

KEY_NULL,//eCMD_wakeup_uni,
MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor,
MSG_LED_COLOR_DISP_MODE_SEL_NO_WY,//eCMD_changecolor1,
MSG_LED_USB_NIGHT_ON,//eCMD_default_open,
MSG_LED_OFF,//eCMD_default_close,
MSG_LED_SENOR_MODE,//eCMD_musicmode,
MSG_LED_COLOR_CHAGE_FAD,//eCMD_Breathingmode,
MSG_LED_COLOR_FULL_CHANGE,//eCMD_ColorfulMode,
MSG_LED_YELLOW,//eCMD_yellow,
MSG_LED_WHITE,//eCMD_white,
MSG_LED_GREEG,//eCMD_green,
MSG_LED_BLUE,//eCMD_blue,
MSG_LED_REG,//eCMD_red,
MSG_LED_PURPLE,//eCMD_purple,
MSG_LED_PINK,//eCMD_pink,

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
