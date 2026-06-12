

#include "circular_buf.h"
#include "sound_mge.h"
#include "audio_adc_api.h"
#include "audio_adc.h"
#include "msg.h"
#include "wdt.h"
#include "timer.h"
#include "gpio.h"
#include "log.h"
#include "vm.h"

#include "byle_cfg.h"
#include "byle_user.h"

#include "user_pwm.h"
#include "byle_kws_app.h"
#include "byle_msg.h"
#include "timer_app.h"

#ifdef MUSIC_LED_CODE_LESS
#include "music_led_code_less.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[led]"
#include "log.h"

LED_DISP_INFO  gLedContrl;
u8	led_bright_dir_temp=0;
u8	led_bright_dir_temp2=0;

u8	LED_PowerCnt=3;
u8 	MyLed_save_Cnt=0;
u16	ledfadeTimer;
u32	flashTimer;

u8	fadedir2=1;
u8	fadedir=1;
u8	time_cnt=0;
u8	time_cnt1=0;
u8	color_mode_sel_temp=0;
u8         LED_Bright_fade_set=0;
u8         LED_Bright_flag=0;
u8         LED_Y_fade_set=0;
u8         LED_W_fade_set=0;
u8         LED_WYRGB_fade_flag=0;
u8         LED_R_fade_set=0;
u8         LED_G_fade_set=0;
u8         LED_B_fade_set=0;

u8       is_led_RGB=0;
 u8       first_Wpwm_int=0;
 u8       first_Ypwm_int=0;
 u8       first_RGBpwm_int=0;
u8       onoffcnt=0;
  u8	   fadecnt=0;
  //u16 led_off_timer;
  u32 led_off_timer_cnt=0;


#define   MAX_RGB_MODE  7  //
 const u32 RGB_MODE[MAX_RGB_MODE+1] =
{
0Xff0000, //red
0X00FF00, //green
0X0000ff, //blue
0XFF00FF, //purple
0X00FFFF, //cyan
0XFFCBC0,//pink
0XFFFF00, //orange
0XFF00FF,

};                                                                                                         

#define   MAX_RAINBOW_RGB_MODE    6  //
 const u32 RAINBOW_RGB_MODE[MAX_RAINBOW_RGB_MODE+1] =
{
	0XFF0000,  //red
	0XFF9C00,	//orange
	0X8BFF00,	//yellow
	0X00FF00,	//green
	0X00FF8B,	//cyan
	0X0000FF,	//blue
	0XFF00FF,	//purple
};

#ifndef MAX_VOICE_BRIGHT_VALUE
#define MAX_VOICE_BRIGHT_VALUE 5
#endif

const u8  voice_bright_vaule[MAX_VOICE_BRIGHT_VALUE+1] =
{
      0,
	26,
	64,
	128,
	192,
	255,
};

u8 get_voice_nearest_userbright(u8 parameter)
{
	for(u8 i=0;i<=MAX_VOICE_BRIGHT_VALUE;i++)
	{
		if(voice_bright_vaule[i]>=parameter)
			{
				printf("-get_nearest_userbright=%d \n",i);

		return i;
			}
	}
	return 0xff;
}






const u8  bright_vaule[MAX_BRIGHT_VALUE+1] =
{
      0,
	2,
	3,
	5,
	7,
	9,
	15,
	20,
	30,
	35,//
	40,//10
	50,
	70,
	90,
	100,
	140,
	255,
};


void led_save_info(void)
{
	puts("led_save_info\n");
  //  local_irq_disable();

	//printf("  gLedContrl.LED_bright4444444444444= %d\n",   gLedContrl.LED_bright);
	vm_write(VM_LED_INFO, &gLedContrl,sizeof(LED_DISP_INFO));
  //  local_irq_enable();
}

void led_read_info(void)
{
    int ret = 0;
 ret=vm_read(VM_LED_INFO, &gLedContrl,sizeof(LED_DISP_INFO));
 if (ret !=sizeof(LED_DISP_INFO))
  {
        puts("led_info is null\n");
        memset(&gLedContrl,0x00,sizeof(LED_DISP_INFO));
    }

	//puts("led_read_info\n");

}
static void led_off_timeout(void)
{
led_off_timer_cnt=0;

post_msg(MSG_LED_OFF);

}

u16 led_off_timer=0;
void led_sleep_off(u32 second)
{
	led_off_timer_cnt=second;

		if(led_off_timer)
		{
			sys_s_hi_timeout_del(led_off_timer);
			led_off_timer=0;
		}
		if(led_off_timer_cnt)
  
	  led_off_timer = sys_s_hi_timerout_add(NULL,led_off_timeout, led_off_timer_cnt*1000);



		
}






#ifdef USE_TIMER_FOR_PWM
u8 b_pwmduty=0;
u8 r_pwmduty=0; 
u8 g_pwmduty=0;      
u8 w_pwmduty=0; 
u8 y_pwmduty=0;     
#define PWM_TIMER_UNIT_US     100//  50// 250  //��λus

#ifndef   PWM_MAX_DUTY
#define PWM_MAX_DUTY  64
#define PWM_MIN_DUTY  (256/PWM_MAX_DUTY)
#endif

#ifndef  PWM_TIMER
#define  PWM_TIMER   JL_TIMER1
#define  PWM_IRQ_TIME_IDX                    IRQ_TIME1_IDX
#endif

#define PWM_TIMER_2MS     (2000/PWM_TIMER_UNIT_US)


SET(interrupt(""))
static void pwm_timer_isr(void)
{

           static u8 pwm_cnt=0;
           PWM_TIMER->CON |= BIT(14);

           pwm_cnt++;
           if(pwm_cnt>PWM_MAX_DUTY)
           pwm_cnt=0;
		   
  
		   
           #ifdef LED_B_PORT
           if(pwm_cnt <=b_pwmduty&&b_pwmduty!=0)
           {
           #ifdef LED_BB_IO_DRIVER
           LED_BB_ON
           #endif
           LED_B_ON
           
           }
           else
           {
           
           
           
           #ifdef LED_BB_IO_DRIVER
           LED_BB_OFF
           #endif
           LED_B_OFF
           }
           
           #endif
           
           #ifdef LED_R_PORT                            
           if(pwm_cnt <=r_pwmduty&&r_pwmduty!=0)
           {
           
           #ifdef LED_RR_IO_DRIVER
           LED_RR_ON
           #endif
           LED_R_ON
           
           }
           else
           {
           
           #ifdef LED_RR_IO_DRIVER
           LED_RR_OFF
           #endif
           LED_R_OFF
           
           }
           
           #endif                                                              
           #ifdef LED_G_PORT                           
           if(pwm_cnt <=g_pwmduty&&g_pwmduty!=0)
           {
           
           #ifdef LED_GG_IO_DRIVER
           LED_GG_ON
           #endif
           LED_G_ON
           
           }
           else
           {
           #ifdef LED_GG_IO_DRIVER
           LED_GG_OFF
           #endif
           LED_G_OFF
           
           }
           #endif      


         #ifdef LED_W_PORT
                              if(pwm_cnt <=w_pwmduty&&w_pwmduty!=0)
                              {
         #ifdef LED_W1_IO_DRIVER
                              LED_W1_ON
         #endif
                              LED_W_ON
                              
                              }
                              else
                              {
                              
                              
                              
         #ifdef LED_W1_IO_DRIVER
                              LED_W1_OFF
         #endif
                              LED_W_OFF
                              }
                              
         #endif

          #ifdef LED_Y_PORT
                                        if(pwm_cnt <=y_pwmduty&&y_pwmduty!=0)
                                        {
          #ifdef LED_Y1_IO_DRIVER
                                        LED_Y1_ON
          #endif
                                        LED_Y_ON
                                        
                                        }
                                        else
                                        {
                                        
                                        
                                        
          #ifdef LED_Y1_IO_DRIVER
                                        LED_Y1_OFF
          #endif
                                        LED_Y_OFF
                                        }
                                        
          #endif

		   
}
 void pwm_timer_open(void)
{
    u32 timer_clk = 24000000;

request_irq(PWM_IRQ_TIME_IDX, 7, pwm_timer_isr, 0); 
//SFR(PWM_TIMER->CON, 10, 4, 7); 
SFR(PWM_TIMER->CON, 10, 4, TIMER_SRC_STD_24M); //ʱ��Դѡ��std24m

SFR(PWM_TIMER->CON, 4, 4, TIMER_PRESCALE_1); 

PWM_TIMER->CNT = 0; 
//PWM_TIMER->PRD = 12000000 / 1000000 / 4 * PWM_TIMER_UNIT_US;

PWM_TIMER->PRD = 24000000 / 1000000*PWM_TIMER_UNIT_US;

PWM_TIMER->CON |= BIT(14);
PWM_TIMER->CON |= BIT(0);
}
 void pwm_timer_close(void)
{

PWM_TIMER->CON &= ~(BIT(1) | BIT(0));
unrequest_irq(IR_EMITTER_IRQ_IDX);

}


#endif

void led_close(u32 user_hw_port)
{
     gpio_set_die(user_hw_port,0);
     gpio_set_direction(user_hw_port,0);
     gpio_set_pull_up(user_hw_port,0);
     gpio_set_pull_down(user_hw_port,0);

      #if(PWM_HIGH_ON==0)
       gpio_write(user_hw_port,1);
	 #else
       gpio_write(user_hw_port,0);
      #endif

   #ifdef RGB_PWM_LOW_EN
   if(user_hw_port==LED_R_PORT||user_hw_port==LED_G_PORT||user_hw_port==LED_B_PORT)
       gpio_write(user_hw_port,1);
	#endif

#ifdef W_PWM_LOW_EN
if(user_hw_port==LED_W_PORT
#ifdef LED_W_PORT1
||user_hw_port==LED_W_PORT1
#endif
#ifdef LED_W_PORT2
||user_hw_port==LED_W_PORT2
#endif
)
	gpio_write(user_hw_port,1);
 #endif

#ifdef Y_PWM_LOW_EN
 if(user_hw_port==LED_Y_PORT
#ifdef LED_Y_PORT1
|| user_hw_port==LED_Y_PORT1
#endif
#ifdef LED_Y_PORT2
 || user_hw_port==LED_Y_PORT2
#endif

 )
 gpio_write(user_hw_port,1);
 #endif


}

void led_r_init(void)
{


#ifdef LED_R_FOR_TIMER_PWM 


#elif defined(LEDR_PWM_OUTPUT_CHANNEL)
user_pwm_init(LED_R_PORT,LEDR_PWM_TIMER,LEDR_PWM_OUTPUT_CHANNEL,LED_R_PWM_FRE);
#endif
}
void led_g_init(void)
{

#ifdef LED_G_FOR_TIMER_PWM 


 #elif defined(LEDG_PWM_OUTPUT_CHANNEL)
user_pwm_init(LED_G_PORT,LEDG_PWM_TIMER,LEDG_PWM_OUTPUT_CHANNEL,LED_G_PWM_FRE);
#ifdef LEDGG_PWM_OUTPUT_CHANNEL
  user_pwm_init(LED_GG_PORT,LEDGG_PWM_TIMER,LEDGG_PWM_OUTPUT_CHANNEL,LED_G_PWM_FRE);
#endif
#endif
}

void led_b_init(void)
{

#ifdef LED_B_FOR_TIMER_PWM 


 #elif defined(LEDB_PWM_OUTPUT_CHANNEL)
user_pwm_init(LED_B_PORT,LEDB_PWM_TIMER,LEDB_PWM_OUTPUT_CHANNEL,LED_B_PWM_FRE);
 #ifdef LEDBB_PWM_OUTPUT_CHANNEL
  user_pwm_init(LED_BB_PORT,LEDBB_PWM_TIMER,LEDBB_PWM_OUTPUT_CHANNEL,LED_B_PWM_FRE);
#endif
#endif
}

void led_w_init(void)
{
#ifdef LED_W_FOR_TIMER_PWM 

 #elif defined(LEDW_PWM_OUTPUT_CHANNEL)
    user_pwm_init(LED_W_PORT,LEDW_PWM_TIMER,LEDW_PWM_OUTPUT_CHANNEL,LED_W_PWM_FRE);
#endif

}

void led_y_init(void)
{
#ifdef LED_W_FOR_TIMER_PWM 
	
 #elif defined(LEDY_PWM_OUTPUT_CHANNEL)

    user_pwm_init(LED_Y_PORT,LEDY_PWM_TIMER,LEDY_PWM_OUTPUT_CHANNEL,LED_Y_PWM_FRE);
#endif
}

void led_r_pwm(u8 duty)
{
#ifdef LED_R_FOR_TIMER_PWM
r_pwmduty=duty*PWM_MAX_DUTY/255;
if(r_pwmduty==0&&duty)
   r_pwmduty=1;
if(r_pwmduty==0)
{
LED_R_OFF
#ifdef LED_RR_IO_DRIVER
LED_RR_OFF
#endif

}

#elif defined(LEDR_PWM_OUTPUT_CHANNEL)

     user_set_pwm_duty(LEDR_PWM_TIMER,duty);
#endif
}
void led_g_pwm(u8 duty)
{
 #ifdef  LED_G_FOR_TIMER_PWM
g_pwmduty=duty*PWM_MAX_DUTY/255;
if(g_pwmduty==0&&duty)
   g_pwmduty=1;

if(g_pwmduty==0)
{
	LED_G_OFF
#ifdef LED_GG_IO_DRIVER
	LED_GG_OFF
#endif
}

#elif defined(LEDG_PWM_OUTPUT_CHANNEL)

     user_set_pwm_duty(LEDG_PWM_TIMER,duty);
 #endif
}
void led_b_pwm(u8 duty)
{

 #ifdef LED_B_FOR_TIMER_PWM
b_pwmduty=duty*PWM_MAX_DUTY/255;
if(b_pwmduty==0&&duty)
   b_pwmduty=1;

if(b_pwmduty==0)
{
	LED_B_OFF
#ifdef LED_BB_IO_DRIVER
	LED_BB_OFF
#endif
}

#elif defined(LEDB_PWM_OUTPUT_CHANNEL)

     user_set_pwm_duty(LEDB_PWM_TIMER,duty);
 #endif
}


void led_w_pwm(u8 duty)
{

#ifdef LED_W_FOR_TIMER_PWM
w_pwmduty=duty*PWM_MAX_DUTY/255;
 if(w_pwmduty==0&&duty)
	w_pwmduty=1;
 if(w_pwmduty==0)
 {
 LED_W_OFF
#ifdef LED_W1_IO_DRIVER
 LED_W1_OFF
#endif
 
 }
 
#elif defined(LEDW_PWM_OUTPUT_CHANNEL)

 #ifdef Y_PWM_LOW_EN
 duty=255-duty;
 #endif
	  if(duty&&duty<MIN_PWM)
	 {
	 duty=MIN_PWM;
	 }

		  user_set_pwm_duty(LEDW_PWM_TIMER,duty);

 #endif
}


void led_y_pwm(u8 duty)
{
#ifdef LED_Y_FOR_TIMER_PWM
	y_pwmduty=duty*PWM_MAX_DUTY/255;
	 if(y_pwmduty==0&&duty)
		y_pwmduty=1;
	 if(y_pwmduty==0)
	 {
	 LED_Y_OFF
#ifdef LED_Y1_IO_DRIVER
	 LED_Y1_OFF
#endif
	 
	 }
	 
#elif defined(LEDY_PWM_OUTPUT_CHANNEL)


#ifdef Y_PWM_LOW_EN


 duty=255-duty;
 #endif
       	 if(duty&&duty<MIN_PWM)
	 	{
		duty=MIN_PWM;
	 	}

		  user_set_pwm_duty(LEDY_PWM_TIMER,duty);
#endif
}
#ifdef AC_DET_PORT
void ac_det_chage_color(void)
{
static u8 ac_det_cnt=0,ac_det_cnt2=0;
     if(!get_ac_det_in())
     	{
	  if(ac_det_cnt<11)
	  	{
                ++ac_det_cnt;
		  printf("----ac_det low=%d\n",ac_det_cnt);
		 }
     	}
	else
	{
	     if(ac_det_cnt>10)
     	   	{
		  printf("----ac_det_chage_color\n");
		  if(++ac_det_cnt2>5)
		  	{
		  	gLedContrl.LED_power_flag=1;
			gLedContrl.wifi_pair_flag=0;
		  	power_on_switch_color();
	              led_power_on();
			ac_det_cnt2=0;
     	   	        ac_det_cnt=0;
		  	}
     	   	}
		 else
		 	{
     	        	ac_det_cnt=0;
		 	}

	}

}

#endif

void LED_Power_Handle(void)
{
static  u16 	timer_500ms=0 ,cnt=0;

cnt++;

#ifdef AC_DET_PORT
    ac_det_chage_color();
#endif
   if(timer_500ms++>10)
   	{
   	timer_500ms=0;


      //   led_sleep_check();



	if(LED_PowerCnt)
	{
		if(--LED_PowerCnt==0)
		{
		gLedContrl.LED_power_flag=0;
			printf("\n 10S led_save_info = %d\n",   LED_PowerCnt);
			led_save_info();

		}
	}
	else if(MyLed_save_Cnt)
	{

		if(--MyLed_save_Cnt==0)
			{

	           	led_save_info();
			}
	}
   	}

}

