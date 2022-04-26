#pragma once

#include "mino.h"

int is_empty(char *field, char h, char w);
int pop_chrck(char *Field, Mino *pmino);
int move_check(char *Field, Mino *pmino, int movedir);
int fall_check(char *Field, Mino *pmino);
void put_mino(char *Field, Mino *pmino);
int rotate_check(char *Field, Mino *pmino, int rotate);
void delterows(char *Field);
int random_mino(void);
void try_move_left(char *Field, Mino *pmino);
void try_move_down(char *Field, Mino *pmino);
void hard_drop(char *Field, Mino *pmino);
void try_move_right(char *Field, Mino *pmino);
