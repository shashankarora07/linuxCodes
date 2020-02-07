
extern unsigned int gpioLed, gpioButton, irqno, *buttonPress;
extern bool LedStatus;

irq_handler_t gpio_irq_handler(unsigned int , void *, struct pt_regs *);
