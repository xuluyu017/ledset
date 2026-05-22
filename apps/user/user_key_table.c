

#include "app_config.h"

#include "typedef.h"
#include "key.h"
#include "byle_msg.h"

#if KEY_IR_EN
#define IRFF00_SHORT_UP		\
                                /*00*/    MSG_PP,\
							    /*01*/    NO_MSG,\
								/*02*/    MSG_MUTE,\
								/*03*/    MSG_PP,\
								/*04*/    MSG_PREV_FILE,\
								/*05*/    MSG_NEXT_FILE,\
								/*06*/    NO_MSG,\
								/*07*/    MSG_VOL_DOWN,\
								/*08*/    MSG_VOL_UP,\
								/*09*/    MSG_0,\
                                                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    MSG_1,\
								/*13*/    MSG_2,\
								/*14*/    MSG_3,\
								/*15*/    MSG_4,\
								/*16*/    MSG_5,\
								/*17*/    MSG_6,\
								/*18*/    MSG_7,\
								/*19*/    MSG_8,\
								/*20*/    MSG_9

#define IRFF00_LONG		\
                                /*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    MSG_FR,\
								/*05*/    MSG_FF,\
								/*06*/    NO_MSG,\
								/*07*/    MSG_VOL_DOWN,\
								/*08*/    MSG_VOL_UP,\
								/*09*/    NO_MSG,\
                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
								/*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG

#define IRFF00_HOLD		\
                                /*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    MSG_FR,\
								/*05*/    MSG_FF,\
								/*06*/    NO_MSG,\
								/*07*/    MSG_VOL_DOWN,\
								/*08*/    MSG_VOL_UP,\
								/*09*/    NO_MSG,\
                                /*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
								/*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG


#define IRFF00_LONG_UP	\
                                /*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    NO_MSG,\
								/*05*/    NO_MSG,\
								/*06*/    NO_MSG,\
								/*07*/    NO_MSG,\
								/*08*/    NO_MSG,\
								/*09*/    NO_MSG,\
								/*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
                                /*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG

#if (KEY_DOUBLE_CLICK_EN)
#define IRFF00_DOUBLE_KICK	\
                                /*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    NO_MSG,\
								/*05*/    NO_MSG,\
								/*06*/    NO_MSG,\
								/*07*/    NO_MSG,\
								/*08*/    NO_MSG,\
								/*09*/    NO_MSG,\
								/*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
                                /*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG

#define IRFF00_TRIPLE_KICK	\
                                /*00*/    NO_MSG,\
                                /*01*/    NO_MSG,\
								/*02*/    NO_MSG,\
								/*03*/    NO_MSG,\
								/*04*/    NO_MSG,\
								/*05*/    NO_MSG,\
								/*06*/    NO_MSG,\
								/*07*/    NO_MSG,\
								/*08*/    NO_MSG,\
								/*09*/    NO_MSG,\
								/*10*/    NO_MSG,\
								/*11*/    NO_MSG,\
								/*12*/    NO_MSG,\
								/*13*/    NO_MSG,\
                                /*14*/    NO_MSG,\
								/*15*/    NO_MSG,\
								/*16*/    NO_MSG,\
								/*17*/    NO_MSG,\
								/*18*/    NO_MSG,\
								/*19*/    NO_MSG,\
								/*20*/    NO_MSG
#endif

#define IRFF00_SHORT		\
                                /*00*/   NO_MSG,\
							    /*01*/   NO_MSG,\
								/*02*/   NO_MSG,\
								/*03*/   NO_MSG,\
								/*04*/   NO_MSG,\
								/*05*/   NO_MSG,\
								/*06*/   NO_MSG,\
								/*07*/   NO_MSG,\
								/*08*/   NO_MSG,\
								/*09*/   NO_MSG,\
                                /*10*/   NO_MSG,\
								/*11*/   NO_MSG,\
								/*12*/   NO_MSG,\
								/*13*/   NO_MSG,\
								/*14*/   NO_MSG,\
								/*15*/   NO_MSG,\
								/*16*/   NO_MSG,\
								/*17*/   NO_MSG,\
								/*18*/   NO_MSG,\
								/*19*/   NO_MSG,\
								/*20*/   NO_MSG

const u16 irff00_msg_table[][IR_KEY_MAX_NUM] = {			//Music模式下的遥控转换表
    /*短按*/	    {IRFF00_SHORT},
    /*短按抬起*/	{IRFF00_SHORT_UP},
    /*长按*/		{IRFF00_LONG},
    /*连按*/		{IRFF00_HOLD},
    /*长按抬起*/	{IRFF00_LONG_UP},
#if (KEY_DOUBLE_CLICK_EN)
    /*双击*/		{IRFF00_DOUBLE_KICK},
    /*三击*/		{IRFF00_TRIPLE_KICK},
#endif
};
#endif

#if KEY_IO_EN
#define IOKEY_SHORT_UP \
							/*00*/		KEY_PWM_CONTROL,\
							/*01*/		MSG_1,\
							/*02*/		MSG_2,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define IOKEY_LONG \
							/*00*/		MSG_0,\
							/*01*/		MSG_1,\
							/*02*/		MSG_2,\
							/*03*/		MSG_3,\
							/*04*/		MSG_1,\
							/*05*/		MSG_2,\
							/*06*/		MSG_3,\
							/*07*/		MSG_4,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define IOKEY_HOLD \
							/*00*/		KEY_PWM_LONG,\
							/*01*/		MSG_1,\
							/*02*/		MSG_2,\
							/*03*/		MSG_3,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define IOKEY_LONG_UP \
							/*00*/		KEY_PRESS_UP,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#if (KEY_DOUBLE_CLICK_EN)
