#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SHREK_DEV "/dev/sexyshrek"

int main(int argc,char**argv) {
    int fd;
    char data[2];
    if ((fd = open(SHREK_DEV, O_RDWR)) < 0) {
        perror("shrek");
    }
    off_t ledno = argv[1][3] - '0';
    lseek(fd,ledno , SEEK_SET);
    if(argc==2){
        read(fd,data,2);
        data[1] = '\0';
        printf("LED%ld Status: %s\n", ledno, data);
    } else {
	if (argv[2][1] == 'n') {
		if ( write(fd,"1",2) < 0 ) {
			perror("write");
		}
	    } else if (argv[2][1] == 'f') {
		if ( write(fd,"0",2) < 0 ) {
			perror("write");
		}
	    }
    }

    close(fd);
    return 0;
}
