#ifndef __BOOT_H__
#define __BOOT_H__

#include "asm_type.h"

#define USE_SYDFILE_NEW 1

struct vm_info {
#if (USE_SYDFILE_NEW == 1)
    u8 align;		  			//from uboot, 按 n * 256 对齐
#endif
    u32 vm_saddr;				//from sdfile, flash addr
    u32 vm_res;					//reverse_bytes
    u32 vm_size;		  		//from sdfile
};

struct sfc_info {
    u32 sfc_base_addr;  		//flash memory addr, from uboot
    u32 app_addr;  				//cpu logic addr, from uboot
};

#if (USE_SYDFILE_NEW == 1)
struct bt_mac_addr {
    u8 value[6];
    u16 value_crc;
};

typedef struct _boot_info {
    struct vm_info vm;
    struct sfc_info sfc;
    u32 flash_size; 		//from uboot
    u16 chip_id; 			//from uboot
    u16 trim_value; 		//from uboot
    // struct bt_mac_addr mac;
    u32 up_suc_flag;
} BOOT_INFO;

//=====================================
struct flash_head {
    u16 crc;
    u16 size4burner;
    u8 vid[4]; 	//u32 vm_eaddr;
    u32 FlashSize;
    u8 FsVersion;  //flash文件结构
    u8 align; 	//对齐 n * 256
    u8 res;
    u8 SpecialOptFlag;
    u8 pid[16];
};

#define SDFILE_NAME_LEN 			16

typedef struct sdfile_file_head {
    u16 head_crc;
    u16 data_crc;
    u32 addr;
    u32 len;
    u8 attr;
    u8 res;
    u16 index;
    char name[SDFILE_NAME_LEN];
} SDFILE_FILE_HEAD;

typedef struct sdfile_file_t {
    u32 fptr;
    struct sdfile_file_head head;
} SDFILE;

typedef struct boot_device_info {
    struct flash_head *fs_info;
    struct sfc_info sfc;
    u16 chip_id; 			//from uboot
    u16 trim_value; 		//from uboot
    u8 bt_mac_addr[8];
} BOOT_DEVICE_INFO;

//存储在虚拟OTP区域,只能写一次
enum votp_id {
    CFG_ID_VOTP_FLASH_INFO = 0,
};

struct flash_wp_arg {   //写保护配置信息
    u8 numOfwp_array;//写保护参数的个数
    u8 wp_sr1_mask; //sr1要保留或修改的bit
    u8 wp_sr2_mask; //sr2要保留或修改的bit
    struct {
        u8 wp_sr1; //写保护sr1取值
        u8 wp_sr2;//写保护sr2取值
        u16 wp_addr;//写保护结束地址,单位K
    } wp_array[32]; //写保护的组数，修改可变长
} __attribute__((packed));

struct flash_otp_arg {//flash otp信息
    u8 otp_lock_sr1_mask;//sr1要保留或修改的bit
    u8 otp_lock_sr2_mask;//sr2要保留或修改的bit
    u8 otp_lock_sr1;//otp sr1的取值
    u8 otp_lock_sr2;//otp sr2的取值
    u16 otp_NumberOfpage;//otp的page数量
    u16 otp_page_size;//otp的page大小
    u32 otp_offset[5];//otp page的偏移地址组数
} __attribute__((packed));

struct flash_io_drv {
    u8 cs_drv: 2;
    u8 clk_drv: 2;
    u8 do_drv: 2;
    u8 di_drv: 2;
    u8 d2_drv: 2;
    u8 d3_drv: 2;
    u8 input_delay: 4;
} __attribute__((packed));

struct	flash_config_info { //flash配置信息的结构体
    u8 write_en_use_50h;//写使能配置0:06H		1:50H
    u8 wr_sr_cmd[2];//寄存器的写命令，当两个命令是一样的，就使用连续写模式
    struct	flash_wp_arg wp;// flash 写保护参数
    struct	flash_otp_arg otp; //flash otp 参数
    struct  flash_io_drv drv; // flash drv 参数
} __attribute__((packed));

#else

#endif  /* #if (USE_SYDFILE_NEW == 1) */

extern BOOT_INFO boot_info;

#endif

