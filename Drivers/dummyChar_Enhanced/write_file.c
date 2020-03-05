#include "header.h"

ssize_t my_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
        pr_notice("my_write() is called\n");
        dump_stack();
        pr_notice("-----------my_write() end-------------\n");
        return len;
}

