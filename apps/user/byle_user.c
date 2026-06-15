
#include "circular_buf.h"
#include "sound_mge.h"
#include "audio_adc_api.h"
#include "audio_adc.h"
#include "msg.h"
#include "wdt.h"
#include "timer.h"
#include "gpio.h"
#include "timer.h"
#define PWM_HIGH_ON 0  // 共阳极灯带：PWM反相输出，HIGH=亮 LOW=灭
#include "byle_cfg.h"
#include "vm.h"
#ifdef MUSIC_LED_CODE_LESS
#include "music_led_code_less.h"
#endif
#ifdef AC_zero_port
#include "ac_zero_det_led.h"
#endif
#ifdef IR_EMITTER_PORT
#include "ir_emitter.h"
#endif

#include "key.h"
#include "byle_user.h"
#include "byle_cmd_lib.h"
#include "byle_kws_app.h"
#include "byle_msg.h"
#include "timer_app.h"
#include "byle_uart_api.h"
#include "user_pwm.h"
#include "gpio.h"

// ==== 双路色温 PWM 配置 ====
// 冷白光 PWM : IO_PORT_DP + JL_TIMER0
// 暖白光 PWM : IO_PORT_DM + JL_TIMER1
#define CW_PWM_PORT    IO_PORT_DP
#define WW_PWM_PORT    IO_PORT_DM
#define CW_PWM_TIMER   JL_TIMER0
#define WW_PWM_TIMER   JL_TIMER1
#define PWM_FREQ       10000
#define LED_PWM_PORT
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[user]"
#include "log.h"
void *my_free(void *pv);
void *free(void *pv)
{
	return my_free(pv);

}

void put_buf(const u8 *buf, int len)
{
	printf_buf(buf,len);

}

int bylelibprintf(const char *format, ...)
{
	
	log_info(format);
	return 0;
	
}
user_global_info_t MyContrl;
UserMode user_mode = {.cabinet_flag = 0, .wardrobe_flag = 0, .shoe_cabinet_flag = 0, .wine_cabinet_flag = 0, .led_mode_set = 0};

 /*----------------------------------------------------------------------------*/
 /**@brief	保存信息到fm_buf
	@param
	@return
	@note  void fm_save_info()
 */
 /*----------------------------------------------------------------------------*/
 void my_user_save_info(void)
 {
	 log_info("my_user_save_info\n");
	 vm_write(VM_MY_INFO, &MyContrl,sizeof(user_global_info_t));
 }
 
 /*----------------------------------------------------------------------------*/
 /**@brief	从vm读取信息
	@param
	@return
	@note  void fm_read_info()
 */
 /*----------------------------------------------------------------------------*/
 void my_user_read_info(void)
 {
	 int ret = 0;
	ret=vm_read(VM_MY_INFO, &MyContrl,sizeof(user_global_info_t));
	 if (ret !=sizeof(user_global_info_t))
	 {
		 log_info("my_user_read_info is null\n");
		 memset(&MyContrl,0x00,sizeof(user_global_info_t));
	 }
 
	 if(MyContrl.data_check!=0x55)
	 {
		 log_info("----MyContrl default data \n");
		
            MyContrl.data_check=0x55;
            MyContrl.vioce_off=0;           
            #ifdef BYLE_KWS_SWITCH
            MyContrl.user_name_index=0;
            MyContrl.power_on_led=0;
            #endif
	 }
	 else
	 	{
	 log_info("my_user_read_info last\n");
	 	}

//我的LED模式数据从VM中读取出来
	ret = vm_read(VM_LED_MODE_INFO, &user_mode, sizeof(UserMode));
	if(ret != sizeof(UserMode))
	{
		log_info("user_mode is null\n");
		memset(&user_mode, 0x00, sizeof(UserMode));
	}
 
 }


#ifdef ENABLE_GET_MIC_ENERGY
  bool is_audio_on=0;

  int audio_dac_energy_get(void)
{
  return 0;
}
 bool get_dac_audio_on_off(void)
{

       return  is_audio_on;
}
#endif

