#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "setup.h"
#include "mino.h"

#define BLANKTOP 2
#define BLANKLEFT 2

#define FIELDADDSTR(y, x, str) \\
    mvaddstr(BLANKTOP+20-(y), \\
             2*(BLANKLEFT+(x)), \\
             (str))

void write_curmino(Mino *pmino);

void write_Field(char *Field, Mino *pmino) {

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
                FIELDADDSTR(h, w, " .");
                continue;
            }
            // not blank
            attrset(COLOR_PAIR(mino_type));
            FIELDADDSTR(h, w, "[]");

        }
    }

    /* 21行目、空白部分には点を置かない */
    for (w = 0; w < 10; ++w) {

        mino_type = GET_TYPE(Field, h, w);
        if (mino_type != BLANK) {
            attrset(COLOR_PAIR(mono_type));
            FIELDADDSTR(h, w, "[]");
        }
    }

    write_curmino(pmino);
}

void write_curmino(Mino *pmino) {

    int i;

    /* Fieldにミノがない場合は何もしない */
    if (pmino->mino < 0) return;

    for (i = 0; i < 4; ++i) {
        FIELDADDSTR(pmino->h
                        +MINOSarray[pmino->mino][pmino->dir][i][0]
                    pmino->w
                        +MINOSarray[pmino->mino][pmino->dir][i][1],
                    "[]");
    }
}
