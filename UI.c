#include <ncurses.h>
#include <unistd.h>

#include "game.h"
#include "setup.h"
#include "UI.h"

#define BLANKTOP 2
#define BLANKLEFT 4

#define FIELDADDSTR(y, x, str) \
    mvaddstr(BLANKTOP+20-(y), \
             2*(BLANKLEFT+(x)), \
             (str))


static void write_Field(Player const *const player) {

    int h, w;
    mino_t mino_type;

    /* 1行目から20行目までの表示。
    行は1行目から、下から数える */
    for (h = 0; h < 20; ++h) {

        for (w = 0; w < 10; ++w) {

            mino_type = block(player, h, w);

            attrset(COLOR_PAIR(mino_type));
            if (mino_type == BLANK) {
                FIELDADDSTR(h, w, " .");
            } else {
                FIELDADDSTR(h, w, "[]");
            }
        }
    }

    /* 21行目、空白部分には点を置かない */
    for (w = 0; w < 10; ++w) {

        mino_type = block(player, h, w);
        attrset(COLOR_PAIR(mino_type));
        if (mino_type == BLANK) {
            FIELDADDSTR(h, w, "  ");
        } else {
            FIELDADDSTR(h, w, "[]");
        }
    }
}

static void write_curmino(Player const *const player) {

    int i;
    mino_t mino_type;
    Coodinate cdn;

    /* 操作ミノがない場合は何もしない */
    if ((mino_type = get_mino_type(player)) == UNDEF) return;

    attrset(COLOR_PAIR(mino_type));
    for (i = 0; i < 4; ++i) {
        cdn = get_mino_coodinate(player, i);
        if (cdn.h < 21) {
            FIELDADDSTR(cdn.h, cdn.w, "[]");
        }
    }
}

static void write_ghost(Player const *const player) {

    int i;
    int is_fallable;
    Coodinate cdns[4];

    for (i = 0; i < 4; ++i) {
        cdns[i] = get_mino_coodinate(player, i);
    }

    is_fallable = 1;

    while (is_fallable) {

        for (i = 0; i < 4; ++i) {
            // -1を見落とさないように
            if (!is_blank(player, cdns[i].h-1, cdns[i].w)) {
                is_fallable = 0;
                break;
            }
        }

        if (is_fallable) {
            for (i = 0; i < 4; ++i) {
                --cdns[i].h;
            }
        }
    }

    attrset(COLOR_PAIR(get_mino_type(player)) | A_BLINK | A_DIM);
    for (i = 0; i < 4; ++i) {
        if (cdns[i].h < 21) {
            FIELDADDSTR(cdns[i].h, cdns[i].w, "[]");
        }
    }
}

static inline char mino_to_char(mino_t mino) {

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

static void write_nexts(Player const *const player) {

    int i;
    mino_t mino;
    char ch;

    for (i = 0; i < get_playerconf(player)->next; ++i) {
        mino = get_nth_next(player, i);
        ch = mino_to_char(mino);
        attrset(COLOR_PAIR(mino));
        mvaddch(BLANKTOP + 1 + 2*i,
                2*(BLANKLEFT+FIELDW+2),
                ch);
    }
}

static void write_hold(Player const *const player) {

    mino_t mino_type;

    if (!(get_playerconf(player)->hold)) {
        return;
    }

    mino_type = get_hold_mino_type(player);

    if (mino_type == UNDEF) {
        attrset(COLOR_PAIR(BLANK));
        mvaddch(BLANKTOP+1, 2*(BLANKLEFT-1), '-');
    } else {
        attrset(COLOR_PAIR(mino_type));
        mvaddch(BLANKTOP+1, 2*(BLANKLEFT-1), mino_to_char(mino_type));
    }
}

void write_all(Player const *const player) {
    write_Field(player);
    write_ghost(player);
    write_curmino(player);
    write_nexts(player);
    write_hold(player);
}
