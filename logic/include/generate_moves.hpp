#ifndef GENERATE_MOVES_HPP
#define GENERATE_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "non_sliding_moves.hpp"
#include "sliding_moves.hpp"
#include "move.hpp"
#include "board.hpp"
#include "magic_values.hpp"

inline std::array<BB, 4096> in_between_lookup;
constexpr BB in_between(int i, int j) {
    return in_between_lookup[i*64 + j];
}

// class Searcher {
    // public:
        // std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
        // Searcher(std::array<magic_structure, 64> ROOK_MAGICS, std::array<magic_structure, 64> BISHOP_MAGICS);
void generate_legal_moves(Board &chess_board, std::vector<Move> &moves);
BB generate_attacks(Board &chess_board);
void set_attack_bitboard(Board &chess_board);
BB get_and_set_pins(Board &chess_board);
// bool is_legal(Board &chess_board, Move move, BB king, BB other_team_bishops_and_queens, BB other_team_rooks_and_queens, int same_team);
// bool is_legal(Board &chess_board, Move move);
// std::vector<Move> filter_illegal_moves(Board chess_board, std::vector<Move> moves);
// void set_pins(Board &chess_board);
// };
#endif // GENERATE_MOVES_HPP