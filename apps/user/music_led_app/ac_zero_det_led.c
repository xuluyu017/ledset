


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
#include "port_wkup.h"

#include "user_pwm.h"
#include "byle_kws_app.h"
#include "byle_msg.h"
#include "timer_app.h"


#ifdef AC_zero_port
#include "ac_zero_det_led.h"

AC_LED_INFO  gLedContrl;
uint8_t LEDbrightdata=0;
u8 led_bright_dir_temp;
u8 fadedir=0;
u8 MyLed_save_Cnt=0;
u8 ledoffcnt=0;


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
	 255,
};






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
	vm_write(VM_LED_INFO, &gLedContrl,sizeof(AC_LED_INFO));
  //  local_irq_enable();
}

void led_read_info(void)
{
    int ret = 0;
 ret=vm_read(VM_LED_INFO, &gLedContrl,sizeof(AC_LED_INFO));
 if (ret !=sizeof(AC_LED_INFO))
  {
        puts("led_info is null\n");
        memset(&gLedContrl,0x00,sizeof(AC_LED_INFO));
    }

	//puts("led_read_info\n");

}
static void led_off_timeout(void)
{

post_msg(MSG_LED_OFF);

}

u16 led_off_timer=0;
void led_sleep_off(u32 second)
{

		if(led_off_timer)
		{
			sys_s_hi_timeout_del(led_off_timer);
			led_off_timer=0;
		}
		if(second)
  
	  led_off_timer = sys_s_hi_timerout_add(NULL,led_off_timeout, second*1000);



		
}

u8		 fadebright=0;
u8 fadecnt=0;
u8       onoffcnt=0;

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

void led_pwm_updata(void)
{

  gLedContrl.LED_on_off=1;
    LEDbrightdata=gLedContrl.LED_bright;
}


void led_bright_set(uint8_t bright)
{
LEDbrightdata=bright;
}

void led_on_off(bool onoff_flag)
{
gLedContrl.LED_on_off=onoff_flag;

if(onoff_flag)
        {

        led_pwm_updata();
        }
else
         {
        LEDbrightdata=0;
         }
	 
	 
}
bool  is_led_on_off(void)
{
	return gLedContrl.LED_on_off;
}




#if  0




#define		AC_zero_PORT_INT	GPIO_B_INT//GPIO_A_INT 		
#define		AC_zero_BIT		(1 << 4)//(1 << 10)	//GPIO_B[21] for signal A 
		
		
#define		AC_ZERO_DET_PORT	   GPIOB//  GPIOA
#define		AC_ZERO_DET_PIN	GPIOB4//	GPIOA10
#define		AC_ZERO_DET_PIN_INIT()	nspGpioInit(AC_ZERO_DET_PORT, AC_ZERO_DET_PIN, GPIO_MODE_IN_PU_NO_DS)
#define		GET_AC_ZERO_DET_STATUS()			(GpioGetReg(AC_ZERO_DET_PORT) & AC_ZERO_DET_PIN)

		
// Initialize coding key scan (GPIO) operation.
// Config interrupt at negative edge of signal-A
void ac_zero_LedCtlInt(void)
{
   AC_ZERO_DET_PIN_INIT();
   LED_BRIGHT_PIN_INIT();

	 
	//enable int
	GpioIntEn(AC_zero_PORT_INT, AC_zero_BIT, GPIO_NEG_EDGE_TRIGGER);

		NVIC_EnableIRQ(GPIO_IRQn);
		
	NVIC_EnableIRQ(TMR0_IRQn);
	Timer0Set(100);//100ms
        Timer0HaltModeSet(1);    //halt mode

}

