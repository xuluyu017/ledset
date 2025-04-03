#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#define USE_FLASH_DEBUG                     1 //开启FLASH调试模式

#define ENABLE_THIS_MOUDLE					1
#define DISABLE_THIS_MOUDLE					0

#define ENABLE								1
#define DISABLE								0
#define NO_CONFIG_PORT						(-1)

#include "app_cfg_def.h"

/*---------系统时钟配置---------------------*/
#define TCFG_PLL_SEL                        PLL_D2p0_192M
#define TCFG_PLL_DIV                        PLL_DIV1
#define TCFG_HSB_DIV                        HSB_DIV1

/*---------UART Configuration---------------*/
#define TCFG_DEBUG_ENABLE                   ENABLE_THIS_MOUDLE//调试总开关
#define TCFG_UART_TX_PORT  					IO_PORTA_14	//串口打印发送脚配置
#define TCFG_UART_BAUDRATE  				1000000     //串口波打印特率配置

/*---------LOW POWER Configuration----------*/
#define KEY_WAKEUP_ENABLE				0//	ENABLE//按键唤醒使能
#define KEY_WAKEUP_IO				    	IO_PORTA_10//按键唤醒IO选择
#define KEY_WAKEUP_EDGE				        FALLING_EDGE//唤醒方式选择
#define KEY_PULLUP_DOWN_ENABLE				ENABLE//内部上下拉是否使能
#define KEY_FILTER_ENABLE				    ENABLE//滤波使能

#define CHARGE_WAKEUP_ENABLE				DISABLE //充电唤醒使能
#define CHARGE_WAKEUP_IO				    IO_PORTA_10//充电唤醒IO
#define CHARGE_WAKEUP_EDGE				    RISING_EDGE//唤醒方式选择
#define CHARGE_PULLUP_DOWN_ENABLE			DISABLE //内部上下拉是否使能
#define CHARGE_FILTER_ENABLE 				ENABLE//滤波使能

#define LOW_PWR_VOLTAGE						3100//低电压阀值
#define LOW_PWR_BASE_CNT  					3//按键滤波次数
//充电唤醒--SDK中没有这个充电唤醒流程，只提供对应的充电唤醒IO,用户需根据对应充电IC进行开发,这里只起demo的作用

/*---------KEY Configuration-------------------*/
#define KEY_IO_EN         	                DISABLE//<IO按键使能
#define KEY_AD_EN				            DISABLE//<AD按键使能
//选择何种按键唤醒
//AD KEY
#define AD_KEY_IO_SEL		                KEY_WAKEUP_IO
//IO KEY
#define IO_KEY_PORT							KEY_WAKEUP_IO
#define   KEY_MIC_EN  DISABLE
#define  KEY_IR_EN  DISABLE
#define IR_KEY_IO			                IO_PORTA_06
#define IR_KEY_IRQ_IDX                      IRQ_TIME2_IDX
#define IR_KEY_TIMER                        JL_TIMER2

#define  KEY_MATRIX_EN  DISABLE

/*---------长按复位 Configuration--------------*/
#define PINR_RESET_IO                       IO_PORTA_01
#define PINR_RESET_LVL                      0 // 0下降沿 1上升沿
#define PINR_PRESS_TIME						0 // 0/1/2/4/8/16 秒 -- 设置为0则关闭长按复位功能
#define PINR_RESET_RELASE					1 // 0释放后复位 1立即复位
#define PINR_PROTECT_EN						0 // 写保护使能

/*---------MCLR复位 Configuration--------------*/
#define CONFIG_MCLR_EN                      FALSE

/*---------VM_SFC Configuration----------------*/
#define VM_SFC_ENABLE                       ENABLE
#define SYD_FS_ENABLE                       ENABLE

/*---------IOVDD configuration-----------------*/
#define TCFG_VDDIOM_LEVEL			        VDDIOM_VOL_33V//选用高档位VDDIO

/*---------ADC相关配置-------------------------*/
#define TCFG_MIC_INPUT_MODE                 MACRO_MIC_INPUT_PA7//MACRO_MIC_INPUT_PA8     //MIC输入模式			//BZ
#define TCFG_MIC_GAIN                       AUMIC_21db              //配置MIC的增益
#define TCFG_MIC_DIG_GAIN                   1.26f                   //dB = 20log10(Mag) Mag = 10^(dB/20)
#define TCFG_MICLDO_LEVEL                   AUMIC_2v3               //TYPEC耳机选用高档位MICLDO
#define TCFG_MIC_BIAS_RS_SEL                AUMIC_1k0               //内置电阻档位选择
#define TCFG_MIC_CAPLESS                 0                     //0:带电容方??1:免电容方??免电容方案会自动校准MIC_BIAS_RS)

/*---------DAC相关配置-------------------------*/
#define TCFG_TONE_PLAY_ENABLE               DISABLE                 //是否使能提示音播放
#define TCFG_DAC_ENABLE                     TCFG_TONE_PLAY_ENABLE   //是否需要DAC播放
#define TCFG_DAC_ANA_VOL                    AUDAC_0db               //DAC模拟增益配置

#if TCFG_TONE_PLAY_ENABLE
#define TCFG_DEC_WTG_ENABLE                 ENABLE                  //使能wtg解码
#endif

/*---------ICACHE RAM 相关配置-----------------*/
#define ICACHE_RAM_TO_RAM_ENABLE			1//ICACHE RAM用作普通RAM使能位
#define ICACHE_RAM_TO_RAM					8192//将多少ICACHE RAM用作普通RAM----配置大小：4K、8K

/*---------FLASH工作模式相关配置-----------------*/
#define CONFIG_SPI_DATA_WIDTH			    4 // 2:flash采用两线工作模式，4:flash采用四线工作模式

#define PMU_TRIM_ENABLE                     1 // 是否使用pmu_trim
#include "byle_cfg.h"

#endif

