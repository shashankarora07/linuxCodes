/* driver init file */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
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
//static int kread, kwrite_nrm;
static atomic_t kval;

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

static ssize_t bytes_read_kmalloc(struct device *dev, struct device_attribute *attr, 
			   char *buff)
{
	/* for testing purpose */
	MSG("started");
	int temp = 0;

	//MSG("kwrite = %d, kread = %d \n",kwrite,kread);
	MSG("kval = %d \n",kval);
	//atomic_set(&kval,temp);
	//MSG("kval = %d and temp = %d\n",kval,temp);

	return sprintf(buff, "%d\n", atomic_read(&kval));

}

static ssize_t bytes_allocate_thr_kmalloc(struct device *dev, struct device_attribute *attr,
				const char *buff, size_t count)
{
	/* for testing purpose */
	MSG("started");
	MSG("buff = %s",buff);
	int temp = 0;

	sscanf(buff, "%d", &temp);
	atomic_set(&kval,temp);

	//kwrite_nrm = (int*)&kwrite;
	//return sprintf(buff, "%d\n", kwrite);
	MSG("kval = %d, temp = %d \n",kval,temp);

	return temp;
}

static struct file_operations device_fops = {
        .open = my_open,
        .release = my_close,
        .read = my_read,
        //.write = my_write,
        .llseek = no_llseek,
};

/** #define DEVICE_ATTR(_name, _mode, _show, _store) \
 *     struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)
**/
/** // interface for exporting device attributes
struct device_attribute {
    struct attribute    attr;
    ssize_t (*show)(struct device *dev, struct device_attribute *attr,
            char *buf);
    ssize_t (*store)(struct device *dev, struct device_attribute *attr,
             const char *buf, size_t count);
};
**/
#undef VERIFY_OCTAL_PERMISSIONS
#define VERIFY_OCTAL_PERMISSIONS(perms) (perms)

static DEVICE_ATTR(kmalloc_test, S_IRUGO | S_IWUGO, bytes_read_kmalloc, bytes_allocate_thr_kmalloc);
//static DEVICE_ATTR(kmalloc_test_read, S_IRUSR, bytes_read_kmalloc, NULL);
//static DEVICE_ATTR(kmalloc_test_write, S_IWUSR, NULL, bytes_allocate_thr_kmalloc);
//static DEVICE_ATTR(kmalloc_test_read, S_IRUGO, bytes_read_kmalloc, NULL);
//static DEVICE_ATTR(kmalloc_test_write, S_IWUGO, NULL, bytes_allocate_thr_kmalloc);


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
	if (IS_ERR(device_class)) {
		MSG("failed to register device class '%s'\n",DRVCLASSNAME);
		ret = PTR_ERR(device_class);
		goto failed_classreg;
	}


	for (i = 0; i < DEVICE_COUNT; ++i) {
		device_dev = device_create(device_class, NULL, MKDEV(MAJOR(device_number),
			MINOR(device_number)+i), NULL, "%s_%d", DRVNAME,i);

		if (unlikely(device_dev < 0)) {
	    	pr_err("%s: failed to create device node /dev/%s.%d !\n",
	    		DRVNAME, DRVNAME,i);
	    	return -1; 
		} else {
			MSG("%s: Created device file :/dev/%s.%d\n",DRVNAME,DRVNAME,i);
		}
	}

	ret = device_create_file(device_dev,&dev_attr_kmalloc_test);
	if (ret < 0) {
		MSG("failed to create write /sys endpoint - continuing without\n");
	}
/*
	ret = device_create_file(device_dev,&dev_attr_kmalloc_test_read);
	if (ret < 0) {
		MSG("failed to create write /sys endpoint - continuing without\n");
	}

	ret = device_create_file(device_dev,&dev_attr_kmalloc_test_write);
	if (ret < 0) {
		MSG("failed to create write /sys endpoint - continuing without\n");
	}
*/

failed_classreg: 
	unregister_chrdev_region(device_number, DEVICE_COUNT);

	return 0;
}

static void __exit mychar_exit(void)
{
	int i = 0;

	MSG("allocated memory about to destroy now..\n");

	device_remove_file(device_dev, &dev_attr_kmalloc_test);
	//device_remove_file(device_dev, &dev_attr_kmalloc_test_read);
	//device_remove_file(device_dev, &dev_attr_kmalloc_test_write);
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
