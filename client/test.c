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
	lseek(fd, 1, SEEK_SET);
	write(fd, "0", 2);
	read(fd, data, 2);

    close(fd);
    return 0;
}

