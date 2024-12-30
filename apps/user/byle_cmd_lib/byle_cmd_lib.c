

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
#ifdef BYLE_LIB_CMD_UART_EN
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[cmdlib]"
#include "log.h"

#include "byle_cmd_lib.h"
#include "byle_uart_api.h"

 int byle_cmd_tx_data(u8 *tx_buf, u16 len)
{
#ifdef BYLE_UATR_ENABLE
byle_uart_tx(tx_buf,len);
#endif
 #ifdef AEC_UATR_ENABLE
	aec_uart_dev_tx(tx_buf,len);
 #endif

return 0;

}
  void  byle_cmd_rx_data(u8 *rx_buf, u16 len)
 {

      byle_cmd_buf_rx_data(rx_buf,len) ;
 }




void uartResponseCommand(u16  Cmd,u8* CmdData,u8 datalen)
{
 ResponseCommand(Cmd, CmdData,datalen);

}

 u16 AudioDecodeCmdProcess(u16  Cmd,u8* CmdData,u8 datalen)
 {
 
 return 0;
}


u16 user_ble_fm_data_handle(u16 command, u8 *data, u8 datalen)
{
	 return 0;

}
 u16 user_helloah_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}

u8  user_projector_data_handle(u16 command, u8 *CmdData,u8 len)
{
	 return 0;

}

void   thermalCmdProcess(u16  Cmd,u8* CmdData,u8 datalen)
{

}
u16  spiLedCmdProcess(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}

u16 user_ble_others_data_handle(u16 command, u8* data, u8 datalen)
{
	return 0;

}

 u16 byle_dev_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
return 0;
}
u16 user_fan_motor_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	 return 0;

}
u16 user_ble_led_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}

u16 user_ble_playctrl_data_handle(u16  Cmd,u8* CmdData)
{
	return 0;

}
u16 user_ble_clock_data_handle(u16	Cmd,u8* CmdData)
{
	return 0;

}

u16  byuni_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}
u16  user_ac_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}

u16  user_ble_soundbox_data_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	return 0;

}


u16 byle_user_cmd_handle(u16  Cmd,u8* CmdData,u8 datalen)
{
	u16 ret = 0;

     	#ifdef BYLE_CMD_KWS
         CmdkwsProcess(Cmd,CmdData,datalen);
     	#endif
       	 #ifdef KWS_AEC_EN
       	 aecCmdProcess(Cmd,CmdData,datalen);
       	 #endif


          return  0;
}
#define  MY_UUID   0x30

  u8 app_uuid(void)
 {
  return MY_UUID;
 }

   #ifndef BL_VER_NUMBER
 #define  BL_VER_NUMBER  2002
   #endif
const u8   mac_addr = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
   
const u8 *bt_get_mac_addr()
{
   return mac_addr;
}
  u16  set_sdk_version( u8 *buf)
 {
		u16 version=BL_VER_NUMBER;
		memcpy(buf, &version, sizeof(BL_VER_NUMBER));
		return sizeof(BL_VER_NUMBER);
 }
 
 
 
#define  OTA_HTTP  "https:xxxx/application"
 
  u16  set_ota_web_http( u8 *buf)
 {
	 char *http= OTA_HTTP;
 
	  memcpy(buf, http, sizeof(OTA_HTTP));
	 return sizeof(OTA_HTTP);
 
 
 }




 #define BYLE_LIB_CMD_BUF_LEN 64
 u8 lib_cmd_buf[BYLE_LIB_CMD_BUF_LEN] AT(.usr_data);

void  byle_rx_circlebuffer_init(u16 len)
{

	CmdRxQueueInit(lib_cmd_buf,len);

}
char *  get_sdk_use_id(void)
{
	char * useid=KWS_USE_ID;

        return useid;
}
#ifndef BYLE_BLE_PROTOCOL_VERSION
#define BYLE_BLE_PROTOCOL_VERSION 0x02
#endif

#ifdef BYLE_LIB_CMD_UART_EN

void byle_rx_task(void)
{
	printf("ble_rx_task statt..555555555555555555..................\n");

  byle_lib_cmd_check();

}
#endif


void byle_cmd_lib_int(void)
{
	u32 err;

	printf("byle_user_int3333333333333333...................\n");

     byle_cmd_lib_check_int(Byle_Authorization_file,KWS_USE_ID);

#if defined(BYLE_LIB_CMD_UART_EN)
	#ifdef BYLE_BLE_PROTOCOL_VERSION
	set_lib_protocol_ver(BYLE_BLE_PROTOCOL_VERSION);
	#endif
  byle_rx_circlebuffer_init(BYLE_LIB_CMD_BUF_LEN);


err=task_create(user_task, NULL, "byle_rx_task");
if (err != OS_NO_ERR) {
	printf("%s creat fail %x\n", __FUNCTION__,	err);
}
#endif


}


#endif



