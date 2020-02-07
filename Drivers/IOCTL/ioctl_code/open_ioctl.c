#include "header.h"
#include "declaration.h"

int ioctl_open(struct inode *ino, struct file *f)
{
	pr_notice("ioctl_open is called\n");
	dump_stack();
	pr_notice("ioctl_open ---------------- end\n");

	return 0;
}
