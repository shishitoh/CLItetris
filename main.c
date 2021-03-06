#include <string.h>
#include <ncurses.h>

#include "setup.h"
#include "UI.h"
#include "game.h"

int main(void) {

    int ch = 0;
    Player player;
    Config conf;

    Init_Display();
    get_config_data(&conf);

    /* 初期化 */
    while (1) {

        /* nextの数, 自動落下間隔(ms) */
        init_player(&player, conf.next, conf.fall);
        nodelay(stdscr, TRUE);

        /* ゲーム中 */
        while (1) {

            write_all(&player, &conf);
            refresh();

            if (is_gameover(&player)) {
                break;
            }

            napms(16);

            ch = getch();

            switch(ch) {
                case 'h': { /* left */
                    key_mov_left(&player);
                    break;
                }
                case 'j': { /* down */
                    key_soft_drop(&player);
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
                case 'a': { /* hold */
                    if (conf.hold) {
                        key_hold(&player);
                        break;
                    }
                }
                default: {
                    break;
                }
            }
            auto_fall(&player);
            rock_down_put_mino(&player);

        }
        free_player(&player);
        nodelay(stdscr, FALSE);
        getch();
    }
    Free_Display();

    return 0;
}