void led_timer_2ms(void)
{
 // LED_Power_Handle();
  led_status_pross();

}


void MusicLedPortClose(void)
{

USB_DM_IN();
 USB_DP_IN() ;


#ifdef LED_R_PORT

led_close(LED_R_PORT);
#endif
#ifdef LED_G_PORT

led_close(LED_G_PORT);
#endif
#ifdef LED_B_PORT

led_close(LED_B_PORT);
#endif
#ifdef LED_W_PORT

led_close(LED_W_PORT);
#endif
#ifdef LED_Y_PORT
led_close(LED_Y_PORT);
#endif



}
void MusicLedPortInt(void)
{
   //  led_w_init();
	// led_w_pwm(10);

#ifdef LED_R_PORT

led_close(LED_R_PORT);
#ifdef LED_R_IO_IN_EN
gpio_set_direction(LED_R_PORT,1);
#endif
#ifdef LED_R_IO_DRIVER			
LED_R_IO_DRIVER_INT();			
#endif							
#if defined(LEDR_PWM_OUTPUT_CHANNEL)&&LEDR_PWM_OUTPUT_CHANNEL
PWMTimerPreInt(LEDR_PWM_TIMER,LED_R_PWM_FRE);
#endif
#endif

#ifdef LED_G_PORT
 led_close(LED_G_PORT);
#ifdef LED_G_IO_DRIVER
LED_G_IO_DRIVER_INT();
#endif
#if defined(LEDG_PWM_OUTPUT_CHANNEL)&&LEDG_PWM_OUTPUT_CHANNEL
PWMTimerPreInt(LEDG_PWM_TIMER,LED_G_PWM_FRE);
#endif
#endif
#ifdef LED_B_PORT
led_close(LED_B_PORT);
#ifdef LED_B_IO_DRIVER
LED_B_IO_DRIVER_INT();
#endif
#if defined(LEDB_PWM_OUTPUT_CHANNEL)&&LEDB_PWM_OUTPUT_CHANNEL
PWMTimerPreInt(LEDB_PWM_TIMER,LED_B_PWM_FRE);
#endif
#endif

#ifdef LED_RR_PORT						
 led_close(LED_RR_PORT);
#ifdef LED_RR_IO_DRIVER
LED_RR_IO_DRIVER_INT();
#endif
#endif
#ifdef LED_GG_PORT						
 led_close(LED_GG_PORT);
#ifdef LED_GG_IO_DRIVER
LED_GG_IO_DRIVER_INT();
#endif
#endif
#ifdef LED_BB_PORT						
 led_close(LED_BB_PORT);
#ifdef LED_BB_IO_DRIVER
LED_BB_IO_DRIVER_INT();
#endif
#endif									

#ifdef LED_W_PORT
 led_close(LED_W_PORT);
 #ifdef LED_W_IO_DRIVER
 LED_W_IO_DRIVER_INT();
 #endif
#ifdef LED_W1_IO_DRIVER
 LED_W1_IO_DRIVER_INT();
#endif
#ifdef LED_W_PORT1
 led_close(LED_W_PORT1);
#endif

#if defined(LEDW_PWM_OUTPUT_CHANNEL)&&LEDW_PWM_OUTPUT_CHANNEL
PWMTimerPreInt(LEDW_PWM_TIMER,LED_W_PWM_FRE);
#endif
#endif


#ifdef LED_Y_PORT
 led_close(LED_Y_PORT);
#ifdef LED_Y_IO_DRIVER
LED_Y_IO_DRIVER_INT();
#endif
#ifdef LED_Y1_IO_DRIVER
 LED_Y1_IO_DRIVER_INT();
#endif
#ifdef LED_Y_PORT1
 led_close(LED_Y_PORT1);
#endif

#if defined(LEDY_PWM_OUTPUT_CHANNEL)&&LEDY_PWM_OUTPUT_CHANNEL
PWMTimerPreInt(LEDY_PWM_TIMER,LED_Y_PWM_FRE);
#endif
#endif
#ifdef USE_TIMER_FOR_PWM
pwm_timer_open();
#endif
first_RGBpwm_int=0;
first_Wpwm_int=0;
first_Ypwm_int=0;

}

void led_pwm_wy_close(void)
{

#ifndef DISABLE_W_LED
		 if(first_Ypwm_int==1)
             {
		  led_y_pwm(0);
              #ifdef LED_Y_PORT
               led_close(LED_Y_PORT);
              #endif
             }

		 if(first_Wpwm_int==1)
             {
		  led_w_pwm(0);
	        #ifdef LED_W_PORT
               led_close(LED_W_PORT);
              #endif

             }
#endif

}

void led_pwm_rbg_open(void)
{
           is_led_RGB=1;


#ifndef DISABLE_W_LED
		 if(first_Ypwm_int==1)
             {
		  led_y_pwm(0);

             }
                #ifdef LED_Y_PORT
                   led_close(LED_Y_PORT);
                 #endif

		 if(first_Wpwm_int==1)
             {
		  led_w_pwm(0);


             }

          #ifdef LED_W_PORT
	   led_close(LED_W_PORT);
	  #endif


#endif
		   printf("-led_pwm_rbg_open\n");

	       led_r_init();
	       led_g_init();
	       led_b_init();
            first_Wpwm_int=1;
             first_Ypwm_int=1;
          first_RGBpwm_int=1;

}
void led_pwm_wy_open(void)
{

            is_led_RGB=0;

#ifndef DISABLE_RGB_LED
             if(first_RGBpwm_int==1)
             {
                led_r_pwm(0);
		   led_g_pwm(0);
	         led_b_pwm(0);
               #ifdef LED_R_PORT
               led_close(LED_R_PORT);
              #endif
              #ifdef LED_G_PORT
               led_close(LED_G_PORT);
              #endif

              #ifdef LED_B_PORT
		   led_close(LED_B_PORT);
              #endif
	     }
#endif

     led_w_init();
     led_y_init();



}

void led_pwm_updata(void)
{
	u32 temp;
	u8 Rtemp;
	u8 Gtemp;
	u8 Btemp;
	u8 Ytemp;
	u8 Wtemp;
       #ifndef LED_W_PORT
	   if (gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
	   	{
				 gLedContrl.LED_R_buf=255;
				 gLedContrl.LED_G_buf=255;
				 gLedContrl.LED_B_buf=255;
				 gLedContrl.LED_Y_buf=0;
				  gLedContrl.LED_W_buf=0;
	   	}
	#endif
#ifndef DISABLE_RGB_LED
	if (gLedContrl.LED_R_buf||gLedContrl.LED_G_buf||gLedContrl.LED_B_buf)
	{
         if(is_led_RGB==0)
		{
              led_pwm_rbg_open();
            }

          Rtemp=gLedContrl.LED_R_buf*gLedContrl.LED_bright/255;
          Gtemp=gLedContrl.LED_G_buf*gLedContrl.LED_bright/255;
          Btemp=gLedContrl.LED_B_buf*gLedContrl.LED_bright/255;

	#ifdef MAX_R_PWM_PERCENT           
		Rtemp=Rtemp*MAX_R_PWM_PERCENT/100;
    #endif                    
	#ifdef MAX_G_PWM_PERCENT           
		Gtemp=Gtemp*MAX_G_PWM_PERCENT/100;
    #endif                    
	#ifdef MAX_B_PWM_PERCENT           
		Btemp=Btemp*MAX_B_PWM_PERCENT/100;
    #endif                    
	   led_r_pwm(Rtemp);
              led_g_pwm(Gtemp);
              led_b_pwm(Btemp);
                   //        printf("r%d g%d b%d\n",gLedContrl.LED_R_buf,gLedContrl.LED_G_buf,gLedContrl.LED_B_buf);
	 }
	else
#endif
	{

          if(first_Wpwm_int==0
              #ifdef  LED_Y_PORT
		  ||first_Ypwm_int==0
		  #endif
		  )
           	{
                 if(gLedContrl.LED_W_buf&&first_Wpwm_int==0)
               	{
                    led_w_init();
           	      first_Wpwm_int=1;
               	}
              #ifdef  LED_Y_PORT
 		    if(gLedContrl.LED_Y_buf&&first_Ypwm_int==0)
               	{
                   led_y_init();
                   first_Ypwm_int=1;
           	    }
		#endif
           	}
      else	 if(is_led_RGB==1
	           )
		{
            led_pwm_wy_open();
             }

          is_led_RGB=0;
	  Ytemp=gLedContrl.LED_Y_buf*gLedContrl.LED_bright/255;
	 Wtemp=gLedContrl.LED_W_buf*gLedContrl.LED_bright/255;

	#ifdef MAX_WY_PWM_PERCENT
	     Ytemp=Ytemp*MAX_WY_PWM_PERCENT/100;
          Wtemp=Wtemp*MAX_WY_PWM_PERCENT/100;
        #endif


        if(Ytemp==0&&gLedContrl.LED_bright&&gLedContrl.LED_Y_buf)
        {
        Ytemp=MIN_PWM;
       }
	if(Wtemp==0&&gLedContrl.LED_bright&&gLedContrl.LED_W_buf)
	   {
	   Wtemp=MIN_PWM;
	 }



         led_w_pwm(Wtemp);
         #ifdef  LED_Y_PORT
         led_y_pwm(Ytemp);
	 #endif
	 }

}

void led_bright_dir_set(u8 bright)
{
	u32 temp;
      gLedContrl.LED_bright=bright;
	led_pwm_updata();
}
void led_pwm_dir_set(u8 bright)
{
	u32 temp,Wtemp,Ytemp;

	if(is_led_RGB)
		{
			if(gLedContrl.LED_R_buf)
		led_r_pwm(bright);
			
			if(gLedContrl.LED_G_buf)
		 led_g_pwm(bright);
			
			if(gLedContrl.LED_B_buf)
        led_b_pwm(bright);
		}
            else
            	{

		   if(first_Wpwm_int==0
		    #ifdef  LED_Y_PORT
				||first_Ypwm_int==0
	             #endif
				)
          				  {
          					   if(gLedContrl.LED_W_buf&&first_Wpwm_int==0)
          					  {
          						  led_w_init();
          						first_Wpwm_int=1;
          					  }
          		 #ifdef  LED_Y_PORT
          				  if(gLedContrl.LED_Y_buf&&first_Ypwm_int==0)
          					  {
          						 led_y_init();
          						 first_Ypwm_int=1;
          					  }
                          #endif
          		    }

		 Ytemp=bright;
		Wtemp=bright;

         #ifdef MAX_WY_PWM_PERCENT
	  Ytemp=Ytemp*MAX_WY_PWM_PERCENT/100;
          Wtemp=Wtemp*MAX_WY_PWM_PERCENT/100;
	  #endif

	  if(gLedContrl.LED_W_buf)
           led_w_pwm(Wtemp);
         #ifdef  LED_Y_PORT
	 if(gLedContrl.LED_Y_buf)
         led_y_pwm(Ytemp);
	 #endif
            	}
}

u8 get_nearest_userbright(u8 parameter)
{
	for(u8 i=0;i<=MAX_BRIGHT_VALUE;i++)
	{
		if(bright_vaule[i]>=parameter)
			{
				printf("-get_nearest_userbright=%d \n",i);

		return i;
			}
	}
	return 0xff;
}
void led_pwm_on_off(u8  onoff_flag)
{
		static u8	last_mode=0xff;
		
			if(onoff_flag==2)
				{
				if(last_mode!=0xff)
				gLedContrl.LED_disp_mode=last_mode;
				last_mode=0xff;
				if(gLedContrl.LED_on_off)
					{
								onoff_flag=1;
					}
				else
					onoff_flag=0;
						  
				   }
					else
						{
						if(gLedContrl.LED_disp_mode!=0xff)
						last_mode=gLedContrl.LED_disp_mode;
							  gLedContrl.LED_disp_mode=0xff;
						}
		
			onoffcnt =0;
			fadecnt=0;
			if(onoff_flag==1)
	{

	led_pwm_updata();
	}
	else
	{
                      #ifndef DISABLE_RGB_LED
			if(is_led_RGB)
				{
					 #if defined(AUTO_DET_RGB_IO)
                                        if(led_wyrgb_on_line_flag&RGB_EN)
					 #endif
					 	{
					led_r_pwm(0);
					led_g_pwm(0);
					led_b_pwm(0);
					 	}
				}
			else
				#endif
				{
				
                                #ifdef LED_W_PORT
					 #if defined(AUTO_DET_RGB_IO)
                                        if(led_wyrgb_on_line_flag&W_EN)
					 #endif
					 led_w_pwm(0);
				#endif
					 
                                 #ifdef	LED_Y_PORT
					 #if defined(AUTO_DET_RGB_IO)
                                        if(led_wyrgb_on_line_flag&Y_EN)
					 #endif
					 led_y_pwm(0);
				 #endif
		}
	}
}

bool  is_led_on_off(void)
{
	return gLedContrl.LED_on_off;
}
bool  is_music_mode(void)
{
 if(gLedContrl.LED_disp_mode==LED_SENSOR_MODE)
	return 1;

 return 0;
}

void led_on_off(bool onoff_flag)
{
	gLedContrl.LED_on_off=onoff_flag;

	if(onoff_flag)
	{
	LED_W_fade_set=gLedContrl.LED_W_buf;
	LED_Y_fade_set=gLedContrl.LED_Y_buf;
	LED_Bright_fade_set=gLedContrl.LED_bright;
	led_pwm_updata();
	}
	else
	{
	gLedContrl.LED_disp_mode=LED_NOR;
#ifndef DISABLE_RGB_LED
	if(is_led_RGB)
		{
         	led_r_pwm(0);
         	led_g_pwm(0);
         	led_b_pwm(0);
		}
	else
#endif
		{
             led_w_pwm(0);
             led_y_pwm(0);
		}
	}
}



void led_wy_bright_switch(void)
{
u16 temp;

if((gLedContrl.LED_Y_buf+gLedContrl.LED_W_buf)>=(DEFAUFT_MAX_WY*2))
return;
  if(gLedContrl.LED_Y_buf>=255||gLedContrl.LED_W_buf>=255||gLedContrl.LED_bright<=128)
 return;
 if(gLedContrl.LED_Y_buf==0&&gLedContrl.LED_W_buf)
   {
   gLedContrl.LED_bright=gLedContrl.LED_W_buf;
    gLedContrl.LED_W_buf=255;
   }
  else if(gLedContrl.LED_W_buf==0&&gLedContrl.LED_Y_buf)
  {
gLedContrl.LED_bright=gLedContrl.LED_Y_buf;
 gLedContrl.LED_Y_buf=255;
   }
else
	{
				 if(gLedContrl.LED_Y_buf>gLedContrl.LED_W_buf&&gLedContrl.LED_W_buf)
				   {
						   gLedContrl.LED_bright=gLedContrl.LED_Y_buf*255/DEFAUFT_MAX_WY;
			   temp=255/ gLedContrl.LED_bright;
						 gLedContrl.LED_W_buf=gLedContrl.LED_W_buf*255/gLedContrl.LED_bright;
				   gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
				   }
		   else if(gLedContrl.LED_W_buf&&gLedContrl.LED_Y_buf)
			   {

			   gLedContrl.LED_bright=gLedContrl.LED_W_buf*255/DEFAUFT_MAX_WY;
				temp=255/ gLedContrl.LED_bright;
				gLedContrl.LED_Y_buf=gLedContrl.LED_Y_buf*temp;
				gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;


			   }
  }



}


void led_power_on(void)
{

	gLedContrl.LED_on_off=1;

	if(gLedContrl.LED_bright<LITTLE_LIGHT_BRIGHT/3)
		gLedContrl.LED_bright=MAX_BRIGHT;


	if(gLedContrl.LED_disp_mode==LED_NOR)
	{
	     #ifndef DISABLE_W_LED
		if(gLedContrl.LED_R_buf||gLedContrl.LED_G_buf||gLedContrl.LED_B_buf)
			{
        		   gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
        		  gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
                      gLedContrl.LED_R_buf=0;
                      gLedContrl.LED_G_buf=0;
                      gLedContrl.LED_B_buf=0;
	               gLedContrl.LED_bright=MAX_BRIGHT;
			}
	     #endif
		 led_wy_bright_switch();


		printf("gLedContrl.LED_disp_mode IS LED_NOR \n");
	    #if defined(DISABLE_WY_SLOWLY_CHANGE)
	   		led_on_off(gLedContrl.LED_on_off);
	      #else
  	 //    #ifdef XIE_LIGHT
          	if(gLedContrl.LED_bright<=LITTLE_LIGHT_BRIGHT||(gLedContrl.LED_W_buf<=LITTLE_LIGHT_BRIGHT&&gLedContrl.LED_Y_buf<=LITTLE_LIGHT_BRIGHT))
          		{

          			LED_Bright_flag=0;
                            LED_WYRGB_fade_flag=0;
          	   		led_on_off(gLedContrl.LED_on_off);
          		}
                else
          	//  #endif
          	  	{
		led_bright_dir_temp=0;
		ledfadeTimer=2;
		LED_Bright_flag=1;
             LED_WYRGB_fade_flag=0;
            is_led_RGB=1;
	    LED_Bright_fade_set=gLedContrl.LED_bright;
          	  	}

          	  #endif
	}
#ifndef DISABLE_RGB_LED
	else
	{
             led_mode_display_int();
	}
	#endif
	printf("  gLedContrl.LED_disp_mode= %d\n",   gLedContrl.LED_disp_mode);
	printf("  gLedContrl.LED_power_flag= %d\n",   gLedContrl.LED_power_flag);
	printf("  gLedContrl.LED_bright= %d\n",   gLedContrl.LED_bright);
	printf("  gLedContrl.LED_R_buf= %d\n",   gLedContrl.LED_R_buf);
	printf("  gLedContrl.LED_G_buf= %d\n",   gLedContrl.LED_G_buf);
	printf("  gLedContrl.LED_B_buf= %d\n",   gLedContrl.LED_B_buf);
	printf("  gLedContrl.LED_W_buf= %d\n",   gLedContrl.LED_W_buf);
	printf("  gLedContrl.LED_Y_buf= %d\n",   gLedContrl.LED_Y_buf);
	printf("  gLedContrl.is_led_RGB= %d\n",   is_led_RGB);
	printf("  gLedContrl.LED_Music_sensitivity= %d\n",   gLedContrl.LED_Music_sensitivity);


}


#ifndef DISABLE_RGB_LED

void led_color_dir_set(u8 LED_r,u8 LED_g,u8 LED_b)
{
	if(is_led_RGB==0)
	{
	 led_pwm_rbg_open();
	 }

         led_r_pwm(LED_r);
         led_g_pwm(LED_g);
         led_b_pwm(LED_b);
}

void led_color_fade_set(u8 LED_r,u8 LED_g,u8 LED_b)
{
u32 temp;
if(is_led_RGB==0)
{
 led_pwm_rbg_open();
 }
LED_R_fade_set=LED_r;
LED_G_fade_set=LED_g;
LED_B_fade_set=LED_b;
LED_WYRGB_fade_flag=1;
LED_Y_fade_set=0;
LED_W_fade_set=0;

}

void led_color_set(u8 LED_r,u8 LED_g,u8 LED_b)
{
u32 temp;
if(is_led_RGB==0)
{
 led_pwm_rbg_open();
 }
gLedContrl.LED_B_buf=LED_b;
 gLedContrl.LED_G_buf=LED_g;
 gLedContrl.LED_R_buf=LED_r;
 led_pwm_updata();
}

void Music_LED_Color_Ran(void)
{
	static u8	Colorfadedir=0;
	Colorfadedir++;
	if(Colorfadedir>=MAX_RGB_MODE)
	Colorfadedir=0;
	gLedContrl.LED_R_buf=BYTE2(RGB_MODE[Colorfadedir]);
	gLedContrl.LED_G_buf=BYTE1(RGB_MODE[Colorfadedir]);
	gLedContrl.LED_B_buf=BYTE0(RGB_MODE[Colorfadedir]);
	led_pwm_updata();

}


void  led_mode_display_int(void)
{
	gLedContrl.LED_bright=MAX_BRIGHT;
	fadedir2=1;
	fadedir=1;
	time_cnt=0;
	time_cnt1=0;
	flashTimer=10;
	led_bright_dir_temp=0;
	led_bright_dir_temp2=0;
	if(gLedContrl.LED_disp_mode==LED_COLOR_CHAGE_FADE)
		{
                 led_bright_dir_temp=5;
		}
	  if(gLedContrl.LED_disp_mode==LED_SENSOR_MODE)
		{
		 led_bright_dir_temp=6;
		
		}


            gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
          	gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
          	gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);



	if(gLedContrl.LED_R_buf==0&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
		{
        	gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
          	gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
          	gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);
		}




  if(is_led_RGB==0)
  {
   led_pwm_rbg_open();
   }

}

