struct file_operations fops={
	.open = bbbgpio_open,
	.unlocked_ioctl = bbbgpio_ioctl,
};
