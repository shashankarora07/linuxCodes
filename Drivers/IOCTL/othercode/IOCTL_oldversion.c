/*
 * This program implements the basic functionalities of the a char driver.....................
 *
 * Santosh Sam Koshy
 * Member Technical Staff
 * C-DAC, Hyderabad
 *
 */

//Initialization Functions...........
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include "ioctl.h"
#include <linux/timer.h>
#include <linux/param.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

#define NAME SAM //Create a Device Number that can be used by the applications as well....
#define SUCCESS 1
static unsigned int gpioLED = 49;
static bool gpioVal = 0;
dev_t Mydev;
//Function Prototypes
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
//ssize_t NAME_write(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp);
//ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t  *offp);
long NAME_ioctl (struct file * filp, unsigned int cmd, unsigned long arg);
void MyKernelFunction(unsigned long data);


//Structure that defines the operations that the driver provides
struct file_operations fops = 
{
	.owner   = THIS_MODULE,
	.open    = NAME_open, 
	//.read    = NAME_read,
	//.write   = NAME_write,
	.release = NAME_release,
	.unlocked_ioctl	 = NAME_ioctl,
};

struct DefaultData {
	int Baudrate;
	int Parity;
	int StopBits;
	int ToggleFreq;
};

//Structure for a character driver
struct cdev *my_cdev;
struct timer_list MyKernelTimer;


//Init Module
static int __init CharDevice_init(void)
{
	int result;
	int MAJOR,MINOR;

	//Mydev = MKDEV(255,0);//Create a device number
	result=alloc_chrdev_region(&Mydev,0,1,"MyCharDevice");//register device region.....
	if(result<0)
	{
		printk(KERN_ALERT "\nThe Region requested for is not obtainable\n");
		return(-1);
	}
	
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nThe Major Number is %d...THe Minor Number is %d\n",MAJOR,MINOR);
	my_cdev = cdev_alloc();//allocate memory to Char Device structure
	my_cdev->ops = &fops;//link our file operations to the char device

	result=cdev_add(my_cdev,Mydev,1);//Notify the kernel abt the new device
	if(result<0)
	{
		printk(KERN_ALERT "\nThe Char Devide has not been created......\n");
		return (-1);
	}
	//printk(KERN_ALERT "\nThis is the Kernel....Open System Call.....\n");
  //printk(KERN_ALERT "\nThis is the KErnel Open Call\n");
	init_timer(&MyKernelTimer);

	if (!gpio_is_valid(gpioLED)){
      		printk(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
      		return -ENODEV;
   	}
	gpio_request(gpioLED, "sysfs");          // gpioLED is hardcoded to 49, request it
	gpio_direction_output(gpioLED, 1);   // Set the gpio to be in output mode and on
	// gpio_set_value(gpioLED, ledOn);          // Not required as set by line above (here for reference)
	gpio_export(gpioLED, false);


	return 0;
}


//Cleanup Module
void __exit CharDevice_exit(void)
{
	//dev_t Mydev;
	int MAJOR,MINOR;
	//Mydev=MKDEV(255,0);
	MAJOR=MAJOR(Mydev);
	MINOR=MINOR(Mydev);
	printk("\nThe Major Number is %d...THe Minor Number is %d\n",MAJOR,MINOR);
	unregister_chrdev_region(Mydev,1);			//unregister the device numbers and the device created
	cdev_del(my_cdev);
	del_timer(&MyKernelTimer);
  	gpio_unexport(gpioLED);

	printk(KERN_ALERT "\nI have unregistered the stuff that was allocated.....Goodbye for ever.....\n");
	return;
}


//Open System Call
int NAME_open(struct inode *inode, struct file *filp)
{
	unsigned long myjiffies, mydelay;

	myjiffies=get_jiffies_64();
	mydelay = myjiffies+HZ;
	MyKernelTimer.expires = mydelay;
	MyKernelTimer.function = MyKernelFunction;
	MyKernelTimer.data = 1;
  	add_timer(&MyKernelTimer);
	
  	printk(KERN_ALERT "\nThis is the KErnel Open Call. Jiffies: %ld, Jiffies Expiry: %ld\n", myjiffies, mydelay);
	return 0;
}

//Close System Call
int NAME_release(struct inode *indoe, struct file *filp)
{
	printk(KERN_ALERT "\nThis is the release method of my Character Driver......Bye Dudes......\n");
	return 0;
} 

long NAME_ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
{
	unsigned long myjiffies, mydelay;
	unsigned int CMD;
	ssize_t retval = SUCCESS;
	char UBuff[20];
	struct DefaultData *ToSend;
	get_user(CMD, &cmd);
	printk ("\n IOCTL function: %d",cmd);
	switch (cmd) {
		case SET_BAUD_RATE:
			//get_user (temp, &arg);
			printk ("\n Setting the Baud Rate to %ld ", arg);
			break;
		case SET_DIRECTION_WRITE:
			printk ("\n The Direction is set to write");
			break;
 		case SET_NO_STOP_BITS:
			//get_user (STOP_BITS, &arg);
			printk ("\n Setting the Num of Stop bits to %ld", arg);
			break;
		case DEFAULT_DATA:
			ToSend = (struct DefaultData *)UBuff;
			copy_from_user(UBuff, (struct DefaultData *)arg, sizeof (struct DefaultData));
			printk ("Default Data is %d", ToSend->ToggleFreq);
			MyKernelTimer.data = ToSend->ToggleFreq;
			myjiffies = get_jiffies_64();
			mydelay = myjiffies + (ToSend->ToggleFreq*HZ);
			mod_timer(&MyKernelTimer,mydelay);
			break;
		default:
			printk ("\nCommand Not Found");
			return (-EINVAL);
	}
	return (retval);
}
	
void MyKernelFunction(unsigned long data)
{
	unsigned long myjiffies, mydelay; 
	
	
        
	gpioVal = !gpioVal;
	gpio_set_value (gpioLED, gpioVal);
	
	//if(flag==0)

        //mydelay=jiffies+2*HZ;
	  printk(KERN_ALERT "Kernel Timer Function. Current Jiffies is %lu\n",(unsigned long)get_jiffies_64());
	//MyKernelTimer.function=MyKernelFunction;
	//MyKernelTimer.data=data;
	 // MyKernelTimer.expires=jiffies + (data*HZ);
	myjiffies = get_jiffies_64();
	mydelay = myjiffies+ data*HZ;
	MyKernelTimer.expires = mydelay;
  add_timer(&MyKernelTimer);
}

	
//Module over ride functions
module_init(CharDevice_init);
module_exit(CharDevice_exit);