#endif

#ifndef DISABLE_POWER_SWITCH

void power_on_switch_color(void)
{

	if(gLedContrl.LED_power_flag)//  led_power_flag ????0
	{
		gLedContrl.LED_disp_mode=LED_NOR;
             //gLedContrl.LED_power_flag++;



	#if defined(SWICH_ONLY_WY)
                   #ifdef POWER_ON_RGB_TO_Y
		  if(gLedContrl.LED_Y_buf==255&&gLedContrl.LED_W_buf==0)
			{
		         gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_Y_buf=0;
           		gLedContrl.LED_W_buf=255;
                         gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			}
		else   if(gLedContrl.LED_W_buf==255&&gLedContrl.LED_Y_buf==0)
              	{
		     gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
	           	gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
           		gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
		    }
        	else
      			{
      		           gLedContrl.LED_bright=MAX_BRIGHT;
          		   gLedContrl.LED_Y_buf=255;
                          gLedContrl.LED_W_buf=0;
                          gLedContrl.LED_G_buf=0;
                 		gLedContrl.LED_B_buf=0;
                 		gLedContrl.LED_R_buf=0;
      			}
		#else
		  if(gLedContrl.LED_Y_buf==0&&gLedContrl.LED_W_buf==255)
			{
		gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_Y_buf=255;
           		gLedContrl.LED_W_buf=0;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			}
		else   if(gLedContrl.LED_Y_buf==255&&gLedContrl.LED_W_buf==0)
              	{
		     gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
	           	gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
           		gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
		    }
        	else
      			{
      		           gLedContrl.LED_bright=MAX_BRIGHT;
          		   gLedContrl.LED_Y_buf=0;
                          gLedContrl.LED_W_buf=255;
                          gLedContrl.LED_G_buf=0;
                 		gLedContrl.LED_B_buf=0;
                 		gLedContrl.LED_R_buf=0;
      			}


		#endif
	#elif defined(LED_Y_PORT)


		  if(gLedContrl.LED_Y_buf==0&&gLedContrl.LED_W_buf==255)
			{
		        gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_Y_buf=255;
           		gLedContrl.LED_W_buf=0;
                        gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;

			}


		else   if(gLedContrl.LED_Y_buf==255&&gLedContrl.LED_W_buf==0)
              	{
		     gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
	           	gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
           		gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
		    }

#ifndef DISABLE_LITTLE_LIGHT


         else  if(gLedContrl.LED_W_buf==DEFAUFT_MAX_WY&&gLedContrl.LED_Y_buf==DEFAUFT_MAX_WY&&gLedContrl.LED_bright==MAX_BRIGHT)

			{
			gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
           		gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
		       gLedContrl.LED_bright=LITTLE_LIGHT_BRIGHT;
                         gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			}
#ifndef DISABLE_RGB_LED
		else   if(gLedContrl.LED_bright==LITTLE_LIGHT_BRIGHT&&gLedContrl.LED_W_buf==DEFAUFT_MAX_WY&&gLedContrl.LED_Y_buf==DEFAUFT_MAX_WY )
			{
		        gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_disp_mode=LED_RGB_MODE;
            		gLedContrl.LED_W_buf=0;
            		gLedContrl.LED_Y_buf=0;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
			gLedContrl.LED_R_buf=255;
			led_mode_display_int();
			}
#endif
#endif
		else
			{
		     gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_Y_buf=0;
           		gLedContrl.LED_W_buf=255;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			}
		#elif defined(LED_W_PORT)
                  if(gLedContrl.LED_W_buf==255&&gLedContrl.LED_bright!=LITTLE_LIGHT_BRIGHT)
			{
		     gLedContrl.LED_bright=LITTLE_LIGHT_BRIGHT;
            		gLedContrl.LED_W_buf=255;
            		gLedContrl.LED_Y_buf=0;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
			gLedContrl.LED_R_buf=0;
			}
#ifndef DISABLE_RGB_LED
       else     if(gLedContrl.LED_bright==LITTLE_LIGHT_BRIGHT&&gLedContrl.LED_W_buf==255)
			{
		      gLedContrl.LED_bright=MAX_BRIGHT;
		    	gLedContrl.LED_disp_mode=LED_RGB_MODE;
            		gLedContrl.LED_W_buf=0;
            		gLedContrl.LED_Y_buf=0;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
			gLedContrl.LED_R_buf=255;
			led_mode_display_int();
			}
#endif
		else
			{
		         gLedContrl.LED_bright=MAX_BRIGHT;
			gLedContrl.LED_Y_buf=0;
           		gLedContrl.LED_W_buf=255;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			}
		#else
			if(++gLedContrl.LED_disp_mode>LED_SENSOR_MODE)
			gLedContrl.LED_disp_mode=LED_COLOR_CHAGE;

		gLedContrl.LED_R_buf=255;
		printf(" LED_disp_mode= %d\n",   gLedContrl.LED_disp_mode);
#endif
		printf(" LED_power_on switch= %d\n",   gLedContrl.LED_W_buf);

	}
	else
	{

        #ifndef SAVE_ALL_COLOR
	     #ifndef DISABLE_W_LED
		if(gLedContrl.LED_R_buf||gLedContrl.LED_G_buf||gLedContrl.LED_B_buf)
			{
        		gLedContrl.LED_disp_mode=LED_NOR;

		    #ifdef POWER_ON_RGB_TO_Y
			gLedContrl.LED_W_buf=0;
			gLedContrl.LED_Y_buf=255;

                   #elif defined(POWER_ON_RGB_TO_W)

        		gLedContrl.LED_W_buf=255;
        		gLedContrl.LED_Y_buf=0;
			#else
        		gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
        		gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
			#endif
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
			gLedContrl.LED_bright=MAX_BRIGHT;

			}
              #if ENABLE_YELLOW==0
              if(gLedContrl.LED_W_buf==0)
        		gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
		#endif

	     #endif
	     #endif



     #ifdef  DISABLE_SAVE_ALL_COLOR
			gLedContrl.LED_bright=MAX_BRIGHT;
        		gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
        		gLedContrl.LED_Y_buf=0;
                    gLedContrl.LED_G_buf=0;
           		gLedContrl.LED_B_buf=0;
           		gLedContrl.LED_R_buf=0;
	#endif


		gLedContrl.LED_power_flag=1;
	}

	LED_PowerCnt=3;
}
#endif

#ifndef DISABLE_RGB_LED

#define    MUSIC_LED_LIB
#endif
#ifdef MUSIC_LED_LIB

 
#ifndef MAX_SAMPLE_VAULE 
#define  MAX_SAMPLE_VAULE  5000
#endif
 led_rgb_parameter  Ledparameter;

void int_music_Ledparameter(void)
{
	Ledparameter.max_sample_value=MAX_SAMPLE_VAULE;
       Ledparameter.LED_Music_sensitivity=255;
	   
}
#endif

static void test_timeout(void)
{
	printf("test_timeout");

}

void MusicLedCtlInt(void)
{
	u8 i=0;



	led_read_info();
	gLedContrl.LED_on_off=1;
	if(gLedContrl.data_check!=0x55)
	{

		gLedContrl.data_check=0x55;
		printf("----first power reset data \n");
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_power_flag=0;
		gLedContrl.LED_disp_mode=LED_NOR;
		gLedContrl.LED_Music_sensitivity=255;
		gLedContrl.LED_G_buf=0;
		gLedContrl.LED_B_buf=0;
		gLedContrl.LED_R_buf=0;
		gLedContrl.LED_W_buf=0;
	     gLedContrl.LED_power_flag=0;
#if defined(LED_W_PORT)||defined(LED_Y_PORT)
		gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
              #ifdef LED_Y_PORT
		gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
	      #endif

#else


		gLedContrl.LED_W_buf=0;
		gLedContrl.LED_Y_buf=0;
		gLedContrl.LED_disp_mode=LED_SENSOR_MODE;
#endif
#if defined(POWER_ON_RGB_TO_WY)
gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;

#elif defined(POWER_ON_RGB_TO_Y)
	gLedContrl.LED_W_buf=0;
	gLedContrl.LED_Y_buf=255;

             #elif defined(MUSIC_LED_DEMO_MODE)
                 gLedContrl.LED_disp_mode=MUSIC_LED_DEMO_MODE;
#elif defined(POWER_ON_RGB_TO_W)
			 gLedContrl.LED_disp_mode=LED_NOR;

		gLedContrl.LED_W_buf=255;
		gLedContrl.LED_Y_buf=0;



#endif







		gLedContrl.is_dc_power_on_led_off=0;



                  #ifdef MUSIC_LED_DEMO_MODE
                  gLedContrl.LED_G_buf=255;
                    gLedContrl.LED_B_buf=0;
                  gLedContrl.LED_R_buf=128;
                  gLedContrl.LED_W_buf=0;
                   gLedContrl.LED_Y_buf=0;
                 gLedContrl.LED_disp_mode=MUSIC_LED_DEMO_MODE;
		 printf("	MUSIC_LED_DEMO_MODE= %d\n",		gLedContrl.LED_disp_mode);
                  #endif
                gLedContrl.LED_Music_sensitivity=127;





	}



#ifndef DISABLE_POWER_SWITCH
	power_on_switch_color();
#endif

#if defined(LED_W_PORT)||defined(LED_Y_PORT)

	   if(gLedContrl.LED_W_buf||gLedContrl.LED_Y_buf)
	   {
		   is_led_RGB=1;
		   gLedContrl.LED_disp_mode=LED_NOR;
		   gLedContrl.LED_G_buf=0;
		   gLedContrl.LED_B_buf=0;
		   gLedContrl.LED_R_buf=0;




	   }
	   else

	   	{
		   is_led_RGB=0;

	   	}
#else
	   if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
		   {
			   gLedContrl.LED_R_buf=255;
		   }

	   is_led_RGB=0;
	#endif




   LED_Bright_flag=0;

   if(gLedContrl.LED_bright<LITTLE_LIGHT_BRIGHT)
	gLedContrl.LED_bright=MAX_BRIGHT;

#ifndef SAVE_ALL_COLOR
#ifdef DISABLE_RGB_LED
#ifdef	LED_W_PORT

	if(gLedContrl.LED_R_buf||gLedContrl.LED_G_buf||gLedContrl.LED_B_buf)
	{
	gLedContrl.LED_W_buf=255;
         #ifdef LED_Y_PORT
        gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
          #endif
	gLedContrl.LED_bright=MAX_BRIGHT;
	}
gLedContrl.LED_disp_mode=LED_NOR;
#endif
#endif



#ifdef USB_RGB_LIGHT
#ifdef  LED_W_PORT
gLedContrl.LED_R_buf=0;
gLedContrl.LED_G_buf=0;
gLedContrl.LED_B_buf=0;
gLedContrl.LED_bright=MAX_BRIGHT;
gLedContrl.LED_W_buf=255;
gLedContrl.LED_Y_buf=0;
is_led_RGB=1;
#else
gLedContrl.LED_R_buf=255;
gLedContrl.LED_G_buf=255;
gLedContrl.LED_B_buf=255;
gLedContrl.LED_bright=MAX_BRIGHT;
gLedContrl.LED_W_buf=0;
gLedContrl.LED_Y_buf=0;
is_led_RGB=0;
#endif

  gLedContrl.LED_disp_mode=LED_NOR;

#endif



#if defined(DC_POWER_ON_TO_WY)
{
gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;
gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
}
#endif


#ifdef POWER_ON_MAX_LIGHT
gLedContrl.LED_bright=POWER_ON_MAX_LIGHT;
#endif

#endif
if(gLedContrl.is_dc_power_on_led_off)
{
      LED_Bright_flag=0;
      LED_WYRGB_fade_flag=0;
       led_on_off(0);

	   printf("  is_dc_power_on_led_off= %d\n",	gLedContrl.LED_on_off);
}   else

	if(gLedContrl.LED_disp_mode==LED_NOR)
	{
		printf("gLedContrl.LED_disp_mode IS LED_NOR \n");

	#if defined(DISABLE_WY_SLOWLY_CHANGE)
	   		led_on_off(gLedContrl.LED_on_off);
	#else

          	if(gLedContrl.LED_bright<=LITTLE_LIGHT_BRIGHT||(gLedContrl.LED_W_buf<=LITTLE_LIGHT_BRIGHT&&gLedContrl.LED_Y_buf<=LITTLE_LIGHT_BRIGHT))
          		{

          			LED_Bright_flag=0;
                            LED_WYRGB_fade_flag=0;
          	   		led_on_off(gLedContrl.LED_on_off);
          		}
                else
		{
		led_bright_dir_temp=0;
		ledfadeTimer=2;
		LED_Bright_flag=1;
             LED_WYRGB_fade_flag=0;
             LED_Bright_fade_set=gLedContrl.LED_bright;
		}
       #endif


	}
#ifndef DISABLE_RGB_LED
	else
	{
             led_mode_display_int();
	}
#endif
	gLedContrl.LED_userbright=get_nearest_userbright(gLedContrl.LED_bright);

	//led_save_info();

power:
#ifndef DISABLE_RGB_LED
	gLedContrl.LED_disp_mode_back=gLedContrl.LED_disp_mode;
#endif
	printf("  gLedContrl.LED_disp_mode= %d\n",   gLedContrl.LED_disp_mode);
	printf("  gLedContrl.LED_bright= %d\n",   gLedContrl.LED_bright);
	printf("  gLedContrl.LED_R_buf= %d\n",   gLedContrl.LED_R_buf);
	printf("  gLedContrl.LED_G_buf= %d\n",   gLedContrl.LED_G_buf);
	printf("  gLedContrl.LED_B_buf= %d\n",   gLedContrl.LED_B_buf);
	printf("  gLedContrl.LED_W_buf= %d\n",   gLedContrl.LED_W_buf);
	printf("  gLedContrl.LED_Y_buf= %d\n",   gLedContrl.LED_Y_buf);
	printf("  gLedContrl.is_led_RGB= %d\n",   is_led_RGB);
	printf("  gLedContrl.LED_Music_sensitivity= %d\n",   gLedContrl.LED_Music_sensitivity);

//sys_hi_timer_add(NULL,test_timeout, 3000);
#ifdef MUSIC_LED_LIB
    int_music_Ledparameter();
#endif

   sys_s_hi_timer_add(NULL, led_timer_2ms, 2);


     sys_hi_timer_add(NULL, LED_Power_Handle, 50);

}
#ifndef DISABLE_RGB_LED


