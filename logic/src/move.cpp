
#include "move.hpp"

// Move Structure 16-bit

Move::Move(const unsigned int &from, const unsigned int &to, const unsigned int &type, const int &promotion, const int &castle_side) :
    from(from),
    to(to),
    type(type | (promotion << 1) | (castle_side << 3)) {}

void print_move_fancy(const Move &m) {
    std::cout << "Move(from=" << m.from << ", to=" << m.to << ")\n";
}