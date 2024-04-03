#include "generate_moves.hpp"

void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) { // should be reserved to size 218
    if (chess_board.state != RUNNING) {
        return;
    }
    // fprintf(stderr, "here\n");
    // fflush(stderr);
    // printf("oh yea\n");
    // pawns
    // print_BB(chess_board.pieces[WHITE]);
    History cur_history = chess_board.history.back();
    int same_team = turn_to_index(chess_board.turn);
    int other_team = turn_to_index(!chess_board.turn);
    BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    BB piece_moves;
    int pos, new_pos, rank_;
    int same_team_pins_idx = chess_board.turn;
    BB cur_attacks;
    Move en_pessant_move = NULL_MOVE;

    // BB pins = 0ULL;
    // king
    chess_board.next_turn(); // skip turn
    set_attack_bitboard(chess_board);
    BB pins = get_and_set_pins(chess_board);
    chess_board.next_turn(); // go back
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    int king_pos = zeroes_start(king);
    piece_moves = king_moves(king_pos, chess_board.pieces[same_team], chess_board.pieces[OTHER_TEAM_ATTACKS]);
    while (piece_moves) {
        new_pos = pop_first_one(piece_moves);
        moves.push_back((Move(king_pos, new_pos, NORMAL_MOVE)));
    }

    if (chess_board.is_double_check) {
        // std::cout << "here" << std::endl;
        if (moves.size() == 0) {
            if (king & chess_board.pieces[OTHER_TEAM_ATTACKS]) { // checkmate
                chess_board.state = CHECKMATE;
            }
            else { // stalemate
                chess_board.state = DRAW;
            }
        }
        return; // if it is double check there is no point in checking other moves
    }

    BB not_king = (chess_board.pieces[FULL] ^ (chess_board.pieces[same_team] & chess_board.pieces[KING]));
    int cur_check_allowance = cur_history.castle;
    int color_to_position = !chess_board.turn * 56;
    // kingside castle
    int shift = !chess_board.turn << 1;
    if (!((0b0110ULL << color_to_position) & (not_king | chess_board.pieces[OTHER_TEAM_ATTACKS])) && (cur_check_allowance & SQUARE_TO_BB[shift])) {
        // std::cout << "got here" << std::endl;
        // print_BB((0b0110ULL << color_to_position));
        moves.push_back((Move(3 + color_to_position, 1 + color_to_position, CASTLE, EMPTY, KINGSIDE_CASTLE)));
    }

    // queenside castle
    shift++; // because queenside castles are after in the mask
    // print_BB(0b00111000 << color_to_position);
    // std::cout << !((0b00111000ULL << color_to_position) & chess_board.pieces[OTHER_TEAM_ATTACKS]) << std::endl;
    if (!((0b01110000ULL << color_to_position) & not_king) && (cur_check_allowance & SQUARE_TO_BB[shift]) && !((0b00111000ULL << color_to_position) & chess_board.pieces[OTHER_TEAM_ATTACKS])) {
        // print_move_fancy((Move(3 + color_to_position, 5 + color_to_position, CASTLE, EMPTY, QUEENSIDE_CASTLE)));
        moves.push_back((Move(3 + color_to_position, 5 + color_to_position, CASTLE, EMPTY, QUEENSIDE_CASTLE)));
    }
    // print_BB(chess_board.check_ray);

    while (same_team_pawns) {
        pos = pop_first_one(same_team_pawns);
        // std::cout << pos << " is pos" << std::endl;
        // std::cout << pos << " ";
        piece_moves = pawn_moves(pos, chess_board.pieces[other_team], chess_board.pieces[EMPTY], chess_board.turn) & chess_board.check_ray;
        // print_BB(chess_board.pieces[other_team]);
        // std::cout << piece_moves << std::endl;
        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[same_team_pins_idx][pos]) | SQUARE_TO_BB[chess_board.pins[same_team_pins_idx][pos]];
        }
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            // print_BB(1 << (63-new_pos));
            rank_ = rank(new_pos);
            if (rank_ != 0 && rank_ != 7) {
                moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
            }
            else { // then it has to be a promotion
                // add all possible promotions
                moves.push_back((Move(pos, new_pos, PROMOTION, QUEEN)));
                moves.push_back((Move(pos, new_pos, PROMOTION, ROOK)));
                moves.push_back((Move(pos, new_pos, PROMOTION, KNIGHT)));
                moves.push_back((Move(pos, new_pos, PROMOTION, BISHOP)));
            }
        }
        // en_pessant
        // if (pos == 32) {
        //     std::cout << abs(cur_history.en_pessant - pos) << std::endl;
        // }
        if ((cur_history.en_pessant != 0) && (abs(cur_history.en_pessant - pos) < 2) && (real_count((SQUARE_TO_BB[cur_history.en_pessant] | SQUARE_TO_BB[pos]) & EDGES) != 2)) { // if it is one off of the en pessant pawn
            // std::cout << () << std::endl;
            en_pessant_move = Move(pos, cur_history.en_pessant + PAWN_DIRECTION_LOOKUP[chess_board.turn], EN_PESSANT);
            // std::cout << "got here" << std::endl;
            // check if it results in a check
            chess_board.play_move(en_pessant_move);
            cur_attacks = generate_attacks(chess_board);
            if (!(king & cur_attacks)) {
                moves.push_back(en_pessant_move);
            }
            chess_board.undo_move(en_pessant_move);
        }
    }
    // knights
    BB same_team_knights = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    int pins_idx;
    // print_BB(same_team_knights);

    while (same_team_knights) {
        pos = pop_first_one(same_team_knights); // from
        piece_moves = knight_moves(pos, chess_board.pieces[same_team]) & chess_board.check_ray;

        if (SQUARE_TO_BB[pos] & pins) {
            pins_idx = chess_board.pins[same_team_pins_idx][pos];
            piece_moves &= in_between(king_pos, pins_idx) | SQUARE_TO_BB[pins_idx];
        }

        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    // bishops
    // std::cout << "got here" << std::endl;
    BB same_team_bishops = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    // print_BB(same_team_bishops);

    while (same_team_bishops) {
        pos = pop_first_one(same_team_bishops); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]) & chess_board.check_ray;
        // print_BB(piece_moves);
        // if (pos == 35) {
        //     // print_BB(chess_board.pieces[FULL] & BISHOP_MAGICS[35].mask);
        //     print_BB(piece_moves);
        // }

        if (SQUARE_TO_BB[pos] & pins) {
            pins_idx = chess_board.pins[same_team_pins_idx][pos];
            piece_moves &= in_between(king_pos, pins_idx) | SQUARE_TO_BB[pins_idx];
        }

        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }
    BB same_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[same_team];

    while (same_team_rooks) {
        pos = pop_first_one(same_team_rooks); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) & chess_board.check_ray;

        if (SQUARE_TO_BB[pos] & pins) {
            pins_idx = chess_board.pins[same_team_pins_idx][pos];
            piece_moves &= in_between(king_pos, pins_idx) | SQUARE_TO_BB[pins_idx];
        }

        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    BB same_team_queens = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];

    while (same_team_queens) {
        pos = pop_first_one(same_team_queens); // from
        // moves for rooks and bishops combined
        piece_moves = (get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos])) & chess_board.check_ray;
        
        if (SQUARE_TO_BB[pos] & pins) {
            pins_idx = chess_board.pins[same_team_pins_idx][pos];
            piece_moves &= in_between(king_pos, pins_idx) | SQUARE_TO_BB[pins_idx];
        }
        // std::cout << same_team_pins_idx << std::endl;
        while (piece_moves) {
            new_pos = pop_first_one(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }
    if (moves.size() == 0) {
        if (king & chess_board.pieces[OTHER_TEAM_ATTACKS]) { // checkmate
            chess_board.state = CHECKMATE;
        }
        else { // stalemate
            chess_board.state = DRAW;
        }
    }
    // moves = filter_illegal_moves(chess_board, moves);
}

