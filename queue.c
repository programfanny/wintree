#include <windows.h>
#include <stdio.h>
#include "queue.h"
Queue InitQueue(){
	Queue seq;
	for(int i = 0; i < QueueMax; i++) {
		seq.data[i] = NULL;
	}
	seq.head = 0;
	seq.rear = -1;
	seq.len = 0;
	return seq;
}
int IsEmptyQueue(Queue seq){
	if (seq.len == 0) {
		return 1;
	}
	return 0;
}
int IsFullQueue(Queue seq){ 
	if (seq.len == QueueMax) {
		return 1;
	}
	return 0;
}
void PushQueue(Queue *seq, pBiTree T){ 
	if (IsFullQueue(*seq)) { 
		printf("ErrorFull");
		return;
	}
	seq->rear = (seq->rear + 1) % QueueMax;
	seq->len++;
	seq->data[seq->rear] = T;
}
void PopQueue(Queue *seq, pBiTree *T){ 
	if (IsEmptyQueue(*seq)) {    
		printf("ErrorEmpty");
		return;
	}
	seq->head = (seq->head + 1) % QueueMax;
	*T = seq->data[seq->head];
	seq->len--;
}