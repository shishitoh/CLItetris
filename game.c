#include <stdio.h>

#include "game.h"

void init_queue(Queue *const Q, const int size) {
    Q->array = (mino_t*)malloc(sizeof(mino_t) * size);
    Q->size = size;
    Q->length = 0;
    Q->start = 0;
}

void free_queue(Queue *const Q) {
    free(Q->array);
    Q->size = 0;
    Q->length = 0;
    Q->start = 0;
}

void queue(Queue *const Q, const mino_t a, int *const suc) {

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

mino_t deque(Queue *const Q, int *const suc) {
    mino_t ret;
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

mino_t get_nth(Queue const *const queue, const int n) {
    return queue->array[(queue->start+n) % queue->size];
}

const mino_t MINOS[7] = {
    IMINO, OMINO, JMINO, ZMINO,
    SMINO, LMINO, TMINO
};

/* Fisher-Yates shuffle */
void shuffle(Nexts *const nexts) {

    int i, j;

    for (i = 0; i < 7; ++i) {
        j = rand() % (i+1);
        (nexts->ShuffledMinos)[i] = (nexts->ShuffledMinos)[j];
        (nexts->ShuffledMinos)[j] = MINOS[i];
    }
}

void init_nexts(Nexts *const nexts, const int size) {

    int i, j;

    init_queue(&(nexts->queue), size);

    for (i = 0; i < size/7; ++i) {
        shuffle(nexts);
        for (j = 0; j < 7; ++j) {
            queue(&(nexts->queue), (nexts->ShuffledMinos)[j], NULL);
        }
    }
    shuffle(nexts);
    for (i = 0; i < size%7; ++i) {
        queue(&(nexts->queue), (nexts->ShuffledMinos)[i], NULL);
    }
    nexts->loop_cnt = size%7;
}

void free_nexts(Nexts *const nexts) {
    free_queue(&(nexts->queue));
}

mino_t pop_next(Nexts *const nexts) {

    mino_t ret;

    if (nexts->loop_cnt == 0) {
        shuffle(nexts);
    }
    ret = deque(&(nexts->queue), NULL);

    queue(&(nexts->queue), nexts->ShuffledMinos[nexts->loop_cnt], NULL);
    nexts->loop_cnt = (nexts->loop_cnt+1) % 7;

    return ret;
}


int is_blank(Player const *const player, const char h, char w) {
    return 0 <= h && h < FIELDH
        && 0 <= w && w < FIELDW
        && BLOCK(player, h, w) == BLANK;
}

int is_mino_putable(Player const *const player,
                    const mino_t mino, const char dir,
                    const char h, const char w) {

    int i;

    for (i = 0; i < 4; ++i) {
        if (is_blank(player,
                     h+MINOSarray[mino][dir][i][0],
                     w+MINOSarray[mino][dir][i][1])) {

            continue;
        }
        return 0;
    }
    return 1;
}

void set_mino(Player *const player, const mino_t mino) {

    player->mino = mino;
    player->mino_dir = DIR_N;
    player->minoh = MINO_POP_H;
    player->minow = MINO_POP_W;
}

void put_mino(Player *const player, const mino_t mino) {

    int i;

    for (i = 0; i < 4; ++i) {
        BLOCK(player,
              player->minoh+MINOSarray[player->mino][player->mino_dir][i][0],
              player->minow+MINOSarray[player->mino][player->mino_dir][i][1])
            = player->mino;
    }
    delete_rows(player);
    set_mino(player, mino);
}

mino_t get_nth_next(Player const *const player, const int n) {
    return get_nth(&((player->nexts).queue), n);
}

int mino_SRS_check(Player const *const player,
                   const mino_t mino, const char dir,
                   const char rot, const char h, const char w) {

    int i;

    for (i = 0; i < 5; ++i) {
        if (is_mino_putable(player,
                            mino,
                            (rot == ROT_L) ? (dir+3)%4 : (dir+1)%4,
                            h+SRSoffsets[mino][SRS_ROT(dir, rot)][i][0],
                            w+SRSoffsets[mino][SRS_ROT(dir, rot)][i][1])) {

            return i;
        }
    }
    return -1;
}

/* rh, rwは現在位置からの移動量 */
int is_mino_movable(Player const *const player,
                    const char rh, const char rw) {

    return is_mino_putable(player,
                           player->mino,
                           player->mino_dir,
                           player->minoh+rh,
                           player->minow+rw);
}

int is_mino_rotatable(Player const *const player, const char rot) {

    return mino_SRS_check(player,
                          player->mino,
                          player->mino_dir,
                          rot,
                          player->minoh,
                          player->minow);
}


void _mov(Player *const player,
          const char rh, const char rw, int *suc) {

    if (is_mino_movable(player, rh, rw)) {
        player->minoh += rh;
        player->minow += rw;
        if (suc != NULL) {
            *suc = 1;
        }
    } else {
        if (suc != NULL) {
            *suc = 0;
        }
    }
}

void init_player(Player *const player, const int nextlen) {

    int i, j;

    /* FieldをBLANKで埋める */
    for (i = 0; i < FIELDH; ++i) {
        for (j = 0; j < FIELDW; ++j) {
            (player->Field)[i][j] = BLANK;
        }
    }

    /* nextを用意 */
    init_nexts(&(player->nexts), nextlen);
    player->nextlen = nextlen;

    /* 現在ミノをセット,ホールドミノを空に */
    set_mino(player, pop_next(&(player->nexts)));
    player->did_hold = 0;
    player->hold_mino = UNDEF;
}

void free_player(Player *const player) {
    free_nexts(&(player->nexts));
}

void key_mov_left(Player *const player) {
    _mov(player, 0, MOV_L, NULL);
}

void key_mov_right(Player *const player) {
    _mov(player, 0, MOV_R, NULL);
}

void key_mov_down(Player *const player) {

    int suc;
    _mov(player, -1, 0, &suc);

    if (!suc) {
        put_mino(player, pop_next(&(player->nexts)));
        player->did_hold = 0;
    }
}

void key_hard_drop(Player *const player) {

    int rh = 0;

    do {
        --rh;
    } while (is_mino_movable(player, rh, 0));
    ++rh;

    player->minoh += rh;
    put_mino(player, pop_next(&(player->nexts)));
    player->did_hold = 0;
}

void _key_rotation(Player *const player, const char rot) {

    int var = is_mino_rotatable(player, rot);

    if (var < 0) {
        return;
    }

    player->minoh += SRSoffsets[player->mino][SRS_ROT(player->mino_dir, rot)][var][0];
    player->minow += SRSoffsets[player->mino][SRS_ROT(player->mino_dir, rot)][var][1];

    player->mino_dir = (rot == ROT_L) ? (player->mino_dir+3)%4 : (player->mino_dir+1)%4;
}


void key_rotation_left(Player *const player) {
    _key_rotation(player, ROT_L);
}

void key_rotation_right(Player *const player) {
    _key_rotation(player, ROT_R);
}

void key_hold(Player *const player) {

    mino_t tmp;

    if (player->did_hold) {
        return;
    }

    tmp = player->hold_mino;
    player->hold_mino = player->mino;
    if (tmp == UNDEF) {
        set_mino(player, pop_next(&(player->nexts)));
    } else {
        set_mino(player, tmp);
    }
    player->did_hold = 1;
}


void delete_rows(Player *const player) {

    int i, brow, trow;
    int exists_empty;

    for (brow = 0, trow = 0; trow < FIELDH; ++trow) {

        exists_empty = 0;

        for (i = 0; i < FIELDW; ++i) {
            if (BLOCK(player, trow, i) == BLANK) {
                exists_empty = 1;
                break;
            }
        }

        if (exists_empty) {
            if (brow < trow) {
                for (i = 0; i < FIELDW; ++i) {
                    BLOCK(player, brow, i) = BLOCK(player, trow, i);
                }
            }
            ++brow;
        }
    }

    for (; brow < FIELDH; ++brow) {
        for (i = 0; i < FIELDW; ++i) {
            BLOCK(player, brow, i) = BLANK;
        }
    }
}

int is_gameover(Player const *const player) {
    return !is_mino_movable(player, 0, 0);
}
