#include "if_decoder_ctrl.h"
#include "decode_mge.h"
#include "vfs.h"
#include "app_config.h"
#include "byle_cfg.h"
#include "byle_kws_app.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG     		"[DEC-G729]"
#include "log.h"
#if (defined(TCFG_DEC_WTG_ENABLE) && TCFG_DEC_WTG_ENABLE)
extern decoder_ops_t *get_g729dec_ops(void);

struct g729_decoder {
    u8 start;
    void *priv;
    decoder_ops_t *dec_ops;
    struct if_decoder_io io;
    void *dec_buff;
};

struct g729_decoder *__g729=NULL;
struct g729_decoder g729_hdl  AT(.g729_mem);
u8 *g729_dec_buff=NULL;

#define  g729_dec_size 1096
//u8 g729_dec_buff[g729_dec_size] AT(.g729_mem) ALIGNED(4);

static int g729_decoder_input(void *priv, u32 addr, void *buf, int len, u8 type)
{
    u32 rlen;
    if (priv == NULL) {
        return 0;
    }

    struct g729_decoder *g729 = (struct g729_decoder *)priv;
    struct decoder_hdl *hdl = (struct decoder_hdl *)g729->priv;

    vfs_seek(hdl->pvfile, addr, SEEK_SET);
    rlen = vfs_read(hdl->pvfile, buf, len);
    return rlen;
}

/*
 * 检查数据是否完毕
 */
static int g729_decoder_check_buf(void *priv, u32 addr, void *buf)
{
    u32 rlen;
    struct g729_decoder *g729 = (struct g729_decoder *)priv;
    rlen = g729_decoder_input(g729->priv, addr, buf, 512, 0);
    return rlen;
}

/*
 * 解码后的数据输出
 */
static u32 g729_decoder_output(void *priv, void *data, int len)
{
    if (priv == NULL) {
        return 0;
    }
    u32 wlen;
    struct g729_decoder *g729 = (struct g729_decoder *)priv;
    struct decoder_hdl *hdl = (struct decoder_hdl *)g729->priv;
    wlen = hdl->output(data, len);
    return wlen;
}

/*
 * 获取文件长度
 */
static u32 g729_decoder_get_lslen(void *priv)
{
    if (priv == NULL) {
        return 0;
    }
    u32 flen = 0;
    struct g729_decoder *g729 = (struct g729_decoder *)priv;
    struct decoder_hdl *hdl = (struct decoder_hdl *)g729->priv;
    fs_get_fsize(hdl->pvfile, &flen);
    return flen;
}

static void *g729_decoder_open(struct decoder_hdl *priv)
{
    u32 need_buf_len;
    decoder_ops_t *dec_ops;
    struct g729_decoder *g729 = __g729;

    /* if(__g729==NULL)
     {
     #ifdef BYLE_KWS_ASR
     	__g729= get_mic_buf_addr()+256+g729_dec_size;
     #else
     	 __g729= my_malloc(sizeof(*g729),MM_NONE);
     #endif
     }*/

    dec_ops = get_g729dec_ops();
    if (!dec_ops) {
        return NULL;
    }

    need_buf_len = dec_ops->need_dcbuf_size();
    if (need_buf_len >g729_dec_size) {
        log_error("buf space is not enough!");
        return NULL;
    }


   g729 = &g729_hdl;
    memset(g729, 0, sizeof(*g729));
    g729->priv          = priv;
    g729->dec_ops       = dec_ops;
    g729->io.priv       = g729;
    g729->io.input      = g729_decoder_input;
    g729->io.output     = g729_decoder_output;
    g729->io.check_buf  = g729_decoder_check_buf;
    g729->io.get_lslen  = g729_decoder_get_lslen;
    return g729;
}

