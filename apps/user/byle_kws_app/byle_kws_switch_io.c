#include "circular_buf.h"
#include "sound_mge.h"
#include "audio_adc_api.h"
#include "audio_adc.h"
#include "msg.h"
#include "wdt.h"
#include "gpio.h"
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[switch_io]"
#include "log.h"

#include "byle_cfg.h"
#include "byle_user.h"
#include "user_pwm.h"
#include "byle_kws_app.h"
#include "byle_msg.h"
#include "timer_app.h"


#ifdef BYLE_KWS_SWITCH
#include "byle_kws_app.h"
#include "byle_kws_switch_io.h"

static u16 user_io_setlight_timer=0;
static u16 user_io_sleep_timer=0;
static u8 back_LED_disp_mode=0;
static u16 user_io_set_timer_out_flag=SET_TIME_OUT;
static u8 power_on_set_timer_out_flag=SET_TIME_OUT;
 byle_switch_global_var_t  switch_global;


void user_gpio_name_time_out(void)
{
	user_io_set_timer_out_flag=0;
	user_io_setlight_timer=0;
	power_on_set_timer_out_flag=0;


log_info("user_gpio_name_time_out=%d\n",user_io_set_timer_out_flag);

}


#ifdef BYLE_KWS_SWITCH_IO
#ifdef BYLE_KWS_SWITCH_IO1
#ifdef BYLE_KWS_SWITCH_IO2

#define BYLE_KWS_SWITCH_INT()  { gpio_set_die(BYLE_KWS_SWITCH_IO,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO, 0);gpio_set_die(BYLE_KWS_SWITCH_IO1,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO1, 0);gpio_set_die(BYLE_KWS_SWITCH_IO2,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO2, 0);}
#define BYLE_KWS_SWITCH_ON()    {gpio_direction_output(BYLE_KWS_SWITCH_IO, 1);gpio_direction_output(BYLE_KWS_SWITCH_IO1, 1);gpio_direction_output(BYLE_KWS_SWITCH_IO2, 1);}
#define BYLE_KWS_SWITCH_OFF()  {gpio_direction_output(BYLE_KWS_SWITCH_IO, 0);gpio_direction_output(BYLE_KWS_SWITCH_IO1, 0);gpio_direction_output(BYLE_KWS_SWITCH_IO2, 0);}
#else
#define BYLE_KWS_SWITCH_INT()  { gpio_set_die(BYLE_KWS_SWITCH_IO,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO, 0);gpio_set_die(BYLE_KWS_SWITCH_IO1,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO1, 0);}
#define BYLE_KWS_SWITCH_ON()    {gpio_direction_output(BYLE_KWS_SWITCH_IO, 1);gpio_direction_output(BYLE_KWS_SWITCH_IO1, 1);}
#define BYLE_KWS_SWITCH_OFF()  {gpio_direction_output(BYLE_KWS_SWITCH_IO, 0);gpio_direction_output(BYLE_KWS_SWITCH_IO1, 0);}

#endif
#else
#define BYLE_KWS_SWITCH_INT()  { gpio_set_die(BYLE_KWS_SWITCH_IO,0);  gpio_set_direction(BYLE_KWS_SWITCH_IO, 0);}
#define BYLE_KWS_SWITCH_ON()    {gpio_direction_output(BYLE_KWS_SWITCH_IO, 1);}
#define BYLE_KWS_SWITCH_OFF()  {gpio_direction_output(BYLE_KWS_SWITCH_IO, 0);}

#endif
#else
#define BYLE_KWS_SWITCH_INT()
#define BYLE_KWS_SWITCH_ON()
#define BYLE_KWS_SWITCH_OFF()
#endif

#ifdef ASR_FLASH_LED
#define ASR_FLASH_LED_INT()  { gpio_set_die(ASR_FLASH_LED,0);  gpio_set_direction(ASR_FLASH_LED, 0);}
#define ASR_FLASH_LED_ON()    gpio_direction_output(ASR_FLASH_LED, 1)
#define ASR_FLASH_LED_OFF()  gpio_direction_output(ASR_FLASH_LED, 0)


static u16 asr_led_flash_timer=0;
static u16 asr_led_flash_cnt=0;


 void asr_led_flash_finish(void)
 {
	 log_info("asr_led_flash_finish=%d\n",asr_led_flash_timer);

  if(asr_led_flash_timer)
 sys_hi_timer_del(asr_led_flash_timer);
 asr_led_flash_timer=0;
 ASR_FLASH_LED_ON();
 }

 void asr_led_flash_isr(void)
{
 static u8 led_flash=0;




 led_flash=~led_flash;
 if(led_flash)
 	{
 	ASR_FLASH_LED_ON();
 	}
else
	{
 	ASR_FLASH_LED_OFF();
 	}


    if(asr_led_flash_cnt)
       {
    		  if(--asr_led_flash_cnt==0)
    		   {
			asr_led_flash_finish();
    		   }

       }

}



