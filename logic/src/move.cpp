
#include "move.hpp"

// Move Structure 16-bit

Move::Move(unsigned int from, unsigned int to, unsigned int type, int promotion, int castle_side) {
    this->from = from;
    this->to = to;
    unsigned int cur_type = type;
    if (type == PROMOTION) {
        cur_type |= promotion << 1;
    }
    else if (type == CASTLE) {
        cur_type |= castle_side << 3;
    }
    this->type = cur_type;
}

void print_move_fancy(Move m) {
    std::cout << "Move(from=" << m.from << ", to=" << m.to << ")\n";
}