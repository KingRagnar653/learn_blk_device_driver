/*
    Linux kernel provide a whole different subsystem called
        block I/O (block layer) subsystem
*/
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h> 

#define BLK_NAME "blk_rs_device"
int major_num;


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
    return 0;
}

static void __exit blk_exit(void) {
    if (major_num) {
        unregister_blkdev(major_num, BLK_NAME);
    }
}

MODULE_AUTHOR("Rahul R S");
MODULE_DESCRIPTION("simple block device driver");
MODULE_LICENSE("GPL");

module_init(blk_init);
module_exit(blk_exit);