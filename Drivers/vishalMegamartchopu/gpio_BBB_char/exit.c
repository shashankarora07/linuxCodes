/** File: exit.c
  * Author: Vishal Chopra
  * Date: 20NOV2016
  * Brief: This function is to unregister all the devices and memory allocation.
*/


#include "header.h"
#include "declaration.h"
static void __exit bbbgpio_exit(void)
{
	if(bbbgpio_device_Ptr != NULL){
		device_destroy(bbbclass_Ptr, MKDEV(MAJOR(bbb_dev), 0));
		cdev_del(&bbbgpio_device_Ptr->cdev);
		kfree(bbbgpio_device_Ptr);
		bbbgpio_device_Ptr = NULL;
	}
	
	if(bbbclass_Ptr != NULL)
	{	
       		class_destroy(bbbclass_Ptr);
		bbbclass_Ptr = NULL;
	}

        unregister_chrdev_region(bbb_dev, 1);
	driver_info("Driver is removed from the kernel on %s at %s\n", __DATE__, __TIME__);
}
module_exit(bbbgpio_exit)
