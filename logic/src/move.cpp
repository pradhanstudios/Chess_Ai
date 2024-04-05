
#include "move.hpp"

// Move Structure 16-bit

Move::Move(const unsigned int from, const unsigned int to, const unsigned int type, const int promotion, const int castle_side) {
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

void print_move_fancy(const Move m) {
    std::cout << "Move(from=" << m.from << ", to=" << m.to << ")\n";
}