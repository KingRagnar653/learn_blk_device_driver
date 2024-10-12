/*
    Linux kernel provide a whole different subsystem called
        block I/O (block layer) subsystem
    register_blkdev Not allocated disk to the system
        genhd.h -> allocate and add and delete disk(device) to the system
        * add_disk (even during the call), the methods will be active (so called after initialisation)
*/
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h> 

#define BLK_NAME "blk_rs_device"
#define BLOCK_MINOR 1
int major_num;

/*
 * to store all elements describing block device
 */
static struct my_blk_dev {
    struct gendisk *gd;
} dev;

void create_disk(struct my_blk_dev *dev) {
    dev->gd = alloc_disk(BLOCK_MINOR);
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