obj-m        = shrek-led.o
shrek-led-y = shrek.o gpio.o

KDIR = /lib/modules/`uname -r`/build

kbuild:
	make -C $(KDIR) M=`pwd`

clean:
	make -C $(KDIR) M=`pwd` clean

cbuild: clean kbuild

shrek.ko: kbuild

install: shrek.ko Kbuild
	sudo insmod ./shrek-led.ko
	#          name           type major minor
	sudo mknod /dev/sexyshrek c    69    69

uninstall:
	sudo rmmod shrek-led
	sudo rm /dev/sexyshrek

reinstall: uninstall install

