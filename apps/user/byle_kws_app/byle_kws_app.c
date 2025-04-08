//


#include "audio_adc.h"
#include "common.h"
#include "hwi.h"
#include "clock.h"
/*#include "gpio.h"*/
#include "sound_mge.h"
#include "circular_buf.h"
#include "audio_adc_api.h"

#include "byle_cfg.h"
#include "byle_user.h"
#include "timer_app.h"

#include "byle_user.h"
#include "timer_app.h"
#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[normal]"
#include "log.h"

#include "byle_kws_app.h"
#include "byle_msg.h"
#include "music_led_code_less.h"
#ifdef AC_zero_port
#include "ac_zero_det_led.h"
#endif
#ifdef IR_EMITTER_PORT
#include "ir_emitter.h"
#endif
#include "decode_mge.h"

#ifdef BYLE_KWS_ASR

extern byle_kws_ctl  gByle_kws_ctl;
#if(KWS_TYPE==kwsC)
#define	MIC_DATA_SIZE	320


static u8 voice_ctl_private_heap[21820];
static u8 voice_ctl_share_heap[9352];

#define	 aas_obuf_size  1024*2
u8 obuf_aas_obuf[aas_obuf_size] ;

#endif

#if(KWS_TYPE==kwsC1)
#define	MIC_DATA_SIZE	320
#define private_heap_size  22056
#define share_heap_size  9800
#define ps_heap_size  8192//-1024*2

static u8 private_heap[private_heap_size];
static u8 share_heap[share_heap_size];
static u8 ps_heap[ps_heap_size]  AT(.usr_data);
#define	 aas_obuf_size  MIC_DATA_SIZE*4
u8 obuf_aas_obuf[aas_obuf_size] ;
#endif

#if(KWS_TYPE==kwsA)
#ifndef  new_kws_en
#define  new_kws_en 1
#endif

#if new_kws_en
#define kws_buffer_size (10*1024)//(5*1024+512)
#define heap_buffer_size2 (19*1024) //(24*1024+512)
#define heap_buffer_size1 (5*1024)//固定5k
u8 kws_buffer[kws_buffer_size] ;	
u8 heap_buffer[heap_buffer_size1] AT(.usr_data)  ;
#else
#define kws_buffer_size (5*1024)//(6*1024+512)
#define heap_buffer_size2 0//(24*1024+512)
#define heap_buffer_size1 0//固定5k
u8 kws_buffer[kws_buffer_size] AT(.usr_data) ;	
u8 heap_buffer[heap_buffer_size1] AT(.usr_data)  ;
#endif
#define	 aas_obuf_size  (1024+512+1024)
#define	MIC_DATA_SIZE	512
u8 obuf_aas_obuf[aas_obuf_size]  AT(.usr_data)  ALIGNED(4);

#endif


#if(KWS_TYPE==kwsD)
#define	MIC_DATA_SIZE	128
#define    kws_buffer_size 11872
#define	 aas_obuf_size  (1024+512)
 u8 kws_buffer[kws_buffer_size];  
  const u8 kw_offset[] = byle_kw_offset; 
  const u8 bigram_list[] =byle_bigram_list;
  const u8 NUM_BIGRAMS = 0;
  const u8 KW_TRIGGER_SCORE=byle_KW_TRIGGER_SCORE;
  const u8 KW_OUT_SCORE=byle_KW_OUT_SCORE;
  const uint16_t WAKE_STEPS=byle_WAKE_STEPS;
  const u8 WAKE_SCORE=byle_WAKE_SCORE; //Seems that now score distribution is similar to before, higher than -50 is very rare
  const u8 USE_KW_PRE_SUPPRESS=byle_USE_KW_PRE_SUPPRESS;
  const u8 KW_PRE_SUPPRESS_MAX_STEPS=byle_KW_PRE_SUPPRESS_MAX_STEPS; //max steps before a kw that will suppress the kw
#ifdef byle_PREFER_LONG_WD
const uint8_t  PREFER_LONG_WD=byle_PREFER_LONG_WD;
#endif
u8 obuf_aas_obuf[aas_obuf_size]  AT(.usr_data) ;

#endif

cbuffer_t cbuf_aas_obuf ;

void *get_mic_buf_addr(void)
{
          return obuf_aas_obuf;
}

void *get_kws_share_addr(void)
{
          return heap_buffer;
}

typedef struct __audio_adc_speaker {
    sound_out_obj  sound;
} __voice_speaker;

__voice_speaker voice_speaker;

