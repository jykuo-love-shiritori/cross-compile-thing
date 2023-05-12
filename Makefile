KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

install:
	sudo insmod ./mod.ko

uninstall:
	sudo rmmod ./mod.ko