BB generate_attacks(Board &chess_board) {
    chess_board.check_ray = MAX_VALUE; // so we can and with normal moves to get all legal moves
    int num_of_checks = 0;
    BB cur_attacks;
    BB attacks = 0ULL;
    int pos, same_team, other_team;
    other_team = turn_to_index(!chess_board.turn);
    BB other_team_king = chess_board.pieces[KING] & chess_board.pieces[other_team];
    int other_team_king_pos = zeroes_start(other_team_king);
    // if the king is in check, it should not be able to back into check
    chess_board.pieces[FULL] ^= other_team_king;
    same_team = turn_to_index(chess_board.turn);
    // sliding pieces first because they're easy
    BB same_team_bishops = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    while (same_team_bishops) {
        pos = pop_first_one(same_team_bishops); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (BISHOP_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    BB same_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[same_team];
    while (same_team_rooks) {
        pos = pop_first_one(same_team_rooks); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (ROOK_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    BB same_team_queens = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];
    while (same_team_queens) {
        pos = pop_first_one(same_team_queens); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (BISHOP_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    // knights
    BB same_team_knights = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    while (same_team_knights) {
        pos = pop_first_one(same_team_knights);
        cur_attacks = knight_moves(pos, chess_board.pieces[same_team]); 
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
            // print_BB(chess_board.check_ray);
            // std::cout << "got here" << std::endl;
            // std::cout << pos << std::endl;
            // print_BB(SQUARE_TO_BB[pos]);
        }
        attacks |= cur_attacks;
    }
    // pawns
    BB same_team_pawns = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    int direction = PAWN_DIRECTION_LOOKUP[chess_board.turn];
    while (same_team_pawns) {
        pos = pop_first_one(same_team_pawns); 
        cur_attacks = PAWN_MOVES[chess_board.turn+2][pos];
        if (cur_attacks & other_team_king) {
            // num_of_checks += 1;
            // print_BB(SQUARE_TO_BB[pos]);
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    // king
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    attacks |= KING_MOVES[zeroes_start(king)];

    // king can not check the other king so we dont need to consider it
    chess_board.is_double_check = num_of_checks == 2;

    chess_board.pieces[FULL] ^= other_team_king; // undoing the thing from the start of the function

    return attacks;
}

void set_attack_bitboard(Board &chess_board) {
    chess_board.pieces[OTHER_TEAM_ATTACKS] = generate_attacks(chess_board);
}

BB get_and_set_pins(Board &chess_board) {
    int pos, same_team, other_team, king_pos;
    BB pins = 0ULL;
    same_team = turn_to_index(chess_board.turn);
    other_team = turn_to_index(!chess_board.turn);
    BB other_team_king = chess_board.pieces[KING] & chess_board.pieces[other_team];
    king_pos = zeroes_start(other_team_king);
    BB same_team_rooks = chess_board.pieces[ROOK] & chess_board.pieces[same_team];
    BB no_other_team_pieces = ~(chess_board.pieces[other_team] ^ other_team_king) ^ chess_board.pieces[EMPTY];
    int other_team_pins_idx = !chess_board.turn;
    // std::fill(chess_board.pins[other_team_pins_idx].begin(), chess_board.pins[other_team_pins_idx].end(), -1);
    // BB not_same_team = ~chess_board.pieces[same_team];
    BB cur_attacks, ray;
    int pin_pos;
    while (same_team_rooks) {
        pos = pop_first_one(same_team_rooks);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (ROOK_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        // print_BB(ray & chess_board.pieces[same_team]);
        if ((cur_attacks & other_team_king) && (real_count(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking and the piece itself*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray | SQUARE_TO_BB[pos];
            // std::cout << other_team_pins_idx << std::endl;
            while (ray) {
                pin_pos = pop_first_one(ray);
                chess_board.pins[other_team_pins_idx][pin_pos] = pos;
            }
        }
    }
    BB same_team_bishops = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    // print_BB(same_team_bishops);
    while (same_team_bishops) {
        pos = pop_first_one(same_team_bishops);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        if ((cur_attacks & other_team_king) && (real_count(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray | SQUARE_TO_BB[pos];
            while (ray) {
                pin_pos = pop_first_one(ray);
                chess_board.pins[other_team_pins_idx][pin_pos] = pos;
            }
        }
    }
    // std::cout << "here" << std::endl;

    BB same_team_queens = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];
    while (same_team_queens) {
        pos = pop_first_one(same_team_queens);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) | get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        if ((cur_attacks & other_team_king) && (real_count(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray | SQUARE_TO_BB[pos];
            while (ray) {
                pin_pos = pop_first_one(ray);
                chess_board.pins[other_team_pins_idx][pin_pos] = pos;
            }
        }
    }
    return pins;
}

// bool is_legal(Board &chess_board, Move move, BB king, BB other_team_bishops_and_queens, BB other_team_rooks_and_queens, int same_team) {
//     int from, type;
//     BB cur_bishop_moves, cur_rook_moves, cur_attacks, cur_king;
//     from = move.from;
//     type = move.type;
//     if (type == NORMAL_MOVE) {
//         // generate queen moves for position
//         cur_bishop_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[from]);
//         cur_rook_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[from]);
//         if ((cur_bishop_moves & other_team_bishops_and_queens) || (cur_rook_moves & other_team_rooks_and_queens)) { // the idea is that if we are being attacked by sliding pieces we might be pinned
//             chess_board.play_move(move);
//             cur_attacks = generate_attacks(chess_board);
//             cur_king = chess_board.pieces[same_team] & chess_board.pieces[KING];
//             chess_board.undo_move(move);
//             if (cur_king & cur_attacks) { // king is in check
//                 return false;
//             }
//         }
//     }
//     else if (type == EN_PESSANT) { // it is too complicated to calculate this, so just brute force it
//         chess_board.play_move(move);
//         cur_attacks = generate_attacks(chess_board);
//         if (king & cur_attacks) {
//             return false;
//         }
//     }
//     else if (type & PROMOTION) {
//         cur_bishop_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[from]);
//         cur_rook_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[from]);
//         if ((cur_bishop_moves & other_team_bishops_and_queens) || (cur_rook_moves & other_team_rooks_and_queens)) {
//             chess_board.play_move(move);
//             cur_attacks = generate_attacks(chess_board);
//             chess_board.undo_move(move);
//             if (king & cur_attacks) { // king is in check
//                 return false;
//             }
//         }
//     // for castles, in move generation we already make sure that we do not run into check
//     }
//     return true;
// }

// bool is_legal(Board &chess_board, Move move) {
//     int same_team, other_team;
//     same_team = turn_to_index(chess_board.turn);
//     other_team = turn_to_index(!chess_board.turn);
//     BB other_team_queens = chess_board.pieces[other_team] & chess_board.pieces[QUEEN];
//     return is_legal(chess_board, move, chess_board.pieces[same_team] & chess_board.pieces[KING], (chess_board.pieces[BISHOP] | other_team_queens) & chess_board.pieces[other_team], (chess_board.pieces[ROOK] | other_team_queens) & chess_board.pieces[other_team], same_team);
// }

// std::vector<Move> filter_illegal_moves(Board chess_board, std::vector<Move> moves) {
//     int same_team, other_team;
//     std::vector<Move> legal;
//     same_team = turn_to_index(chess_board.turn);
//     other_team = turn_to_index(!chess_board.turn);
//     BB same_team_king = chess_board.pieces[same_team] & chess_board.pieces[KING];
//     BB other_team_bishops_and_queens = (chess_board.pieces[BISHOP] | chess_board.pieces[QUEEN]) & chess_board.pieces[other_team];
//     BB other_team_rooks_and_queens = (chess_board.pieces[ROOK] | chess_board.pieces[QUEEN]) & chess_board.pieces[other_team];
//     // std::vector<Move> filtered_moves;
//     legal.reserve(MAX_LEGAL_MOVES);
//     for (Move move : moves) {
//         if (!is_legal(chess_board, move, same_team_king, other_team_bishops_and_queens, other_team_rooks_and_queens, same_team)) {
//             continue;
//         }
//         legal.push_back(move);
//     }
//     return legal;
// }
