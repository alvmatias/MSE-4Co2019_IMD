ifneq ($(KERNELRELEASE),)
obj-m := mylcd.o
else
KDIR := $(HOME)/linux-kernel-labs/src/linux
all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
