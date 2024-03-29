// still a work in progress
#include "move.hpp"

// Move Structure 16-bit
// upto move_type(3-bit) + piece_type(3-bit) | to(6-bit) | from(6-bit)
// for castle, en_pessant and promotion, typing is implied so it becomes just the move type

Move generate_move(int from, int to, int type, int promotion, int castle_side) {
    Move out;
    out = from;
    out |= to << 6;
    out |= type << 12;
    if (type == PROMOTION) {
        out |= promotion << 13;
    }
    else if (type == CASTLE) {
        out |= castle_side << 15;
    }
    
    return out;
}

void print_move_fancy(Move m) {
    std::cout << "Move(from=" << (m & FIRST_SIX) << ", to=" << ((m >> 6) & FIRST_SIX) << ")\n";
}