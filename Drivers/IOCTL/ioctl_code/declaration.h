extern dev_t device_major;
extern struct cdev c_dev;
extern struct class *device_class;
extern struct device *device_dev;

int ioctl_open(struct inode *, struct file *);
ssize_t ioctl_read(struct file *, char __user *, size_t, loff_t *);
ssize_t ioctl_write(struct file *, const char __user *, size_t, loff_t *);
static long my_ioctl(struct file *, unsigned int, unsigned long);
