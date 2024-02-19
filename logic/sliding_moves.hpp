#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"

BB blocker_mask_rook(int position);

BB blocker_mask_bishop(int position);

std::array<BB, 4096> blocker_boards(BB blocker_mask);

BB moveboard_bishop(BB blocker_board, int position);

BB moveboard_rook(BB blocker_board, int position);