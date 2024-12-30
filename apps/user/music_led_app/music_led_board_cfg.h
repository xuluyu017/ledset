/*
原理图版本 U320A8     V1 :    R=A14 ;             B=PA2 ;                   G=PA5;             (LEDB)  带三级管      高亮   MIC_BIAS=PA8   MIC=PA7
                                                V2 :    R=A14 ;             B=PA2 ;                   G=PA5;             (LEDA)  带三级管      高亮    MIC_BIAS=PA8    MIC=PA7
                                                V3 :    R=A14 ;             B=PA2,PA3 ;            G=PA5,PA0;      (LEDB)  不带三级管低亮   MIC_BIAS=PA8    MIC=PA7
                                                V4 :    R=A14 ;             B= PA5,PA0;            G=PA2,PA3 ;     (LEDA)  不带三级管低亮    MIC_BIAS=PA8   MIC=PA7
                                                V5 :    W=PA2 ；         Y=PA5 ；                                                   带三级管     高亮   MIC_BIAS=PA8    MIC=PA7
                                                V6 :    W=A14 PA0 ；                                                                           带三级管       高亮    MIC_BIAS=PA8    MIC=PA7  
                                                V7 :	  Y=PA5 ,PA0；W=PA2,PA3 ；					       不带三级管  低亮     MIC_BIAS=PA8    MIC=PA7 


*/







#ifdef US320A8_WY_V5
//#define POWER_ON_MAX_LIGHT  80	
// #define MAX_WY_PWM_PERCENT 65 //80         
#define LED_W_PORT IO_PORTA_02
#define LED_Y_PORT  IO_PORTA_00
#define PWM_HIGH_ON 1
#define DC_POWER_ON_TO_WY          
#endif


#ifdef US320A8_RGB_V4
//#define POWER_ON_MAX_LIGHT  80
#define LED_R_PORT  IO_PORTC_03
#define LED_G_PORT  IO_PORTB_01
#define LED_B_PORT  IO_PORTB_02

#define LED_R_IO_DRIVER  1
#define LED_G_IO_DRIVER  1
#define LED_B_IO_DRIVER  1

#define LED_GG_PORT  IO_PORTB_00
#define LED_BB_PORT  IO_PORTB_03
#define LED_GG_IO_DRIVER  1
#define LED_BB_IO_DRIVER  1

#define PWM_HIGH_ON 0

#define LED_R_FOR_TIMER_PWM
#define LED_G_FOR_TIMER_PWM
#define LED_B_FOR_TIMER_PWM

#undef TCFG_VDDIOM_LEVEL
#define TCFG_VDDIOM_LEVEL			        VDDIOM_VOL_33V

#endif

#ifdef US320A8_RGB_V2
//#define POWER_ON_MAX_LIGHT  80
#define LED_R_PORT  IO_PORTC_03
#define LED_G_PORT  IO_PORTB_02
#define LED_B_PORT  IO_PORTB_01
#define LED_R_FOR_TIMER_PWM
#define LED_G_FOR_TIMER_PWM
#define LED_B_FOR_TIMER_PWM
#define PWM_HIGH_ON 1
#endif

#ifdef US320A8_RGB_V1
//#define POWER_ON_MAX_LIGHT  80
#define LED_R_PORT  IO_PORTC_03
#define LED_G_PORT  IO_PORTB_01
#define LED_B_PORT  IO_PORTB_02

#define LED_R_FOR_TIMER_PWM
#define LED_G_FOR_TIMER_PWM
#define LED_B_FOR_TIMER_PWM

#define PWM_HIGH_ON 1
#endif

#ifdef US320A8_RGB_V3
//#define POWER_ON_MAX_LIGHT  80
	
#define LED_R_PORT  IO_PORTA_02//IO_PORTC_03      
#define LED_G_PORT  IO_PORTA_01//IO_PORTB_02      
#define LED_B_PORT  IO_PORTA_00//IO_PORTA_11//IO_PORTB_01     

#define LED_R_IO_DRIVER  1
#define LED_G_IO_DRIVER  1
#define LED_B_IO_DRIVER  1

#define LED_RR_PORT  IO_PORTA_03         
#define LED_GG_PORT  IO_PORTA_04//IO_PORTB_03     
#define LED_BB_PORT  IO_PORTA_05//IO_PORTB_00     

#define LED_RR_IO_DRIVER  1              
#define LED_GG_IO_DRIVER  1
#define LED_BB_IO_DRIVER  1

#define PWM_HIGH_ON 0

#define LED_R_FOR_TIMER_PWM
#define LED_G_FOR_TIMER_PWM
#define LED_B_FOR_TIMER_PWM

// #define MAX_R_PWM_PERCENT 100         
// #define MAX_G_PWM_PERCENT 50//60         
// #define MAX_B_PWM_PERCENT 50//60         
#define LED_G_PORT_SET_HD0_VALUE 1             //BZ
#define LED_G_PORT_SET_HD1_VALUE 0       

