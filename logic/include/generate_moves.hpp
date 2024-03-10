#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"
#include "non_sliding_moves.hpp"
#include "sliding_moves.hpp"
#include "move.hpp"
#include "board.hpp"
#include "magic_values.hpp"

// class Searcher {
    // public:
        // std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
        // Searcher(std::array<magic_structure, 64> ROOK_MAGICS, std::array<magic_structure, 64> BISHOP_MAGICS);
void generate_legal_moves(Board chess_board, std::vector<Move> &moves);
// };