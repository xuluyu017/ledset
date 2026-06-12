#ifndef __LIBKWS_BYLE_H__
#define __LIBKWS_BYLE_H__
#include "byle_kwsA_SDK_lib.h"


#define BYLE_ASR_TIMEOUT           UNI_ASR_TIMEOUT   

static const u16 Byle_kws_msg_table[]=
{ 
    KEY_NULL,               // 0: eCMD_wakeup_uni
    KEY_NULL,               // 1: eCMD_exitUni
    TURNON_LED,             // 2: eCMD_light_on
    TURNOFF_LED,            // 3: eCMD_light_off
    LIGHT_UP_LED,           // 4: eCMD_light_up
    LIGHT_DOWN_LED,         // 5: eCMD_light_down
    SET_COLOR_WHITE_LED,    // 6: eCMD_set_white
    DELAY_MODE_LED,         // 7: eCMD_delay_off
    SET_COLOR_WARM_LED,     // 8: eCMD_set_warm
    SET_COLOR_NEUTRAL_LED,  // 9: eCMD_set_neutral
};


#endif
