
#include "decode_mge.h"
#include "vfs.h"
#include "msg.h"
#include "clock.h"
#include "sound_mge.h"
#include "circular_buf.h"
#include "audio_dac.h"
#include "audio_dac_api.h"
#include "my_malloc.h"
#include "byle_cfg.h"
#include "byle_kws_app.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG     		"[Tone]"
#include "log.h"

//实现提示音播放
struct _tone_info {
    u8 busy;//忙状态
    u8 playing;//播放中
    u8 dec_ing;//解码中
    u8 channels;//声道数
    struct decoder_hdl d;
    struct decoder_plug_ops *p;
};
struct _tone_info tone_info;
cbuffer_t tone_dac_cbuf;

u8 *tone_dac_obuf=NULL;
#define tone_dac_obuf_size 256
sound_out_obj tone_sound;
extern const struct decoder_plug_ops decoder_plug_begin[];
extern const struct decoder_plug_ops decoder_plug_end[];

//解码中断
___interrupt
void tone_play_soft_isr(void)
{
    int err;
    bit_clr_swi(IRQ_SOFT0_IDX);
    err = tone_info.p->run(tone_info.d.decoder);
    if (err) {
        tone_info.dec_ing = 0;
    }
}
void tone_play_end(void)
{

//关闭软中断解码进程
HWI_Uninstall(IRQ_SOFT0_IDX);
//注销dac数据接口
unregist_dac_channel(&tone_sound);
//注销解码器
tone_info.p->release(tone_info.d.decoder);
tone_info.d.decoder = NULL;
tone_info.p = NULL;

if (tone_info.d.pvfile) {
	vfs_file_close(&tone_info.d.pvfile);
	tone_info.d.pvfile = NULL;
}
if (tone_info.d.pvfs) {
	vfs_fs_close(&tone_info.d.pvfs);
	tone_info.d.pvfs = NULL;
}

tone_info.playing = 0;
tone_info.dec_ing = 0;
tone_info.busy = 0;
#ifndef BYLE_KWS_ASR
my_free(tone_dac_obuf);
#endif
tone_dac_obuf=NULL;

          					  	 PA_MUTE();

}

//DAC取数回调函数
static void tone_play_kick(void)
{
    u32 data_size = cbuf_get_data_size((cbuffer_t *)tone_sound.p_obuf);
    //cbuf数据小于N时,启动软中断
    if (data_size < tone_dac_obuf_size/2) {
        if (tone_info.dec_ing) {
            //未解码完成时,触发软中断进行解码操作
            bit_set_swi(IRQ_SOFT0_IDX);
        } else if (data_size == 0) {
            //dac取数完成才算是播放完成
            tone_info.playing = 0;
            post_msg(MSG_TONE_PLAY_END);
	    tone_play_end();
        }
    }
}

//双声道合成单声道
static u32 tone_double2signal(void *input, void *output, u32 input_len)
{
    s32 t_sp;
    s16 *ibuf = (s16 *)input;
    s16 *obuf = (s16 *)output;
    for (u32 i = 0; i < input_len / 4; i++) {
        t_sp = *ibuf;
        ibuf += 1;
        t_sp += *ibuf;
        ibuf += 1;
        t_sp = t_sp / 2;
        if (t_sp > 32767) {
            t_sp = 32767;
        } else if (t_sp < -32768) {
            t_sp = -32768;
        }
        *obuf = (s16)t_sp;
        obuf += 1;
    }
    return (input_len / 2);
}

//单声道合成双声道
static u32 tone_single2double(void *input, void *output, u32 input_len)
{
    s16 *ibuf = (s16 *)input;
    s16 *obuf = (s16 *)output;
    for (u32 i = 0; i < (input_len / 2); i++) {
        obuf[2 * i + 0] = ibuf[i];
        obuf[2 * i + 1] = ibuf[i];
    }
    return (input_len * 2);
}

//解码器输出接口
static u32 tone_play_output(void *buff, u32 len)
{
    u32 tlen, slen;
    u32 wlen = len;
    s16 obuf[40];

    //输出到DAC的循环buffer
    if (tone_info.channels == DAC_TRACK_NUMBER) {
        wlen = cbuf_get_available_space((cbuffer_t *)tone_sound.p_obuf);
        wlen = (wlen > len) ? len : wlen;
        wlen = cbuf_write((cbuffer_t *)tone_sound.p_obuf, buff, wlen);
    } else if ((tone_info.channels == 1) && (DAC_TRACK_NUMBER == 2)) {
        //单声道合成双声道
        tlen = (len <= (sizeof(obuf) / 2)) ? len : (sizeof(obuf) / 2);
        tlen = tone_single2double(buff, obuf, tlen);

        //检查cbuf可写入长度
        slen = cbuf_get_available_space((cbuffer_t *)tone_sound.p_obuf);
        slen = (slen > tlen) ? tlen : slen;
        slen = cbuf_write((cbuffer_t *)tone_sound.p_obuf, obuf, slen);

        //消耗的长度为写入数据的一半
        wlen = slen / 2;
    } else if ((tone_info.channels == 2) && (DAC_TRACK_NUMBER == 1)) {
        //双声道合成单声道
        tlen = (len <= (sizeof(obuf) * 2)) ? len : (sizeof(obuf) * 2);
        tlen = tone_double2signal(buff, obuf, tlen);

        //检查cbuf可写入长度
        slen = cbuf_get_available_space((cbuffer_t *)tone_sound.p_obuf);
        slen = (slen > tlen) ? tlen : slen;
        slen = cbuf_write((cbuffer_t *)tone_sound.p_obuf, obuf, slen);

        //消耗的长度为写入数据的2倍
        wlen = slen * 2;
    }
    return wlen;
}