void mic_bias_ldo( u8 on)
{
if(on)	
       {

       JL_ADDA->ADA_CON2 |= BIT(0);//  开mic——bias
       
	   printf("MIC_BIAS out \n");
       }
else
         {
         JL_ADDA->ADA_CON2 &= (~BIT(0));//  关mic——bias
         

         
		 printf("MIC_BIAS off \n");
    
         
         
         
         }
}

void user_io_init(void)
{

#ifdef LED_PWM_PORT
    gpio_direction_output(CW_PWM_PORT, 0);
    gpio_direction_output(WW_PWM_PORT, 0);
#endif

    key_init();

#ifdef MUSIC_LED_CODE_LESS
MusicLedPortInt();
#endif

#ifdef EN_MIC_BIAS_IO_PORT
 MIC_BIAS_POWER(1) ;
#endif
#ifdef BYLE_KWS_SWITCH
byle_switch_gpio_init();
#endif
#ifdef AC_zero_port
ac_zero_Led_ioInt();
#endif
#ifdef IR_EMITTER_PORT
pwm_io_close(IR_EMITTER_PORT,0);
#endif

}



#ifdef  USER_MCU_TIMER
 
#define   TIMER_UNIT_US    2000//  50// 250  //单位us
 
  SET(interrupt(""))
 static void user_2m_timer_isr(void)
 {
 
	static u8 pwm_cnt=0;
	USER_MCU_TIMER->CON |= BIT(14);
	byle_timer_2ms();
 }
 
 static void user_timer_open(void)
 {
	 u32 timer_clk = 24000000;
 
 request_irq(USER_IRQ_TIME_IDX, 5, user_2m_timer_isr, 0); 
 SFR(USER_MCU_TIMER->CON, 10, 4, TIMER_SRC_STD_24M); //时钟源选择std24m
 SFR(USER_MCU_TIMER->CON, 4, 4, TIMER_PRESCALE_32); 
 USER_MCU_TIMER->CNT = 0; 
 USER_MCU_TIMER->PRD = 24 *TIMER_UNIT_US/32;
 USER_MCU_TIMER->CON |= BIT(14);
 USER_MCU_TIMER->CON |= BIT(0);
 }
 
#endif


#ifdef USER_SW_IRQ_IDX
SET(interrupt(""))
void user_sw_interrupt()
{
	bit_clr_swi(USER_SW_IRQ_IDX);
        user_sw_interrupt_run();
}
#endif
void user_sw_interrupt_en(void)
{
#ifdef USER_SW_IRQ_IDX
	bit_set_swi(USER_SW_IRQ_IDX);
#endif
}


 void user_sw_interrupt_int(void)
{
#ifdef USER_SW_IRQ_IDX
HWI_Uninstall(USER_SW_IRQ_IDX);
HWI_Install(USER_SW_IRQ_IDX, (u32)user_sw_interrupt, 2);
#endif
}

void ldo_off(void)
{
static u8 flag=0;
	log_info("ldo_off33333333333 last\n");
	flag=~flag;
mic_bias_ldo(flag);

}
 void  user_power_on_init(void)
{


my_user_read_info();

#ifdef Byle_Authorization_file
local_irq_disable();
byle_Authorization_lib_int(Byle_Authorization_file,KWS_USE_ID);
local_irq_enable();
#endif

#ifdef USER_SW_IRQ_IDX
user_sw_interrupt_int();
#endif



#ifdef  USER_MCU_TIMER
user_timer_open();
#endif

#ifdef BYLE_KWS_SWITCH
byle_switch_power_on();
#endif

#ifdef ENABLE_GET_MIC_ENERGY
 get_mic_energy_lib_int(MIC_NO_AUDIO_VAULE);
#endif

#ifdef BYLE_LIB_CMD_UART_EN
byle_cmd_lib_int();
#endif

#ifdef BYLE_TIMER_APP
timer_app_init();
#endif

#ifdef BYLE_UATR_ENABLE
byle_uart_init();
#endif  

#ifdef MUSIC_LED_CODE_LESS
MusicLedCtlInt();
#endif

#ifdef LED_PWM_PORT
    user_pwm_init(CW_PWM_PORT, CW_PWM_TIMER, 1, PWM_FREQ);
    user_set_pwm_duty(CW_PWM_TIMER, 255);
    user_pwm_init(WW_PWM_PORT, WW_PWM_TIMER, 1, PWM_FREQ);
    user_set_pwm_duty(WW_PWM_TIMER, 255);
#endif

#ifdef AC_zero_port
ac_zero_det_LedCtlInt();
#endif
#ifdef IR_EMITTER_PORT
ir_emitter_init();
#endif

#ifdef USE_SPI_DRIVE_RGBLED
	spileds_init();
#endif

#ifdef FAN_MOTOR
fan_motor_int();
#endif

#ifdef DISABLE_MIC_BIAS
mic_bias_ldo(0);
#endif

//sys_hi_timer_add(NULL, ldo_off, 3000);


}


		   
void app_timer_loop(void)  // 系统2ms  
{
	
#ifndef USER_MCU_TIMER
 byle_timer_2ms();
#endif


}
static u32 mscnt = 0;

