#include "generate_moves.hpp"

// Searcher::Searcher(std::array<magic_structure, 64> ROOK_MAGICS, std::array<magic_structure, 64> BISHOP_MAGICS) {
//     this->ROOK_MAGICS = ROOK_MAGICS;
//     this->BISHOP_MAGICS = BISHOP_MAGICS;
// }

// Searcher::Searcher(std::array<magic_structure, 64> ROOK_MAGICS, std::array<magic_structure, 64> BISHOP_MAGICS) {\

// }

// void generate_pawn_moves(Board chess_board, std::vector<Move> &moves) {
//     int same_team = turn_to_index(chess_board.turn);
//     int other_team = turn_to_index(!chess_board.turn);
//     BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
//     BB piece_moves;
//     int pos, new_pos, rank;
//     while (same_team_pawns) {
//         pos = pop_first_one(same_team_pawns);
//         // std::cout << pos << " is pos" << std::endl;
//         // std::cout << pos << " ";
//         piece_moves = pawn_moves(1ULL << pos, chess_board.pieces[other_team], chess_board.pieces[EMPTY], chess_board.turn ? 8 : -8);
//         // print_BB(chess_board.pieces[other_team]);
//         // std::cout << piece_moves << std::endl;
//         while (piece_moves) {
//             new_pos = pop_first_one(piece_moves);
//             // print_BB(1 << (63-new_pos));
//             rank = int(new_pos / 8);
//             if (pos != 0 && pos != 7) {
//                 moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
//             }
//             else { // then it has to be a promotion
//                 // add all possible promotions
//                 moves.push_back(generate_move(pos, new_pos, PROMOTION, QUEEN));
//                 moves.push_back(generate_move(pos, new_pos, PROMOTION, ROOK));
//                 moves.push_back(generate_move(pos, new_pos, PROMOTION, KNIGHT));
//                 moves.push_back(generate_move(pos, new_pos, PROMOTION, BISHOP));
//             }
//         }
//         // en_pessant
//         if (chess_board.enpessent_history.back() != -1 && abs(chess_board.enpessent_history.back() - pos) < 2 && pos != chess_board.enpessent_history.back() && (chess_board.piece_data[pos] >> 3) == chess_board.turn /*so if it is white ot os 0 or black is 1*/) { // if it is one off of the en pessant pawn
//             moves.push_back(generate_move(pos, chess_board.enpessent_history.back() - chess_board.turn ? -8 : 8, EN_PESSANT));
//         }
//     }
// }

// void generate_knight_moves(Board chess_board, std::vector<Move> &moves) {
//     int same_team = turn_to_index(chess_board.turn);
//     BB same_team_knights = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
//     BB piece_moves;
//     int pos, new_pos, rank;
//     // print_BB(same_team_knights);

//     while (same_team_knights) {
//         pos = pop_first_one(same_team_knights); // from
//         piece_moves = knight_moves(1ULL << pos, chess_board.pieces[same_team]);

//         while (piece_moves) {
//             new_pos = pop_first_one(piece_moves);
//             moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
//         }
//     }
// }

// void get_moves_from_BB(BB bitboard, int start, std::vector<Move> &moves) {
//     int pos;
//     while (bitboard) {
//         pos = pop_first_one(bitboard);
//         moves.push_back(generate_move(start, pos, NORMAL_MOVE));
//     }
// }

void generate_legal_moves(Board chess_board, std::vector<Move> &moves) { // should be reserved to size 218
    // fprintf(stderr, "here\n");
    // fflush(stderr);
    // printf("oh yea\n");
    // pawns
    // print_BB(chess_board.pieces[WHITE]);
    int same_team = turn_to_index(chess_board.turn);
    int other_team = turn_to_index(!chess_board.turn);
    BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    BB piece_moves;
    int pos, new_pos, rank;
    while (same_team_pawns) {
        pos = pop_first_one(same_team_pawns);
        // std::cout << pos << " is pos" << std::endl;
        // std::cout << pos << " ";
        piece_moves = pawn_moves(1ULL << pos, chess_board.pieces[other_team], chess_board.pieces[EMPTY], chess_board.turn ? 8 : -8);
        // print_BB(chess_board.pieces[other_team]);
        // std::cout << piece_moves << std::endl;
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            // print_BB(1 << (63-new_pos));
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
        if (chess_board.enpessent_history.back() != -1 && abs(chess_board.enpessent_history.back() - pos) < 2 && pos != chess_board.enpessent_history.back() && ((chess_board.piece_data[pos] & 0b111) >> 3) == chess_board.turn /*so if it is white ot os 0 or black is 1*/) { // if it is one off of the en pessant pawn
            moves.push_back(generate_move(pos, chess_board.enpessent_history.back() - chess_board.turn ? -8 : 8, EN_PESSANT));
        }
    }
    // knights
    BB same_team_knights = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    // print_BB(same_team_knights);

    while (same_team_knights) {
        pos = pop_first_one(same_team_knights); // from
        piece_moves = knight_moves(1ULL << pos, chess_board.pieces[same_team]);

        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
        }
    }

    // bishops
    // std::cout << "got here" << std::endl;
    BB same_team_bishops = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    // print_BB(same_team_bishops);

    while (same_team_bishops) {
        pos = pop_first_one(same_team_bishops); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (BISHOP_MAGICS)[pos]);
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
        }
    }
    BB same_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[same_team];

    while (same_team_rooks) {
        pos = pop_first_one(same_team_rooks); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (ROOK_MAGICS)[pos]);
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
        }
    }

    BB same_team_queens = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];

    while (same_team_queens) {
        pos = pop_first_one(same_team_queens); // from
        // moves for rooks and bishops combined
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (ROOK_MAGICS)[pos]) ^ get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (BISHOP_MAGICS)[pos]);
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
        }
    }
}