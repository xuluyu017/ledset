#ifndef __MUSIC_LED_CODE_LESS_H__
#define __MUSIC_LED_CODE_LESS_H__

#ifdef MUSIC_LED_CODE_LESS





#ifndef  LED_WY_PWM_FRE
#define   LED_WY_PWM_FRE  20000//2400*5*5/4  // 800 k
#endif

#ifndef  LED_RGB_PWM_FRE
#define   LED_RGB_PWM_FRE  LED_WY_PWM_FRE//2400*5*5/4  // 800 k
#endif


#define   LED_BR_PWM_DUTY    LED_RGB_PWM_FRE*2/3  //

#define   LED_W_PWM_FRE    LED_WY_PWM_FRE  //
#ifndef LED_Y_PWM_FRE
#define   LED_Y_PWM_FRE    LED_WY_PWM_FRE  //
#endif
#define   LED_R_PWM_FRE    LED_RGB_PWM_FRE  //
#define   LED_G_PWM_FRE    LED_RGB_PWM_FRE  //
#define   LED_B_PWM_FRE    LED_RGB_PWM_FRE  //

#ifdef LED_R_PORT
#ifndef LEDR_PWM_TIMER 
#define  LEDR_PWM_TIMER   JL_TIMER0
#endif
//#define  LEDR_PWM_OUTPUT_CHANNEL   USE_OUTPUT_CHANNEL_1   		//BZ

#endif

#ifdef LED_G_PORT
#ifndef LEDG_PWM_TIMER 
#define  LEDG_PWM_TIMER   JL_TIMER1  
#endif
//#define  LEDG_PWM_OUTPUT_CHANNEL   USE_OUTPUT_CHANNEL_2   		//BZ

#endif

#ifdef LED_B_PORT
#ifndef LEDB_PWM_TIMER
#define  LEDB_PWM_TIMER   JL_TIMER2
#endif
//#define  LEDB_PWM_OUTPUT_CHANNEL   0  		//BZ

#endif

// #ifdef LED_W_PORT           //BZ   128 142A定时器不够 故注释掉
// #ifndef LEDW_PWM_TIMER
// #define  LEDW_PWM_TIMER   JL_TIMER0
// #endif
// #define  LEDW_PWM_OUTPUT_CHANNEL   USE_OUTPUT_CHANNEL_1  

// #endif

// #ifdef LED_Y_PORT            //BZ   142A定时器不够 故注释掉
// #ifndef LEDY_PWM_TIMER
// #define  LEDY_PWM_TIMER   JL_TIMER1
// #endif
// #define  LEDY_PWM_OUTPUT_CHANNEL   USE_OUTPUT_CHANNEL_2  

