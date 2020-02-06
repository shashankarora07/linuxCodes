#include"header.h"

char keystroke_device[] = "keystroke";


irqreturn_t keystroke_handler(int irq, void *drvdata)
{				
	unsigned status;
	unsigned scancode;

	pr_debug("keystroke handler called!!\n");

	scancode = inb(0x60);
	status = inb(0x61);

	pr_debug("Caught scancode: %u and status: %u\n", scancode, status);

	return IRQ_HANDLED;
}

static int __init keystroke_init(void)
{
	int ret = 0;

	
	pr_notice("Registering Ketstroke handler for IRQ 1\n");

	ret = request_irq(1, keystroke_handler, IRQF_SHARED, keystroke_device, THIS_MODULE);

	if (unlikely(ret)) {
		goto out;
	}
	pr_notice("Ketstroke IRQ registered..\n");

out:
	return ret;

}

module_init(keystroke_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shashank Arora");
MODULE_DESCRIPTION("A keystroke module: prints scancode on keypress/release");
