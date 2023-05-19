no:
	@echo "DON'T USE THIS"


.PHONY: all
all: module client

.PHONY: module
module:
	@echo "BUILD MO"
	@make -C ./module kbuild

.PHONY: module-install module-uninstall
module-install:
	make -C ./module install

module-uninstall:
	make -C ./module uninstall

.PHONY: reinstall
reinstall: module-uninstall module-install

.PHONY: client
client:
	@echo "BUILD C"
	@make -C ./client

create-dev:
	#          name           type major minor
	sudo mknod /dev/sexyshrek c    69    69

remove-dev:
	sudo rm /dev/sexyshrek -f

.PHONY: clean
clean: remove-dev
	make -C ./module clean
	make -C ./client clean
