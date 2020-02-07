#include"header.h"
#include"declaration.h"

unsigned int irqno;
unsigned int gpioLed = 49;
unsigned int gpioButton = 115;
static unsigned int numPresses = 0;
unsigned int *buttonPress = &numPresses;
bool LedStatus = 0;
char gpio_device[] = "gpio_handler";


static int __init gpio_init(void){

	int result = 0;
	
	#ifdef DEBUG		
	printk(KERN_INFO "GPIO_TEST Initialization Begin\n");
	#endif

	if(!gpio_is_valid(gpioLed)){
		printk(KERN_INFO "gpioLed is invalid\n");
		return -ENODEV;
	}
	
	LedStatus = 1;
	gpio_request(gpioLed, "sysfs");   // to allocate the gpio number, label should be in sysfs 	
	gpio_direction_output(gpioLed, LedStatus);
	gpio_export(gpioLed, 0);
	
	// Now allocate the button in sys
	gpio_request(gpioButton, "sysfs");		
	gpio_direction_input(gpioButton);    // button to be as input
	gpio_set_debounce(gpioButton, 200);  // It is important to set debounce in button to prevent it from false alarm, kind of delay bewteen 2 times press.
	gpio_export(gpioButton, 0);
	
	#ifdef DEBUG
	printk(KERN_INFO "Button status: %d\n", gpio_get_value(gpioButton));
	#endif	

	irqno = gpio_to_irq(gpioButton);     // from this function we get the unique interrupt no. associated with gpio button, we often called Interrupt request line (IRQ).
	
	#ifdef DEBUG
	printk(KERN_INFO "Button is mapped with IRQ no. = %d\n",irqno);
	#endif
	
	result = request_irq(irqno, gpio_irq_handler, IRQF_TRIGGER_RISING, gpio_device, NULL);      //  It will enable a given interrupt line for handling the handler function with request_irq(). basically it will register an interrupt handler. 	
	
	#ifdef DEBUG
	printk(KERN_INFO "The interrupt request result is %d\n",result);
	#endif
	
	return 0;	
}

irq_handler_t gpio_irq_handler(unsigned irq, void *dev_id, struct pt_regs *regs){
	
	printk(KERN_INFO "GOT Interrupt!!  Now Button state : %d\n",gpio_get_value(gpioButton));
	LedStatus = !LedStatus;
	gpio_set_value(gpioLed,LedStatus);
	numPresses++;

	return IRQ_HANDLED;  // Announce that IRQ handled properly
}

module_init(gpio_init);
