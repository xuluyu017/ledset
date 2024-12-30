/*
** adpcm.h - include file for adpcm coder.
**
** Version 1.0, 7-Jul-92.
*/

#ifndef _BYLE_CODE_H
#define _BYLE_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

struct code_state {
    short	valprev;	/* Previous output value */
    char	index;		/* Index into stepsize table */
};

int byle_audio_code_lib(short *indata, int audio_len,unsigned char *outdata);
int byle_audio_decode_lib(unsigned char *indata, int len,short *outdata);
int byle_audio_double_code_lib(short *mic_in, int mic_len,short *ref_in, int ref_len,unsigned char *out_buf);
int byle_audio_double_decode_lib(unsigned char *indata, int len,short *outdata);

int byle_auido_coder(short *indata, unsigned char *outdata, int len, struct code_state *state);
int byle_auido_decoder(unsigned char *indata, short *outdata, int len, struct code_state *state);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* ADPCM_H*/
