#include "header.h"
#include "declaration.h"
#include "ioctl_driver.h"


long bbbgpio_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	struct bbbgpio_ioctl_struct *ioctl_user_struct, ioctl_struct;
	
	memset(&ioctl_struct, 0, sizeof(struct bbbgpio_ioctl_struct));

	if( mutex_trylock(&bbbgpio_device_Ptr->io_mutex) == 0 ){	// aquire lock for the ioctl operation
		driver_err("%s Mutex not free\n", DRIVER_NAME);
		return -EBUSY;
	}
	ioctl_user_struct = (struct bbbgpio_ioctl_struct *)ioctl_param;

	if( copy_from_user(&ioctl_struct, ioctl_user_struct, sizeof( struct bbbgpio_ioctl_struct ) ) ){
		driver_err("%s: unable to copy from user\n", DRIVER_NAME);
		return -EINVAL;		
	}	

	switch(ioctl_num){
	/*-----------------Case to register GPIO pin------------------*/
	case IOCBBBGPIORP:
	{
		gpio_request(ioctl_struct.gpio_number, "sysfs");
		
		if(ioctl_struct.gpio_direction == 0)
			gpio_direction_input(ioctl_struct.gpio_number); // for Push Button
		else
			gpio_direction_output(ioctl_struct.gpio_number, 1); // for LED
		gpio_export(ioctl_struct.gpio_number, 0);
		mutex_unlock(&bbbgpio_device_Ptr->io_mutex);
	}
	break;
	
	}
	return 0;
}
