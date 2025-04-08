#ifndef  __UPGRADE_H__
#define  __UPGRADE_H__


struct loader_arg_list {
    void *sram_to_usb;
    void *usb_to_sram;
    void **pHook;
    u32 dlmode;
    void **hid_hook;
};
typedef void (*CALL_LOADER)(struct loader_arg_list *arg_list);

#endif  /*UPGRADE_H*/

