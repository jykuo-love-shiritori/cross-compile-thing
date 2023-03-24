#include "util.h"

int gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	int fd;
	char buf[64];

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);

	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	printf("%s\n", dirStatus);

	write(fd, dirStatus, 4);

	close(fd);
	return 0;
}
