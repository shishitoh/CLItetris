#pragma once

#include <time.h>
#include <sys/time.h>

#include "mino.h"

#define FIELDH 40
#define FIELDW 10

/* ミノの出現位置 ミノの最下段が21列目(1スタート)
となるように定義 */
#define MINO_POP_H 18
#define MINO_POP_W 3

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

typedef struct {
    mino_t *array;
    int size;
    int length;
    int start;
} Queue;

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
    int fall_ms;
    struct timeval fall_tv;
    int rock_down_ms;
    int rock_down_count;
    struct timeval rock_down_tv;
} Player;

int is_blank(Player const *const player, const char h, const char w);
mino_t get_nth_next(Player const *const player, const int n);
void init_player(Player *const player, const int nextlen, const int fall_ms);
void free_player(Player *const player);
void key_mov_left(Player *const player);
void key_mov_right(Player *const player);
void key_soft_drop(Player *const player);
void key_hard_drop(Player *const player);
void key_rotation_left(Player *const player);
void key_rotation_right(Player *const player);
void key_hold(Player *const player);
int is_gameover(Player const *const player);
void auto_fall(Player *const player);
void rock_down_put_mino(Player *const player);