void voice_control_kick_sound(void *_sound)
{
    sound_out_obj *sound = (sound_out_obj *)_sound;
    if (sound && sound->p_obuf)
    {
        u32 size = cbuf_get_data_size(sound->p_obuf);
    
                if (size >= MIC_DATA_SIZE)
                {
            
                    sound->enable |= B_DEC_KICK;
					
                 
                }
	
    }
}
u8 mic_data_buf[MIC_DATA_SIZE] __attribute__((aligned(4)));
void mic_read(void)
{

    sound_out_obj *sound = &voice_speaker.sound;
    if (NULL == sound) {
        return;
    }

if (0 == (sound->enable & B_DEC_RUN_EN)) {
	return;
}
/*每10ms输送320个byte给算法识别*/
if (sound->enable & B_DEC_KICK) {
	sound->enable &= ~B_DEC_KICK;

 u16 olen = 0;
 olen = cbuf_read(sound->p_obuf, (void *)&mic_data_buf[0], MIC_DATA_SIZE);
 
 if (olen != MIC_DATA_SIZE) {
 return;
 }


byle_kws_audio_rx_cb((s16*)mic_data_buf,MIC_DATA_SIZE);  

}
}


void kws_adc_init(void)
{

	    //存储mic数据的内存初始化
    memset(&voice_speaker, 0, sizeof(voice_speaker));
    cbuf_init(&cbuf_aas_obuf, &obuf_aas_obuf[0], sizeof(obuf_aas_obuf));
    voice_speaker.sound.p_obuf = &cbuf_aas_obuf;
    regist_audio_adc_channel(&voice_speaker.sound, (void *)voice_control_kick_sound);
}



int user_handle_action( int index)
{
u16 msg=KEY_NULL;


#ifdef BYLE_kws_music_led
 msg=kws_led_handle_action(index);
#elif defined(BYLE_KWS_SWITCH)           
byle_switch_handle_action(index);

#else
if(Byle_kws_msg_table[index]!=KEY_NULL)
 	{

	printf("appmsg=%d\n",index);
          app_task_put_key_msg(Byle_kws_msg_table[index]);
          msg=Byle_kws_msg_table[index];
 	}
#endif

#ifdef BYLE_UATR_ENABLE
#ifdef BYLE_UART_SEND_USER_CMD_TABLE
byle_uart_tx(&cmd_table[index][0],BYLE_UART_SEND_USER_CMD_TABLE);
printf_buf(&cmd_table[index][0],BYLE_UART_SEND_USER_CMD_TABLE);
#endif
#endif

#ifdef IR_EMITTER_PORT
index= ir_emitter_handle_action(index);
#endif

return index;

}

#if(KWS_TYPE==kwsA)

static unsigned int byle_bkdr_hash(const char* key) {
  unsigned int seed = 31; // 31 131 1313 13131 131313 etc..
  unsigned int hash = 0;
  unsigned char *cp = (unsigned char *)key;
  if (NULL == cp) {
    return 0;
  }
  while (*cp) {
    hash = (hash * seed + (*cp++)) & 0xFFFFFFFF;
  }
  return hash;
}

static int user_map_command(const char *command)
{
 u8 cmd_data=0;
  unsigned int hashCode = byle_bkdr_hash(command);

int hashTableSize = sizeof(g_nlu_content_mapping) / sizeof(g_nlu_content_mapping[0]);
int index=-1;
int i;

for (i = 0; i < hashTableSize; i++) {

  if (hashCode == g_nlu_content_mapping[i].key_word_hash_code
         #ifdef RE_DEFINE_MAPPING
         ||hashCode ==RE_DEFINE_MAPPING
         #endif
)

    {
	index = g_nlu_content_mapping[i].nlu_content_str_index;
        #ifdef RE_DEFINE_MAPPING
         if(hashCode==RE_DEFINE_MAPPING)
        index =RE_DEFINE_MAPPING_index;
        #endif

	  printf("map_command_index=%d\n",index);
	 
      return index;
    }
  }
  return -1;
}
#endif

void byle_kws_app_pause(u8 pause)
{
printf( " cur pause=%d", pause);

if(pause==0)
   {
	set_audio_adc_run(1);

   }
else
   {

	 set_audio_adc_run(0);

   }
cbuf_clear(&cbuf_aas_obuf);

//printf( " pause gByle_kws_ctl.state=%d", gByle_kws_ctl.state);

}





kws_result_msg_t kws_msg = {0};

