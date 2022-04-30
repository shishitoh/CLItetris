#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "setup.h"
#include "UI.h"
#include "core.h"
#include "nexts.h"

int main(void) {

    int ch;
    int hold_mino;
    int do_hold;
    char *Field;
    Mino smino;
    Nexts nexts;

    Init_Display();

    /* 初期化 */
    while (1) {

        Init_Game(&Field, &smino, &nexts, &hold_mino);

        /* ゲーム中 */
        while (1) {

            /* ミノがなければ生成する,
            生成できなければゲームオーバー */
            if (smino.mino < 0) {
                smino.mino = pop_next(&nexts);
                if (pop_check(Field, &smino)) {
                    init_mino(&smino);
                } else {
                    break;
                }
            }

            write_all(Field, &smino, &nexts, 5, hold_mino);
            refresh();

            ch = getch();
            /* vim_like key bind */
            switch(ch) {
                case 'h': { /* left */
                    try_move_left(Field, &smino);
                    break;
                }
                case 'j': { /* down */
                    try_move_down(Field, &smino, &do_hold);
                    break;
                }
                case 'k': { /* up */
                    hard_drop(Field, &smino, &do_hold);
                    break;
                }
                case 'l': { /* right */
                    try_move_right(Field, &smino);
                    break;
                }
                case 's': { /* 半時計 */
                    try_rotate(Field, &smino, 1);
                    break;
                }
                case 'd': { /* 時計 */
                    try_rotate(Field, &smino, 0);
                    break;
                }
                case 'a': {
                    hold(&hold_mino, &smino, &do_hold);
                    break;
                }
                default: {
                    continue;
                }
            }
            deleterows(Field);
        }

        Free_Game(&Field, &nexts);
    }
    Free_Display();

    return 0;
}
