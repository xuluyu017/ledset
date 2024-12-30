#ifndef __LIBKWS_UTILS_H__
#define __LIBKWS_UTILS_H__

#define UNI_ASR_TIMEOUT           10    //ASR timeout Xs

#define UNI_MIC_ARRY_TYPE_LINER   0
#define UNI_MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1

#define KWS_WAKEUP_SCORE_THRED    (-1.54)
#define KWS_SLEEP_SCORE_THRED    (-1.06)
#define KWS_CMD_SCORE_THRED       (-1.5)
#define KWS_HASHTABLE_SIZE        (67)

/*---MIC config---*/
#define ACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR
#define UNI_MIC_ARRY_NUM          1

#define LOCAL_TONE_MAX_VOLUME      "[101]"
#define LOCAL_TONE_MIN_VOLUME      "[102]"
#define DEFAULT_PCM_WAKEUP        "[103]"
#define DEFAULT_PCM_ASR_TIMEOUT   "[104]"
#define DEFAULT_PCM_WELCOME       "[105]"

typedef struct {
  unsigned int  key_word_hash_code; /* 存放识别词汇对应的hashcode */
  unsigned char nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char          *hash_collision_orginal_str; /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */
} uni_nlu_content_mapping_t;

enum {
  eCMD_wakeup_uni,
  eCMD_default_open,
  eCMD_default_close,
  eCMD_brightthelight,
  eCMD_dimthelight,
  eCMD_changecolor,
  eCMD_musicmode,
  eCMD_ColorfulMode,
  eCMD_BRAND_DEFAULT,
  eCMD_ACTION_CLOSE,
  eCMD_ACTION_OPEN,
  eCMD_MODE_AUTO,
  eCMD_MODE_COOL,
  eCMD_MODE_CHUSHI,
  eCMD_MODE_TONGF,
  eCMD_MODE_HOT,
  eCMD_ACTION_TEMP_16,
  eCMD_ACTION_TEMP_17,
  eCMD_ACTION_TEMP_18,
  eCMD_ACTION_TEMP_19,
  eCMD_ACTION_TEMP_20,
  eCMD_ACTION_TEMP_21,
  eCMD_ACTION_TEMP_22,
  eCMD_ACTION_TEMP_23,
  eCMD_ACTION_TEMP_24,
  eCMD_ACTION_TEMP_25,
  eCMD_ACTION_TEMP_26,
  eCMD_ACTION_TEMP_27,
  eCMD_ACTION_TEMP_28,
  eCMD_ACTION_TEMP_29,
  eCMD_ACTION_TEMP_30,
  eCMD_WIND_AUTO,
  eCMD_WIND_MIN,
  eCMD_WIND_MAX,
  eCMD_WIND_UD_AUTO,
  eCMD_WIND_UD_STOP,
  eCMD_WIND_LR_AUTO,
  eCMD_WIND_LR_STOP,
  eCMD_TIME_1H,
  eCMD_TIME_2H,
  eCMD_TIME_4H,
  eCMD_TIME_8H,
  eCMD_TIME_CANCEL,
  eCMD_BRAND_SET,
  eCMD_BRAND_MATCH,
  eCMD_BRAND_MATCH_ST,
  eCMD_BRAND_AUX,
  eCMD_BRAND_ZHIGAO,
  eCMD_BRAND_SONGXIA,
  eCMD_BRAND_GREE,
  eCMD_BRAND_HAIXIN,
  eCMD_BRAND_HAIER,
  eCMD_BRAND_KELON,
  eCMD_BRAND_MIDEA,
  eCMD_BRAND_CHANGHONG,
  eCMD_BRAND_RILI,
  eCMD_BRAND_DAJIN,
  eCMD_BRAND_XIAOMI,
  eCMD_BRAND_SANSUNG,
  eCMD_BRAND_XTE,
  eCMD_BRAND_KANGJIA,
  eCMD_BRAND_GELANSHI,
  eCMD_BRAND_CHUANGWEI,
  eCMD_BRAND_DONGZHI,
  eCMD_BRAND_LG,
  eCMD_BRAND_TCL,
};

#define NLU_CONTENT_SIZE 66
#define NLU_CONTENT_MAP_SIZE 67

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];
#endif
