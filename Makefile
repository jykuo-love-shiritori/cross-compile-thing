obj-m := shrek.o

KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

cbuild: clean kbuild

shrek.ko: kbuild

install: shrek.ko
	sudo insmod ./shrek-led.ko
	#sudo cat /proc/devices | grep sexyshrek
	#          name           type major minor
	sudo mknod /dev/sexyshrek c    69    69

uninstall:
	sudo rmmod shrek-led
	sudo rm -f /dev/sexyshrek

test:
	./script/test.sh