//启动提示音播放
int tone_play_start(const char *path)
{
    u32 err;
    struct stream_fmt fmt;

          					  	 PA_UNMUTE();

    tone_play_stop(0);
    tone_info.busy = 1;

    log_info("tone play: %s", path);

    err = vfs_mount(&tone_info.d.pvfs, (void *)NULL, (void *)NULL);
    if (err) {
        log_error("fii vfs mount : 0x%x\n", err);
        goto __start_err;
    }

    err = vfs_openbypath(tone_info.d.pvfs, &tone_info.d.pvfile, path);
    if (err) {
        log_error("fii vfs openbypath : 0x%x\n", err);
        goto __start_err;
    }

    tone_info.d.output = tone_play_output;
    tone_info.d.decoder = NULL;
    tone_info.p = NULL;

    //搜索解码器
    const struct decoder_plug_ops *p;
    for (p = decoder_plug_begin; p < decoder_plug_end; p++) {
        tone_info.d.decoder = p->init(&tone_info.d);
        err = p->ioctl(tone_info.d.decoder, NODE_IOC_GET_FMT, (int)&fmt);
        if (err == 0) {
            log_info("channel_num: %d, sample_rate: %d\n", fmt.channel_num, fmt.sample_rate);
            tone_info.p = (struct decoder_plug_ops *)p;
            tone_info.channels = fmt.channel_num;
            break;
        }
        p->release(tone_info.d.decoder);
        tone_info.d.decoder = NULL;
    }
    if (tone_info.p == NULL) {
        log_error("no decoder\n");
        goto __start_err;
    }
	

    //初始化DAC的cbuf空间,以及注册软中断
    dac_sr_set(fmt.sample_rate);
    HWI_Install(IRQ_SOFT0_IDX, (u32)tone_play_soft_isr, IRQ_DECODER_IP);

	if(tone_dac_obuf==NULL)
	{
	
        #ifdef BYLE_KWS_ASR
	tone_dac_obuf=get_mic_buf_addr();//my_malloc(256,MM_NONE);
	#else
	tone_dac_obuf=my_malloc(tone_dac_obuf_size,MM_NONE);
	#endif
	memset(tone_dac_obuf,0x00,tone_dac_obuf_size);
	
	}

	
    memset(&tone_sound, 0, sizeof(tone_sound));
    cbuf_init(&tone_dac_cbuf, &tone_dac_obuf[0], sizeof(tone_dac_obuf));
    tone_sound.p_obuf = &tone_dac_cbuf;
    tone_sound.kick = tone_play_kick;
    tone_info.playing = 1;
    tone_info.dec_ing = 1;

    //注册dac数据buffer接口
    regist_dac_channel(&tone_sound);
    //开启软中断进行解码
    bit_set_swi(IRQ_SOFT0_IDX);
    return 0;

__start_err:
    tone_info.busy = 0;
    if (tone_info.d.pvfile) {
        vfs_file_close(&tone_info.d.pvfile);
        tone_info.d.pvfile = NULL;
    }
    if (tone_info.d.pvfs) {
        vfs_fs_close(&tone_info.d.pvfs);
        tone_info.d.pvfs = NULL;
    }
    return -1;
}

//停止提示音播放
void tone_play_stop(u8 wait)
{
    if (tone_info.busy == 0) {
        return;
    }

    //等待dac把数据取走
    if (wait && tone_info.playing) {
        while (cbuf_get_data_size((cbuffer_t *)tone_sound.p_obuf) != 0) {
            asm("nop");
        }
        mdelay(10);
    }

    //关闭软中断解码进程
    HWI_Uninstall(IRQ_SOFT0_IDX);
    //注销dac数据接口
    unregist_dac_channel(&tone_sound);
    //注销解码器
    tone_info.p->release(tone_info.d.decoder);
    tone_info.d.decoder = NULL;
    tone_info.p = NULL;

    if (tone_info.d.pvfile) {
        vfs_file_close(&tone_info.d.pvfile);
        tone_info.d.pvfile = NULL;
    }
    if (tone_info.d.pvfs) {
        vfs_fs_close(&tone_info.d.pvfs);
        tone_info.d.pvfs = NULL;
    }
    tone_info.playing = 0;
    tone_info.dec_ing = 0;
    tone_info.busy = 0;


}




//获取提示音是否正在播放
u8 tone_play_is_busy(void)
{
    return tone_info.busy;
}