static int g729_decoder_start(void *_g729)
{
    u32 err = 0;
    AUDIO_DECODE_PARA mode;
    struct g729_decoder *g729 = (struct g729_decoder *)_g729;
    ASSERT(g729, "handle null");

    if (g729->start) {
        return 0;
    }
    if(g729_dec_buff==NULL)
    	{
#ifdef BYLE_KWS_ASR
        g729_dec_buff= get_mic_buf_addr()+256;
#else
 g729_dec_buff= my_malloc(g729_dec_size,MM_NONE);
#endif
	memset(g729_dec_buff,0x00,g729_dec_size);
    	}
    /*
     * 创建解码工作区BUF
     */
    u32 need_buf_len = g729->dec_ops->need_dcbuf_size();
    if (need_buf_len > g729_dec_size) {
        ASSERT(0, "buf space is not enough!");
    }
    g729->dec_buff = (void *)g729_dec_buff;

    /*
     * 打开解码器
     */
    err = g729->dec_ops->open(g729->dec_buff, &g729->io, NULL);
    if (err) {
        log_error("g729 dec open err\n");
        goto __err;
    }

    /*
     * 设置工作模式
     */
    mode.mode = 1;
    g729->dec_ops->dec_config(g729->dec_buff, SET_DECODE_MODE, (u8 *)&mode);

    /*
     * 格式检查
     */
    if (g729->dec_ops->format_check) {
        err = g729->dec_ops->format_check(g729->dec_buff);
        if (err) {
            log_debug("g729 format_check err!\n");
            goto __err;
        }
    }
    return 0;

__err:
    g729->dec_buff = NULL;
#ifndef BYLE_KWS_ASR
//my_free(__g729);
#endif	
    __g729 = NULL;
#ifndef BYLE_KWS_ASR
my_free(tone_dac_obuf);
#endif	

    return err;

}

static int g729_decoder_get_fmt(void *_g729, struct stream_fmt *fmt)
{
    int err;
    dec_inf_t *dec_inf;
    struct g729_decoder *g729 = (struct g729_decoder *)_g729;
    ASSERT(g729, "handle null");

    if (g729->start == 0) {
        err = g729_decoder_start(g729);
        if (err) {
            return err;
        }
    }

    if (g729->dec_ops->get_dec_inf) {
        dec_inf = g729->dec_ops->get_dec_inf(g729->dec_buff);
        if (dec_inf) {
            fmt->sample_rate = dec_inf->sr;
            fmt->channel_num = dec_inf->nch;
            return 0;
        }
    }

    log_error("get fmt err!");
    return -EPERM;
}

static int g729_decoder_run(void *_g729)
{
    int err;
    struct g729_decoder *g729 = (struct g729_decoder *)_g729;
    ASSERT(g729, "handle null");
    if (g729->start == 0) {
        err = g729_decoder_start(_g729);
        if (err) {
            return err;
        }
        g729->start = 1;
    }
    err = g729->dec_ops->run(g729->dec_buff, 0);
    return err;
}

static int g729_decoder_ioctl(void *_g729, int cmd, int arg)
{
    int err = 0;
    struct g729_decoder *g729 = (struct g729_decoder *)_g729;
    ASSERT(g729, "handle null");
    switch (cmd) {
    case NODE_IOC_SET_FMT:
        break;
    case NODE_IOC_GET_FMT:
        err = g729_decoder_get_fmt(_g729, (struct stream_fmt *)arg);
        break;
    case NODE_IOC_START:
        break;
    case NODE_IOC_SUSPEND:
    case NODE_IOC_STOP:
        g729->start = 0;
        break;
    }
    return err;
}

static void g729_decoder_close(void *_g729)
{
    struct g729_decoder *g729 = (struct g729_decoder *)_g729;
    if (g729 == NULL) {
        return;
    }
    g729->dec_buff = NULL;
    __g729 = NULL;
	g729_dec_buff=NULL;
}

REGISTER_DECODER_PLUG(g729_dec_plug) = {
    .coding_type    = AUDIO_CODING_G729,
    .init           = g729_decoder_open,
    .run            = g729_decoder_run,
    .ioctl          = g729_decoder_ioctl,
    .release        = g729_decoder_close,
};

#endif

