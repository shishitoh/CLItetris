#include <stdio.h>
#include <string.h>

#include "config.h"

int get_config(Config *conf) {

    /* デフォルト値 */
    conf->fall = 500;
    conf->next = 5;
    conf->hold = 1;

    FILE* fp = fopen("./config", "r");
    if (fp == NULL) {
        return 0;
    }

    char buf[256], fi[256];
    int se;

    while(1) {

        /* error処理 */
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            if (feof(fp)) {
                break;
            } else {
                return 0;
            }
        }

        /* コメントアウトチェック */
        if (buf[0] == '#') {
            continue;
        } else {
            /* 正常に読み込めなかった場合 */
            if (sscanf(buf, "%s%d", fi, &se) != 2) {
             return 0;
            } else {
                if (strcmp(fi, "fall") == 0) {
                    conf->fall = se;
                } else if (strcmp(fi, "next") == 0) {
                    conf->next = se;
                } else if (strcmp(fi, "hold") == 0) {
                    conf->hold = se;
                }
            }
        }
    }

    if (fclose(fp) == EOF) {
        return 0;
    }

    return 1;
}
