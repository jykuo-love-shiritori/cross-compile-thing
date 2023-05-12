KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

cbuild: clean kbuild

shrek.ko: kbuild

install: shrek.ko
	sudo insmod ./shrek.ko

uninstall:
	sudo rmmod shrek

reinstall: uninstall install

create-dev:
	#          name           type major minor
	sudo mknod /dev/sexyshrek c    69    69

remove-dev:
	sudo rm /dev/sexyshrek
