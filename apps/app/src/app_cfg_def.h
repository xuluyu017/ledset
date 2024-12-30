#include "app_config.h"
/*---------FLASH SYSTEM Configuration-------*/
#if USE_FLASH_DEBUG
#define VM_SFC_ENABLE                       ENABLE
#define SYD_FS_ENABLE                       ENABLE
#endif

/*---------FLASH Configuration--------------*/
#define TFG_EXT_FLASH_EN				    DISABLE

/*---------SD Configuration-----------------*/
#define TFG_SD_EN				            DISABLE
#define TFG_SDPG_ENABLE                     DISABLE
/*---------Power Wakeup IO------------------*/
#define POWER_WAKEUP_IO				        IO_PORTA_02
#define POWER_WAKEUP_EDGE				    RISING_EDGE

/*---------EQ Configuration-----------------*/
#define SIMPLE_EQ_ENABLE                    DISABLE//简单eq总使能

/*---------USB Configuration----------------*/
#define TCFG_PC_ENABLE						ENABLE  //PC模块使能
#define TCFG_USB_MSD_CDROM_ENABLE           DISABLE
#define TCFG_USB_EXFLASH_UDISK_ENABLE       DISABLE //外掛FLASH UDISK
#define TCFG_UDISK_ENABLE					DISABLE //U盘模块使能
#define TCFG_OTG_USB_DEV_EN                 ENABLE
#define TCFG_USB_PORT_CHARGE                DISABLE
#define SD_CDROM_EN                         0


