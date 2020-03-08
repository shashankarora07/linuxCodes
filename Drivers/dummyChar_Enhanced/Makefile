KERNDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m := jarvis.o
jarvis-objs := init_file.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
