/***********************************Jieli tech************************************************
  File : dac_api.c
  By   : liujie
  Email: liujie@zh-jieli.com
  date : 2019-1-14
********************************************************************************************/
#include "audio_dac_api.h"
#include "audac_basic.h"
#include "string.h"
#include "uart.h"
#include "config.h"
#include "audio.h"
#include "clock.h"
#include "circular_buf.h"
#include "audio_adc.h"
#include "app_config.h"

/* #define LOG_TAG_CONST       NORM */
#define LOG_TAG_CONST       OFF
#define LOG_TAG             "[audio_dac_api]"
#include "log.h"

#define B_DAC_MUTE		BIT(0)
#define B_DAC_FADE_EN   BIT(1)
#define B_DAC_FADE_OUT  BIT(2)

const u16 vol_tab[] = {
    0	,
    164	,
    191	,
    223	,
    260	,
    303	,
    353	,
    412	,
    480	,
    559	,
    652	,
    760	,
    887	,
    1034	,
    1205	,
    1405	,
    1638	,
    1910	,
    2227	,
    2597	,
    3028	,
    3530	,
    4115	,
    4798	,
    5594	,
    6523	,
    7605	,
    8867	,
    10338	,
    12053	,
    14052	,
    16384
};


#define MAX_VOL_LEVEL ((sizeof(vol_tab)/2) - 1)
#define MAX_PHY_VOL   vol_tab[MAX_VOL_LEVEL]

DAC_MANAGE dac_mge;

#define DAC_PACKET_SIZE     (256)//样点
#if ICACHE_RAM_TO_RAM_ENABLE

u16 audio_dac_buf[DAC_PACKET_SIZE * DAC_TRACK_NUMBER] ALIGNED(4) AT(.usr_data) ;//双声道样点buffer
#else
u16 audio_dac_buf[DAC_PACKET_SIZE * DAC_TRACK_NUMBER] ALIGNED(4)  ;//双声道样点buffer

#endif	
#define DAC_MAX_SP          (sizeof(audio_dac_buf) / (DAC_TRACK_NUMBER * (AUDAC_BIT_WIDE / 8)))
#define DAC_SP_SIZE         (DAC_TRACK_NUMBER * (AUDAC_BIT_WIDE / 8))
#define DAC_PNS_SIZE        64//样点数小于多少时起中断

_OTP_CONST_  DAC_CTRL_HDL audio_dac_ops = {
    .buf           = audio_dac_buf,
    .sp_total      = DAC_MAX_SP,
    .sp_size       = DAC_SP_SIZE,
    .pns           = DAC_PNS_SIZE,
    .chl           = DAC_TRACK_NUMBER,
};

void dac_init_api(u32 sr)
{
    stereo_dac_vol(31, 31);
    dac_digital_open(sr, (void *)&audio_dac_ops);
    udelay(2000);
    audio_dac_analog_open(0);
    audio_dac_analog_vol(TCFG_DAC_ANA_VOL, TCFG_DAC_ANA_VOL);
}

void dac_off_api(void)
{
    dac_digital_close();
}

u8 stereo_dac_vol(u8 vol_l, u8 vol_r)
{
    u16 vol_l_phy, vol_r_phy;
    if (vol_l > MAX_VOL_LEVEL) {
        vol_l = MAX_VOL_LEVEL;
    }
    if (vol_r > MAX_VOL_LEVEL) {
        vol_r = MAX_VOL_LEVEL;
    }
    vol_l_phy = vol_tab[vol_l];
    vol_r_phy = vol_tab[vol_r];
    dac_digital_vol(vol_l_phy, vol_r_phy);
    return vol_l;
}

bool regist_dac_channel(void *psound)
{
    if (dac_mge.flag) {
        return false;
    }
    dac_mge.sound = psound;
    dac_mge.flag = 1;
    return true;
}

bool unregist_dac_channel(void *psound)
{
    if (dac_mge.flag) {
        if (dac_mge.sound == psound) {
            local_irq_disable();
            dac_mge.flag = 0;
            dac_mge.sound = 0;
            local_irq_enable();
        }
    }
    return true;
}

bool dac_cbuff_active(void *sound_hld)
{
    sound_out_obj *psound = sound_hld;
    if (psound->enable & (B_DEC_PAUSE | B_DEC_FIRST)) {
        if (cbuf_get_data_size(psound->p_obuf) >= (cbuf_get_space(psound->p_obuf) / 2)) {
            psound->enable &= ~B_DEC_FIRST;
        }
        return false;
    } else {
        return true;
    }
}