void asr_led_flash(u8 flash_cnt, u32 timers)
{
	log_info("asr_led_flash_start=%d\n",asr_led_flash_timer);
asr_led_flash_cnt=flash_cnt*2;
 if(asr_led_flash_timer)
sys_hi_timer_del(asr_led_flash_timer);
asr_led_flash_timer=sys_hi_timer_add(NULL, asr_led_flash_isr, timers);
}


void asr_led_flash_start(void)
{
	log_info("asr_led_flash_start=%d\n",asr_led_flash_timer);

 if(asr_led_flash_timer)
sys_hi_timer_del(asr_led_flash_timer);
asr_led_flash_timer=sys_hi_timer_add(NULL, asr_led_flash_isr, 300);

}



#endif


void byle_switch_gpio_init(void)
{
#ifdef BYLE_KWS_SWITCH_IO

BYLE_KWS_SWITCH_INT();

#endif
#ifdef ASR_FLASH_LED
  ASR_FLASH_LED_INT() ;
  ASR_FLASH_LED_ON();
#endif



  log_info( " %ssuccess", __func__);
}
void byle_switch_power_on(void)
{

#ifdef ASR_FLASH_LED
ASR_FLASH_LED_INT() ;
ASR_FLASH_LED_ON();

#endif



if(MyContrl.power_on_led==0x55)
{

BYLE_KWS_SWITCH_OFF();
switch_global.on_off=0;

}
else
{
BYLE_KWS_SWITCH_ON();
 switch_global.on_off=1;
}


 #ifdef KWS_CMD_MODE
#if(KWS_CMD_MODE==ENGINE_KWS_MODE_CMD)
 user_io_setlight_timer=sys_hi_timeout_add(NULL, user_gpio_name_time_out, 1000*SET_TIME_OUT);
#endif
#endif
switch_global.bright=255;


}

void byle_switch_sleep_time_out(void)
{
	switch_global.on_off=0;
	BYLE_KWS_SWITCH_OFF();
     #ifdef BYLE_APP_TYPE
     	  rsp_byle_switch_status();
     #endif

}





void sleep_off_check(void)
{
	u8 cmd_rsp_buffer[50]={0};
	u8 offset=0, i;

 if(switch_global.sleep_cnt)
 	{
      switch_global.sleep_cnt--;
	  if(switch_global.sleep_cnt==0)
	  	{
	  byle_switch_sleep_time_out();
	  sys_hi_timer_del(user_io_sleep_timer);
	  	}

       // if(switch_global.sleep_cnt%2==0)
 	{
 		cmd_rsp_buffer[offset++] =(switch_global.sleep_cnt&0xff00)>>8;
		cmd_rsp_buffer[offset++] =(u8)switch_global.sleep_cnt;

		log_info("switch_global.sleep_cnt=%d\n",switch_global.sleep_cnt);
		
 	}
 }

}


void  byle_switch_sleep_off(u16 sleep_s)
{

if(user_io_sleep_timer)
sys_hi_timer_del(user_io_sleep_timer);
if(sleep_s)
{
 user_io_sleep_timer=sys_hi_timer_add(NULL, sleep_off_check, 1000);
}
 switch_global.sleep_cnt=sleep_s;

}






void byle_switch_io_high(void)
{
	log_info("byle_switch_io_high \n");
    	switch_global.on_off=1;
      #ifdef BYLE_KWS_SWITCH_IO
      BYLE_KWS_SWITCH_ON() ;
      #endif
      #ifdef BEEP_IO
	  deep_timer_start(1);
	 #endif

}

void byle_switch_io_low(void)
{
	log_info("byle_switch_io_low \n");

	switch_global.on_off=0;
      #ifdef BEEP_IO
      	deep_timer_start(1);
       #endif
       #ifdef BYLE_KWS_SWITCH_IO
       BYLE_KWS_SWITCH_OFF()  ;
       #endif
	   if(user_io_sleep_timer)
	   sys_hi_timer_del(user_io_sleep_timer);

}


void byle_switch_io_set(u8 onoff)
{
if(onoff)
	byle_switch_io_high();
else
	byle_switch_io_low();


}

u8 flash_cnt=0;
u16 cmd_flash_timer=0;

void cmd_ok_set_time_out(void)
{
if(switch_global.on_off)
{
BYLE_KWS_SWITCH_ON();
}
else

{
BYLE_KWS_SWITCH_OFF();
}


cmd_flash_timer=0;


}


