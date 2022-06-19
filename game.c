#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"

static void init_queue(Queue *const Q, const int size) {
    Q->array = (mino_t*)malloc(sizeof(mino_t) * size);
    Q->size = size;
    Q->length = 0;
    Q->start = 0;
}

static void free_queue(Queue *const Q) {
    free(Q->array);
    Q->size = 0;
    Q->length = 0;
    Q->start = 0;
}

static void queue(Queue *const Q, const mino_t a, int *const suc) {

    if (Q->length == Q->size) {
        if (suc) {
            *suc = 0;
        }
        return;
    }
    (Q->array)[(Q->start+Q->length) % Q->size] = a;
    ++(Q->length);
    if (suc) {
        *suc = 1;
    }
    return;
}

static mino_t deque(Queue *const Q, int *const suc) {
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
static inline mino_t get_nth(Queue const *const queue, const int n) {
    return queue->array[(queue->start+n) % queue->size];
}


static const mino_t MINOS[7] = {
    IMINO, OMINO, JMINO, ZMINO,
    SMINO, LMINO, TMINO
};

/* Fisher-Yates shuffle */
static void shuffle(Nexts *const nexts) {

    int i, j;

    for (i = 0; i < 7; ++i) {
        j = rand() % (i+1);
        (nexts->ShuffledMinos)[i] = (nexts->ShuffledMinos)[j];
        (nexts->ShuffledMinos)[j] = MINOS[i];
    }
}

static void init_nexts(Nexts *const nexts, const int size) {

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

static void free_nexts(Nexts *const nexts) {
    free_queue(&(nexts->queue));
}

static mino_t pop_next(Nexts *const nexts) {

    mino_t ret;

    if (nexts->loop_cnt == 0) {
        shuffle(nexts);
    }
    ret = deque(&(nexts->queue), NULL);

    queue(&(nexts->queue), nexts->ShuffledMinos[nexts->loop_cnt], NULL);
    nexts->loop_cnt = (nexts->loop_cnt+1) % 7;

    return ret;
}


static int is_mino_putable(Player const *const player,
                    const mino_t mino, const int dir,
                    const int h, const int w) {

    int i;

    for (i = 0; i < 4; ++i) {
        if (!is_blank(player,
                      h+MINOSarray[mino][dir][i][0],
                      w+MINOSarray[mino][dir][i][1])) {

            return 0;
        }
    }
    return 1;
}


static int mino_SRS_check(Player const *const player,
                   const mino_t mino, const int dir,
                   const int rot, const int h, const int w) {

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
static inline int is_mino_movable(Player const *const player,
                    const int rh, const int rw) {

    return is_mino_putable(player,
                           player->mino,
                           player->mino_dir,
                           player->minoh+rh,
                           player->minow+rw);
}

static inline int is_land(Player const *const player) {
    return !is_mino_movable(player, -1, 0);
}

static inline int is_mino_rotatable(Player const *const player, const int rot) {

    return mino_SRS_check(player,
                          player->mino,
                          player->mino_dir,
                          rot,
                          player->minoh,
                          player->minow);
}

static void delete_rows(Player *const player) {

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

static void _mov(Player *const player,
          const int rh, const int rw, int *suc) {

    int inc_rock_down = 0;

    if (is_mino_movable(player, rh, rw)) {
        inc_rock_down = is_land(player) ? 1 : inc_rock_down;
        player->minoh += rh;
        player->minow += rw;
        inc_rock_down = is_land(player) ? 1 : inc_rock_down;
        if (suc != NULL) {
            *suc = 1;
        }
    } else {
        if (suc != NULL) {
            *suc = 0;
        }
    }

    if (inc_rock_down) {
        ++(player->rock_down_count);
        gettimeofday(&(player->rock_down_tv), NULL);
    }
}

static void _key_rotation(Player *const player, const int rot) {

    int inc_rock_down = 0;

    int var = is_mino_rotatable(player, rot);

    if (var < 0) {
        return;
    }

    inc_rock_down = is_land(player) ? 1 : inc_rock_down;

    player->minoh += SRSoffsets[player->mino][SRS_ROT(player->mino_dir, rot)][var][0];
    player->minow += SRSoffsets[player->mino][SRS_ROT(player->mino_dir, rot)][var][1];

    player->mino_dir = (rot == ROT_L) ? (player->mino_dir+3)%4 : (player->mino_dir+1)%4;

    inc_rock_down = is_land(player) ? 1 : inc_rock_down;

    if (inc_rock_down) {
        ++(player->rock_down_count);
        gettimeofday(&(player->rock_down_tv), NULL);
    }
}

static void set_mino(Player *const player, const mino_t mino) {

    player->mino = mino;
    player->mino_dir = DIR_N;
    player->minoh = MINO_POP_H;
    player->minow = MINO_POP_W;
    gettimeofday(&(player->fall_tv), NULL);
    player->rock_down_count = 0;
    if (is_land(player)) {
        gettimeofday(&(player->rock_down_tv), NULL);
        ++(player->rock_down_count);
    }
}

static void put_mino(Player *const player, const mino_t mino) {

    int i;

    for (i = 0; i < 4; ++i) {
        BLOCK(player,
              player->minoh+MINOSarray[player->mino][player->mino_dir][i][0],
              player->minow+MINOSarray[player->mino][player->mino_dir][i][1])
            = player->mino;
    }
    delete_rows(player);
    player->did_hold = 0;
    set_mino(player, mino);
}


static int pass_fall_time(Player const *const player) {

    long long int from_ms;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    /* オーバーフロー防止 */
    from_ms = (tv.tv_sec - (player->fall_tv).tv_sec) * 1000;
    from_ms += (tv.tv_usec - (player->fall_tv).tv_usec) / 1000;

    if (from_ms > player->fall_ms) {
        return 1;
    } else {
        return 0;
    }
}

static int pass_rock_down_time(Player const *const player) {

    long long int from_ms;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    from_ms = (tv.tv_sec - (player->rock_down_tv).tv_sec) * 1000;
    from_ms += (tv.tv_usec - (player->rock_down_tv).tv_usec) / 1000;

    if (from_ms > player->rock_down_ms) {
        return 1;
    } else {
        return 0;
    }
}

inline int is_blank(Player const *const player, const int h, int w) {
    return 0 <= h && h < FIELDH
        && 0 <= w && w < FIELDW
        && BLOCK(player, h, w) == BLANK;
}

inline mino_t get_nth_next(Player const *const player, const int n) {
    return get_nth(&((player->nexts).queue), n);
}

void init_player(Player *const player, const int nextlen, const int fall_ms) {

    int i, j;

    /* FieldをBLANKで埋める */
    for (i = 0; i < FIELDH; ++i) {
        for (j = 0; j < FIELDW; ++j) {
            (player->Field)[i][j] = BLANK;
        }
    }

    /* nextを用意 */
    init_nexts(&(player->nexts), nextlen+1);
    player->nextlen = nextlen;

    /* 現在ミノをセット,ホールドミノを空に */
    set_mino(player, pop_next(&(player->nexts)));
    player->did_hold = 0;
    player->hold_mino = UNDEF;

    /* タイマー系のセット */
    player->fall_ms = fall_ms;
    player->rock_down_ms = 500;
}

void free_player(Player *const player) {
    free_nexts(&(player->nexts));
}

inline void key_mov_left(Player *const player) {
    _mov(player, 0, -1, NULL);
}

inline void key_mov_right(Player *const player) {
    _mov(player, 0, 1, NULL);
}

inline void key_soft_drop(Player *const player) {
    _mov(player, -1, 0, NULL);
}

void key_hard_drop(Player *const player) {

    while (is_mino_movable(player, -1, 0)) {
        --(player->minoh);
    }

    put_mino(player, pop_next(&(player->nexts)));
    player->did_hold = 0;
}

inline void key_rotation_left(Player *const player) {
    _key_rotation(player, ROT_L);
}

inline void key_rotation_right(Player *const player) {
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

inline int is_gameover(Player const *const player) {
    return !is_mino_movable(player, 0, 0);
}

void auto_fall(Player *const player) {

    if (pass_fall_time(player)) {
        key_soft_drop(player);
        gettimeofday(&(player->fall_tv), NULL);
    }
}

void rock_down_put_mino(Player *const player) {

    if (is_land(player)
        && (pass_rock_down_time(player)
            || player->rock_down_count >= 15)) {

        put_mino(player, pop_next(&(player->nexts)));
        player->did_hold = 0;
    }
}
