#include <iostream>

#include "constants.hpp"
#include "bit_operations.hpp"
#include "sliding_moves.hpp"
#include "non_sliding_moves.hpp"


// const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"; // starting fen
const std::string starting_fen = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";

// bitboards
BB empties;
BB white;
BB black;
BB kings;
BB queens;
BB rooks;
BB bishops;
BB knights;
BB pawns;

void open_fen(std::string fen) {
    char cur;
    int board_ptr = 0;
    for (int i = fen.length(); i > 0; i--)
    {
        cur = fen[i-1];

        if (isdigit(cur))
        {
            // skip squares
            board_ptr += std::stoi(&cur);
        }
        else if (cur == '/')
        {
            // next row
            continue;
        }
        else
        {
            // // place piece
            // board[board_ptr] = (islower(cur) ? Black : White) | (fen_to_piece[std::tolower(cur)]);
            // board_ptr++;
            isupper(cur) ? set_bit_on(white, board_ptr) : set_bit_on(black, board_ptr);
            cur = char(std::tolower(cur));
            if (cur == 'k') {
                set_bit_on(kings, board_ptr);
            }
            else if (cur == 'q') {
                set_bit_on(queens, board_ptr);
            }
            else if (cur == 'r') {
                set_bit_on(rooks, board_ptr);
            }
            else if (cur == 'b') {
                set_bit_on(bishops, board_ptr);
            }
            else if (cur == 'n') {
                set_bit_on(knights, board_ptr);
            }
            else if (cur == 'p') {
                set_bit_on(pawns, board_ptr);
            }

            board_ptr++;
        }
    }
    empties = ~(white | black);
}


int main() {
    // open_fen(starting_fen);
    // std::vector<BB> blocker_board = blocker_boards(blocker_mask_rook(23));
    // for (int i = 0; i < blocker_board.size(); i++) {
    //     print_BB(blocker_board[i]);
    // }
    // BB blockermask = blocker_mask_rook(23);
    // std::cout << "example blockermask" << '\n';
    // print_BB(blockermask);
    // std::cout << "example blockerboard" << '\n';
    // print_BB(blockerboards[45]);
    // std::cout << blockerboards.size() << '\n';
    // print_BB(moveboard_rook(blockerboards[45], 23));
    return 0;
}