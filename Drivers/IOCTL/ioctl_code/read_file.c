#include "header.h"
#include "declaration.h"


ssize_t ioctl_read(struct file *fp, char __user *buff, size_t len, loff_t *off)
{
	pr_notice("ioctl_read() is called\n");
	dump_stack();
	pr_notice("....ioctl_read().....end\n");

	return 0;
}