u32 timer_get_ms(void)
{
  return mscnt ;
}

void byle_timer_2ms(void)
{
    mscnt++;

#ifdef BYLE_TIMER_APP
app_timer_scan(2);
#endif
if (0 == (mscnt %500)) 
   {
	//app_task_put_key_msg(MSG_SYS_1S);
   }

}
#ifdef US320A8_kwsA_MINIQ

#define MOTOR_STOP 0
#define MOTOR_RUN_FF 1
#define MOTOR_RUN_FR 2
u16 motor_to_stop_timer=0;
u8 motor_status=MOTOR_STOP;

void  motor_ff(void)
{
log_info("motor_ff");
 MOTOR_A_H();
MOTOR_B_L();
}
void  motor_fr(void)
{
log_info("motor_fr");
 MOTOR_A_L();
MOTOR_B_H();
}
void  motor_stop(void)
{
log_info("motor_stop");

 MOTOR_A_L();
MOTOR_B_L();
}
void to_stop_timeout(void)
{
   if(motor_status==MOTOR_RUN_FF)
   motor_ff();
   if(motor_status==MOTOR_RUN_FR)
   motor_fr();

log_info("to_stop_timeout");
motor_to_stop_timer=0;

}
int minQ_app_msg(int msg)
{
	switch(msg)
		{
                         case MSG_MOTOR_FF:
			log_info("MSG_MOTOR_FF");
			
			if(motor_status==MOTOR_STOP)
				{
			motor_ff();
			motor_status=MOTOR_RUN_FR;
				}
			else if(motor_status==MOTOR_RUN_FF)
				{
				if(motor_to_stop_timer==0)
				motor_to_stop_timer=sys_hi_timeout_add(NULL, to_stop_timeout, 300);
				motor_stop();
			        motor_status=MOTOR_RUN_FR;
				}
			else if(motor_status==MOTOR_RUN_FR)
						{
					       if(motor_to_stop_timer==0)
				              motor_to_stop_timer=sys_hi_timeout_add(NULL, to_stop_timeout, 300);
						motor_stop();
					     motor_status=MOTOR_RUN_FF;
						}

			
                         break;
                         
                         case MSG_MOTOR_FR:
			 log_info("MSG_MOTOR_FR");
                         break;
		}

return msg;

}


#endif


u8 led_max_flag = 0;
u8 led_flag = 0;
s16 duty_cnt = 0;
u16 timer_show_id = 0;
u8 led_up_down_eage = 0;
u16 led_delay_timer_id = 0;
extern u8 led_color_mode;

#define LED_PWM_OFF_DUTY        255
#define LED_FADE_TIME_MS        2000
#define LED_FADE_INTERVAL_MS    20
#define LED_FADE_STEPS          (LED_FADE_TIME_MS / LED_FADE_INTERVAL_MS)

