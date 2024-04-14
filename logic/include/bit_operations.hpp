#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#ifdef _WIN32
#include <intrin.h>
#endif

#include "constants.hpp"
#include "move.hpp"

// #define cool(x, y)
inline std::array<BB, 64> SQUARE_TO_BB;
// #define shift_back(bitboard, back)
// #define set_bit_on(bitboard, index)
// #define set_bit_off(bitboard, index)
// #define fast_reverse_bit(bitboard, index)
// #define get_bit(bitboard, index)
// #define zeroes_end(bitboard)
// #define zeroes_start(bitboard)
// #define is_within_board(pos)
// #define rank(pos)
// #define file(pos)

constexpr int real_count(const BB &bitboard) {
    #ifndef _WIN32 // not windows
    return __builtin_popcountll(bitboard);
    #else
    return __popcnt64(bitboard);
    #endif
    
}

constexpr int zeroes_start(const BB &bitboard) {
    #ifndef _WIN32 // not windows
    return __builtin_ctzll(bitboard);
    #else
    return _tzcnt_u64(bitboard); 
    #endif
}

constexpr int pop_first_one(BB &bitboard) {
    int first = zeroes_start(bitboard);
    bitboard &= bitboard - 1;
    return first;
}

void print_BB(const BB &bitboard);

constexpr BB shift_up(const BB &bitboard, const int &up) { // used for pawn move generation
    if (up < 0) {
        return bitboard >> -up;
    }
    return (bitboard << up);
}

constexpr void set_bit_on(BB &bitboard, const int &index) {
    bitboard |= SQUARE_TO_BB[index];
}

constexpr void set_bit_off(BB &bitboard, const int &index) {
    bitboard &= ~SQUARE_TO_BB[index];
}

constexpr void fast_reverse_bit(BB &bitboard, const int &index) {
    bitboard ^= SQUARE_TO_BB[index];
}

constexpr BB get_bit(const BB &bitboard, const int &index) {
    return bitboard & SQUARE_TO_BB[index];
    // return (bitboard >> index) & 1ULL;
}

constexpr bool is_within_board(const int &pos) {
    return pos >= 0 && pos < 64;
}

// pos = 0b11111 = 63
// first three bits tell use file, last three tell us rank

constexpr int file(const int &pos) {
    return pos & 7; // last three bits
}

constexpr int rank(const int &pos) {
    return pos >> 3; // first three bits
}

#endif // BIT_OPERATIONS_HPP