void led_mode_fade_set(u8 LedR,u8 LEDG,u8 LEDB,u8 time)
{
	if (flashTimer==0)
	{
		flashTimer= time;

		if(time_cnt==1)
		{
			if(fadedir>240)
				time_cnt=2;
			else if(fadedir<10)
				fadedir++;
			else	 if(fadedir<50)
				fadedir+=5;
			else	 if(fadedir<150)
				fadedir+=10;
			else	  if(fadedir<240)
				fadedir+=15;
			else
				fadedir++;
		if(fadedir<RGB_MIN_BRIGHT)
                                fadedir=RGB_MIN_BRIGHT;

		}
		else if(time_cnt==2)
		{
			if(fadedir==RGB_MIN_BRIGHT)
			{
				time_cnt=1;
			}
			else if(fadedir<10)
				fadedir--;
			else	 if(fadedir<50)
				fadedir-=5;
			else	 if(fadedir<150)
				fadedir-=10;
			else
				fadedir-=15;
		}
		else
		{
			gLedContrl.LED_R_buf=LedR;
			gLedContrl.LED_G_buf=LEDG;
			gLedContrl.LED_B_buf=LEDB;
			time_cnt=2;
		}
		led_bright_dir_set(fadedir);
	}

}

void color_change(void)
{
static u8	led_bright_dir_temp=0;




         if(++led_bright_dir_temp>=MAX_RGB_MODE)
               {
                led_bright_dir_temp=0;
               }
		 gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
		 gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
		  gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);

            led_color_dir_set(gLedContrl.LED_R_buf,gLedContrl.LED_G_buf,gLedContrl.LED_B_buf);
			printf("blye change=%d \n",led_bright_dir_temp);

}

void color_chage_fade(void)
{
static u8 time_cnt=1;
		if(time_cnt==1)
			{
				if(gLedContrl.LED_R_buf)
				gLedContrl.LED_R_buf--;
				if(gLedContrl.LED_G_buf<255)
				gLedContrl.LED_G_buf++;
                        if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==255)
				time_cnt=2;
			}
			else	if(time_cnt==2)
			{
			          if(gLedContrl.LED_G_buf)
				gLedContrl.LED_G_buf--;
				if(gLedContrl.LED_B_buf<255)
				gLedContrl.LED_B_buf++;
                        if(gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==255)
				time_cnt=3;
			}
			else	if(time_cnt==3)
			{
			          if(gLedContrl.LED_B_buf)
				gLedContrl.LED_B_buf--;
				if(gLedContrl.LED_R_buf<255)
				gLedContrl.LED_R_buf++;
                        if(gLedContrl.LED_B_buf==0&&gLedContrl.LED_R_buf==255)
				time_cnt=4;
			}
			else
			{
				gLedContrl.LED_R_buf=255;
				gLedContrl.LED_G_buf=0;
				gLedContrl.LED_B_buf=0;
				time_cnt=1;
			}
  led_pwm_updata();
}




void Music_LED_Color_Fade(void)
{
static u8	time_cnt=0;
static u8	fadedir=1;
static u8	led_bright_dir_temp2=0;
 if(time_cnt==0)
       			{
       				if(fadedir>250)
       					time_cnt=1;
       				else  if(fadedir<15)
       					fadedir++;
       				else	  if(fadedir<50)
       					fadedir+=2;
       				else	  if(fadedir<250)
       					fadedir+=5;
       				else
       					fadedir++;
				   if(fadedir<RGB_MIN_BRIGHT)
                               fadedir=RGB_MIN_BRIGHT;



       			}
       			else if(time_cnt==1)
       			{
           			if(fadedir==RGB_MIN_BRIGHT)
			         {
			            //   fadedir=0;
       					if(led_bright_dir_temp2++>=MAX_RGB_MODE)
       						led_bright_dir_temp2=0;
       					gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp2]);
       					gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp2]);
       					gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp2]);


       					//  printf("LED led_bright_dir_temp=%d \n",led_bright_dir_temp2);
       				//	printf("gLedContrl.LED_R_buf:%d\r\n", gLedContrl.LED_R_buf);
       					//printf("gLedContrl.LED_G_buf:%d\r\n", gLedContrl.LED_G_buf);
       					//printf("gLedContrl.LED_B_buf:%d\r\n", gLedContrl.LED_B_buf);

       					time_cnt=0;
       				}
       				else if(fadedir<15)
       					fadedir--;
       				else	 if(fadedir<50)
       					fadedir-=2;
       				else
       					fadedir-=5;
       			}
       			else
       				time_cnt=0;

       led_bright_dir_set(fadedir);

}


#endif
u8		 fadebright=0;
void led_set_flash(u8 cnt)
{
	onoffcnt =cnt;
	fadecnt=0;
       fadebright=80;

}
void led_set_flash_with(u8 cnt,u8 bright)
{
	onoffcnt =cnt;
	
	fadecnt=0;
       fadebright=bright;

}

void led_set_fade(u8 cnt,u8 bright)
{
	fadecnt =cnt;
	fadebright=bright;
	onoffcnt=0;

	if(gLedContrl.LED_on_off)
		{
	        if(fadebright> gLedContrl.LED_bright)
		fadebright=gLedContrl.LED_bright;
               led_bright_dir_temp=fadebright;
	           fadedir=0;
		}
	else
		{
           led_bright_dir_temp=0;
	     fadedir=1;
		}
}
#ifndef	DISABLE_LED_ADJ_FADE

void led_rgb_fade_set_pross(void)
{
	if(LED_WYRGB_fade_flag)
			{
	//			printf("LED_WYRGB_fade_flag is true \n");
	//			printf("gLedContrl.LED_W_buf:%d\r\n", gLedContrl.LED_W_buf);
	//			printf("gLedContrl.LED_Y_buf:%d\r\n", gLedContrl.LED_Y_buf);
					 if(LED_R_fade_set==0&&LED_B_fade_set==0&&LED_B_fade_set==0&&(LED_W_fade_set||LED_Y_fade_set))
						{
						 gLedContrl.LED_R_buf=0;
						 gLedContrl.LED_G_buf=0;
						 gLedContrl.LED_B_buf=0;
						}
			else if(LED_W_fade_set==0&&LED_Y_fade_set==0&&(LED_R_fade_set||LED_B_fade_set||LED_B_fade_set))
				{
						   gLedContrl.LED_W_buf=0;
						   gLedContrl.LED_Y_buf=0;
				}

				if(LED_W_fade_set>gLedContrl.LED_W_buf)
				{
					led_bright_dir_temp2=LED_W_fade_set-gLedContrl.LED_W_buf;
				/*	if(gLedContrl.LED_W_buf<10)
					gLedContrl.LED_W_buf++;
					else	 if(gLedContrl.LED_W_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_W_buf+=4;
					else	  if(gLedContrl.LED_W_buf<250&&led_bright_dir_temp2>5)
					gLedContrl.LED_W_buf+=5;
					else*/
						  if(led_bright_dir_temp2<15)
							ledfadeTimer=3;
							else if(led_bright_dir_temp2<128)
							ledfadeTimer=2;
							  else
							ledfadeTimer=1;

					gLedContrl.LED_W_buf++;



				}
				else if(LED_W_fade_set<gLedContrl.LED_W_buf)
				{
					led_bright_dir_temp2=gLedContrl.LED_W_buf-LED_W_fade_set;

				/*	if(gLedContrl.LED_W_buf<10)
					gLedContrl.LED_W_buf--;
					else   if(gLedContrl.LED_W_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_W_buf-=4;
					else	   if(led_bright_dir_temp2>5)
					gLedContrl.LED_W_buf-=5;
					else	 */
							 if(led_bright_dir_temp2<15)
							ledfadeTimer=3;
							else if(led_bright_dir_temp2<128)
							ledfadeTimer=2;
							  else
							ledfadeTimer=1;
					gLedContrl.LED_W_buf--;
                                   



				}
				else
				{
					gLedContrl.LED_W_buf=LED_W_fade_set;
				}

				if(LED_Y_fade_set>gLedContrl.LED_Y_buf)
				{
					led_bright_dir_temp2=LED_Y_fade_set-gLedContrl.LED_Y_buf;
				/*	if(gLedContrl.LED_Y_buf<10)
					gLedContrl.LED_Y_buf++;
					else if(gLedContrl.LED_Y_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_Y_buf+=4;
					else	 if(gLedContrl.LED_Y_buf<250&&led_bright_dir_temp2>5)
					gLedContrl.LED_Y_buf+=5;
					else   */
					   if(led_bright_dir_temp2<15)
							ledfadeTimer=3;
							else if(led_bright_dir_temp2<128)
							ledfadeTimer=2;
							  else
							ledfadeTimer=1;
					gLedContrl.LED_Y_buf++;
                                   
				}
				else	if(LED_Y_fade_set<gLedContrl.LED_Y_buf)
				{
					led_bright_dir_temp2=gLedContrl.LED_Y_buf-LED_Y_fade_set;

					/*if(gLedContrl.LED_Y_buf<10)
					gLedContrl.LED_Y_buf--;
					else   if(gLedContrl.LED_Y_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_Y_buf-=4;
					else	   if(led_bright_dir_temp2>5)
					gLedContrl.LED_Y_buf-=5;
					else*/
							   if(led_bright_dir_temp2<15)
							ledfadeTimer=3;
							else if(led_bright_dir_temp2<128)
							ledfadeTimer=2;
							  else
							ledfadeTimer=1;

					gLedContrl.LED_Y_buf--;
                                   



				}
				else
				{
					gLedContrl.LED_Y_buf=LED_Y_fade_set;
				}
#ifndef DISABLE_RGB_LED

				if(LED_R_fade_set>gLedContrl.LED_R_buf)
				{

					led_bright_dir_temp2=LED_R_fade_set-gLedContrl.LED_R_buf;
					/*if(gLedContrl.LED_R_buf<10)
					gLedContrl.LED_R_buf++;
					else	 if(gLedContrl.LED_R_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_R_buf+=4;
					else	  if(gLedContrl.LED_R_buf<250&&led_bright_dir_temp2>5)
					gLedContrl.LED_R_buf+=5;
					else	*/
        				   if(led_bright_dir_temp2<15)
        				ledfadeTimer=3;
        				else if(led_bright_dir_temp2<128)
        				ledfadeTimer=2;
        				  else
        				ledfadeTimer=1;

					gLedContrl.LED_R_buf++;
                                 



				}
				else	if(LED_R_fade_set<gLedContrl.LED_R_buf)
				{
					led_bright_dir_temp2=gLedContrl.LED_R_buf-LED_R_fade_set;
					/*if(gLedContrl.LED_R_buf<10)
					gLedContrl.LED_R_buf--;
					else   if(gLedContrl.LED_R_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_R_buf-=4;
					else	   if(led_bright_dir_temp2>5)
					gLedContrl.LED_R_buf-=5;
					else	*/
                                          if(led_bright_dir_temp2<15)
                                          ledfadeTimer=3;
                                          else if(led_bright_dir_temp2<128)
                                          ledfadeTimer=2;
                                          else
                                          ledfadeTimer=1;


					gLedContrl.LED_R_buf--;
                                


				}
				else
				{
					gLedContrl.LED_R_buf=LED_R_fade_set;
				}

				if(LED_G_fade_set>gLedContrl.LED_G_buf)
				{

					led_bright_dir_temp2=LED_R_fade_set-gLedContrl.LED_G_buf;
					/*if(gLedContrl.LED_G_buf<10)
					gLedContrl.LED_G_buf++;
					else if(gLedContrl.LED_G_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_G_buf+=4;
					else if(gLedContrl.LED_G_buf<250&&led_bright_dir_temp2>5)
					gLedContrl.LED_G_buf+=5;
					else*/

					if(led_bright_dir_temp2<15)
					ledfadeTimer=3;
					else if(led_bright_dir_temp2<128)
					ledfadeTimer=2;
					else
					ledfadeTimer=1;

					gLedContrl.LED_G_buf++;
                                    



				}
				else	if(LED_G_fade_set<gLedContrl.LED_G_buf)
				{
					led_bright_dir_temp2=gLedContrl.LED_G_buf-LED_G_fade_set;
					/*if(gLedContrl.LED_G_buf<15)
					gLedContrl.LED_G_buf--;
					else   if(gLedContrl.LED_G_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_G_buf-=4;
					else	   if(led_bright_dir_temp2>5)
					gLedContrl.LED_G_buf-=5;
					else*/
						if(led_bright_dir_temp2<15)
						ledfadeTimer=3;
						else if(led_bright_dir_temp2<128)
						ledfadeTimer=2;
						else
						ledfadeTimer=1;

					gLedContrl.LED_G_buf--;
                                    


				}
				else
				{
					gLedContrl.LED_G_buf=LED_G_fade_set;
				}

				if(LED_B_fade_set>gLedContrl.LED_B_buf)
				{
					led_bright_dir_temp2=LED_B_fade_set-gLedContrl.LED_B_buf;
					/*if(gLedContrl.LED_B_buf<10)
					gLedContrl.LED_B_buf++;
					else	 if(gLedContrl.LED_B_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_B_buf+=4;
					else	  if(gLedContrl.LED_B_buf<250&&led_bright_dir_temp2>5)
					gLedContrl.LED_B_buf+=5;
					else		*/
						if(led_bright_dir_temp2<15)
						ledfadeTimer=3;
						else if(led_bright_dir_temp2<128)
						ledfadeTimer=2;
						else
						ledfadeTimer=1;

					gLedContrl.LED_B_buf++;

                                            


				}
				else if(LED_B_fade_set<gLedContrl.LED_B_buf)
				{
					led_bright_dir_temp2=gLedContrl.LED_B_buf-LED_B_fade_set;
				/*	if(gLedContrl.LED_B_buf<10)
					gLedContrl.LED_B_buf--;
					else   if(gLedContrl.LED_B_buf<50&&led_bright_dir_temp2>4)
					gLedContrl.LED_B_buf-=4;
					else	   if(led_bright_dir_temp2>5)
					gLedContrl.LED_B_buf-=5;
					else	*/
					if(led_bright_dir_temp2<15)
					ledfadeTimer=3;
					else if(led_bright_dir_temp2<128)
					ledfadeTimer=2;
					else
					ledfadeTimer=1;

					gLedContrl.LED_B_buf--;
                        


				}
				else
				{
					gLedContrl.LED_B_buf=LED_B_fade_set;
				}
#endif
				if(
                              #ifndef DISABLE_RGB_LED
				gLedContrl.LED_R_buf==LED_R_fade_set&&gLedContrl.LED_G_buf==LED_G_fade_set&&
				gLedContrl.LED_B_buf==LED_B_fade_set&&
				#endif
				gLedContrl.LED_Y_buf==LED_Y_fade_set
				&&gLedContrl.LED_W_buf==LED_W_fade_set
				)
				{
					LED_WYRGB_fade_flag=0;
				}
			
				led_pwm_updata();
			}


}
#endif

#ifndef BRIGHT_FADE_DEFAULT_TIME
#define BRIGHT_FADE_DEFAULT_TIME 1
#endif

