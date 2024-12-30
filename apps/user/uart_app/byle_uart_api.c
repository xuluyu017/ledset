/**********************************Copyright (c)**********************************

*********************************************************************************/
/**
 * @file    byle_api.c

******************************************************************************/

#include "audio_adc.h"
#include "common.h"
#include "hwi.h"
#include "clock.h"
#include "gpio.h"
#include "sound_mge.h"
#include "circular_buf.h"
#include "audio_adc_api.h"
#include "uart_dev.h"
#include "byle_cfg.h"
#include "byle_user.h"
#include "timer_app.h"
#include "audio_dac.h"
#include "audio_dac_api.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[normal]"
#include "log.h"



#ifdef BYLE_UATR_ENABLE
#include "byle_uart_api.h"
#include "byle_cmd_lib.h"

extern void print_buf(u8 *buf, u32 len);



/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */

#define UART_DATA_LEN      64
static u8 uart_cbuf[UART_DATA_LEN] __attribute__((aligned(4)));



 u8 byle_cmd_r_buf[UART_DATA_LEN*2] AT(.usr_data);  
 cbuffer_t byle_cmd_rx_cbuffer;


 uart_bus_t *byle_uart_bus ;     //*!这里只是定义3个串口 具体对应串口几 看 uartx_arg 填写的串口

void byle_uart_tx(u8 *data, u16 len)
{
	if(byle_uart_bus)
	{
        (byle_uart_bus)->write(data, len);		//把数据写到DMA
         }

}






static void uart_isr_hook(void *arg, u32 status)
{
    const uart_bus_t *ubus = arg;
    u8 uart_rxbuf[AEC_UART_DATA_LEN];
    u32 uart_rxcnt = 0;

	
    if (status == UT_RX||status == UT_RX_OT) {
		uart_rxcnt = ubus->read(uart_rxbuf, AEC_UART_DATA_LEN, 0);
	  cbuf_write(&(byle_cmd_rx_cbuffer), uart_rxbuf, uart_rxcnt);
	    bit_set_swi(IRQ_SOFT2_IDX);	 
         #ifdef BYLE_LIB_CMD_UART_EN
	     byle_cmd_rx_data(uart_rxbuf, uart_rxcnt);
         #endif
		 
    	}

      if(status==UT_TX)
      {
	 //log_info("tx");
      }
}

//*************************************************
void byle_uart_Send_Str(unsigned char *pData)
{
    u16 i;
    u8 send_data[256];

    
	i = 0;
	while (*(pData + i) != 0)
	{
	send_data[i]=*(pData + i);
	//    Module_Send_Char(*(pData + i));
		i++;
	}
      byle_uart_tx(send_data,i);	
	
}


const  struct uart_platform_data_t byle_uart_arg =
{
	.tx_pin = BYLE_UATR_TX_PIN ,//*!这里选择实际串口 串口2无DMA模式 rx_cbuf 必须为NULL rx_cbuf_size必须为0 否则注册失败
	.rx_pin = BYLE_UATR_RX_PIN ,//*!这里选择实际串口 串口2无DMA模式 rx_cbuf 必须为NULL rx_cbuf_size必须为0 否则注册失败
	.rx_cbuf = uart_cbuf,
	.rx_cbuf_size = UART_DATA_LEN,
	.frame_length = BYLE_UATR_FRAME_LEN,
	.rx_timeout =BYLE_UATR_TIME_OUT,
	.isr_cbfun = uart_isr_hook,
	.baud = BYLE_UART_BAUD,
	.is_9bit = 0,
};

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */



void byle_uart_init(void)
{


	log_info("byle_uart_init..................................................................444444444\n");
	//os_sem_create(&byle_rx_sem, 0);
 cbuf_init(&(byle_cmd_rx_cbuffer), byle_cmd_r_buf, UART_DATA_LEN*2);

    byle_uart_bus =uart_dev_open(&byle_uart_arg);
    if (byle_uart_bus != NULL) {
        log_info("byle_uart_init() success test =%d \n",BYLE_UART_BAUD);
//	byle_uart_Send_Str("uart_dev_open() success\n");
 	HWI_Uninstall(IRQ_SOFT2_IDX);
     HWI_Install(IRQ_SOFT2_IDX, (u32)byle_uart_service, 3);

    }
   else
   {
	   log_info("uart_dev_open() error 000000000000000000000\n");
   }
	

}

extern void kws_play_tone( u8 index );

void byle_rx_data_handle(u8 *data)
{
u8 i;
	log_info("id=%x\n",data[0]);
	log_info("data=%x\n",data[1]);

      #ifdef KWS_TONE_EN
      	for(i=0;i<BYLE_KWS_IDEX_TONE_NONE;i++)
      	{
      	if(tone_cmd_table[i]==data[1])
      		{
      		 kws_play_tone(i);
      		}
      	}
      #endif
}



SET(interrupt(""))
void  byle_uart_service()
{
	u8 Temp;
	u16 CheckSum;
	u16 CheckSum1;
	u8 i;
        unsigned short offset = 0;
	static s16  SlaveRxIndex=-2;
        static  u16 rx_value_len = 0;
	unsigned char byle_data_process_buf[5]; 
	
       bit_clr_swi(IRQ_SOFT2_IDX);
	while(cbuf_get_data_len(&(byle_cmd_rx_cbuffer)))
	{
                       cbuf_read(&(byle_cmd_rx_cbuffer),&Temp,1);		
			// put_u8hex(Temp);
			if(SlaveRxIndex == -2)
			{
				if(Temp == 0xa5)       
				{
				offset=0;					
				SlaveRxIndex = -1;	
				}
				
			}
			else if(SlaveRxIndex == -1)
				{
					if(Temp == 0xFA)	   
					{
					offset=0;					
					SlaveRxIndex = 0;	
					}
					
				}

			else  
			{
				      byle_data_process_buf[offset++] = Temp; 
					SlaveRxIndex++;
		                      if(SlaveRxIndex==6&&Temp==0XFB)
		                      	{
	                           	SlaveRxIndex = -2;
					offset=0;
                                           log_info("byle_data_process_buf[1]=%x, byle_data_process_buf[2]=%x\n",byle_data_process_buf[0],byle_data_process_buf[1]);
						log_info("ok\n");
					byle_rx_data_handle(byle_data_process_buf+1);
					
		                      	}
				  else    if(SlaveRxIndex>5)
				  {
					 log_info("error\n");

				  }
			}
	}
}

void byle_uart_data_handle(void)
{



}



#endif

