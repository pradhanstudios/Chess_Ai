#include "evaluation.hpp"

int evaluate(Board &chess_board) noexcept { // neutral evaluation of position
    int eval = 0;

    float phase = std::max(
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
        if (no_color_piece != PAWN && no_color_piece != KING) {
            eval += PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)];
        }
        else {
            // tapered eval
            if (no_color_piece == PAWN) {
                eval += (PIECE_TABLES_MG[orient(piece, pos)] * phase + (1 - phase) * PIECE_TABLES_EG[orient(piece, pos)]);
            }
            else { // king
                eval += (PIECE_TABLES_MG[320 + orient(piece, pos)] * phase + (1 - phase) * PIECE_TABLES_EG[64 + orient(piece, pos)]);
            }
        }
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]) << std::endl;
    }

    pieces = chess_board.pieces[FULL] & chess_board.pieces[BLACK];
    while (pieces) {
        pos = pop_first_one(pieces);
        piece = chess_board.piece_data[pos];
        no_color_piece = no_color(piece);
        if (no_color_piece != PAWN && no_color_piece != KING) {
            eval -= PIECE_TABLES_MG[((no_color_piece-1)<<5) + orient(piece, pos)];
        }
        else {
            // tapered eval
            if (no_color_piece == PAWN) {
                eval -= (PIECE_TABLES_MG[orient(piece, pos)] * phase + (1 - phase) * PIECE_TABLES_EG[orient(piece, pos)]);
            }
            else { // king
                eval -= (PIECE_TABLES_MG[320 + orient(piece, pos)] * phase + (1 - phase) * PIECE_TABLES_EG[64 + orient(piece, pos)]);
            }
        }
        // std::cout << multiply_based_on_color((PIECE_TABLES[no_color(piece)-1][orient(piece, pos)]) << std::endl;
    }

    return eval;
}