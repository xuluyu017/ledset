#ifndef __AU_ADC_H__
#define __AU_ADC_H__

#include "typedef.h"
#include "audio.h"

#define AU_ADC_24BIT         BIT(20)
#define AU_ADC_UDE           BIT(11)
#define AU_ADC_CIC_48M       BIT(9)
#define AU_ADC_FIFO_REST     BIT(8)
#define AU_ADC_PND           BIT(7)
#define AU_ADC_PND_CLR       BIT(6)
#define AU_ADC_IE            BIT(5)
#define AU_ADC_EN            BIT(4)

#define AUDIO_ADC_SP_BITS     16
#define AUDIO_ADC_PACKET_SIZE 192 //点数,除3能整除 除16能整除
#define AUDIO_ADC_SP_SIZE     (AUDIO_ADC_SP_BITS / 8)

void audio_adc_isr(void);
u32 audio_adc_get_sr(void);
void audio_adc_set_sr(u32 sr);

u32 audio_adc_digital_open(u32 sr, u32 throw);
void audio_adc_digital_close(void);

void audio_adc_analog_open();
void audio_adc_analog_close();

u16 adc_core_get_mic_bias_res(void);
//省隔直电容的校准
void audio_adc_trim(void);

#define MACRO_MIC_INPUT_PA7             0
#define MACRO_MIC_INPUT_PA6             1
#define MACRO_MIC_INPUT_DIFF_PA7_PA6    2
#define MACRO_MIC_INPUT_PA8             3
#define MACRO_MIC_INPUT_PA5             4


typedef enum __AUDIO_MICLDO_VS {
    AUMIC_2v3 = 0,
    AUMIC_2v5 = 1,
    AUMIC_2v7 = 2,
    AUMIC_2v9 = 3,
    AUMIC_3v0 = 4,
    AUMIC_3v1 = 5,
    AUMIC_3v2 = 6,
    AUMIC_3v3 = 7,
} AUDIO_MICLDO_VS;
extern AUDIO_MICLDO_VS const audio_adc_mic_ldo_vs;

typedef enum __AUDIO_MICBIAS_RS {
    AUMIC_0k5 = 1,
    AUMIC_1k0 = 2,
    AUMIC_1k5 = 3,
    AUMIC_2k0 = 4,
    AUMIC_2k5 = 5,
    AUMIC_3k0 = 6,
    AUMIC_3k5 = 7,
    AUMIC_4k0 = 8,
    AUMIC_4k5 = 9,
    AUMIC_5k0 = 0xa,
    AUMIC_6k0 = 0xb,
    AUMIC_7k0 = 0xc,
    AUMIC_8k0 = 0xd,
    AUMIC_9k0 = 0xe,
    AUMIC_10k = 0xf,
} AUDIO_MICBIAS_RS;
extern AUDIO_MICBIAS_RS const audio_adc_mic_bias_rs;

typedef enum __AUDIO_MICPGA_G {
    AUMIC_0db = 0,
    AUMIC_3db = 1,
    AUMIC_9db = 2,
    AUMIC_15db = 3,
    AUMIC_21db = 4,
} AUDIO_MICPGA_G;


#endif
