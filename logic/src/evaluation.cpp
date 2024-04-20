#include "evaluation.hpp"

int simple_eval(const Board &chess_board) noexcept {
    int eval = 0;
    eval += real_count(chess_board.pieces[PAWN] & chess_board.pieces[WHITE]) * piece_values[PAWN];
    eval += real_count(chess_board.pieces[KNIGHT] & chess_board.pieces[WHITE]) * piece_values[KNIGHT];
    eval += real_count(chess_board.pieces[BISHOP] & chess_board.pieces[WHITE]) * piece_values[BISHOP];
    eval += real_count(chess_board.pieces[ROOK] & chess_board.pieces[WHITE]) * piece_values[ROOK];
    eval += real_count(chess_board.pieces[QUEEN] & chess_board.pieces[WHITE]) * piece_values[QUEEN];
    eval -= real_count(chess_board.pieces[PAWN] & chess_board.pieces[BLACK]) * piece_values[PAWN];
    eval -= real_count(chess_board.pieces[KNIGHT] & chess_board.pieces[BLACK]) * piece_values[KNIGHT];
    eval -= real_count(chess_board.pieces[BISHOP] & chess_board.pieces[BLACK]) * piece_values[BISHOP];
    eval -= real_count(chess_board.pieces[ROOK] & chess_board.pieces[BLACK]) * piece_values[ROOK];
    eval -= real_count(chess_board.pieces[QUEEN] & chess_board.pieces[BLACK]) * piece_values[QUEEN];

    return eval;
}
