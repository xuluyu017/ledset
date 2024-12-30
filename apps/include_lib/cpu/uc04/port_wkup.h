#ifndef _PORT_WKUP_H
#define _PORT_WKUP_H

#include "typedef.h"
/**
 * 注意：JL_WAKEUP 区别于PMU管理的唤醒。可理解为一个独立的模块使用。但在低功耗的情况下，中断无效。
 */

//uc04
/* IO唤醒源: 25个唤醒源:
 *    事件0~16: PA0-PA14,USB_DP,USB_DM
 *    事件17~24: GP_ICH0~GP_ICH7 (与事件0~16组成双边沿)
 * */

// enum gpio_irq_edge {
//     PORT_IRQ_DISABLE = 0,      ///< Disable PORT interrupt
//     PORT_IRQ_EDGE_RISE  = 1,   ///< PORT interrupt type : rising edge
//     PORT_IRQ_EDGE_FALL   = 2,  ///< PORT interrupt type : falling edge
//     PORT_IRQ_ANYEDGE = 3,      ///支持< PORT interrupt type : both rising and falling edge
// };

#define PORT_WKUP_IRQ_PRIORITY  3//中断优先级，范围:0~7(低~高)
#define PORT_WKUP_SRC_NUM_MAX  25//唤醒源个数


int port_wkup_enable(u8 port, u8 edge, void (*cbfun)(void));
void port_wkup_disable(u8 port);
int port_ich_wkup_enable(u8 port, u8 edge, void (*cbfun)(void));
void port_ich_wkup_disable(u8 port);




// int port_wkup_interrupt_init(u32 port, u32 edge, void (*cbfun)(u32 port, u32 edge));
// int port_wkup_interrupt_deinit(u32 port);

// void port_irq_change_callback(u32 port, void (*cbfun)(u32, u32));
// void port_irq_change_en_state(u32 port, u8 wkup_en);//wkup_en:1:en,0:disable
// void port_irq_change_edge_state(u32 port, u32 edge);
// u8 port_irq_get_edge_state(u32 port);

#endif
