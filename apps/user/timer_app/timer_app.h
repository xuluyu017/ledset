/********************************************************************************
 *
 *                            Module: softtimer
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lushb    2017-08-17 17:13     1.0               build this file
 ********************************************************************************/
#ifndef __softtimer_h__
#define __softtimer_h__
#include "typedef.h"

typedef enum
{
    TIMER_TYPE_NORMAL,//周期定时器
    TIMER_TYPE_SINGLE_SHOT,//单发定时器
} timer_type_e;

typedef enum
{
    TIMER_STATE_NOUSED,//没有使用
    TIMER_STATE_RUNNING,//正在运行
    TIMER_STATE_STOPED,//停止

} timer_state_e;

/* 软定时器服务例程 */
typedef void (*timer_proc)(void);


//typedef void *timer_proc;

//typedef timer_proc   void *;

/*  软定时器数据结构体 */
typedef struct
{
    timer_state_e state;
    timer_type_e type;
    uint32_t timeout;          //软定时器 定时周期
    uint32_t timeout_expires;  //软定时器 下一次超时绝对时间点
    timer_proc func_proc;    //超时服务例程
    u8 priority;
    //void *data;
} app_timer_t;

extern uint32_t g_hal_1ms_count;
/* 最大软定时器数目*/
#ifndef COM_APP_TIMER_MAX
#define COM_APP_TIMER_MAX  5
#endif
void handle_app_timers(app_timer_t *timers, uint8_t count);
s8 init_app_timers(app_timer_t *timers, uint8_t count);
app_timer_t *get_timer(u16 timerid);
void timer_app_init(void);
void timer_app_func_proc(void);
void app_timer_scan(u8 one_time_ms);
uint32_t hal_sys_timer_get(void);
s8 restart_app_timer(s8 timer_id);

void user_timestamp_run(void);
timer_state_e get_app_timer_sta(s8 timerid);
uint32_t get_app_timer_expires(s8 timerid);




/*-----------------------------------------------------------*/

/*
 *  Usr Timer
 */

void usr_timer_schedule();

//*----------------------------------------------------------------------------*/
/**@brief   usr_timer定时扫描增加接口
   @param
			priv:私有参数
			func:定时扫描回调函数
			msec:定时时间， 单位：毫秒
			priority:优先级,范围：0/1
   @return  定时器分配的id号
   @note    1、usr_timer的参数priority（优先级）为1，使用该类定时器，系统无法进入低功耗
    		2、usr_timer的参数priority（优先级）为0，使用该类定时器，系统低功耗会忽略该节拍，节拍不会丢失，但是周期会变
			3、usr_timer属于异步接口， add的时候注册的扫描函数将在硬件定时器中时基到时候被调用。
			4、对应释放接口usr_timer_del
*/
/*----------------------------------------------------------------------------*/
u16 usr_timer_add(void *priv, void (*func)(void *priv), u32 msec, u8 priority);
//*----------------------------------------------------------------------------*/
/**@brief   usr_timer超时增加接口
   @param
			priv:私有参数
			func:超时回调函数
			msec:定时时间， 单位：毫秒
			priority:优先级,范围：0/1
   @return  定时器分配的id号
   @note    1、usr_timerout的参数priority（优先级）为1，使用该类定时器，系统无法进入低功耗
    		2、usr_timerout的参数priority（优先级）为0，使用该类定时器，系统低功耗会忽略该节拍，节拍不会丢失，但是周期会变
			3、usr_timerout属于异步接口， add的时候注册的扫描函数将在硬件定时器中时基到时候被调用。
			4、对应释放接口usr_timerout_del
			4、timeout回调只会被执行一次
*/
/*----------------------------------------------------------------------------*/
u16 usr_timeout_add(void *priv, void (*func)(void *priv), u32 msec, u8 priority);

//*----------------------------------------------------------------------------*/
/**@brief   usr_timer修改定时扫描时间接口
   @param
			id:usr_timer_add时分配的id号
			msec:定时时间， 单位：毫秒
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
int usr_timer_modify(u16 id, u32 msec);
//*----------------------------------------------------------------------------*/
/**@brief   usr_timerout修改超时时间接口
   @param
			id:usr_timerout_add时分配的id号
			msec:定时时间， 单位：毫秒
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
int usr_timeout_modify(u16 id, u32 msec);
//*----------------------------------------------------------------------------*/
/**@brief   usr_timer删除接口
   @param
			id:usr_timer_add时分配的id号
   @return
   @note    注意与usr_timer_add成对使用
*/
/*----------------------------------------------------------------------------*/
void usr_timer_del(u16 id);
//*----------------------------------------------------------------------------*/
/**@brief   usr_timeout删除接口
   @param
			id:usr_timerout_add时分配的id号
   @return
   @note    注意与usr_timerout_add成对使用
*/
/*----------------------------------------------------------------------------*/
void usr_timeout_del(u16 id);
void sw_timer_action_msg( int msg);

//*----------------------------------------------------------------------------*/
/**@brief   usr_time输出调试信息
   @param

   @return
   @note    1.调试时可用
   			2.将输出所有被add定时器的id及其时间(msec)
*/
/*----------------------------------------------------------------------------*/
void usr_timer_dump(void);
/*-----------------------------------------------------------*/






#define sys_timer_add(a, b, c)\
    usr_timer_add(a, b, c, 0)

#define sys_timeout_add(a, b, c)\
    usr_timeout_add(a, b, c, 0)


#define sys_timer_del(a)\
    usr_timer_del(a)
    
#define sys_timeout_del(a)\
    usr_timer_del(a)

#define sys_hi_timer_schedule()\
    usr_timer_schedule()

#define sys_hi_timer_add(a, b, c)\
    usr_timer_add(a, b, c, 1)

#define sys_hi_timeout_add(a, b, c)\
    usr_timeout_add(a, b, c, 1)

#define sys_hi_timer_modify(a, b)\
    usr_timer_modify(a, b)

#define sys_hi_timeout_modify(a, b)\
    usr_timeout_modify(a, b)

#define sys_hi_timer_del(a)\
    usr_timer_del(a)

#define sys_hi_timeout_del(a)\
    usr_timeout_del(a)

#define sys_s_hi_timer_add(a, b, c)\
    usr_timer_add(a, b, c, 0)

#define sys_s_hi_timerout_add(a, b, c)\
    usr_timeout_add(a, b, c, 0)

#define sys_s_hi_timer_modify(a, b)\
    usr_timer_modify(a, b)

#define sys_s_hi_timeout_modify(a, b)\
    usr_timeout_modify(a, b)

#define sys_s_hi_timer_del(a)\
    usr_timer_del(a)

#define sys_s_hi_timeout_del(a)\
    usr_timeout_del(a)
    

#endif
