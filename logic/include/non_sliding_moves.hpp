#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"

// direction should be -8 (white), or 8 (black) for
BB pawn_moves(BB pawns_to_move, BB opposite_team, BB empties, int direction);

BB knight_moves(BB knights_to_move, BB same_team);