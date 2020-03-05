#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#define MAJOR_NO 100

extern dev_t device_major;


int my_open(struct inode *, struct file *);
int my_close(struct inode *, struct file *);
ssize_t my_read(struct file *, char __user *, size_t , loff_t *);
ssize_t my_write(struct file *, const char __user *, size_t , loff_t *);


