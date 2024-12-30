#include "app_config.h"
#include "byle_cfg.h"
#define _CAT2(a,b) a ## _ ## b
#define CAT2(a,b) _CAT2(a,b)

#define _CAT3(a,b,c) a ## _ ## b ##  _  ## c
#define CAT3(a,b,c) _CAT3(a,b,c)

#define _CAT4(a,b,c,d) a ## _ ## b ##  _  ## c ## _ ## d
#define CAT4(a,b,c,d) _CAT4(a,b,c,d)


#ifndef CONFIG_DOWNLOAD_MODEL
#define CONFIG_DOWNLOAD_MODEL                   USB                   //下载模式选择，可选配置USB\SERIAL
#endif

#ifndef CONFIG_ENTRY_ADDRESS
#define CONFIG_ENTRY_ADDRESS                    0x4000100             //程序入口地址，一般不需要修改(跟张恺讨论过把RESERVED_OPT=0合并到一个配置项)
#endif

#ifndef CONFIG_SPI_DATA_WIDTH
#define CONFIG_SPI_DATA_WIDTH                   4                      //data_width[0 1 2 3 4] 3的时候uboot自动识别2或者4线
#endif

#ifndef CONFIG_SPI_CLK_DIV
#define CONFIG_SPI_CLK_DIV                      3                     //clk [0-255]
#endif

//mode:
//	  0 RD_OUTPUT,		 1 cmd 		 1 addr
//    1 RD_I/O,   		 1 cmd 		 x addr
//	  2 RD_I/O_CONTINUE] no_send_cmd x add
#ifndef CONFIG_SPI_MODE
#define CONFIG_SPI_MODE                         0
#endif

//port:
//	  0  优先选A端口
//	  1  优先选B端口
#ifndef CONFIG_SPI_PORT
#define CONFIG_SPI_PORT                         0
#endif

#ifndef CONFIG_MCLR_EN
#define CONFIG_MCLR_EN                          FALSE
#endif

#ifndef CONFIG_PINR_RESET_EN
#define CONFIG_PINR_RESET_EN                    FALSE
#endif


[EXTRA_CFG_PARAM]
NEW_FLASH_FS = YES;
CHIP_NAME = AD12N;
ENTRY = CONFIG_ENTRY_ADDRESS;
PID = UC04;
VID = 0.01;
RESERVED_OPT = 0;
OTP_CFG_SIZE = 512;
CHECK_OTA_BIN = NO;
DOWNLOAD_MODEL = CONFIG_DOWNLOAD_MODEL;
NEED_RESERVED_4K = YES;
NEED_RESERVED_AREA = NO;
SPECIAL_OPT = 0;
FORCE_4K_ALIGN = YES;

//专家模式配置项
//lvd: 1.8v 2.0v 2.1v 2.2v 2.3v 2.4v 2.5v
[MASTER_CONFIG_OPTIONS]
EN_ACT = FALSE;
LVD_RST_EN = TRUE;
LVD_LEVEL = 2.5v;
IOVDD_LEVEL = 3.3v;
MCLR_EN = CONFIG_MCLR_EN;
PINR_IO_ENABLE = CONFIG_PINR_RESET_EN;

[BURNER_PASSTHROUGH_CFG]
FLASH_WRITE_PROTECT = YES;

// #####匹配的芯片版本,请勿随意改动######
[CHIP_VERSION]
SUPPORTED_LIST = A, B;


// #####UBOOT配置项，请勿随意调整顺序####
[SYS_CFG_PARAM]
SPI = CAT4(CONFIG_SPI_DATA_WIDTH, CONFIG_SPI_CLK_DIV, CONFIG_SPI_MODE, CONFIG_SPI_PORT);
  #ifndef byle_tnk_file
           #if(Byle_Authorization_file==Byle_Authorization_keyI)
           #define  byle_tnk_file	  file--authrun-AD12N-id-317_keyI.tkn
           #endif
  #endif
 
 
 #ifdef KWS_AEC_EN
  #ifndef AEC_tnk_file
  #define  AEC_tnk_file  file--authrun-AD12N-id-101_kwsB_AEC.tkn
  #endif
 
 #endif
   
  #if(KWS_TYPE== kwsA)&&defined(BYLE_KWS_ASR)
  
#define USER_VM_LEN 8K
   #ifdef KWA_customizable_lib
   #define  kwsA_tnk_file	  file--authrun-AD12N-id-128_kwsA_nor.tkn
  #endif
  #ifndef kwsA_tnk_file
  #define  kwsA_tnk_file	file--authrun-AD12N-id-128_kwsA.tkn
  #endif
  
  #endif	
   
  #if(KWS_TYPE== kwsB)&&defined(BYLE_KWS_ASR)
  #ifndef kwsB_tnk_file
  #define  kwsB_tnk_file	file--authrun-AD12N-id-100_kwsB2.tkn
  #endif
  #endif	

#if(KWS_TYPE== kwsC||KWS_TYPE== kwsC1)&&defined(BYLE_KWS_ASR)
#ifndef kwsC_tnk_file
#define  kwsC_tnk_file	  file--authrun-AD12N-id-KwsC.tkn
#undef byle_tnk_file
#define  byle_tnk_file	  file--authrun-AD12N-id-317_keyI_old.tkn 
#endif
#endif	  

