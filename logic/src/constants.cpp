#include "constants.hpp"

// bitboards

const Move FIRST_FOUR = 0b1111;
const Move FIRST_SIX = 0b111111;

const BB A_FILE = 0x8080808080808080ULL; // represent a file
const BB B_FILE = A_FILE >> 1;
const BB C_FILE = A_FILE >> 2;
const BB D_FILE = A_FILE >> 3;
const BB E_FILE = A_FILE >> 4;
const BB F_FILE = A_FILE >> 5;
const BB G_FILE = A_FILE >> 6;
const BB H_FILE = A_FILE >> 7;

const BB RANK_1 = 0x00000000000000FFULL; // represent first rank
const BB RANK_2 = RANK_1 << 8;
const BB RANK_3 = RANK_1 << 16;
const BB RANK_4 = RANK_1 << 24;
const BB RANK_5 = RANK_1 << 32;
const BB RANK_6 = RANK_1 << 40;
const BB RANK_7 = RANK_1 << 48;
const BB RANK_8 = RANK_1 << 56;

const BB EDGES = A_FILE | H_FILE | RANK_1 | RANK_8;

const BB TOPLEFTTORIGHT = 9241421688590303745ULL;
const BB BOTLEFTTORIGHT = 72624976668147840ULL;