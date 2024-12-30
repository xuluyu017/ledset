#include "config.h"
#include "common.h"
#include "gpio.h"
#include "crc16.h"
#include "circular_buf.h"
#include "app_config.h"
#include "aec_uart_debug.h"
#include "errno-base.h"
#include "uart_dev.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[uart_ext]"
#include "log.h"

#if ((defined TCFG_DATA_EXTERN_ENABLE) && (TCFG_DATA_EXTERN_ENABLE))

#define PCM_CHANN				1		        //PCM数据通道数,一般发送512byte,DMA模式需要2.7ms
#define PCM_SINGLE_SIZE			160//128   //PCM每路数据的大小
#define PCM_PACKET_SIZE			(PCM_SINGLE_SIZE * PCM_CHANN + 2) //2个byte crc
#define PACKET_MAX              2

struct aec_uart_s {
    volatile u8 busy;
    volatile u8 read_index;
    volatile u8 write_index;
    volatile u8 write_full;
    short *uartSendBuf[PACKET_MAX];
    u8 pcm_channel;
    u16 pcm_single_size;
    u16 pcm_packet_size;
};
short packet_buf[PACKET_MAX][PCM_PACKET_SIZE];
struct aec_uart_s aec_uart_src;
struct aec_uart_s *aec_uart;
extern u16 chip_crc16(void *ptr, u32  len); //CRC校验

static int aec_uart_dev_open(void)
{
    gpio_direction_output(TCFG_DATA_EXTERN_PORT, 1);
    gpio_set_fun_output_port(TCFG_DATA_EXTERN_PORT, FO_UART1_TX, 1, 1);
    JL_UART1->BAUD = ((48000000L / 2000000) / 4 - 1);
    JL_UART1->CON0 = BIT(13) | BIT(12) | BIT(0);
    JL_UART1->CON0 |= BIT(13);  //清Tx pending
    gpio_set_hd(TCFG_DATA_EXTERN_PORT, 1);
    gpio_set_hd0(TCFG_DATA_EXTERN_PORT, 1);
    log_info("success\n");
    return 0;
}

/*关闭串口*/
static void aec_uart_dev_close(void)
{
    log_info("aec_uart_dev_close() ...\n");
    JL_UART1->CON0 = BIT(13) | BIT(12) | BIT(10);
    gpio_set_hd(TCFG_DATA_EXTERN_PORT, 0);
    gpio_set_hd0(TCFG_DATA_EXTERN_PORT, 0);
}
/*串口发送数据*/
static void aec_uart_dev_write(u8 *buf, u32 len)
{
    aec_uart->busy = 1;
    JL_UART1->CON0 |= BIT(13);
    JL_UART1->TXADR = (u32)buf;
    JL_UART1->TXCNT = len;
}
/*********************************************************
*                  aec_uart_run
* Description: 数据写卡发送任务
* Arguments  : None
* Return     : None
* Note(s)    : None.
*********************************************************/
void aec_uart_run(void)
{
    u8 tmp_r_index;
    if (aec_uart) {

        if (JL_UART1->CON0 & BIT(15)) {
            aec_uart->busy = 0;
        }

        if ((aec_uart->busy == 0) && (aec_uart->write_index != aec_uart->read_index)) {
            log_char('w');
            tmp_r_index = aec_uart->read_index;
            aec_uart->busy = 0;
            //计算CRC16
            aec_uart->uartSendBuf[tmp_r_index][PCM_PACKET_SIZE - 2] = chip_crc16(aec_uart->uartSendBuf[tmp_r_index], (PCM_PACKET_SIZE - 2) * sizeof(short));
            aec_uart_dev_write((u8 *)aec_uart->uartSendBuf[tmp_r_index], PCM_PACKET_SIZE * sizeof(short));
            tmp_r_index++;
            if (tmp_r_index >= PACKET_MAX) {
                tmp_r_index = 0;
            }
            aec_uart->read_index = tmp_r_index;
        }
    }
};

