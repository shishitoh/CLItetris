#pragma once

#include <stdlib.h>

#include "mino.h"

#define FIELDH 40
#define FIELDW 10

/* ミノの出現位置 ミノの最下段が20列目(1スタート)
となるように定義 */
#define MINO_POP_H 17
#define MINO_POP_W 3

#define MOV_R 1
#define MOV_L -1

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3
#define ROT_R 0
#define ROT_L 1

/* SRSoffsetsにおけるmino配列の持ち方と
上のマクロ6つの値に依存 */
#define SRS_ROT(dir, rot) ( (dir) + ((rot) << 2) )

#define BLOCK(player, h, w) (player->Field)[h][w]

/* 直接使用はしない */
typedef struct {
    mino_t *array;
    int size;
    int length;
    int start;
} Queue;

/* 直接使用はしない */
typedef struct {
    Queue queue;
    mino_t ShuffledMinos[7];
    int loop_cnt;
} Nexts;

typedef struct {
    mino_t Field[FIELDH][FIELDW];
    mino_t mino;
    char mino_dir;
    char minoh;
    char minow;
    int hold_mino;
    int did_hold;
    Nexts nexts;
    int nextlen;
} Player;

int is_blank(Player const *const player, const char h, const char w);
mino_t get_nth_next(Player const *const player, const int n);
void init_player(Player *const player, const int nextlen);
void free_player(Player *const player);
void key_mov_left(Player *const player);
void key_mov_right(Player *const player);
void key_mov_down(Player *const player);
void key_hard_drop(Player *const player);
void key_rotation_left(Player *const player);
void key_rotation_right(Player *const player);
void key_hold(Player *const player);
void delete_rows(Player *const player);
int is_gameover(Player const *const player);
