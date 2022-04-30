#pragma once

#define INITH 17
#define INITW 3

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

#define GET_TYPE(f, y, x) (f)[10*(y) + (x)]

typedef struct tetrimino {
    int mino, dir;
    char h, w;
} Mino;

void init_mino(Mino *pmino);

/* ?MINO[向き][構成block][ブロックの相対位置]
向きについては初期の向きが0、そこから
時計回りするたびに1, 2, 3となる。 */
/* ブロックの相対位置は{h, w} */
extern const char (*MINOSarray[16])[4][2];
void init_MINOSarray(void);

extern const char (*SRSoffsets[16])[5][2];
void init_SRSoffsets(void);