void byle_kws_task(void)
{
	int ret,i;
	int is_play = 0;

      #if  KWS_TONE_EN
	  if(gByle_kws_ctl.state!= KWS_WORK_RUNNING)
	return ;  	
      #endif

	
        mic_read();
       ret =byle_kws_process(&kws_msg);
        if (ret == 0)
      	{
      	

	 #if (KWS_TYPE==kwsA)
	 
	 kws_msg.command_index=user_map_command(byle_kws_get_cmd());
	     printf("score=%d.%02d\n", (int)kws_msg.score, ((int)(kws_msg.score*100))%100);
		 
		 
	   printf(" kws cmd %s  index=%d", byle_kws_get_cmd(),kws_msg.command_index);

         
	   
           #ifdef EN_SCORE_THRED_FOR_EVERY_CMD
		   
	 printf("thred=%d.%02d\n", (int)g_nlu_score_thred[kws_msg.command_index], ((int)(g_nlu_score_thred[kws_msg.command_index]*100))%100);
           if(kws_msg.score>=g_nlu_score_thred[kws_msg.command_index])
           #else
	   printf("thred=%d.%02d\n", (int)KWS_CMD_SCORE_THRED, ((int)(KWS_CMD_SCORE_THRED*100))%100);
           if ((gByle_kws_ctl.engine_mode==ENGINE_KWS_MODE_WAKEUP&&kws_msg.score >= KWS_WAKEUP_SCORE_THRED)||
           (gByle_kws_ctl.engine_mode==ENGINE_KWS_MODE_CMD&&kws_msg.score >= KWS_CMD_SCORE_THRED))
           #endif
           	{
	  #endif

		
                     #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
		 
                              if(gByle_kws_ctl.engine_mode== ENGINE_KWS_MODE_WAKEUP)
                              	{
                              	
                                   #ifdef wakeUp_cmd
                              	for(i=0;i<wakeUp_cmd;i++)
                              	{
                              	if(kws_msg.command_index==wakeUp_cmd_table[i])
                              	{
                              	printf(" on wake up cmd %d ", kws_msg.command_index);
                                   byle_kws_timer_out_set(BYLE_ASR_TIMEOUT);       
                              	break;
                              	}
                              	}
                              	if(i>=wakeUp_cmd)
                              	{
                              	
                              	printf(" no wake up cmd %d ", kws_msg.command_index);
                              	return ;
                              	}
                                     #endif


                                        #if KWS_TONE_EN==0
        				byle_kws_user_stop(KWS_WORK_IDLE);
        				 printf("wake up cmd\r\n");
        				 gByle_kws_ctl.engine_mode=ENGINE_KWS_MODE_CMD; 
                      			  byle_kws_user_start(gByle_kws_ctl.engine_mode);	
        				  #endif
						  
                              	}

							  
                              #if  KWS_TONE_EN
                              byle_kws_app_pause(1);
                              byle_kws_user_stop(KWS_WORK_IDLE); // 释放 kws RAM资源,播放提示音
			       gByle_kws_ctl.engine_mode=ENGINE_KWS_MODE_CMD; 
                              if(tone_play_start(tone_table[kws_msg.command_index])!=0)
                              {
                              is_play=1;
                              byle_kws_user_start(gByle_kws_ctl.engine_mode);   
                              }
                              #endif


							  
			   byle_kws_timer_out_set(BYLE_ASR_TIMEOUT);	 
                     #endif

                  #if (KWS_TYPE==kwsA)
                  
                  	   
                  	   user_handle_action(kws_msg.command_index);
                       }
                  #endif

           
				  
               #if (KWS_TYPE==kwsC||KWS_TYPE==kwsC1)
                  printf("\r\n Cmd%d\r\n (%u)\r\n", kws_msg.command_index,kws_msg.score);
               user_handle_action(kws_msg.command_index);
               #endif



			   

	}

}


  void set_kws_timeout(void)
 {
	 u8 data_rsp_buffer[2];
	 u8 cmd_data=0;
			int ret = 0;
		 printf("kws timeout");
          #ifdef BYLE_UATR_ENABLE
          #ifdef SEND_USER_CMD_EXIT
	  byle_uart_tx(&cmd_table[SEND_USER_CMD_EXIT][0],BYLE_UART_SEND_USER_CMD_TABLE);
	 printf_buf(&cmd_table[SEND_USER_CMD_EXIT][0],BYLE_UART_SEND_USER_CMD_TABLE);
          #endif
          #endif
		  
        #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
	byle_kws_user_stop(KWS_WORK_IDLE);
	gByle_kws_ctl.engine_mode=ENGINE_KWS_MODE_WAKEUP; 
	byle_kws_user_start(gByle_kws_ctl.engine_mode); 		 
         #endif
 }

 void byle_kws_timer_out_set(u8 time_out)
{
gByle_kws_ctl.kws_asr_time_out_cnt=time_out;
printf( "kws_asr_time_out set :%d", time_out);

}
void kws_1000ms_timer(void)
{

 

         if(gByle_kws_ctl.kws_asr_time_out_cnt)
         {
         
		 printf("gByle_kws_ctl.kws_asr_time_out_cnt=%d\n",gByle_kws_ctl.kws_asr_time_out_cnt);
		 if(--gByle_kws_ctl.kws_asr_time_out_cnt==0)
		 	{
		 	
			 set_kws_timeout();
		 	}
           
         }




}


