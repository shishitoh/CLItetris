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

#define DIR_N 0
#define DIR_E 1
#define DIR_S 2
#define DIR_W 3
#define ROT_R 0
#define ROT_L 1

/* SRSoffsetsにおけるmino配列の持ち方と
上のマクロ6つの値に依存 */
#define SRS_ROT(dir, rot) ( (dir) + ((rot) << 2) )

typedef int mino_t;

extern const char (*MINOSarray[])[4][2];

extern const char (*SRSoffsets[])[5][2];
