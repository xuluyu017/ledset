#include "includes.h"
#include "cpu.h"
#include "gpio.h"
#include "app_config.h"
#include "timer.h"
#include "vm.h"

#include "byle_cfg.h"
#include "byle_user.h"

#include "user_pwm.h"
#include "byle_kws_app.h"
#include "byle_msg.h"
#include "timer_app.h"
#include "ir_emitter.h"

#ifdef MUSIC_LED_CODE_LESS
#include "music_led_code_less.h"
#endif

#ifdef IR_EMITTER_PORT
#if(IR_LIB_TYPE==lib_irB)
#include "lib_irB.h"
#endif

#if(IR_LIB_TYPE==lib_irA)
#include "lib_irA.h"
#endif

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[ir]"
#include "log.h"


#ifndef IR_EMITTER_PWM_TIMER
#define  IR_EMITTER_PWM_TIMER   JL_TIMER0
#endif

#ifndef IR_EMITTER_PWM_OUTPUT_CHANNEL
#define  IR_EMITTER_PWM_OUTPUT_CHANNEL   0 
#endif

#ifndef IR_EMITTER_PWM_FRE
#define   IR_EMITTER_PWM_FRE    38000  //
#endif


#ifndef IR_EMITTER_IRQ_IDX
#define IR_EMITTER_TIMER  JL_TIMER0
#define IR_EMITTER_IRQ_IDX  IRQ_TIME0_IDX
#endif



#if(IR_EMITTER_IRQ_IDX==IRQ_TIME1_IDX)
#define IR_EMITTER_TIMER                       JL_TIMER1
#define IR_EMITTER_TIME_REG                    JL_TIMER1

#elif(IR_EMITTER_IRQ_IDX==IRQ_TIME0_IDX)
#define IR_EMITTER_TIMER                       JL_TIMER0
#define IR_EMITTER_TIME_REG                    JL_TIMER0

#elif(IR_EMITTER_IRQ_IDX==IRQ_TIME2_IDX)
#define IR_EMITTER_TIMER                       JL_TIMER2
#define IR_EMITTER_TIME_REG                    JL_TIMER2

#endif

s_ir_send_arrays *g_ir_arrays ;

static AcInputParamType airInfo = {0};
#ifdef WAIT_IR_SEND_TO_TONE
OS_SEM  ir_send_sem;
#endif
IR_LEARN_INFO  ir_learn_info;
OS_SEM  ir_task_sem;
u16 ir_status=IR_IDLE;
static u16	ir_emitter_check_timer = 0;
static u8  send_cnt=0;
static u8 IrLib_ok=0;

#define MAX_search_BRAND_NUM 124

const uint16_t search_tab[MAX_search_BRAND_NUM][5] = {
{79,	62,	36,	22,	177            },
{768,	50,	33,	23,	252            },
{78,	65,	37,	24,	253            },
{77,	64,	39,	25,	254            },
{76,	63,	260,	26,	250    },
{73,	54,	325,	27,	249        },
{74,	61,	40,	404,	251        },
{75,	63,	31,	19,	112            },
{635,	97,	32,	20,	188            },
{636,	246,	30,	350,	247        },
{326,	321,	258,	351,	344    },
{340,	49,	327,	16,	345        },
{341,	56,	29,	18,	346            },
{342,	60,	38,	15,	284            },
{343,	51,	259,	349,	44     },
{28,	52,	347,	348,	45     },
{89,	0,	6,	163,	410        },
{90,	1,	7,	164,	411        },
{91,	2,	8,	165,	119        },
{92,	3,	13,	166,	121        },
{93,	4,	14,	167,	122        },
{94,	5,	134,	168,	123    },
{95,	628,	135,	169,	124},
{86,	629,	9,	170,	125    },
{87,	131,	10,	171,	126    },
{642,	114,	11,	161,	128    },
{136,	133,	12,	162,	117    },
{137,	111,	329,	684,	113},
{195,	153,	218,	225,	67 },
{196,	154,	219,	226,	68 },
{197,	155,	220,	227,	69 },
{198,	156,	221,	228,	70 },
{199,	332,	222,	229,	72 },
{200,	333,	223,	319,	66 },
{201,	377,	224,	318,	353},
{202,	378,	268,	317,	354},
{203,	379,	269,	358,	355},
{204,	380,	270,	359,	356},
{205,	381,	271,	360,	103},
{206,	382,	362,	361,	157},
{208,	383,	363,	151,	158},
{209,	148,	364,	152,	375},
{210,	331,	147,	149,	160},
{211,	372,	150,	193,	384},
{212,	373,	330,	386,	649},
{194,	374,	385,	159,	88 },
{106,	235,	287,	295,	179},
{334,	236,	286,	296,	180},
{357,	237,	394,	297,	181},
{107,	238,	699,	395,	182},
{693,	239,	700,	245,	178},
{694,	240,	701,	244,	175},
{695,	241,	698,	173,	176},
{696,	242,	172,	174,	183},
{697,	243,	71,	190,	338    },
{214,	216,	335,	709,	717},
{400,	100,	300,	401,	301},
{101,	702,	402,	102,	302},
{303,	403,	703,	304,	704},
{104,	705,	305,	105,	405},
{406,	306,	706,	207,	707},
{407,	307,	308,	708,	408},
{108,	409,	109,	309,	110},
{710,	310,	711,	311,	312},
{712,	613,	213,	313,	713},
{714,	314,	614,	615,	115},
{715,	215,	315,	616,	116},
{316,	716,	217,	617,	17 },
{618,	118,	718,	619,	719},
{320,	120,	720,	620,	621},
{321,	21,	721,	622,	322    },
{623,	323,	624,	324,	625},
{626,	627,	127,	129,	130},
{230,	630,	231,	631,	132},
{232,	632,	233,	633,	634},
{234,	34,	35,	336,	337        },
{638,	138,	339,	639,	139},
{140,	640,	141,	41,	641    },
{42,	142,	143,	43,	643    },
{144,	644,	645,	145,	646},
{146,	46,	246,	647,	48     },
{248,	648,	650,	651,	352},
{652,	653,	654,	655,	255},
{256,	656,	657,	257,	58 },
{658,	659,	660,	261,	61 },
{661,	62,	662,	262,	263    },
{663,	64,	664,	264,	365    },
{665,	265,	266,	366,	666},
{367,	267,	667,	368,	668},
{369,	669,	670,	370,	371},
{671,	672,	272,	273,	673},
{674,	274,	275,	675,	276},
{676,	376,	277,	677,	678},
{278,	279,	679,	280,	680},
{80,	281,	681,	81,	82     },
{682,	282,	283,	683,	83 },
{84,	184,	85,	685,	285    },
{185,	686,	186,	387,	187},
{687,	288,	388,	688,	389},
{189,	289,	689,	690,	390},
{290,	191,	291,	391,	691},
{192,	292,	692,	392,	293},
{393,	294,	96,	396,	97     },
{397,	298,	398,	299,	399},
{99,	541,	555,	632,	645},
{722,	735,	724,	725,	731},
{727,	728,	729,	730,	726},
{732,	733,	734,	723,	737},
{736,	738,	739,	740,	796},
{741,	742,	747,	751,	797},
{743,	750,	748,	753,	749},
{744,	754,	756,	757,	758},
{745,	755,	759,	761,	760},
{746,	762,	763,	765,	767},
{768,	769,	770,	771,	764},
{772,	774,	777,	780,	781},
{773,	775,	778,	782,	783},
{776,	779,	784,	786,	787},
{785,	788,	790,	791,	794},
{789,	792,	795,	798,	752},
{800,	811,	802,	803,	806},
{804,	805,	807,	808,	809},
{810,	801,	812,	813,	814},
{815,	801,	812,	808,	809},
};

#define MAX_BRAND_NUM 37+4+3				//BZ
static const uint16_t PPCODE_TAB[MAX_BRAND_NUM+1][5] = {
{79,	62, 36, 22, 177}, //格力,美的,海尔,奥克斯，小米
{79,77,326,340,342},    //格力
{49,284,44,56},    		//美的
{49,310,178},    				//小天鹅
{113,134,111,130},    	//海信
{113,134,111,130},    	//科龙，
{3,315,628},    				//TCL
{40,30,37,29,38},    //海尔
{72,99},   		//月兔
{236,241,717},    //日立
{23,22,15,27},    		//奥克斯，
{6,10,154},    //志高
{195,208,193,88},    //松下
{229,691,227,226},    //大金
{177,302,647},    //小米
{169},    //三星
{702,249,251,112,247},    //长虹，
{154,103,331,151,649},	  //三菱重工
{96,60,97, 98 ,321},//华凌空调
{349,650,101,184,13},//康佳空调
{154,103,331,151,649},	  //三菱电机
{69,353,354},			//格兰仕空调
{309,292},				//扬子空调
{338,729},				//创维空调
{97,284},				//东芝空调
{322,188,175,232,387},	//三洋空调
{90,95,86,88},			//LG 空调
{296,297,735,736},		//约克空调
{177,255,175,256},		//樱花空调
{354,621},				//内田空调
{81,84,85}, 			//新科空调
{114},					//华宝空调
{657,239,266,690},		//长岭空调
{267},					//乐华空调
{68,667,665,759},		//麦克维尔空调
{801},					//飞利浦空调
 //新增4个
{188,189,190,191,192},					//新飞空调
{287,286,394	},					//夏普
{139,140,141,25,142},	  //惠而浦
{106,334,357,107,693},	  //春兰


// {25,147,148,149,150},	  //三菱			//BZ		US320B4_irA_kwsA_K1lib_ch_RGB_V3
{87,89,91,92,95},	  //乐金空调 是 LG空调	????	改为LG 空调		//BZ		无
{24,316,214,215,216},	  //澳柯玛			//BZ
{218,219,220,221,222},	  //富士通			//BZ
};
#if  !defined(MUSIC_LED_CODE_LESS)&&!defined(USE_SPI_DRIVE_RGBLED)
void led_pwm_on_off(u8 onoff_flag)
{

}
void led_set_flash(u8 cnt)
{

}
void led_set_fade(u8 cnt,u8 bright)
{

}
bool  is_led_on_off(void)
{
  return 0;
}
#endif

#define     VM_LEARN_IR_INFO			VM_IR_EMITTER_INFO+1
#define     VM_FAN_IR_INFO			VM_IR_EMITTER_INFO+2



void ir_learn_save_info(void)
{
	printf("ir_learn_read_info=%d\n",ir_learn_info.ac_num);
	airInfo.pp_num=ir_learn_info.ac_brand;
	local_irq_disable();
	syscfg_write(VM_LEARN_IR_INFO, &ir_learn_info,sizeof(IR_LEARN_INFO));
	local_irq_enable();
}

