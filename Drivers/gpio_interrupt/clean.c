#include"header.h"
#include"declaration.h"

static void __exit gpio_exit(void){
	
	#ifdef DEBUG
	printk(KERN_INFO "In the end: Button state : %d\n",gpio_get_value(gpioButton));
	printk(KERN_INFO "In the end Button was pressed : %d times\n",*buttonPress);
	#endif
	
	gpio_set_value(gpioLed, 0);   // Turn off Led
	gpio_unexport(gpioLed);	      // Remove from sysfs
	free_irq(irqno,NULL);         // free the IRQ no.
	gpio_unexport(gpioButton);	
	gpio_free(gpioLed);           //  Free the Led pin
	gpio_free(gpioButton);	
	
	#ifdef DEBUG
	printk(KERN_INFO "GPIO_TEST: GoodBye from the kernel\n");
	#endif
		
}

module_exit(gpio_exit);
