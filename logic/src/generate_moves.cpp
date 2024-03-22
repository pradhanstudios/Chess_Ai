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

void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) { // should be reserved to size 218
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
        // if (pos == 35) {
        //     // print_BB(chess_board.pieces[FULL] & BISHOP_MAGICS[35].mask);
        //     print_BB(piece_moves);
        // }
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
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (BISHOP_MAGICS)[pos]);
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
        }
    }

    // king
    chess_board.next_turn(); // skip turn
    set_attack_bitboard(chess_board);
    chess_board.next_turn(); // go back
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    piece_moves = king_moves(king, chess_board.pieces[same_team], chess_board.pieces[OTHER_TEAM_ATTACKS]);
    pos = pop_first_one(king);
    while (piece_moves) {
        new_pos = pop_first_one(piece_moves);
        moves.push_back(generate_move(pos, new_pos, NORMAL_MOVE));
    }
    int check_mask = 0ULL;
    int cur_check_allowance = chess_board.castle_history.back();
    int color_to_position = chess_board.turn ? 0 : 56;
    // kingside castle
    int shift = chess_board.turn ? 0 : 2;
    check_mask = 0b1111 << color_to_position;
    if (((0b0110 << color_to_position) & ~(chess_board.pieces[FULL] | chess_board.pieces[OTHER_TEAM_ATTACKS])) && ((cur_check_allowance>>shift) & 1)) {
        moves.push_back(generate_move(3 + color_to_position, 1 + color_to_position, CASTLE, EMPTY, KINGSIDE_CASTLE));
    }

    // queenside castle
    check_mask = 0b10111000 << color_to_position;
    shift++; // because queenside castles are after in the mask
    if (((0b01110000 << color_to_position) & ~chess_board.pieces[FULL]) && ((cur_check_allowance>>shift) & 1) && ((0b001100000 << color_to_position) & ~(chess_board.pieces[OTHER_TEAM_ATTACKS]))) {
        moves.push_back(generate_move(3 + color_to_position, 4 + color_to_position, CASTLE, EMPTY, QUEENSIDE_CASTLE));
    }
    moves = filter_illegal_moves(chess_board, moves);
}

BB generate_attacks(Board chess_board) {
    BB attacks = 0ULL;
    int pos, same_team, other_team;
    same_team = turn_to_index(chess_board.turn);
    other_team = turn_to_index(!chess_board.turn);
    // sliding pieces first because they're easy
    BB same_team_bishops = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    while (same_team_bishops) {
        pos = pop_first_one(same_team_bishops); // from
        attacks |= get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (BISHOP_MAGICS)[pos]);
    }
    BB same_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[same_team];
    while (same_team_rooks) {
        pos = pop_first_one(same_team_rooks);
        attacks |= get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (ROOK_MAGICS)[pos]);
    }
    BB same_team_queens = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];
    while (same_team_queens) {
        pos = pop_first_one(same_team_queens);
        attacks |= get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos, (BISHOP_MAGICS)[pos]);
    }
    // knights
    attacks |= knight_moves(chess_board.pieces[KNIGHT] & chess_board.pieces[same_team], chess_board.pieces[same_team]); 

    // pawns
    BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    int direction = chess_board.turn ? 8 : -8;
    attacks |= (shift_back(same_team_pawns & ~(((direction+1) == -7) ? ~(A_FILE) : ~(H_FILE)), direction+1) | shift_back(same_team_pawns & ~(((direction-1) == -9) ? ~(H_FILE) : ~(A_FILE)), direction-1));

    // king
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    attacks |= 
    (((king & ~(RANK_8 | A_FILE)) << 9) |
    ((king & ~(RANK_8)) << 8) |
    ((king & ~(RANK_8 | H_FILE)) << 7) |
    ((king & ~(A_FILE)) << 1) |
    ((king & ~(H_FILE)) >> 1) |
    ((king & ~(RANK_1 | A_FILE)) >> 7) |
    ((king & ~(RANK_1)) >> 8) |
    ((king & ~(RANK_1 | H_FILE)) >> 9));

    return attacks;
}

void set_attack_bitboard(Board &chess_board) {
    chess_board.pieces[OTHER_TEAM_ATTACKS] = generate_attacks(chess_board);
}