void cmd_ok_set(void)
{
	if(cmd_flash_timer)
	sys_hi_timer_del(cmd_flash_timer);

cmd_flash_timer=sys_hi_timeout_add(NULL, cmd_ok_set_time_out, 500);
if(switch_global.on_off)
{
BYLE_KWS_SWITCH_OFF();
}
else
{
BYLE_KWS_SWITCH_ON();
}
#ifdef BEEP_IO
deep_timer_start(1);
 #endif


}





 void light_set_flash(u8 dir_on)
{

     if(switch_global.on_off)
     {
     if(dir_on)
              	{
              BYLE_KWS_SWITCH_OFF();
              	}
	 else
	 	{
		 BYLE_KWS_SWITCH_ON();
	 	}
     }
     else
     {

     if(dir_on)
     	{
              BYLE_KWS_SWITCH_ON();
     	}
    else
    	{
	BYLE_KWS_SWITCH_OFF();
    	}
     }
     #ifdef BEEP_IO
     deep_timer_start(1);
     #endif



}

void cmd_set_flash_time_out(void)
{




        if(flash_cnt>1)
        {
         light_set_flash((flash_cnt-1)%2);
        }
   else
	 {
	   if(cmd_flash_timer)
	  sys_hi_timer_del(cmd_flash_timer);
	   light_set_flash(0);
	   cmd_flash_timer=0;

        }
        if(flash_cnt)
        	{
        flash_cnt--;
        	}
		log_info("flash_cnt2222222222222222=%d \n",flash_cnt);

}


void cmd_light_set_flash(u8 cnt)
{



	flash_cnt=cnt;

	if(cmd_flash_timer)
	sys_hi_timer_del(cmd_flash_timer);
	cmd_flash_timer=sys_hi_timer_add(NULL, cmd_set_flash_time_out, 300);
	light_set_flash(1);
	log_info("cmd_light_set_flash=%d \n",cmd_flash_timer);

}


#if(Byle_Kws_Lib_Type==KWSA_switch_libS1||Byle_Kws_Lib_Type==KWSC_switch_libC1||Byle_Kws_Lib_Type==KWSA_switch_libS2||Byle_Kws_Lib_Type==KWSA_switch_libS3)      

int byle_switch_handle_action( int cmd_index)
{

	log_info("BYLE_BYLE_KWS_SWITCH_LIB =%d  %d\n",cmd_index,eCMD_changecolor);
	
   switch(cmd_index)
    {
    #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
    	case  eCMD_wakeup_uni:

			cmd_ok_set();
    		 break;
	#endif	
	
	case  eCMD_default_open:		 
		case eCMD_open0:
			log_info("eCMD_default_open \n");
			byle_switch_io_high();
			break;
		   case eCMD_close0:
		   case  eCMD_default_close:		
				log_info("eCMD_default_close \n");
				byle_switch_io_low();
			break;
		 case	eCMD_changecolor:
		 	
			log_info("eCMD_changecolor \n");
			 sys_hi_timeout_add(NULL, byle_switch_io_high, 300);
			byle_switch_io_low();
						break;

			 

    }
return 0;
}

#else
static u16 lamp_on_timer_out_id=0;
#define NAME_START_INDEX  eCMD_default_name

