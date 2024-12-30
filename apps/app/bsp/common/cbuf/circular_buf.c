#include "circular_buf.h"
#include "string.h"
//#define CBUF_EN
#ifdef CBUF_EN
#define cbuf_putchar(x)        putchar(x)
#define cbuf_puts(x)           puts(x)
#define cbuf_u32hex(x)         put_u32hex(x)
#define cbuf_u8hex(x)          put_u8hex(x)
#define cbuf_buf(x,y)          printf_buf(x,y)
// #define debug                   printf
#else
#define cbuf_putchar(...)
#define cbuf_puts(...)
#define cbuf_u32hex(...)
#define cbuf_u8hex(...)
#define cbuf_buf(...)
#endif
#define debug(...)

u32 cbuf_read(cbuffer_t *cbuffer, void *buf, u32 len)
{
    u32 r_len = len;
    u32 copy_len;
    cbuf_puts("cbuf r a\n");
    if (!cbuffer) {
        return 0;
    }

    cbuf_puts("cbuf r b\n");
    if ((u32)cbuffer->read_ptr >= (u32)cbuffer->end) {
        cbuffer->read_ptr = (u8 *)cbuffer->begin;
    }

    cbuf_puts("cbuf r c\n");
    if (cbuffer->data_len < len) {
        /* memset(buf, 0, len); */
        return 0;
    }

    cbuf_puts("cbuf r d\n");

    copy_len = (u32)cbuffer->end - (u32)cbuffer->read_ptr;
    if (copy_len > len) {
        copy_len = len;
    }
    len -= copy_len;

    cbuf_puts("cbuf r e\n");
    memcpy(buf, cbuffer->read_ptr, copy_len);
    //printf_data(cbuffer->read_ptr,copy_len) ;

    if (len == 0) {
        cbuffer->read_ptr += copy_len;
    } else {
        memcpy((u8 *)buf + copy_len, cbuffer->begin, len);
        //printf_data(cbuffer->begin,len);
        cbuffer->read_ptr = cbuffer->begin + len;
    }

    cbuf_puts("cbuf r f\n");
    local_irq_disable();
    cbuffer->tmp_len = cbuffer->data_len -= r_len;

    cbuffer->tmp_len = cbuffer->data_len;

    local_irq_enable();

    cbuf_puts("cbuf r g\n");
    return r_len;
}

u32 cbuf_write(cbuffer_t *cbuffer, void *buf, u32 len)
{
    u32 length;
    u32 remain_len;

    if (!cbuffer) {
        return 0;
    }

    local_irq_disable();
    if ((cbuffer->total_len - cbuffer->data_len) < len) {
        len = cbuffer->total_len - cbuffer->data_len ;
        if (len == 0) {
            local_irq_enable();
            return 0;
        }
    }
    /* local_irq_enable(); */


    length = (u32)cbuffer->end - (u32)cbuffer->write_ptr;
    if (length >= len) {
        memcpy(cbuffer->write_ptr, buf, len);
        cbuffer->write_ptr += len;
    } else {
        remain_len = len - length;
        memcpy(cbuffer->write_ptr, buf, length);
        memcpy(cbuffer->begin, ((u8 *)buf) + length, remain_len);
        cbuffer->write_ptr = (u8 *)cbuffer->begin + remain_len;
    }

    /* local_irq_disable(); */
    cbuffer->data_len += len;
    cbuffer->tmp_len = cbuffer->data_len ;
    cbuffer->tmp_ptr = cbuffer->write_ptr ;
    local_irq_enable();

    return len;
}

u32 cbuf_is_write_able(cbuffer_t *cbuffer, u32 len)
{
    u32 w_len;

    if (!cbuffer) {
        return 0;
    }
    w_len = cbuffer->total_len - cbuffer->data_len;
    if (w_len < len) {
        return 0;
    }

    return w_len;
}

void *cbuf_write_alloc(cbuffer_t *cbuffer, u32 *len)
{
    u32 data_len;

    if (!cbuffer) {
        // puts("null ptr \r\n") ;
        return 0;
    }

    local_irq_disable();
    *len = cbuffer->end - cbuffer->write_ptr;
    data_len = cbuffer->total_len - cbuffer->data_len;
    if (*len == 0) {
        cbuffer->write_ptr = cbuffer->begin;
        *len = data_len;
    }
    if (*len > data_len) {
        *len = data_len;
    }
    local_irq_enable();
    return cbuffer->write_ptr;
}

void cbuf_write_updata(cbuffer_t *cbuffer, u32 len)
{
    local_irq_disable();
    cbuffer->tmp_ptr = cbuffer->write_ptr += len;
    cbuffer->tmp_len = cbuffer->data_len += len;
    local_irq_enable();
}


void *cbuf_read_alloc(cbuffer_t *cbuffer, u32 *len)
{
    u32 data_len;

    if (!cbuffer) {
        //  puts("null ptr \r\n") ;
        return 0;
    }
    local_irq_disable();
    if ((u32)cbuffer->read_ptr >= (u32)cbuffer->end) {
        cbuffer->read_ptr = (u8 *)cbuffer->begin;
    }

    data_len = cbuffer->data_len ;

    *len  = (u32)cbuffer->end - (u32)cbuffer->read_ptr;
    if (data_len <= *len) {
        *len = data_len;
    }
    local_irq_enable();
    return cbuffer->read_ptr;
}

void cbuf_read_updata(cbuffer_t *cbuffer, u32 len)
{
    local_irq_disable();

    cbuffer->read_ptr += len;
    if ((u32)cbuffer->read_ptr >= (u32)cbuffer->end) {
        cbuffer->read_ptr = (u8 *)cbuffer->begin;
    }

    cbuffer->tmp_len =  cbuffer->data_len -= len;

    local_irq_enable();
}

void cbuf_init(cbuffer_t *cbuffer, void *buf, u32 size)
{
    cbuffer->data_len = 0;
    cbuffer->tmp_len = 0 ;
    cbuffer->begin = buf;
    cbuffer->read_ptr = buf;
    cbuffer->write_ptr = buf;
    cbuffer->tmp_ptr = buf;
    cbuffer->end = (u8 *)buf + size;
    cbuffer->total_len = size;
}

void cbuf_clear(cbuffer_t *cbuffer)
{
    local_irq_disable();
    cbuffer->read_ptr = cbuffer->begin;
    cbuffer->tmp_ptr = cbuffer->write_ptr = cbuffer->begin;
    cbuffer->data_len = 0;
    cbuffer->tmp_len = 0 ;
    local_irq_enable();

}


 u32 cbuf_get_data_size(cbuffer_t *cbuffer)
{

        return cbuffer->data_len;
}

 u32 cbuf_get_data_len(cbuffer_t *cbuffer)
{

        return cbuffer->data_len;
}


u32 cbuf_rewrite(cbuffer_t *cbuffer, void *begin, void *buf, u32 len)
{
//    u32 sr;
    u32 length;
    u32 remain_len;
    u8 *write_ptr = (u8 *)begin ;
    if (!cbuffer) {
        return 0;
    }

    length = (u32)cbuffer->end - (u32)write_ptr;
    if (length >= len) {

        memcpy(cbuffer->write_ptr, buf, len);
        write_ptr += len;

    } else {

        remain_len = len - length;
        memcpy(write_ptr, buf, length);
        memcpy(cbuffer->begin, ((u8 *)buf) + length, remain_len);

    }


    return len;
}


