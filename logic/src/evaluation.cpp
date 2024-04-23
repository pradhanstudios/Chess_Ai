#include "evaluation.hpp"

int evaluate(Board &chess_board) noexcept { // neutral evaluation of position
    int eval = 0;

    BB pieces = chess_board.pieces[FULL];
    int pos, piece;
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        eval += multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]), (chess_board.piece_data[pos]));
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]), (chess_board.piece_data[pos])) << std::endl;
    }

    return eval;
}