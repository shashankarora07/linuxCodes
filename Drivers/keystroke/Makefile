obj-m := keystroke.o
keystroke-objs := initFile.o exitFile.o

KERBUILD := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KERBUILD) M=$(PWD) modules
clean:
	make -C $(KERBUILD) M=$(PWD) clean
install:
	make -C $(KERBUILD) M=$(PWD) modules_install
