#ifndef __AUDIO_DAC_API_H__
#define __AUDIO_DAC_API_H__
#include "typedef.h"
#include "audio_dac.h"

void dac_init_api(u32 sr);
void dac_off_api(void);
u8 stereo_dac_vol(u8 vol_l, u8 vol_r);
bool regist_dac_channel(void *sound);
bool unregist_dac_channel(void *dec_hld);

bool dac_cbuff_active(void *sound_hld);

#endif


