#pragma once

#include "mino.h"

void initField(char **Field);
void freeField(char **Field);
void Init_Display(void);
void Init_Game(char **Field, Mino *pmino);
void Free(void);
