#ifndef __decode_mge_h_
#define __decode_mge_h_

#include "typedef.h"
#include "errno-base.h"
/*
 *******************************************************************
 *						Codec Definitions
 *******************************************************************
 */
#define AUDIO_CODING_UNKNOW         0x00000000
#define AUDIO_CODING_MP3            0x00000001
#define AUDIO_CODING_WMA            0x00000002
#define AUDIO_CODING_WAV            0x00000004
#define AUDIO_CODING_JLA_LW         0x00000008
#define AUDIO_CODING_SBC            0x00000010
#define AUDIO_CODING_MSBC           0x00000020
#define AUDIO_CODING_G729           0x00000040
#define AUDIO_CODING_CVSD           0x00000080
#define AUDIO_CODING_PCM            0x00000100
#define AUDIO_CODING_AAC            0x00000200
#define AUDIO_CODING_MTY            0x00000400
#define AUDIO_CODING_FLAC           0x00000800
#define AUDIO_CODING_APE            0x00001000
#define AUDIO_CODING_M4A            0x00002000
#define AUDIO_CODING_AMR            0x00004000
#define AUDIO_CODING_DTS            0x00008000
#define AUDIO_CODING_APTX           0x00010000
#define AUDIO_CODING_LDAC           0x00020000
#define AUDIO_CODING_G726           0x00040000
#define AUDIO_CODING_MIDI           0x00080000
#define AUDIO_CODING_OPUS           0x00100000
#define AUDIO_CODING_SPEEX          0x00200000
#define AUDIO_CODING_LC3            0x00400000
#define AUDIO_CODING_WTGV2          0x01000000
#define AUDIO_CODING_ALAC           0x02000000
#define AUDIO_CODING_SINE           0x04000000
#define AUDIO_CODING_F2A            0x08000000
#define AUDIO_CODING_AIFF           0x10000000
#define AUDIO_CODING_JLA            0x20000000
#define AUDIO_CODING_OGG            0x40000000
#define AUDIO_CODING_LHDC           0x80000000

#define NODE_IOC_OPEN_IPORT         0x00010000
#define NODE_IOC_CLOSE_IPORT        0x00010001
#define NODE_IOC_OPEN_OPORT         0x00010002
#define NODE_IOC_CLOSE_OPORT        0x00010003

#define NODE_IOC_SET_FILE           0x00020000
#define NODE_IOC_OPEN_FILE          0x00020001
#define NODE_IOC_GET_FMT            0x00020002
#define NODE_IOC_SET_FMT            0x00020003
#define NODE_IOC_CLR_FMT            0x00020004
#define NODE_IOC_GET_DELAY          0x00020005      // 获取缓存数据的延时
#define NODE_IOC_SET_SCENE          0x00020006      // 设置数据流的场景
#define NODE_IOC_SET_CHANNEL        0x00020007
#define NODE_IOC_NEGOTIATE          0x00020008      // 各个节点参数协商
#define NODE_IOC_FLUSH_OUT          0x00020009
#define NODE_IOC_SET_TIME_STAMP     0x0002000a
#define NODE_IOC_SYNCTS             0x0002000b
#define NODE_IOC_NODE_CONFIG        0x0002000c
#define NODE_IOC_SET_PRIV_FMT 		0x0002000d		//设置节点私有参数
#define NODE_IOC_NAME_MATCH         0x0002000e
#define NODE_IOC_SET_PARAM          0x0002000f
#define NODE_IOC_GET_PARAM          0x00020010
#define NODE_IOC_DECODER_PP		    0x00020011		//解码暂停再重开
#define NODE_IOC_DECODER_FF	    	0x00020012		//快进
#define NODE_IOC_DECODER_FR    		0x00020013		//快退
#define NODE_IOC_DECODER_REPEAT     0x00020014      //无缝循环播放
#define NODE_IOC_DECODER_DEST_PLAY  0x00020015      //跳到指定位置播放
#define NODE_IOC_GET_CUR_TIME  		0x00020016      //获取音乐播放当前时间
#define NODE_IOC_GET_TOTAL_TIME  	0x00020017      //获取音乐播放总时间
#define NODE_IOC_GET_BP  			0x00020018      //获取解码断点信息
#define NODE_IOC_SET_BP         	0x00020019      //设置解码断点信息
#define NODE_IOC_SET_FILE_LEN       0x0002001a      //设置解码文件长度信息
#define NODE_IOC_SET_BP_A           0x0002001b      //设置复读A点
#define NODE_IOC_SET_BP_B           0x0002001c      //设置复读B点
#define NODE_IOC_SET_AB_REPEAT      0x0002001d      //设置AB点复读模式
#define NODE_IOC_GET_ODEV_CACHE     0x0002001e      //获取输出设备的缓存采样数
#define NODE_IOC_SET_BTADDR         0x0002001f
#define NODE_IOC_SET_ENC_FMT        0x00020020
#define NODE_IOC_GET_ENC_FMT        0x00020021
#define NODE_IOC_GET_HEAD_INFO      0x00020022      //获取编码的头文件信息
#define NODE_IOC_SET_TASK           0x00020023
#define NODE_IOC_FSEEK              0x00020024
#define NODE_IOC_SET_BIT_WIDE       0x00020025
#define NODE_IOC_FLOW_CTRL_ENABLE   0x00020026
#define NODE_IOC_GET_BTADDR         0x00020027

#define NODE_IOC_START              0x00040001
#define NODE_IOC_PAUSE              0x00040002
#define NODE_IOC_SUSPEND            0x00040004
#define NODE_IOC_STOP               0x00040008

struct stream_fmt {
    u8 channel_num;
    u32 sample_rate;
};

struct decoder_hdl {
    void *decoder;
    void *pvfs;//文件系统句柄
    void *pvfile;//文件句柄
    u32(*output)(void *data, u32 len);
};

struct decoder_plug_ops {
    int coding_type;
    void *(*init)(struct decoder_hdl *dec_hdl);
    int (*run)(void *);
    int (*ioctl)(void *, int, int);
    void (*release)(void *);
};

extern const struct decoder_plug_ops decoder_plug_begin[];
extern const struct decoder_plug_ops decoder_plug_end[];
#define REGISTER_DECODER_PLUG(plug) \
    const struct decoder_plug_ops plug sec_used(.decoder_plug)

int tone_play_start(const char *path);
void tone_play_stop(u8 wait);
u8 tone_play_is_busy(void);

#endif


