#pragma once
#include "constants.hpp"
#include "sliding_moves.hpp"

// struct _Moves {
//     std::array<BB, 64> ROOK_MASKS;
//     std::array<BB, 64> BISHOP_MASKS;
//     std::array<std::array<BB, 4096>, 64> ROOK_MOVES;
//     std::array<std::array<BB, 4096>, 64> BISHOP_MOVES;
// };
// std::array<BB, 64> rook_masks, bishop_masks;
// std::array<std::array<BB, 4096>, 64> rook_moves, bishop_moves;
// std::array<BB, 64> &rook_masks, std::array<BB, 64> &bishop_masks, std::array<std::array<BB, 4096>, 64> &rook_moves, std::array<std::array<BB, 4096>, 64> &bishop_moves

void init_moveboards(std::array<BB, 64> &rook_masks, std::array<BB, 64> &bishop_masks, std::array<std::array<BB, 4096>, 64> &rook_moves, std::array<std::array<BB, 4096>, 64> &bishop_moves);

// std::array<BB, 64> ROOK_MASKS;
// std::array<BB, 64> BISHOP_MASKS;
// std::array<std::array<BB, 4096>, 64> ROOK_MOVES;
// std::array<std::array<BB, 4096>, 64> BISHOP_MOVES;

// void init_moveboards() {
//     _init_moveboards_helper()
// }