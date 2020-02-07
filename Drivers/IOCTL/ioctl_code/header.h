#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "ioctl_chrDevice"