#define LED_B_PORT_SET_HD0_VALUE 1             //BZ
#define LED_B_PORT_SET_HD1_VALUE 0       

#define LED_R_PORT_SET_HD0_VALUE 1             //BZ
#define LED_R_PORT_SET_HD1_VALUE 0//1       

#define LED_RR_PORT_SET_HD0_VALUE 1             //BZ
#define LED_RR_PORT_SET_HD1_VALUE 0       

#define LED_BB_PORT_SET_HD0_VALUE 0//0             //BZ
#define LED_BB_PORT_SET_HD1_VALUE 0       

#define LED_GG_PORT_SET_HD0_VALUE 0//0             //BZ
#define LED_GG_PORT_SET_HD1_VALUE 0      

#define MAX_MIN_LIGHT_FLASH_LED    
#undef TCFG_VDDIOM_LEVEL
#define TCFG_VDDIOM_LEVEL			        VDDIOM_VOL_36V

#endif


#ifdef US320A8_WY_V7
#define LED_W_PORT_SET_HD0_VALUE 0        
#define LED_W_PORT_SET_HD1_VALUE 1
#define LED_Y_PORT_SET_HD0_VALUE 0 
#define LED_Y_PORT_SET_HD1_VALUE 1
#define LED_W1_PORT_SET_HD0_VALUE 0       
#define LED_W1_PORT_SET_HD1_VALUE 1
#define LED_Y1_PORT_SET_HD0_VALUE 0 
#define LED_Y1_PORT_SET_HD1_VALUE 1     

#define LED_W_PORT IO_PORTA_02
#define LED_W_PORT1 IO_PORTA_03
#define LED_Y_PORT  IO_PORTA_00
#define LED_Y_PORT1  IO_PORTA_05
#define LED_W_IO_DRIVER  1
#define LED_Y_IO_DRIVER  1
#define LED_W1_IO_DRIVER  1
#define LED_Y1_IO_DRIVER  1
#define PWM_HIGH_ON 0
#define LED_W_FOR_TIMER_PWM
#define LED_Y_FOR_TIMER_PWM
#define MAX_MIN_LIGHT_FLASH_LED    
#define DC_POWER_ON_TO_WY           
#undef TCFG_VDDIOM_LEVEL            
#define TCFG_VDDIOM_LEVEL			        VDDIOM_VOL_36V      
#endif

#ifndef PWM_HIGH_ON
#define PWM_HIGH_ON 1
#endif



#if  PWM_HIGH_ON
#define PWM_ON   1
#define PWM_OFF 0
#else
#define PWM_ON   0
#define PWM_OFF 1

#endif

#define USB_DM_IN()	 { gpio_set_direction(IO_PORT_DM,1);gpio_set_pull_up(IO_PORT_DM,0);gpio_set_pull_down(IO_PORT_DM,0);}
#define USB_DP_IN() 	 {  gpio_set_direction(IO_PORT_DP,1);gpio_set_pull_up(IO_PORT_DP,0);gpio_set_pull_down(IO_PORT_DP,0);}



