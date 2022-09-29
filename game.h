#pragma once

#include <time.h>
#include <sys/time.h>

#include "mino.h"
#include "config.h"

#define FIELDH 40
#define FIELDW 10

/* ミノの出現位置 ミノの最下段が21列目(1スタート)
となるように定義 */
#define MINO_POP_H 18
#define MINO_POP_W 3

typedef struct queue Queue;
typedef struct nexts Nexts;
typedef struct player Player;

typedef struct coodinate {
    int h;
    int w;
} Coodinate;

mino_t block(Player const *const player, const int h, const int w);
int is_blank(Player const *const player, const int h, const int w);
mino_t get_mino_type(Player const *const player);
mino_t get_hold_mino_type(Player const *const player);
Coodinate get_mino_coodinate(Player const *constplayer, const int i);
mino_t get_nth_next(Player const *const player, const int n);
void player_new(Player **const pplayer, Config *const pconf);
void player_free(Player **const pplayer);
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
Config *const get_playerconf(Player const *const player);
