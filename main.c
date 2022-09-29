#include <string.h>
#include <ncurses.h>

#include "setup.h"
#include "UI.h"
#include "game.h"

int main(void) {

    int ch = 0;
    Player *player = NULL;
    Config conf;

    Init_Display();
    if (!get_config(&conf)) {
        Free_Display();
        return 1;
    }

    /* 初期化 */
    while (1) {

        /* nextの数, 自動落下間隔(ms) */
        player_new(&player, &conf);
        nodelay(stdscr, TRUE);

        /* ゲーム中 */
        while (1) {

            write_all(player);
            refresh();

            if (is_gameover(player)) {
                break;
            }

            napms(16);

            ch = getch();

            switch(ch) {
                case 'h': { /* left */
                    key_mov_left(player);
                    break;
                }
                case 'j': { /* down */
                    key_soft_drop(player);
                    break;
                }
                case 'k': { /* up */
                    key_hard_drop(player);
                    break;
                }
                case 'l': { /* right */
                    key_mov_right(player);
                    break;
                }
                case 's': { /* 反時計 */
                    key_rotation_left(player);
                    break;
                }
                case 'd': { /* 時計 */
                    key_rotation_right(player);
                    break;
                }
                case 'a': { /* hold */
                    if (conf.hold) {
                        key_hold(player);
                        break;
                    }
                }
                default: {
                    break;
                }
            }
            auto_fall(player);
            rock_down_put_mino(player);

        }
        player_free(&player);
        nodelay(stdscr, FALSE);
        getch();
    }
    Free_Display();

    return 0;
}
