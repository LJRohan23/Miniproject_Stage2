#include "files.h"
#include "queue.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

files iob[OPEN_MAX];
int Fillbuffer(files *fp) {
	if(fp->mode != _READ || fp->mode != _WRITE || fp->mode != _RDWR ) {
		return EOF;
	}
	if(fp-> flag == _UNBUF) { //currently unbuffered
		fp->base = (char *)malloc(BUFSIZE);
		if(fp->base  == NULL) {
			return EOF;
		}
	}
	fp->ptr = fp->base;//resetting the buffer position
	fp->cnt = read(fp->fd, fp->base, BUFSIZE);
	if(--fp->cnt < 0) {
		if(fp->cnt == -1) {
			fp->mode = _EOF;
		}
		else
			fp->flag = _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (int)(*fp->ptr); //check this
}
int Fgetc(files *fp) {
	char ch, x;
	if(!qempty(&fp->q1)) {
		ch = dequeue(&fp->q1);
		return (int)ch;
	}
	fp->total_r ++;
	if(--fp->cnt < 0 ) {
		if((x = Fillbuffer(fp)) == EOF )
			return EOF;
	}
	return (int)*(fp->ptr++);
}
int Ungetc(char ch, files *fp) {
	if(!qfull(&fp->q1)) {
		enqueue(&fp->q1, ch);
		return (int)ch;
	}
	return EOF;
}
files *Fopen(char *filename, char *mode) {
	files *fp;
	int fd;
	for(fp = iob; fp <= iob + OPEN_MAX; fp++) {
		if(fp->flag != _READ && fp->flag != _WRITE && fp->flag !=_RDWR)
			break;
	}
	if(fp >= iob + OPEN_MAX)
		return NULL;
	if(mode[0] == 'r') {
		if(mode[1] == '\0') {
			fd = open(filename, O_RDONLY);
			if(fd == -1) {
				perror("Unable to open : ");
				return NULL;
			}
			fp->flag = _READ;
		}
		else if(mode[1] == '+') {
			fd = open(filename, O_RDWR);
			if(fd == -1) {
				perror(" Unable to open the file : ");
				return NULL;
			}	
			fp-> flag = _RDWR;
		}
		else
			return NULL;
	}
	else if(mode[0] == 'w') {
		if(mode[1] == '\0') {
			fd = creat(filename, PERMS);
			if(fd == -1)
				return NULL;
			fp->flag = _WRITE;
		}
		else if(mode[1] == '+') {
			fd = creat(filename, O_RDWR);
			if(fd == -1)
				return NULL;
		}
		else
			return NULL;
	}
	fp -> fd = fd;
	fp -> base = NULL;
	fp -> ptr = NULL;
	fp -> cnt = 0;
	fp -> total_r = 0;
}		
int Ftell(files *fp) {
	return fp->total_r;
}
int Fseek(files *fp, long offset, int whence) {
	while(!qempty(&fp->q1)) {
		dequeue(&fp->q1);
	}
	int i = 0, x;
	char ch;
	if(whence == 0) {
		if(offset <= 0) {
			lseek(fp->fd, 0, SEEK_SET);
			fp->total_r = 0;
			return 0;
		}
		
		lseek(fp->fd, 0, SEEK_SET);
		while( i <= offset-1 && (x = read(fp -> fd, &ch ,1))) {
			i ++;
		}
		if(i<= offset -1) {//end reached
			fp->total_r = offset;
			fp->base = NULL;
			fp ->ptr = NULL;
			fp -> mark = i;
			fp -> exceed = 1;
		}else if(x != 0) {
			fp -> total_r = i;
		}
	}else if(whence == 1) {
		if(fp->exceed != 1) {
			//implies total_r is valid position
			if(offset <= 0) {
				if(fabs(offset) >= fp->total_r) {
					lseek(fp->fd, 0, 0);
					fp->total_r = 0;
				}
				lseek(fp->fd, offset, SEEK_CUR);
				fp->total_r += offset;
				Fillbuffer(fp);
			}else if(offset > 0) {
				i = 0;
				while(i <= offset - 1 && (x = read(fp -> fd, &ch, 1))) {
					i++;
				}
				if(x == 0) {//end reached
					fp->mark = fp->total_r + i;
					fp->total_r = offset;
					fp->base = NULL;
					fp->ptr = NULL;
					fp ->exceed = 1;
				}else if(i > offset - 1) {
					fp->total_r += i;
					Fillbuffer(fp);
				}
			}
		}
		else if(fp->exceed ==1){ //no characters left..end of file reached.
			if(offset >= 0) {
				fp->total_r += offset;
			}
			else if (offset < 0) {
				if(fp->total_r + offset <= fp->mark)
					fp->total_r -= offset;
				Fillbuffer(fp);
			}
		}
	}
	else
		return -1;
}
void Frewind(files *fp) {
	Fseek(fp, 0, SEEK_SET);
	Fillbuffer(fp);
}
