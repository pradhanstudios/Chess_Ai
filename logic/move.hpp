#pragma once

#include "constants.hpp"

enum MOVE_TYPES {
    NORMAL_MOVE = 0b0001,
    PROMOTION = 0b0010,
    CASTLE = 0b0011,
    EN_PESSANT = 0b0100,
};

Move generate_move(int from, int to, int type, int piece = 0, int castle_side = 0) { // you dont have to specify piece if you are doing an en pessant
    Move out = from;
    out |= (to << 6);
    out |= (type << 12);
    if (type == PROMOTION || type == NORMAL_MOVE) { // then we have to specify the piece
        out |= (piece << 13);
    }
    else if (type == EN_PESSANT) { // then we have to specify which side
        out |= (castle_side << 14);
    }
    return out;
}

