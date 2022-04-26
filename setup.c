#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "setup.h"

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

void Init(char **Field) {

    /* 乱数初期化 */
    srand((unsigned char)time(NULL));

    /* ターミナルの画面制御開始 */
    initscr();

    /* 色一覧を定義 */
    start_color();
    init_pair(BLANK, COLOR_WHITE, COLOR_BLACK);
    init_pair(IMINO, COLOR_WHITE, COLOR_CYAN);
    init_pair(OMINO, COLOR_WHITE, COLOR_YELLOW);
    init_pair(SMINO, COLOR_WHITE, COLOR_GREEN);
    init_pair(ZMINO, COLOR_WHITE, COLOR_RED);
    init_pair(JMINO, COLOR_WHITE, COLOR_BLUE);
    init_pair(LMINO, COLOR_WHITE, COLOR_BLACK);
    init_pair(TMINO, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(OJAMA, COLOR_WHITE, COLOR_GRAY);

    /* 入力を表示しないように、
    エンター無しで入力を受け取るように */
    noecho();
    cbreak();

    /* ミノを置くフィールドを確保 */
    initField(Field);

    /* 現在の端末画面をクリア */
    erase();
}

void Free(void) {
    endwin();
}
