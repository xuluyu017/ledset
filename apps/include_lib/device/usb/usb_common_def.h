#ifndef __USB_COMMON_DEFINE_H__
#define __USB_COMMON_DEFINE_H__

///<<<注意此文件不要放函数声明, 只允许宏定义, 并且差异化定义可以根据需求在对应板卡中重新定义, 除非新增，否则不要直接修改这里
///<<<注意此文件不要放函数声明, 只允许宏定义, 并且差异化定义可以根据需求在对应板卡中重新定义, 除非新增，否则不要直接修改这里
///<<<注意此文件不要放函数声明, 只允许宏定义, 并且差异化定义可以根据需求在对应板卡中重新定义, 除非新增，否则不要直接修改这里
//
/**************************************************************************/
/*
               CLASS  BITMAP
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0
                                   HID    AUDIO  SPEAKER   Mass Storage
*/
/**************************************************************************/
#define     SPEAKER_CLASS       (1UL << (0))
#define     MIC_CLASS           (1UL << (1))
#define     HID_CLASS           (1UL << (2))
#define     CDC_CLASS           (1UL << (3))
#define     MASSSTORAGE_CLASS   (1UL << (4))

#define     AUDIO_CLASS         (SPEAKER_CLASS|MIC_CLASS)

#define     USB_ROOT2   0

#define     USB_DEVICE_CLASS_CONFIG         (MIC_CLASS)

#if 1//TCFG_PC_ENABLE
#define TCFG_USB_SLAVE_ENABLE               1
#if (USB_DEVICE_CLASS_CONFIG & MASSSTORAGE_CLASS)
#define TCFG_USB_SLAVE_MSD_ENABLE           1
#else
#define TCFG_USB_SLAVE_MSD_ENABLE           0
#endif

#if (USB_DEVICE_CLASS_CONFIG & AUDIO_CLASS)
#define TCFG_USB_SLAVE_AUDIO_ENABLE         1
#else
#define TCFG_USB_SLAVE_AUDIO_ENABLE         0
#endif

#if (USB_DEVICE_CLASS_CONFIG & HID_CLASS)
#define TCFG_USB_SLAVE_HID_ENABLE           1
#else
#define TCFG_USB_SLAVE_HID_ENABLE           0
#endif

#if (USB_DEVICE_CLASS_CONFIG & CDC_CLASS)
#define TCFG_USB_SLAVE_CDC_ENABLE           1
#else
#define TCFG_USB_SLAVE_CDC_ENABLE           0
#endif

#else  /* TCFG_PC_ENABLE == 0*/
#define TCFG_USB_SLAVE_ENABLE               0
#define TCFG_USB_SLAVE_MSD_ENABLE           0
#define TCFG_USB_SLAVE_AUDIO_ENABLE         0
#define TCFG_USB_SLAVE_HID_ENABLE           0
#define TCFG_USB_SLAVE_CDC_ENABLE           0
#endif



#if 0
#ifdef AUDIO_PCM_DEBUG
#undef TCFG_PC_ENABLE
#undef TCFG_UDISK_ENABLE
#define TCFG_PC_ENABLE						DISABLE_THIS_MOUDLE//PC模块使能
#define TCFG_UDISK_ENABLE					DISABLE_THIS_MOUDLE//U盘模块使能
#endif/*AUDIO_PCM_DEBUG*/

#if TCFG_UDISK_ENABLE
#define MOUNT_RETRY                         3
#define MOUNT_RESET                         40
#define MOUNT_TIMEOUT                       50
#define     USB_HOST_ENABLE                 1
#else
#define     USB_HOST_ENABLE                 0
#endif

#if  TCFG_USB_PORT_CHARGE
#define TCFG_OTG_MODE_CHARGE                (OTG_CHARGE_MODE)
#else
#define TCFG_OTG_MODE_CHARGE                0
#endif

#if (TCFG_PC_ENABLE)
#define TCFG_PC_UPDATE                      1
#define TCFG_OTG_MODE_SLAVE                 (OTG_SLAVE_MODE)
#else
#define TCFG_PC_UPDATE                      0
#define TCFG_OTG_MODE_SLAVE                 0
#endif

#if (USB_HOST_ENABLE)
#define TCFG_OTG_MODE_HOST                  (OTG_HOST_MODE)
#else
#define TCFG_OTG_MODE_HOST                  0
#endif

#define TCFG_OTG_SLAVE_ONLINE_CNT           2
#define TCFG_OTG_SLAVE_OFFLINE_CNT          2

#define TCFG_OTG_HOST_ONLINE_CNT            0
#define TCFG_OTG_HOST_OFFLINE_CNT           3

#ifndef TCFG_OTG_MODE
// #define TCFG_OTG_MODE                       (TCFG_OTG_MODE_HOST|TCFG_OTG_MODE_SLAVE|TCFG_OTG_MODE_CHARGE)
#define TCFG_OTG_MODE                       (TCFG_OTG_MODE_HOST)
#endif

#define TCFG_OTG_DET_INTERVAL               50


#endif
#endif
