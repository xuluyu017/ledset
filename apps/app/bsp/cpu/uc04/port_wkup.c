#include "typedef.h"
#include "irq.h"
#include "gpio.h"
#include "port_wkup.h"
#include "clock.h"

/* #define LOG_TAG_CONST       PORT_WKUP */
#define LOG_TAG             "[PORT_WKUP]"
#include "log.h"

/**
 * 注意：JL_WAKEUP 区别于PMU管理的唤醒。可理解为一个独立的模块使用。但在低功耗的情况下，中断无效。
 */

//uc04
/* IO唤醒源: 25个唤醒源:
 *    事件0~16: PA0-PA14,USB_DP,USB_DM
 *    事件17~24: GP_ICH0~GP_ICH7 (与事件0~16组成双边沿)
 * */

static void (*port_wkup_irq_cbfun[PORT_WKUP_SRC_NUM_MAX])(void) = {NULL};
static u8 gp_ich_wkup_port[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
/**
 * @brief 引脚中断函数
 */
___interrupt
void port_wkup_irq(void)
{
    u8 wakeup_index = 0;
    /* printf("png:0x%x\n", JL_WAKEUP->CON3); */
    for (; wakeup_index < PORT_WKUP_SRC_NUM_MAX; wakeup_index++) {
        if ((JL_WAKEUP->CON0 & BIT(wakeup_index)) && (JL_WAKEUP->CON3 & BIT(wakeup_index))) {
            JL_WAKEUP->CON2 |= BIT(wakeup_index);
            if (port_wkup_irq_cbfun[wakeup_index]) {
                /* printf(">"); */
                port_wkup_irq_cbfun[wakeup_index]();
            }
        }
    }
}


/*
 * @brief 使能IO口[唤醒/外部中断]
 * @parm port 端口 such as:IO_PORTA_00
 * @parm edge 检测边缘，1 下降沿，0 上升沿
 * @parm cbfun 中断回调函数
 * @return 0 成功，< 0 失败
 */
int port_wkup_enable(u8 port, u8 edge, void (*cbfun)(void))
{
    u8 wkup_s = port;
    if (port >= IO_PORT_MAX) {
        log_error("unsupported port\n");
        return -1;
    }
    if (port >= IO_PORT_DP) {
        wkup_s = port - IO_PORT_DP + 15;
    }
    if (JL_WAKEUP->CON0 & BIT(wkup_s)) {
        log_error("PORT WKUP(%d) has been used\n", port);
        return -1;
    }
    JL_WAKEUP->CON2 = 0xffffffff;  //clear pending
    gpio_set_direction(port, 1);
    gpio_set_die(port, 1);
    if (edge) {
        JL_WAKEUP->CON1 |= BIT(wkup_s);  //detect falling edge
        gpio_set_pull_up(port, 1);
        gpio_set_pull_down(port, 0);
    } else {
        JL_WAKEUP->CON1 &= ~BIT(wkup_s);  //detect rising edge
        gpio_set_pull_up(port, 0);
        gpio_set_pull_down(port, 1);
    }

    if (cbfun) {
        port_wkup_irq_cbfun[wkup_s] = cbfun;
    }
    request_irq(IRQ_PORT_IDX, PORT_WKUP_IRQ_PRIORITY, (u32)port_wkup_irq, 0);//中断优先级
    JL_WAKEUP->CON2 |= BIT(wkup_s);  //clear pending
    JL_WAKEUP->CON0 |= BIT(wkup_s);  //wakeup enable
    /* printf("wkup_s:%d, port:%x [en%x,png%x](ok)\n",wkup_s,port,JL_WAKEUP->CON0,JL_WAKEUP->CON3); */
    return 0;
}

/*
 * @brief 失能IO口[唤醒/外部中断]
 * @parm port 端口 such as:IO_PORTA_00
 * @return null
 */
void port_wkup_disable(u8 port)
{
    u8 i = 0;
    u8 wkup_s = port;
    if (port >= IO_PORT_MAX) {
        log_error("%s parameter:port error!", __func__);
        return ;
    }
    if (port >= IO_PORT_DP) {
        wkup_s = port - IO_PORT_DP + 15;
    }

    if (JL_WAKEUP->CON0 & BIT(wkup_s)) {
        JL_WAKEUP->CON0 &= ~BIT(wkup_s);
        JL_WAKEUP->CON2 |= BIT(wkup_s);
        port_wkup_irq_cbfun[wkup_s] = NULL;
        printf("wkup_s:%d, port:%d (close)\n", wkup_s, port);
        for (i = 17; i < PORT_WKUP_SRC_NUM_MAX; i++) {
            if (JL_WAKEUP->CON0 & BIT(i)) {
                if (gp_ich_wkup_port[i - 17] == port) {
                    return;
                }
            }
        }
        gpio_set_die(port, 0);
        gpio_set_direction(port, 1);
        gpio_set_pull_up(port, 0);
        gpio_set_pull_down(port, 0);
    } else {
        log_error("port wkup source:%d has been closed!", wkup_s);
    }
}
/*
 * @brief 使能ICH0~7口[唤醒/外部中断]
 * @parm port 端口 such as:IO_PORTA_00
 * @parm edge 检测边缘，1 下降沿，0 上升沿
 * @parm cbfun 中断回调函数
 * @return 0 成功，< 0 失败
 */
int port_ich_wkup_enable(u8 port, u8 edge, void (*cbfun)(void))
{
    if (port >= IO_PORT_MAX) {
        log_error("unsupported port\n");
        return -1;
    }
    /* __ich_sel: */

    u8 wkup_s = gpio_port_wkup_ich_enable(port);
    if (wkup_s > 7) {
        log_error("Input Channel Use Full\n");
        return -1;
    }
    wkup_s += 17;
    if (JL_WAKEUP->CON0 & BIT(wkup_s)) {
        log_error("PORT WKUP(%d) has been used\n", wkup_s);
        /* gpio_port_wkup_ich_disable(wkup_s - 17); */
        /* if (wkup_s < 26) { */
        /*     goto __ich_sel; */
        /* } */
        return -1;
    }
    JL_WAKEUP->CON2 = 0xffffffff;  //clear pending
    gpio_set_direction(port, 1);
    gpio_set_die(port, 1);
    if (edge) {
        JL_WAKEUP->CON1 |= BIT(wkup_s);  //detect falling edge
        gpio_set_pull_up(port, 1);
        gpio_set_pull_down(port, 0);
    } else {
        JL_WAKEUP->CON1 &= ~BIT(wkup_s);  //detect rising edge
        gpio_set_pull_up(port, 0);
        gpio_set_pull_down(port, 1);
    }

    if (cbfun) {
        port_wkup_irq_cbfun[wkup_s] = cbfun;
    }
    request_irq(IRQ_PORT_IDX, PORT_WKUP_IRQ_PRIORITY, (u32)port_wkup_irq, 0);//中断优先级
    JL_WAKEUP->CON2 |= BIT(wkup_s);  //clear pending
    JL_WAKEUP->CON0 |= BIT(wkup_s);  //wakeup enable
    gp_ich_wkup_port[wkup_s - 17] = port;
    /* printf("wkup_s:%d, port:%x [en%x,png%x](ok)\n", wkup_s, port, JL_WAKEUP->CON0, JL_WAKEUP->CON3); */
    return 0;
}
/*
 * @brief 失能IO口[唤醒/外部中断]
 * @parm port 端口 such as:IO_PORTA_00
 * @return null
 */
void port_ich_wkup_disable(u8 port)
{
    if (port >= IO_PORT_MAX) {
        log_error("%s parameter:port error!", __func__);
        return ;
    }

    u8 wkup_s, i;
    for (i = 0; i < 8; i++) {
        if (port == gp_ich_wkup_port[i]) {
            wkup_s = i + 17;
            break;
        }
    }
    if (i > 7) {
        log_error("%s parameter:port error!", __func__);
        return;
    }

    u8 wkup_s1 = port;
    if (port >= IO_PORT_DP) {
        wkup_s1 = port - IO_PORT_DP + 15;
    }

    if (JL_WAKEUP->CON0 & BIT(wkup_s)) {
        JL_WAKEUP->CON0 &= ~BIT(wkup_s);
        JL_WAKEUP->CON2 |= BIT(wkup_s);
        gpio_port_wkup_ich_disable(wkup_s - 17);
        gp_ich_wkup_port[wkup_s - 17] = 0xff;
        if ((JL_WAKEUP->CON0 & BIT(wkup_s1)) == 0) {
            gpio_set_die(port, 0);
            gpio_set_direction(port, 1);
            gpio_set_pull_up(port, 0);
            gpio_set_pull_down(port, 0);
        }
        port_wkup_irq_cbfun[wkup_s] = NULL;
        printf("wkup_s:%d, port:%d (close)\n", wkup_s, port);
    } else {
        log_error("port wkup source:%d has been closed!", wkup_s);
    }
}
/*********************************************************************************************************
 * ******************************           使用举例如下           ***************************************
 * ******************************************************************************************************/
#if 0
void wdt_clear();
void _pa6(void)
{
    printf("%s\n", __func__);
}
void _pa7(void)
{
    printf("%s\n", __func__);
}
void _ich0(void)
{
    printf("%s\n", __func__);
}
void _ich1(void)
{
    printf("%s\n", __func__);
}

#define _EDGE_1  0 //0:rise 1:fall
#define _EDGE_2  1
void port_wkup_test()
{
    printf("-------------------port wkup isr---------------------------\n");
    port_wkup_enable(IO_PORTA_06, _EDGE_1, _pa6);//上升沿触发
    port_wkup_enable(IO_PORTA_07, _EDGE_1, _pa7);
    /* port_wkup_enable(IO_PORT_DP, _EDGE_1, _dp); */
    /* port_wkup_enable(IO_PORT_DM, _EDGE_1, _dm); */
    port_ich_wkup_enable(IO_PORTA_06, _EDGE_2, _ich0);//下升沿触发 ich与pa6组成双边沿
    port_ich_wkup_enable(IO_PORTA_07, _EDGE_2, _ich1);//下升沿触发 ich与pa7组成双边沿

    port_ich_wkup_disable(IO_PORTA_07);
    /* port_wkup_disable(IO_PORTA_07); */
    while (1) {
        printf("-");
        mdelay(500);
        wdt_clear();
    }
}
#endif

