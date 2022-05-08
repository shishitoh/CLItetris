#include <string.h>
#include <ncurses.h>

#include "setup.h"
#include "UI.h"
#include "game.h"

int main(void) {

    int ch;
    Player player;

    Init_Display();
    fprintf(stderr, "Init_Display\n");

    /* 初期化 */
    while (1) {

        fprintf(stderr, "before init_player\n");
        init_player(&player, 5);
        fprintf(stderr, "init_player\n");

        /* ゲーム中 */
        while (1) {

            write_all(&player);
            refresh();

            ch = getch();

            switch(ch) {
                case 'h': { /* left */
                    key_mov_left(&player);
                    break;
                }
                case 'j': { /* down */
                    key_mov_down(&player);
                    break;
                }
                case 'k': { /* up */
                    key_hard_drop(&player);
                    break;
                }
                case 'l': { /* right */
                    key_mov_right(&player);
                    break;
                }
                case 's': { /* 反時計 */
                    key_rotation_left(&player);
                    break;
                }
                case 'd': { /* 時計 */
                    key_rotation_right(&player);
                    break;
                }
                case 'a': {
                    key_hold(&player);
                    break;
                }
                default: {
                    continue;
                }
            }

            if (is_gameover(&player)) {
                break;
            }
        }
    }
    Free_Display();

    return 0;
}
