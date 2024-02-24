#include "generate_moves.hpp"

std::vector<Move> generate_legal_moves(Board chess_board) {
    // pawns
    std::vector<Move> moves;
    int same_team = turn_to_index(chess_board.turn);
    int other_team = turn_to_index(!chess_board.turn);
    BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    BB piece_moves;
    int pos, new_pos, rank;
    while (same_team_pawns) {
        pos = pop_first_one(same_team_pawns);
        piece_moves = pawn_moves(1ULL << pos, chess_board.pieces[other_team], chess_board.pieces[EMPTY], chess_board.turn ? 8 : -8);
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            rank = int(new_pos / 8);
            if (pos != 0 && pos != 7) {
                moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
            }
            else { // then it has to be a promotion
                // add all possible promotions
                moves.push_back(generate_move(pos, new_pos, PROMOTION, QUEEN));
                moves.push_back(generate_move(pos, new_pos, PROMOTION, ROOK));
                moves.push_back(generate_move(pos, new_pos, PROMOTION, KNIGHT));
                moves.push_back(generate_move(pos, new_pos, PROMOTION, BISHOP));
            }
        }
        // en_pessant
        if (chess_board.enpessents.back() != -1 && abs(chess_board.enpessents.back() - pos) < 2) { // if it is one off of the en pessant pawn
            moves.push_back(generate_move(pos, chess_board.enpessents.back() - chess_board.turn ? -8 : 8, EN_PESSANT));
        }
    }
    // knights
    BB same_team_knights = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];

    while (same_team_knights) {
        pos = pop_first_one(same_team_knights);
        piece_moves = knight_moves(1ULL << pos, chess_board.pieces[same_team]);
        while (piece_moves) {
            moves.push_back(generate_move(pos, pop_first_one(piece_moves), NORMAL_MOVE));
        }
    }


    return moves;
}