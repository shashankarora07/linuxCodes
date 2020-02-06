obj-m := ioctlLed.o
ioctlLed-objs := IOCTL.o

KERNBUILD := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: 
	make -C $(KERNBUILD) M=$(PWD) modules
clean:
	make -C $(KERNBUILD) M=$(PWD) clean
