#pragma once
#include "constants.hpp"

BB shift_back(BB bitboard, int back);

void set_bit_on(BB &bitboard, int index);

void set_bit_off(BB &bitboard, int index);

void fast_reverse_bit(BB &bitboard, int index);

BB get_bit(BB bitboard, int index);

int zeroes_end(BB bitboard);

int zeroes_start(BB bitboard);

int pop_first_one(BB &bitboard);

int real_count(BB bitboard);

bool is_within_board(int pos);

void print_BB(BB bitboard);

void print_Move_bits(Move move);

int file(int pos);

int rank(int pos);

