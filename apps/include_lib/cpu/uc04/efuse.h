#ifndef  __EFUSE_H__
#define  __EFUSE_H__

#include "cpu.h"

extern struct efuse_page0_t efuse_page0;
extern struct efuse_page1_t efuse_page1;

void efuse_save_otp_cfg_2_ram(void);
bool is_boot_otp();
u32 efuse_get_vbat_trim_420(void);
u32 efuse_get_vbat_trim_435(void);
u32 efuse_get_charge_cur_trim(void);
u32 efuse_get_lighting_mode(void);
u32 efuse_get_lighting_2_typec(void);
u32 efuse_get_audio_rdac_trim(void);
u32 efuse_get_audio_vbg_trim(void);

u32 efuse_get_lrc_pll_nr();
u32 efuse_get_wvdd_trim();

#endif  /* EFUSE_H */
