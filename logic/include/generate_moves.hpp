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
constexpr BB in_between(const int i, const int j) noexcept {
    return in_between_lookup[i*64 + j];
}

void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) noexcept;
void generate_captures(Board &chess_board, std::vector<Move> &moves) noexcept;
BB generate_attacks(Board &chess_board) noexcept;
void set_attack_bitboard(Board &chess_board) noexcept;
BB get_and_set_pins(Board &chess_board) noexcept;
#endif // GENERATE_MOVES_HPP