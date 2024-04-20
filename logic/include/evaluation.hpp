#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "board.hpp"

inline std::array<int, 7> piece_values = {0, 10, 33, 35, 50, 90, 0};
int simple_eval(const Board &chess_board) noexcept;

#endif // EVALUATION_HPP