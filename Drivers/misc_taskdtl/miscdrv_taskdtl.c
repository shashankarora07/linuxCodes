/* driver init file */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>

#ifdef DEBUG_PRINT
#define MSG(string, args...)      \
	pr_info("%s: %s-%d: " string, \
		MISCDRVNAME, __FUNCTION__, __LINE__, ##args)
#else
	#define MSG(string, args...)
#endif


#define MISCDRVNAME "miscdrv"
#define MISCDEVNAME "taskdtl_miscdev"
#define DBGDIRNAME "disp_task"
#define DBGFILENAME1 "disp_taskInfo"

static struct device *device_ptr;
static struct dentry *parent;

typedef struct taskdetail {
	u32 pidinfo;
} taskdtl;

static taskdtl *taskdtl_ptr;

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

static ssize_t debug_taskinfo_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{

	return len;
}

static ssize_t debug_taskinfo_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{

}

static struct file_operations device_fops = {
        .open = my_open,
        .release = my_close,
        .read = my_read,
        //.write = my_write,
        .llseek = no_llseek,
};

static struct file_operations debug_fops = {
	.read = debug_taskinfo_read,
	.write = debug_taskinfo_write,
};

static struct miscdevice taskdtl_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISCDEVNAME,
	.mode = 0666,
	.fops = &device_fops,
};




static int __init miscdrv_init(void) //constructor
{	
	int ret = 0;
	//struct dentry *dbgfile;
	
	MSG("%s: .....misc char driver init....\n", MISCDRVNAME);

	/* device_number = MKDEV(0,0);  Combination of major and minor no */
	parent = debugfs_create_dir(DBGDIRNAME,NULL);
	if (!parent) {
		pr_err("debugfs_create_dir: failed to create /sys/kernel/debug/disp_task\n");
		if (PTR_ERR(parent) == -ENODEV)
			pr_info("debugfs support not available?\n");
		ret = PTR_ERR(parent);
		goto DBGFS_ERR;
	}

	taskdtl_ptr = (taskdtl *)kmalloc (sizeof(taskdtl), GFP_KERNEL);
	if (!taskdtl_ptr) {
		pr_err("%s: kmalloc failed!\n", MISCDRVNAME);
		ret = -ENOMEM;
		goto OUT;
	}
	taskdtl_ptr->pidinfo = 0;

	if (!debugfs_create_file(DBGFILENAME1, 0666, parent, (void *)&taskdtl_ptr, &debug_fops)) {
		pr_err("debugfs_create_file: failed to create /sys/kernel/debug/%s/%s\n", \
			DBGDIRNAME,DBGFILENAME1);
		ret = PTR_ERR(parent);
		goto DBGFS_ERR;
	}

	ret = misc_register(&taskdtl_dev);
	if (unlikely(ret)) {
		pr_err("%s: could not register the misc device mydev", MISCDRVNAME);
		return ret;
	}

	device_ptr = taskdtl_dev.this_device;
	MSG("device ptr = %pK; device node: /dev/%s\n",device_ptr,MISCDEVNAME);
	MSG("Created Debugfs entries successfully\n");


OUT: 
	return ret;

DBGFS_ERR:
	debugfs_remove_recursive(parent);
	return ret;

	return 0;
}

static void __exit miscdrv_exit(void)
{
	MSG("misc char driver exit\n");

	/* Unregister the device with the kernel */
	debugfs_remove_recursive(parent);
	misc_deregister(&taskdtl_dev);

	MSG("driver is unregistered. Done!\n");
}


module_init(miscdrv_init);
module_exit(miscdrv_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shashank");
MODULE_DESCRIPTION("misc Dummy Driver");