u16 led_fade_timer_id = 0;
u16 led_fade_step = 0;
s16 led_fade_start_cw = LED_PWM_OFF_DUTY;
s16 led_fade_start_ww = LED_PWM_OFF_DUTY;
s16 led_fade_target_cw = LED_PWM_OFF_DUTY;
s16 led_fade_target_ww = LED_PWM_OFF_DUTY;
u8 led_fade_target_on = 0;
u8 led_current_cw_duty = LED_PWM_OFF_DUTY;
u8 led_current_ww_duty = LED_PWM_OFF_DUTY;

static u8 led_limit_duty(s16 duty)
{
	if (duty < 0) {
		return 0;
	}
	if (duty > LED_PWM_OFF_DUTY) {
		return LED_PWM_OFF_DUTY;
	}
	return (u8)duty;
}

static void led_pwm_apply(u8 cw_duty, u8 ww_duty)
{
	user_set_pwm_duty(CW_PWM_TIMER, cw_duty);
	user_set_pwm_duty(WW_PWM_TIMER, ww_duty);
	led_current_cw_duty = cw_duty;
	led_current_ww_duty = ww_duty;
}

static void led_color_temp_get_duty(u8 brightness, u8 *cw_duty, u8 *ww_duty)
{
	switch (led_color_mode) {
		case 0:
			*cw_duty = brightness;
			*ww_duty = LED_PWM_OFF_DUTY;
			break;
		case 1:
			*cw_duty = LED_PWM_OFF_DUTY;
			*ww_duty = brightness;
			break;
		case 2:
			*cw_duty = brightness * 2 / 3;
			*ww_duty = brightness * 1 / 3;
			break;
		default:
			*cw_duty = LED_PWM_OFF_DUTY;
			*ww_duty = LED_PWM_OFF_DUTY;
			break;
	}
}

static void led_fade_cancel(void)
{
	if (led_fade_timer_id) {
		sys_hi_timer_del(led_fade_timer_id);
		led_fade_timer_id = 0;
	}
}

static void led_fade_process(void *priv)
{
	s16 cw_duty;
	s16 ww_duty;

	(void)priv;

	led_fade_step++;
	if (led_fade_step >= LED_FADE_STEPS) {
		led_pwm_apply(led_limit_duty(led_fade_target_cw), led_limit_duty(led_fade_target_ww));
		led_fade_cancel();
		led_flag = led_fade_target_on;
		return;
	}

	cw_duty = led_fade_start_cw + ((led_fade_target_cw - led_fade_start_cw) * led_fade_step) / LED_FADE_STEPS;
	ww_duty = led_fade_start_ww + ((led_fade_target_ww - led_fade_start_ww) * led_fade_step) / LED_FADE_STEPS;
	led_pwm_apply(led_limit_duty(cw_duty), led_limit_duty(ww_duty));
}

static void led_fade_start(u8 target_on)
{
	u8 target_cw = LED_PWM_OFF_DUTY;
	u8 target_ww = LED_PWM_OFF_DUTY;

	led_fade_cancel();

	if (target_on) {
		led_color_temp_get_duty(led_limit_duty(duty_cnt), &target_cw, &target_ww);
	}

	led_fade_start_cw = led_current_cw_duty;
	led_fade_start_ww = led_current_ww_duty;
	led_fade_target_cw = target_cw;
	led_fade_target_ww = target_ww;
	led_fade_target_on = target_on;
	led_fade_step = 0;
	led_flag = target_on;

	if (led_fade_start_cw == led_fade_target_cw && led_fade_start_ww == led_fade_target_ww) {
		led_pwm_apply(target_cw, target_ww);
		return;
	}

	led_fade_timer_id = sys_hi_timer_add(NULL, led_fade_process, LED_FADE_INTERVAL_MS);
}

static void led_fade_on(void)
{
	led_fade_start(1);
}

static void led_fade_off(void)
{
	led_fade_start(0);
}
u8 led_color_mode = 0;  // 0=白光, 1=暖光, 2=中性光

