#include "header.h"
#include "declaration.h"

static void __exit mychar_exit(void)
{
	pr_notice("allocated memory about to destroy\n");
	cdev_del(&c_device);
	device_destroy(device_class, device_major);
	class_destroy(device_class);
	unregister_chrdev_region(device_major, 1);

	pr_notice("driver is out\n");
}
module_exit(mychar_exit);
