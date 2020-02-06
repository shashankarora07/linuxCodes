#include"header.h"

static void __exit keystroke_exit(void)
{
    free_irq(1, THIS_MODULE);
    pr_notice("Keystroke  module unloaded\n");

}

module_exit(keystroke_exit);
