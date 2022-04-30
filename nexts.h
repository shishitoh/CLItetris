#pragma once

typedef struct QUEUE {
    int *array;
    int size;
    int length;
    int start;
} Queue;

void init_queue(Queue *Q, int size);
void free_queue(Queue *Q);
void queue(Queue *Q, int a, int *suc);
int deque(Queue *Q, int *suc);

typedef struct NEXTS {
    Queue queue;
    int loop_cnt;
} Nexts;

void init_nexts(Nexts *nexts, int size);
int pop_next(Nexts *nexts);
int Nget_element(Nexts *nexts, int l);
void free_nexts(Nexts *nexts);
