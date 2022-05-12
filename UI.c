#include <ncurses.h>
#include <unistd.h>

#include "game.h"

#define BLANKTOP 2
#define BLANKLEFT 4

#define FIELDADDSTR(y, x, str) \
    mvaddstr(BLANKTOP+20-(y), \
             2*(BLANKLEFT+(x)), \
             (str))


void write_Field(Player const *const player) {

    int h, w;
    mino_t mino_type;

    /* 1行目から20行目までの表示。
    行は1行目から、下から数える */
    for (h = 0; h < 20; ++h) {

        for (w = 0; w < 10; ++w) {

            mino_type = BLOCK(player, h, w);

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

        mino_type = BLOCK(player, h, w);
        if (mino_type == BLANK) {
            attrset(COLOR_PAIR(mino_type));
            FIELDADDSTR(h, w, "  ");
        } else {
            attrset(COLOR_PAIR(mino_type));
            FIELDADDSTR(h, w, "[]");
        }
    }
}

void write_curmino(Player const *const player) {

    int i;
    int _h;

    /* Fieldにミノがない場合は何もしない */
    if (player->mino == UNDEF) return;

    for (i = 0; i < 4; ++i) {
        _h = player->minoh+MINOSarray[player->mino][player->mino_dir][i][0];
        if (_h < 21) {
            attrset(COLOR_PAIR(player->mino));
            FIELDADDSTR(player->minoh
                            +MINOSarray[player->mino][player->mino_dir][i][0],
                        player->minow
                            +MINOSarray[player->mino][player->mino_dir][i][1],
                        "[]");
        }
    }
}

void write_ghost(Player const *const player) {

    int i, _h, _w, _hh;
    int all_empty;
    _h = player->minoh;
    _w = player->minow;

    while (1) {

        all_empty = 1;

        for (i = 0; i < 4; ++i) {
            if (!is_blank(player,
                          _h+MINOSarray[player->mino][player->mino_dir][i][0],
                          _w+MINOSarray[player->mino][player->mino_dir][i][1])) {

                all_empty = 0;
                break;
            }
        }

        if (!all_empty) {
            ++_h;
            break;
        }
        --_h;
    }

    attrset(COLOR_PAIR(player->mino) | A_BLINK | A_DIM);
    for (i = 0; i < 4; ++i) {
        _hh = _h+MINOSarray[player->mino][player->mino_dir][i][0];
        if (_hh < 21) {
            FIELDADDSTR(_h+MINOSarray[player->mino][player->mino_dir][i][0],
                        _w+MINOSarray[player->mino][player->mino_dir][i][1],
                        "[]");
        }
    }
}

char mino_to_char(mino_t mino) {

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

void write_nexts(Player const *const player) {

    int i;
    mino_t mino;
    char ch;

    for (i = 0; i < player->nextlen; ++i) {
        mino = get_nth_next(player, i);
        ch = mino_to_char(mino);
        attrset(COLOR_PAIR(mino));
        mvaddch(BLANKTOP + 1 + 2*i,
                2*(BLANKLEFT+FIELDW+2),
                ch);
    }
}

void write_hold(Player const *const player) {

    if (player->hold_mino == UNDEF) {
        attrset(COLOR_PAIR(BLANK));
        mvaddch(BLANKTOP+1, 2*(BLANKLEFT-1), '-');
    } else {
        attrset(COLOR_PAIR(player->hold_mino));
        mvaddch(BLANKTOP+1, 2*(BLANKLEFT-1), mino_to_char(player->hold_mino));
    }
}

void write_all(Player const *const player) {
    write_Field(player);
    write_ghost(player);
    write_curmino(player);
    write_nexts(player);
    write_hold(player);
}
