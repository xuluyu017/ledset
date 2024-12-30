/***********************************Jieli tech************************************************
  File : adc_api.c
  By   : liujie
  Email: liujie@zh-jieli.com
  date : 2019-1-14
********************************************************************************************/
#include "circular_buf.h"
#include "string.h"
#include "uart.h"
#include "config.h"
#include "audio.h"
#include "audio_adc.h"
#include "audio_adc_api.h"
#include "sound_mge.h"
#include "app_config.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[normal]"
#include "log.h"

typedef struct _AUDIO_ADC_MANAGE {
    sound_out_obj *sound;
    void (*kick)(void *);
    u8 flag;
} AUDIO_ADC_MANAGE;

AUDIO_ADC_MANAGE audio_adc_mge;

void set_audio_adc_run( u8 flag)
{
	audio_adc_mge.flag=flag;
	printf( " set_audio_adc_run=%d", audio_adc_mge.flag);

}



void fill_audio_adc_fill(u8 *buf, u32 len)
{

    u32 wlen;

    if (0 == audio_adc_mge.flag) {
        return;
    }
    if (0 == (audio_adc_mge.sound->enable & B_DEC_RUN_EN)) {
        return;
    }

    wlen = cbuf_write(audio_adc_mge.sound->p_obuf, buf, len);
    if (wlen != len) {
        log_char('*');
        cbuf_clear(audio_adc_mge.sound->p_obuf);
        return;
    }

    if (NULL !=  audio_adc_mge.kick) {
        audio_adc_mge.kick(audio_adc_mge.sound);
    }

}

bool regist_audio_adc_channel(void *psound, void *kick)
{
    if (audio_adc_mge.flag) {
        return false;
    }
    audio_adc_mge.sound = psound;
    audio_adc_mge.kick = kick;
    audio_adc_mge.flag = 1;
    return true;
}

bool unregist_audio_adc_channel(void *psound)
{
    u8 i;
    sound_out_obj *ps = psound;
    if (audio_adc_mge.flag) {
        if (audio_adc_mge.sound == psound) {
            local_irq_disable();
            audio_adc_mge.flag = 0;
            audio_adc_mge.sound = 0;
            audio_adc_mge.kick = NULL;
            local_irq_enable();
        }
    }
    return true;
}

