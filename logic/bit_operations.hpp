#pragma once
#include "constants.hpp"

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

int real_count(BB bitboard) {
    int count;
    for (count = 0; bitboard != 0; count++, bitboard &= bitboard-1);
    return count;
}

bool is_within_board(int pos) {
    return pos >= 0 && pos < 64;
}

void print_BB(BB bitboard) {
    for (int i = 63; i >= 0; i--) {
        int cur = bitboard >> i;

        if ((i+1) % 8 == 0) {
            std::cout << std::endl;
        }

        if (cur & 1) {
            std::cout << "1 ";
        }

        else {
            std::cout << "0 ";
        }
    }
    std::cout << std::endl;
}

void print_Move_bits(Move move) {
    for (int i = 15; i >= 0; i--) {
        int cur = move >> i;
        if (cur & 1) {
            std::cout << "1 ";
        }

        else {
            std::cout << "0 ";
        }
    }
    std::cout << std::endl;
}