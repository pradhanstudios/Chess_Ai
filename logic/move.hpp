#pragma once

#include "constants.hpp"

// Move Structure 20-bit
// move_type(2-bit)| capture_type(3-bit) | piece_type(3-bit) | to(6-bit) | from(6-bit)

Move generate_move(int from, int to, int type, int piece = 0, int capture = 0, int castle_side = 0) { // you dont have to specify piece if you are doing an en pessant
    Move out;
    out.data = from;
    out.data |= (to << 6);
    out.data |= (piece << 12);
    out.data |= (piece << 15);
    out.data |= (type << 18);
    if (type == EN_PESSANT) { // then we have to specify which side
        out.data |= (castle_side << 14);
    }
    return out;
}
