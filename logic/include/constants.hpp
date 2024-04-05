#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

// eventually maybe try int_fast_64_t and fastint16_t
typedef uint64_t BB; // short for Bitboard

enum MOVE_TYPES {
    NORMAL_MOVE = 0b0000,
    PROMOTION = 0b0001,
    CASTLE = 0b0010, // 0 for queenside and 1 for kingside
    EN_PESSANT = 0b0100,
};

extern const int MAX_LEGAL_MOVES;
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