// 根据色温模式和亮度，设置双路 PWM
void led_color_temp_apply(u8 brightness)
{
    u8 cw_duty, ww_duty;

	led_fade_cancel();

    switch (led_color_mode) {
        case 0:  // 白光：仅冷白亮
            cw_duty = brightness;
            ww_duty = 255;
            break;
        case 1:  // 暖光：仅暖白亮
            cw_duty = 255;
            ww_duty = brightness;
            break;
        case 2:  // 中性光：冷白67% + 暖白33%
            cw_duty = brightness * 2 / 3;
            ww_duty = brightness * 1 / 3;
            break;
        default:
            return;
    }

    led_pwm_apply(cw_duty, ww_duty);
}

void led_delay_off(void)
{
	log_info("led_delay_off");
	led_fade_cancel();
	led_pwm_apply(LED_PWM_OFF_DUTY, LED_PWM_OFF_DUTY);
	led_delay_timer_id = 0;
	led_flag = 0;
}

void led_set_ledoff(void)
{
	log_info("led_set_ledoff");
	led_fade_cancel();
	led_pwm_apply(LED_PWM_OFF_DUTY, LED_PWM_OFF_DUTY);
}

void led_set_ledon(void)
{
	log_info("led_set_ledon");
	led_color_temp_apply(duty_cnt);
}

void led_show_twice(void)
{
	static u8 cnt = 0;
	u8 val = 255 * (cnt % 2);
	led_fade_cancel();
	led_pwm_apply(val, val);
	if(cnt == 3)
	{
		cnt = 0;
		sys_hi_timer_del(timer_show_id);
		timer_show_id = 0;
		return;
	}
	cnt++;
}

void led_set_mode_over(void)
{
	user_mode.led_mode_set = 0;
}