#define IOKEY_DOUBLE_KICK \
							/*00*/		KEY_DOUBLE_CLICK,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define IOKEY_TRIPLE_KICK \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#endif

#define IOKEY_SHORT \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

const u16 iokey_msg_table[][IO_KEY_MAX_NUM] = {
    /*短按*/		{IOKEY_SHORT},
    /*短按抬起*/	{IOKEY_SHORT_UP},
    /*长按*/		{IOKEY_LONG},
    /*连按*/		{IOKEY_HOLD},
    /*长按抬起*/	{IOKEY_LONG_UP},
#if (KEY_DOUBLE_CLICK_EN)
    /*双击*/		{IOKEY_DOUBLE_KICK},
    /*三击*/		{IOKEY_TRIPLE_KICK},
#endif
};
#endif

#if KEY_AD_EN
#ifdef US320_WY_V7_BAT_KEY_SHAKE				//BZ
#define ADKEY_SHORT_UP \
							/*00*/		MSG_SHAKE_CHANGE_COLOR,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		MSG_SHAKE_CHANGE_COLOR,\
							/*05*/		MSG_SHAKE_CHANGE_COLOR,\
							/*06*/		MSG_SHAKE_CHANGE_COLOR,\
							/*07*/		MSG_SHAKE_CHANGE_COLOR,\
							/*08*/		MSG_SHAKE_CHANGE_COLOR,\
							/*09*/		MSG_SHAKE_CHANGE_COLOR,\

#define ADKEY_LONG \
							/*00*/		MSG_LED_OFF,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_HOLD \
							/*00*/		MSG_SHAKE_OFF,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_LONG_UP \
							/*00*/		MSG_SHAKE_OFF,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#if (KEY_DOUBLE_CLICK_EN)
#define ADKEY_DOUBLE_KICK \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_TRIPLE_KICK \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#endif

#define ADKEY_SHORT \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

const u16 adkey_msg_table[][AD_KEY_MAX_NUM] = {
    /*短按*/		{ADKEY_SHORT},
    /*短按抬起*/	{ADKEY_SHORT_UP},
    /*长按*/		{ADKEY_LONG},
    /*连按*/		{ADKEY_HOLD},
    /*长按抬起*/	{ADKEY_LONG_UP},
#if (KEY_DOUBLE_CLICK_EN)
    /*双击*/		{ADKEY_DOUBLE_KICK},
    /*三击*/		{ADKEY_TRIPLE_KICK},
#endif
};
#else    //BZ
#define ADKEY_SHORT_UP \
							/*00*/		MSG_PP,\
							/*01*/		MSG_PREV_FILE,\
							/*02*/		MSG_NEXT_FILE,\
							/*03*/		MSG_VOL_DOWN,\
							/*04*/		MSG_VOL_UP,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_LONG \
							/*00*/		MSG_POWER_OFF,\
							/*01*/		MSG_FR,\
							/*02*/		MSG_FF,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_HOLD \
							/*00*/		NO_MSG,\
							/*01*/		MSG_FR,\
							/*02*/		MSG_FF,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_LONG_UP \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#if (KEY_DOUBLE_CLICK_EN)
#define ADKEY_DOUBLE_KICK \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#define ADKEY_TRIPLE_KICK \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

#endif

#define ADKEY_SHORT \
							/*00*/		NO_MSG,\
							/*01*/		NO_MSG,\
							/*02*/		NO_MSG,\
							/*03*/		NO_MSG,\
							/*04*/		NO_MSG,\
							/*05*/		NO_MSG,\
							/*06*/		NO_MSG,\
							/*07*/		NO_MSG,\
							/*08*/		NO_MSG,\
							/*09*/		NO_MSG,\

const u16 adkey_msg_table[][AD_KEY_MAX_NUM] = {
    /*短按*/		{ADKEY_SHORT},
    /*短按抬起*/	{ADKEY_SHORT_UP},
    /*长按*/		{ADKEY_LONG},
    /*连按*/		{ADKEY_HOLD},
    /*长按抬起*/	{ADKEY_LONG_UP},
#if (KEY_DOUBLE_CLICK_EN)
    /*双击*/		{ADKEY_DOUBLE_KICK},
    /*三击*/		{ADKEY_TRIPLE_KICK},
#endif
};
#endif    //BZ
#endif

u16 user_key_msg_filter(u8 key_status, u8 key_num, u8 key_type)
{	
    u16 msg = NO_MSG;
    switch (key_type) {
#if KEY_IO_EN
    case KEY_TYPE_IO:
        msg = iokey_msg_table[key_status][key_num];
        break;
#endif
#if KEY_AD_EN
    case KEY_TYPE_AD:
        msg = adkey_msg_table[key_status][key_num];
        break;
#endif
#if KEY_IR_EN
    case KEY_TYPE_IR:
        msg = irff00_msg_table[key_status][key_num];
        break;
#endif
    default:
        break;
    }

    return msg;
}