bool is_legal(Board chess_board, Move move, BB king, BB other_team_bishops_and_queens, BB other_team_rooks_and_queens, int same_team, int other_team) {
    int from, to, type, same_team, other_team;
    BB cur_bishop_moves, cur_rook_moves, cur_attacks, cur_king;
    from = move & FIRST_SIX;
    to = (move >> 6) & FIRST_SIX;
    type = (move >> 12) & FIRST_FOUR;
    if (type == NORMAL_MOVE) {
        // generate queen moves for position
        cur_bishop_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], from, (BISHOP_MAGICS)[from]);
        cur_rook_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], from, (ROOK_MAGICS)[from]);
        if ((cur_bishop_moves & other_team_bishops_and_queens) || (cur_rook_moves & other_team_rooks_and_queens)) { // the idea is that if we are being attacked by sliding pieces we might be pinned
            chess_board.play_move(move);
            cur_attacks = generate_attacks(chess_board);
            cur_king = chess_board.pieces[same_team] & chess_board.pieces[KING];
            // I dont have to undo move because I did not reference it, so it doesn't matter
            if (cur_king & cur_attacks) { // king is in check
                return false;
            }
        }
    }
    else if (type == EN_PESSANT) { // it is too complicated to calculate this, so just brute force it
        chess_board.play_move(move);
        cur_attacks = generate_attacks(chess_board);
        if (king & cur_attacks) {
            return false;
        }
    }
    else if (type == PROMOTION) {
        cur_bishop_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], from, (BISHOP_MAGICS)[from]);
        cur_rook_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], from, (ROOK_MAGICS)[from]);
        if ((cur_bishop_moves & other_team_bishops_and_queens) || (cur_rook_moves & other_team_rooks_and_queens)) {
            chess_board.play_move(move);
            cur_attacks = generate_attacks(chess_board);
            // I dont have to undo move because I did not reference it, so it doesn't matter
            if (king & cur_attacks) { // king is in check
                return false;
            }
        }
    // for castles, in move generation we already make sure that we do not run into check
    }
    return true;
}

bool is_legal(Board chess_board, Move move) {
    int same_team, other_team;
    same_team = turn_to_index(chess_board.turn);
    other_team = turn_to_index(!chess_board.turn);
    BB other_team_queens = chess_board.pieces[other_team] & chess_board.pieces[QUEEN];
    return is_legal(chess_board, move, chess_board.pieces[same_team] & chess_board.pieces[KING], (chess_board.pieces[BISHOP] | other_team_queens) & chess_board.pieces[other_team], (chess_board.pieces[ROOK] | other_team_queens) & chess_board.pieces[other_team], same_team, other_team);
}

std::vector<Move> filter_illegal_moves(Board chess_board, std::vector<Move> moves) {
    int from, to, type, same_team, other_team;
    BB cur_moves, cur_attacks;
    std::vector<Move> legal;
    same_team = turn_to_index(chess_board.turn);
    other_team = turn_to_index(!chess_board.turn);
    BB same_team_king = chess_board.pieces[same_team] & chess_board.pieces[KING];
    BB other_team_bishops_and_queens = (chess_board.pieces[BISHOP] | chess_board.pieces[QUEEN]) & chess_board.pieces[other_team];
    BB other_team_rooks_and_queens = (chess_board.pieces[ROOK] | chess_board.pieces[QUEEN]) & chess_board.pieces[other_team];
    std::vector<Move> filtered_moves;
    filtered_moves.reserve(MAX_LEGAL_MOVES);
    for (Move move : moves) {
        if (!is_legal(chess_board, move, same_team_king, other_team_bishops_and_queens, other_team_rooks_and_queens, same_team, other_team)) {
            continue;
        }
        filtered_moves.push_back(move);
    }
    return filtered_moves;
}
// void set_pins(Board &chess_board) {
//     int pos, same_team, other_team, direction, rank, file, difference, king_pos_rank, king_pos_file, king_pos, cur_attacks;
//     same_team = turn_to_index(chess_board.turn);
//     other_team = turn_to_index(!chess_board.turn);
//     BB same_team_king = chess_board.pieces[KING] & chess_board.pieces[same_team];
//     king_pos = zeroes_start(same_team_king);
//     king_pos_rank = king_pos & 7;
//     king_pos_file = int(king_pos / 8);
//     BB no_same_team_pieces = ~(chess_board.pieces[same_team] ^ (same_team_king)) & chess_board.pieces[FULL];
//     BB cur_attacks, pin_ray;
//     BB other_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[other_team];
//     BB cur_line;
//     while (other_team_rooks) {
//         pos = pop_first_one(other_team_rooks);
//         cur_attacks = get_sliding_moves(no_same_team_pieces, chess_board.pieces[other_team], pos, (ROOK_MAGICS)[pos]);
//         if (cur_attacks & same_team_king) {
//             rank = pos & 7;
//             file = int(pos / 8);
//             if (rank == king_pos_rank) {
//                 cur_attacks &= (RANK_1 << (8 * rank)) & (((1 << (abs(file - king_pos_file) + 1)) - 1) << (file < king_pos_file ? file : king_pos_file) /*min*/);
//                 cur_attacks ^= same_team_king;
//                 cur_attacks &= chess_board.pieces[same_team];
//                 if (real_count(cur_attacks) == 1) { // only one piece is being pinned
//                     chess_board.pins[chess_board.turn][zeroes_start(cur_attacks)] = cur_attacks; 
//                 }
//             }
//         }
//     } 
// }