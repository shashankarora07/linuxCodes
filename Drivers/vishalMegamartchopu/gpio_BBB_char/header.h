/** File: header.h
  * Author: Vishal Chopra
  * Date: 20 Nov 2016
  * Brief: Include all the header files 
*/


#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio.h>
/*
  ===================================
  DRIVER INFO
  ==================================
*/

#define DRIVER_AUTHOR "Vishal Chopra"
#define DRIVER_DESC "GPIO driver for BBB"
#define DRIVER_NAME "bbbgpio"
#define DEVICE_CLASS_NAME "bbbgpio_class_new"
#define DEVICE_PROCESS "bbbgpio%d"
/*
  ===================================
  DRIVER DEBUG
  ===================================
*/

#ifdef DEBUGGING

#define DEBUGGING_DEBUG KERN_DEBUG
#define DEBUGGING_INFO KERN_INFO
#define DEBUGGING_ERR KERN_ERR
#define DEBUGGING_WARNING KERN_WARNING

#define driver_dbg(format, arg...) do{ DEBUGGING_DEBUG printk( format, ##arg );} while(0)
#define driver_info(format, arg...) do{DEBUGGING_INFO printk( format, ##arg );} while(0)
#define driver_err(format, arg...) do{DEBUGGING_ERR printk( format, ##arg );} while(0)
#define driver_warn(format, arg...) do{DEBUGGING_WARNING printk( format, ##arg );} while(0)

#else

#define driver_dbg(format, arg...) 
#define driver_info(format, arg...) do{printk( KERN_INFO format, ##arg );} while(0)
#define driver_err(format, arg...) do{printk( KERN_ERR format, ##arg );} while(0)
#define driver_warn(format, arg...) do{printk( KERN_WARNING format, ##arg );} while(0)

#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("VISH");
MODULE_DESCRIPTION(DRIVER_DESC);