int byle_switch_handle_action( int cmd_index)
{
	log_info("BYLE_BYLE_KWS_SWITCH_LIB=%d \n",cmd_index);
switch(cmd_index)
    {
    	case  eCMD_wakeup_uni:

			cmd_ok_set();
    		 break;



    		case   eCMD_sleep10s:
				if(switch_global.on_off&&user_io_set_timer_out_flag)
				{

		byle_switch_sleep_off(10);


    			 cmd_ok_set();
				}
    		break;
    		case   eCMD_sleep30s:

				if(switch_global.on_off&&user_io_set_timer_out_flag)
					{

			byle_switch_sleep_off(30);


    			 cmd_ok_set();
					}
    		break;
    		case   eCMD_sleep1min:

				if(switch_global.on_off&&user_io_set_timer_out_flag)
					{

                        byle_switch_sleep_off(60);


    			 cmd_ok_set();
					}
    		break;
    		case   eCMD_sleep5min:

				if(switch_global.on_off&&user_io_set_timer_out_flag)
					{

			byle_switch_sleep_off(60*5);


    			 cmd_ok_set();
					}
    		break;
    		case   eCMD_sleep10min:
				if(switch_global.on_off&&user_io_set_timer_out_flag)
					{



			byle_switch_sleep_off(60*10);


    			 cmd_ok_set();
					}
    		break;
    		case   eCMD_cancelsleep:
				if(switch_global.on_off&&user_io_set_timer_out_flag)
					{
    			if(user_io_sleep_timer)
    			sys_hi_timer_del(user_io_sleep_timer);

    			cmd_ok_set();
					}
    			log_info("eCMD_cancelsleep  \n");
    		break;
    		case   eCMD_defaultulton:

				if(user_io_set_timer_out_flag)
					{
    			MyContrl.power_on_led=0;
    			my_user_save_info();
    			cmd_ok_set();
					}
    			log_info("eCMD_defaultulton  \n");
    		break;

    		case   eCMD_defaultultoff:

				if(user_io_set_timer_out_flag)
					{
    			MyContrl.power_on_led=0x55;
    			my_user_save_info();
    			cmd_ok_set();
					}
    			log_info("eCMD_defaultultoff  \n");
    		break;
				 case		   eCMD_setlight:

			user_io_set_timer_out_flag=30;
			cmd_ok_set();
			log_info("eCMD_setlight	\n");

			if(user_io_setlight_timer)
				sys_hi_timer_del(user_io_setlight_timer);
			user_io_setlight_timer=sys_hi_timeout_add(NULL, user_gpio_name_time_out, 1000*SET_TIME_OUT);

			break;

    		case  eCMD_default_name:
		log_info("eCMD_default_name \n");
               if(user_io_set_timer_out_flag)
               	{
               	MyContrl.power_on_led=0;
		MyContrl.user_name_index=0;
		my_user_save_info();
		cmd_ok_set();
               	}
		break;
	case  eCMD_default_open:
		if(MyContrl.user_name_index==0)
			{

			log_info("eCMD_default_open \n");
			byle_switch_io_high();
			}
		break;
       case  eCMD_default_close:
		   if(MyContrl.user_name_index==0)
       		{

       		log_info("eCMD_default_close \n");
       		byle_switch_io_low();
       		}
       	break;
     case   eCMD_defaultchangecolor:
	if(switch_global.on_off&&MyContrl.user_name_index==0)
	{
		 sys_hi_timeout_add(NULL, byle_switch_io_high, 300);
		byle_switch_io_low();
	}
		log_info("eCMD_changecolor \n");
					break;


    }
 if(cmd_index>=eCMD_mode1_name&&cmd_index<=eCMD_max_cmd)
		 {
			u8 temp=(cmd_index-eCMD_mode1_name);
			u8 temp1;
			log_info("temp%d,cmd_index%d	 user_name_inde=%d\n",temp,cmd_index,MyContrl.user_name_index);
			if(temp%SWITCH_GRID_NUM==0)
			 {
					if(user_io_set_timer_out_flag)
					 {

							   MyContrl.user_name_index=(cmd_index-eCMD_mode1_name)/SWITCH_GRID_NUM+1;

							   log_info("name 222222222222222222user_name_index %d \n",MyContrl.user_name_index);
							   my_user_save_info();
							   cmd_ok_set();
							   user_io_set_timer_out_flag=0;
					 }
			 }
		 else	if(temp%SWITCH_GRID_NUM==1)
			 {

						   temp1=(cmd_index-eCMD_mode1_name-1)/SWITCH_GRID_NUM+1;
						 log_info("(cmd_index-eCMD_mode1_name-1)/SWITCH_GRID_NUM+1=%d\n",temp1);
					 if( MyContrl.user_name_index==(cmd_index-eCMD_mode1_name-1)/SWITCH_GRID_NUM+1)
					 {

					 log_info("open light 5555555555555555 \n");
				   byle_switch_io_high();
					 }
			 }

		 else	if(temp%SWITCH_GRID_NUM==2)
			 {

			 temp1=(cmd_index-eCMD_mode1_name-2)/SWITCH_GRID_NUM+1;
			  log_info("(cmd_index-eCMD_mode1_name-2)/SWITCH_GRID_NUM+1=%d\n",temp1);

					 log_info("(cmd_index-eCMD_mode1_name-1)/SWITCH_GRID_NUM+1%d\n",(cmd_index-eCMD_mode1_name-1)/SWITCH_GRID_NUM+1);

					 if( MyContrl.user_name_index==(cmd_index-eCMD_mode1_name-2)/SWITCH_GRID_NUM+1)
					 {

					 log_info("close	light 777777777777777 \n");
				   byle_switch_io_low();
					 }
			 }

		 else	if(temp%SWITCH_GRID_NUM==3)
				 {

			 temp1=(cmd_index-eCMD_mode1_name-3)/SWITCH_GRID_NUM+1;
			  log_info("(cmd_index-eCMD_mode1_name-3)/SWITCH_GRID_NUM+1=%d\n",temp1);

						 if( MyContrl.user_name_index==(cmd_index-eCMD_mode1_name-3)/SWITCH_GRID_NUM+1)
						 {

						 log_info("changecolor 66666666666666666 \n");
						 if(switch_global.on_off)
							 {
									 sys_hi_timeout_add(NULL, byle_switch_io_high, 300);
									 byle_switch_io_low();
							 }
						 }
				 }


		 }






}
#endif

#endif



