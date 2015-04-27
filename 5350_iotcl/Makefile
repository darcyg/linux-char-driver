obj-m += rt5350_io_ctl.o

# KERN_DIR:=/lib/modules/$(shell uname -r)/build
KERN_DIR:=/root/RT288x_SDK/source/linux-2.6.21.x/


all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean

distclean:
	rm *.mod.c *.o *.ko modules.order Module.*
	
