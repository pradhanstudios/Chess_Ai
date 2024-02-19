#pragma once

#include "constants.hpp"

const std::array<BB, 64> ROOK_MAGICS;
const std::array<BB, 64> ROOK_SHIFTS;
const std::array<BB, 64> BISHOP_MAGICS;
const std::array<BB, 64> BISHOP_SHIFTS;

int calculate_index(BB input_bitboard, BB magic, int shift);