
#pragma bss_seg(".dev_lst.data.bss")
#pragma data_seg(".dev_lst.data")
#pragma const_seg(".dev_lst.text.const")
#pragma code_seg(".dev_lst.text")
#pragma str_literal_override(".dev_lst.text.const")

#include "config.h"
#include "common.h"
#include "app_config.h"
#include "device.h"
#include "norflash.h"
#include "sd.h"
#include "gpio.h"
#include "msg.h"

#define LOG_TAG_CONST       NORM
/* #define LOG_TAG_CONST       OFF */
#define LOG_TAG             "[device_list]"
#include "log.h"


REGISTER_DEVICES(device_table) = {
#ifdef VM_SFC_ENABLE
#if VM_SFC_ENABLE
    {.name = __SFC_NANE, .ops = &sfc_dev_ops, .priv_data = (void *)NULL},
#endif
#endif
};
// *INDENT-ON*

int	devices_init_api()
{
    set_device_node((struct dev_node *)device_node_begin, (struct dev_node *)device_node_end);

    return devices_init();
}



#if 0
#define SD0_DEV_TEST	0
#if SD0_DEV_TEST
static u8 sd_buffer[512];
void *sdx_dev_get_cache_buf(void)
{
    return sd_buffer;
}
#endif
#define n_n	1
static u8 dev_read_buf[n_n * 512] = {0};
static u8 dev_write_buf[n_n * 512] = {0};
static u8 dev_backup_buf[n_n * 512] = {0};
extern void delay(volatile u32 t);

void device_test_demo(void)
{
    log_info("%s()  enter!\n", __func__);
    u32 i = 0;
    struct device *device;
    /* log_info("device_node_begin : 0x%x, device_node_end : 0x%x\n", (u32)device_node_begin, (u32)device_node_end); */

    devices_init_api();

#if SD0_DEV_TEST
    log_info("%s(), line:%d\n", __func__, __LINE__);
    sdx_force_set_online("sd0");
    device = dev_open("sd0", 0);
#endif
    log_info("%s(), line:%d\n", __func__, __LINE__);
    if (NULL == device) {
        log_info("device null!\n");
        return;
    } else {
        log_info("device open ok!!\n");
    }
    log_info("%s(), line:%d\n", __func__, __LINE__);

    log_info("original dev data:\n");
    dev_bulk_read(device, dev_read_buf, 0, n_n);
    memcpy(dev_backup_buf, dev_read_buf, n_n * 512);
    log_info_hexdump(dev_read_buf, n_n * 512);

#if 0	//erase test
    log_info("--------------dev sector erase(0)-------------\n");
    if (dev_ioctl(device, IOCTL_ERASE_SECTOR, 0)) {
        log_info("erase error!\n");
        while (1);
    }
    log_info("after erase read_buf:\n");
    dev_byte_read(device, dev_read_buf, 0, n_n * 512);
    log_info_hexdump(dev_read_buf, n_n * 512);
#endif
#if 1	//bulk test
    for (int i = 0; i < 512; i++) {
        dev_write_buf[i] = 'a' + i;
    }
    log_info("dev bulk write/read test!\n");
    dev_bulk_write(device, dev_write_buf, 0, n_n);
    delay(10000);
    dev_bulk_read(device, dev_read_buf, 0, n_n);
    log_info("bulk read buf:\n");
    log_info_hexdump(dev_read_buf, n_n * 512);
#endif
#if	1	//byte test
    for (int i = 0; i < 512; i++) {
        dev_write_buf[i] = 'A' + i;
    }
    log_info("dev byte write/read test!\n");
    dev_byte_write(device, dev_write_buf, 0, n_n * 512);
    delay(10000);
    dev_byte_read(device, dev_read_buf, 0, n_n * 512);
    log_info("byte read buf:\n");
    log_info_hexdump(dev_read_buf, n_n * 512);
#endif

    log_info("backup release!\n");
    dev_bulk_write(device, dev_backup_buf, 0, n_n * 512);
    dev_bulk_read(device, dev_read_buf, 0, n_n * 512);
    log_info_hexdump(dev_read_buf, n_n * 512);

    dev_close(device);
}
#endif
