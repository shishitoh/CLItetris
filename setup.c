#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#include "mino.h"
#include "game.h"

#define MYCOLOR_BLACK COLOR_BLACK
#define MYCOLOR_RED COLOR_RED
#define MYCOLOR_CYAN COLOR_CYAN
#define MYCOLOR_BLUE COLOR_BLUE
#define MYCOLOR_YELLOW COLOR_YELLOW
#define MYCOLOR_MAGENTA COLOR_MAGENTA
#define MYCOLOR_GREEN COLOR_GREEN
#define MYCOLOR_ORANGE 12 // この数字は適当、多分8以上ならOK
#define MYCOLOR_GRAY 13

void Init_Display(void) {

    /* ターミナルの画面制御開始 */
    initscr();

    /* 色一覧を定義 */
    start_color();
    init_color(MYCOLOR_ORANGE, 1000, 500, 0);
    init_color(MYCOLOR_GRAY, 250, 250, 250);

    /* terminalの初期カラーを-1, -1に設定 */
    use_default_colors();

    init_pair(BLANK, -1, -1);
    init_pair(IMINO, -1, MYCOLOR_CYAN);
    init_pair(OMINO, -1, MYCOLOR_YELLOW);
    init_pair(SMINO, -1, MYCOLOR_GREEN);
    init_pair(ZMINO, -1, MYCOLOR_RED);
    init_pair(JMINO, -1, MYCOLOR_BLUE);
    init_pair(LMINO, -1, MYCOLOR_ORANGE);
    init_pair(TMINO, -1, MYCOLOR_MAGENTA);
    init_pair(OJAMA, -1, MYCOLOR_GRAY);

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
