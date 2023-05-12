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
	FILE *fp = fopen(SHREK_DEV, "w+");
	if (fp == NULL) {
		perror("Failed to open character device");
		exit(1);
	}
	char buf[1024];
	char buf2[1024] = "";

	char bufa[1024] = "GET OUT OF MY SWAMP!";
	fwrite(bufa, 20, 1, fp);
	fread(buf2, 100, 1, fp);
	printf("read: %s\n", buf2);

	char bufb[1024] = "CLEAR STRING";
	fwrite(bufb, 20, 1, fp);
	fread(buf2, 100, 1, fp);
	printf("read: %s\n", buf2);
	fclose(fp);
	return 0;
}

