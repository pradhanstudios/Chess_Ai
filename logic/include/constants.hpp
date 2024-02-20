#pragma once

#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

// eventually maybe try int_fast_64_t and fastint16_t
typedef uint64_t BB; // short for Bitboard
typedef uint16_t Move;


enum MOVE_TYPES {
    NORMAL_MOVE = 0,
    PROMOTION = 1,
    CASTLE = 2, // 0 for queenside and 1 for kingside
    EN_PESSANT = 3,
};

// bitboards

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