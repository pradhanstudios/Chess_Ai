#ifndef UCI_HPP
#define UCI_HPP

#include "move.hpp"
#include "board.hpp"

const extern std::array<char, 8> num_to_col;
const extern std::array<char, 6> num_to_piece;

std::string move_to_uci(Move move);
Move uci_to_move(std::string uci, Board chess_board);
#endif // UCI_HPP