#if 0//defined(Byle_Kws_Lib_Type)&&(Byle_Kws_Lib_Type==KWSA_SDK_lib)
#include "grammar.h"

 const char *get_grammar_addr(void)
 {
     return global_kws_lp_grammar;
 }

#endif

void byle_kws_app_exit(void)
{
 printf("byle_kws_app_exit...!\r\n");
unregist_audio_adc_channel(&voice_speaker.sound);
byle_kws_user_stop(KWS_WORK_INIT);
task_kill("byle_asr");

}

#if(KWS_TYPE==kwsC1)
int *get_key_word_idx(void)
{
/*int keyword_idx[KEY_WORDS_NUM]={0,1,2,3,4,5,6,7,8,9,10,11};*/
u8 max_idx = keyword_idx[0];
u8 has_duplicate = 0;
for (u8 i = 1; i < KEY_WORDS_NUM; i++) {
	if (max_idx < keyword_idx[i]) {
		max_idx = keyword_idx[i];
	} else if (max_idx == keyword_idx[i]) {
		has_duplicate = 1;
	}
}
 if (has_duplicate || max_idx != keyword_idx[KEY_WORDS_NUM - 1]) {
 	log_error("keyword idx input error");
 	return NULL;
 }

 
 return keyword_idx;
}
#endif

void byle_kws_app(void)
{
    u32 err;
    int msg[2];
    //adc enable
    kws_adc_init();
    printf("kws app run start ...!\r\n");
   gByle_kws_ctl.state = KWS_WORK_INIT;
  gByle_kws_ctl.engine_mode=ENGINE_KWS_MODE_CMD;	

  
  local_irq_disable();
#if(KWS_TYPE==kwsC)
 err=byle_kws_init(voice_ctl_private_heap, voice_ctl_share_heap, confidence);
#endif

#if(KWS_TYPE==kwsC1)
 err=byle_kws_init(1,private_heap,private_heap_size, share_heap, share_heap_size,ps_heap,ps_heap_size,confidence,get_key_word_idx(),KEY_WORDS_NUM);
#endif

#if(KWS_TYPE==kwsA)
  printf("  int heap_buffer_size=%d   kws_buffer_size=%d  \r\n",heap_buffer_size1+heap_buffer_size2,kws_buffer_size);
#if heap_buffer_size2
byle_kws_set_heap_size(heap_buffer_size2);
#endif
err=byle_kws_init(kws_buffer,kws_buffer_size,heap_buffer,heap_buffer_size1);

#endif

#if(KWS_TYPE==kwsD)
	 err=byle_kws_init(kws_buffer,kws_buffer_size);
#endif
  local_irq_enable();

  if(err==errKWS_NoError)
  	{
	  printf("kbyle_kws_init ok!\r\n");
  	}
  else
    printf("kbyle_kws_init error =%d!\r\n",err);


  #if(KWS_CMD_MODE==ENGINE_KWS_MODE_WAKEUP)
if(gByle_kws_ctl.engine_mode==ENGINE_KWS_MODE_CMD)
 byle_kws_timer_out_set(BYLE_ASR_TIMEOUT);
sys_hi_timer_add(NULL, kws_1000ms_timer, 1000);

#endif


voice_speaker.sound.enable |= B_DEC_RUN_EN | B_DEC_FIRST;

err=task_create(byle_kws_task, NULL, "byle_asr");
if (err != OS_NO_ERR) {
	printf("%s creat fail %x\n", __FUNCTION__,	err);
}
printf("  byle_kws_app start ...........\r\n");

//sys_s_hi_timerout_add(NULL, byle_kws_app_exit, 10000); //test

}


void kws_tone_end_ctrl(void)
{

	   printf("kws_tone_end_ctrl\n");
	   byle_kws_user_start(gByle_kws_ctl.engine_mode);	 
	   byle_kws_app_pause(0);


}

int byle_kws_app_msg(int msg)
{



  switch (msg) {
case MSG_TONE_PLAY_END:
	kws_tone_end_ctrl();

break;

  case MSG_BYLE_KWS_ON_OFF:

	break;

    case MSG_BYLE_KWS_START:
		byle_kws_user_start(gByle_kws_ctl.engine_mode);   
		byle_kws_app_pause(0);
      break;
    case MSG_BYLE_KWS_STOP:
		byle_kws_app_pause(1);
		byle_kws_user_stop(KWS_WORK_INIT); 
      break;
    default:
      break;
  }
  return 0;
}

#endif
