#ifndef __AU_ADC_API_H__
#define __AU_ADC_API_H__

#include "typedef.h"

void audio_adc_isr(void);
void fill_audio_adc_fill(u8 *buf, u32 len);
bool regist_audio_adc_channel(void *psound, void *kick);
bool unregist_audio_adc_channel(void *psound);
void set_audio_adc_run( u8 flag);

#endif
