#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


/*
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
*/

#include "precompiled_libs.hpp" // speed up compile time


typedef uint64_t BB; // short for Bitboard

enum MOVE_TYPES {
    NORMAL_MOVE = 0b0000,
    PROMOTION = 0b0001,
    CASTLE = 0b0010, // 0 for queenside and 1 for kingside
    EN_PESSANT = 0b0100,
};

enum FILE_RANK {
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8,
};

const extern int INF;
const extern int NEGINF;

extern const int MAX_LEGAL_MOVES;
extern const int MAX_CAPTURE_MOVES;
extern const unsigned int FIRST_SIX;
// bitboards

extern const BB MAX_VALUE;
extern const BB A_FILE; // represent a file
extern const BB B_FILE;
extern const BB C_FILE;
extern const BB D_FILE;
extern const BB E_FILE;
extern const BB F_FILE;
extern const BB G_FILE;
extern const BB H_FILE;

extern const BB RANK_1; // represent first rank
extern const BB RANK_2;
extern const BB RANK_3;
extern const BB RANK_4;
extern const BB RANK_5;
extern const BB RANK_6;
extern const BB RANK_7;
extern const BB RANK_8;

extern const BB EDGES;

extern const BB TOPLEFTTORIGHT;
extern const BB BOTLEFTTORIGHT;
#endif // CONSTANTS_HPP