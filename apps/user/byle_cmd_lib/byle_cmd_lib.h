


#ifndef BLE_CMD_LIB_H
#define BLE_CMD_LIB_H




#define   SLAVE_USER_KWS_INDEX_CMD               0XFD01
#define   SLAVE_USER_KWS_TONE_CMD                 0XFD02
#define   SLAVE_USER_KWS_SET_ASR_TIMEOUT_CMD   0XFD03

#define   SLAVE_USER_KWS_SET_AEC_EN             0XFD04
#define   SLAVE_USER_KWS_GET_AEC_EN             0XFD05
#define   SLAVE_USER_MSG             0XFDF5



 void ResponseCommand(u16 Cmd, u8* Buf, u8 Len);
 void byle_cmd_buf_rx_data(u8 *addr, u16 len);
void byle_lib_cmd_check(void);
 void byle_cmd_lib_check_int(u8 Authorization_type,char * useid);
 void CmdRxQueueInit(u8 *buf,u16 len);
 void set_lib_protocol_ver( u8 protocol_ver);
 int byle_cmd_tx_data(u8 *tx_buf, u16 len);
void  byle_cmd_rx_data(u8 *rx_buf, u16 len);
void byle_cmd_lib_int(void);

void uartResponseCommand(u16  Cmd,u8* CmdData,u8 datalen);

#endif
