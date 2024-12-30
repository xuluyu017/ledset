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
  eCMD_A1,
  eCMD_A2,
  eCMD_A3,
  eCMD_A4,
  eCMD_A5,
  eCMD_A6,
  eCMD_A7,
  eCMD_A8,
  eCMD_A9,
  eCMD_A10,
  eCMD_A11,
  eCMD_A12,
  eCMD_A13,
  eCMD_A14,
  eCMD_A15,
  eCMD_A16,
  eCMD_A17,
  eCMD_A18,
  eCMD_A19,
  eCMD_A20,
  eCMD_A21,
  eCMD_A22,
  eCMD_A23,
  eCMD_A24,
  eCMD_A25,
  eCMD_A26,
  eCMD_A27,
  eCMD_A28,
  eCMD_A29,
  eCMD_A30,
  eCMD_A31,
  eCMD_A32,
  eCMD_A33,
  eCMD_A34,
  eCMD_A35,
  eCMD_A36,
  eCMD_A37,
  eCMD_A38,
  eCMD_A39,
  eCMD_A40,
  eCMD_A41,
  eCMD_A42,
  eCMD_A43,
  eCMD_A44,
  eCMD_A45,
  eCMD_A46,
  eCMD_A47,
  eCMD_A48,
  eCMD_A50,
  eCMD_A51,
  eCMD_A52,
  eCMD_A53,
  eCMD_A54,
  eCMD_A55,
  eCMD_A56,
  eCMD_A57,
  eCMD_A58,
  eCMD_A59,
  eCMD_A60,
  eCMD_A61,
  eCMD_A62,
  eCMD_A63,
  eCMD_A64,
  eCMD_A65,
  eCMD_A66,
  eCMD_A67,
  eCMD_A68,
  eCMD_A69,
  eCMD_A70,
  eCMD_A71,
  eCMD_A72,
  eCMD_A73,
  eCMD_A74,
  eCMD_A75,
  eCMD_A76,
  eCMD_A77,
  eCMD_A78,
  eCMD_A79,
  eCMD_A80,
  eCMD_A81,
};

#define NLU_CONTENT_SIZE 81
#define NLU_CONTENT_MAP_SIZE 84

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];
#endif
