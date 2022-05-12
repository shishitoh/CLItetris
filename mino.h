#pragma once

/* memo: SRS = super rotation system */

/* これに対応する画面出力はしない */
#define UNDEF -1

/* 0はなし、init_pairを定義するときに困るので */
#define IMINO 1
#define OMINO 2
#define SMINO 3
#define ZMINO 4
#define JMINO 5
#define LMINO 6
#define TMINO 7
#define OJAMA 8
#define BLANK 9

typedef int mino_t;

extern const char (*MINOSarray[])[4][2];

extern const char (*SRSoffsets[])[5][2];
