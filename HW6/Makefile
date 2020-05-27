# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
	ccflags-y += -g
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko
endif
