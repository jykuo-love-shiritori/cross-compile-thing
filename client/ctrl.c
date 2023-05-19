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
	fd = open(SHREK_DEV, O_WRONLY);
    off_t ledno = argv[1][3] - '0';
    lseek(fd,ledno , SEEK_SET);
    if(argc==2){
        read(fd,data,2);
        data[1] = '\0';
        printf("%s Status: %s\n",
            argv[1],
            data);
    }
    else if (argv[2][1] == 'n') {
		write(fd,"1",2);
	} else if (argv[2][1] == 'f') {
		write(fd,"0",2);
	}

    close(fd);
    return 0;
}