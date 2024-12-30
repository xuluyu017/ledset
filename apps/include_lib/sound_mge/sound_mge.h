#ifndef __SOUND_EFFECT_API_H__
#define __SOUND_EFFECT_API_H__
#include "typedef.h"



//sound.enable bits list
#define B_DEC_RUN_EN     BIT(0)
#define B_DEC_OBUF_EN    BIT(1)
#define B_DEC_EFFECT     BIT(2)
#define B_DEC_ERR        BIT(3)

#define B_DEC_PAUSE      BIT(7)
#define B_DEC_KICK       BIT(8)

#define B_REC_RUN        BIT(9)
#define B_DEC_FIRST      BIT(10)

typedef struct _sound_in_obj {
    void *p_dbuf;
    void *ops;
} sound_in_obj;

typedef struct _sound_out_obj {
    void *p_obuf;
    void *effect;
    void (*kick)(void);
    volatile u32 enable;
} sound_out_obj;

typedef struct _EFFECT_OBJ__ {
    void *p_si;                                     /*point to sound in*/
    int (*run)(void *hld, short *inbuf, int len);
    sound_out_obj sound;
} EFFECT_OBJ;

int sound_output(void *priv, void *data, int len);
int sound_input(void *priv, void *data, int len);
bool sound_out_init(sound_out_obj *psound, void *cbuf);

#endif