void ir_learn_read_info(void)
{
    int ret = 0;
    ret=syscfg_read(VM_LEARN_IR_INFO, &ir_learn_info,sizeof(IR_LEARN_INFO));	
      if (ret !=sizeof(IR_LEARN_INFO))
      {
          printf("ir_learn_info is null\n");
		  ir_learn_info.ac_num=0;
		  ir_learn_info.ac_brand=0;
		  ir_learn_info.ac_search=0xff;
    }
	printf("ir_learn_read_info=%d\n",ir_learn_info.ac_num);
           
	for(u8 i=0;i<ir_learn_info.ac_num;i++)
	 {	 
	  printf("table=%d\n", ir_learn_info.ac_table[ i]);
	 }
}

void add_ac_ir_learn_device(u16 device)
{
	u8 i=0;

	  for( i=0;i<MAX_AC_LEARN_IR_NUM;i++)
	   {
	   if(device==ir_learn_info.ac_table[i])
	   	{
	   	
		printf("add_device is exist =%d %d\n",device,i);
		return ;
	   	}
	  
	  }


      if(ir_learn_info.ac_num<MAX_AC_LEARN_IR_NUM)
      {
       ir_learn_info.ac_num++;
      }

	for( i=0;i<MAX_AC_LEARN_IR_NUM;i++)
	 {
	ir_learn_info.ac_table[MAX_AC_LEARN_IR_NUM-i]=ir_learn_info.ac_table[MAX_AC_LEARN_IR_NUM-i-1];
	}
	
	ir_learn_info.ac_table[0]=device;

    printf("add_ac_ir_lear num=%d\n",ir_learn_info.ac_num);
     for( i=0;i<ir_learn_info.ac_num;i++)
      {	  
       printf("table=%d\n", ir_learn_info.ac_table[ i]);
      }
	 ir_learn_save_info();
	 ir_learn_info.ac_brand=0;
	 ir_learn_info.ac_search=0xff;

   
}
void add_ac_ir_learn_brand(u8 brand)
{

  printf("add_ac_ir_learn_brand=%d\n", brand);
  if(brand<MAX_BRAND_NUM)
  	{
         ir_learn_info.ac_num=0;
         ir_learn_info.ac_brand=brand;
	 ir_learn_info.ac_search=0xff;
         ir_learn_save_info();
  	}
   
}






void ir_emtter_save_info(void)
{
	printf("save_info airInfo.brand=%d\n",airInfo.pp_num);

	syscfg_write(VM_IR_EMITTER_INFO, &airInfo,sizeof(AcInputParamType));
}

void airInfo_reset(void)
{
	memset(&airInfo,0x00,sizeof(AcInputParamType));
	airInfo.area = 0;
	airInfo.device = 79;//BRAND_GREE
	airInfo.keynum = ACKEY_POWER;
	airInfo.mode = ACMODE_COOL;
	airInfo.fan = ACFAN_AUTO;
	airInfo.swing = (AC_SWING_OFF<<AC_SWING_UD)|(AC_SWING_OFF<<AC_SWING_LR);
	airInfo.udsf = AC_UDSF_OFF;
	airInfo.lrsf = AC_LRSF_OFF;
	airInfo.temp = 26;
	airInfo.auto_temp = AC_AUTO_TEMP_0;
	airInfo.timer = 0;
	airInfo.flag = AC_FLAG_LIGHT;
	airInfo.pp_num=0;////格力,美的,海尔,奥克斯，小米
	airInfo.brand=BRAND_GREE;

}


void factory_reset(void)
{

   	led_set_flash(8);
          #ifdef ASR_FLASH_LED
          asr_led_flash(3,800);
          #endif	
		  
		  local_irq_disable();
	 memset(&ir_learn_info,0x00,sizeof(IR_LEARN_INFO));
	  ir_learn_save_info();
	  
	  airInfo_reset();
	  ir_emtter_save_info();

       #ifdef EN_FAN_IR
         clear_ir_learn_data();
       	#endif
		
		local_irq_enable();
  printf("factory_reset5555555555=%d\n",airInfo.pp_num);

}

void ir_emtter_read_info(void)
{
    int ret = 0;
   ret=syscfg_read(VM_IR_EMITTER_INFO, &airInfo,sizeof(AcInputParamType));
    if (ret !=sizeof(AcInputParamType))
    {
        printf("airInfo is null\n");
	airInfo_reset();

    }
	printf("airInfo.pp_num=%d\n",airInfo.pp_num);
	//airInfo.device = PPCODE_TAB[airInfo.pp_num-1][0];
	printf("airInfo.device=%d\n",airInfo.device);
	ir_learn_read_info();


}


#define PWM_TIMER_UNIT_US     10//  50// 250  //单位us

 u16	ir_emtter_timers = 0;
 u16	ir_array_num = 0;
 u16	ir_carrior_len = 0;
static u8	ir_emtter_on = 0;
static u8  is_flashing = 0;
static u16	set_brand_timer = 0;
u8 is_true_cmd=0;
static u16	power_set_brand_timer = 0;

static uint16_t code_inc_brk = 0;
static uint16_t save_code_num = 0;

void ir_emitter_pwm_duty(u8 duty)
{
#if IR_EMITTER_PWM_LOW_EN
duty=255-duty;
#endif
//user_set_mcpwm_duty(pwm_ch0,duty);

user_set_pwm_duty(IR_EMITTER_PWM_TIMER,duty);

}

void   ir_emitter_stop(void)
{
ir_emtter_timers=0;
ir_array_num = 0;
ir_carrior_len = 0;


ir_emitter_pwm_duty(0);
//mcpwm_close(pwm_ch0);

PWM_dis_output_channle(IR_EMITTER_PORT,IR_EMITTER_PWM_TIMER);


#ifdef	IR_EMITTER_PORT_USE_MIC_BIAS

 printf("MIC_BIAS out \n");

mic_bias_ldo(1);
#endif


led_set_flash(1);

IR_EMITTER_TIME_REG->CON &= ~(BIT(1) | BIT(0));
unrequest_irq(IR_EMITTER_IRQ_IDX);

ir_emtter_on = 0;
#ifdef USE_TIMER_FOR_PWM
#if IR_EMITTER_IRQ_IDX==PWM_IRQ_TIME_IDX
	 pwm_timer_open();
#endif
#endif


}

#if(IR_LIB_TYPE==lib_irB)
#define IR_UNIT_US   PWM_TIMER_UNIT_US

#else
#define IR_UNIT_US     (PWM_TIMER_UNIT_US/10)//  50// 250  //单位us
#endif



___interrupt
static void ir_emitter_isr()
{

    IR_EMITTER_TIME_REG->CON |= BIT(14);
 ir_send_data_handle(g_ir_arrays);	
 // ir_send_data_handle();	

}

static void ir_emitter_timer_open(void)
{
    u32 timer_clk = 24000000;
	printf("ir_emitter_timer_open \n");

request_irq(IR_EMITTER_IRQ_IDX, 7, ir_emitter_isr, 0); 
//SFR(PWM_TIMER->CON, 10, 4, 7); 
SFR(IR_EMITTER_TIME_REG->CON, 10, 4, TIMER_SRC_STD_24M); //???????std24m

SFR(IR_EMITTER_TIME_REG->CON, 4, 4, TIMER_PRESCALE_1); 

IR_EMITTER_TIME_REG->CNT = 0; 
//PWM_TIMER->PRD = 12000000 / 1000000 / 4 * PWM_TIMER_UNIT_US;

IR_EMITTER_TIME_REG->PRD = 24000000 / 1000000*PWM_TIMER_UNIT_US;

IR_EMITTER_TIME_REG->CON |= BIT(14);
IR_EMITTER_TIME_REG->CON |= BIT(0);
}




void ir_emitter_start(void)
{


	 printf("ir_emitter_start \n");
	 
     #ifdef USE_TIMER_FOR_PWM
     #if IR_EMITTER_IRQ_IDX==PWM_IRQ_TIME_IDX
     	  pwm_timer_close();
     #endif
     #endif

     led_pwm_on_off(0);
   #ifdef  IR_EMITTER_PORT_USE_MIC_BIAS
   mic_bias_ldo(0);
   #endif



	 ir_emtter_timers=0;
	  ir_array_num = 0;
        ir_carrior_len = 0;

        if(ir_emtter_on==0)
        {
     //  ir_emitter_mc_pwm_int(pwm_ch0,IR_EMITTER_PWM_FRE);   
   user_pwm_init(IR_EMITTER_PORT,IR_EMITTER_PWM_TIMER,IR_EMITTER_PWM_OUTPUT_CHANNEL,IR_EMITTER_PWM_FRE);
        ir_emitter_pwm_duty(0);	  
        ir_emitter_timer_open();
        }

      
	  printf("ir_emitter_start ok\n");

        ir_emtter_on = 1;


}




void housekeeper_irc_set_ac_state(bool is_on) {

    if(is_on)
    	airInfo.flag |= AC_FLAG_POWER_ON;
    else
    	airInfo.flag &= (~AC_FLAG_POWER_ON);
    airInfo.keynum = ACKEY_POWER;
}


void housekeeper_irc_set_ac_mode(AC_MODE_TYPE mode) {

	airInfo.flag |= AC_FLAG_POWER_ON;
	airInfo.mode = mode;
	airInfo.keynum = ACKEY_MODE;

	if((airInfo.mode==AC_MODE_AUTO) || (airInfo.mode==AC_MODE_DRY)) {
		airInfo.fan = AC_WIND_AUTO;
	}
}



void housekeeper_irc_set_ac_wind(AC_WIND_SPEED speed) {
	airInfo.flag |= AC_FLAG_POWER_ON;
	airInfo.fan = speed;
	airInfo.keynum = ACKEY_FAN;

	if((airInfo.mode==AC_MODE_AUTO) || (airInfo.mode==AC_MODE_DRY)) {
		airInfo.fan = AC_WIND_AUTO;
	}
}


void housekeeper_irc_set_ac_tmp(uint8_t dat) {
	uint8_t tmp = dat;
	airInfo.flag |= AC_FLAG_POWER_ON;
	if(airInfo.temp>tmp)
		airInfo.keynum = ACKEY_DEC;
	else
		airInfo.keynum = ACKEY_INC;
	airInfo.temp = tmp;
}





