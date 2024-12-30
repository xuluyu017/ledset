#ifndef __AUDIO_DAC_H__
#define __AUDIO_DAC_H__

#include "typedef.h"
#include "audio.h"

/********* define for DAC_CON0**********************************/
#define A_DAC_PND          BIT(7)
#define A_DAC_CLR_PND      BIT(6)
#define A_DAC_IE           BIT(5)
#define A_DAC_EN           BIT(4)
#define A_DAC_DFIFOR       BIT(8)

#define DAC_TRACK_NUMBER   1
#define AUDAC_BIT_WIDE     16

typedef struct _DAC_CTRL_HDL {
    void *buf;
    u32  pns;     //dac中断门槛
    u16  sp_total;
    u8   sp_size;
    u8   chl;
} DAC_CTRL_HDL;

typedef enum __AUDIO_DAC_ANA_VOL {
    AUDAC_M14db = 0,
    AUDAC_M12db = 1,
    AUDAC_M10db = 2,
    AUDAC_M8db = 3,
    AUDAC_M6db = 4,
    AUDAC_M4db = 5,
    AUDAC_M2db = 6,
    AUDAC_0db = 7,
} AUDIO_DAC_ANA_VOL;

typedef enum __AUDIO_VCMO_VOLTAGE {
    AUDAC_VCMO_1v2 = 0,
    AUDAC_VCMO_1v3 = 1,
    AUDAC_VCMO_1v4 = 2,
    AUDAC_VCMO_1v5 = 3,
} AUDIO_VCMO_VOLTAGE;

void dac_digital_open(u32 sr, void *_pos);
void dac_digital_close(void);
void audio_dac_isr(void);
u32 dac_sr_read(void);
void dac_digital_vol(u16 dac_l, u16 dac_r);
void dac_sr_set(u32 sr);
void audio_dac_analog_vol(AUDIO_DAC_ANA_VOL vol_l, AUDIO_DAC_ANA_VOL vol_r);
u32 fifo_dac_fill(u8 *buf, u32 len);
void audio_dac_analog_open(u8 mode);
void audio_dac_analog_close(void);

void dac_core_analog_mute(void);
void dac_core_analog_unmute(void);
void dac_core_set_pns100n_en(u8 en);
void adc_core_set_bias_en(u8 en);

#endif