// #endif




	 typedef enum _LED_PLAY_MODE_
	 {
		 LED_PLAY_MODE_ONE, 	// 单曲循环播放
		 LED_PLAY_MODE_ALL, 	// 顺序循环播放
		 LED_PLAY_MODE_RANDOM,		   // 随机播放
		 LED_PLAY_MODE_SUM,
	 } LED_PLAYER_MODE;



	 typedef enum _LED_STATE
	 {
		 LED_OFF=0,	  // 播放
		 LED_NOR ,	// 空闲
		 LED_COLOR_CHAGE,	  //  彩虹呼吸
		 LED_COLOR_RED_CHAGE,//漫彩虹
		 LED_HEARTBEAT, 		  //心跳
		 LED_RED_SLOW_FADE, // 漫红呼吸
		 LED_GREEN_SLOW_FADE, //漫绿呼吸
		 LED_BLUE_SLOW_FADE,//漫蓝呼吸
		 LED_ALARM, 				// 报警灯
		 LED_FLASH_STACK,	 // 闪烁
		 LED_COLOR_CHAGE_FADE,// 漫彩呼吸
		 LED_GREEN_MOOD,	  // 绿色心情
		 LED_SUNSET,// 夕阳美景
		 LED_SENSOR_MODE,//闊充箰寰嬪姩
		 
		 LED_ColorfulMode,	  //  彩虹呼吸
		 
		 LED_ColorfulflashMode,	  //  闪烁变色
	      LED_RGB_MODE,

		 LED_MAX,		   // 快退中
	 } LED_STATE;


  #define  LED_AUTO_DISP_MAX_MODE  LED_SUNSET
  #define  LED_AUTO_DISP_MIN_MODE  LED_COLOR_CHAGE 
  #define  LED_AUTO_DISP_MODE_MAX_NUNBER  (LED_SUNSET-LED_COLOR_CHAGE+1)
    #define  LED_APP_DISP_MODE_MAX_NUNBER  (LED_RGB_MODE-LED_COLOR_CHAGE)




	 typedef struct _LED_DISP_INFO_
	 {

	 bool	LED_on_off;
	 u8 LED_disp_mode; // 文件扇区号
      #ifndef DISABLE_RGB_LED
	 u8 LED_disp_mode_back; // 文件扇区号
	 #endif
	 u16   LED_flash_time;
	 u8  LED_bright;
	 u8  LED_userbright;
	 u8  LED_R_buf;
	 u8  LED_G_buf;
	 u8  LED_B_buf;
	 u8  LED_W_buf;
	 u8  LED_Y_buf;
	 u8  LED_power_flag;
	 u8  LED_Music_sensitivity;
             u8	is_dc_power_on_led_off;
	 u8  data_check;
	 } LED_DISP_INFO;

	 extern u8		   LED_WYRGB_fade_flag;
	 extern u8		   LED_W_fade_set;
	 extern u8		   LED_Y_fade_set;
	 extern u8		   LED_R_fade_set;
	 extern u8		   LED_G_fade_set;
	 extern u8		   LED_B_fade_set;
	  extern u8               LED_PowerCnt;
	 extern u8		    led_bright_dir_temp;
	 extern u8		   LED_Bright_fade_set;
	 extern u8		   LED_Bright_flag;
	 extern u8		   LED_Y_fade_set;
	 extern u8		   LED_WYRGB_fade_flag;
	 extern   u16 ledfadeTimer;
	 extern u32  flashTimer;
	 extern LED_DISP_INFO  gLedContrl ;// __attribute__((section("data"))) = {0};
typedef struct 							//BZ	防止编译出错
	  {
		u32 max_sample_value;
		u16 mic_off_sample_value;
		u8	LED_Music_sensitivity;
	} led_rgb_parameter;				//BZ


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


#ifndef DEFAUFT_FLASH_SPEED
#define   DEFAUFT_FLASH_SPEED    20  //
#endif
void pwm_timer_close(void);
void pwm_timer_open(void);

void led_set_flash(u8 cnt);
void led_set_fade(u8 cnt,u8 bright);
bool  is_music_mode(void);
	 void get_mic_energy_int(void);
	 bool Audio_spec_color_display(void);
	 bool  Audio_spec_bright_display(void);
	 u32 byle_get_lib_mic_energy(void);
	 void led_pwm_on_off(u8  onoff_flag);

	 int kws_led_handle_action( u16 cmd_index);
	 void led_pwm_updata(void);
	 void led_display_int(void);
	 void  led_mode_display_int(void);
	 void led_pwm_wy_close(void);
	 void led_mode_fade_set(u8 LedR,u8 LEDG,u8 LEDB,u8 time);
	 void led_on_off(bool onoff_flag);
	 bool  is_led_on_off(void);
	 void led_bright_dir_set(u8 bright);
	 void Audio_spec_display(void);//频谱灯
	 void MusicLedPortInt(void);
	 void led_port_det_int(void);
	 void clear_auto_play_mode(void);
	 void led_color_set(u8 LED_r,u8 LED_g,u8 LED_b );
	 void led_color_fade_set(u8 LED_r,u8 LED_g,u8 LED_b);
	 void LED_Power_Handle(void);
	 void MusicLedCtlInt(void);
	 void led_status_pross(void);
	 void MusicLEDMsgProccess(int Msg);
	 void led_timer_1ms(void);
	 
	 void led_timer_2ms(void);
	 void led_power_on(void);
	 void power_on_switch_color(void);
	 extern LED_DISP_INFO  gLedContrl;
void updata_led_to_app(void);
	 u8 get_nearest_userbright(u8 parameter);
	 extern u8  MyLed_save_Cnt;
#endif
#endif