#if defined( LED_B_FOR_TIMER_PWM) ||defined( LED_G_FOR_TIMER_PWM)||defined( LED_R_FOR_TIMER_PWM)||defined( LED_W_FOR_TIMER_PWM) ||defined( LED_Y_FOR_TIMER_PWM)     
#define USE_TIMER_FOR_PWM 
#define  PWM_TIMER   JL_TIMER1
#define  PWM_IRQ_TIME_IDX                    IRQ_TIME1_IDX

        
        #ifdef LED_B_FOR_TIMER_PWM
        #define LED_B_ON        gpio_direction_output(LED_B_PORT,PWM_ON);
        #define LED_B_OFF        gpio_direction_output(LED_B_PORT,PWM_OFF);
        #endif
		
          #ifdef LED_R_FOR_TIMER_PWM      
        #define LED_R_ON        gpio_direction_output(LED_R_PORT,PWM_ON);
        #define LED_R_OFF        gpio_direction_output(LED_R_PORT,PWM_OFF);
        #endif

       #ifdef LED_G_FOR_TIMER_PWM
        #define LED_G_ON        gpio_direction_output(LED_G_PORT,PWM_ON);
        #define LED_G_OFF        gpio_direction_output(LED_G_PORT,PWM_OFF);
       #endif

	   
        #ifdef LED_BB_PORT
        #define LED_BB_ON        gpio_direction_output(LED_BB_PORT,PWM_ON);
        #define LED_BB_OFF        gpio_direction_output(LED_BB_PORT,PWM_OFF);
        #endif
        
        #ifdef LED_RR_PORT
        #define LED_RR_ON        gpio_direction_output(LED_RR_PORT,PWM_ON);
        #define LED_RR_OFF        gpio_direction_output(LED_RR_PORT,PWM_OFF);
        #endif
        
        #ifdef LED_GG_PORT
        #define LED_GG_ON        gpio_direction_output(LED_GG_PORT,PWM_ON);
        #define LED_GG_OFF        gpio_direction_output(LED_GG_PORT,PWM_OFF);
        #endif


      //    #ifdef LED_R_PORT           
      //  #define LED_R_ON		gpio_direction_output(LED_R_PORT,PWM_ON);
      //  #define LED_R_OFF		 gpio_direction_output(LED_R_PORT,PWM_OFF);
      //  #endif



          #ifdef LED_W_FOR_TIMER_PWM
        #define LED_W_ON		 gpio_direction_output(LED_W_PORT,PWM_ON);
        #define LED_W_OFF		  gpio_direction_output(LED_W_PORT,PWM_OFF);

           #ifdef LED_W_PORT1
         #define LED_W1_ON		 gpio_direction_output(LED_W_PORT1,PWM_ON);
         #define LED_W1_OFF		  gpio_direction_output(LED_W_PORT1,PWM_OFF);
          #endif
		 
        #endif
       
       #ifdef LED_Y_FOR_TIMER_PWM
        #define LED_Y_ON		 gpio_direction_output(LED_Y_PORT,PWM_ON);
        #define LED_Y_OFF		  gpio_direction_output(LED_Y_PORT,PWM_OFF);

	   #ifdef LED_Y_PORT1
         #define LED_Y1_ON		 gpio_direction_output(LED_Y_PORT1,PWM_ON);
         #define LED_Y1_OFF		  gpio_direction_output(LED_Y_PORT1,PWM_OFF);
          #endif
       #endif



#endif




#ifdef  MUSIC_LED_CODE_LESS


#define MAX_BRIGHT_VALUE  15	
#define MIN_BRIGHT  1//255
//#define MAX_WY_PWM_PERCENT  35//255
#define COLOR_TEMP_KEY_DISABLE_NIGHT_LIGHT
#define CHANGE_COLOR_REPEAT_CMD
//#define ADJ_BRIGHT_REPEAT_CMD                                         
#define LITTLE_LIGHT_BRIGHT	30
#define DISABLE_LED_ADJ_FADE

#ifdef BYLE_KWS_ASR
#define USB_RGB_LIGHT
#define BYLE_kws_music_led
#endif

#ifndef LED_R_PORT
#ifndef LED_G_PORT
#ifndef LED_B_PORT
#define  DISABLE_RGB_LED
#endif
#endif
#endif
#ifdef LED_Y_PORT
#define  ENABLE_YELLOW 1
#else
#define  ENABLE_YELLOW 0
#endif


#ifndef DISABLE_RGB_LED
#define  ENABLE_GET_MIC_ENERGY
#define MUSIC_MODE_NEED_REPEAT_CMD
#endif


#endif


#ifdef LED_GG_IO_DRIVER
#ifndef LED_GG_PORT_SET_HD0_VALUE      
#define LED_GG_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_GG_PORT_SET_HD1_VALUE      
#define LED_GG_PORT_SET_HD1_VALUE 1
#endif
#define LED_GG_IO_DRIVER_INT()      { gpio_set_hd0(LED_GG_PORT,LED_GG_PORT_SET_HD0_VALUE);gpio_set_hd(LED_GG_PORT, LED_GG_PORT_SET_HD1_VALUE); gpio_direction_output(LED_GG_PORT,PWM_OFF); }
// #define LED_GG_IO_DRIVER_INT()      { gpio_set_die(LED_GG_PORT,0);gpio_set_direction(LED_GG_PORT,0); gpio_direction_output(LED_GG_PORT,PWM_OFF); }

#endif                                          

#ifdef LED_BB_IO_DRIVER
#ifndef LED_BB_PORT_SET_HD0_VALUE      
#define LED_BB_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_BB_PORT_SET_HD1_VALUE      
#define LED_BB_PORT_SET_HD1_VALUE 1
#endif
#define LED_BB_IO_DRIVER_INT()      { gpio_set_hd0(LED_BB_PORT,LED_BB_PORT_SET_HD0_VALUE);gpio_set_hd(LED_BB_PORT, LED_BB_PORT_SET_HD1_VALUE); gpio_direction_output(LED_BB_PORT,PWM_OFF); }
// #define LED_BB_IO_DRIVER_INT()      { gpio_set_die(LED_BB_PORT,0);gpio_set_direction(LED_BB_PORT,0); gpio_direction_output(LED_BB_PORT,PWM_OFF); }

