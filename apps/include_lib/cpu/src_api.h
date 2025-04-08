#ifndef __SRC_API_H__
#define __SRC_API_H__
#include "typedef.h"
#include "src_para.h"

void src_mode_init(void);
void *src_api(void *obuf, SRC_PARA_API *p_src_para, void **ppsound, int(*output)(void *priv, void *data, int len), void *sdata);
void *link_src_sound(void *p_sound_out, void *p_dac_cbuf, void **pp_effect, u32 insample, u32 outsample, u8 ch, int(*output)(void *priv, void *data, int len), void *sdata);

#endif

