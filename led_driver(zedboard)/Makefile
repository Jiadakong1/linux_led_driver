ifeq ($(KERNELRELEASE),)

KERNEL_DIR ?=/home/zxy/Desktop/ZedBoard/kernel/linux-xlnx
PWD :=$(shell pwd)

modules:
	 $(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

.PHONY:modules clean
clean:
	 $(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
else
    obj-m := led_driver.o
endif
