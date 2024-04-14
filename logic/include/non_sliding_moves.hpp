#ifndef NON_SLIDING_MOVES_HPP
#define NON_SLIDING_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"

inline std::array<BB, 2> PAWN_DOUBLE_RANK_LOOKUP = {RANK_7, RANK_2};
inline std::array<int, 2> PAWN_DIRECTION_LOOKUP = {-8, 8};
inline std::array<std::array<BB, 64>, 4> PAWN_MOVES;
inline std::array<BB, 64> KNIGHT_MOVES;
inline std::array<BB, 64> KING_MOVES;

constexpr BB pawn_moves(int pos, BB opposite_team, BB empties, bool turn) {
    return (PAWN_MOVES[turn][pos] & empties) | (shift_up(shift_up(SQUARE_TO_BB[pos] & (PAWN_DOUBLE_RANK_LOOKUP[turn]), PAWN_DIRECTION_LOOKUP[turn]) & empties, PAWN_DIRECTION_LOOKUP[turn]) & empties) | (PAWN_MOVES[turn+2][pos] & opposite_team);
    // return (shift_back(pawns_to_move, direction) & empties) | (shift_back(shift_back(pawns_to_move & (direction < 0 ? RANK_7 : RANK_2), direction) & empties, direction) & empties) | ((shift_back(pawns_to_move & ~A_FILE, direction+1) | shift_back(pawns_to_move & ~H_FILE, direction-1)) & opposite_team);
}

constexpr BB knight_moves(const int &pos, const BB &same_team) {
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
    // BB attacks = 0ULL;
    // int pos = 0;
    // while (knights_to_move) {
    //     pos = pop_first_one(knights_to_move);
    //     attacks |= ;
    // }
    return KNIGHT_MOVES[pos] & ~same_team;
    // return
    //         (((knights_to_move & ~(RANK_1 | A_FILE | B_FILE)) >> 6) |
    //         ((knights_to_move & ~(RANK_1 | G_FILE | H_FILE)) >> 10) |
    //         ((knights_to_move & ~(RANK_8 | G_FILE | H_FILE)) << 6) |
    //         ((knights_to_move & ~(RANK_8 | A_FILE | B_FILE)) << 10) | 
    //         ((knights_to_move & ~(RANK_1 | RANK_2 | A_FILE)) >> 15) | 
    //         ((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) >> 17) |
    //         ((knights_to_move & ~(RANK_8 | RANK_7 | H_FILE)) << 15) |
    //         ((knights_to_move & ~(RANK_8 | RANK_7 | A_FILE)) << 17)) & ~same_team;
}

constexpr BB king_moves(const int &pos, const BB &same_team, const BB &other_team_attacks) {
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
    //    return (
    //     ((king & ~(RANK_8 | A_FILE)) << 9) |
    //     ((king & ~(RANK_8)) << 8) |
    //     ((king & ~(RANK_8 | H_FILE)) << 7) |
    //     ((king & ~(A_FILE)) << 1) |
    //     ((king & ~(H_FILE)) >> 1) |
    //     ((king & ~(RANK_1 | A_FILE)) >> 7) |
    //     ((king & ~(RANK_1)) >> 8) |
    //     ((king & ~(RANK_1 | H_FILE)) >> 9)
    //    ) & ~(same_team | other_team_attacks);
};
#endif // NON_SLIDING_MOVES_HPP