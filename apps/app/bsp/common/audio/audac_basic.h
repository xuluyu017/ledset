#ifndef __AUDAC_BASIC_H__
#define __AUDAC_BASIC_H__

#include "typedef.h"
#include "audio_dac.h"
#include "sound_mge.h"
#include "app_config.h"

typedef struct _DAC_MANAGE {
    sound_out_obj *sound;
    u8 flag;
} DAC_MANAGE;


extern DAC_MANAGE dac_mge;


#endif

