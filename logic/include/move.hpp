#pragma once

#include "constants.hpp"

// Move Structure 16-bit
// upto move_type(3-bit) + piece_type(3-bit) | to(6-bit) | from(6-bit)
// for castle and en_pessant, typing is implied so it becomes just the move type

Move generate_move(int from, int to, int type, int promotion = 0, int castle_side = 0);