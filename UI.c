#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "nexts.h"
#include "setup.h"
#include "mino.h"

#define BLANKTOP 2
#define BLANKLEFT 2

#define FIELDADDSTR(y, x, str) \
    mvaddstr(BLANKTOP+20-(y), \
             2*(BLANKLEFT+(x)), \
             (str))


void write_Field(char *Field, Mino *pmino) {

    int h, w;
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
        if (mino_type == BLANK) {
            attrset(COLOR_PAIR(mino_type));
            FIELDADDSTR(h, w, "  ");
        } else {
            attrset(COLOR_PAIR(mino_type));
            FIELDADDSTR(h, w, "[]");
        }
    }
}

void write_curmino(Mino *pmino) {

    int i;

    /* Fieldにミノがない場合は何もしない */
    if (pmino->mino < 0) return;

    for (i = 0; i < 4; ++i) {
        attrset(COLOR_PAIR(pmino->mino));
        FIELDADDSTR(pmino->h
                        +MINOSarray[pmino->mino][pmino->dir][i][0],
                    pmino->w
                        +MINOSarray[pmino->mino][pmino->dir][i][1],
                    "[]");
    }
}

static char mino_to_char(int mino) {
    
    switch(mino) {
        case IMINO: {
            return 'I';
        }
        case OMINO: {
            return 'O';
        }
        case SMINO: {
            return 'S';
        }
        case ZMINO: {
            return 'Z';
        }
        case JMINO: {
            return 'J';
        }
        case LMINO: {
            return 'L';
        }
        case TMINO: {
            return 'T';
        }
    }
    return 0;
}

void write_nexts(Nexts *nexts, int L) {

    int i;
    int mino;
    char ch;

    for (i = 0; i < L; ++i) {
        mino = Nget_element(nexts, i);
        ch = mino_to_char(mino);
        attrset(COLOR_PAIR(mino));
        mvaddch(BLANKTOP + 1 + 2*i,
                2*(BLANKLEFT+10+BLANKLEFT),
                ch);
    }
}

void write_all(char *Field, Mino *pmino, Nexts *nexts, int L) {
    write_Field(Field, pmino);
    write_curmino(pmino);
    write_nexts(nexts, L);
}

