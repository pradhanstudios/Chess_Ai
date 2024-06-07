#ifndef MOVE_HPP
#define MOVE_HPP

#include "constants.hpp"


struct Move {
    unsigned int from : 6;
    unsigned int to : 6;
    unsigned int type : 4; 
    int16_t score;
    Move(const unsigned int from = 0, const unsigned int to = 0, const unsigned int type = 0b0000, const int promotion = 0, const int castle_side = 0);
    inline bool operator==(const Move &other_move) {
        return (other_move.from == this->from) && (other_move.to == this->to) && (other_move.type == this->type);
    }
    inline bool operator!=(const Move &other_move) {
        return !this->operator==(other_move);
    }
};


const inline Move NULL_MOVE = Move();
void print_move_fancy(const Move &m);

#endif // MOVE_HPP