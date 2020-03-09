KERNDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

EXTRA_CFLAGS += -DDEBUG_PRINT
obj-m := jarvis.o
jarvis-objs := init_file.o
all:
	make -C $(KERNDIR) M=$(PWD) modules
install:
	make -C $(KERNDIR) M=$(PWD) modules_install
clean:
	make -C $(KERNDIR) M=$(PWD) clean