void housekeeper_irc_set_ac_swept(uint8_t swept) {
	if(swept==0){
		airInfo.udsf = AC_UDSF_AUTO;
		airInfo.lrsf = AC_LRSF_AUTO;
		airInfo.keynum = ACKEY_WIND_UD;
	} else if(swept==1){
		airInfo.udsf = AC_UDSF_OFF;
		airInfo.lrsf = AC_LRSF_OFF;
		airInfo.keynum = ACKEY_WIND_UD;
	} else if(swept==2){
		airInfo.udsf = AC_UDSF_AUTO;
		airInfo.keynum = ACKEY_WIND_UD;
	} else if(swept==3){
		airInfo.lrsf = AC_LRSF_AUTO;
		airInfo.keynum = ACKEY_WIND_LR;
	} else if(swept==4){
		airInfo.udsf = AC_UDSF_OFF;
		airInfo.keynum = ACKEY_WIND_UD;
	} else if(swept==5){
		airInfo.lrsf = AC_LRSF_OFF;
		airInfo.keynum = ACKEY_WIND_LR;
	}
}



static u16 AC_Timer=0;
u32 AC_Timer_off_cnt=0;
u32 AC_Timer_on_cnt=0;

void ac_timeout_check(void)
{

if(AC_Timer_off_cnt)
             {
         
         		if(--AC_Timer_off_cnt==0)
         		{
         			if(AC_Timer_on_cnt==0)
         				{
         			sys_s_hi_timer_del(AC_Timer);
         			AC_Timer=0;
         				}
         			housekeeper_irc_set_ac_state(0);
					
		                 ir_emitter(ACTION_CLOSE);
         		}
           }
 if(AC_Timer_on_cnt)
             {
         
         		if(--AC_Timer_on_cnt==0)
         		{
         			if(AC_Timer_off_cnt==0)
         				{
         			sys_s_hi_timer_del(AC_Timer);
         			AC_Timer=0;
         				}
         			housekeeper_irc_set_ac_state(1);
					
					ir_emitter(ACTION_OPEN);
         		}
           }
		
}

void AC_Timer_off_Set(u32 second) 
	{
		AC_Timer_off_cnt=second;

			if(AC_Timer_off_cnt)
				{
				if(AC_Timer==0)
					{
		                        AC_Timer = sys_s_hi_timer_add(NULL,ac_timeout_check, 1000);
					}
				      #ifdef BEEP_IO
                        		deep_timer_start(2);
                        	         #endif
                                led_set_flash(2*second/(60*60));
				}
		else
		{
                    
                    led_set_flash(2);
                 #ifdef BEEP_IO
		deep_timer_start(1);
	         #endif

              }

	printf("AC_Timer_off_cnt=%d\r\n",AC_Timer_off_cnt);
	
			
	}

void AC_Timer_on_Set(u32 second) 
	{
		AC_Timer_on_cnt=second;

			if(AC_Timer_on_cnt)
				{
				if(AC_Timer==0)
					{
		                        AC_Timer = sys_s_hi_timer_add(NULL,ac_timeout_check, 1000);
					}
				
                                led_set_flash(2*second/(60*60));
				#ifdef BEEP_IO
                                deep_timer_start(2);
                                #endif
				}
		else
		{
                    
                    led_set_flash(2);
               #ifdef BEEP_IO
                deep_timer_start(1);
                #endif

              }
	
	printf("AC_Timer_on_cnt=%d\r\n",AC_Timer_on_cnt);
	
			
	}

void housekeeper_irc_set_ac_light(bool light) 
{
	if(light)
		airInfo.flag |= AC_FLAG_LIGHT;
	else
		airInfo.flag &= (~AC_FLAG_LIGHT);
	airInfo.keynum = ACKEY_LIGHT;
}

typedef enum {
  IR_SEARCH_IDLE = 0,
  IR_SEARCH_SEND,
  IR_SEARCH_KWS,

}SEARCH_IR_STATUS;
static u8 search_ir_status=IR_SEARCH_IDLE;
static u8 search_timer_out=0;


void ac_search_ir_send_handle(void)
{


	  
	if(ir_learn_info.ac_search<MAX_search_BRAND_NUM)  
	{
	if(search_tab[ir_learn_info.ac_search][send_cnt]==0&&send_cnt<5)
	send_cnt++;
	airInfo.device=search_tab[ir_learn_info.ac_search][send_cnt];
	} 
	
	if(airInfo.device&&send_cnt<5)
	{				 
                   
                   if(0 == getAirConditionerDat(&airInfo,&g_ir_arrays->ir_arrays[0].ir_carrior_len, &g_ir_arrays->ir_arrays[0].ir_carriors[0]))
                   {
                   
                   // g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carriors[g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len-1]+=4000;
                   
                   //printf("lead =%d\n",g_ir_arrays->ir_arrays[0].ir_carriors[0]);
                   //g_ir_arrays->ir_arrays[0].ir_carriors[0]-=15;
                   //	printf("read =%d\n",read_arrays.ir_arrays[0].ir_carrior_len);
                   //if(read_arrays.ir_arrays[0].ir_carrior_len>IR_CARRIOR_MAX)
                   //	read_arrays.ir_arrays[0].ir_carrior_len=IR_CARRIOR_MAX;
                   
                   
                   for( int j=0;j<g_ir_arrays->ir_arrays[0].ir_carrior_len;j++)
                   {
                   g_ir_arrays->ir_arrays[0].ir_carriors[j]=g_ir_arrays->ir_arrays[0].ir_carriors[j]/IR_UNIT_US;
                   
                   //printf("%d\n", g_ir_arrays->ir_arrays[0].ir_carriors[j]);
                   // g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len++;
                   }
                   
                   printf("irA device=%d,   ir_carrior_len %d\n\r",airInfo.device,g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len);
                   
                   g_ir_arrays->ir_array_num=1;
                   send_cnt++;	 
                  
                   }
                   else
                   {
                   
                   g_ir_arrays->ir_array_num=0;
                   printf("getAirConditionerDat error\n");
                   
                   }
					   
	}


}

void search_handle(void)
{
	printf("search_ir_status=%d\n",search_ir_status);
	byle_kws_timer_out_set(BYLE_ASR_TIMEOUT);		

   switch(search_ir_status)
   	{
              case  IR_SEARCH_IDLE:
	
       	       #ifdef BYLE_KWS_ASR
                 byle_kws_app_pause(1);
                 #endif
		 printf("byle_kws_app_pause=%d\n",search_ir_status);
		g_ir_arrays->ir_array_num=0;	
	       search_timer_out=0;
		 send_cnt=0;
		search_ir_status=	IR_SEARCH_SEND;
		ir_emitter_start();
				 
               break;
	     case  IR_SEARCH_SEND:

		     if(send_cnt>=5)
                  	{
                         ir_emitter_stop();
			 led_set_flash(20);
                        #ifdef BYLE_KWS_ASR
                       byle_kws_app_pause(0);
                       #endif
					   
			  printf("byle_kws  ing=%d\n",search_ir_status);
                  	search_timer_out=30;
					
				search_ir_status=	IR_SEARCH_KWS;
				break;
                  	}
			 
		  if(g_ir_arrays->ir_array_num==0)	 
		   ac_search_ir_send_handle();	   
         	break;

			
               case  IR_SEARCH_KWS:
		   if(search_timer_out)
                   {
                   
                         if(--search_timer_out==0)
                         {   
                             ir_learn_info.ac_search++;
                                if(ir_learn_info.ac_search>=MAX_search_BRAND_NUM)
                                {
                                ir_status=IR_IDLE;
				led_set_flash(2);
                                #ifdef ASR_FLASH_LED
                                asr_led_flash_finish();
                                #endif
                                ir_learn_info.ac_search=0xff;
                                }
                             else
                             	{
				 search_ir_status=IR_SEARCH_IDLE;
				 search_timer_out=0;
				send_cnt=0;
				
				printf("ac_search next=%d\n",ir_learn_info.ac_search);
                             	}
                         }		 
		 }
               break;


			   
   	}

	

}
bool is_scaning = false;

void ac_scan_exit() 
	{

	ir_status=IR_IDLE;
	is_scaning = false;
	code_inc_brk = 0;
	ir_learn_save_info();
}

void housekeeper_irc_ac_scan_start(void)
{
	is_scaning = true;
         save_code_num = airInfo.device;
         code_inc_brk = 0;
	ir_status=IR_AC_SEARCH;
	search_ir_status=IR_SEARCH_IDLE;
	ir_learn_info.ac_search=0;


	// user_send_key_msg(MSG_LED_BLUE);

         led_set_flash(5);
         #ifdef ASR_FLASH_LED
         asr_led_flash_start();
         #endif

	airInfo.area = 0;
	airInfo.keynum = ACKEY_POWER;
	airInfo.mode = ACMODE_COOL;
	airInfo.fan = ACFAN_AUTO;
	airInfo.swing = (AC_SWING_OFF<<AC_SWING_UD)|(AC_SWING_OFF<<AC_SWING_LR);
	airInfo.udsf = AC_UDSF_OFF;
	airInfo.lrsf = AC_LRSF_OFF;
	airInfo.temp = 26;
	airInfo.auto_temp = AC_AUTO_TEMP_0;
	airInfo.timer = 0;
	airInfo.flag = AC_FLAG_POWER_ON|AC_FLAG_LIGHT;
	airInfo.device = search_tab[ir_learn_info.ac_search][0];
	 ir_emitter(ACTION_OPEN);
}

#ifdef IR_RECEIVE_IO
 u32 	ir_receive_timers = 0;
uint8_t rc_setp = 0;
static u16 is_matching = 0;
u8 is_receive_ok = 0;
#define RECEIVE_TIMER_UNIT_US     10//  50// 250  //单位us
 u8	timer_unit_us = RECEIVE_TIMER_UNIT_US;

 


int get_ir_receive_io()
{
  return gpio_read(IR_RECEIVE_IO);
}




___interrupt
static void ir_receive_isr()
{

    IR_EMITTER_TIME_REG->CON |= BIT(14);


	
    if(is_matching) 	
    	{

        ir_receive_data_handle(g_ir_arrays);
    	}


}

void   ir_timer_stop(void)
{

IR_EMITTER_TIME_REG->CON &= ~(BIT(1) | BIT(0));
rc_setp = 0;

unrequest_irq(IR_EMITTER_IRQ_IDX);
//printf("ir_receiver timer_stop \r\n");
}



static void ir_receive_timer_open(void)
{
  u32 prd_cnt;
  u8	timer_div_index;
  	#ifdef IR_POWER_PORT
	IR_POWER_ON();
        #endif
  printf("ir_receive_timer_open \r\n");
   for (timer_div_index = 0; timer_div_index < (sizeof(timer0_div) / sizeof(timer0_div[0])); timer_div_index++) {
   prd_cnt = RECEIVE_TIMER_UNIT_US * (APP_TIMER_CLK / 1000000) / timer0_div[timer_div_index];
   if (prd_cnt > MIN_TIME_CNT && prd_cnt < MAX_TIME_CNT) {
   break;
   }
   }
   IR_EMITTER_TIME_REG->CON = BIT(14);//清pending
    IR_EMITTER_TIME_REG->CNT = 0;
    IR_EMITTER_TIME_REG->PRD = prd_cnt;
    request_irq(IR_EMITTER_IRQ_IDX, 6, ir_receive_isr, 0);
       IR_EMITTER_TIME_REG->CON = (timer_div_index << 4) | BIT(0); //lsb clk
	  timer_unit_us = RECEIVE_TIMER_UNIT_US;
	   rc_setp = 0;


	   
}



