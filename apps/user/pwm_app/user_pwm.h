#ifndef __USER_PWM_H__
#define __USER_PWM_H__

#include "mcpwm.h"


#define   USE_OUTPUT_CHANNEL_0     1   ///使用output channel 0
#define   USE_OUTPUT_CHANNEL_1     2   ///使用output channel 1
#define   USE_OUTPUT_CHANNEL_2     3   ///使用output channel 2

#define  PWM_MAX_DUTY_VALUE      255       ///pwm占空比最大值10000，不用改
#define  PWM_SYS_OSC_HZ          24000000    ///PWM的晶振源选择24M
void PWM_output_channle(u32 hw_port,JL_TIMER_TypeDef *JL_TIMERx,u8 outputchannl );
void PWM_dis_output_channle(u32 hw_port,JL_TIMER_TypeDef *JL_TIMERx);
void user_pwm_init(u32 user_hw_port,JL_TIMER_TypeDef *JL_TIMERx,u8 outputchannl,u32 freq);
void PWMTimerPreInt(JL_TIMER_TypeDef *JL_TIMERx,u32 freq);
void user_set_pwm_duty(JL_TIMER_TypeDef * JL_TIMERx,u8 duty);
void pwm_io_close(u32 user_hw_port,u8 high_low);
void user_mc_pwm_int(pwm_ch_num_type pwm_ch_num,u32 freq);
void user_set_mcpwm_duty(pwm_ch_num_type pwm_ch_num,u8 duty);

#endif
