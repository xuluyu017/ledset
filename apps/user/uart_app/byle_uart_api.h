/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    BYLE_api.h
 * @version v2.5.6
 * @date    2020.12.16
 * @brief   用户需要主动调用的函数都在该文件内
 */

/****************************** 免责声明 ！！！ *******************************
由于MCU类型和编译环境多种多样，所以此代码仅供参考，用户请自行把控最终代码质量，
涂鸦不对MCU功能结果负责。
******************************************************************************/

#ifndef __BYLE_API_H_
#define __BYLE_API_H_





/*
byle 默认的通讯协议
1、通信协议数据格式
[header1][header2][ID][CMD][datal][data2][checksum][BCCcheck][end]
header1:A5
header2:FA
ID:产品ID，可以自定义，暂定为 0
CMD:指令号码,语音模块对外发为0x81，语音模块接收为0x85。
data1，data2:指会内容
checksum: header1+header2+ID+CMD+datal+data2的累和
BCCcheck:header1、header2、ID、CMD、datal、data2 的异或校验
unsigned char calculate_xor_checksum( unsigned char* data, size_t length) 
{  
unsigned char checksum = 0;    
for (size_t i = 0; i < length; i++) 
{     
checksum ^= data[i];   
}    
return checksum;
}
end:结束字符,在此为FB
2、串口UART1通信:波特率9600 数据位8 无奇偶校验 停止位1


帧头(2byte)	 产品ID(1byte)"	"CMD(1byte)"	"命令词ID(2byte)"	"和校验(1byte)"	"BCC和校验(1byte)"	"帧尾(1byte)"
0xA5      0xFA	        0x00	                   0x81	             0x01  0x00  	                 0x21	                       0xDF	                            0xFB
*/
		 
#define         FRAME_DATA_LEN 9
#define         FRAME_HEAD1             0xA5
#define         FRAME_HEAD2         0xFA 
#define         FRAME_ID         0  
#define         FRAME_CMD         0x81
#define         FRAME_DATA1      0x01
#define         FRAME_DATA2       0x00
#define         FRAME_CHECK_SUM    0x21
#define         FRAME_BCCcheck    0xDF
#define         FRAME_END              0XFB


#define         FRAME_ID_OFSET    2 
#define         CMD_OFSET    3 
#define         DATA1_OFSET     4
#define         DATA2_OFSET     5
#define         CHECK_SUM_OFSET     6

#define   KWS_INDEX_CMD         0x81
#define   FAN_APP_UART_CMD 0x84
#define   TONE_UART_CMD 0X85
#define   TONE_ON_OFF_CMD 0X89
#define   VOLUME_UART_CMD 0X86
#define   BYLE_UART_SEND_USER_CMD_TABLE  FRAME_DATA_LEN
#define   UART_TX_AS_RX_CMD
#define   CMD_WAKEUP_DATA 0X81
#define  SEND_USER_CMD_POWER    NLU_CONTENT_SIZE
#define  SEND_USER_CMD_EXIT         NLU_CONTENT_SIZE+1


/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */
void byle_uart_service(void);

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */
 
void byle_uart_tx(u8 *data, u16 len);
void byle_uart_init(void);


#endif
