#ifndef BBBGPIO_IOCTL_H
#define BBBGPIO_IOCTL_H

struct bbbgpio_ioctl_struct{
	int gpio_number:7;
	int gpio_direction:1;
	int write_buffer:8;
	int read_buffer:8;
	int irq_number;
};

#define _IOCTL_MAGIC 'K'

#define IOCBBBGPIORP _IOW(_IOCTL_MAGIC, 0, struct bbbgpio_ioctl_struct *)

#endif
