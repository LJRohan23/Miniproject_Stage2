#include "queue.h"
int qfull(queue *q) {
	if((q->front) == 0 && (q->rear) == QMAX-1 || (q->front) - 1 == (q->rear))
		return 1;
	return 0;
}
int qempty(queue *q) {
	return (q->front == -1 );
}
void qinit(queue *q) {
	q->front = q->rear = -1;
}
void enqueue(queue *q, char ch) {
	if(q->rear== -1) {
		q->front = q->rear = 0;
	}
	else
		if(q->rear == QMAX - 1) {
			q->rear = 0;
		}
		else
			q->rear ++;
	q->arr[q->rear] = ch;
}
char dequeue(queue *q) {
	char temp = q->arr[q->front];
	if(q->front == q->rear)
		q->front = q->rear = -1;
	else
		if(q->front == QMAX - 1)
			q->front = 0;
		else
				q->front++;
	return temp;
};

