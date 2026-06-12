#ifndef __LIBKWS_UTILS_H__
#define __LIBKWS_UTILS_H__

#define UNI_ASR_TIMEOUT           10    //ASR timeout Xs

#define UNI_MIC_ARRY_TYPE_LINER   0
#define UNI_MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1

#define KWS_WAKEUP_SCORE_THRED    (0.1)
#define KWS_SLEEP_SCORE_THRED    (1.4)
#define KWS_CMD_SCORE_THRED       (-0.7)
#define KWS_HASHTABLE_SIZE        (11)

/*---MIC config---*/
#define ACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR
#define UNI_MIC_ARRY_NUM          1

#define LOCAL_TONE_MAX_VOLUME      "[101]"
#define LOCAL_TONE_MIN_VOLUME      "[102]"
#define DEFAULT_PCM_WAKEUP        "[-1]"
#define DEFAULT_PCM_ASR_TIMEOUT   "[103]"
#define DEFAULT_PCM_SLEEP         "[103]"

typedef struct {
  unsigned int  key_word_hash_code; /* 存放识别词汇对应的hashcode */
  unsigned char nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char          *hash_collision_orginal_str; /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */
} uni_nlu_content_mapping_t;

enum {
  eCMD_wakeup_uni,
  eCMD_exitUni,
  eCMD_light_on,
  eCMD_light_off,
  eCMD_light_up,
  eCMD_light_down,
  eCMD_set_white,
  eCMD_delay_off,
  eCMD_set_warm,
  eCMD_set_neutral,
};

#define NLU_CONTENT_SIZE 10
#define NLU_CONTENT_MAP_SIZE 11

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];
#endif
