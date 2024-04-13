#include "bit_operations.hpp"

// #define shift_back(bitboard, back) (back > 0 ? (bitboard << back): (bitboard >> -back))
// #define set_bit_on(bitboard, index) (bitboard |= shift_back(1ULL, index))
// #define set_bit_off(bitboard, index) (bitboard &= ~shift_back(1ULL, index);)
// #define fast_reverse_bit(bitboard, index) (bitboard ^= 1ULL << index;)
// #define get_bit(bitboard, index) (bitboard &= ~(1ULL << index))
// #define zeroes_end(bitboard) (__builtin_clzll(bitboard))
// #define zeroes_start(bitboard) (__builtin_ctzll(bitboard))
// #define is_within_board(pos) (pos >= 0 && pos < 64)
// #define rank(pos) (int(pos / 8))
// #define file(pos) (pos & 7)

// BB shift_back(BB bitboard, int back){
//     return back > 0 ? (bitboard << back): (bitboard >> -back);
// }

// void set_bit_on(BB &bitboard, int index) {
//     bitboard |= shift_back(1ULL, index);
// }

// void set_bit_off(BB &bitboard, int index) {
//     bitboard &= ~shift_back(1ULL, index);
// }

// void fast_reverse_bit(BB &bitboard, int index) { // xor is supposed to be faster
//     bitboard ^= 1ULL << index;
// }

// BB get_bit(BB bitboard, int index) {
//     // return bitboard & (1ULL << index);
//     return (bitboard >> index) & 1ULL;
// }

// int zeroes_end(BB bitboard) {
//     return __builtin_clzll(bitboard);
// }

// int zeroes_start(BB bitboard) {
//     return __builtin_ctzll(bitboard);
// }

// inline int pop_first_one(BB &bitboard) {
//     int first = zeroes_start(bitboard);
//     bitboard &= bitboard - 1;
//     return first;
// }

// int real_count(BB bitboard) {
//     int count;
//     // while (bitboard) {
//     //     count++;
//     //     bitboard &= (bitboard - 1);
//     // }
//     for (count = 0; bitboard != 0; count++, bitboard &= bitboard-1);
//     return count;
// }

// bool is_within_board(int pos) {
//     return pos >= 0 && pos < 64;
// }

void print_BB(const BB &bitboard) {
    int cur;
    for (int i = 63; i >= 0; i--) {
        cur = bitboard >> i;

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

// int file(int pos) {
//     return pos / 8; // auto cast to int
// }

// int rank(int pos) {
//     return pos & 7;
// }