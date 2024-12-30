
#include "msg.h"
#include "log.h"
#include "byle_cfg.h"
#include "timer_app.h"
#include "hwi.h"
#include "byle_msg.h"
#include "byle_user.h"

#ifdef BYLE_TIMER_APP
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[timer]"
#include "log.h"



static app_timer_t app_timer_vct[COM_APP_TIMER_MAX];
#define TIMER_CNT   2 //MS
static u32 sys_time_count = 0;

void app_timer_scan(u8 one_time_ms)
{

sys_time_count += one_time_ms;
handle_app_timers(app_timer_vct,COM_APP_TIMER_MAX);

}

void set_timer_func_table(timer_proc func_proc)
{
regist_sw_irq(func_proc,3);
}
void set_timer_func_proc(void)
{
//timer_app_func_proc(); 在timer 中断执行 ，要求函数运算少，优先级高
//regist_sw_irq(timer_app_func_proc,3);
user_sw_interrupt_en();


}




u16 setlight_timer=0;
u16 setlight_timer1=0;
u16 setlight_timer2=0;

void timer_5000ms_test(void)
{
log_info("5ss=%d",setlight_timer);
    if(setlight_timer)
      {
      usr_timeout_del(setlight_timer);
      }
}
void timer_1000ms_test(void)
{

log_info("1ss");
}

void timer_10ms_test(void)
{
 static u8 cnt ;
 cnt++;

 if(cnt%100==0)
log_info("5ms");
}


void timer_3000ms_test(void)
{

log_info("3ss timeout ");
}


void usr_timer_schedule(void )
{
	log_info("usr_timer_schedule int \n");
	setlight_timer=sys_hi_timer_add(NULL, timer_5000ms_test, 1000*5);

	log_info("setlight_timer=%d",setlight_timer);
	setlight_timer1=sys_s_hi_timer_add(NULL, timer_1000ms_test, 1000);
	log_info("setlight_timer1=%d",setlight_timer1);

        setlight_timer2=sys_hi_timeout_add(NULL, timer_3000ms_test, 3000);
        log_info("setlight_timer2=%d",setlight_timer2);


}

void timer_app_init(void)
{
	log_info("sw_timer_init   \n");

	init_app_timers(app_timer_vct,COM_APP_TIMER_MAX);
     //  usr_timer_schedule();


}

app_timer_t *get_timer(u16 timerid)
{

	if(timerid >= COM_APP_TIMER_MAX)
	return NULL;

	return &app_timer_vct[timerid];

}
uint32_t hal_sys_timer_get(void)
{
	return sys_time_count;
}

#endif

