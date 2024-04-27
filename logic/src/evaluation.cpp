#include "evaluation.hpp"

int evaluate(Board &chess_board) noexcept { // neutral evaluation of position
    int eval = 0;

    BB pieces = chess_board.pieces[FULL] & chess_board.pieces[WHITE];
    int pos, piece;
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        eval += (PIECE_TABLES[((no_color(piece)-1)<<5) + orient(piece, pos)]), (chess_board.piece_data[pos]);
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]), (chess_board.piece_data[pos])) << std::endl;
    }

    pieces = chess_board.pieces[FULL] & chess_board.pieces[BLACK];
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        eval -= (PIECE_TABLES[((no_color(piece)-1)<<5) + orient(piece, pos)]), (chess_board.piece_data[pos]);
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]), (chess_board.piece_data[pos])) << std::endl;
    }

    return eval;
}