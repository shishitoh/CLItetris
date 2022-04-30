#include <stdio.h>
#include <stdlib.h>

#include "mino.h"
#include "nexts.h"

void init_queue(Queue *Q, int size) {
    Q->array = (int*)malloc(sizeof(int) * size);
    Q->size = size;
    Q->length = 0;
    Q->start = 0;
}

void free_queue(Queue *Q) {
    free(Q->array);
    Q->size = 0;
    Q->length = 0;
    Q->start = 0;
}

void queue(Queue *Q, int a, int *suc) {

    if (Q->length == Q->size) {
        if (suc != NULL) {
            *suc = 0;
        }
        return;
    }
    (Q->array)[(Q->start+Q->length) % Q->size] = a;
    Q->length += 1;
    if (suc != NULL) {
        *suc = 1;
    }
    return;
}

int deque(Queue *Q, int *suc) {
    int ret;
    if (Q->length == 0) {
        if (suc) {
            *suc = 0;
        }
        return 0;
    }
    ret = Q->array[Q->start];
    Q->start = (Q->start+1) % Q->size;
    --(Q->length);
    if (suc) {
        *suc = 1;
    }
    return ret;
}

/* キューの定義ここまで */

static const int MINOS[7] = {
    IMINO, OMINO, JMINO, ZMINO,
    SMINO, LMINO, TMINO
};

static int SHUFFLEDMINOS[7];

/* Fisher-Yates shuffle */
static void shuffle() {

    int i, j;

    for (i = 0; i < 7; ++i) {
        j = rand() % (i+1);
        SHUFFLEDMINOS[i] = SHUFFLEDMINOS[j];
        SHUFFLEDMINOS[j] = MINOS[i];
    }
}


void init_nexts(Nexts *nexts, int size) {

    int i, j;

    init_queue(&(nexts->queue), size);

    for (i = 0; i < size/7; ++i) {
        shuffle();
        for (j = 0; j < 7; ++j) {
            queue(&(nexts->queue), SHUFFLEDMINOS[j], NULL);
        }
    }
    shuffle();
    for (i = 0; i < size%7; ++i) {
        queue(&(nexts->queue), SHUFFLEDMINOS[i], NULL);
    }
    nexts->loop_cnt = size%7;
}

int pop_next(Nexts *nexts) {

    if (nexts->loop_cnt == 0) {
        shuffle();
    }
    int ret = deque(&(nexts->queue), NULL);

    queue(&(nexts->queue), SHUFFLEDMINOS[nexts->loop_cnt], NULL);

    nexts->loop_cnt = (nexts->loop_cnt+1) % 7;

    return ret;
}

int Nget_element(Nexts *nexts, int l) {
    return ((nexts->queue)).array[((nexts->queue).start+l) % (nexts->queue).size];
}

void free_nexts(Nexts *nexts) {
    free_queue(&(nexts->queue));
}
