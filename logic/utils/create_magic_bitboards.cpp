/*
-------------------------------
--skip to 184 for actual code--
-------------------------------
*/
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

typedef uint64_t BB; // short for Bitboard

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


BB shift_back(BB bitboard, int back) {
    return back >= 0 ? (bitboard << back): (bitboard >> -back);
}

void set_bit_on(BB &bitboard, int index) {
    bitboard |= shift_back(1ULL, index);
}

void set_bit_off(BB &bitboard, int index) {
    bitboard &= ~shift_back(1ULL, index);
}

BB get_bit(BB bitboard, int index) {
    return bitboard & shift_back(1ULL, index);
}

int zeroes_end(BB bitboard) {
    return __builtin_clzll(bitboard);
}

int zeroes_start(BB bitboard) {
    return __builtin_ctzll(bitboard);
}

bool is_within_board(int pos) {
    return pos >= 0 && pos < 64;
}

BB blocker_mask_rook(int position) {
    BB blocker_mask_up = H_FILE << (position % 8);
    blocker_mask_up &= ~(RANK_1 | RANK_8);
    BB blocker_mask_side = RANK_1 << int(position / 8) * 8;
    blocker_mask_side &= ~(A_FILE | H_FILE);
    BB blocker_mask = blocker_mask_up | blocker_mask_side;
    // print_BB(blocker_mask);
    set_bit_off(blocker_mask, position);

    return blocker_mask;
}

BB blocker_mask_bishop(int position) {
    BB bitboard;
    int i = position; 
    while ((i % 8 != 0) && (i < 56)) {
        set_bit_on(bitboard, i);
        i += 7;
    }
    i = position;
    while ((i % 8 != 0) && (i > 7)) {
        set_bit_on(bitboard, i);
        i -= 9;
    }
    i = position;
    while ((i > 7) && (i % 8 != 7)) {
        set_bit_on(bitboard, i);
        i -= 7;
    }
    i = position;
    while ((i < 56) && (i % 8 != 7)) {
        set_bit_on(bitboard, i);
        i += 9;
    }
    set_bit_off(bitboard, position);
    return bitboard;
}

std::vector<BB> blocker_boards(BB blocker_mask) {
    std::vector<int> poss;
    BB noedge = blocker_mask & ~EDGES;

    for (int i = 0; i < 64; i++) {
        if (get_bit(noedge, i)) {
            poss.push_back(i);
        }
    }

    int max_num = (1 << poss.size()) - 1;

    std::vector<BB> output;
    BB cur_BB = 0ULL;
    int i = 0;
    while (i <= max_num) {
        cur_BB = 0ULL;
        for (int j = 0; j < poss.size(); j++) {
            if (i & (1 << j)) {
                set_bit_on(cur_BB, poss[j]);
            }
        }

        output.push_back(cur_BB);

        i++;
    }

    return output;
}

BB moveboard_bishop(BB blocker_board, int position) {
    BB moveboard = 0ULL;
    
    for (int dir : {9, 7, -7, -9}) {
        /*
        A = 9
        B = 7
        C = -9
        D = -7
        A0B
        0X0
        D0C
        */
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir == (7 || -9)) ? 7 : 0)) && (!get_bit(blocker_board, i))) {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}

BB moveboard_rook(BB blocker_board, int position) {
    BB moveboard = 0ULL;

    for (int dir : {8, -8, 1, -1}) {
        /*
        A = -1
        B = 1
        C = 8
        D = -8
        0C0
        AXB
        0D0
        */
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir < 0)) ? 7 : 0) && (!get_bit(blocker_board, i))) {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}

typedef struct magic_bitboard {
    BB mask;
    BB magic;
    int shift;

} magic_bitboard;

int calculate_index(BB input_bitboard, magic_bitboard magic) {
    return (input_bitboard ^ magic.magic) >> magic.shift;
}