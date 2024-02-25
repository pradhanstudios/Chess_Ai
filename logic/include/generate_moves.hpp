#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"
#include "non_sliding_moves.hpp"
#include "sliding_moves.hpp"
#include "move.hpp"
#include "board.hpp"

void generate_legal_moves(Board chess_board, std::vector<Move> &moves);