KERNDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m := jarvis.o
jarvis-objs := init_file.o exit_file.o open_file.o close_file.o read_file.o write_file.o

all:
	make -C $(KERNDIR) M=$(PWD) modules
clean:
	make -C $(KERNDIR) M=$(PWD) clean
