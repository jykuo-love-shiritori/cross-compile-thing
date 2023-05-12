#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SHREK_DEV "/dev/sexyshrek"

int main() {
	long long sz;

	//int fd = open(SHREK_DEV, O_RDWR);
	int fd = open(SHREK_DEV, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open character device");
		exit(1);
	}

	char buf[1];
	sz = read(fd, buf, 1);

	printf("read: %d", sz);

	close(fd);
	return 0;
}

