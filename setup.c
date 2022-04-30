#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "mino.h"
#include "nexts.h"

#define MYCOLOR_BLACK COLOR_BLACK
#define MYCOLOR_RED COLOR_RED
#define MYCOLOR_CYAN COLOR_CYAN
#define MYCOLOR_BLUE COLOR_BLUE
#define MYCOLOR_YELLOW COLOR_YELLOW
#define MYCOLOR_MAGENTA COLOR_MAGENTA
#define MYCOLOR_GREEN COLOR_GREEN
#define MYCOLOR_ORANGE 12 // この数字は適当、多分8以上ならOK
#define MYCOLOR_GRAY 113

void initField(char **Field) {

    int i;

    *Field = (char*)malloc(sizeof(char) * 10 * 40);

    for (i = 0; i < 10*40; ++i) {
        (*Field)[i] = BLANK;
    }
}

void freeField(char **Field) {
    free(*Field);
    *Field = NULL;
}

void Init_Display(void) {

    /* ターミナルの画面制御開始 */
    initscr();

    /* 色一覧を定義 */
    start_color();
    init_color(MYCOLOR_ORANGE, 1000, 500, 0);
    init_color(MYCOLOR_GRAY, 250, 250, 250);

    init_pair(BLANK, COLOR_WHITE, MYCOLOR_BLACK);
    init_pair(IMINO, COLOR_WHITE, MYCOLOR_CYAN);
    init_pair(OMINO, COLOR_WHITE, MYCOLOR_YELLOW);
    init_pair(SMINO, COLOR_WHITE, MYCOLOR_GREEN);
    init_pair(ZMINO, COLOR_WHITE, MYCOLOR_RED);
    init_pair(JMINO, COLOR_WHITE, MYCOLOR_BLUE);
    init_pair(LMINO, COLOR_WHITE, MYCOLOR_ORANGE);
    init_pair(TMINO, COLOR_WHITE, MYCOLOR_MAGENTA);
    init_pair(OJAMA, COLOR_WHITE, MYCOLOR_GRAY);

    /* 入力を表示しないように、
    エンター無しで入力を受け取るように */
    noecho();
    cbreak();
    curs_set(0);

    /* 現在の端末画面をクリア */
    erase();

    init_MINOSarray();
    init_SRSoffsets();
}

void Init_Game(char **Field, Mino *pmino, Nexts *nexts) {

    /* 乱数初期化 */
    srand((unsigned char)time(NULL));

    /* ミノを置くフィールドを確保 */
    initField(Field);

    pmino->mino = -1;

    /* next配列の初期化 */
   init_nexts(nexts, 6); 
}

void Free_Game(char **Field, Nexts *nexts) {
    free(*Field);
    *Field = NULL;

    free_nexts(nexts);
}

void Free_Display(void) {
    endwin();
}
