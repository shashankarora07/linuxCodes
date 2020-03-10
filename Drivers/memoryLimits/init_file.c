/* driver init file */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#ifdef DEBUG_PRINT
#define MSG(string, args...)      \
	pr_info("%s: %s-%d: " string, \
		DRVNAME, __FUNCTION__, __LINE__, ##args)
#else
	#define MAG(string, args...)
#endif


#define DRVNAME "myCharDevice"
#define DRVCLASSNAME "myCharDevice_Class"
#define DEVICE_COUNT 1

struct chardrv_dev {
	char name[10];
	struct cdev cdev;
} *c_device;

dev_t device_number;

//struct cdev c_device;
static struct class *device_class = NULL;
static struct device *device_dev;

static int my_open(struct inode *ino, struct file *filp)
{
	MSG("Device node with minor # %d being used\n", iminor(ino));

	MSG("my_open() is called\n");

	switch (iminor(ino)) {
		case 0:
				//filp->f_op = &device_fops;
				break;
		default:
				return -ENXIO;
	}

	dump_stack();
	MSG("-----------my_open() end-------------\n");
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
        //int mlen = len;
		int minsize = 1;
		//char *zbuf;
		int32_t totalsize = minsize * PAGE_SIZE;
        MSG("my_read() is called\n");
        //dump_stack();

        MSG("len received = %ld\n",len);
        
        if (len > PAGE_SIZE) {
        	if (len % PAGE_SIZE == 0) {
        		minsize = len / PAGE_SIZE;
        		totalsize = minsize * PAGE_SIZE;
        	} else {
        		minsize = len / PAGE_SIZE;
        		totalsize = (minsize * PAGE_SIZE) + PAGE_SIZE;
        	} 
        }

        MSG("totalsize = %d\n",totalsize);

  /*      if ( (zbuf=kzalloc (totalsize, GFP_KERNEL)) == NULL) {
        	pr_err("kzalloc failed");
        	return -ENOMEM;
        }
*/
        //if( copy_to_user((int32_t *)buff,&totalsize,sizeof(totalsize))){
       /* if (put_user (totalsize,buff)) {
        	pr_err("put_user failed\n");
        	return -EFAULT;
        }
*/
        MSG("-----------my_read() end-------------\n");
        return totalsize;
}

int my_close(struct inode *ino, struct file *filp)
{
	MSG("my_close() is called\n");
    dump_stack();
    MSG("-----------my_close() end-------------\n");
	return 0;
}

static struct file_operations device_fops = {
        .open = my_open,
        .release = my_close,
        .read = my_read,
        //.write = my_write,
        .llseek = no_llseek,
};

static DEVICE_ATTR(kmalloc_test,0660,);




static int __init mychar_init(void) //constructor
{	
	int ret = 0, i = 0;
	
	MSG("....INside driver init.....");

	/* device_number = MKDEV(0,0);  Combination of major and minor no */

	ret = alloc_chrdev_region(&device_number, 0, DEVICE_COUNT, DRVNAME);
	if (unlikely(ret < 0)) {
	    pr_err("failed to register on major no : %d\n", device_number);
	    return ret;
	} else {
		MSG("%s: registered with major number : %d\n", DRVNAME,MAJOR(device_number));
	}

	c_device = kzalloc(DEVICE_COUNT * sizeof(struct chardrv_dev), GFP_KERNEL);
	if (NULL == c_device) {
		pr_err("%s: kzalloc failed! \n",DRVNAME);
		return -ENOMEM;
	}

	for (i = 0; i < DEVICE_COUNT; ++i) {
		cdev_init(&c_device[i].cdev,&device_fops);
		c_device[i].cdev.owner = THIS_MODULE;
		c_device[i].cdev.ops = &device_fops;
		/* device_number = MKDEV(0,0);  Combination of major and minor no */
		ret = cdev_add(&c_device[i].cdev, MKDEV(MAJOR(device_number),
			MINOR(device_number)+i), 1);
		if (unlikely(ret < 0)) {
	    	pr_err("%s : failed to add character device : %d: ERROR:%d\n",
	    		DRVNAME, device_number,ret);
	    	return ret;
	    } else {
	    	MSG("%s cdev %s.%d added\n",DRVNAME, DRVNAME, i);
	    }
	}
	/* Create the devices.
	 * !Note! : APIs class_create, device_create, etc exported as EXPORT_SYMBOL_GPL(...); 
	 * so will not show up unless the module license is GPL.
	 */	

	device_class = class_create(THIS_MODULE, DRVCLASSNAME);

	for (i = 0; i < DEVICE_COUNT; ++i) {
		device_dev = device_create(device_class, NULL, MKDEV(MAJOR(device_number),
			MINOR(device_number)+i), NULL, "%s.%d", DRVNAME,i);

		if (unlikely(device_dev < 0)) {
	    	pr_err("%s: failed to create device node /dev/%s.%d !\n",
	    		DRVNAME, DRVNAME,i);
	    	return -1; 
		} else {
			MSG("%s: Created device file :/dev/%s.%d\n",DRVNAME,DRVNAME,i);
		}
	}

	device_create_file(device_dev,);

	return 0;
}

static void __exit mychar_exit(void)
{
	int i = 0;

	MSG("allocated memory about to destroy now..\n");
	for (i = 0; i < DEVICE_COUNT; ++i) {
		cdev_del(&c_device[i].cdev);
		device_destroy(device_class, MKDEV(MAJOR(device_number),
			MINOR(device_number)+i));
	}
	class_destroy(device_class);
	kfree(c_device);
	unregister_chrdev_region(device_number, DEVICE_COUNT);

	MSG("driver is unregistered. Done!\n");
}


module_init(mychar_init);
module_exit(mychar_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shashank");
MODULE_DESCRIPTION("Dummy Driver");
