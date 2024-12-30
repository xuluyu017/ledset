#include "typedef.h"
#include "key.h"
#include "msg.h"
#include "app_config.h"

#if KEY_AD_EN

#define AD_KEY_SHORT \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_SHORT_UP \
							/*00*/		MSG_KEY_SHORT_UP,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_LONG \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_HOLD \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_LONG_UP \
							/*00*/		MSG_KEY_LONG_UP,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_DOUBLE_CLICK \
							/*00*/		MSG_KEY_DOUBLE_CLICK,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define AD_KEY_TRIPLE_CLICK \
							/*00*/		MSG_KEY_TRIPLE_CLICK,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

const u16 adkey_msg_table[][AD_KEY_MAX_NUM] = {
    /*短按*/		{AD_KEY_SHORT},
    /*短按抬起*/	{AD_KEY_SHORT_UP},
    /*长按*/		{AD_KEY_LONG},
    /*连按*/		{AD_KEY_HOLD},
    /*长按抬起*/	{AD_KEY_LONG_UP},
#if KEY_DOUBLE_CLICK_EN
    /*双击*/		{AD_KEY_DOUBLE_CLICK},
    /*三击*/        {AD_KEY_TRIPLE_CLICK}
#endif
};


#endif

#if KEY_IO_EN

#define IO_KEY_SHORT \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_SHORT_UP \
							/*00*/		MSG_KEY_SHORT_UP,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_LONG \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_HOLD \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_LONG_UP \
							/*00*/		MSG_KEY_LONG_UP,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_DOUBLE_CLICK \
							/*00*/		MSG_KEY_DOUBLE_CLICK,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\

#define IO_KEY_TRIPLE_CLICK \
							/*00*/		MSG_KEY_TRIPLE_CLICK,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\


const u16 iokey_msg_table[][IO_KEY_MAX_NUM] = {
    /*短按*/		{IO_KEY_SHORT},
    /*短按抬起*/	{IO_KEY_SHORT_UP},
    /*长按*/		{IO_KEY_LONG},
    /*连按*/		{IO_KEY_HOLD},
    /*长按抬起*/	{IO_KEY_LONG_UP},
#if KEY_DOUBLE_CLICK_EN
    /*双击*/		{IO_KEY_DOUBLE_CLICK},
    /*三击*/        {IO_KEY_TRIPLE_CLICK}
#endif
};

#endif

u16 app_key_msg_filter(u8 key_status, u8 key_num, u8 key_type)
{
    u16 msg = NO_MSG;
    switch (key_type) {
#if KEY_AD_EN
    case KEY_TYPE_AD:
        msg = adkey_msg_table[key_status][key_num];
        break;
#endif
#if KEY_IO_EN
    case KEY_TYPE_IO:
        msg = iokey_msg_table[key_status][key_num];
        break;
#endif
    default:
        break;
    }
    return msg;
}

