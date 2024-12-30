#ifndef __AC_LED_H__
#define __AC_LED_H__

#ifdef AC_zero_port





typedef struct _WY_LED_INFO_
{

bool	LED_on_off;
u8  LED_bright;
u8  LED_userbright;
u8  data_check;
} AC_LED_INFO;

extern AC_LED_INFO  gLedContrl ;


#define  FLASH_MAX_TIME 10

#define   MAX_BRIGHT  255
#ifndef DEFAUFT_MAX_WY
#define   DEFAUFT_MAX_WY    255  //
#endif

#define   STEP_BRIGHT  1
#ifndef MAX_BRIGHT_VALUE
#define MAX_BRIGHT_VALUE  30
#endif

#ifndef RGB_MIN_BRIGHT
#define RGB_MIN_BRIGHT  0//255
#endif
#ifndef MIN_PWM
#define MIN_PWM  1
#endif

#ifndef LITTLE_LIGHT_BRIGHT
#define LITTLE_LIGHT_BRIGHT  30
#endif

#ifndef MIN_BRIGHT
#define MIN_BRIGHT  1//255
#endif

 int kws_led_handle_action( u16 cmd_index);
 void led_on_off(bool onoff_flag);
 bool  is_led_on_off(void);
 void led_bright_dir_set(u8 bright);
 void ac_zero_Led_ioInt(void);
 
 void set_port_interrupt_edge(u32 port, u8 edge);
 void ac_zero_det_LedCtlInt(void);
 void AC_zero_det_led_MsgProccess(int Msg);
#endif
#endif



