#include "header.h"

int my_open(struct inode *ino, struct file *filp)
{
	pr_notice("my_open() is called\n");
	dump_stack();
	pr_notice("-----------my_open() end-------------\n");
	return 0;
}