//timer1 int fucntion
static uint16_t Timer0CountNum = 0;
__attribute__((section(".driver.isr"))) void Timer0Interrupt(void)
{

	Timer0IntClr();
	if(AC_zero)
      AC_cnt++;	
	
	if(Timer0CountNum)
	{
	Timer0CountNum--;
		if(Timer0CountNum==5)
			{
		          LED_BRIGHT_PIN_HI();
			}
		else    if(Timer0CountNum==0)
		LED_BRIGHT_PIN_LW();
	}
}
// Coding key signal-A interrupt.
__attribute__((section(".driver.isr"))) void GpioInterrupt(void)
{
	if(GpioIntFlagGet(AC_zero_PORT_INT) == AC_zero_BIT)
		{
		if(AC_cnt2++>100)
			{
   		     if(AC_zero==0)
   		     	{
	 	       AC_cnt_data=AC_cnt/100-20;
			   AC_cnt=0;
			AC_zero=1;
		      // APP_DBG("AC_cnt_data:%d\r\n", AC_cnt_data);	
   		     	}
			 else    
			 AC_zero=0;	

			 
			AC_cnt2=0;   
		   }
GpioIntClr(AC_zero_PORT_INT, AC_zero_BIT);
if(GET_AC_ZERO_DET_STATUS())
{
	GpioIntEn(AC_zero_PORT_INT, AC_zero_BIT, GPIO_NEG_EDGE_TRIGGER);
}
else
{
	GpioIntEn(AC_zero_PORT_INT, AC_zero_BIT, GPIO_POS_EDGE_TRIGGER);
}

 if(LEDbrightdata==MAX_BRIGHT)
 	{
 	Timer0CountNum=0;
      		LED_BRIGHT_PIN_HI();
 	}
else if(LEDbrightdata)
     {
     
     		LED_BRIGHT_PIN_LW();

         Timer0CountNum= AC_cnt_data-LEDbrightdata*AC_cnt_data/MAX_BRIGHT+5;
		 
		 	//	   APP_DBG("Timer0CountNum:%d\r\n", Timer0CountNum);	

     }
else
{
 	Timer0CountNum=0;

     		LED_BRIGHT_PIN_LW();
}

	
}
}
#endif


uint8_t    AC_zero=1;
uint16_t   AC_cnt=0;
uint16_t   AC_cnt_data=0;
uint16_t   AC_cnt2=0;
static uint16_t TimerCountNum = 0;

#define		AC_ZERO_DET_PIN_INIT()	          { gpio_set_die(AC_zero_port,0);gpio_set_direction(AC_zero_port,1);gpio_set_pull_down(AC_zero_port,0);gpio_set_pull_up(AC_zero_port,0); }
#define		GET_AC_ZERO_DET_STATUS()	   gpio_read(AC_zero_port) 

#define		LED_BRIGHT_PIN_INIT()	{ gpio_set_die(LED_W_PORT,0);gpio_set_direction(LED_W_PORT,0);gpio_direction_output(LED_W_PORT,0); }
#define  LED_BRIGHT_PIN_HI()  {gpio_direction_output(LED_W_PORT,1); }
#define  LED_BRIGHT_PIN_LW()  {gpio_direction_output(LED_W_PORT,0); }



#define  PWM_TIMER   JL_TIMER1
#define  PWM_IRQ_TIME_IDX                    IRQ_TIME1_IDX
#define PWM_TIMER_UNIT_US     100//  50// 250  //��λus

SET(interrupt(""))
static void pwm_timer_isr(void)
{

           static u8 pwm_cnt=0;
           PWM_TIMER->CON |= BIT(14);
           if(AC_zero)
           AC_cnt++; 
           
           if(TimerCountNum)
           {
           TimerCountNum--;
                if(TimerCountNum==5)
                {
                LED_BRIGHT_PIN_HI();
                }
                else    if(TimerCountNum==0)
                LED_BRIGHT_PIN_LW();
           }


		   
}

static void pwm_timer_open(void)
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


#define GPIO_NEG_EDGE_TRIGGER 1
#define GPIO_POS_EDGE_TRIGGER 0

void ac_zero_port_irq(void)
{

		if(AC_cnt2++>100)
			{
			 if(AC_zero==0)
				{
			   AC_cnt_data=AC_cnt/100-20;
			   AC_cnt=0;
			AC_zero=1;
			  // APP_DBG("AC_cnt_data:%d\r\n", AC_cnt_data);	
				}
			 else	 
			 AC_zero=0; 

			 
			AC_cnt2=0;	 
		   }
		
       	if(GET_AC_ZERO_DET_STATUS())
       	{
       		set_port_interrupt_edge(AC_zero_port,GPIO_NEG_EDGE_TRIGGER);
       	}
       	else
       	{
       		set_port_interrupt_edge(AC_zero_port,  GPIO_POS_EDGE_TRIGGER);
       	}
	
	 if(LEDbrightdata==MAX_BRIGHT)
		{
		             TimerCountNum=0;
				LED_BRIGHT_PIN_HI();
		}
	else if(LEDbrightdata)
		 {
		 
			LED_BRIGHT_PIN_LW();
			 TimerCountNum= AC_cnt_data-LEDbrightdata*AC_cnt_data/MAX_BRIGHT+5;
			 
				//	   APP_DBG("Timer0CountNum:%d\r\n", Timer0CountNum);	
	
		 }
	else
	{
		TimerCountNum=0;
	
		LED_BRIGHT_PIN_LW();
	}
	
		
}




