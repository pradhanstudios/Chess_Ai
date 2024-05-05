#include "evaluation.hpp"

int evaluate(Board &chess_board) noexcept { // neutral evaluation of position
    int mg_eval = 0, eg_eval = 0;

    float phase = std::min(
        24, 
        real_count(chess_board.pieces[BISHOP]) + real_count(chess_board.pieces[KNIGHT]) + 2*real_count(chess_board.pieces[ROOK]) + 4*real_count(chess_board.pieces[QUEEN])
        ) / 24;
    // std::cout << int(phase*24) << std::endl;
    BB pieces = chess_board.pieces[FULL] & chess_board.pieces[WHITE];
    int pos, piece, no_color_piece;
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        no_color_piece = no_color(piece);
        mg_eval += PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)] + piece_values_mg[no_color_piece];
        eg_eval += PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)] + piece_values_eg[no_color_piece];
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]) << std::endl;
    }

    pieces = chess_board.pieces[FULL] & chess_board.pieces[BLACK];
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        no_color_piece = no_color(piece);
        mg_eval -= PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)] + piece_values_mg[no_color_piece];
        eg_eval -= PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)] + piece_values_eg[no_color_piece];
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]) << std::endl;
    }
    // std::cout << static_cast<int>(static_cast<float>(mg_eval) * phase + (1.0f - phase) * static_cast<float>(eg_eval)) << std::endl;
    return static_cast<int>(static_cast<float>(mg_eval) * phase + (1.0f - phase) * static_cast<float>(eg_eval));
}