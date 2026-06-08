#ifndef KWS_H_
#define KWS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CAPTURE_SAMPLE_BYTE       2
#define CAPTURE_SAMPLE_RATE       16000
#define CAPTURE_SAMPLE_LEN_MS     (CAPTURE_SAMPLE_RATE / 1000)
#define CAPTURE_FRAME_MS          16
#define CAPTURE_FRAME_LEN_1CH     (CAPTURE_FRAME_MS * CAPTURE_SAMPLE_LEN_MS)
#define CAPTURE_FRAME_SIZE_1CH    (CAPTURE_FRAME_LEN_1CH * CAPTURE_SAMPLE_BYTE)

#define KWS_DECODER_BUF_SIZE (20 * 1024)

  typedef enum {
    KWS_WORK_INIT = 0,
    KWS_WORK_IDLE,
    KWS_WORK_RUNNING
  } kws_work_state;

  typedef enum {
    ENGINE_KWS_MODE_WAKEUP = 0,
    ENGINE_KWS_MODE_CMD,
    ENGINE_KWS_MODE_INVALID
  } engine_kws_mode;

  typedef struct kws_context {
    void *                    kws;
    kws_work_state            state;
    engine_kws_mode           engine_mode;
    int                       am_id;
    char                      cmd[32];
  } kws_context_t;

  typedef struct kws_result_msg {
    float         score;
    int           event_id;
    char          command[256];
  } kws_result_msg_t;

  int unisound_kws_init(void);
  int unisound_kws_deinit(void);
  int unisound_kws_mode_set(engine_kws_mode mode);
  engine_kws_mode unisound_kws_mode_get(void);
  int unisound_kws_process(void *mic, kws_result_msg_t *kws_msg);
  int unisound_kws_start(void *decoder_pool, int size);
  int unisound_kws_stop(void);

  unsigned int bkdr_hash(const char* key);

  void us325_heap_init(void *addr1, int size1, void *addr2, int size2);

#ifdef __cplusplus
}
#endif
#endif
