#include "includes.h"
#include "gpio.h"
#include "timer.h"
#include "byle_cfg.h"
#include "user_pwm.h"
#include "log.h"
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[pwm]"
#include "log.h"

 const u32 byle_TIMERx_table[3] = {
    (u32)JL_TIMER0,
    (u32)JL_TIMER1,
    (u32)JL_TIMER2,
};
#ifndef PWM_HIGH_ON
#define PWM_HIGH_ON 1
#endif

void PWM_output_channle(u32 hw_port,JL_TIMER_TypeDef *JL_TIMERx,u8 outputchannl )
{
	u8 tmr_num;
	for (tmr_num = 0; tmr_num < 3; tmr_num ++) {
		if ((u32)JL_TIMERx == byle_TIMERx_table[tmr_num]) {
			break;
		}
		if (tmr_num == 2) {
			return;
		}
	}
	gpio_set_die(hw_port, 1);
	gpio_set_pull_up(hw_port, 0);
	gpio_set_pull_down(hw_port, 0);
	gpio_set_direction(hw_port, 0);

       gpio_och_sel_output_signal(hw_port, OUTPUT_CH_SIGNAL_TIMER0_PWM + tmr_num);


}

void user_pwm_init(u32 user_hw_port,JL_TIMER_TypeDef *JL_TIMERx,u8 outputchannl,u32 freq)
	{
		u8 tmr_num;
		for (tmr_num = 0; tmr_num < 3; tmr_num ++) {
			if ((u32)JL_TIMERx == byle_TIMERx_table[tmr_num]) {
				break;
			}
			if (tmr_num == 2) {
				return;
			}
		}
		gpio_set_die(user_hw_port, 1);
		gpio_set_pull_up(user_hw_port, 0);
		gpio_set_pull_down(user_hw_port, 0);
		gpio_set_direction(user_hw_port, 0);
		gpio_och_sel_output_signal(user_hw_port, OUTPUT_CH_SIGNAL_TIMER0_PWM + tmr_num);
		//初始化timer
		JL_TIMERx->CON = 0;
		SFR(JL_TIMERx->CON, 10, 4, TIMER_SRC_STD_24M); //时钟源选择std24m
		u32 timer_clk = 24000000;
		SFR(JL_TIMERx->CON, 4, 4, TIMER_PRESCALE_2); //pset=2
		JL_TIMERx->CNT = 0; 							//清计数值
		JL_TIMERx->PRD = timer_clk / (2 * freq); 		//设置周期
		//设置初始占空比
                 
		#if defined(IR_EMITTER_PORT)&&IR_EMITTER_PWM_LOW_EN
                 if(user_hw_port==IR_EMITTER_PORT)
                 	{
                 	
                       JL_TIMERx->CON |=BIT(9);
                 	}
		  else
		#endif
			{
 		  #if PWM_HIGH_ON
                    JL_TIMERx->CON &=~BIT(9);
                     #else
                     JL_TIMERx->CON |=BIT(9);
                             #endif
 			}
		JL_TIMERx->PWM =0;	//0~10000对应0~100%
		
			log_info("JL_TIMERx->CON %d JL_TIMERx->PRD %d  JL_TIMERx->PWM %d \n",JL_TIMERx->CON,JL_TIMERx->PRD,JL_TIMERx->PWM);
	}


void PWMTimerPreInt(JL_TIMER_TypeDef *JL_TIMERx,u32 freq)
{

}


void user_set_pwm_duty(JL_TIMER_TypeDef * JL_TIMERx,u8 duty)
{
   u32 duty1;
   JL_TIMERx->PWM = (JL_TIMERx->PRD * duty) / PWM_MAX_DUTY_VALUE; ///0~10000对应0~100%
JL_TIMERx->CON |= BIT(8) | (0b01 << 0); 		//计数模式 

	
	//log_info("PWM %d \n",JL_TIMERx->PWM);
}
void pwm_io_close(u32 user_hw_port,u8 high_low)
{

	
       #ifdef	IR_EMITTER_PORT_USE_MIC_BIAS
                 if(user_hw_port==IR_EMITTER_PORT)
                 	{
			 gpio_set_die(user_hw_port, 1);
                        gpio_set_direction(user_hw_port,1);
                        gpio_set_pull_up(user_hw_port,0);
                        gpio_set_pull_down(user_hw_port,0); 
		      printf("IR_EMITTER_PORTwwwwwwwwwww\n",user_hw_port);

                 	}
		else
          #endif				
			{
           	 gpio_set_die(user_hw_port,0);
           	 gpio_set_direction(user_hw_port,0);
           	gpio_set_pull_up(user_hw_port,0);
           	gpio_set_pull_down(user_hw_port,0);	
                    gpio_write(user_hw_port,high_low);

			}
}		  

void PWM_dis_output_channle(u32 hw_port,JL_TIMER_TypeDef *JL_TIMERx)
{
	u8 tmr_num;
	for (tmr_num = 0; tmr_num < 3; tmr_num ++) {
		if ((u32)JL_TIMERx == byle_TIMERx_table[tmr_num]) {
			break;
		}
		if (tmr_num == 2) {
			return;
		}
	}

	   printf("PWM_dis_output_channle\n",hw_port);
	   JL_TIMERx->CON |= BIT(14);	   //清pend
	   JL_TIMERx->CON &= ~BIT(8);	   //清输出是能
	   JL_TIMERx->CON &= ~BIT(1);	   //关闭定时器
	   JL_TIMERx->CON &= ~BIT(0);

	   gpio_och_disable_output_signal(hw_port, OUTPUT_CH_SIGNAL_TIMER0_PWM + tmr_num);

          #if PWM_HIGH_ON
	   pwm_io_close(hw_port,0);	  
	  #else
	  pwm_io_close(hw_port,1);	 
	  #endif
	  

}

void user_mc_pwm_int(pwm_ch_num_type pwm_ch_num,u32 freq)
{
    struct pwm_platform_data pwm_p_data;
    pwm_p_data.pwm_aligned_mode = pwm_edge_aligned;         //边沿对齐
    pwm_p_data.pwm_ch_num = pwm_ch_num;                        //通道号
    pwm_p_data.frequency = freq;                            //1KHz
    pwm_p_data.duty = 0;                                 //占空比50%
    pwm_p_data.h_pin = -1;                         //任意引脚
    pwm_p_data.l_pin = -1;                         //任意引脚,不需要就填-1
    pwm_p_data.complementary_en = 0;                        //两个引脚的波形, 0: 同步,  1: 互补，互补波形的占空比体现在H引脚上
    mcpwm_init(&pwm_p_data);


}
void user_set_mcpwm_duty(pwm_ch_num_type pwm_ch_num,u8 duty)
{
	
	//log_debug("pwm_ch= %d ",duty);
	mcpwm_set_duty(pwm_ch_num,duty*10000/255);


}