void led_status_pross(void)
{
static u8 temp;
static u8 on_off=0;

	if(flashTimer)
		flashTimer--;

         if(onoffcnt)
         {
           fadecnt=0;
        	if(flashTimer==0)
        	{
            	onoffcnt--;
              if(onoffcnt==0)
              	{

		if(gLedContrl.LED_on_off)
              	    led_bright_dir_set(gLedContrl.LED_bright);
		else
			led_pwm_dir_set(0);

              	}
            else  if(onoffcnt%2==0)
              	{
              	led_pwm_dir_set(50);
              	}
            	   else
            	   	{

		   led_pwm_dir_set(0);

            	   	}
		flashTimer=24;
            	}
	      return;
         }
	else	  if(fadecnt)
		  {
			 if(flashTimer==0)
			 {
			 if(fadedir==0)
			 	{

			             led_pwm_dir_set(led_bright_dir_temp);
				   if(led_bright_dir_temp)
          			   	{
                                          led_bright_dir_temp--;
          			   	}
				    else
				    	{
					fadecnt--;
					fadedir=1;
				    	}
			 	}
			  else
			  	{

				    led_pwm_dir_set(led_bright_dir_temp);
				  	if(++led_bright_dir_temp>fadebright)
					{
					fadecnt--;
					fadedir=0;
					}
			  	}


 			   if(fadecnt==0)
 				 {
               		        if(gLedContrl.LED_on_off)
               			 led_bright_dir_set(gLedContrl.LED_bright);
               		         else
               			 led_pwm_dir_set(0);
 				 }

		                  flashTimer=2;
			 }
		   return;
		  }
		 if(gLedContrl.LED_on_off==0)
			 return;


	if(ledfadeTimer)
		ledfadeTimer--;

	switch(gLedContrl.LED_disp_mode)
	{
		case LED_NOR:
#ifndef	DISABLE_LED_ADJ_FADE


		if (ledfadeTimer)
		{
			break;
		}


			ledfadeTimer=3;
#endif

		if(LED_Bright_flag)
		{
			//log_info(" 111 !!! \n");

		    #ifdef  DISABLE_LED_ADJ_FADE
			ledfadeTimer=0;
			LED_Bright_flag=0;
			led_bright_dir_set(LED_Bright_fade_set);
			MyLed_save_Cnt=5;
			break;
                     #endif
		//	 printf("led_bright_dir_temp:%d\r\n", led_bright_dir_temp);
			if(LED_Bright_fade_set>led_bright_dir_temp)
			{
				led_bright_dir_temp2=LED_Bright_fade_set-led_bright_dir_temp;
				/*if(led_bright_dir_temp<15)
				led_bright_dir_temp++;
				else	 if(led_bright_dir_temp<90&&led_bright_dir_temp2>2)
				led_bright_dir_temp+=2;
				else	 if(led_bright_dir_temp<250&&led_bright_dir_temp2>5)
				led_bright_dir_temp+=5;
				else  led_bright_dir_temp++;*/

				if(led_bright_dir_temp2<5)
						ledfadeTimer=BRIGHT_FADE_DEFAULT_TIME*4;
                        else if(led_bright_dir_temp2<128)
						ledfadeTimer=BRIGHT_FADE_DEFAULT_TIME*2;
                          else
						ledfadeTimer=BRIGHT_FADE_DEFAULT_TIME;
				 led_bright_dir_temp++;
				 		
				led_bright_dir_set(led_bright_dir_temp);
			}
			else if(LED_Bright_fade_set<led_bright_dir_temp)
			{
				led_bright_dir_temp2=led_bright_dir_temp-LED_Bright_fade_set;

			  /* 	if(led_bright_dir_temp<15)
				led_bright_dir_temp--;
				else	 if(led_bright_dir_temp<90&&led_bright_dir_temp2>2)
				led_bright_dir_temp-=2;
				else	 if(led_bright_dir_temp2>5)
				led_bright_dir_temp-=5;
				else led_bright_dir_temp--;*/
          	
						ledfadeTimer=BRIGHT_FADE_DEFAULT_TIME;
				 led_bright_dir_temp--;
		
				led_bright_dir_set(led_bright_dir_temp);
			}
			else
			{
				led_bright_dir_temp=LED_Bright_fade_set;
				LED_Bright_fade_set=0;
				LED_Bright_flag=0;
			     updata_led_to_app();

			}
		}
#ifdef	DISABLE_LED_ADJ_FADE
if(LED_WYRGB_fade_flag)
{
	ledfadeTimer=0;
       LED_WYRGB_fade_flag=0;
	   
         gLedContrl.LED_R_buf=LED_R_fade_set;
         gLedContrl.LED_G_buf=LED_G_fade_set;
         gLedContrl.LED_B_buf=LED_B_fade_set;
         gLedContrl.LED_W_buf=LED_W_fade_set;
         gLedContrl.LED_Y_buf=LED_Y_fade_set;
		 led_pwm_updata();
}
#else
		led_rgb_fade_set_pross();

#endif
		break;

#ifndef DISABLE_RGB_LED
#ifdef ENABLE_GET_MIC_ENERGY

		case LED_SENSOR_MODE:
		//  gLedContrl.LED_W_buf=0;
		//	printf("LED_SENSOR_MODE  is_music_on=%d\n",is_music_on);
		//  if(ledfadeTimer)

		//ledfadeTimer=3;
		//    Audio_spec_display();

		
	#ifndef DISABLE_DAC_OUT

           if(is_audio_on)
           	{
	  
		if(Audio_spec_color_display())
	        break;
           	}
	#endif

	
           if(get_mic_onoff())
           	{
        		if(Audio_spec_color_display())
	
		 break;
           	}


		if (flashTimer==0)
       		{
			
       			flashTimer=15;
                        Music_LED_Color_Fade();
       		}
		break;

#endif




		case LED_COLOR_CHAGE_FADE:

		if (flashTimer==0)
		{
			flashTimer=gLedContrl.LED_flash_time;

			if(time_cnt==0)
			{
			
				if(fadedir>250)
					time_cnt=1;// fade out
				else  if(fadedir<15)
					fadedir++;
				else	  if(fadedir<50)
					fadedir+=2;
				else	  if(fadedir<250)
					fadedir+=5;
				else
					fadedir++;

				if(fadedir<RGB_MIN_BRIGHT)
                                fadedir=RGB_MIN_BRIGHT;


			}
			else if(time_cnt==1)
			{
			  #if RGB_MIN_BRIGHT
				if(fadedir<=RGB_MIN_BRIGHT)
			   #else
				if(fadedir==0)
			   #endif
				{
                             //   fadedir=0;
                                 // if(gLedContrl.LED_disp_mode!=LED_W_CHAGE_FADE)
                                  	{





					//   printf("led_bright_dir_temp:%d\r\n", led_bright_dir_temp);
					if(++led_bright_dir_temp>MAX_RGB_MODE)
						{
						  led_bright_dir_temp=0;
						}


                                      #ifdef ENABLE_WY_FADE

                                           if(led_bright_dir_temp==(MAX_RGB_MODE-1)
                                           )
                                           {
                                           gLedContrl.LED_Y_buf=0;
                                           gLedContrl.LED_W_buf=255;
                                           gLedContrl.LED_R_buf=0;
                                           gLedContrl.LED_G_buf=0;
                                           gLedContrl.LED_B_buf=0;
                                           }
                                           else if(led_bright_dir_temp==MAX_RGB_MODE
                                           )
                                           {
                                           gLedContrl.LED_Y_buf=255;
                                           gLedContrl.LED_W_buf=0;
                                           gLedContrl.LED_R_buf=0;
                                           gLedContrl.LED_G_buf=0;
                                           gLedContrl.LED_B_buf=0;

                                           }
					else
				      #endif
				      	{
					  gLedContrl.LED_Y_buf=0;
					gLedContrl.LED_W_buf=0;
					gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
					gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
					gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);
				      	}
                                  	}
					//printf("gLedContrl.LED_R_buf:%d\r\n", gLedContrl.LED_R_buf);
					//printf("gLedContrl.LED_G_buf:%d\r\n", gLedContrl.LED_G_buf);
					//printf("gLedContrl.LED_B_buf:%d\r\n", gLedContrl.LED_B_buf);


					time_cnt=0;
				}
                   
				else if(fadedir<15)
					fadedir--;
				else if(fadedir<50)
					fadedir-=2;
				else
					fadedir-=5;
			}
			else
				time_cnt=0;

		//	printf("fadedir:%d\r\n", fadedir);
			//  printf("time_cnt:%d\r\n", time_cnt);

			//	if(gLedContrl.LED_flash_time==0)
			//		fadedir=2;
			led_bright_dir_set(fadedir);
		}
		break;


			case LED_ColorfulflashMode:

				if (flashTimer==0&&gLedContrl.LED_flash_time!=0)
				{

				flashTimer=gLedContrl.LED_flash_time+20;
				if(on_off)
					{
					if(led_bright_dir_temp>=MAX_RGB_MODE)
					   led_bright_dir_temp=0;

					gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
					gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
					gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);
					led_bright_dir_temp++;
					led_r_pwm(0);
					led_g_pwm(0);
					led_b_pwm(0);

				    on_off=0;
					}
				else
					{
				      on_off=1;

					  led_bright_dir_set(gLedContrl.LED_bright);

					}


				}
				break;




		case LED_ColorfulMode:


			if (flashTimer==0&&gLedContrl.LED_flash_time!=0)
			{
				if(led_bright_dir_temp>=MAX_RGB_MODE)
			       led_bright_dir_temp=0;
				flashTimer=gLedContrl.LED_flash_time*3+20;
				gLedContrl.LED_R_buf=BYTE2(RGB_MODE[led_bright_dir_temp]);
				gLedContrl.LED_G_buf=BYTE1(RGB_MODE[led_bright_dir_temp]);
				gLedContrl.LED_B_buf=BYTE0(RGB_MODE[led_bright_dir_temp]);

				led_bright_dir_temp++;

				led_pwm_updata();
			}
			break;




#endif

	}

}

