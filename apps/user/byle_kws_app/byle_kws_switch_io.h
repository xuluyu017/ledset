#ifndef _BYLE_KWS_SWITCH_IO_H_
#define _BYLE_KWS_SWITCH_IO_H_


typedef struct _SWITCH_INFO_
{
  u8  user_name_index;
  u8  power_on_led;
} SWITCH_INFO;

typedef struct{
    u8 switch_name[20];
    u8 open_name[20];
    u8 close_name[20];
    u8  id;	 
}SWITCH_NAME_INFO;


typedef struct
{
bool on_off		  :1;
u16 sleep_cnt;
u16 timer_on_cnt;
u8   bright;

} byle_switch_global_var_t;


extern byle_switch_global_var_t  switch_global;
void byle_switch_MsgProccess(int Msg);
void  byle_switch_gpio_init(void);
void byle_switch_power_on(void);
void wczy001_led_on_off(void);
void rsp_byle_switch_status(void);
u16 byle_switch_data_handle(u16 command, u8 *data, u16 datalen);
void rsp_default_name(void);
void cmd_ok_set(void);
bool byle_switch_check_tone_play( void);
void asr_led_flash(u8 flash_cnt, u32 timers);
int byle_switch_handle_action( int cmd_index);

#endif

