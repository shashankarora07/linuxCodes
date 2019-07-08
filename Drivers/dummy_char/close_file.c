#include "header.h"

int my_close(struct inode *ino, struct file *filp)
{
	pr_notice("my_close() is called\n");
    dump_stack();
    pr_notice("-----------my_close() end-------------\n");
	return 0;
}
