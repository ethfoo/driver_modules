obj-m +=memdev.o

CURRENT_PATH :=$(shell pwd)
KERNEL :=$(shell uname -r)
KERNEL_PATH :=/usr/src/linux-headers-$(KERNEL)

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean 
