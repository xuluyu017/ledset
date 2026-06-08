#ifndef __BYLE_USER_H__
#define __BYLE_USER_H__
#include "vm.h"

extern bool is_audio_on;

#define syscfg_write(hdl, data_buf, len) vm_write(hdl, data_buf, len) 
#define  syscfg_read(hdl, data_buf, len) vm_read(hdl, data_buf, len)  


#define irq_disable(x)  bit_clr_ie(x)
#define irq_enable(x)  bit_set_ie(x)


typedef struct xSTATIC_QUEUE {
    uint8_t run;
    uint8_t timeout;
} StaticQueue_t;

typedef StaticQueue_t OS_SEM;
int os_sem_create(OS_SEM *sem, int flag);
int os_sem_accept(OS_SEM *sem);
int os_sem_pend(OS_SEM *sem, int timeout);
int os_sem_post(OS_SEM *sem);
int os_sem_del(OS_SEM *sem, int block);
int os_sem_set(OS_SEM *sem, u16 cnt);

typedef struct
{
 u8  vioce_off;
  #ifdef BYLE_KWS_SWITCH
 u8  user_name_index;
 u8  power_on_led;
 #endif
  u8 data_check;

} user_global_info_t;

typedef struct user_mode_info
{
	u8 cabinet_flag;
	u8 wardrobe_flag;
	u8 shoe_cabinet_flag;
	u8 wine_cabinet_flag;
	u8 led_mode_set;

}UserMode;

 extern user_global_info_t MyContrl;
 void user_loop(void);
bool get_mic_onoff(void);
void	 set_mic_onoff(u8 onoff);
void get_mic_energy_lib_int(int  mic_off_value);
void lib_get_mic_onoff_detect(void);

bool  byle_Authorization_lib_int(u8 Authorization_type,char * useid);
void user_io_init(void);
void mic_enercy_rx_cb( s16 *data, int len);
int byle_user_app_msg(int msg);
void  user_power_on_init(void);
void app_task_put_key_msg(int msg);
int puts(const char *out);
void byle_timer_2ms(void);
int byle_get_msg(int *msg);
int byle_post_msg(int msg);
void byle_app(void);
void user_app(void); 
void byle_app_exit(void);
void set_2ms_flag(void);
int task_create(void (*task)(void *p), void *p, const char *name);
int task_kill(const char *name);
void task_run(void);
bool regist_sw_irq(void *func_proc, u8 prio);
void user_sw_interrupt_en(void);
void user_sw_interrupt_run(void);
void mic_bias_ldo( u8 on);






#define OS_ERR_NONE   0

enum {
    OS_NO_ERR = 0,
    OS_TRUE,
    OS_ERR_EVENT_TYPE,
    OS_ERR_PEND_ISR,
    OS_ERR_POST_NULL_PTR,
    OS_ERR_PEVENT_NULL,
    OS_ERR_POST_ISR,
    OS_ERR_QUERY_ISR,
    OS_ERR_INVALID_OPT,
    OS_ERR_TASK_WAITING,
    OS_ERR_PDATA_NULL,
    OS_TIMEOUT,
    OS_TIMER,
    OS_TASKQ,
    OS_TASK_NOT_EXIST,
    OS_ERR_EVENT_NAME_TOO_LONG,
    OS_ERR_FLAG_NAME_TOO_LONG,
    OS_ERR_TASK_NAME_TOO_LONG,
    OS_ERR_PNAME_NULL,
    OS_ERR_TASK_CREATE_ISR,
    OS_MBOX_FULL,
    OS_Q_FULL,
    OS_Q_EMPTY,
    OS_Q_ERR,
    OS_ERR_NO_QBUF,
    OS_PRIO_EXIST,
    OS_PRIO_ERR,
    OS_PRIO_INVALID,
    OS_SEM_OVF,
    OS_TASK_DEL_ERR,
    OS_TASK_DEL_IDLE,
    OS_TASK_DEL_ISR,
    OS_NO_MORE_TCB,
    OS_TIME_NOT_DLY,
    OS_TIME_INVALID_MINUTES,
    OS_TIME_INVALID_SECONDS,
    OS_TIME_INVALID_MILLI,
    OS_TIME_ZERO_DLY,
    OS_TASK_SUSPEND_PRIO,
    OS_TASK_SUSPEND_IDLE,
    OS_TASK_RESUME_PRIO,
    OS_TASK_NOT_SUSPENDED,
    OS_MEM_INVALID_PART,
    OS_MEM_INVALID_BLKS,
    OS_MEM_INVALID_SIZE,
    OS_MEM_NO_FREE_BLKS,
    OS_MEM_FULL,
    OS_MEM_INVALID_PBLK,
    OS_MEM_INVALID_PMEM,
    OS_MEM_INVALID_PDATA,
    OS_MEM_INVALID_ADDR,
    OS_MEM_NAME_TOO_LONG,
    OS_ERR_MEM_NO_MEM,
    OS_ERR_NOT_MUTEX_OWNER,
    OS_TASK_OPT_ERR,
    OS_ERR_DEL_ISR,
    OS_ERR_CREATE_ISR,
    OS_FLAG_INVALID_PGRP,
    OS_FLAG_ERR_WAIT_TYPE,
    OS_FLAG_ERR_NOT_RDY,
    OS_FLAG_INVALID_OPT,
    OS_FLAG_GRP_DEPLETED,
    OS_ERR_PIP_LOWER,
    OS_ERR_MSG_POOL_EMPTY,
    OS_ERR_MSG_POOL_NULL_PTR,
    OS_ERR_MSG_POOL_FULL,

};

#endif
