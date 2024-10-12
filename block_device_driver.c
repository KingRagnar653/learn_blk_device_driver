/*
    Linux kernel provide a whole different subsystem called
        block I/O (block layer) subsystem
    register_blkdev Not allocated disk to the system
        genhd.h -> allocate and add and delete disk(device) to the system
        * add_disk (even during the call), the methods will be active (so called after initialisation)
    blk_queue_logical_block_size() to inform kernel of different sector size , although communication btw device driver still be 512 sector size
*/
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h> 
#include<linux/gendisk.h>
#include<linux/blkdev.h>

#define BLK_NAME "blk_rs_device"
#define BLOCK_MINOR 1
#define NR_SECTORS 1024

int major_num;

/*
 * to store all elements describing block device
 */
static struct my_blk_dev {
    spinlock_t lock;
    struct gendisk *gd;
    struct request_queue *queue;
} dev;

int my_block_open(struct block_device *bdev, fmode_t mode) {
    printk("OPEN: blk device open\n");
    return 0;
}

int my_block_release(struct gendisk *gd, fmode_t mode) {
    printk("RELEASE: blk device released\n");
    return 0;
}

struct block_device_operations my_block_ops = {
    .owner = THIS_MODULE,
    .open = my_block_open,
    .release = my_block_release
};

void create_disk(struct my_blk_dev *dev) {
    dev->gd = alloc_disk(BLOCK_MINOR);
    /*
     * Fill the important fields for gendisk
     */
    dev->gd->first_minor = 0;
    dev->gd->major = major_num;
    dev->gd->fops = &my_block_ops;
    dev->gd->queue = dev->queue;
    dev->gd->private_data = dev;
    snprintf(dev->gd->name, 32, "rsblock");
    set_capacity(dev->gd, NR_SECTORS);

    add_disk(dev->gd);
}

void delete_disk(struct my_blk_dev *dev) {
    del_gendisk(dev->gd);
}


static int __init blk_init(void) {
    /*
     * register blk device to a unused major number
     */
    major_num = register_blkdev(0, BLK_NAME);
    if (major_num < 0) {
        printk("INIT: unable to register blk device\n");
        return -EBUSY;
    }
    printk("INIT: major_num %d\n", major_num);
    
    create_disk(&dev);
    return 0;
}

static void __exit blk_exit(void) {
    if (major_num) {
        delete_blk_device(&dev);
        unregister_blkdev(major_num, BLK_NAME);
    }
}

MODULE_AUTHOR("Rahul R S");
MODULE_DESCRIPTION("simple block device driver");
MODULE_LICENSE("GPL");

module_init(blk_init);
module_exit(blk_exit);