/*********************************************************
*                  aec_uart_open
* Description: 打开数据写卡接口
* Arguments  : nch 总通道数，single_size 单个通道的数据大小
* Return     : 0 成功 其他 失败
* Note(s)    : None.
*********************************************************/
int aec_uart_open(u8 nch, u16 single_size)
{
    u8 i;
    struct aec_uart_s *aec_uart_t = &aec_uart_src;

    log_info("aec_uart_open ch:%d, single_size:%d\n", nch, single_size);

    //PCM数据通道数,一般发送512byte,DMA模式需要2.7ms
    aec_uart_t->pcm_channel = nch;
    //PCM每路数据点数的大小
    aec_uart_t->pcm_single_size = single_size >> 1;
    //一包数据的大小，16位，2个byte crc
    aec_uart_t->pcm_packet_size = aec_uart_t->pcm_single_size * aec_uart_t->pcm_channel + 2;
    for (i = 0; i < PACKET_MAX; i++) {
        aec_uart_t->uartSendBuf[i] = &packet_buf[i][0];
    }

    if (aec_uart_dev_open() != 0) {
        log_error("[err] uart open err\n");
        goto __error_exit;
    }

    log_info("aec_uart_init ok\n");
    aec_uart = aec_uart_t;
    return 0;
__error_exit:
    for (i = 0; i < PACKET_MAX; i++) {
        aec_uart_t->uartSendBuf[i] = NULL;
    }
    aec_uart_t = NULL;
    return -EINVAL;

}

/*********************************************************
*                  aec_uart_init
* Description: 数据写卡初始化
* Arguments  : None.
* Return     : 0 成功 其他 失败
* Note(s)    : None.
*********************************************************/
int aec_uart_init(void)
{
    return aec_uart_open(PCM_CHANN, PCM_SINGLE_SIZE << 1);
}

/*********************************************************
*                  aec_uart_fill
* Description: 填写对应通道的数据
* Arguments  : ch 通道号，buf 数据地址，size 数据大小
* Return     : 0 成功 其他 失败
* Note(s)    : None.
*********************************************************/
int aec_uart_fill(u8 ch, void *buf, u16 size)
{
    u8 tmp_w_index;
    u16 points = size >> 1;
    if (aec_uart && (ch < aec_uart->pcm_channel)) {
        if (aec_uart->write_full) {
            return 0;
        }

        if (points != aec_uart->pcm_single_size) {
            log_error("aec ch %d buf clear 0:%d\n", ch, size);
            memset(buf, 0, aec_uart->pcm_single_size * sizeof(short));
        }

        tmp_w_index = aec_uart->write_index + 1;
        if (tmp_w_index >= PACKET_MAX) {
            tmp_w_index = 0;
        }
        if (tmp_w_index == aec_uart->read_index) {
            aec_uart->write_full = 1;
            return 0;
        }
        log_char('f');
        memcpy(aec_uart->uartSendBuf[aec_uart->write_index] + aec_uart->pcm_single_size * ch, buf, aec_uart->pcm_single_size * sizeof(short));
    }
    return 0;
}

/**********************************************************
*                  aec_uart_write
* Description: 将写入通话的数据写入串口buffer
* Arguments  : None.
* Return     : None.
* Note(s)    : None.
**********************************************************/
void aec_uart_write(void)
{
    u8 tmp_w_index;
    if (aec_uart) {
        tmp_w_index = aec_uart->write_index + 1;
        if (tmp_w_index >= PACKET_MAX) {
            tmp_w_index = 0;
        }
        if (aec_uart->write_full) {
            log_char('B');
            aec_uart->write_full = 0;
            return;
        } else {
            aec_uart->write_index = tmp_w_index;
        }
    }
}

/**********************************************************
*                  aec_uart_close
* Description: 关闭数据写卡
* Arguments  : None.
* Return     : 0 成功 其他 失败
* Note(s)    : None.
**********************************************************/
int aec_uart_close(void)
{
    u8 i;
    if (aec_uart == NULL) {
        return 0;
    }
    aec_uart_close();
    for (i = 0; i < PACKET_MAX; i++) {
        aec_uart->uartSendBuf[i] = NULL;
    }
    aec_uart = NULL;
    log_info("aec_uart_close\n");
    return 0;
}

#endif

