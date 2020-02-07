/*  File: bbbgpio_open.h
 *  Author: Vishal Chopra
 *  Date: 4 Dec 2016
 *  Purpose: This file is open the character driver.
*/

#include "header.h"
#include "declaration.h"

int bbbgpio_open(struct inode *inode, struct file *file)
{
	driver_info("%s OPEN\n", DRIVER_NAME);
	if(mutex_trylock(&bbbgpio_device_Ptr->io_mutex) == 0){
		driver_err("%s Unable to aquire mutex\n", DRIVER_NAME);
		return -EBUSY;
	}	
	if(bbbgpio_device_Ptr->is_open == 1){
		driver_err("%s Device is already open\n", DRIVER_NAME);
		return -EBUSY;
	}

	bbbgpio_device_Ptr->is_open = 1;
	mutex_unlock(&bbbgpio_device_Ptr->io_mutex);
	driver_info("%s Driver is opened Sucessfully\n", DRIVER_NAME);
	return 0;
}
