#ifndef NON_SLIDING_MOVES_HPP
#define NON_SLIDING_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"

inline std::array<BB, 2> PAWN_DOUBLE_RANK_LOOKUP = {RANK_7, RANK_2};
inline std::array<int, 2> PAWN_DIRECTION_LOOKUP = {-8, 8};
inline std::array<std::array<BB, 64>, 4> PAWN_MOVES;
inline std::array<BB, 64> KNIGHT_MOVES;
inline std::array<BB, 64> KING_MOVES;

constexpr BB pawn_moves(const int pos, const BB &opposite_team, const BB &empties, const bool turn) noexcept {
    return (PAWN_MOVES[turn][pos] & empties) | (shift_up(shift_up(SQUARE_TO_BB[pos] & (PAWN_DOUBLE_RANK_LOOKUP[turn]), PAWN_DIRECTION_LOOKUP[turn]) & empties, PAWN_DIRECTION_LOOKUP[turn]) & empties) | (PAWN_MOVES[turn+2][pos] & opposite_team);
}

constexpr BB knight_moves(const int pos, const BB &same_team) noexcept {
    /*
    A>>6;B<<6;C>>10;D<<10;E>>15;F<<15;G>>17;H<<17;
    00H0F000
    0D000B00
    00010000
    0A000C00
    00E0G000
    00000000
    00000000
    00000000
    */
    return KNIGHT_MOVES[pos] & ~same_team;
}

constexpr BB king_moves(const int pos, const BB &same_team, const BB &other_team_attacks) noexcept {
    /*
    A = <<9
    B = <<8
    C = <<7
    D = <<1
    E = >>1
    F = >>7
    G = >>8
    H = >>9
    ABC
    DXE
    FGH
    */
   return KING_MOVES[pos] & ~(same_team | other_team_attacks);
};
#endif // NON_SLIDING_MOVES_HPP