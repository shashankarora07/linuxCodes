#include "header.h"

ssize_t my_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
        pr_notice("my_read() is called\n");
        dump_stack();
        pr_notice("-----------my_read() end-------------\n");
        return 0;
}
