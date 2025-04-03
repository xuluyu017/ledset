#ifndef __LIBKWS_UTILS_H__
#define __LIBKWS_UTILS_H__

#define UNI_ASR_TIMEOUT           15    //ASR timeout Xs

#define UNI_MIC_ARRY_TYPE_LINER   0
#define UNI_MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1

#define KWS_WAKEUP_SCORE_THRED    (0.44)
#define KWS_SLEEP_SCORE_THRED    (1.57)
#define KWS_CMD_SCORE_THRED       (-1.72)
#define KWS_HASHTABLE_SIZE        (84)

/*---MIC config---*/
#define ACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR
#define UNI_MIC_ARRY_NUM          1

#define LOCAL_TONE_MAX_VOLUME      "[101]"
#define LOCAL_TONE_MIN_VOLUME      "[102]"
#define DEFAULT_PCM_WAKEUP        "[-1]"
#define DEFAULT_PCM_ASR_TIMEOUT   "[-1]"

typedef struct {
  unsigned int  key_word_hash_code; /* 存放识别词汇对应的hashcode */
  unsigned char nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char          *hash_collision_orginal_str; /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */
} uni_nlu_content_mapping_t;

enum {
  eCMD_wakeup_uni,
  eCMD_FAN_ON,
  eCMD_FAN_OFF,
  eCMD_POWER_ON,
  eCMD_POWER_OFF,
  eCMD_SWING_ON,
  eCMD_SWING_OFF,
  eCMD_Windspeed1,
  eCMD_Windspeed2,
  eCMD_Windspeed3,
  eCMD_Windspeed4,
  eCMD_Windspeed5,
  eCMD_Windspeed6,
  eCMD_default_open,
  eCMD_default_close,
  eCMD_open0,
  eCMD_close0,
  eCMD_open1,
  eCMD_close1,
  eCMD_changemode,
  eCMD_changemode1,
  eCMD_changecolor,
  eCMD_coolmode,
  eCMD_warmmode,
  eCMD_sunmode,
  eCMD_sunmode1,
  eCMD_brightthelight,
  eCMD_dimthelight,
  eCMD_coolmode2,
  eCMD_warmmode2,
  eCMD_LIGHT_ON,
  eCMD_LIGHT_OFF,
  eCMD_LIGHT2_ON,
  eCMD_LIGHT2_OFF,
  eCMD_LIGHT3_ON,
  eCMD_LIGHT3_OFF,
  eCMD_minlight,
  eCMD_maxlight,
};

#define NLU_CONTENT_SIZE 38
#define NLU_CONTENT_MAP_SIZE 39

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];


 
static const char *const tone_table[] = {
"/dir_wtg/1.wtg",
};

#endif
