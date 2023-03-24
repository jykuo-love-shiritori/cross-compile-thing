#include "util.h"

int gpio_set_value(unsigned int gpio, const char *value) {
	int fd;
	char buf[64];

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);

	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	printf("%s\n", value);

	write(fd, value, 2);

	close(fd);
	return 0;
}