void user_in_ac_match() 
{
	printf("user_in_ac_match ing....\r\n");

  

	is_matching=20*10; // 5s 超时
	is_receive_ok = 0;
	memset(g_ir_arrays->ir_arrays[0],0,IR_CARRIOR_MAX);
	g_ir_arrays->ir_arrays[0].ir_carrior_len = 0;	
	ir_receive_timer_open();
         #ifdef BYLE_KWS_ASR
         #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
        byle_kws_timer_out_set(20);
	 #endif
         #endif
		 
         #ifdef BYLE_CMD_KWS
	 cmd_kws_timer_out_set(20);
         #endif
}
// match start






void housekeeper_irc_ac_match_stop() 
{
	is_matching = 0;
	is_receive_ok = 0;
	ir_status=IR_IDLE;
        #ifdef IR_RECEIVE_IO
        ir_timer_stop();
        led_pwm_on_off(2);
        #ifdef ASR_FLASH_LED
        asr_led_flash_finish();
        #endif	
        #endif

	   #ifdef BYLE_CMD_KWS
	 restar_music_resume();
	 #endif
	printf("exit match!\r\n");
}

void handle_receive_data() {
	uint16_t codenum = 0;
	
	//printf("handle_receive_data:\r\n");
	//	ir_timer_stop();
		/*for(codenum=0; codenum<wave_num; codenum+=2) {
			data[codenum] -= 1;
		}

		for(codenum=1; codenum<wave_num; codenum+=2) {
			data[codenum] += 2;
		}*/

	//	if(g_ir_arrays->ir_arrays[0].ir_carrior_len>280)
		//	g_ir_arrays->ir_arrays[0].ir_carrior_len=280;
		//g_ir_arrays->ir_arrays[0].ir_carrior_len--;
		
	printf("handle_receive_len%d:\r\n",g_ir_arrays->ir_arrays[0].ir_carrior_len);
		for(codenum=0; codenum<g_ir_arrays->ir_arrays[0].ir_carrior_len; codenum++) {
			
			printf("d[%d]:%d",codenum+1,g_ir_arrays->ir_arrays[0].ir_carriors[codenum]*RECEIVE_TIMER_UNIT_US);
			g_ir_arrays->ir_arrays[0].ir_carriors[codenum]=g_ir_arrays->ir_arrays[0].ir_carriors[codenum]*RECEIVE_TIMER_UNIT_US/40;
			if(codenum%2==0)
				g_ir_arrays->ir_arrays[0].ir_carriors[codenum] -= 1;
			else
				g_ir_arrays->ir_arrays[0].ir_carriors[codenum] += 2;
				
			}

		
		gByle_kws_ctl.is_audio_on=1;


		if(0 == matchAirConditionerCode(g_ir_arrays->ir_arrays[0].ir_carriors[0],g_ir_arrays->ir_arrays[0].ir_carrior_len,&codenum)) {

                        is_matching=0; // 5s 超时
                        is_receive_ok=0;
                        housekeeper_irc_ac_match_stop();

			
			if(codenum == 633|| codenum == 634)//测试代码				//BZ		US320B4_RECEIVE_CFG
			{
				
				user_send_key_msg(MSG_IR_RECEIVE_TEST_OK);
				
			}else 
 		            {
 				
                                 add_ac_ir_learn_device(codenum);
 				printf("ac code ok11111111111:%d\r\n",codenum);
 				led_set_flash(6);
 				   #ifdef ASR_FLASH_LED
                            	   asr_led_flash(3,800);
                            	   #endif	
 				 #ifdef AC_TONE_PAIROK
        		                 kws_play_tone(AC_TONE_PAIROK);				 
                              #endif
 			}
							 
		} else {
			printf("match err\r\n");
				
				is_matching=0; // 5s 超时
				is_receive_ok=0;
				
				gByle_kws_ctl.is_audio_on=0;
			 #ifdef AC_TONE_PAIRERR
 		       kws_play_tone(AC_TONE_PAIRERR);
                       #endif
			   housekeeper_irc_ac_match_stop();
				 #ifdef ASR_FLASH_LED
                           	   asr_led_flash(1,800);
                           	   #endif	
                        	led_set_flash(2);
		}
}






void  ac_match_handle(void)
{

#ifdef AC_TONE
if(tone_get_status())
{
is_flashing = 1;
return ;
}
#endif


if(is_flashing)
{
  is_flashing--;
  
  printf("is_flashing=%d\r\n",is_flashing);

     
	 if(is_flashing==3)
		 led_pwm_on_off(0);

		 
  if(is_flashing==0)
  	{
      
	  led_pwm_on_off(0);
         user_in_ac_match();
  	}
}


  if(is_matching)
{

    is_matching--;
	if(is_matching==0)
	   {
	
              #ifdef AC_TONE_PAIRERR
	         kws_play_tone(AC_TONE_PAIRERR);
		  #endif
		  housekeeper_irc_ac_match_stop();
		  
                   led_set_flash(2);
	   }

}

      if(is_receive_ok>=4)
      {
      
	 printf("receive_ir cnt=%d,\r\n",is_receive_ok);
	     ir_status=IR_IDLE;
	     ir_timer_stop();
              handle_receive_data();
      }


 


}


void housekeeper_irc_ac_match_start(void) 
{

#ifdef IR_RECEIVE_IO

	gpio_direction_input(IR_RECEIVE_IO);
	gpio_set_pull_down(IR_RECEIVE_IO, 0);
	gpio_set_pull_up(IR_RECEIVE_IO, 1);
	gpio_set_die(IR_RECEIVE_IO, 1);

          #ifdef ASR_FLASH_LED
         asr_led_flash_start();
         #endif
    	led_set_flash(20);
   	 is_flashing = 10;
	#ifdef AC_TONE
	is_flashing = 30;
	#endif
	ir_status=IR_AC_MACH;

          #ifdef BYLE_CMD_KWS
          cancel_music_resume();
          #endif
#endif

printf("housekeeper_irc_ac_match_start\r\n");

}
#if defined(EN_FAN_IR)||defined(EN_TV_IR)||defined(EN_LAMP_IR)

u8 pair_fan_ir_key=0;
u8 pair_tv_ir_key=0;
u8 pair_lamp_ir_key=0;



static void ir_learn_Info_vm_write(learn_ir_array *p, u8 index)
{

    s32 ret = 0;
	
   ret = syscfg_write(VM_FAN_IR_INFO + index, p, sizeof(learn_ir_array));
    if (ret < 0) {
        return;
    }
printf("ir_learn_Info_vm_write %d ok:\r\n",index);

}

static void ir_learn_Info_vm_read(learn_ir_array *p, u8 index)
{

    s32 ret = 0;
    ret = syscfg_read(VM_FAN_IR_INFO + index, p, sizeof(learn_ir_array));
    if (ret < 0) {

		p->ir_carrior_len=0;
        return;
    }
	printf("ir_learn_Info_vm_read %d ok:\r\n",index);

}

void clear_ir_learn_data(void)
{
u8 i;
learn_ir_array  fanirInfo;
fanirInfo.ir_carrior_len=0;
u8 max_vm_key=0;

#ifdef EN_FAN_IR
 max_vm_key+=MAX_FAN_KEY;
#endif

#ifdef EN_TV_IR
max_vm_key+=MAX_TV_KEY;
#endif

#ifdef EN_LAMP_IR
max_vm_key+=MAX_LAMP_KEY;
#endif

printf("max_vm_key%d :\r\n",max_vm_key);

for(i=0; i<max_vm_key; i++)
ir_learn_Info_vm_write(&fanirInfo,i);

}


bool play_key_next_tone(void)
{




if(pair_fan_ir_key)
{
#ifdef AC_TONE_FAN_IR_KEY
if(pair_fan_ir_key>1)
  kws_play_tone(AC_TONE_FAN_IR_KEY+MAX_FAN_KEY-pair_fan_ir_key);
#endif
printf("MAX_FAN_KEY-pair_fan_ir_key %d ok:\r\n",MAX_FAN_KEY-pair_fan_ir_key);

pair_fan_ir_key--;

}
#ifdef EN_TV_IR
else if(pair_tv_ir_key)
{
#ifdef AC_TONE_TV_IR_KEY
  if(pair_tv_ir_key>1)
  kws_play_tone(AC_TONE_TV_IR_KEY+MAX_TV_KEY-pair_tv_ir_key);
#endif
printf("MAX_FAN_KEY-pair_fan_ir_key %d ok:\r\n",MAX_TV_KEY-pair_tv_ir_key);

pair_tv_ir_key--;
}
#endif

#ifdef EN_LAMP_IR
 else if(pair_lamp_ir_key)
 {
#ifdef AC_TONE_LAMP_IR_KEY
   if(pair_lamp_ir_key>1)
   kws_play_tone(AC_TONE_LAMP_IR_KEY+MAX_LAMP_KEY-pair_lamp_ir_key);
#endif
 printf("MAX_LAMP_KEY %d ok:\r\n",MAX_LAMP_KEY-pair_lamp_ir_key);
 
 pair_lamp_ir_key--;
 }
#endif



   if(pair_fan_ir_key==0
#ifdef EN_TV_IR
  &&pair_tv_ir_key==0
 #endif
 #ifdef EN_LAMP_IR
  &&pair_lamp_ir_key==0
 #endif
  )
 {
  return 1;
 }




 return 0;
}