#if(KWS_TYPE== kwsD)&&defined(BYLE_KWS_ASR)
#ifndef kwsD_tnk_file
#define  kwsD_tnk_file	  file--authrun-AD12N-id-KwsD.tkn
#endif
#endif	  

  
  #ifdef IR_LIB_TYPE
           #if(IR_LIB_TYPE== lib_irA)
             #ifndef irA_tnk_file
               #define  irA_tnk_file file--authrun-AD12N-id-127_irA.tkn
               #endif
           #endif		 
            #if(IR_LIB_TYPE== lib_irB)
          #define  irB_tnk_file  file--authrun-AD12N-id-101_irB.tkn
          #endif
  #endif 


 // ########多方算法授权文件##############################################

#ifdef byle_tnk_file
#if defined(kwsA_tnk_file)&&defined(AEC_tnk_file) 
[FW_ADDITIONAL] 
FILE_LIST=(file=kwsA_tnk_file:type=0xec),(file=AEC_tnk_file:type=0xec),(file=byle_tnk_file:type=0xec)
#elif defined(kwsA_tnk_file)&&defined(irA_tnk_file)&&defined(irB_tnk_file)
[FW_ADDITIONAL] 
FILE_LIST=(file=kwsA_tnk_file:type=0xec),(file=irA_tnk_file:type=0xec),(file=irB_tnk_file:type=0xec),(file=byle_tnk_file:type=0xec)
#elif defined(kwsA_tnk_file)&&defined(irA_tnk_file)
[FW_ADDITIONAL] 
FILE_LIST=(file=kwsA_tnk_file:type=0xec),(file=irA_tnk_file:type=0xec),(file=byle_tnk_file:type=0xec)
#elif defined(kwsA_tnk_file)
[FW_ADDITIONAL] 
FILE_LIST=(file=kwsA_tnk_file:type=0xec),(file=byle_tnk_file:type=0xec)
#elif defined(kwsC_tnk_file)
[FW_ADDITIONAL] 
FILE_LIST=(file=kwsC_tnk_file:type=0xec),\
                  (file=byle_tnk_file:type=0xec)
#elif defined(kwsD_tnk_file)
[FW_ADDITIONAL] 
FILE_LIST=(file=byle_tnk_file:type=0xec),\
                  (file=kwsD_tnk_file:type=0xec)
                                   
#elif defined(AEC_tnk_file) 
[FW_ADDITIONAL] 
FILE_LIST=(file=AEC_tnk_file:type=0xec),(file=byle_tnk_file:type=0xec)
#else
[FW_ADDITIONAL] 
FILE_LIST=(file=byle_tnk_file:type=0xec)
#endif 

[BURNER_PASSTHROUGH_CFG]
DOWNLOAD_ONLY_AUTHRUN=YES;

#endif

[TOOL_CONFIG]
1TO2_MIN_VER = 2.27.8
1TO8_MIN_VER = 3.1.23

#ifndef CONFIG_VM_ADDR
#define CONFIG_VM_ADDR		0
#endif

#ifdef TCFG_VM_SIZE
#define __VM_SIZE(size)     size##K
#define _VM_SIZE(size)      __VM_SIZE(size)
#define CONFIG_VM_LEAST_SIZE	_VM_SIZE(TCFG_VM_SIZE)
#endif

#ifndef CONFIG_VM_LEAST_SIZE
#define CONFIG_VM_LEAST_SIZE    8K
#endif

#ifndef CONFIG_VM_OPT
#define CONFIG_VM_OPT       1
#endif

#ifndef CONFIG_EEPROM_ADDR
#define CONFIG_EEPROM_ADDR	AUTO
#endif

#ifndef CONFIG_EEPROM_LEN
#define CONFIG_EEPROM_LEN	8K
#endif

#ifndef CONFIG_EEPROM_OPT
#define CONFIG_EEPROM_OPT	1
#endif

#ifndef CONFIG_BURNER_INFO_SIZE
#define CONFIG_BURNER_INFO_SIZE		32
#endif


// ########flash空间使用配置区域###############################################
// #PDCTNAME:    产品名，对应此代码，用于标识产品，升级时可以选择匹配产品名
// #BOOT_FIRST:  1=代码更新后，提示APP是第一次启动；0=代码更新后，不提示
// #UPVR_CTL：   0：不允许高版本升级低版本   1：允许高版本升级低版本
// #XXXX_ADR:    区域起始地址	AUTO：由工具自动分配起始地址
// #XXXX_LEN:    区域长度		CODE_LEN：代码长度
// #XXXX_OPT:    区域操作属性
// #
// #
// #
// #操作符说明  OPT:
// #	0:  下载代码时擦除指定区域
// #	1:  下载代码时不操作指定区域
// #	2:  下载代码时给指定区域加上保护
// ############################################################################
                              [RESERVED_CONFIG]
//for volatile memory area cfg
VM_ADR = CONFIG_VM_ADDR;
VM_LEN = CONFIG_VM_LEAST_SIZE;
VM_OPT = CONFIG_VM_OPT;

//for volatile eeprom area cfg
EEPROM_ADR = CONFIG_EEPROM_ADDR;
EEPROM_LEN = CONFIG_EEPROM_LEN;
EEPROM_OPT = CONFIG_EEPROM_OPT;

[BURNER_CONFIG]
SIZE = CONFIG_BURNER_INFO_SIZE;


