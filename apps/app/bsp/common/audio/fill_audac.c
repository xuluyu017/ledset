/***********************************Jieli tech************************************************
  File : dac_api.c
  By   : liujie
  Email: liujie@zh-jieli.com
  date : 2019-1-14
********************************************************************************************/
#include "audac_basic.h"
#include "audio_dac_api.h"
#include "circular_buf.h"

#define LOG_TAG_CONST       NORM
/* #define LOG_TAG_CONST       OFF */
#define LOG_TAG             "[audio_dac_api]"
#include "log.h"

u32 fifo_dac_fill(u8 *buf, u32 len)
{
    u32 r_data_size;
    u32 ret = 0;
    memset(buf, 0, len);
    if (dac_mge.flag == 0) {
        goto __audio_null;
    }

    if (false == dac_cbuff_active(dac_mge.sound)) {
        goto __audio_null;
    }

    r_data_size = cbuf_get_data_size(dac_mge.sound->p_obuf);
    if (r_data_size >= len) {
        cbuf_read(dac_mge.sound->p_obuf, buf, len);
        ret = len;
    } else {
        cbuf_read(dac_mge.sound->p_obuf, buf, r_data_size);
        ret = r_data_size;
    }
    if (dac_mge.sound->kick) {
        dac_mge.sound->kick();
    }

__audio_null:
    return ret;
}


