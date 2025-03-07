/*********************************************************************************************
    *   Filename        : main.c

    *   Description     :

    *   Author          :

    *   Email           :

    *   Last modifiled  :

    *   Copyright:(c)JIELI  2011-2017  @ , All Rights Reserved.
*********************************************************************************************/

#pragma bss_seg(".main.data.bss")
#pragma data_seg(".main.data")
#pragma const_seg(".main.text.const")
#pragma code_seg(".main.text")
#pragma str_literal_override(".main.text.const")

#include "config.h"
#include "common.h"
#include "gpio.h"
#include "clock.h"
#include "app_config.h"
#include "init.h"
#include "init_app.h"
#include "msg.h"
#include "device.h"
#include "asm/power_interface.h"
#include "power_api.h"
#include "asm/debug.h"
#include "wdt.h"
#include "msg.h"
#include "icache.h"
#include "boot_efuse.h"
// #include "byle_user.h"
// #include "byle_cfg.h"
// #include "byle_kws_app.h"
#include "mcpwm.h"
#define LOG_TAG_CONST       MAIN
#define LOG_TAG             "[main]"
#include "log.h"

#if ICACHE_RAM_TO_RAM_ENABLE
extern int cache_ram_addr[];
extern int cache_ram_begin[];
extern int cache_ram_size[];
#endif

AT(.common)
void change_icache(void)
{
#if ICACHE_RAM_TO_RAM_ENABLE
    IcuSetWayNum(4 - ICACHE_RAM_TO_RAM / 4096);
    IcuFlushinvAll();
    memcpy((void *)cache_ram_addr, (void *)cache_ram_begin, (unsigned long)cache_ram_size);
#endif
}


__attribute__((noreturn))
void c_main(int cfg_addr)
{
    change_icache();
    q32DSP(0)->PMU_CON1 &= ~BIT(8);
    irq_init();
    local_irq_disable();
    local_irq_enable();

    p33_fast_access(P3_PR_PWR, BIT(3), CONFIG_MCLR_EN);
    gpio_longpress_pin0_reset_config(PINR_RESET_IO, PINR_RESET_LVL, PINR_PRESS_TIME, PINR_RESET_RELASE, PINR_PROTECT_EN);
#if TCFG_DAC_ENABLE
    JL_ADDA->DAA_CON2 |= BIT(15) | BIT(20);//避免DAC与IO合绑问题
#endif

    port_init();

    efuse_init();
//   user_io_init();

    log_init(TCFG_UART_BAUDRATE);

    pll_sel(TCFG_PLL_SEL, TCFG_PLL_DIV, TCFG_HSB_DIV);

    dump_clock_info();

    wdt_init(WDT_8S);

    log_info("time & date %s %s \n  OTP c_main\n", __TIME__, __DATE__);

    dump_efuse();

    reset_source_dump();

    sys_power_init();

    system_init();
//    user_power_on_init();

#if PMU_TRIM_ENABLE
    pmu_trim(0, 0);
#endif
 //app();
//    user_app(); 

extern void mcpwm_test(void);
mcpwm_test();
 
   log_info(" in loop\r\n");

    while (1) {
        printf("run\n");

pwm_set(8000);

            delay(100000000);
        wdt_clear();
    }
}


