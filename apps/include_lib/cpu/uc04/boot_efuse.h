#ifndef  __BOOT_EFUSE_H__
#define  __BOOT_EFUSE_H__

#include "typedef.h"
#include "asm/power/pmu_flag.h"

extern struct efuse_page1_t efuse_page1;
extern struct efuse_page0_t efuse_page0;
extern void efuse_init(void);
extern void dump_efuse(void);

u8 boot_efuse_get_audio_vbg(void);
u8 boot_efuse_get_mvbg_lev(void);
u8 boot_efuse_get_wvdd_lev(void);

#endif