int byle_user_app_msg(int msg)
{
	switch(msg)
		{
			case KEY_PWM_CONTROL:
				printf("KEY_PWM_CONTROL");
				if(led_flag)
				{
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				else
				{
					led_fade_on();
				}
				break;
			case KEY_PWM_LONG:
				printf("KEY_PWM_LONG");
				printf("duty_cnt = %d\n", duty_cnt);
				if(led_flag)
				{
					if(!led_max_flag)
					{
						if(duty_cnt<242)
						{
							duty_cnt +=1;
							led_color_temp_apply(duty_cnt);
						}
						if(duty_cnt >= 242 && led_up_down_eage == 0)
						{
							led_up_down_eage = 1;
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);
							user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
						}
					}
					else
					{
						if(duty_cnt>0)
						{
							duty_cnt -=1;
							led_color_temp_apply(duty_cnt);
						}
						if(duty_cnt <= 0 && led_up_down_eage == 0)
						{
							led_up_down_eage = 1;
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);
							user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
						}
					}
				}
				break;
			case KEY_PRESS_UP:
				printf("KEY_PRESS_UP");
				led_max_flag = !led_max_flag;
				led_up_down_eage = 0;
					// if(duty_cnt == 255)
					// 	led_max_flag = 1;
					// else if(duty_cnt == 0)
					// 	led_max_flag = 0;
				break;
			case KEY_DOUBLE_CLICK:
				printf("KEY_DOUBLE_CLICK");
				{
					static u8 led_flag2 = 0;
					u8 val = led_flag2 ? 0 : 255;
					user_set_pwm_duty(CW_PWM_TIMER, val);
					user_set_pwm_duty(WW_PWM_TIMER, val);
					led_flag2 = !led_flag2;
				}
				break;
				// ... (其他原有 case)
        
        case SET_COLOR_WHITE_LED:
            printf(">>> 语音指令: 调成白光\n");
            led_flag = 1;
            led_color_mode = 0;
            led_color_temp_apply(duty_cnt);
            break;

        case SET_COLOR_WARM_LED:
            printf(">>> 语音指令: 调成暖光\n");
            led_flag = 1;
            led_color_mode = 1;
            led_color_temp_apply(duty_cnt);
            break;

        case SET_COLOR_NEUTRAL_LED:
            printf(">>> 语音指令: 调成中性光\n");
            led_flag = 1;
            led_color_mode = 2;
            led_color_temp_apply(duty_cnt);
            break;

        // ...
			case TURNON_LED:
				printf("TURNON_LED");
				if(!user_mode.cabinet_flag && !user_mode.wardrobe_flag && !user_mode.shoe_cabinet_flag && !user_mode.wine_cabinet_flag)
				{
					led_fade_on();
				}
				break;
			case CABINET_LED_ON:
				printf("CABINET_LED_ON");
				if(user_mode.cabinet_flag)
				{
					led_fade_on();
				}
				break;
			case WARDROBE_LED_ON:
				printf("WARDROBE_LED_ON");
				if(user_mode.wardrobe_flag)
				{
					led_fade_on();
				}
				break;
			case SHOE_CABINET_LED_ON:
				printf("SHOE_CABINET_LED_ON");
				if(user_mode.shoe_cabinet_flag)
				{
					led_fade_on();
				}
				break;
			case WINE_CABINET_LED_ON:
				printf("WINE_CABINET_LED_ON");
				if(user_mode.wine_cabinet_flag)
				{
					led_fade_on();
				}
				break;
			case TURNOFF_LED:
				printf("TURNOFF_LED");
				if(!user_mode.cabinet_flag && !user_mode.wardrobe_flag && !user_mode.shoe_cabinet_flag && !user_mode.wine_cabinet_flag)		
				{		
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				break;
			case CABINET_LED_OFF:
				printf("CABINET_LED_OFF");
				if(user_mode.cabinet_flag)
				{
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				break;
			case WARDROBE_LED_OFF:
				printf("WARDROBE_LED_OFF");	
				if(user_mode.wardrobe_flag)
				{
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				break;
			case SHOE_CABINET_LED_OFF:
				printf("SHOE_CABINET_LED_OFF");
				if(user_mode.shoe_cabinet_flag)
				{
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				break;
			case WINE_CABINET_LED_OFF:
				printf("WINE_CABINET_LED_OFF");
				if(user_mode.wine_cabinet_flag)
				{
					led_fade_off();
					sys_hi_timer_del(led_delay_timer_id);
					led_delay_timer_id = 0;
				}
				break;
			case LIGHT_UP_LED:
				printf("LIGHT_UP_LED");	
				if(!user_mode.cabinet_flag && !user_mode.wardrobe_flag && !user_mode.shoe_cabinet_flag && !user_mode.wine_cabinet_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt>0)
							duty_cnt -=51;
						if(duty_cnt <= 0)
						{
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}

						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case CABINET_LED_UP:
				printf("CABINET_LED_UP");
				if(user_mode.cabinet_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt>0)
							duty_cnt -=51;
						if(duty_cnt <= 0)
						{
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case WARDROBE_LED_UP:
				printf("WARDROBE_LED_UP");
				if(user_mode.wardrobe_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt>0)
							duty_cnt -=51;
						if(duty_cnt <= 0)
						{
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case SHOE_CABINET_LED_UP:
				printf("SHOE_CABINET_LED_UP");
				if(user_mode.shoe_cabinet_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt>0)
							duty_cnt -=51;
						if(duty_cnt <= 0)
						{
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case WINE_CABINET_LED_UP:
				printf("WINE_CABINET_LED_UP");
				if(user_mode.wine_cabinet_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt>0)
							duty_cnt -=51;
						if(duty_cnt <= 0)
						{
							duty_cnt = 0;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case LIGHT_DOWN_LED:
				printf("LIGHT_DOWN_LED");
				if(!user_mode.cabinet_flag && !user_mode.wardrobe_flag && !user_mode.shoe_cabinet_flag && !user_mode.wine_cabinet_flag)
				{	
					if(led_flag)
					{
						if(duty_cnt<242)
							duty_cnt +=51;
						if(duty_cnt >= 242)
						{
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case CABINET_LED_DOWN:
				printf("CABINET_LED_DOWN");
				if(user_mode.cabinet_flag)
				{
					if(led_flag)
					{
						if(duty_cnt<242)
							duty_cnt +=51;
						if(duty_cnt >= 242)
						{
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case WARDROBE_LED_DOWN:
				printf("WARDROBE_LED_DOWN");
				if(user_mode.wardrobe_flag)
				{
					if(led_flag)
					{
						if(duty_cnt<242)
							duty_cnt +=51;
						if(duty_cnt >= 242)
						{
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case SHOE_CABINET_LED_DOWN:
				printf("SHOE_CABINET_LED_DOWN");
				if(user_mode.shoe_cabinet_flag)
				{
					if(led_flag)
					{
						if(duty_cnt<242)
							duty_cnt +=51;
						if(duty_cnt >= 242)
						{
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case WINE_CABINET_LED_DOWN:
				printf("WINE_CABINET_LED_DOWN");
				if(user_mode.wine_cabinet_flag)
				{
					if(led_flag)
					{
						if(duty_cnt<242)
							duty_cnt +=51;
						if(duty_cnt >= 242)
						{
							duty_cnt = 242;
							user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
							sys_hi_timeout_add(NULL,led_set_ledon, 500);
							break;
						}
						led_color_temp_apply(duty_cnt);
					}
				}
				break;
			case SLEEP_MODE_LED:
				printf("SLEEP_MODE_LED");		
					if(led_flag)
					{
						duty_cnt = 242;
						led_color_temp_apply(duty_cnt);
					}
				break;
			case DELAY_MODE_LED:
				printf("DELAY_MODE_LED");
					if(led_flag)
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL,led_set_ledon, 500);
						led_delay_timer_id = sys_hi_timeout_add(NULL, led_delay_off, 30000);
					}
				break;
			case SHOW_MODE_LED:
				printf("SHOW_MODE_LED");				
					if(led_flag)
					{
						duty_cnt = 0;
						led_color_temp_apply(duty_cnt);					
					}
				break;
			case LOW_POWER_MODE_LED:
				printf("LOW_POWER_MODE_LED");			
					if(led_flag)
					{
						duty_cnt = 153;
						led_color_temp_apply(duty_cnt);					
					}
				break;
			case SET_LIGHT_LED:
				printf("SET_LIGHT_LED");
				if(!led_flag)
				{
					led_color_temp_apply(duty_cnt);
					sys_hi_timeout_add(NULL, led_set_ledoff,500);
					user_mode.led_mode_set = 1;
					sys_hi_timeout_add(NULL, led_set_mode_over,10000);
				}
				else 
				{
					user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
					sys_hi_timeout_add(NULL, led_set_ledon,500);
					user_mode.led_mode_set = 1;
					sys_hi_timeout_add(NULL, led_set_mode_over,10000);
				}
				break;
			case SET_NORMAL_MODE_LED:
				printf("SET_NORMAL_MODE_LED");
				if(user_mode.led_mode_set)
				{
					if(!led_flag)
					{
						led_color_temp_apply(duty_cnt);
						sys_hi_timeout_add(NULL, led_set_ledoff,500);					
					}
					else 
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL, led_set_ledon,500);				
					}
					user_mode.cabinet_flag = 0;
					user_mode.wardrobe_flag = 0;
					user_mode.shoe_cabinet_flag = 0;
					user_mode.wine_cabinet_flag = 0;
					user_mode.led_mode_set = 0;
					
					vm_write(VM_LED_MODE_INFO, (u8*)&user_mode, sizeof(UserMode));
				}
				break;
			case SET_CABINET_LED:
				printf("SET_CABINET_LED");
				if(user_mode.led_mode_set)
				{
					if(!led_flag)
					{
						led_color_temp_apply(duty_cnt);
						sys_hi_timeout_add(NULL, led_set_ledoff,500);					
					}
					else 
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL, led_set_ledon,500);				
					}
					user_mode.cabinet_flag = 1;
					user_mode.wardrobe_flag = 0;
					user_mode.shoe_cabinet_flag = 0;
					user_mode.wine_cabinet_flag = 0;
					user_mode.led_mode_set = 0;

					vm_write(VM_LED_MODE_INFO, (u8*)&user_mode, sizeof(UserMode));
				}
				break;
			case SET_WARDROBE_LED:
				printf("SET_WARDROBE_LED");
				if(user_mode.led_mode_set)
				{
					if(!led_flag)
					{
						led_color_temp_apply(duty_cnt);
						sys_hi_timeout_add(NULL, led_set_ledoff,500);					
					}
					else 
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL, led_set_ledon,500);				
					}
					user_mode.cabinet_flag = 0;
					user_mode.wardrobe_flag = 1;
					user_mode.shoe_cabinet_flag = 0;
					user_mode.wine_cabinet_flag = 0;
					user_mode.led_mode_set = 0;

					vm_write(VM_LED_MODE_INFO, (u8*)&user_mode, sizeof(UserMode));
				}
				break;
			case SET_SHOE_CABINET_LED:
				printf("SET_SHOE_CABINET_LED");
				if(user_mode.led_mode_set)
				{
					if(!led_flag)
					{
						led_color_temp_apply(duty_cnt);
						sys_hi_timeout_add(NULL, led_set_ledoff,500);					
					}
					else 
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL, led_set_ledon,500);				
					}
					user_mode.cabinet_flag = 0;
					user_mode.wardrobe_flag = 0;
					user_mode.shoe_cabinet_flag = 1;
					user_mode.wine_cabinet_flag = 0;
					user_mode.led_mode_set = 0;

					vm_write(VM_LED_MODE_INFO, (u8*)&user_mode, sizeof(UserMode));
				}
				break;
			case SET_WINE_CABINET_LED:
				printf("SET_WINE_CABINET_LED");
				if(user_mode.led_mode_set)
				{
					if(!led_flag)
					{
						led_color_temp_apply(duty_cnt);
						sys_hi_timeout_add(NULL, led_set_ledoff,500);					
					}
					else 
					{
						user_set_pwm_duty(CW_PWM_TIMER, 255);user_set_pwm_duty(WW_PWM_TIMER, 255);
						sys_hi_timeout_add(NULL, led_set_ledon,500);				
					}
					user_mode.cabinet_flag = 0;
					user_mode.wardrobe_flag = 0;
					user_mode.shoe_cabinet_flag = 0;
					user_mode.wine_cabinet_flag = 1;
					user_mode.led_mode_set = 0;

					vm_write(VM_LED_MODE_INFO, (u8*)&user_mode, sizeof(UserMode));
				}
				break;
			default:
                        #ifdef MUSIC_LED_CODE_LESS
                     	 MusicLEDMsgProccess(msg);
                        #endif
                        #ifdef USE_SPI_DRIVE_RGBLED
                       spiLedMsgProccess(msg);
                     	#endif
                         #ifdef FAN_MOTOR
                         FanMotorMsgProccess(msg);
                         #endif
                        #ifdef US320A8_kwsA_MINIQ
                         minQ_app_msg(msg);
                        #endif
                       #ifdef BYLE_KWS_ASR
			byle_kws_app_msg(msg);
                       #endif

						 
			break;
		}		


        return msg;
}

void app_task_put_key_msg(int msg)
{

post_msg(msg);

}
int byle_get_msg(int *msg)
{
    int  err;
	err=get_msg(&msg[0]);
	if (MSG_NO_ERROR != err)
	{
		msg[0] = NO_MSG;
		printf("get msg err 0x%x\n", err);
	}
	
    return err;

}


int byle_post_msg(int msg)
{
return post_msg(msg);
}

void user_task(void)
{
#ifndef USER_SW_IRQ_IDX
user_sw_interrupt_run();  //如果没有软中断，在task  跑  中断函数
#endif

//log_info("user_task...!\r\n"); //  用户可以在这个task 执行一些循环一直跑的任务，或者用task_create创建线程任务

}




void user_app(void) 
{
u32 err;

err=task_create(user_task, NULL, "user");//task_create demo 创建user线程任务
if (err != OS_NO_ERR) {
	printf("%s creat fail %x\n", __FUNCTION__,	err);
}
key_init();
//指示灯IO初始化
#ifdef BYLE_KWS_ASR
 byle_kws_app();
#endif
#ifdef KWS_AEC_EN
 byle_es_app();
#endif

byle_app();

//sys_s_hi_timerout_add(NULL, byle_app_exit, 10000); //test    退出byle_app_exit


}



