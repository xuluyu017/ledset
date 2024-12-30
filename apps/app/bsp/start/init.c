
#pragma bss_seg(".init.data.bss")
#pragma data_seg(".init.data")
#pragma const_seg(".init.text.const")
#pragma code_seg(".init.text")
#pragma str_literal_override(".init.text.const")

#include "init.h"
#include "tick_timer_driver.h"
#include "device.h"
#include "msg.h"
#include "my_malloc.h"
#include "init_app.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[init]"
#include "log.h"
extern const u8 have_tick_timer;
void system_init(void)
{
    my_malloc_init();
    if (have_tick_timer == 1) {
        tick_timer_init();
    }
    message_init();
    app_system_init();
}