void handle_learn_receive_data() 
	{
	u8 max_vm_key=0;
	learn_ir_array	learnirInfo;
	printf("handle_learn_receive_data33=%d:\r\n",g_ir_arrays->ir_arrays[0].ir_carrior_len);
	learnirInfo.ir_carrior_len=0;
	if(g_ir_arrays->ir_arrays[0].ir_carrior_len>MAX_IR_CARRIOR)
		g_ir_arrays->ir_arrays[0].ir_carrior_len=MAX_IR_CARRIOR;
		for(learnirInfo.ir_carrior_len=0; learnirInfo.ir_carrior_len<g_ir_arrays->ir_arrays[0].ir_carrior_len; learnirInfo.ir_carrior_len++) {
			
			learnirInfo.ir_carriors[learnirInfo.ir_carrior_len]=g_ir_arrays->ir_arrays[0].ir_carriors[learnirInfo.ir_carrior_len];
			printf("d[%d]:%d",learnirInfo.ir_carrior_len,learnirInfo.ir_carriors[learnirInfo.ir_carrior_len]);
                             if(g_ir_arrays->ir_arrays[0].ir_carriors[learnirInfo.ir_carrior_len]>(10000/RECEIVE_TIMER_UNIT_US)&&learnirInfo.ir_carrior_len>10)
                             {
                             printf(" dele repeat key %d  max=%d:\r\n",g_ir_arrays->ir_arrays[0].ir_carriors[learnirInfo.ir_carrior_len],learnirInfo.ir_carrior_len);
                             break;
                             }
			}
		
local_irq_disable();

#ifdef EN_FAN_IR
    if(pair_fan_ir_key)
    	{
    	
		printf(" fan ir_learn vm id %d :\r\n",MAX_FAN_KEY-pair_fan_ir_key);
     ir_learn_Info_vm_write(&learnirInfo,MAX_FAN_KEY-pair_fan_ir_key);
    	}
#endif

#ifdef EN_TV_IR
    if(pair_tv_ir_key)
    	{
        #ifdef EN_FAN_IR
        max_vm_key+=MAX_FAN_KEY;
        #endif
		
	printf(" tv ir_learn vm id %d :\r\n",max_vm_key+MAX_TV_KEY-pair_tv_ir_key);
        ir_learn_Info_vm_write(&learnirInfo,max_vm_key+MAX_TV_KEY-pair_tv_ir_key);
    	}
#endif	

#ifdef EN_LAMP_IR
    if(pair_lamp_ir_key)
    	{
		
     #ifdef EN_FAN_IR
     max_vm_key+=MAX_FAN_KEY;
       #endif

       #ifdef EN_TV_IR
       	 max_vm_key+=MAX_TV_KEY;
       #endif

	   printf(" lamp ir_learn vm id %d :\r\n",max_vm_key+MAX_LAMP_KEY-pair_lamp_ir_key);
     ir_learn_Info_vm_write(&learnirInfo,MAX_LAMP_KEY+max_vm_key-pair_lamp_ir_key);
    	}
#endif	
local_irq_enable();

}

void IR_receive_start(void) 
{
	printf("IR_receive_start ing....\r\n");
	gpio_direction_input(IR_RECEIVE_IO);
	gpio_set_pull_down(IR_RECEIVE_IO, 0);
	gpio_set_pull_up(IR_RECEIVE_IO, 1);
	gpio_set_die(IR_RECEIVE_IO, 1);
          #ifdef IR_POWER_PORT
	IR_POWER_ON();
	#endif
	is_receive_ok = 0;
	memset(g_ir_arrays->ir_arrays[0],0,IR_CARRIOR_MAX);
	g_ir_arrays->ir_arrays[0].ir_carrior_len = 0;
	ir_receive_timer_open();
}


void ir_learn_stop() 
{
	is_receive_ok = 0;
	is_matching=0;
	
	ir_status=IR_IDLE;
	ir_timer_stop();
	  led_pwm_on_off(2);
       #ifdef IR_POWER_PORT
	IR_POWER_OFF();
	#endif	

	   #ifdef BYLE_CMD_KWS
	 restar_music_resume();
	 #endif
	printf("exit match!\r\n");
}



 void ir_receive_timer_stop()
{
	 is_receive_ok = 0;
	 ir_timer_stop();
      #ifdef IR_POWER_PORT
	 IR_POWER_OFF();
      #endif	

      #ifdef BYLE_KWS_ASR
        byle_kws_app_pause(0);
      #endif
	

}

void  ir_learn_handle(void )
{
static u8 one_timer_ouu_cnt=0;


#ifdef AC_TONE
if(tone_get_status())
{
is_flashing = 1;
return ;
}
#endif
if(is_flashing)
{
  is_flashing--;
  
  printf("is_flashing=%d\r\n",is_flashing);

		 
  if(is_flashing==0)
  	{
         led_pwm_on_off(0);
         IR_receive_start();
	one_timer_ouu_cnt=10*10;	
	is_matching=3*one_timer_ouu_cnt; // 3 个学习按键 超时
	#ifdef BYLE_KWS_ASR
        byle_kws_app_pause(1);
	#endif
  	}
}


  if(is_receive_ok)
  {
  is_receive_ok=0;
  
         one_timer_ouu_cnt=0;
         ir_receive_timer_stop();
	handle_learn_receive_data();
	if(play_key_next_tone())
	{
	#ifdef AC_TONE_PAIROK
	 kws_play_tone(AC_TONE_PAIROK);
	 #endif	  
	 ir_learn_stop();
	   led_set_flash(6);
	}
	else
	{
	  led_set_flash(6);
	   is_flashing = 30;
	}
	   

  } 

  if(is_matching)
	{


    is_matching--;

	if(is_matching==0)
		{		
		ir_learn_stop();
		led_set_flash(3);
               #ifdef AC_TONE_PAIRERR
		kws_play_tone(AC_TONE_PAIRERR);
		#endif
               one_timer_ouu_cnt=0;
		}
	else  if(one_timer_ouu_cnt)
	   {
	          
		  if(--one_timer_ouu_cnt==0)
		  	{
       		
			ir_receive_timer_stop();
				if(play_key_next_tone())
                           	{
                            	#ifdef AC_TONE_PAIRERR
                            	 kws_play_tone(AC_TONE_PAIRERR);
                            	 #endif	  
                            	 ir_learn_stop();
				 led_set_flash(3);
                            	}
                            	else
                            	{
                            	  led_set_flash(6);
                            	   is_flashing = 30;
                            	}
		  	}

	
	   }

}


}

void ir_learn_start(void) 
{
led_set_flash(20);
is_flashing = 10;
ir_status=IR_LEARN;
#ifdef BYLE_CMD_KWS
cancel_music_resume();
#endif	
#ifdef BYLE_KWS_ASR

#if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
 byle_kws_timer_out_set(20);
#endif
#endif

}

void learn_ir_send_handle(void )
{
	if(g_ir_arrays->ir_array_num==0) 
		{
	
          	 ir_emitter_stop();
          	unrequest_irq(IR_EMITTER_IRQ_IDX);
          	send_cnt=0;
          	printf("ir stop");
               led_pwm_on_off(2);
          	if(!is_led_on_off())
          	 led_set_flash(4);
		ir_status=IR_IDLE;
		#ifdef WAIT_IR_SEND_TO_TONE
            	os_sem_post(&ir_send_sem);
             #endif
		}


}
void ir_learn_emitter(u8 action)
{
	u16 i=0,j=0,cmp_size,to_cnt;
	learn_ir_array  irInfo;

	
        if(ir_status!=IR_IDLE)
        {
        is_true_cmd=0;
           return;
        }	
	
       ir_learn_Info_vm_read(&irInfo,action);
	g_ir_arrays->ir_array_num=0;
	  
      printf("fanirInfo.ir_carrior_len %d",irInfo.ir_carrior_len);
	  if(irInfo.ir_carrior_len)
	  	{
               for( i=0;i<irInfo.ir_carrior_len;i++)
              {
		 g_ir_arrays->ir_arrays[0].ir_carriors[i]= irInfo.ir_carriors[i]*(RECEIVE_TIMER_UNIT_US/PWM_TIMER_UNIT_US);
              }
		 g_ir_arrays->ir_arrays[0].ir_carrior_len=irInfo.ir_carrior_len;
		  g_ir_arrays->ir_array_num=1;
		  printf("ir_array_num%d len%d:",i,g_ir_arrays->ir_arrays[0].ir_carrior_len);
		 // put_buf(g_ir_arrays->ir_arrays[0],g_ir_arrays->ir_arrays[0].ir_carrior_len);
		  
		  ir_status=IR_LEARN_SEND;
		  ir_emitter_start();

		  #ifdef WAIT_IR_SEND_TO_TONE
                     wati_ir_send_end();
                    #endif
	  	}
      else
      	{

        is_true_cmd=0;

      	}



}


#ifdef EN_TV_IR
void tv_ir_match_start(void) 
{

        ir_learn_start();
        printf("TV_ir_match_start\r\n");
	  is_true_cmd=1;
	  pair_tv_ir_key=MAX_TV_KEY;  
	  is_matching=10*10*MAX_TV_KEY; // 10s 超时
	  pair_fan_ir_key=0;
	  pair_lamp_ir_key=0;


}
void tv_ir_emitter(u8 action)
{
u8 max_vm_key=0;
#ifdef EN_FAN_IR
max_vm_key+=MAX_FAN_KEY;
#endif
ir_learn_emitter(max_vm_key+action);


}

#endif
#ifdef EN_LAMP_IR
void lamp_ir_match_start(void) 
{

        ir_learn_start();
         printf("lamp_ir_match_start\r\n");
	  is_true_cmd=1;
	  pair_lamp_ir_key=MAX_LAMP_KEY;  
	  is_matching=10*10*MAX_LAMP_KEY; // 10s 超时
	  pair_tv_ir_key=0;
	  pair_fan_ir_key=0;


}
void lapm_ir_emitter(u8 action)
{
	u8 max_vm_key=0;

#ifdef EN_FAN_IR
	max_vm_key+=MAX_FAN_KEY;
#endif
	
#ifdef EN_TV_IR
	max_vm_key+=MAX_TV_KEY;
#endif

ir_learn_emitter(max_vm_key+action);


}

#endif

#ifdef EN_FAN_IR

void fan_ir_match_start(void) 
{


printf("fan_ir_match_start\r\n");
ir_learn_start();
is_true_cmd=1;
is_matching=10*10*MAX_FAN_KEY; // 10s 超时
pair_fan_ir_key=MAX_FAN_KEY;	 
pair_tv_ir_key=0;
pair_lamp_ir_key=0;

}

void fan_ir_emitter(u8 action)
{
	ir_learn_emitter(action);
}
#else
void fan_ir_match_start(void) 
{
is_true_cmd=0;
}
void fan_ir_emitter(u8 action)
{

is_true_cmd=0;
}
#endif

#endif
#endif


#ifdef WAIT_IR_SEND_TO_TONE
void wati_ir_send_end(void)
{
printf("wati_ir_send...");

os_sem_pend(&ir_send_sem,300);
printf("wati_ir_send_end");


}
#endif

