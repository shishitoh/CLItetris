#pragma once

#define INITH 17
#define INITW 3

#define IMINO 0
#define OMINO 1
#define SMINO 2
#define ZMINO 3
#define JMINO 4
#define LMINO 5
#define TMINO 6
#define OJAMA 7
#define BLANK 8

#define BLANKTOP 2
#define BLANKLEFT 2

#define GET_TYPE(f, y, x) (f)[10*(y) + (x)]


typedef struct tetrimino {
    int mino, dir;
    char h, w;
} Mino;

void init_mino(Mino *pmino) {
    pmino->dir = 0;
    pmino->h = INITH;
    pmino->q = INITW;
}

/* ?MINO[向き][構成block][ブロックの相対位置]
向きについては初期の向きが0、そこから
時計回りするたびに1, 2, 3となる。 */
/* ブロックの相対位置は{h, w} */
char Iarray[4][4][2] = {
    {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
    {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}}
};

char Oarray[4][4][2] = {
    {{2, 1}, {2, 2}, {3, 1}, {3, 2}},
    {{2, 1}, {2, 2}, {3, 1}, {3, 2}},
    {{2, 1}, {2, 2}, {3, 1}, {3, 2}},
    {{2, 1}, {2, 2}, {3, 1}, {3, 2}},
};

char Sarray[4][4][2] = {
    {{2, 0}, {2, 1}, {3, 1}, {3, 2}},
    {{1, 2}, {2, 1}, {2, 2}, {3, 1}},
    {{1, 0}, {1, 2}, {2, 2}, {2, 3}},
    {{1, 1}, {2, 0}, {2, 1}, {3, 0}}
};

char Zarray[4][4][2] = {
    {{2, 1}, {2, 2}, {3, 0}, {3, 1}},
    {{1, 1}, {2, 1}, {2, 2}, {3, 2}},
    {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
    {{1, 0}, {2, 0}, {2, 1}, {3, 1}}
};

char Jarray[4][4][2] = {
    {{2, 0}, {2, 1}, {2, 2}, {3, 0}},
    {{1, 1}, {2, 1}, {3, 1}, {3, 2}},
    {{1, 2}, {2, 0}, {2, 1}, {2, 2}},
    {{1, 0}, {1, 1}, {2, 1}, {3, 1}}
};

char Larray[4][4][2] = {
    {{2, 0}, {2, 1}, {2, 2}, {3, 2}},
    {{1, 1}, {1, 2}, {2, 1}, {3, 1}},
    {{1, 0}, {2, 0}, {2, 1}, {2, 2}},
    {{1, 1}, {2, 1}, {3, 0}, {3, 1}}
};

char Tarray[4][4][2] = {
    {{2, 0}, {2, 1}, {2, 2}, {3, 1}},
    {{1, 1}, {2, 1}, {2, 2}, {3, 1}},
    {{1, 1}, {2, 0}, {2, 1}, {2, 2}},
    {{1, 2}, {2, 0}, {2, 1}, {3, 1}}
};

/* SRSのズレの一覧配列,8の内0, 1, 2, 3は
それぞれの向きからの時計回り回転時のoffset、
4, 5, 6, 7はそれぞれ0, 1, 2, 3の向きからの
半時計回り回転時のoffset */
char Ioffsets[8][5][2] = {
    {{0, 0}, {0, -2}, {0, 1}, {-1, -2}, {2, 1}},
    {{0, 0}, {0, -1}, {0, 2}, {2, -1}, {-1, 2}},
    {{0, 0}, {0, 2}, {0, -1}, {1, 2}, {-2, -1}},
    {{0, 0}, {0, -2}, {0, 1}, {-2, 1}, {1, -2}},
    {{0, 0}, {0, -1}, {0, 2}, {2, -1}, {-1, 2}},
    {{0, 0}, {0, 2}, {0, -1}, {1, 2}, {-2, -1}},
    {{0, 0}, {0, 1}, {0, -2}, {-2, 1}, {1, -2}},
    {{0, 0}, {0, 1}, {0, -2}, {-1, -2}, {2, 1}},
}

char NOTIoffsets[8][5][2] = {
    {{0, 0}, {0, -1}, { 1, -1}, {-2, 0}, {-2, -1}},
    {{0, 0}, {0,  1}, {-1,  1}, { 2, 0}, { 2,  1}},
    {{0, 0}, {0,  1}, { 1,  1}, {-2, 0}, {-2,  1}},
    {{0, 0}, {0, -1}, {-1, -1}, { 2, 0}, { 2, -1}},
    {{0, 0}, {0,  1}, { 1,  1}, {-2, 0}, {-2,  1}},
    {{0, 0}, {0,  1}, {-1,  1}, { 2, 0}, { 2,  1}},
    {{0, 0}, {0, -1}, { 1, -1}, {-2, 0}, {-2, -1}},
    {{0, 0}, {0, -1}, {-1, -1}, { 2, 0}, { 2, -1}},
}

char ***MINOSarray[16];
MINOSarray[IMINO] = Iarray;
MINOSarray[OMINO] = Oarray;
MINOSarray[SMINO] = Sarray;
MINOSarray[ZMINO] = Zarray;
MINOSarray[JMINO] = Jarray;
MINOSarray[LMINO] = Larray;
MINOSarray[TMINO] = Tarray;

char ***SRSoffsets[16];
SRSoffsets[IMINO] = Ioffsets;
SRSoffsets[OMINO] = NOTIoffsets;
SRSoffsets[SMINO] = NOTIoffsets;
SRSoffsets[ZMINO] = NOTIoffsets;
SRSoffsets[JMINO] = NOTIoffsets;
SRSoffsets[LMINO] = NOTIoffsets;
SRSoffsets[TMINO] = NOTIoffsets;
