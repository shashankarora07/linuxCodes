obj-m := keystroke_tasklet.o
keystroke_tasklet-objs := keystroke_tasklet_init.o keystroke_tasklet_exit.o

KERBUILD := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KERBUILD) M=$(PWD) modules
clean:
	make -C $(KERBUILD) M=$(PWD) clean
install:
	make -C $(KERBUILD) M=$(PWD) modules_install
