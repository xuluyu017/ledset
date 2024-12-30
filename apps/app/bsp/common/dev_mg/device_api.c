#include "device.h"

#define LOG_TAG_CONST       NORM
#define LOG_TAG             "[dev_api]"
#include "log.h"


struct dev_node_mg device_node;

#define list_for_each_device(p) \
    for(p = device_node.device_node_begin; p < device_node.device_node_end; p++)

void *dev_open(const char *name, void *arg)
{
    int i;
    int err;
    const struct dev_node *p;
    struct device *device;
    list_for_each_device(p) {
        if (!strcmp(p->name, name)) {
            err = p->ops->open(name, &device, arg);
            if (err) {
                return NULL;
            }
            device->ops = p->ops;
            atomic_inc(&device->ref);
            return device;
        }
    }
    return NULL;
}

bool dev_online(const char *name)
{
    const struct dev_node *p;
    list_for_each_device(p) {
        if (!strcmp(p->name, name)) {
            if (p->ops->online) {
                return p->ops->online(p);
            }
            break;
        }
    }
    return false;
}

int dev_seek(void *_device, u32 offset, int orig)
{
    int ret = 0;
    struct device *device = (struct device *)_device;
    if (device->ops->seek) {
        ret = device->ops->seek(device, offset, orig);
    }
    return ret;
}

int dev_ioctl(void *_device, int cmd, u32 arg)
{
    struct device *device = (struct device *)_device;
    if (device->ops->ioctl) {
        return device->ops->ioctl(device, cmd, arg);
    }
    return 0;
}

int dev_close(void *_device)
{
    struct device *device = (struct device *)_device;
    int res = atomic_sub_return(1, &device->ref);
    /* if (atomic_dec_and_test(&device->ref)) { */
    if (0 == res) {
        if (device->ops->close) {
            return device->ops->close(device);
        }
    }
    return res;
    /* return 0;//atomic_return(&device->ref); */
}

int dev_bulk_read(void *_device, void *buf, u32 sector, u32 sector_num)
{
    int rlen = 0;
    struct device *device = (struct device *)_device;

    if (device->ops->bulk_read) {
        rlen = device->ops->bulk_read(device, buf, sector_num, sector);
    }
    return rlen;
}

int dev_bulk_write(void *_device, void *buf, u32 sector, u32 sector_num)
{
    int wlen = 0;
    struct device *device = (struct device *)_device;
    if (device->ops->bulk_write) {
        wlen = device->ops->bulk_write(device, buf, sector_num, sector);
    }
    return wlen;
}

int dev_byte_read(void *_device, void *buf, u32 offset, u32 len)
{
    int rlen = 0;
    struct device *device = (struct device *)_device;
    if (device->ops->read) {
        rlen = device->ops->read(device, buf, len, offset);
    }
    return rlen;
}

int dev_byte_write(void *_device, void *buf, u32 offset, u32 len)
{
    int wlen = 0;
    struct device *device = (struct device *)_device;
    if (device->ops->write) {
        wlen = device->ops->write(device, buf, len, offset);
    }
    return wlen;
}

int devices_init()
{
    int err;
    const struct dev_node *p;
    const struct device_operations *ops;
    list_for_each_device(p) {
        ops = p->ops;
        if (ops && ops->init) {
            err = ops->init(p, p->priv_data);
            if (err) {
                /* ASSERT(0, "device: %s init_err: %d\n", p->name, err); */
            }
        }
    }
    return 0;
}

struct dev_node_mg *set_device_node(struct dev_node *node_start, struct dev_node *node_end)
{
    struct dev_node_mg *node;
    node = &device_node;
    node->device_node_begin = node_start;
    node->device_node_end = node_end;
    return node;
}


