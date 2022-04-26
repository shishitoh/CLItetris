#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "mino.h"

int is_empty(char *Field, char h, char w) {
    return 0 <= h && h < 40
        && 0 <= w && w < 10
        && GET_TYPE(Field, h, w) == BLANK;
}

int pop_check(char *Field, Mino *pmino) {

    int i;

    for (i = 0; i < 4; ++i) {
        if (is_empty(Field,
                     INITH+MINOSarray[pmino->mino][0][i][0],
                     INITW+MINOSarray[pmino->mino][0][i][1])) {

            continue;
        }
        return 0;
    }
    return 1;
}

/* movedirは右移動で1, 左移動で-1 */
int move_check(char *Field, Mino *pmino, int movedir) {

    int i = 0;

    for (i = 0; i < 4; ++i) {
        if (is_empty(Field,
                     pmino->h+MINOSarray[pmino->mino][pmino->dir][i][0],
                     pmino->w+MINOSarray[pmino->mino][pmino->dir][i][1]+movedir)) {

            continue;
        }
        return 0;
    }
    return 1;
}


int fall_check(char *Field, Mino *pmino) {

    int i;

    for (i = 0; i < 4; ++i) {
        if (is_empty(Field,
                     pmino->h+MINOSarray[pmino->mino][pmino->dir][i][0]-1,
                     pmino->w+MINOSarray[pmino->mino][pmino->dir][i][1])) {

            continue;
        }
        return 0;
    }
    return 1;
}

void put_mino(char *Field, Mino *pmino) {

    int i;

    for (i = 0; i < 4; ++i) {
        GET_TYPE(Field,
                 pmino->h+MINOSarray[pmino->mino][pmino->dir][i][0],
                 pmino->w+MINOSarray[pmino->mino][pmino->dir][i][1])
            = pmino->mino;
    }
    pmino->mino = -1;
}

/* rotationが不可能なとき-1を, 可能なときSRSの候補の数字を返す */
int rotate_check(char *Field, Mino *pmino, int rotate) {

    int i, j;

    int empty_all;

    for (i = 0; i < 5; ++i) {

        empty_all = 1;

        for (j = 0; j < 4; ++j) {
            if (is_empty(Field,
                         pmino->h+SRSoffsets[pmino->mino][pmino->dir+rotate<<2][i][0]
                             +MINOSarray[pmino->mino][pmino->dir][j][0],
                         pmino->w+SRSoffsets[pmino->mino][pmino->dir+rotate<<2][i][1]
                             +MINOSarray[pmino->mino][pmino->dir][j][1])) {

                continue;
            }
            empty_all = 0;
            break;
        }
        if (empty_all) {
            return i;
        }
    }
    return -1;
}

void deleterows(char *Field) {

    int i, brow = 0, trow = 0;
    int exists_empty;

    while (trow < 40) {

        exists_empty = 0;

        for (i = 0; i < 10; ++i) {
            if (GET_TYPE(Field, trow, i) == BLANK) {
                exists_empty = 1;
                break;
            }
        }

        if (exists_empty) {
            if (brow < trow) {
                for (i = 0; i < 10; ++i) {
                    GET_TYPE(Field, brow, i) = GET_TYPE(Field, trow, i);
                }
            }
            ++brow;
        }
        ++trow;
    }

    /* 余りを空白で埋める */
    while (brow < 40) {
        for (i = 0; i < 10; ++i) {
            GET_TYPE(Field, brow, i) = BLANK;
        }
        ++brow
    }
}

int random_mino(mino) {

    /* この方法だと厳密なランダムにはならない */
    switch (((unsigned int)rand()) % 7) {
        case 0:
            return IMINO;
        case 1:
            return OMINO;
        case 2:
            return SMINO;
        case 3:
            return ZMINO;
        case 4:
            return JMINO;
        case 5:
            return LMINO;
        case 6:
            return TMINO;
    }
}

void try_move_left(char *Field, Mino *pmino) {

    if (move_check(Field, pmino, -1)) {
        --(pmino->w);
    }
}

void try_move_down(char *Field, Mino *pmino) {

    if (fall_check(Field, pmino)) {
        --(pmino->h);
    } else {
        put_mino(Field, pmino);
    }
}

void hard_drop(char *Field, Mino *pmino) {
    /* 未実装 */
}

void try_move_right(char *Field, Mino *pmino) {

    if (move_check(Field, pmino, 1)) {
        ++(pmino->w);
    }
}
