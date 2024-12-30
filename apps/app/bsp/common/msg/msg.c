#include "msg.h"
#include "irq.h"
#include "common.h"
/* #include "string.h" */
#include "circular_buf.h"
#include "uart.h"
#include <stdarg.h>
#include "config.h"

/* #define LOG_TAG_CONST       NORM */
#define LOG_TAG_CONST       OFF
#define LOG_TAG             "[msg]"
#include "log.h"

static cbuffer_t msg_cbuf;
static u32 msg_pool[MAX_POOL];

int get_msg(int *msg)
{
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    u32 tlen = cbuf_read(&msg_cbuf, (void *)msg, 4);
    if (4 != tlen) {
        OS_EXIT_CRITICAL();
        __asm__ volatile("idle");
        *msg = NO_MSG;
        return MSG_NO_MSG;
    }
    OS_EXIT_CRITICAL();
    return MSG_NO_ERROR;
}

int post_msg(int msg)
{
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    if (!cbuf_is_write_able(&msg_cbuf, 4)) {
        OS_EXIT_CRITICAL();
        return MSG_BUF_NOT_ENOUGH;
    }
    cbuf_write(&msg_cbuf, (void *)&msg, 4);
    OS_EXIT_CRITICAL();
    return MSG_NO_ERROR;
}

void clear_all_message(void)
{
    cbuf_clear(&msg_cbuf);
}

void message_init()
{
    cbuf_init(&msg_cbuf, msg_pool, sizeof(msg_pool));
    cbuf_clear(&msg_cbuf);
}

