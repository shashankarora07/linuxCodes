#/usr/bin/bash
CONFIG_MODULE_SIG=n
INSTALLDIR=$(shell pwd)/modules
 
obj-m := init_mod.o
init_mod-objs :=  init.o exit.o bbbgpio_open.o ioctl_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -rf $(INSTALLDIR)
	@mkdir $(INSTALLDIR)
	@mv *.ko *.mod.c *.o  *.symvers *.order $(INSTALLDIR)
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@rm -rf $(INSTALLDIR)
