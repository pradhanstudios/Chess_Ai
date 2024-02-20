#pragma once

#include "constants.hpp"

extern const std::array<BB, 64> ROOK_MAGICS;
extern const std::array<BB, 64> ROOK_SHIFTS;
extern const std::array<BB, 64> BISHOP_MAGICS;
extern const std::array<BB, 64> BISHOP_SHIFTS;

int calculate_index(BB input_bitboard, BB magic, int shift);