void set_port_interrupt_edge(u32 port, u8 edge)
{
	u8 wkup_s = port;
	if (port >= IO_PORT_DP) {
	wkup_s = port - IO_PORT_DP + 15;
	}

      if (edge) {
      	JL_WAKEUP->CON1 |= BIT(wkup_s);  //detect falling edge
      	gpio_set_pull_up(port, 1);
      	gpio_set_pull_down(port, 0);
      } else {
      	JL_WAKEUP->CON1 &= ~BIT(wkup_s);  //detect rising edge
      	gpio_set_pull_up(port, 0);
      	gpio_set_pull_down(port, 1);
      }

}


void ac_zero_Led_ioInt(void)
{
       LED_BRIGHT_PIN_INIT();

}

void AC_zero_det_led_MsgProccess(int Msg)
{
	u16 temp;

        if(Msg<MSG_LED_MIN||Msg>MSG_LED_MAX)
        {


          return ;

        }


	MyLed_save_Cnt=3;

	printf("acLEDMsgProccess:%d\r\n", Msg);

	switch(Msg)
	{


			  case	MSG_LED_SLEEP_CANSEL:


					   ledoffcnt=0;
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
			  gLedContrl.LED_bright=LITTLE_LIGHT_BRIGHT;
			 printf("MSG_LED_USB_NIGHT_ON==%d\n",gLedContrl.LED_on_off);
			 gLedContrl.LED_on_off=1;
			 led_on_off(gLedContrl.LED_on_off);
               printf("-MSG_LED_USB_NIGHT_ON55555555555 ");

		break  ;


		case MSG_LED_ON:
		if(gLedContrl.LED_on_off==0)
             {

			gLedContrl.LED_on_off=1;
			led_on_off(gLedContrl.LED_on_off);
       		}

			break  ;

		case MSG_LED_OFF:
			printf("---------- led_off msg ");

			gLedContrl.LED_on_off=0;
			led_on_off(gLedContrl.LED_on_off);
			break  ;


		case MSG_LED_ON_OFF:

		printf("LED_on_off==%d!\r\n",gLedContrl.LED_on_off);

		if(gLedContrl.LED_on_off)
		{
		gLedContrl.LED_on_off=0;
		}
		else
		{
		gLedContrl.LED_on_off=1;
		}
		led_on_off(gLedContrl.LED_on_off);
		break  ;


		case MSG_LED_VOICE_MAX_BRIGHT:
			 if(gLedContrl.LED_on_off==0)
			 break;
		case	 MSG_LED_MAX_BRIGHT:
		case MSG_LED_BRIGHT_SET_100PERCENT:
           gLedContrl.LED_bright=255;
           gLedContrl.LED_on_off=1;
           led_on_off(gLedContrl.LED_on_off);
			break;

		 case	  MSG_LED_MID_BRIGHT:
            gLedContrl.LED_on_off=1;
		    gLedContrl.LED_bright=255/2;
            led_on_off(gLedContrl.LED_on_off);
			break;




	 case MSG_LED_VOICE_MIN_BRIGHT:
            if(gLedContrl.LED_on_off==0)
            break;
            case    MSG_LED_MIN_BRIGHT:
            
            gLedContrl.LED_on_off=1;
            gLedContrl.LED_bright=MIN_BRIGHT;
            led_on_off(gLedContrl.LED_on_off);
            break;

		case MSG_LED_VOICE_BRIGHT_UP:

		    if( gLedContrl.LED_on_off==0)
				break;
			 

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
   
             gLedContrl.LED_bright=voice_bright_vaule[gLedContrl.LED_userbright];
           printf("voice 	LED_bright%d   LED_userbright=%d \n",gLedContrl.LED_bright,gLedContrl.LED_userbright);
            gLedContrl.LED_on_off=1;
   
   		 led_on_off(gLedContrl.LED_on_off);

		  break;


		case MSG_LED_BRIGHT_UP:

		printf("-gLedContrl.LED_bright=%d \n",gLedContrl.LED_bright);
      
		if(gLedContrl.LED_userbright<MAX_BRIGHT_VALUE)
			gLedContrl.LED_userbright++;
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

          if(gLedContrl.LED_userbright>MAX_VOICE_BRIGHT_VALUE)
          gLedContrl.LED_userbright=MAX_VOICE_BRIGHT_VALUE;
          gLedContrl.LED_bright=bright_vaule[gLedContrl.LED_userbright];
          printf(" 	LED_bright%d   LED_userbright=%d \n",gLedContrl.LED_bright,gLedContrl.LED_userbright);
          gLedContrl.LED_on_off=1;
          led_on_off(gLedContrl.LED_on_off);
		break;
		case MSG_LED_VOICE_BRIGHT_DOWM:

		  if( gLedContrl.LED_on_off==0)
		  break;

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
          gLedContrl.LED_bright=voice_bright_vaule[gLedContrl.LED_userbright];
          printf("voice 	LED_bright%d   LED_userbright=%d \n",gLedContrl.LED_bright,gLedContrl.LED_userbright);
          gLedContrl.LED_on_off=1;
          led_on_off(gLedContrl.LED_on_off);

		  break;

		case MSG_LED_BRIGHT_DOWM:
       printf("-gLedContrl.LED_bright=%d \n",gLedContrl.LED_bright);
         if(gLedContrl.LED_bright<bright_vaule[MIN_BRIGHT])
        gLedContrl.LED_bright=bright_vaule[MIN_BRIGHT];
		if(gLedContrl.LED_userbright>MAX_BRIGHT_VALUE)
			gLedContrl.LED_userbright=MAX_BRIGHT_VALUE;
		if(gLedContrl.LED_userbright>MIN_BRIGHT)
			gLedContrl.LED_userbright--;
		#ifdef MAX_MIN_LIGHT_FLASH_LED
		else if(Msg==MSG_LED_VOICE_BRIGHT_DOWM)
			   onoffcnt=2;
		#endif
		   gLedContrl.LED_bright=bright_vaule[gLedContrl.LED_userbright];
          printf(" 	LED_bright%d   LED_userbright=%d \n",gLedContrl.LED_bright,gLedContrl.LED_userbright);
          gLedContrl.LED_on_off=1;
         led_on_off(gLedContrl.LED_on_off);
		break  ;


	}

}
void LED_Power_Handle(void)
{
static  u16 	timer_500ms=0 ,cnt=0;
cnt++;

   if(timer_500ms++>10)
   	{
   	timer_500ms=0;
   if(MyLed_save_Cnt)
	{

		if(--MyLed_save_Cnt==0)
			{

	           	led_save_info();
			}
	}
   	}

}

