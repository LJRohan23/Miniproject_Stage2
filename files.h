#ifndef __FILES_H
#define __FILES_H
#include <stdio.h>
#include <stdarg.h>
#include "queue.h"
#define BUFSIZE 1024
#define OPEN_MAX 20
#define PERMS 0666
typedef struct files {
	int total_r; //total read characters (for ftell() access)
	char *ptr; //buffer pointer location (to be used by several functions)
	char *base; //location of buffer
	queue q1; //the queue data type for ungetc(), Will be referred by other functions too
	int cnt; //number of characters remaining in the buffer
	int fd; //file descriptor
	int flag; //denotes the type of the file
	int mode; //mode of file opening...rd, wr, rdwr
	int mark; //location of the last character of the file
	int exceed; //flag for determining EOF encounter
	char *name; //name of the file passed as parameter
}files;
enum FLAGS {
	_READ ,
	_WRITE ,
	_RDWR ,
	_EOF ,
	_ERR ,
	_UNBUF ,
	_BUF 
};
int fillbuffer(files *);
files *Fopen(char *, char *);
int Ftell(files *);
void Rewind(files *);
int Fseek(files *, long, int);
int Fgetpos(files *, fpos_t);
int Fsetpos(files *, const fpos_t);
size_t Fread(void *, size_t , size_t , files *);
size_t Fwrite(const void *, size_t , size_t , files *);
int Fscanf(files *, char *, ...);
int Fprintf(files *, const char *, ...);
files *Fclose(char *);
int Fgetc(files *);
int Ungetc(char, files*);
#endif
