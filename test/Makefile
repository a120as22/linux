obj-m += bh-1750.o
obj-m += hello.o
obj-m += bh-1750dev.o

PWD := $(shell pwd)
OUTPUTDIR=/lib/modules/`uname -r`/kernel/drivers/net/usb/
OVERLAY := /boot/overlays

ifeq ($(ARCH),)
ARCH := arm64
endif
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE :=
endif
ifeq ($(KDIR),)
KDIR := /lib/modules/$(shell uname -r)/build
ifeq ($(ARCH),i686)
ifeq ($(wildcard $KDIR/arch/$ARCH),)
ARCH=i386
endif
endif
endif

default:
	$(MAKE) ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -C $(KDIR) M=$(PWD) modules

install: default
	cp $(PWD)/bh-1750.ko /lib/modules/$(shell uname -r)/kernel/drivers/net/usb/
	depmod
dtb:
	dtc -@ -Hepapr -I dts -O dtb -o bh1750.dtb bh1750.dts 
	sudo cp $(PWD)/*.dtb $(OVERLAY)

clean:
	rm -rf *~ .tmp_versions modules.order Module.symvers
	find . -type f -name "*~" -o -name "*.o" -o -name "*.ko" -o -name "*.cmd" -o -name "*.mod.c" |  xargs rm -rf