void ac_ir_send_loop(void)
{
	u16 i=0,j=0,device_temp=0;
#if(IR_LIB_TYPE==lib_irA)
//s_ir_a_send_arrays  read_arrays;

// 搜索空调使用

printf(" ac_num%d  ac_brand=%d  ac_search%d\r\n",ir_learn_info.ac_num,ir_learn_info.ac_brand,ir_learn_info.ac_search);

if(ir_learn_info.ac_search<MAX_search_BRAND_NUM)  
{
if(search_tab[ir_learn_info.ac_search][send_cnt]==0&&send_cnt<5)
	send_cnt++;
device_temp=search_tab[ir_learn_info.ac_search][send_cnt];
airInfo.device=device_temp;

} 
else if(ir_learn_info.ac_num)
{
    if(send_cnt<ir_learn_info.ac_num)
   device_temp=ir_learn_info.ac_table[send_cnt];
     airInfo.device=device_temp;
	 
	 printf("num%d device=%d",ir_learn_info.ac_num,device_temp);
}
else 
{
  device_temp=PPCODE_TAB[ir_learn_info.ac_brand][send_cnt];
  airInfo.device=device_temp;
}

//airInfo.device=740;

printf("byle send_cnt%d  device_temp=%d device=%d\r\n",send_cnt,device_temp,airInfo.device);


if(device_temp&&send_cnt<5)
{                

                   if(0 == getAirConditionerDat(&airInfo,&g_ir_arrays->ir_arrays[0].ir_carrior_len, &g_ir_arrays->ir_arrays[0].ir_carriors[0]))
                   {

			// g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carriors[g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len-1]+=4000;
			
			//printf("lead =%d\n",g_ir_arrays->ir_arrays[0].ir_carriors[0]);
			//g_ir_arrays->ir_arrays[0].ir_carriors[0]-=15;
                    //  printf("read =%d\n",read_arrays.ir_arrays[0].ir_carrior_len);
			//if(read_arrays.ir_arrays[0].ir_carrior_len>IR_CARRIOR_MAX)
			//  read_arrays.ir_arrays[0].ir_carrior_len=IR_CARRIOR_MAX;

			
			  for( j=0;j<g_ir_arrays->ir_arrays[0].ir_carrior_len;j++)
			  	{
			 g_ir_arrays->ir_arrays[0].ir_carriors[j]=g_ir_arrays->ir_arrays[0].ir_carriors[j]/IR_UNIT_US;
			 
			//printf("%d\n", g_ir_arrays->ir_arrays[0].ir_carriors[j]);
			// g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len++;
			  	}
			  
			  printf("irA device=%d,   ir_carrior_len %d\n\r",airInfo.device,g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len);
			  
			  g_ir_arrays->ir_array_num=1;
			 send_cnt++;	 

                   }
                   else
                   {
                   
				   g_ir_arrays->ir_array_num=0;
                   	printf("getAirConditionerDat error\n");
                   
                   }
				   
}
else
#endif

{
	ir_status=IR_IDLE;
	 ir_emitter_stop();
	send_cnt=0;
	printf("ir stop");
	led_pwm_on_off(2);

	if(!is_led_on_off())
	 led_set_flash(4);
	else
	led_set_flash(2);
        #ifdef WAIT_IR_SEND_TO_TONE
        os_sem_post(&ir_send_sem);
        #endif
          #ifdef BYLE_KWS_ASR
          byle_kws_app_pause(0);
          #endif

}





}



void ac_ir_send_handle(void)
{
	printf("a%d",g_ir_arrays->ir_array_num);

if(g_ir_arrays->ir_array_num==0) 
ac_ir_send_loop();
#ifdef MUSIC_LED_CODE_LESS
MyLed_save_Cnt=0;
#endif

}

#ifdef  MUSIC_LED

extern u8 	MyLed_save_Cnt;
#endif
void ir_emitter(int action)
{
	u16 i=0,j=0,cmp_size,to_cnt;
	printf(" ir_emitter  =%d \n",ir_status);

    if(ir_status!=IR_IDLE)
    {
    
    is_true_cmd=0;
    return;
    }	



    #if(IR_LIB_TYPE==lib_irA)
	if(IrLib_ok==0)
		{
		printf(" IrLib_error =%d \n",IrLib_ok);
		return;
		}
	#endif

#ifdef BYLE_KWS_ASR
	 byle_kws_app_pause(1);
#endif
      memset(g_ir_arrays,0,sizeof(s_ir_send_arrays));
	
     #if(IR_LIB_TYPE==lib_irB)
	 
	 s_ir_send_arrays_B temp;
       printf("ir_emitter action=%d  airInfo.brand=%d\n",action,airInfo.brand);
      
       create_ir_arrays(&temp, airInfo.brand, action);
      printf("ivw ir_array_num:%d   ir_carrior_len %d",temp.ir_array_num,temp.ir_arrays[0].ir_carrior_len);
    if(temp.ir_array_num>IR_SEND_ARRAY_MAX)
	  temp.ir_array_num=IR_SEND_ARRAY_MAX;
      for( i=0;i<temp.ir_array_num;i++)
              {
               	  if(temp.ir_arrays[i].ir_carrior_len>IR_CARRIOR_MAX)
		temp.ir_arrays[i].ir_carrior_len=IR_CARRIOR_MAX;	
				  
              	  for( j=0;j<temp.ir_arrays[i].ir_carrior_len;j++)
              	  	{
             
		 g_ir_arrays->ir_arrays[i].ir_carriors[j]= temp.ir_arrays[i].ir_carriors[j]/IR_UNIT_US;
      		printf("ir_array_num%d len%d:",i,temp.ir_arrays[i].ir_carrior_len);
              	  	}
      	      //  put_buf(g_ir_arrays->ir_arrays[i],g_ir_arrays->ir_arrays[i].ir_carrior_len);
              
              }
      
      #endif
	  



 ir_status=IR_AC_SEND;
 ir_emitter_start();
  send_cnt=0;
  
#ifdef BEEP_IO
   deep_timer_start(1);
 #endif
#ifdef MUSIC_LED_CODE_LESS
 if(MyLed_save_Cnt)
 MyLed_save_Cnt=0;
 #endif

#ifdef WAIT_IR_SEND_TO_TONE
  wati_ir_send_end();
#endif


}


void brand_time_out(void)
{
   ir_status=IR_IDLE;
   if(set_brand_timer)
   sys_hi_timeout_del(set_brand_timer);
   set_brand_timer=0;
   led_set_fade(1,50);//	 led_set_flash(2);
   #ifdef ASR_FLASH_LED
   asr_led_flash_finish();
   #endif   
   printf("brand_time_out=%d \n",set_brand_timer);
}


void set_brand_time_out(void)
{

   ir_status=IR_AC_BRAND;
   led_set_fade(100,50);//	
   #ifdef ASR_FLASH_LED
   asr_led_flash_start();
   #endif   
   if(set_brand_timer)
   sys_hi_timeout_del(set_brand_timer);
   set_brand_timer=sys_hi_timeout_add(NULL, brand_time_out, BYLE_ASR_TIMEOUT*1000);
   printf("set_brand_time_out=%d \n",set_brand_timer);
   #ifdef BEEP_IO
   deep_timer_start(2);
   #endif

}



static u8 cmd_index_repeat=0;


void air_brand_change(u8 brand)
{
	printf(" ir_status   =%d \n",ir_status);

        #ifdef REPEAT_BRAND_FOR_SET
         if(power_set_brand_timer==0&&airInfo.pp_num!=0)
        {
        	is_true_cmd=0;
        
        	return;
        } else 
        #endif

         if(ir_status!=IR_AC_BRAND&& ir_status!=IR_AC_MACH)    
         	{
         	is_true_cmd=0;
         
         	return;
         	}
         
#ifdef IR_RECEIVE_IO
         if(ir_status==IR_AC_MACH||ir_status==IR_AC_BRAND)
         {
         housekeeper_irc_ac_match_stop();
         }
#endif
	  ir_status=IR_IDLE;

      if(set_brand_timer==0)
      {
      
      #ifdef REPEAT_BRAND_FOR_SET
      #ifdef AC_TONE_ENTER_BRAND
      kws_play_tone(AC_TONE_ENTER_BRAND);
      is_true_cmd=0;
      #endif
      set_brand_time_out();
      return;
      #endif
      printf(" set_brand_time_out off  =%d \n",is_true_cmd);
      
      }
	  
      if(set_brand_timer)
      sys_hi_timeout_del(set_brand_timer);
      set_brand_timer=0;

      if(brand<MAX_BRAND_NUM)
      {
      add_ac_ir_learn_brand(brand);
      
      }

     #ifdef BEEP_IO
     deep_timer_start(3);
     #endif
     #ifdef ASR_FLASH_LED
     asr_led_flash(4,500);
     #endif   
     
     led_set_flash(6);
     // ir_emitter(ACTION_OPEN);
     
     #ifdef REPEAT_BRAND_FOR_SET
     #ifdef AC_TONE_BRAND_OK
     kws_play_tone(AC_TONE_BRAND_OK);
     is_true_cmd=0;
     #endif
     #endif
     #ifdef BYLE_APP_TYPE
     rsp_ac_status();
     #endif
}