void MusicLEDMsgProccess(int Msg)
{
	u16 temp;
	u8 static pairing_hold_key_cnt=0;
	u8 static wifi_test=0;

        if(Msg<MSG_LED_MIN||Msg>MSG_LED_MAX)
        {


          return ;

        }


	MyLed_save_Cnt=3;

	printf("MusicLEDMsgProccess:%d\r\n", Msg);


	switch(Msg)
	{


#ifndef DISABLE_RGB_LED

          case MSG_LED_COLOR_DISP_MODE_WITH_W:
			gLedContrl.LED_disp_mode=LED_NOR;
			gLedContrl.LED_on_off=1;



			LED_Y_fade_set=0;
			LED_W_fade_set=0;
		         color_mode_sel_temp++;

			if(color_mode_sel_temp==MAX_RGB_MODE)
				{
				LED_R_fade_set=0;
				LED_G_fade_set=0;
				LED_B_fade_set=0;
				LED_W_fade_set=255;

				}
			else
				{
				LED_R_fade_set=BYTE2(RGB_MODE[color_mode_sel_temp]);
				LED_G_fade_set=BYTE1(RGB_MODE[color_mode_sel_temp]);
				LED_B_fade_set=BYTE0(RGB_MODE[color_mode_sel_temp]);
				LED_WYRGB_fade_flag=1;
				}

				if(color_mode_sel_temp>MAX_RGB_MODE)
				{
			        color_mode_sel_temp=0;
				}
			printf("MSG_LED_COLOR_DISP_MODE_SEL_NO_WY%d==R:%d G:%d B:%d  w%d!\r\n",color_mode_sel_temp,LED_R_fade_set,LED_G_fade_set,LED_B_fade_set,LED_W_fade_set);
			break  ;


		case MSG_LED_COLOR_DISP_MODE_SEL_NO_WY:
		gLedContrl.LED_disp_mode=LED_NOR;
		gLedContrl.LED_on_off=1;

		LED_R_fade_set=BYTE2(RGB_MODE[color_mode_sel_temp]);
		LED_G_fade_set=BYTE1(RGB_MODE[color_mode_sel_temp]);
		LED_B_fade_set=BYTE0(RGB_MODE[color_mode_sel_temp]);
		LED_WYRGB_fade_flag=1;
		LED_Y_fade_set=0;
		LED_W_fade_set=0;
		if(++color_mode_sel_temp>=MAX_RGB_MODE)
		color_mode_sel_temp=0;
		printf("MSG_LED_COLOR_DISP_MODE_SEL_NO_WY%d==R:%d G:%d B:%d !\r\n",color_mode_sel_temp,LED_R_fade_set,LED_G_fade_set,LED_B_fade_set);
		break  ;



		case MSG_LED_COLOR_SEL_WITH_W :
		case MSG_LED_COLOR_SEL_WITH_WY:



				gLedContrl.LED_on_off=1;
			   if(gLedContrl.LED_bright<50)
			   gLedContrl.LED_bright=255;

			if(color_mode_sel_temp++>=(MAX_RGB_MODE+3))
				color_mode_sel_temp=0;



			  if(color_mode_sel_temp==0)
			  {
			  
			  LED_R_fade_set=0;
			  LED_G_fade_set=0;
			  LED_B_fade_set=0;
			  LED_W_fade_set=255;
			  LED_Y_fade_set=0;
			  LED_WYRGB_fade_flag=1;
			  ledfadeTimer=50;
                      #ifndef LED_W_PORT
				  color_mode_sel_temp++;
			  #endif


			  }
			if(color_mode_sel_temp==1)
			  {
			  LED_R_fade_set=0;
			  LED_G_fade_set=0;
			  LED_B_fade_set=0;
			  LED_Y_fade_set=255;
			  LED_W_fade_set=0;
			  LED_WYRGB_fade_flag=1;
                         #ifndef LED_Y_PORT
                         color_mode_sel_temp++;
                         #endif


			  }
			 if(color_mode_sel_temp==2)
			  {
			  LED_R_fade_set=0;
			  LED_G_fade_set=0;
			  LED_B_fade_set=0;
			  LED_Y_fade_set=DEFAUFT_MAX_WY;
			  LED_W_fade_set=DEFAUFT_MAX_WY;
			  LED_WYRGB_fade_flag=1;
                          #ifndef LED_Y_PORT
                          #ifndef LED_W_PORT
                          color_mode_sel_temp++;
                          #endif
                          #endif



			  }
			if(color_mode_sel_temp>2)
			  	{

                    	               	{
                    			LED_R_fade_set=BYTE2(RGB_MODE[color_mode_sel_temp-3]);
                    			LED_G_fade_set=BYTE1(RGB_MODE[color_mode_sel_temp-3]);
                    			LED_B_fade_set=BYTE0(RGB_MODE[color_mode_sel_temp-3]);
                    			LED_W_fade_set=0;
                    			LED_Y_fade_set=0;
                    			LED_WYRGB_fade_flag=1;
                    	               	}
			  	}
			ledfadeTimer=50;
			gLedContrl.LED_disp_mode=LED_NOR;
			printf("color_mode_sel_temp:%d\r\n", color_mode_sel_temp);
			printf("R:%d G:%d B:%d W:%d Y:%d\r\n",LED_R_fade_set,LED_G_fade_set,LED_B_fade_set,LED_W_fade_set ,LED_Y_fade_set);

			break  ;


		case MSG_LED_COLOR_SEL_MODE:
           //if(gLedContrl.LED_on_off==0)
				//break  ;
			gLedContrl.LED_on_off=1;

		   if(gLedContrl.LED_bright<50)
		   gLedContrl.LED_bright=255;
		if(color_mode_sel_temp++>=7)
			color_mode_sel_temp=0;
	          printf("color_mode_sel_temp:%d\r\n", color_mode_sel_temp);
		LED_R_fade_set=BYTE2(RGB_MODE[color_mode_sel_temp]);
		LED_G_fade_set=BYTE1(RGB_MODE[color_mode_sel_temp]);
		LED_B_fade_set=BYTE0(RGB_MODE[color_mode_sel_temp]);
		LED_W_fade_set=0;
		LED_Y_fade_set=0;
		LED_WYRGB_fade_flag=1;
		ledfadeTimer=50;
		gLedContrl.LED_disp_mode=LED_NOR;

		break  ;






		case MSG_LED_SENOR_MODE:
		gLedContrl.LED_W_buf=0;
		gLedContrl.LED_Y_buf=0;
		gLedContrl.LED_on_off=1;
		gLedContrl.LED_disp_mode=LED_SENSOR_MODE;
		led_mode_display_int();
		color_mode_sel_temp=LED_SENSOR_MODE-LED_COLOR_CHAGE+3;
		printf("MSG_LED_SENOR_MODE\n");

		break  ;


		case MSG_LED_DISP_MODE:
		gLedContrl.LED_flash_time=DEFAUFT_FLASH_SPEED;
		gLedContrl.LED_W_buf=0;
		gLedContrl.LED_Y_buf=0;
		gLedContrl.LED_bright=MAX_BRIGHT;

		if(gLedContrl.LED_disp_mode==LED_NOR)
			gLedContrl.LED_disp_mode=LED_COLOR_CHAGE;
		else
		{
			if(gLedContrl.LED_disp_mode<LED_COLOR_CHAGE)
			gLedContrl.LED_disp_mode=LED_COLOR_CHAGE;

	          if(++gLedContrl.LED_disp_mode>LED_SENSOR_MODE)
			gLedContrl.LED_disp_mode=LED_COLOR_CHAGE;
		}


		gLedContrl.LED_on_off=1;

			led_mode_display_int();
		printf("gLedContrl.LED_disp_mode==%d!\r\n",gLedContrl.LED_disp_mode);
		break  ;

#endif


			  case	MSG_LED_SLEEP_CANSEL:


						 led_sleep_off(0);
						   onoffcnt=2;
					printf("MSG_LED_SLEEP_CANSEL\n");
				   break;



			   case   MSG_LED_SLEEP_1MIN_OFF:

			  if(gLedContrl.LED_on_off)
				 {
				  led_sleep_off(60);
					   onoffcnt=4;
				 }
			  printf("MSG_LED_SLEEP_1HOUR_OFF\n");
			  break;


			  case	 MSG_LED_SLEEP_15MIN_OFF:
				  if(gLedContrl.LED_on_off)
					 {
				 led_sleep_off(15*60);
					  onoffcnt=4;
					 }
			 printf("MSG_LED_SLEEP_1HOUR_OFF\n");
			 break;



			  case	 MSG_LED_SLEEP_30MIN_OFF:
				  if(gLedContrl.LED_on_off)
					 {

				 led_sleep_off(30*60);
					  onoffcnt=6;
					 }
			 printf("MSG_LED_SLEEP_30MIN_OFF\n");
			 break;

				 case	MSG_LED_SLEEP_10S_OFF:
					 if(gLedContrl.LED_on_off)
						 {
					 led_sleep_off(10);
					 onoffcnt=2;
						 }
					printf("MSG_LED_SLEEP_10S_OFF\n");
					break;




			   case   MSG_LED_SLEEP_10MIN_OFF:

				   if(gLedContrl.LED_on_off)
					 {
							  led_sleep_off(10*60);
							  onoffcnt=4;
					 }
			 printf("MSG_LED_SLEEP_10MIN_OFF\n");
			  break;

			  case	 MSG_LED_SLEEP_30S_OFF:
				 if(gLedContrl.LED_on_off)
					 {
					  led_sleep_off(30);
								 onoffcnt=2;
					 }
			 printf("MSG_LED_SLEEP_30S_OFF\n");
			 break;


				 case	MSG_LED_SLEEP_1HOUR_OFF:


			if(gLedContrl.LED_on_off)
			 {

				 led_sleep_off(60*60);
					  onoffcnt=4;
			 }
			 printf("MSG_LED_SLEEP_1HOUR_OFF\n");
			 break;
				 case	MSG_LED_SLEEP_2HOUR_OFF:
					 if(gLedContrl.LED_on_off)
						 {
				 led_sleep_off(60*2*60);
					  onoffcnt=6;
						 }

			 printf("MSG_LED_SLEEP_2HOUR_OFF\n");
			 break;


            case    MSG_LED_USB_NIGHT_ON:

			 if(gLedContrl.LED_bright<LITTLE_LIGHT_BRIGHT)
			  gLedContrl.LED_bright=MAX_BRIGHT;
			 printf("MSG_LED_USB_NIGHT_ON==%d\n",gLedContrl.LED_on_off);

                      #ifdef POWER_ON_SAVE_ALL_COLOR
			if(gLedContrl.LED_on_off==0)
				{
				
                           #ifndef DISABLE_RGB_LED
                                    gLedContrl.LED_disp_mode=gLedContrl.LED_disp_mode_back;
                        #endif
			            gLedContrl.LED_on_off=1;

				  printf("gLedContrl.LED_disp_mode=%d\n",gLedContrl.LED_disp_mode);
                                      #ifdef DISABLE_RGB_LED
					 gLedContrl.LED_disp_mode=LED_NOR;
                                      #endif
				  if( gLedContrl.LED_disp_mode==LED_NOR)
				  	{


					 led_on_off(gLedContrl.LED_on_off);

					printf("LED_Bright_fade_set444444444444444444444\n");

				  	}

                                #ifndef DISABLE_RGB_LED

				  else
				  	{


					  led_mode_display_int();

					  printf("MSG_LED_USB_NIGHT_ON44\n");
				  	}
				  #endif
				}
			break;
                         #endif



                          #if defined(	LED_W_PORT)
                       #ifndef DISABLE_RGB_LED
                        LED_R_fade_set=0;
                        LED_G_fade_set=0;
                        LED_B_fade_set=0;
                        #endif
				LED_W_fade_set=gLedContrl.LED_W_buf;


                              #ifdef	  LED_Y_PORT
				 LED_Y_fade_set=gLedContrl.LED_Y_buf;
				   if(LED_Y_fade_set==0&&LED_W_fade_set==0)
                                	{
                                         LED_Y_fade_set=255;
					LED_W_fade_set=255;
                                	}
				#else

				if(LED_W_fade_set==0)
					{
					LED_W_fade_set=255;
					}
			         #endif
			   #else
			   	LED_R_fade_set=255;
				LED_G_fade_set=255;
				LED_B_fade_set=255;
				LED_W_fade_set=0;
				LED_Y_fade_set=0;
				#ifndef USB_RGB_LIGHT_SAVE_LAST_LED
				gLedContrl.LED_bright=MAX_BRIGHT;
				#endif
			   #endif
				LED_WYRGB_fade_flag=1;
			   if(gLedContrl.LED_on_off==0)
				   {
				   gLedContrl.LED_R_buf=0;
					 gLedContrl.LED_G_buf=0;
				   gLedContrl.LED_B_buf=0;
				   gLedContrl.LED_Y_buf=0;
				   gLedContrl.LED_W_buf=0;
				   }

				gLedContrl.LED_on_off=1;
			   gLedContrl.LED_disp_mode=LED_NOR;

						printf("-MSG_LED_USB_NIGHT_ON55555555555 ");

		break  ;


		case MSG_LED_ON:
		if(gLedContrl.LED_on_off==0)
                  		{
                  		
			gLedContrl.LED_on_off=1;
			led_on_off(gLedContrl.LED_on_off);
       	    //   led_power_on();
       		}

			break  ;

		case MSG_LED_OFF:
			printf("---------- led_off msg ");

			gLedContrl.LED_on_off=0;
			led_on_off(gLedContrl.LED_on_off);

			printf("  gLedContrl.LED_R_buf= %d\n",   gLedContrl.LED_R_buf);
	printf("  gLedContrl.LED_G_buf= %d\n",   gLedContrl.LED_G_buf);
	printf("  gLedContrl.LED_B_buf= %d\n",   gLedContrl.LED_B_buf);
	printf("  gLedContrl.LED_W_buf= %d\n",   gLedContrl.LED_W_buf);
	printf("  gLedContrl.LED_Y_buf= %d\n",   gLedContrl.LED_Y_buf);
			gLedContrl.LED_disp_mode=LED_NOR;

			break  ;




		case MSG_LED_ON_OFF:

		printf("LED_on_off==%d!\r\n",gLedContrl.LED_on_off);

		if(gLedContrl.LED_on_off)
		{
			gLedContrl.LED_on_off=0;
			led_on_off(gLedContrl.LED_on_off);
			LED_Bright_flag=0;
                  LED_PowerCnt=0;
		}
		else
		{

	       led_power_on();
		}
		break  ;



     case  MSG_LED_VOICE_COLOR_TEMP:
	 	if(gLedContrl.LED_on_off==0)
			break  ;

    case  MSG_LED_COLOR_TEMP:





	     LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;






if(gLedContrl.LED_R_buf||gLedContrl.LED_G_buf||gLedContrl.LED_B_buf)
			 {

		 LED_W_fade_set=255;
		 LED_Y_fade_set=0;
		 }

 else if(LED_W_fade_set==255&&LED_Y_fade_set==0)
	 {
		 LED_W_fade_set=0;
		 LED_Y_fade_set=255;
			 LED_WYRGB_fade_flag=1;


	 }
	 else if(LED_W_fade_set==0&&LED_Y_fade_set==255)

	 {
		 LED_W_fade_set=DEFAUFT_MAX_WY;
		 LED_Y_fade_set=DEFAUFT_MAX_WY;
	 }
	 #ifndef COLOR_TEMP_KEY_DISABLE_NIGHT_LIGHT
	 else if((LED_W_fade_set==DEFAUFT_MAX_WY)&&(LED_Y_fade_set==DEFAUFT_MAX_WY)&&gLedContrl.LED_bright==MAX_BRIGHT&&Msg!=MSG_LED_VOICE_COLOR_TEMP)
	 {
		 LED_W_fade_set=LITTLE_LIGHT_BRIGHT;
		 LED_Y_fade_set=LITTLE_LIGHT_BRIGHT;
	 }
		  #endif

	 else
	 {
		 LED_W_fade_set=255;
		 LED_Y_fade_set=0;
	 }


		gLedContrl.LED_on_off=1;


	if(LED_WYRGB_fade_flag==0)
	{
		if(gLedContrl.LED_bright!=MAX_BRIGHT)
			{
	                    gLedContrl.LED_W_buf=gLedContrl.LED_bright*gLedContrl.LED_W_buf/255;
	                    gLedContrl.LED_Y_buf=gLedContrl.LED_bright*gLedContrl.LED_W_buf/255;
			}

	}


	LED_WYRGB_fade_flag=1;

	#ifdef DISABLE_WY_SLOWLY_CHANGE
	gLedContrl.LED_W_buf=LED_W_fade_set;
	gLedContrl.LED_Y_buf=LED_Y_fade_set;
	gLedContrl.LED_R_buf=0;
	gLedContrl.LED_G_buf=0;
	gLedContrl.LED_B_buf=0;
	LED_WYRGB_fade_flag=0;
	   led_on_off(gLedContrl.LED_on_off);
      #endif

		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;
		printf("W=%d  Y=%d\n",LED_W_fade_set,LED_Y_fade_set);

		break  ;




		case MSG_LED_VOICE_COLOR_TEMP_YELLOW:
			if(gLedContrl.LED_on_off==0)
				break;

		case MSG_LED_COLOR_TEMP_YELLOW:
			printf("--MSG_LED_COLOR_TEMP_YELLOW \n");

                  #if ENABLE_YELLOW
			LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_Y_fade_set=255;
		LED_W_fade_set=0;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		LED_Y_fade_set=255-LED_W_fade_set;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;
                #endif
		break;
		case MSG_LED_VOICE_COLOR_TEMP_WHITE:
			if(gLedContrl.LED_on_off==0)
				break;

		case MSG_LED_COLOR_TEMP_WHITE:
			printf("--MSG_LED_COLOR_TEMP_WHITE \n");




		#if defined(DISABLE_W_LED)
	        LED_R_fade_set=255;
		LED_G_fade_set=255;
		LED_B_fade_set=255;
		LED_WYRGB_fade_flag=1;
	       gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;
		break  ;
               #endif
               	{

		LED_Y_fade_set=0;
		LED_W_fade_set=255;
                }
			LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;

		break;

		case MSG_LED_COLOR_TEMP_MOON:
		LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_Y_fade_set=0;
		LED_W_fade_set=255*3/4;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		LED_Y_fade_set=255-LED_W_fade_set;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;
		break  ;

		case MSG_LED_VOICE_COLOR_TEMP_SUN:
			if(gLedContrl.LED_on_off==0)
				break;

		case MSG_LED_COLOR_TEMP_SUN:
		LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_Y_fade_set=0;
		LED_W_fade_set=DEFAUFT_MAX_WY;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		LED_Y_fade_set=DEFAUFT_MAX_WY;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;
		break  ;


		case MSG_LED_COLOR_TEMP_SUNSET:
		LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_Y_fade_set=0;
		LED_W_fade_set=255/4;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		LED_Y_fade_set=255-LED_W_fade_set;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;
		gLedContrl.LED_disp_mode=LED_NOR;

		ledfadeTimer=50;
		break  ;


		case MSG_LED_COLOR_TEMP_CANDLE:
		LED_R_fade_set=0;
		LED_G_fade_set=0;
		LED_B_fade_set=0;
		LED_Y_fade_set=0;
		LED_W_fade_set=0;
		LED_WYRGB_fade_flag=1;
		gLedContrl.LED_on_off=1;
		LED_Y_fade_set=255-LED_W_fade_set;
		if(gLedContrl.LED_disp_mode!=LED_NOR)
		gLedContrl.LED_bright=MAX_BRIGHT;

		gLedContrl.LED_disp_mode=LED_NOR;
		ledfadeTimer=50;
		break  ;


               case   MSG_LED_WY_MAX_BRIGHT:
		case MSG_LED_BRIGHT_SET_100PERCENT:
			printf("--max light \n");

             		LED_R_fade_set=0;
             		LED_G_fade_set=0;
             		LED_B_fade_set=0;
                       #ifdef	LED_Y_PORT

             		LED_Y_fade_set=DEFAUFT_MAX_WY;
			#endif
             		LED_W_fade_set=DEFAUFT_MAX_WY;
             		LED_WYRGB_fade_flag=1;
             		gLedContrl.LED_on_off=1;
             		gLedContrl.LED_bright=MAX_BRIGHT;
             		gLedContrl.LED_disp_mode=LED_NOR;
             		ledfadeTimer=50;
			break;

		 case	  MSG_LED_MID_BRIGHT:
			  gLedContrl.LED_disp_mode=LED_NOR;
			 ledfadeTimer=5;
			 LED_Bright_flag=1;
			LED_Bright_fade_set=128;
			 if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0&&gLedContrl.LED_W_buf==0&&gLedContrl.LED_Y_buf==0)
			{
			 gLedContrl.LED_W_buf=255;
			}

			    if(gLedContrl.LED_on_off==0)
         		    	{
         		 led_bright_dir_temp=0;
         		 gLedContrl.LED_bright=0;
         		    	}
         		 else
         		led_bright_dir_temp=gLedContrl.LED_bright;
			gLedContrl.LED_userbright=MAX_BRIGHT_VALUE/2;

			gLedContrl.LED_on_off=1;
			break;


	 case MSG_LED_VOICE_MAX_BRIGHT:


		  if(gLedContrl.LED_on_off==0)
			  break;

              case    MSG_LED_MAX_BRIGHT:
		  gLedContrl.LED_disp_mode=LED_NOR;


 		if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0&&gLedContrl.LED_W_buf==0
                    #ifdef	LED_Y_PORT
			&&gLedContrl.LED_Y_buf==0
                     #endif
		)
  			{
                           #ifdef	LED_W_PORT
  			 gLedContrl.LED_W_buf=255;
			 is_led_RGB=1;
			  #else
			  gLedContrl.LED_R_buf=255;
			  #endif
  			}



		    if(gLedContrl.LED_on_off==0)
		    	{

		 led_bright_dir_temp=0;
		 gLedContrl.LED_bright=0;
		    	}
		 else
		led_bright_dir_temp=gLedContrl.LED_bright;
		gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;


			 ledfadeTimer=5;
		 LED_Bright_flag=1;
		LED_Bright_fade_set=255;
		gLedContrl.LED_on_off=1;
		break;

	 case MSG_LED_VOICE_MIN_BRIGHT:
		  if(gLedContrl.LED_on_off==0)
			  break;
              case    MSG_LED_MIN_BRIGHT:

		  gLedContrl.LED_disp_mode=LED_NOR;

		  if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0&&gLedContrl.LED_W_buf==0
             	 #ifdef LED_Y_PORT
             	&&gLedContrl.LED_Y_buf==0
             	  #endif
			  	)
				  {
                           #ifdef   LED_W_PORT
                           gLedContrl.LED_W_buf=255;
                           is_led_RGB=1;
                           #else
                           gLedContrl.LED_R_buf=255;
                           #endif

				   }



		 ledfadeTimer=5;
		 LED_Bright_flag=1;
		 LED_Bright_fade_set=LITTLE_LIGHT_BRIGHT;

                 if(gLedContrl.LED_on_off==0)
                 	{
           		 led_bright_dir_temp=0;
           		 gLedContrl.LED_bright=0;
                 	}
		 else
		 led_bright_dir_temp=gLedContrl.LED_bright;

		 gLedContrl.LED_on_off=1;
		gLedContrl.LED_userbright=MIN_BRIGHT;
		break;

		case MSG_LED_VOICE_BRIGHT_UP:

		    if( gLedContrl.LED_on_off==0)
				break;
			  if(gLedContrl.LED_disp_mode!=LED_NOR)
				  {
                   				  if(gLedContrl.LED_bright<voice_bright_vaule[MIN_BRIGHT])
                   			  gLedContrl.LED_bright=voice_bright_vaule[MIN_BRIGHT];
                   			if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
                   			  {
                   			 gLedContrl.LED_G_buf=255;
                   			  }
                   			gLedContrl.LED_disp_mode=LED_NOR;
				  }

                        #ifdef LED_Y_PORT
                         else if(is_led_RGB==0&& gLedContrl.LED_bright==MAX_BRIGHT)
                        	 {
                        printf("1LED_W_buf=%d	LED_Y_buf=%d	 \n",gLedContrl.LED_W_buf,gLedContrl.LED_Y_buf);
                                          if((gLedContrl.LED_Y_buf+gLedContrl.LED_W_buf)>=(DEFAUFT_MAX_WY*2))
                                           {
                                                #ifdef MAX_MIN_LIGHT_FLASH_LED
                                                 onoffcnt=2;
                                                 #endif
					 #ifdef BEEP_IO
                                            deep_timer_start(1);
                                            #endif
                                           break;
                                           }

                        		if(gLedContrl.LED_Y_buf>=255||gLedContrl.LED_W_buf>=255)
                        		 {
						  #ifdef MAX_MIN_LIGHT_FLASH_LED
                                                 onoffcnt=2;
                                                 #endif
							 #ifdef BEEP_IO
                                            deep_timer_start(1);
                                            #endif
					 break;
                        		 }

                        	   if(gLedContrl.LED_Y_buf==0&&gLedContrl.LED_W_buf)
                        		 {
                          gLedContrl.LED_bright=gLedContrl.LED_W_buf;
                           gLedContrl.LED_W_buf=255;
                        		 }

                        	else  if(gLedContrl.LED_W_buf==0&&gLedContrl.LED_Y_buf)
                        		 {
                          gLedContrl.LED_bright=gLedContrl.LED_Y_buf;
                           gLedContrl.LED_Y_buf=255;
                        		 }

                          else
                        	 {
                        	   if(gLedContrl.LED_Y_buf>gLedContrl.LED_W_buf&&gLedContrl.LED_W_buf)
                        		 {
                        				 gLedContrl.LED_bright=gLedContrl.LED_Y_buf*255/DEFAUFT_MAX_WY;
                        	 temp=255/ gLedContrl.LED_bright;
                        			   gLedContrl.LED_W_buf=gLedContrl.LED_W_buf*255/gLedContrl.LED_bright;
                        		 gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
                        		 }
                         else if(gLedContrl.LED_W_buf&&gLedContrl.LED_Y_buf)
                        	 {

                        	 gLedContrl.LED_bright=gLedContrl.LED_W_buf*255/DEFAUFT_MAX_WY;
                        	  temp=255/ gLedContrl.LED_bright;
                        	  gLedContrl.LED_Y_buf=gLedContrl.LED_Y_buf*temp;
                        	  gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;


                        				/* gLedContrl.LED_bright=gLedContrl.LED_W_buf*255/DEFAUFT_MAX_WY;
                        			   gLedContrl.LED_Y_buf=gLedContrl.LED_Y_buf*DEFAUFT_MAX_WY/gLedContrl.LED_bright;
                        		 gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;*/
                        	 }
                        	 }

                        printf("3 LED_W_buf=%d  LED_Y_buf=%d	LED_bright=%d  \n",gLedContrl.LED_W_buf,gLedContrl.LED_Y_buf,gLedContrl.LED_bright);


                        	 }

                        #endif




                     if(LED_Bright_flag==0)
                         gLedContrl.LED_userbright=get_voice_nearest_userbright(gLedContrl.LED_bright);

            	printf(" gLedContrl.LED_userbright=%d  gLedContrl.LED_bright=%d\n", gLedContrl.LED_userbright,gLedContrl.LED_bright);



                    if(gLedContrl.LED_userbright<MAX_VOICE_BRIGHT_VALUE)
                     gLedContrl.LED_userbright++;

                     else
                     {
                     #ifdef MAX_MIN_LIGHT_FLASH_LED
                     onoffcnt=2;
                     #endif
                     gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;
                     gLedContrl.LED_bright=255;
                     #ifdef BEEP_IO
                     deep_timer_start(1);
                     #endif

                     break;
                     }
                     if(gLedContrl.LED_userbright>MAX_VOICE_BRIGHT_VALUE)
                     gLedContrl.LED_userbright=MAX_VOICE_BRIGHT_VALUE;
                     gLedContrl.LED_disp_mode=LED_NOR;
                      ledfadeTimer=5;
                     LED_Bright_flag=1;
                     led_bright_dir_temp=gLedContrl.LED_bright;
                     LED_Bright_fade_set=voice_bright_vaule[gLedContrl.LED_userbright];
                    printf("voice 	LED_bright%d   LED_userbright=%d  LED_Bright_fade_set=%d\n",gLedContrl.LED_bright,gLedContrl.LED_userbright,LED_Bright_fade_set);
                     gLedContrl.LED_on_off=1;

		  break;


                case        MSG_LED_BRIGHT_LOOP:
		case MSG_LED_BRIGHT_UP:





		printf("-gLedContrl.LED_bright=%d \n",gLedContrl.LED_bright);
         	    if(gLedContrl.LED_disp_mode!=LED_NOR)
         	    	{
         	    	if(gLedContrl.LED_bright<bright_vaule[MIN_BRIGHT])
         		gLedContrl.LED_bright=bright_vaule[MIN_BRIGHT];
			  if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
			  	{
                            gLedContrl.LED_G_buf=255;
			  	}
		      gLedContrl.LED_disp_mode=LED_NOR;
         	    	}

