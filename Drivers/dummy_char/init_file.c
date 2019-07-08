/* driver init file */

#include "header.h"
#include "declaration.h"

struct file_operations device_fops = {
        open: my_open,
        release: my_close,
        read: my_read,
        write: my_write
};

dev_t device_major;
struct cdev c_device;
struct class *device_class;
struct device *device_dev;


static int __init mychar_init(void) //constructor
{
	printk(KERN_INFO "driver init in happening");
	
	int ret = 0;

	/* device_major = MKDEV(0,0);  Combination of major and minor no */

	ret = alloc_chrdev_region(&device_major, 0, 1,"my_char_device");
	if (unlikely(ret < 0)) {
	    pr_err("failed to register on major no : %d\n", device_major);
	    return -1;
	}
	pr_notice("Allocated device on major number : %d\n", MAJOR(device_major));

	cdev_init(&c_device,&device_fops);
	ret = cdev_add(&c_device, device_major, 1);
	if (unlikely(ret < 0)) {
	    pr_err("failed to add character device : %d\n", device_major);
	    return -1;
	}
	
	device_class = class_create(THIS_MODULE, "char_device_class");
	device_dev = device_create(device_class, NULL, device_major, NULL, "chardevice0");
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
