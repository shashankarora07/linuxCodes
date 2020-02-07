obj-m := gpio.o
gpio-objs := init.o clean.o
#KERNBUILD := /home/shashank/BBB/linux-4.20.3
KERNBUILD := /home/shashank/linux-stable
all:
	make -C $(KERNBUILD) M=$(PWD) modules
clean:
	make -C $(KERNBUILD) M=$(PWD) clean
