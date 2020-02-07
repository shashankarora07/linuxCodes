#include "header.h"
#include "declaration.h"


static void __exit ioctl_driver_exit(void) 
{
	pr_notice("Successfully memory allocated destoryed\n");
	device_destroy(device_class,device_major);
	class_destroy(device_class);
	cdev_del(&c_dev);
	unregister_chrdev_region(device_major,1);

	pr_notice("driver out\n");
}

module_exit(ioctl_driver_exit);
