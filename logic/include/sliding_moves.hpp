#ifndef SLIDING_MOVES_HPP
#define SLIDING_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"

BB blocker_mask_rook(const int &position);

BB blocker_mask_bishop(const int &position);

std::array<BB, 4096> blocker_boards(const BB &blocker_mask);

BB moveboard_bishop(const BB &blocker_board, const int &position);

BB moveboard_rook(const BB &blocker_board, const int &position);
#endif // SLIDING_MOVES_HPP