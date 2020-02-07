KERNBUILD ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m:= jarvis.o
jarvis-objs := ioctl_init.o ioctl_exit.o open_ioctl.o read_file.o write_file.o

all:
	make -C $(KERNBUILD) M=$(PWD) modules
clean:
	make -C $(KERNBUILD) M=$(PWD) clean
