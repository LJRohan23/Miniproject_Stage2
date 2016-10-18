#ifndef __QUEUE_H
#define __QUEUE_H
#define QMAX 1024
typedef struct queue {
	int front, rear;
	char arr[QMAX];
}queue;
int qfull(queue *);
int qempty(queue *);
void enqueue(queue *, char);
char dequeue(queue *);
#endif