#endif                                          
#ifdef LED_RR_IO_DRIVER
#ifndef LED_RR_PORT_SET_HD0_VALUE      
#define LED_RR_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_RR_PORT_SET_HD1_VALUE      
#define LED_RR_PORT_SET_HD1_VALUE 1
#endif
#define LED_RR_IO_DRIVER_INT()      { gpio_set_hd0(LED_RR_PORT,LED_RR_PORT_SET_HD0_VALUE);gpio_set_hd(LED_RR_PORT, LED_RR_PORT_SET_HD1_VALUE); gpio_direction_output(LED_RR_PORT,PWM_OFF); }
#endif                                          



#ifdef LED_R_IO_DRIVER
#ifndef LED_R_PORT_SET_HD0_VALUE      
#define LED_R_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_R_PORT_SET_HD1_VALUE      
#define LED_R_PORT_SET_HD1_VALUE 1
#endif
#define LED_R_IO_DRIVER_INT()      { gpio_set_hd0(LED_R_PORT,LED_R_PORT_SET_HD0_VALUE);gpio_set_hd(LED_R_PORT, LED_R_PORT_SET_HD1_VALUE); gpio_direction_output(LED_R_PORT,PWM_OFF); }
#endif                                          


#ifdef LED_B_IO_DRIVER
#ifndef LED_B_PORT_SET_HD0_VALUE      
#define LED_B_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_B_PORT_SET_HD1_VALUE      
#define LED_B_PORT_SET_HD1_VALUE 1
#endif
#define LED_B_IO_DRIVER_INT()      { gpio_set_hd0(LED_B_PORT,LED_B_PORT_SET_HD0_VALUE);gpio_set_hd(LED_B_PORT, LED_B_PORT_SET_HD1_VALUE); gpio_direction_output(LED_B_PORT,PWM_OFF); }
#endif                                          


#ifdef LED_G_IO_DRIVER
#ifndef LED_G_PORT_SET_HD0_VALUE      
#define LED_G_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_G_PORT_SET_HD1_VALUE      
#define LED_G_PORT_SET_HD1_VALUE 1
#endif
#define LED_G_IO_DRIVER_INT()      { gpio_set_hd0(LED_G_PORT,LED_G_PORT_SET_HD0_VALUE);gpio_set_hd(LED_G_PORT, LED_G_PORT_SET_HD1_VALUE); gpio_direction_output(LED_G_PORT,PWM_OFF); }
#endif                                          


#ifdef LED_W_IO_DRIVER
#ifndef LED_W_PORT_SET_HD0_VALUE      
#define LED_W_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_W_PORT_SET_HD1_VALUE      
#define LED_W_PORT_SET_HD1_VALUE 1
#endif
#define LED_W_IO_DRIVER_INT()      { gpio_set_hd0(LED_W_PORT, LED_W_PORT_SET_HD0_VALUE);gpio_set_hd(LED_W_PORT, LED_W_PORT_SET_HD1_VALUE); gpio_direction_output(LED_W_PORT,PWM_OFF); }
#endif

#ifdef LED_W1_IO_DRIVER
#ifndef LED_W1_PORT_SET_HD0_VALUE      
#define LED_W1_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_W1_PORT_SET_HD1_VALUE      
#define LED_W1_PORT_SET_HD1_VALUE 1
#endif
#define LED_W1_IO_DRIVER_INT()      { gpio_set_hd0(LED_W_PORT1, LED_W1_PORT_SET_HD0_VALUE);gpio_set_hd(LED_W_PORT1, LED_W1_PORT_SET_HD1_VALUE); gpio_direction_output(LED_W_PORT1,PWM_OFF); }
#endif




#ifdef LED_Y_IO_DRIVER
#ifndef LED_Y_PORT_SET_HD0_VALUE      
#define LED_Y_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_Y_PORT_SET_HD1_VALUE      
#define LED_Y_PORT_SET_HD1_VALUE 1
#endif
#define LED_Y_IO_DRIVER_INT()      { gpio_set_hd0(LED_Y_PORT, LED_Y_PORT_SET_HD0_VALUE);gpio_set_hd(LED_Y_PORT, LED_Y_PORT_SET_HD1_VALUE); gpio_direction_output(LED_Y_PORT,PWM_OFF); }
#endif

#ifdef LED_Y1_IO_DRIVER
#ifndef LED_Y1_PORT_SET_HD0_VALUE      
#define LED_Y1_PORT_SET_HD0_VALUE 1
#endif
#ifndef LED_Y1_PORT_SET_HD1_VALUE      
#define LED_Y1_PORT_SET_HD1_VALUE 1
#endif
#define LED_Y1_IO_DRIVER_INT()      { gpio_set_hd0(LED_Y_PORT1, LED_Y1_PORT_SET_HD0_VALUE);gpio_set_hd(LED_Y_PORT1, LED_Y1_PORT_SET_HD1_VALUE); gpio_direction_output(LED_Y_PORT1,PWM_OFF); }
#endif