int ir_emitter_handle_action( int  cmd_index)
	{

	
	is_true_cmd=1;
         #ifdef REPEAT_BRAND_FOR_SET
         	if(set_brand_timer==0)
         	{
         	cmd_index_repeat=cmd_index;
         	}
         	else if(cmd_index_repeat!=cmd_index)
         		{
         	      is_true_cmd=0;
         	      printf(" cmd_index_repeat=%d \n",cmd_index_repeat);
         	      return -1;
         	    }

			
         #endif

	printf("ir_emitter=%d \n",cmd_index);




	   #ifdef SEND_USER_CMD_EXIT
	           if(SEND_USER_CMD_EXIT==cmd_index)
	           	{
                  printf("is_ir_busy=%d \n",is_ir_busy());
                       if(is_ir_busy())
                       	{
                       	
                       return -1 ;
                       	}
	           	}
            #endif
	  if(cmd_index>=NLU_CONTENT_SIZE)
			return cmd_index ;

  

	switch(cmd_index)
		{
		#ifdef user_wakeup_uni1
		 case	user_wakeup_uni1:
		 #endif
		case   eCMD_wakeup_uni:
			
                          #ifdef ASR_FLASH_LED
			  asr_led_flash(2,300);
                          #endif
			   break;

		case   eCMD_BRAND_DEFAULT://7恢复出厂设置
		
		           factory_reset();
		          break;
		case   eCMD_ACTION_OPEN ://1	//8 打开空调
			   housekeeper_irc_set_ac_state(1);
		            ir_emitter(ACTION_OPEN);
		            break;
		case   eCMD_ACTION_CLOSE ://2  //9 关闭空调
			   housekeeper_irc_set_ac_state(0);
			    ir_emitter(ACTION_CLOSE);
		            break;
		case   eCMD_MODE_AUTO://	 10  //自动模式
		          housekeeper_irc_set_ac_mode(AC_MODE_AUTO);
			   ir_emitter(ACTION_MODE_AUTO);
		            break;
		case   eCMD_MODE_COOL://	 11  //制冷模式
		           housekeeper_irc_set_ac_mode(AC_MODE_COOL);
			  ir_emitter(ACTION_MODE_COOL);
		            break;
		case   eCMD_MODE_CHUSHI:// 12	//除湿模式//抽湿模式
		           housekeeper_irc_set_ac_mode(AC_MODE_DRY);
			   ir_emitter(ACTION_MODE_CHUSHI);
		            break;
		case   eCMD_MODE_TONGF://	13	//通风模式
		          housekeeper_irc_set_ac_mode(AC_MODE_FAN);
			  ir_emitter(ACTION_MODE_TONGF);
		            break;
		case   eCMD_MODE_HOT://14	 //制热模式
		            housekeeper_irc_set_ac_mode(AC_MODE_HEAT);
			   ir_emitter(ACTION_MODE_HOT);
		            break;                              
		case   eCMD_ACTION_TEMP_16://15  //16度
		housekeeper_irc_set_ac_tmp(ACTION_TEMP_16);			  
			   ir_emitter(ACTION_TEMP_16);
		            break;
	     case  	eCMD_ACTION_TEMP_17://16  //17度
			  housekeeper_irc_set_ac_tmp(ACTION_TEMP_17);			  
			   ir_emitter(ACTION_TEMP_17);
		            break;
		case   eCMD_ACTION_TEMP_18://17	 //18度
		case   eCMD_ACTION_TEMP_19://18	 //19度
		case   eCMD_ACTION_TEMP_20://19	 //20度
		case   eCMD_ACTION_TEMP_21://20	 //21度
		case   eCMD_ACTION_TEMP_22://21	 //22度
		case   eCMD_ACTION_TEMP_23://22	 //23度
		case   eCMD_ACTION_TEMP_24://23	 //24度
		case   eCMD_ACTION_TEMP_25://24	 //25度
		case   eCMD_ACTION_TEMP_26://25	 //26度
		case   eCMD_ACTION_TEMP_27://26	 //27度
		case   eCMD_ACTION_TEMP_28://27	 //28度
		case   eCMD_ACTION_TEMP_29://28	 //29度
		case   eCMD_ACTION_TEMP_30://29	 //30度
			  housekeeper_irc_set_ac_tmp(cmd_index-eCMD_ACTION_TEMP_18+18);			  
			   ir_emitter(cmd_index-eCMD_ACTION_TEMP_18+ACTION_TEMP_18);
		            break;
		case   eCMD_WIND_AUTO://	 30:// //自动风量
		
		        housekeeper_irc_set_ac_wind(AC_WIND_AUTO);
			ir_emitter(ACTION_WIND_AUTO);
				 break; 							 
		case   eCMD_WIND_MIN://31	 //最小风量
		
		        housekeeper_irc_set_ac_wind(AC_WIND_LOW);
			ir_emitter(ACTION_WIND_MIN);
				 break; 							 
		case   eCMD_WIND_MID://32	 //中等风量
		
		          housekeeper_irc_set_ac_wind(AC_WIND_MID);
			ir_emitter(ACTION_WIND_MID);
				 break; 							 
		case   eCMD_WIND_MAX://33	//最大风量
		
		      housekeeper_irc_set_ac_wind(AC_WIND_HIGH);
			ir_emitter(ACTION_WIND_MAX);
				 break; 	


	

	case 	eCMD_WIND_UDLR_AUTO://开启扫风
				  housekeeper_irc_set_ac_swept(0);
				  
				  ir_emitter(ACTION_WIND_SX);
				  break;		
				  
           case	 eCMD_WIND_UDLR_STOP://   36  //停止扫风
             
                         housekeeper_irc_set_ac_swept(1);
                         ir_emitter(ACTION_WIND_STOP);
                         break;	  

				 
		case   eCMD_WIND_UD_AUTO://34	 //上下扫风
			  housekeeper_irc_set_ac_swept(2);
		
			ir_emitter(ACTION_WIND_SX);
				 break; 							 
		case   eCMD_WIND_JZ://35	//居中扫风
		
		housekeeper_irc_set_ac_swept(0);
					ir_emitter(ACTION_WIND_JZ);
				 break; 	
		case   eCMD_WIND_UD_STOP://	 36  //停止扫风
		
		              housekeeper_irc_set_ac_swept(4);
					ir_emitter(ACTION_WIND_STOP);
				 break; 	
		case   eCMD_WIND_LR_AUTO://37	 //左右摆风
		
		             housekeeper_irc_set_ac_swept(3);
				ir_emitter(ACTION_WIND_UD);
				 break; 	
		case   eCMD_WIND_LR_STOP://38	停止摆风
		
		             housekeeper_irc_set_ac_swept(5);
					ir_emitter(ACTION_WIND_UD_STOP);
				 break; 	

             #ifdef IR_RECEIVE_IO

		case		 eCMD_BRAND_MATCH:
			housekeeper_irc_ac_match_start();//	 housekeeper_irc_ac_scan_start();	
		
			break;	   
			case		 eCMD_BRAND_MATCH_ST:

					housekeeper_irc_ac_match_stop();//  ac_scan_exit();
					break;
               #endif
#ifndef US320B4_RECEIVE_CFG		//BZ		US320B4_irA_kwsA_K1lib_ch_RGB_V3_RECEIVE
#ifdef USER_AC_MATCH_CMD			//BZ	US320B4_irA_kwsA_K1lib_ch_RGB_V3
		case		 eCMD_BRAND_MATCH:			//BZ
			set_brand_time_out();

			break;
			case		 eCMD_BRAND_MATCH_ST:				//BZ

					brand_time_out();
					break;
#endif			//BZ
#endif   //BZ
		case		 eCMD_BRAND_SET:
			
			set_brand_time_out();// housekeeper_irc_ac_scan_start();				 
				 	 break; 
		case   eCMD_BRAND_SEARCH://	39	 搜索空调
		#ifndef NO_SEARCH_AC			//BZ
			housekeeper_irc_ac_scan_start();	
			#endif		//BZ
				 break; 	
		case   eCMD_BRAND_SEARCH_STOP://	  40 停止搜索
	ac_scan_exit();
				 break; 	

                     case eCMD_TIME_1H:
                     AC_Timer_off_Set(60*60);
                     break;	   
                     case eCMD_TIME_2H:
                     AC_Timer_off_Set(2*60*60);
                     break;	  
                     case eCMD_TIME_3H:					//BZ		US320B4_irA_kwsA_K1lib_ch_RGB_V3
                     AC_Timer_off_Set(3*60*60);
                     break;
                     case eCMD_TIME_5H:					//BZ
                     AC_Timer_off_Set(5*60*60);
                     break;
                     case eCMD_TIME_6H:					//BZ
                     AC_Timer_off_Set(6*60*60);
                     break;
                     case eCMD_TIME_7H:					//BZ
                     AC_Timer_off_Set(7*60*60);
                     break;
                     case eCMD_TIME_4H:
                     AC_Timer_off_Set(4*60*60);
                     break;	   
                     case eCMD_TIME_8H:
                     AC_Timer_off_Set(8*60*60);
                     break;	  
                     case eCMD_TIME_CANCEL:
                     AC_Timer_off_Set(0);
                     break;	  

						  
		case   eCMD_BRAND_AUX://	 42
					air_brand_change(BRAND_AUX);
				 break; 	
		case   eCMD_BRAND_ZHIGAO://43
					air_brand_change(BRAND_ZHIGAO);
				 break; 	
		case   eCMD_BRAND_SONGXIA ://44
					air_brand_change(BRAND_SONGXIA);
				 break; 	
		case   eCMD_BRAND_GREE://  45
					air_brand_change(BRAND_GREE);
				 break; 	
		case   eCMD_BRAND_HAIXIN	 ://46
					air_brand_change(BRAND_HAIXIN);
				 break; 	
		case   eCMD_BRAND_HAIER:// 47
					air_brand_change(BRAND_HAIER);
				 break; 	
		case   eCMD_BRAND_KELON	 ://://48
					air_brand_change(BRAND_KELON);
				 break; 	
		case   eCMD_BRAND_MIDEA:// 49
					air_brand_change(BRAND_MIDEA);
				 break; 	
		case   eCMD_BRAND_CHANGHONG ://50
					air_brand_change(BRAND_CHANGHONG);
				 break; 	
		case   eCMD_BRAND_RILI ://51
					air_brand_change(BRAND_RILI);
				 break; 	
		case   eCMD_BRAND_DAJIN	 ://52
					air_brand_change(BRAND_DAJIN);
				 break; 	
		case   eCMD_BRAND_XIAOMI	 ://53
					air_brand_change(BRAND_XIAOMI);
				 break; 	
		case   eCMD_BRAND_SANSUNG://54
					air_brand_change(BRAND_SANSUNG);
				 break; 	
		case   eCMD_BRAND_SANLING://55
					air_brand_change(BRAND_SANLING);
				 break; 	
		case   eCMD_BRAND_HUALING://56
					air_brand_change(BRAND_HUALING);
				 break; 	
		case   eCMD_BRAND_XTE://57
					air_brand_change(BRAND_XIAOTIANER);
				 break; 	
		case   eCMD_BRAND_KANGJIA://58
					air_brand_change(BRAND_KANGJIA);
				 break; 	
		case   eCMD_BRAND_YUETU	 ://59
					air_brand_change(BRAND_YUETU);
				 break; 	
		case   eCMD_BRAND_SANLINGDIANJI:// 60
					air_brand_change(BRAND_SANLINGDIANJI);
				 break; 	
		case   eCMD_BRAND_GELANSHI://:// 61
					air_brand_change(BRAND_GELANSHI);
				 break; 	
		case   eCMD_BRAND_YANGZI://62
					air_brand_change(BRAND_YANGZI);
				 break; 	
		case   eCMD_BRAND_CHUANGWEI://63
					air_brand_change(BRAND_CHUANGWEI);
				 break; 	
		case   eCMD_BRAND_DONGZHI  ://64
					air_brand_change(BRAND_DONGZHI);
				 break; 	
		case   eCMD_BRAND_SANYANG  ://65
					air_brand_change(BRAND_SANYANG);
				 break; 	
		case   eCMD_BRAND_YORK://  66
					air_brand_change(BRAND_YORK);
				 break; 	
		case   eCMD_BRAND_YINGHUA://67
					air_brand_change(BRAND_YINGHUA);
				 break; 	
		case   eCMD_BRAND_NEITIAN://68
					air_brand_change(BRAND_NEITIAN);
				 break; 	
		case   eCMD_BRAND_XINKE	 ://://69
					air_brand_change(BRAND_XINKE);
				 break; 	
		case   eCMD_BRAND_HUABAO://70
					air_brand_change(BRAND_HUABAO);
				 break; 	
		case   eCMD_BRAND_CHANGLING	 ://71
					air_brand_change(BRAND_CHANGLING);
				 break; 	
		case   eCMD_BRAND_LESHENG://72
					air_brand_change(BRAND_LESHENG);
				 break; 	
		case   eCMD_BRAND_MAIKEWEIER://	73
					air_brand_change(BRAND_MAIKEWEIER);
				 break; 	
		case   eCMD_BRAND_FEILIPU://74
					air_brand_change(BRAND_FEILIPU);
				 break; 	
		 case   eCMD_BRAND_LG  ://75
		 case   eCMD_BRAND_LG1://76
		 			air_brand_change(BRAND_LG);
				 break; 	
		case   eCMD_BRAND_TCL://	 77
		case   eCMD_BRAND_TCL1:// 78
		case   eCMD_BRAND_TCL2:// 79
		case   eCMD_BRAND_TCL3://	 80
		case   eCMD_BRAND_TCL4://81
			air_brand_change(BRAND_TCL);
				 break; 	
              case	eCMD_BRAND_XINFEI://81
              air_brand_change(BRAND_XINFEI);
              break;	 
			  case eCMD_BRAND_XIAPU:						//BZ
              case	 eCMD_BRAND_SHAP://81
              air_brand_change(BRAND_SHAP);
              break;	  
              case   eCMD_BRAND_HUIERPU://81
              air_brand_change(BRAND_HUIERPU);
              break;	   
              case   eCMD_BRAND_CHUNLAN://81
              air_brand_change(BRAND_CHUNLAN);
              break; 	

              case	eCMD_BRAND_LEJIN://81							//BZ	乐金空调		US320B4_irA_kwsA_K1lib_ch_RGB_V3
              air_brand_change(BRAND_LEJIN);
              break;
              case	 eCMD_BRAND_AOKEMA://81							//BZ	澳柯玛
              air_brand_change(BRAND_AOKEMA);
              break;
              case   eCMD_BRAND_FUSHITONG://81							//BZ	富士通
              air_brand_change(BRAND_FUSHITONG);
              break;



				 
#ifdef EN_FAN_IR
				 
		case eCMD_SET_FAN_PAIR:
		fan_ir_match_start();
		break;	   
		case eCMD_FAN_OPEN:
			fan_ir_emitter(FAN_OPEN);			
			break;	   
		case eCMD_FAN_CLOSE:
			fan_ir_emitter(FAN_CLOSE);			
			break;	
		case eCMD_FAN_SWING:
		       fan_ir_emitter(FAN_SWING);			
			break;	
		case eCMD_FAN_SPEED:
		       fan_ir_emitter(FAN_SPEED);			
			break;			
		case eCMD_FAN_SPEED_UP:
		       fan_ir_emitter(FAN_SPEED_UP);			
			break;			
		case eCMD_FAN_SPEED_DM:
		       fan_ir_emitter(FAN_SPEED_DM);			
			break;			
		case eCMD_FAN_SPEED_MODE:
			fan_ir_emitter(FAN_SPEED_MODE);			
			break;
		case eCMD_FAN_TIMER:
		       fan_ir_emitter(FAN_TIMER);			
			break;
#endif

#ifdef EN_TV_IR	
	case eCMD_SET_TV_PAIR:
	tv_ir_match_start();
	break;	   
	case eCMD_TV_OPEN:
		tv_ir_emitter(TV_OPEN);			
		break;	   
	case eCMD_TV_CLOSE:
		tv_ir_emitter(TV_CLOSE);			
		break;	
	case eCMD_TV_CH_UP:
		   tv_ir_emitter(TV_CH_UP);			
		break;	
	case eCMD_TV_CH_DM:
		   tv_ir_emitter(TV_CH_DM);			
		break;			
	case eCMD_TV_VOL_UP:
		   tv_ir_emitter(TV_VOL_UP);			
		break;			
	case eCMD_TV_VOL_DOWM:
		   tv_ir_emitter(TV_VOL_DOWM);			
		break;			
	case eCMD_TV_MUTE:
		tv_ir_emitter(TV_MUTE); 		
		break;
	case eCMD_TV_UNMUTE:
		tv_ir_emitter(TV_MUTE); 		
		break;
	case eCMD_TV_SOURCE:
		tv_ir_emitter(TV_SOURCE); 		
		break;

#endif

#ifdef EN_LAMP_IR	
			case eCMD_SET_LAMP_PAIR:
			lamp_ir_match_start();
			break;	   
			case eCMD_LAMP_OPEN:
				lapm_ir_emitter(LAMP_OPEN); 		
				break;	   
			case eCMD_LAMP_CLOSE:
				lapm_ir_emitter(LAMP_CLOSE);			
				break;	
			case eCMD_LAMP_COLOR_TEMP_UP:
				   lapm_ir_emitter(LAMP_COLOR_TEMP_UP); 		
				break;	
			case eCMD_LAMP_COLOR_TEMP_DM:
				   lapm_ir_emitter(LAMP_COLOR_TEMP_DM); 		
				break;			
			case eCMD_LAMP_LIGHT_UP:
				   lapm_ir_emitter(LAMP_LIGHT_UP);			
				break;			
			case eCMD_LAMP_LIGHT_DM:
				   lapm_ir_emitter(LAMP_LIGHT_DM);			
				break;			
			case eCMD_LAMP_NIGHT_OPEN:
				lapm_ir_emitter(LAMP_NIGHT_OPEN); 		
				break;
			case eCMD_LAMP_NIGHT_CLOSE:
				lapm_ir_emitter(LAMP_NIGHT_CLOSE); 		
				break;
	
				break;
		
#endif

				
		}
	if(is_true_cmd==0)
        return -1;
	
	return cmd_index;
                       
    }
