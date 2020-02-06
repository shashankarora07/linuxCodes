#include"header.h"

char keystroke_device[] = "keystroke";

static void keystroke_tasklet_handler(unsigned long data)
{
	unsigned status;
	unsigned scancode;

	pr_notice("keystroke_tasklet_handler called!! PID = %u\n",current->pid);
	status = inb(0x64);
	scancode = inb(0x60);

	pr_notice("Caught Scancode = %u and Status = %u\n",scancode,status);	
	pr_notice("keystroke_tasklet_handler: hard IRQ context: %lu \n",in_irq());
	pr_notice("keystroke_tasklet_handler: soft IRQ context: %lu \n",in_softirq());
	pr_notice("keystroke_tasklet_handler: interrupt context: %lu \n",in_interrupt());
	pr_notice("----------------------------------------");
	dump_stack();	
	mdelay(1000);
	pr_notice("----------------------------------------");

	pr_notice("keystroke_tasklet_handler finished.....\n");
}


DECLARE_TASKLET(keystroke_tasklet,keystroke_tasklet_handler,0);

/*
	#define DECLARE_TASKLET(name,func,data) \
	struct tasklet_struct keystroke_tasklet = 
	{ func: keystroke_taskler_handler,
	  data: (unsigned long) keystroke_device
	}
*/

irqreturn_t keystroke_irq_handler(int irq, void *drvdata)
{
	pr_notice("keystroke_irq_handler called! pid = %d\n",current->pid);
	pr_debug("keystroke_irq_handler hard IRQ context: %lu \n",in_irq());
	pr_debug("keystroke_irq_handler soft IRQ context: %lu \n",in_softirq());
	pr_debug("keystroke_irq_handler interrupt context: %lu \n",in_interrupt());

	tasklet_schedule(&keystroke_tasklet);
	
	return IRQ_HANDLED;
}

static int __init keystroke_init(void)
{
	int ret = 0;

	pr_notice("Registering keystroke handler for IRQ 1\n");
	
	ret = request_irq(1, keystroke_irq_handler, IRQF_SHARED, keystroke_device, THIS_MODULE);

	if (unlikely(ret)) {
		goto out;
	}
	pr_notice("Keystroke IRQ registered\n");

out: 
	return ret;

}

module_init(keystroke_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shashank Arora");
MODULE_DESCRIPTION("A keystroke tasklet handler module: prints scancode on keypress/release");
