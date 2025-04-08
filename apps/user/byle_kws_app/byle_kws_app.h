#ifndef __BYLE_KWS_H__
#define __BYLE_KWS_H__
#include "byle_msg.h"

#ifdef BYLE_KWS_ASR
#ifdef BYLE_KWS_SWITCH
#include "byle_kws_switch_io.h"
#endif
#if (Byle_Kws_Lib_Type==KwsA_FAN_LIGHT_F6_180K||Byle_Kws_Lib_Type==KwsA_FAN_LIGHT_F6_100K)
#include "byle_kwsA_FAN_LIGHT_F6_lib.h"
#include "byle_kwsA_nor.h"
#endif

#if (Byle_Kws_Lib_Type==kwsA_wy_light_libC2)
#include "byle_kwsA_wy_light_libC2.h"
#include "byle_kwsA_nor.h"
#endif

#if (Byle_Kws_Lib_Type==KWSC_light_libD1)
#include "byle_kwsC_light_libD1.h"
#endif

#if (Byle_Kws_Lib_Type==KWSC_light_libC1)
#include "byle_kwsC_light_libC1.h"
#endif
#if (Byle_Kws_Lib_Type==KWSC_light_libE1)
#include "byle_kwsC_light_libE1.h"
#endif
#if (Byle_Kws_Lib_Type==KWSC1_light_libF1)
#include "byle_kwsC1_light_libF1.h"
#endif

#if (Byle_Kws_Lib_Type==KWSA_light_libK4)
#include "byle_kwsA_light_libK4.h"
#endif

#if (Byle_Kws_Lib_Type==KWSA_light_libC1)
#include "byle_kwsA_light_libC1.h"
#endif

#if (Byle_Kws_Lib_Type==KWSA_light_libD1)
#include "byle_kwsA_light_libD1.h"
#endif

#if (Byle_Kws_Lib_Type==KWSA_ac_libK3)         
#include "byle_kwsA_Air_Conditioner_K3_lib.h"       
#endif       

#if (Byle_Kws_Lib_Type==KWSA_ac_libK2)        
#include "byle_kwsA_Air_Conditioner_K2_lib.h"      
#endif       

#if (Byle_Kws_Lib_Type==KWSA_ac_libK1)
#include "byle_kwsA_Air_Conditioner_K1_lib.h"
#endif


#if (Byle_Kws_Lib_Type==KWSA_switch_libS1)         
#include "byle_kwsA_switch_libS1.h"
#endif

#if (Byle_Kws_Lib_Type==KWSC_switch_libC1)         
#include "byle_kwsC_switch_libS1.h"
#endif


#if (Byle_Kws_Lib_Type==KWSA_SDK_lib)         
#include "byle_kwsA_SDK_lib.h"
#include "byle_kwsA_nor.h"

#endif



#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum __KWS_STATUS
{
	errKWS_NoError = 0,
	errKWS_NotReady,
	errKWS_InvalidParam,
	errKWS_InvalidLicense,
	errKWS_BufferOverflow
}
KWS_STATUS;


typedef enum {
  KWS_WORK_INIT = 0,
  KWS_WORK_IDLE,
  KWS_WORK_RUNNING
} kws_work_state;

typedef enum {
  ENGINE_MODE_WAKEUP = 0,
  ENGINE_MODE_CMD,
  ENGINE_MODE_INVALID
} engine_kws_mode;

typedef struct {
u16 kws_asr_time_out_cnt;
kws_work_state			state;
engine_kws_mode			engine_mode;
} byle_kws_ctl;


void byle_kws_app(void);

#if(KWS_TYPE==kwsA)
const char * byle_kws_get_cmd(void);

typedef struct kws_result_msg {
  float 		score;
  int			command_index;
} kws_result_msg_t;
#else

typedef struct kws_result_msg {
  int16_t 		score;
  int			command_index;
} kws_result_msg_t;
#endif

void byle_kws_audio_rx_cb(s16 *data, int len);
void byle_kws_task(void);
int byle_kws_mode_set(engine_kws_mode mode);
int byle_kws_user_stop(kws_work_state  stop_work_state);
int byle_kws_user_start(engine_kws_mode  engine_mode);
void byle_kws_app_pause(u8 pause);
void *get_mic_buf_addr(void);
void *get_kws_share_addr(void);
void kws_tone_end_ctrl(void);
int byle_kws_app_msg(int msg);


#if(KWS_TYPE==kwsC1)
int  byle_kws_init(int model, char *private_heap, int private_heap_size, char *share_heap,int share_heap_size , char *ps_heap, int ps_heap_size,float *confidence,int *keyword_idx, u8 keyword_num );
#endif
#if(KWS_TYPE==kwsC)
int  byle_kws_init(char *private_heap, char *share_heap, float *confidence );
#endif

#if(KWS_TYPE==kwsA)
void byle_kwsA_audio_rx_cb(s16 *data, int len);
bool  get_Authorization_check(void);
int  byle_kws_init(u8 *dec_buffer, int dec_buffer_size,u8 *heap, int  heap_size);
int  byle_kws_set_heap_size( int  heap_size);


#endif

#if(KWS_TYPE==kwsD)
int8_t byle_kws_init(int8_t*kws_buffer, int32_t kws_buffer_size);
#endif


int  byle_kws_process(kws_result_msg_t *kws_msg);
 void byle_kws_timer_out_set(u8 time_out);   //BZ
#ifdef __cplusplus
}
#endif
#endif

#endif