#ifdef REPEAT_BRAND_FOR_SET

void power_set_timer_out(void)
{

power_set_brand_timer=0;

}
#endif


bool is_ir_busy(void)
{
if(ir_emtter_on||ir_status==IR_AC_MACH)
	return 1;

return 0;
}

void ms100_timer_out(void *priv)
{
     if(ir_status!=IR_IDLE)
     {
     	os_sem_post(&ir_task_sem);
     }
}
void ir_task(void *priv )
{

	 //  if(os_sem_pend(&ir_task_sem, 0)!=1) return;
      switch(ir_status)
      {
#ifdef IR_RECEIVE_IO
      case  IR_LEARN:
      ir_learn_handle();
       break;
      case  IR_LEARN_SEND:
      learn_ir_send_handle();
      break;
      case  IR_AC_MACH:
      ac_match_handle();
      break;
#endif
      case  IR_AC_SEND:
	  	
      ac_ir_send_handle();
      break;
      case  IR_AC_SEARCH:
      search_handle();
      break;
      default:
	 break;
	  	
      }
    
}

void ir_emitter_init(void)
{
	u32 timer_clk;
    int err = 1;
  printf("ir_emitter_init\n");
#ifdef WAIT_IR_SEND_TO_TONE
os_sem_create(&ir_send_sem, 0);
#endif

  ir_emtter_read_info();
#ifdef REPEAT_BRAND_FOR_SET
   power_set_brand_timer=sys_hi_timeout_add(NULL, power_set_timer_out, BYLE_ASR_TIMEOUT*1000);
#endif

   #if(IR_LIB_TYPE==lib_irA)
         IrLibVersion soft,lib;
	char *compile_time;
	if(getIrLibVersion(&soft,&lib,&compile_time)==0)
		{
		printf("soft V%x.%02x.%02x lib V%x.%02x.%02x @ %s \n",soft.hVer,soft.mVer,soft.sVer,lib.hVer,lib.mVer,lib.sVer,compile_time);
		IrLib_ok=1;

		
		printf("Ir_LibA_ok!\n");
		}
	else
		{
		
		IrLib_ok=0;
		printf("get version err!\n");
		}


   #endif
   
   ir_status=IR_IDLE;
 //   housekeeper_irc_ac_match_start();
 	os_sem_create(&ir_task_sem, 0);
       sys_timer_add(NULL, ir_task, 100);
	   g_ir_arrays=get_mic_buf_addr();

  //err=task_create(ir_task, NULL, "ir_task");
 // if (err != OS_NO_ERR) {
	//  printf("%s creat fail %x\n", __FUNCTION__,  err);
//   }
#if(IR_LIB_TYPE==lib_irB)
ir_emitter(ACTION_OPEN);
#endif
}
#if(IR_LIB_TYPE==lib_irA)

 void send_test_data(void) 
 {
  
 
	 airInfo.device = 634;
	 housekeeper_irc_set_ac_state(1);
	 if(0 == getAirConditionerDat(&airInfo,g_ir_arrays->ir_arrays[0].ir_carrior_len, g_ir_arrays->ir_arrays[0].ir_carriors[0]))
	 {
		   for( int j=0;j<g_ir_arrays->ir_arrays[0].ir_carrior_len;j++)
			  g_ir_arrays->ir_arrays[0].ir_carriors[j]= g_ir_arrays->ir_arrays[0].ir_carriors[j]/IR_UNIT_US;
			   printf("irA test device=%d,	 ir_carrior_len %d\n\r",airInfo.device,g_ir_arrays->ir_arrays[g_ir_arrays->ir_array_num].ir_carrior_len); 		   
			   g_ir_arrays->ir_array_num=1;
			   send_cnt=5;
	 }
	 ir_status=IR_AC_SEND;
	 ir_emitter_start();
 
 }
 #endif
 int iremitterMsgProccess(int Msg)
 {
 
	 switch(Msg)
	 {
	  case	MSG_IR_EMITTER_TEST:	 
		 
		 printf("MSG_IR_EMITTER_TEST start");
		 
           #ifdef IR_RECEIVE_IO
            housekeeper_irc_ac_match_start();
           #endif
		 break ;
	 case  MSG_IR_RECEIVE_TEST_OK:
	 	
                #if(IR_LIB_TYPE==lib_irA)
                		 send_test_data();
                #endif
			break ;
 
		 
		}
 
 return Msg;
 }

 void  ac_off_timeout(void)
 {
	 AC_Timer_off_cnt=0;
 	housekeeper_irc_set_ac_state(0);
	   ir_emitter(ACTION_CLOSE);
 }
 void  ac_on_timeout(void)
 {
	 AC_Timer_on_cnt=0;
	 housekeeper_irc_set_ac_state(1);
	 ir_emitter(ACTION_OPEN);
 }



#endif

