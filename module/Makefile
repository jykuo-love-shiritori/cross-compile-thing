obj-m := shrek.o

KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

cbuild: clean kbuild

shrek.ko: kbuild

install: shrek.ko
	sudo insmod ./shrek.ko
	#          name           type major minor
	sudo mknod /dev/sexyshrek c    69    69

uninstall:
	sudo rmmod shrek
	sudo rm /dev/sexyshrek

reinstall: uninstall install

