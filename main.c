#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "mino.h"
#include "setup.h"

int main(void) {

    int ch;
    char *Field;
    Mino smino;

    /* 初期化 */
    while (1) {

        Init(&Field);

        /* ゲーム中 */
        while (1) {

            /* ミノがなければ生成する,
            生成できなければゲームオーバー */
            if (smino.mino < 0) {
                smino.mino = random_mino();
                if (pop_check(Field, smino.mino)) {
                    init_mino(&smino);
                } else {
                    break;
                }
            }

            ch = getch();
            /* vim_like key bind */
            switch(ch) {
                case h: { /* left */
                    try_move_left(Field, &smino);
                    break;
                }
                case j: { /* down */
                    move_down(Field, &smino);
                    break;
                }
                case k: { /* up */
                    move_up(Field, &smino);
                    break;
                }
                case l: { /* right */
                    move_right(Field, &smino);
                    break;
                }
                default: {
                    continue;
                }
            }

            whiteField(Field);
        }

        Free();
    }

    return 0;
}
