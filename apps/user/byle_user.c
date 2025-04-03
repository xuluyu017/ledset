
#include "circular_buf.h"
#include "sound_mge.h"
#include "audio_adc_api.h"
#include "audio_adc.h"
#include "msg.h"
#include "wdt.h"
#include "timer.h"
#include "gpio.h"
#include "timer.h"
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


int byle_user_app_msg(int msg)
{
	switch(msg)
		{

			case MSG_POWER_OFF:
		
				break;
			
			case MSG_POWER_ON:
		
				break;
			case MSG_SYS_1S:
				log_info("MSG_SYS_1S");
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

#ifdef BYLE_KWS_ASR
 byle_kws_app();
#endif
#ifdef KWS_AEC_EN
 byle_es_app();
#endif

byle_app();

//sys_s_hi_timerout_add(NULL, byle_app_exit, 10000); //test    退出byle_app_exit


}



