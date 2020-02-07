#include "header.h"
#include "declaration.h"

#define MAGIC_NO 'x'

#define WR_VALUE _IOW(MAGIC_NO, 'a', int32_t*)
#define RD_VALUE _IOR(MAGIC_NO, 'b', int32_t*)

struct file_operations f_ops = {
	.open = ioctl_open,
	.read = ioctl_read,
	.write = ioctl_write,
	.unlocked_ioctl = my_ioctl
	//release: ioctl_release,
};

dev_t device_major;
struct class *device_class;
struct cdev c_dev;
struct device *device_dev;
int32_t value;



static int __init ioctl_driver_init(void)
{
	pr_notice("Registering IOCTL driver\n");

	int ret = 0;

	ret = alloc_chrdev_region(&device_major,0,1,DEVICE_NAME);
	if (unlikely(ret < 0)) {
		pr_err("failed to register major no: %d\n",device_major);
		return -1;
	}
	
	pr_notice("allocated device on major no : %d\n",MAJOR(device_major));
	
	cdev_init(&c_dev,&f_ops);
	ret = cdev_add(&c_dev,device_major,1);
	if (unlikely(ret < 0)) {
		pr_err("cdev_add failed to add char device\n");
		return -1;
	}
	
	device_class = class_create(THIS_MODULE,"ioctl_class");
	device_dev = device_create(device_class, NULL, device_major, NULL, "ioctlDevice");
	if (unlikely(device_dev < 0)) {
		pr_err("failed to create char device and registers with sysfs\n");
		return -1;
	}
	
	pr_notice("device class successfully created\n");

	return 0;
}


static long my_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{

	switch(cmd) {
		case WR_VALUE:
				copy_from_user(&value,(int32_t *)arg, sizeof(value));
				pr_notice("value read from userspace = %d\n",value);
				break;
		case RD_VALUE:
				//value += 10;
				copy_to_user((int32_t*)arg,&value,sizeof(value));
				break;
	}

	return 0;
}


module_init(ioctl_driver_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gullu Don MOGEE vala");
MODULE_DESCRIPTION("IOCTL implementation");
