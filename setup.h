#pragma once

#include "mino.h"

void Init_Display(void);
void Free_Display(void);

typedef struct {
    int fall;
    int next;
    int hold;
} Config;

int get_config_data(Config *conf);