#ifdef LED_Y_PORT
		    else if(is_led_RGB==0&& gLedContrl.LED_bright==MAX_BRIGHT)
		     	{
		printf("1LED_W_buf=%d  LED_Y_buf=%d     \n",gLedContrl.LED_W_buf,gLedContrl.LED_Y_buf);
		 if((gLedContrl.LED_Y_buf+gLedContrl.LED_W_buf)>=(DEFAUFT_MAX_WY*2))
		 	{

                    break;
		 	}

                   if(gLedContrl.LED_Y_buf>=255||gLedContrl.LED_W_buf>=255)
                   	{
		   break;
                   	}

                  if(gLedContrl.LED_Y_buf==0&&gLedContrl.LED_W_buf)
                  	{
			 gLedContrl.LED_bright=gLedContrl.LED_W_buf;
			  gLedContrl.LED_W_buf=255;
                  	}

	           else  if(gLedContrl.LED_W_buf==0&&gLedContrl.LED_Y_buf)
                  	{
			 gLedContrl.LED_bright=gLedContrl.LED_Y_buf;
			  gLedContrl.LED_Y_buf=255;
                  	}

             else
			 	{
		     	  if(gLedContrl.LED_Y_buf>gLedContrl.LED_W_buf&&gLedContrl.LED_W_buf)
		     	  	{
                            gLedContrl.LED_bright=gLedContrl.LED_Y_buf*255/DEFAUFT_MAX_WY;
				temp=255/ gLedContrl.LED_bright;
            			  gLedContrl.LED_W_buf=gLedContrl.LED_W_buf*255/gLedContrl.LED_bright;
			        gLedContrl.LED_Y_buf=DEFAUFT_MAX_WY;
		     	  	}
			else if(gLedContrl.LED_W_buf&&gLedContrl.LED_Y_buf)
				{

				gLedContrl.LED_bright=gLedContrl.LED_W_buf*255/DEFAUFT_MAX_WY;
				 temp=255/ gLedContrl.LED_bright;
				 gLedContrl.LED_Y_buf=gLedContrl.LED_Y_buf*temp;
				 gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;


                           /* gLedContrl.LED_bright=gLedContrl.LED_W_buf*255/DEFAUFT_MAX_WY;
            			  gLedContrl.LED_Y_buf=gLedContrl.LED_Y_buf*DEFAUFT_MAX_WY/gLedContrl.LED_bright;
			        gLedContrl.LED_W_buf=DEFAUFT_MAX_WY;*/
				}
              	}

		printf("3 LED_W_buf=%d  LED_Y_buf=%d   LED_bright=%d  \n",gLedContrl.LED_W_buf,gLedContrl.LED_Y_buf,gLedContrl.LED_bright);


		     	}

#endif
                if(LED_Bright_flag==0)


			gLedContrl.LED_userbright=get_nearest_userbright(gLedContrl.LED_bright);



		if(gLedContrl.LED_userbright<MAX_BRIGHT_VALUE)
			gLedContrl.LED_userbright++;
		else if(Msg==MSG_LED_BRIGHT_LOOP)
			{
			  gLedContrl.LED_userbright=MIN_BRIGHT;
			  printf("MSG_LED_BRIGHT_LOOP=%d \n",gLedContrl.LED_bright);

			}
		else
			{

			printf("MAX_BRIGHT_VALUE=%d \n",MAX_BRIGHT_VALUE);
			gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;
                      gLedContrl.LED_bright=255;
                      #ifdef BEEP_IO
                      	deep_timer_start(2);
                       #endif
                        break;
			}


		if(gLedContrl.LED_userbright>MAX_BRIGHT_VALUE)
		gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;

		gLedContrl.LED_disp_mode=LED_NOR;
		gLedContrl.LED_on_off=1;


		ledfadeTimer=5;
		LED_Bright_flag=1;
		led_bright_dir_temp=gLedContrl.LED_bright;
		LED_Bright_fade_set=bright_vaule[gLedContrl.LED_userbright];


		printf("  LED_bright%d   LED_userbright=%d  LED_Bright_fade_set=%d\n",gLedContrl.LED_bright,gLedContrl.LED_userbright,LED_Bright_fade_set);
		gLedContrl.LED_on_off=1;
		break;
		case MSG_LED_VOICE_BRIGHT_DOWM:

		  if( gLedContrl.LED_on_off==0)
		  break;


                   if(LED_Bright_flag==0)
                   gLedContrl.LED_userbright=get_voice_nearest_userbright(gLedContrl.LED_bright);

                   gLedContrl.LED_disp_mode=LED_NOR;
                   if(gLedContrl.LED_userbright>MAX_VOICE_BRIGHT_VALUE)
                   gLedContrl.LED_userbright=MAX_VOICE_BRIGHT_VALUE;
                   if(gLedContrl.LED_userbright>MIN_BRIGHT)
                   gLedContrl.LED_userbright--;
                   #ifdef MAX_MIN_LIGHT_FLASH_LED
                   else
                   	{
                       #ifdef BEEP_IO
			 deep_timer_start(1);
			   #endif
                      onoffcnt=2;
                   	}
                   #endif
                   printf("--------------gLedContrl.LED_userbright=%d \n",gLedContrl.LED_userbright);
                   gLedContrl.LED_disp_mode=LED_NOR;
                   gLedContrl.LED_on_off=1;
                   ledfadeTimer=5;
                   LED_Bright_flag=1;
                   led_bright_dir_temp=gLedContrl.LED_bright;
                   LED_Bright_fade_set=voice_bright_vaule[gLedContrl.LED_userbright];
                   gLedContrl.LED_on_off=1;

		  break;

		case MSG_LED_BRIGHT_DOWM:





		printf("-gLedContrl.LED_bright=%d  gLedContrl.LED_Y_buf =%d gLedContrl.LED_W_buf=%d\n",gLedContrl.LED_bright,gLedContrl.LED_Y_buf,gLedContrl.LED_W_buf);
            	    if(gLedContrl.LED_disp_mode!=LED_NOR)
         	    	{
         	     	    	if(gLedContrl.LED_bright<bright_vaule[MIN_BRIGHT])
         		gLedContrl.LED_bright=bright_vaule[MIN_BRIGHT];
			  if(gLedContrl.LED_R_buf==0&&gLedContrl.LED_G_buf==0&&gLedContrl.LED_B_buf==0)
			  	{
                            gLedContrl.LED_G_buf=255;
			  	}

         	    	}



		if(LED_Bright_flag==0)
		   gLedContrl.LED_userbright=get_nearest_userbright(gLedContrl.LED_bright);

		gLedContrl.LED_disp_mode=LED_NOR;
		if(gLedContrl.LED_userbright>MAX_BRIGHT_VALUE)
			gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;
		if(gLedContrl.LED_userbright>MIN_BRIGHT)
			gLedContrl.LED_userbright--;
		#ifdef MAX_MIN_LIGHT_FLASH_LED
		else if(Msg==MSG_LED_VOICE_BRIGHT_DOWM)
			   onoffcnt=2;
		#endif
		printf("--------------gLedContrl.LED_userbright=%d \n",gLedContrl.LED_userbright);
		gLedContrl.LED_disp_mode=LED_NOR;
		gLedContrl.LED_on_off=1;
		ledfadeTimer=5;
		LED_Bright_flag=1;
		led_bright_dir_temp=gLedContrl.LED_bright;
		LED_Bright_fade_set=bright_vaule[gLedContrl.LED_userbright];
		gLedContrl.LED_on_off=1;
		break  ;


#ifndef DISABLE_RGB_LED

		case MSG_LED_VOICE_DISP_MODE:


			gLedContrl.LED_W_buf=0;
			gLedContrl.LED_Y_buf=0;
			gLedContrl.LED_on_off=1;
			gLedContrl.LED_flash_time=DEFAUFT_FLASH_SPEED;
			if(gLedContrl.LED_disp_mode==LED_FLASH_STACK)
				{
				 gLedContrl.LED_disp_mode=LED_ColorfulMode;
				 gLedContrl.LED_flash_time=100;
				}

			else if(gLedContrl.LED_disp_mode==LED_ColorfulMode)
			{
			gLedContrl.LED_disp_mode=LED_COLOR_CHAGE_FADE;

			}
			else if(gLedContrl.LED_disp_mode==LED_COLOR_CHAGE_FADE)
				{
				gLedContrl.LED_disp_mode=LED_COLOR_CHAGE;

				}
                           else
                           	{
                           	    gLedContrl.LED_disp_mode=LED_FLASH_STACK;

                           	}
			led_mode_display_int();
			break  ;


		case MSG_LED_COLOR_FULL_CHANGE:

		gLedContrl.LED_W_buf=0;
		gLedContrl.LED_Y_buf=0;
		gLedContrl.LED_on_off=1;

		if(gLedContrl.LED_flash_time==30)
			gLedContrl.LED_flash_time=10;
		else
		gLedContrl.LED_flash_time=30;

		gLedContrl.LED_disp_mode=LED_ColorfulMode;
		led_mode_display_int();

		printf("LED_ColorfulMode==%d!\r\n",LED_Bright_fade_set);
		break  ;




		case MSG_LED_COLOR_CHAGE_FAD:


		gLedContrl.LED_W_buf=0;
		gLedContrl.LED_Y_buf=0;
		gLedContrl.LED_on_off=1;
		gLedContrl.LED_flash_time=DEFAUFT_FLASH_SPEED;
		gLedContrl.LED_disp_mode=LED_COLOR_CHAGE_FADE;
		led_mode_display_int();

		printf("MSG_LED_COLOR_CHAGE_FAD==%d!\r\n",gLedContrl.LED_flash_time);
		break  ;



       case MSG_LED_REG:
				   gLedContrl.LED_on_off=1;
                          	 LED_R_fade_set=255;
                     	 LED_G_fade_set=0;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                  #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
                  			gLedContrl.LED_bright=MAX_BRIGHT;
                   #endif
				gLedContrl.LED_disp_mode=LED_NOR;
	                           	break  ;

                        case MSG_LED_REG1:
				gLedContrl.LED_on_off=1;
                          	 LED_R_fade_set=255;
                     	 LED_G_fade_set=64;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                                   #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
					 gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
	                           	break  ;
			          case MSG_LED_REG2:
				 gLedContrl.LED_on_off=1;
                          	 LED_R_fade_set=255;
                     	 LED_G_fade_set=128;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                         #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
	                           	break  ;
                        case MSG_LED_REG3:
							gLedContrl.LED_on_off=1;
                          	 LED_R_fade_set=255;
                     	 LED_G_fade_set=156;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                          #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
				  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
	                           	break  ;


		                case MSG_LED_crimson:
                              LED_R_fade_set=220;
                              LED_G_fade_set=20;
                              LED_B_fade_set=60;
                              LED_Y_fade_set=0;
                              LED_W_fade_set=0;
                              LED_WYRGB_fade_flag=1;
							  gLedContrl.LED_on_off=1;
                             #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
				 gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
                              gLedContrl.LED_disp_mode=LED_NOR;
                              break  ;
                              case MSG_LED_salmon:

                              LED_R_fade_set=200;
                              LED_G_fade_set=180;
                              LED_B_fade_set=0;
                              LED_Y_fade_set=0;
                              LED_W_fade_set=0;
                              LED_WYRGB_fade_flag=1;
			     #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
				gLedContrl.LED_bright=MAX_BRIGHT;
                                  #endif
                              gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
                              break  ;

                             case MSG_LED_NATURAL:
                             #ifdef	 DISABLE_W_LED
                             LED_R_fade_set=200;
                             LED_G_fade_set=128;
                             LED_B_fade_set=255;
                             LED_Y_fade_set=0;
                             LED_W_fade_set=0;
			       #else
				LED_R_fade_set=0;
				LED_G_fade_set=0;
				LED_B_fade_set=0;
				LED_Y_fade_set=128;
				LED_W_fade_set=128;
				#endif
				gLedContrl.LED_on_off=1;

                             LED_WYRGB_fade_flag=1;
                          #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
	                gLedContrl.LED_bright=MAX_BRIGHT;
			#endif
                             gLedContrl.LED_disp_mode=LED_NOR;
                             break	;

                           case MSG_LED_WHITE:
                           #ifdef	DISABLE_W_LED
                           LED_R_fade_set=255;
                           LED_G_fade_set=255;
                           LED_B_fade_set=255;
                           LED_Y_fade_set=0;
                           LED_W_fade_set=0;
			   #else
			   LED_R_fade_set=0;
			   LED_G_fade_set=0;
			   LED_B_fade_set=0;
			   LED_Y_fade_set=0;
			   LED_W_fade_set=255;
			   #endif



                           LED_WYRGB_fade_flag=1;
			   gLedContrl.LED_on_off=1;
			  #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
                           gLedContrl.LED_bright=MAX_BRIGHT;
			  #endif
                           gLedContrl.LED_disp_mode=LED_NOR;
						   printf("MSG_LED_WHITE \r\n");


                           break  ;
                           case MSG_LED_gold:

                           LED_R_fade_set=255;
                           LED_G_fade_set=156;
                           LED_B_fade_set=0;
                           LED_Y_fade_set=0;
                           LED_W_fade_set=0;
                           LED_WYRGB_fade_flag=1;
                                 #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						   gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
                           gLedContrl.LED_disp_mode=LED_NOR;
						   gLedContrl.LED_on_off=1;
                           break  ;


                        case MSG_LED_REG4:

                          	 LED_R_fade_set=255;
                     	 LED_G_fade_set=255;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;


   			case MSG_LED_GREEG:

       	             LED_R_fade_set=0;
                     	 LED_G_fade_set=255;
                             LED_B_fade_set=0;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
						  gLedContrl.LED_on_off=1;
                   #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
					 gLedContrl.LED_bright=MAX_BRIGHT;
                    #endif
				gLedContrl.LED_disp_mode=LED_NOR;
                 	break  ;
            case MSG_LED_GREEG1:

       	             LED_R_fade_set=0;
                     	 LED_G_fade_set=255;
                             LED_B_fade_set=64;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
						  gLedContrl.LED_on_off=1;
                        #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
                 	break  ;
			case		MSG_LED_cyan:
			case MSG_LED_GREEG2:

       	             LED_R_fade_set=0;
                     	 LED_G_fade_set=255;
                             LED_B_fade_set=128;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                     #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
			gLedContrl.LED_bright=MAX_BRIGHT;
                       #endif
				gLedContrl.LED_disp_mode=LED_NOR;
                 	break  ;

      			case MSG_LED_GREEG3:

       	             LED_R_fade_set=0;
                     	 LED_G_fade_set=255;
                             LED_B_fade_set=255;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
						  gLedContrl.LED_on_off=1;
                                 #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
                 	break  ;
				case MSG_LED_GREEG4:
				 printf("MSG_LED_BLUE4\r\n");
				 LED_R_fade_set=0;
				LED_G_fade_set=255;
				LED_B_fade_set=255;
				LED_Y_fade_set=0;
				LED_W_fade_set=0;
				LED_WYRGB_fade_flag=1;
                      #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
				gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
			gLedContrl.LED_disp_mode=LED_NOR;
			break  ;



   			case MSG_LED_BLUE:

			             LED_R_fade_set=0;
                     	 LED_G_fade_set=0;
                             LED_B_fade_set=255;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
#ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
		  gLedContrl.LED_bright=MAX_BRIGHT;
 #endif
				gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
	                                       	break  ;

	            case MSG_LED_BLUE1:


			             LED_R_fade_set=64;
                     	 LED_G_fade_set=0;
                             LED_B_fade_set=255;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                          #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
	                                       	break  ;

	              case MSG_LED_BLUE2:

	 		             LED_R_fade_set=128;
                     	 LED_G_fade_set=0;
                             LED_B_fade_set=255;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                           #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
				  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
	                                       	break  ;

	            case MSG_LED_BLUE3:

			             LED_R_fade_set=255;
                     	 LED_G_fade_set=0;
                             LED_B_fade_set=255;
                          	  LED_Y_fade_set=0;
                     	  LED_W_fade_set=0;
                     	  LED_WYRGB_fade_flag=1;
                           #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
						  gLedContrl.LED_bright=MAX_BRIGHT;
				  #endif
				gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
	                                       break  ;
	 case MSG_LED_BLUE4:

		 printf("MSG_LED_BLUE4\r\n");
		LED_R_fade_set=255;
	LED_G_fade_set=0;
	 LED_B_fade_set=255;
	LED_Y_fade_set=0;
	LED_W_fade_set=0;
	LED_WYRGB_fade_flag=1;
         #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
	gLedContrl.LED_bright=MAX_BRIGHT;
	  #endif
	gLedContrl.LED_disp_mode=LED_NOR;
				gLedContrl.LED_on_off=1;
	break  ;
	 case MSG_LED_YELLOW:
				 gLedContrl.LED_on_off=1;
						   LED_R_fade_set=255;
					   LED_G_fade_set=255;
						   LED_B_fade_set=0;
							LED_Y_fade_set=0;
						LED_W_fade_set=0;
						LED_WYRGB_fade_flag=1;
          #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
			 gLedContrl.LED_bright=MAX_BRIGHT;
           #endif
			  gLedContrl.LED_disp_mode=LED_NOR;
	break  ;
	 case MSG_LED_PURPLE:
				 gLedContrl.LED_on_off=1;
						   LED_R_fade_set=255;
					   LED_G_fade_set=0;
						   LED_B_fade_set=255;
							LED_Y_fade_set=0;
						LED_W_fade_set=0;
						LED_WYRGB_fade_flag=1;
         #ifndef  USB_RGB_LIGHT_SAVE_LAST_LED
         		gLedContrl.LED_bright=MAX_BRIGHT;
          #endif

			  gLedContrl.LED_disp_mode=LED_NOR;
	break  ;
#endif
	}

}



#ifdef BYLE_kws_music_led
#if defined(KWS_light_libC1)
#include "byle_kws_light_libC1.h"
#endif

#define USER_IO_ACTION
static u16 light_timer=0;
u8 wakup_flag=0;
static u16 user_io_setlight_timer=0;
static u16 user_io_set_timer_out_flag=BYLE_ASR_TIMEOUT;
#ifdef MUSIC_MODE_NEED_REPEAT_CMD
static u16 repeat_cmd_timer=0;
static u8 last_cmd=0;

#endif
void user_send_key_msg(u16 key_event)
{
 /*   struct sys_event e;
    e.type = SYS_KEY_EVENT;
	e.u.key.type = KEY_DRIVER_TYPE_USER;
    e.u.key.event = key_event;
    e.u.key.value = 0;
    e.u.key.msg = 0;
    sys_event_notify(&e);
*/
//	 MusicLEDMsgProccess(key_event);
// post_msg(1,key_event);

app_task_put_key_msg(key_event);
}




void user_gpio_set_time_out(void)
{
	user_io_set_timer_out_flag=0;
	user_io_setlight_timer=0;
	
      #ifndef  DIASBLE_eCMD_setlight
	  if(light_timer)
	 sys_hi_timer_del(light_timer);
	light_timer=0;
	#endif
     #ifdef MUSIC_MODE_NEED_REPEAT_CMD
	 
	 last_cmd=0;
	if(repeat_cmd_timer)
	   sys_hi_timer_del(repeat_cmd_timer);
	repeat_cmd_timer=0;
	#endif
         #ifdef ASR_FLASH_LED
           asr_led_flash_finish();
         #endif

printf("user_gpio_set_time_out=%d\n",user_io_set_timer_out_flag);

}
void light_timer_time_out(void)
{
	light_timer=0;
printf("light_timer_time_out=%d\n",light_timer);

}

void timer_reset(u8 sec)
{
 user_io_set_timer_out_flag=sec;
 if(user_io_setlight_timer)
sys_hi_timeout_del(user_io_setlight_timer);       				        
 if(sec)
 	{

user_io_setlight_timer=sys_hi_timeout_add(NULL, user_gpio_set_time_out, 1000*sec);
 	}
printf("timer_reset=%d\n",user_io_set_timer_out_flag);
#if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)  			
byle_kws_timer_out_set(BYLE_ASR_TIMEOUT);   		
#endif     			

}

void send_led_msg(uint8_t cmd_index)
{

#if defined(DISABLE_RGB_LED)
             #if defined(LED_Y_PORT)
             
             if(music_led_wy_msg_table[cmd_index]!=KEY_NULL)
             {
				 printf("music_led_wy_msg_table333333 %d\n",cmd_index);
             
             user_send_key_msg(music_led_wy_msg_table[cmd_index]);
		 timer_reset(BYLE_ASR_TIMEOUT);
             }
             #else
             if(music_led_w_msg_table[cmd_index]!=KEY_NULL)
             {
             
			 printf("music_led_w_msg_table333333 %d\n",cmd_index);
             user_send_key_msg(music_led_w_msg_table[cmd_index]);
		 timer_reset(BYLE_ASR_TIMEOUT);
             }	
             #endif
#else
                #if defined(LED_Y_PORT)
                if(music_led_wyrgb_msg_table[cmd_index]!=KEY_NULL)
                {
                user_send_key_msg(music_led_wyrgb_msg_table[cmd_index]);
		timer_reset(BYLE_ASR_TIMEOUT);
                }	
                #elif   defined(LED_W_PORT)
                if(music_led_wrgb_msg_table[cmd_index]!=KEY_NULL)
                {
                user_send_key_msg(music_led_wrgb_msg_table[cmd_index]);
		timer_reset(BYLE_ASR_TIMEOUT);
                }	
                #else
                 if(Byle_kws_msg_table[cmd_index]!=KEY_NULL)
                 {
                 user_send_key_msg(Byle_kws_msg_table[cmd_index]);
                 timer_reset(BYLE_ASR_TIMEOUT);
                 }	
                #endif
#endif

}




int kws_led_handle_action( u16 cmd_index)
	{
	
	u16 msg=KEY_NULL;
	static u8 last_color=0;
	u8 timer_reset_en=0;
		printf("lite_music_led=%d \n",cmd_index);

#if(KWS_CMD_MODE==ENGINE_KWS_MODE_CMD)

#ifdef MUSIC_MODE_NEED_REPEAT_CMD
	if(is_music_mode()&&user_io_set_timer_out_flag==0&&light_timer==0)
	{
		
		 if(repeat_cmd_timer)
		 	{
		 	
			user_io_set_timer_out_flag=1;
		       sys_hi_timer_del(repeat_cmd_timer);
		 	}
		repeat_cmd_timer=sys_hi_timeout_add(NULL, user_gpio_set_time_out, 1000*3);
		printf("last_cmd=%d cmd_index=%d \n",last_cmd,cmd_index);
		
		if(last_cmd!=cmd_index)
			{
			
			last_cmd=cmd_index;
			 return KEY_NULL ;
			}
	}
#endif
#endif


	switch(cmd_index)
		{
		       case   eCMD_wakeup_uni:
                       
                            #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
                        
							
				 if(is_led_on_off())
							{
				     #ifdef ASR_FLASH_LED
				    asr_led_flash_start();
				        #endif
						
                                   #ifdef WAKE_UP_LED_FLASH
				  led_set_flash(2);
				 #else			   
				      led_set_fade(2,50);//	 led_set_flash(2);
                                #endif 		
								
						  if(light_timer)
							  sys_hi_timer_del(light_timer);
						  light_timer=0;
						  wakup_flag=1;
						//  light_timer=sys_hi_timeout_add(NULL, light_timer_time_out, 1000*5);
							}
				 else
				 	{
                               #ifdef ASR_FLASH_LED
				asr_led_flash_start();
				#endif
				
                                #ifdef WAKE_UP_LED_FLASH
				led_set_flash(3);
				#else
				led_set_fade(2,30);//led_set_flash(3);
				#endif
				 timer_reset(BYLE_ASR_TIMEOUT);
				 	}
                               #ifdef BEEP_IO
					deep_timer_start(1);
				#endif

                              #endif
			
			
				 break;
                            #ifdef USER_open0
				case  USER_open0:
			    #endif
                               #ifdef USER_open2
                               case  USER_open2:
                               #endif
                               #ifdef USER_open3
                               case  USER_open3:
                               #endif
                               #ifdef USER_open4
                               case  USER_open4:
                               #endif
                               
                               #ifdef USER_open5
                               case  USER_open5:
                               #endif

				
				 case	eCMD_default_open:
						printf("open light	\n");
			     if(is_led_on_off()==0&&light_timer==0)
				 {
					 printf("eCMD_default_open \n");
                               

                                 send_led_msg(cmd_index);
				   }
				 
				  break;
       #if(KWS_CMD_MODE==ENGINE_KWS_MODE_CMD)
                #ifndef  DIASBLE_eCMD_setlight
				  case eCMD_sleep30s:
				  case eCMD_sleep10min:
				  case  eCMD_sleep30min:
				  case eCMD_cancelsleepoff:
					  printf("eCMD_TIMER =%d   is_led_on_off()%d\n",light_timer,is_led_on_off());
				  if((light_timer||wakup_flag)&&is_led_on_off())
					 {
					
					send_led_msg(cmd_index);
					timer_reset(0);
					 if(light_timer)
					  sys_hi_timer_del(light_timer);
					 light_timer=0;
					       #ifdef BEEP_IO
					deep_timer_start(1);
						#endif
						wakup_flag=0;
					 }
				 
				   break;
				 #ifdef USER_SET_LIGHT1
				    case  USER_SET_LIGHT1:
				  #endif
				   case  eCMD_setlight:
					   printf("eCMD_setlight   \n");
				 if(is_led_on_off())
				 	{
				 
				 led_set_flash(2);	 
				  if(light_timer)
					  sys_hi_timer_del(light_timer);
				  light_timer=sys_hi_timeout_add(NULL, user_gpio_set_time_out, 5000);
				         #ifdef BEEP_IO
					deep_timer_start(1);
						#endif
				  printf("eCMD_setlight2=%d\n",light_timer);
				 	}
				 
				  break;
				 #endif

                        #ifdef eCMD_setcolor
                          case        eCMD_setcolor:
				  printf("eCMD_setcolor   \n");
               			 if(is_led_on_off())
               			{
				 led_set_flash(2);	 
				timer_reset(BYLE_ASR_TIMEOUT);
               			}
				break;
                          #endif		


       
                              #ifdef ADJ_BRIGHT_REPEAT_CMD
                              #ifdef USER_BRIGHT_UP1
                              case 	USER_BRIGHT_UP1:
                              #endif
                              #ifdef USER_BRIGHT_DOWM1
                              case 	USER_BRIGHT_DOWM1:
                              #endif				
                              case	   eCMD_brightthelight:
                              case	   eCMD_dimthelight:
                              #endif
                              
                              #ifdef CHANGE_COLOR_REPEAT_CMD 
                              case eCMD_changecolor:
                              #ifdef USER_CHANGE_COLOR2
                              case USER_CHANGE_COLOR2:
                              #endif
                              
                              #ifdef USER_CHANGE_COLOR3
                              case USER_CHANGE_COLOR3:
                              #endif
                              #ifdef USER_CHANGE_COLOR4
                              case USER_CHANGE_COLOR4:
                              #endif
                              #endif
                               
                               if(user_io_set_timer_out_flag&&is_led_on_off()&&light_timer==0)
                               {
                               printf("eCMD_setcolor %d  \n",cmd_index);
                               send_led_msg(cmd_index);
                               
                               }
                               
                               else
                               {
                             
                               timer_reset(3);
                               
                               }
                               
                               break;


				case   eCMD_yellow:
				case	  eCMD_white:
				#ifdef USER_WY_COLOR_REPEAT	
				case	  eCMD_coolmode:
				case	  eCMD_warmmode:
				case	  eCMD_sunmode:
				#ifdef USER_SUNMODE1
				case	  USER_SUNMODE1:
                                #endif
                               #endif
					
                                  #if (defined(DISABLE_RGB_LED)&&defined(LED_Y_PORT))
						{
						     if(user_io_set_timer_out_flag&&is_led_on_off()&&light_timer==0)
							{
							
							printf("eCMD_setcolor22 %d  \n",cmd_index);
							send_led_msg(cmd_index);

							}
							 else
							{
							
							printf("timer_reset 33%d  \n",cmd_index);
							 timer_reset(3);
							 
						        }
						break;
						}

                                   #endif
					
				case   eCMD_green:
                                case   eCMD_blue:
				 case   eCMD_red:
				case   eCMD_purple:
				case    eCMD_cyan:
			                  if(user_io_set_timer_out_flag&&is_led_on_off()&&light_timer==0)
						{
						  printf("eCMD_setcolor %d	\n",cmd_index);
						 send_led_msg(cmd_index);
										   
						}
        
					else
						{

            					#ifndef DISABLE_RGB_LED
            					  timer_reset(3);
                                                     #endif
					
						}
						 
						 break;
				#endif
				default:
					#ifdef MAX_LED_CMD_INDEX
				if(cmd_index>MAX_LED_CMD_INDEX)
					break;
				#endif	
			    if(is_led_on_off()&&light_timer==0)
              			{
					printf("eCMD_led5555 %d \n",cmd_index); 		
                                send_led_msg(cmd_index);
				
              			}
			  break;
									

		}
	return KEY_NULL ;

}
#endif

void updata_led_to_app(void)
{

}


#endif

