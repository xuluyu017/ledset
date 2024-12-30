#ifndef MSG_H
#define MSG_H

#include "typedef.h"
#include "uart.h"

enum {
//SYS_MSG_START_LINE
    MSG_0 = 0,
    MSG_1,
    MSG_2,
    MSG_3,
    MSG_4,
    MSG_5,
    MSG_6,

    MSG_KEY_SHORT_UP,
    MSG_KEY_LONG_UP,
    MSG_KEY_DOUBLE_CLICK,
    MSG_KEY_TRIPLE_CLICK,
    ///APP
    MSG_500MS,

    MSG_CFG_RX_DATA,

    MSG_SMART_00,
    MSG_SMART_01,
    MSG_SMART_02,
    MSG_SMART_03,
    MSG_SMART_04,
    MSG_SMART_05,
    MSG_SMART_06,
    MSG_SMART_07,
    MSG_SMART_08,
    MSG_SMART_09,
    MSG_SMART_10,
    MSG_SMART_11,
    MSG_SMART_12,
    MSG_SMART_13,
    MSG_SMART_14,
    MSG_SMART_15,
    MSG_SMART_16,
    MSG_SMART_17,
    MSG_SMART_18,
    MSG_SMART_19,

    MSG_SMART_20,
    MSG_SMART_21,
    MSG_SMART_22,
    MSG_SMART_23,
    MSG_SMART_24,
    MSG_SMART_25,
    MSG_SMART_26,
    MSG_SMART_27,
    MSG_SMART_28,
    MSG_SMART_29,

    MSG_SMART_30,
    MSG_SMART_31,
    MSG_SMART_32,
    MSG_SMART_33,
    MSG_SMART_34,

    MSG_TONE_PLAY_END,
    KEY_USER_START,
    MSG_COMMON_MAX,//common最大消息
    NO_MSG = 0x0fff,
};

enum {
    MSG_NO_ERROR = 0,
    MSG_NO_MSG = 0,
    MSG_EVENT_EXIST = -1,
    MSG_NOT_EVENT = -2,
    MSG_EVENT_PARAM_ERROR = -3,
    MSG_BUF_NOT_ENOUGH = -4,
    MSG_CBUF_ERROR = -5,
};

#ifndef MAX_POOL
#define MAX_POOL			    32//128
#endif
int get_msg(int *msg);
int post_msg(int msg);
void clear_all_message(void);
void message_init(void);


#endif



