#include "includes.h"
#include "init.h"
#include "device.h"
#include "clock.h"
#include "gpio.h"
#include "adc_api.h"
#include "audio_dac_api.h"
#include "audio_adc.h"
#include "audio.h"
#include "asm/power_interface.h"
#include "app_config.h"
#include "flash_init.h"
#include "byle_cfg.h"
#include "aec_uart_debug.h"
#include "key.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[init]"
#include "log.h"

struct os_argv_table {
    void *f;
    void *m;
    void *stack_free;
    void *stack_malloc;
    /* void *suppress_tick_and_sleep; */
    // void *exp_hook;
    /* void *i_hook; */
    // void *pchar;
    void *log_e;
    void *enter_critical;
    void *exit_critical;
    void *get_ms;
    void *cpu_task_sw;
    // int (*cpu_in_irq)(void);
    int (*cpu_irq_disabled)(void);
    void *jiffies_addr;
    void *jiffies_unit_addr;
};

#ifndef DAC_SR
#define DAC_SR 8000
#endif



void app_system_init(void)
{
    devices_init_api();

    adc_api_init();

    key_init();

    /* audio */
    audio_init();

#if TCFG_DAC_ENABLE
  dac_init_api(DAC_SR);
#endif
#ifdef MIC_ADC_ENABLE
    /* mic analog */
#ifdef EN_MIC_BIAS_IO_PORT
   MIC_BIAS_POWER(1);
#endif


   audio_adc_analog_open();
   audio_adc_digital_open(16000, 2);
#endif
#if TCFG_MIC_CAPLESS
	printf("TCFG_MIC_CAPLESS=%d",TCFG_MIC_CAPLESS);

    audio_adc_trim();
#endif
	


    flash_system_init();

}

