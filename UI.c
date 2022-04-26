#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "setup.h"

void writefield(char ***Field) {

    int i, j, h, w;
    char mino_type;

    /* 1行目から20行目までの表示。
    行は1行目から、下から数える */
    for (h = 0; h < 20; ++h) {

        for (w = 0; w < 10; ++w) {

            mino_type = GET_TYPE(Field, h, w);

            // blank
            if (mino_type == BLANK) {
                attrset(COLOR_PAIR(mino_type));
                MVADDSTR(BLANK+20-h, BLANKLEFT+w, " .");
                continue;
            }
            // not blank
            attrset(COLOR_PAIR(mino_type));
            MVADDSTR(BLANKTOP+20-h, BLANKLEFT+w, "[]");

        }
    }

    /* 21行目、空白部分には点を置かない */
    for (w = 0; w < 10; ++w) {

        mino_type = GET_TYPE(Field, h, w);

        attrset(COLOR_PAIR(mono_type));
        MVADDSTR(BLANKTOP, BLANKLEFT+w, "[]");

    }
}

void white_curmino(Mino *pmino) {

    int i;

    if (pmino->mino < 0) return;

    for (i = 0; i < 4; ++i) {
        MVADDSTR(BLANKTOP+20
                    -(pmino->h+MINOSarray[pmino->mino][pmino->dir][i][0]),
                BLANKLEFT
                    +(pmino->w+MINOSarray[pmino->mino][pmino->dir][i][1]),
                "[]");
    }
}
