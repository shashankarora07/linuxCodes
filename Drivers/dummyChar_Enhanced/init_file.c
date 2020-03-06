/* driver init file */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRVNAME "myCharDevice"
#define DEVICE_COUNT 1

static int my_open(struct inode *ino, struct file *filp)
{
	MSG( "Device node with minor # %d being used\n", iminor(ino));

	pr_notice("my_open() is called\n");
	dump_stack();
	pr_notice("-----------my_open() end-------------\n");
	return 0;
}

static struct file_operations device_fops = {
        .open = my_open,
        .release = my_close,
        .read = my_read,
        .write = my_write,
        .llseek = no_llseek,
};

//static struct file_operations

dev_t device_number;
struct cdev c_device;
struct class *device_class;
struct device *device_dev;


static int __init mychar_init(void) //constructor
{
	printk(KERN_INFO "driver init in happening");
	
	int ret = 0;

	/* device_number = MKDEV(0,0);  Combination of major and minor no */

	ret = alloc_chrdev_region(&device_number, 0, 1, DRVNAME);
	if (unlikely(ret < 0)) {
	    pr_err("failed to register on major no : %d\n", device_number);
	    return -1;
	}
	pr_notice("Allocated device on major number : %d\n", MAJOR(device_number));

	cdev_init(&c_device,&device_fops);
	ret = cdev_add(&c_device, device_number, 1);
	if (unlikely(ret < 0)) {
	    pr_err("failed to add character device : %d\n", device_number);
	    return -1;
	}
	
	device_class = class_create(THIS_MODULE, "char_device_class");
	device_dev = device_create(device_class, NULL, device_number, NULL, "chardevice0");
	if (unlikely(device_dev < 0)) {
	    pr_err("failed to create character device class \n");
	    return -1; 
	}
	pr_notice("Created device driver and device file successfully \n");

	return 0;
}
module_init(mychar_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shashank");
MODULE_DESCRIPTION("Dummy Driver");
