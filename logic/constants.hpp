#pragma once

#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

// eventually maybe try int_fast_64_t
typedef uint64_t BB; // short for Bitboard
typedef struct Move {
    unsigned int data : 20;
} Move;


enum MOVE_TYPES {
    NORMAL_MOVE,
    PROMOTION,
    CASTLE,
    EN_PESSANT,
};

// bitboards

const BB A_FILE; // represent a file
const BB B_FILE;
const BB C_FILE;
const BB D_FILE;
const BB E_FILE;
const BB F_FILE;
const BB G_FILE;
const BB H_FILE;

const BB RANK_1; // represent first rank
const BB RANK_2;
const BB RANK_3;
const BB RANK_4;
const BB RANK_5;
const BB RANK_6;
const BB RANK_7;
const BB RANK_8;

const BB EDGES;

const BB TOPLEFTTORIGHT;
const BB BOTLEFTTORIGHT;