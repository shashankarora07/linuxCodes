#include<linux/init.h>
#include<linux/interrupt.h>
#include<linux/gpio.h>
#include<linux/kernel.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A Button/Led test driver for BBB");
MODULE_AUTHOR("GULLUDON");


#ifndef DEBUG
#define DEBUG
#endif
