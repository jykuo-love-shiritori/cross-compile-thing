#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SHREK_DEV "/dev/sexyshrek"

int main() {
	int fd;
	char data[2];

	fd = open(SHREK_DEV, O_RDWR);

	write(fd, "1", 2);
	read(fd, data, 2);

    close(fd);
    return 0;
}

