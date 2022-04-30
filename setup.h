#pragma once

#include "mino.h"
#include "nexts.h"

void initField(char **Field);
void freeField(char **Field);
void Init_Display(void);
void Init_Game(char **Field, Mino *pmino, Nexts *nexts);
void Free_Game(char **Field, Nexts *nexts);
void Free_Display(void);
