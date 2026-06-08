#ifndef __LIBKWS_UTILS_H__
#define __LIBKWS_UTILS_H__

#define UNI_ASR_TIMEOUT           30    //ASR timeout Xs

#define UNI_MIC_ARRY_TYPE_LINER   0
#define UNI_MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1

#define KWS_WAKEUP_SCORE_THRED    (0.1)
#define KWS_SLEEP_SCORE_THRED    (1.4)
#define KWS_CMD_SCORE_THRED       (-2.69)
#define KWS_HASHTABLE_SIZE        (31)

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
  eCMD_TurnOn,
  eCMD_CabinetOn,
  eCMD_WardrobeOn,
  eCMD_ShoeCabinetOn,
  eCMD_WineCabinetOn,
  eCMD_TurnOff,
  eCMD_CabinetOff,
  eCMD_WardrobeOff,
  eCMD_ShoeCabinetOff,
  eCMD_WineCabinetOff,
  eCMD_LightUp,
  eCMD_CabinetUp,
  eCMD_WardrobeUp,
  eCMD_ShoeCabinetUp,
  eCMD_WineCabinetUp,
  eCMD_LightDown,
  eCMD_CabinetDown,
  eCMD_WardrobeDown,
  eCMD_ShoeCabinetDown,
  eCMD_WineCabinetDown,
  eCMD_SleepMode,
  eCMD_DelayTurnOff,
  eCMD_lighttingMode,
  eCMD_LowPowerMode,
  eCMD_SetLight,
  eCMD_SetNormal,
  eCMD_SetCabinet,
  eCMD_SetWardrobe,
  eCMD_SetShoeCabinet,
  eCMD_SetWineCabinet,
};

#define NLU_CONTENT_SIZE 31
#define NLU_CONTENT_MAP_SIZE 31

extern const char* g_nlu_content_str[NLU_CONTENT_SIZE][2];
extern const uni_nlu_content_mapping_t g_nlu_content_mapping[NLU_CONTENT_MAP_SIZE];
#endif