void ac_zero_det_LedCtlInt(void)
{
	u8 i=0; 
	led_read_info();
	gLedContrl.LED_on_off=1;
	if(gLedContrl.data_check!=0x55)
	{
	gLedContrl.LED_bright=255;

	}
	led_on_off(gLedContrl.LED_on_off);
	//enable int
	port_wkup_enable(AC_zero_port, GPIO_NEG_EDGE_TRIGGER,ac_zero_port_irq);
	pwm_timer_open();
	sys_hi_timer_add(NULL, LED_Power_Handle, 50);
}

#ifdef BYLE_kws_music_led


void music_key_msg(u16 key_event)
{
 /*   struct sys_event e;
    e.type = SYS_KEY_EVENT;
	e.u.key.type = KEY_DRIVER_TYPE_USER;
    e.u.key.event = key_event;
    e.u.key.value = 0;
    e.u.key.msg = 0;
    sys_event_notify(&e);
*/
	 AC_zero_det_led_MsgProccess(key_event);

	//user_send_key_msg(key_event);
}






void send_led_msg(uint8_t cmd_index)
{


             if(music_led_w_msg_table[cmd_index]!=KEY_NULL)
             {

			 printf("acled_w_msg_table %d\n",cmd_index);
             AC_zero_det_led_MsgProccess(music_led_w_msg_table[cmd_index]);
             }

}




int kws_led_handle_action( u16 cmd_index)
	{

	u16 msg=KEY_NULL;
		printf("kws_music_led=%d \n",cmd_index);



	switch(cmd_index)
		{
               #ifdef user_wakeup_uni1
               case	user_wakeup_uni1:
               #endif
		       case   eCMD_wakeup_uni:

                   #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)

     	           #ifdef BEEP_IO
     		      deep_timer_start(1);
                 #else
				   
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
				 	}
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
                 if(is_led_on_off()==0)
                 {
                 printf("eCMD_default_open \n");
                 
                 
                 send_led_msg(cmd_index);
                 }
                 
                 break;
				default:
					#ifdef MAX_LED_CMD_INDEX
				if(cmd_index>MAX_LED_CMD_INDEX)
					break;
				#endif
			    if(is_led_on_off())
              			{
					printf("eCMD_led5555 %d \n",cmd_index);
                                send_led_msg(cmd_index);

              			}
			  break;


		}
	return KEY_NULL ;

}
#endif

#endif

