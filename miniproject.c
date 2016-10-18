#include "files.h"
#include "queue.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
void Usage() {
	int fd;
	char ch;
	fd = open("help.c", O_RDONLY);
	while(read(fd, &ch, 1))
		putchar(ch);
}
// implementing a queue of characters

int main(int argc, char *argv[]) {
	if(argc == 2) {
		if(strcmp(argv[1], "-h") == 0) {
			Usage();
			printf("\n");
			return 0;
		}
		else {
			errno = EINVAL;
			perror("Invalid syntax : ");
			return errno;
		}
	}
}
