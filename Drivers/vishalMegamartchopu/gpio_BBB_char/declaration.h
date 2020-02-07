/** File: declaration.h
  * AUTHOR: Vishal Chopra
  * DATE 20 NOV 2016
  * Brief: This file contain all the declaration of the variable.
*/


struct bbbgpio_device{
	struct cdev cdev;
	struct device *device_Ptr;
	struct mutex io_mutex;
	u8 is_open;
};

extern struct bbbgpio_device *bbbgpio_device_Ptr;
extern struct class *bbbclass_Ptr;
extern dev_t bbb_dev;
int bbbgpio_open(struct inode *, struct file *);
extern long bbbgpio_ioctl(struct file *, unsigned int, unsigned long);
