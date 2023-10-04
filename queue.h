#ifndef _QUQUQ_H_
#define _QUEEUE_H_

#include "treetype.h"

Queue InitQueue();
int IsEmptyQueue(Queue seq);
int IsFullQueue(Queue seq);
void PushQueue(Queue *seq, pBiTree T);
void PopQueue(Queue *seq, pBiTree *T);

#endif