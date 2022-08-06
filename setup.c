#include <ncurses.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setup.h"
#include "mino.h"
#include "game.h"

#define MYCOLOR_BLACK 16
#define MYCOLOR_WHITE 17
#define MYCOLOR_RED 18
#define MYCOLOR_CYAN 19
#define MYCOLOR_BLUE 20
#define MYCOLOR_YELLOW 21
#define MYCOLOR_MAGENTA 22
#define MYCOLOR_GREEN 23
#define MYCOLOR_ORANGE 24
#define MYCOLOR_GRAY 25

void Init_Display(void) {

    /* ターミナルの画面制御開始 */
    initscr();

    /* 色一覧を定義 */
    start_color();
    init_color(MYCOLOR_BLACK, 0, 0, 0);
    init_color(MYCOLOR_WHITE, 1000, 1000, 1000);
    init_color(MYCOLOR_RED, 1000, 0, 0);
    init_color(MYCOLOR_CYAN, 0, 1000, 1000);
    init_color(MYCOLOR_BLUE, 0, 0, 1000);
    init_color(MYCOLOR_YELLOW, 1000, 1000, 0);
    init_color(MYCOLOR_MAGENTA, 500, 0, 500);
    init_color(MYCOLOR_GREEN, 0, 1000, 0);
    init_color(MYCOLOR_ORANGE, 1000, 625, 0);
    init_color(MYCOLOR_GRAY, 250, 250, 250);

    /* terminalの初期カラーを-1, -1に設定 */
    use_default_colors();

    init_pair(BLANK, -1, -1);
    init_pair(IMINO, MYCOLOR_WHITE, MYCOLOR_CYAN);
    init_pair(OMINO, MYCOLOR_WHITE, MYCOLOR_YELLOW);
    init_pair(SMINO, MYCOLOR_WHITE, MYCOLOR_GREEN);
    init_pair(ZMINO, MYCOLOR_WHITE, MYCOLOR_RED);
    init_pair(JMINO, MYCOLOR_WHITE, MYCOLOR_BLUE);
    init_pair(LMINO, MYCOLOR_WHITE, MYCOLOR_ORANGE);
    init_pair(TMINO, MYCOLOR_WHITE, MYCOLOR_MAGENTA);
    init_pair(OJAMA, MYCOLOR_WHITE, MYCOLOR_GRAY);

    /* 入力を表示しないように、
    エンター無しで入力を受け取るように */
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);

    /* 現在の端末画面をクリア */
    erase();

    /* 乱数初期化 */
    srand((unsigned char)time(NULL));
}

void Free_Display(void) {
    endwin();
}

int get_config_data(Config *conf) {

    /* デフォルト値 */
    conf->fall = 500;
    conf->next = 5;
    conf->hold = 1;

    FILE* fp = fopen("./config", "r");
    if (fp == NULL) {
        return 0;
    }

    char buf[256], fi[256];
    int se;

    while(1) {

        /* error処理 */
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            if (feof(fp)) {
                break;
            } else {
                return 0;
            }
        }

        /* コメントアウトチェック */
        if (buf[0] == '#') {
            continue;
        } else {
            /* 正常に読み込めなかった場合 */
            if (sscanf(buf, "%s%d", fi, &se) != 2) {
             return 0;
            } else {
                if (strcmp(fi, "fall") == 0) {
                    conf->fall = se;
                } else if (strcmp(fi, "next") == 0) {
                    conf->next = se;
                } else if (strcmp(fi, "hold") == 0) {
                    conf->hold = se;
                }
            }
        }
    }

    if (fclose(fp) == EOF) {
        return 0;
    }

    return 1;
}
