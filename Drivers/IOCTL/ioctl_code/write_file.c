#include "header.h"
#include "declaration.h"


ssize_t ioctl_write(struct file *fp, const char __user *buff, size_t len, loff_t *off)
{
	pr_notice("ioctl_write() is called\n");
	dump_stack();
	pr_notice("....ioctl_write().....end\n");

	return 0;
}
