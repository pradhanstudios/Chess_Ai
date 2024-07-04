#include "generate_moves.hpp"

void generate_psuedo_legal_moves(Board &chess_board, std::vector<Move> &movelist, bool legal) {
    if (chess_board.state != RUNNING) {
        return;
    }

    History cur_history = chess_board.history.back();
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!chess_board.turn];

    std::vector<Move> moves;
    moves.reserve(512); // idk what the max psuedo legal moves are in a position

    const BB king = chess_board.pieces[same_team] & chess_board.pieces[KING];
    const int king_pos = lsb(king);
    BB king_attackers = chess_board.square_attacks(king_pos, same_team);
    int num_of_checks = count_bits(king_attackers);

    BB mask = MAX_VALUE;
    if (num_of_checks > 0) {
        BB lsb_king_attackers = king_attackers & -king_attackers;
        mask = in_between(king_pos, lsb(king_attackers));
        generate_non_pawn_moves<KING>(chess_board, same_team, king, ~mask /*opposite mask for king because it can not get in the way*/, moves);
        mask |= lsb_king_attackers;
    }

    else {
        generate_non_pawn_moves<KING>(chess_board, same_team, king, mask, moves);
    }
    
    if (num_of_checks > 1) { // if it is double check then no point in generating other moves
        goto end;
    }

{
    generate_castles(chess_board, king, king_attackers, cur_history, moves);

    BB same_team_pieces = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    generate_promotions<CAPTURE>(chess_board, same_team_pieces, chess_board.pieces[other_team], mask, moves);
    generate_promotions<QUIET>(chess_board, same_team_pieces, chess_board.pieces[other_team], mask, moves);
    generate_normal_pawn_moves<CAPTURE>(chess_board, same_team_pieces, chess_board.pieces[other_team], mask, moves);
    generate_en_pesssant_moves(chess_board, same_team_pieces, cur_history, moves);
    generate_normal_pawn_moves<QUIET>(chess_board, same_team_pieces, chess_board.pieces[other_team], mask, moves);
    

    // all non pawn move generation are all basically the same, so we can all group them together into one function (generate_non_pawn_moves())
    same_team_pieces = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    
    generate_non_pawn_moves<KNIGHT>(chess_board, same_team, same_team_pieces, mask, moves);

    same_team_pieces = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];

    generate_non_pawn_moves<BISHOP>(chess_board, same_team, same_team_pieces, mask, moves);

    same_team_pieces = chess_board.pieces[ROOK] & chess_board.pieces[same_team];

    generate_non_pawn_moves<ROOK>(chess_board, same_team, same_team_pieces, mask, moves);

    same_team_pieces = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];

    generate_non_pawn_moves<QUEEN>(chess_board, same_team, same_team_pieces, mask, moves);
}
end:
    if (legal) {
        filter_illegal_moves(chess_board, moves, movelist);

        if (movelist.size() == 0) {

            if (king_attackers) { // checkmate
                chess_board.state = CHECKMATE;
            }

            else { // stalemate
                chess_board.state = DRAW;
            }
        }
    }
}

void filter_illegal_moves(Board &chess_board, std::vector<Move> &psuedo_legal, std::vector<Move> &legal) {
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    BB king = chess_board.pieces[same_team] & chess_board.pieces[KING];
    const BB possibly_pinned = chess_board.possibly_pinned_pieces_from_square(lsb(king), same_team);

    for (const Move &move : psuedo_legal) {
        if (chess_board.is_move_legal(move, possibly_pinned, same_team, king)) {
            legal.push_back(move);
        }
    }
}

void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) noexcept { // should be reserved to size 218
    if (chess_board.state != RUNNING) {
        return;
    }

    History cur_history = chess_board.history.back();
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!chess_board.turn];

    // king

    chess_board.next_turn(); // skip turn
    set_attack_bitboard(chess_board);
    BB pins = get_and_set_pins(chess_board);
    chess_board.next_turn(); // go back
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    int king_pos = lsb(king);
    BB piece_moves = king_moves(king_pos, chess_board.pieces[same_team], chess_board.pieces[OTHER_TEAM_ATTACKS]);
    int new_pos = 0;
    while (piece_moves) {
        new_pos = pop_lsb(piece_moves);
        moves.push_back((Move(king_pos, new_pos, NORMAL_MOVE)));
    }

    if (chess_board.is_double_check) {
        if (moves.size() == 0) {
            if (king & chess_board.pieces[OTHER_TEAM_ATTACKS]) { // checkmate
                chess_board.state = CHECKMATE;
            }
        }
        return; // if it is double check there is no point in checking other moves
    }
    BB not_king = (chess_board.pieces[FULL] ^ (chess_board.pieces[same_team] & chess_board.pieces[KING]));
    int cur_check_allowance = cur_history.castle;
    int color_to_position = !chess_board.turn * 56;
    // kingside castle
    int shift = !chess_board.turn << 1;
    if (!((0b1110ULL << color_to_position) & (not_king | chess_board.pieces[OTHER_TEAM_ATTACKS])) && (cur_check_allowance & SQUARE_TO_BB[shift])) {
        moves.push_back((Move(3 + color_to_position, 1 + color_to_position, CASTLE, EMPTY, KINGSIDE_CASTLE)));
    }

    // queenside castle
    shift++; // because queenside castles are after in the mask
    
    if (!((0b01110000ULL << color_to_position) & not_king) && (cur_check_allowance & SQUARE_TO_BB[shift]) && !((0b00111000ULL << color_to_position) & chess_board.pieces[OTHER_TEAM_ATTACKS])) {
        moves.push_back((Move(3 + color_to_position, 5 + color_to_position, CASTLE, EMPTY, QUEENSIDE_CASTLE)));
    }
    
    BB same_team_pieces = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    int pos, rank_;
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        
        piece_moves = pawn_moves(pos, chess_board.pieces[other_team], chess_board.pieces[EMPTY], chess_board.turn) & chess_board.check_ray;
        
        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }
        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
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
        if ((chess_board.pieces[FULL] & SQUARE_TO_BB[cur_history.en_pessant]) && (cur_history.en_pessant != 0) && (abs(cur_history.en_pessant - pos) < 2) && (count_bits((SQUARE_TO_BB[cur_history.en_pessant] | SQUARE_TO_BB[pos]) & EDGES) != 2)) { // if it is one off of the en pessant pawn
            Move en_pessant_move = Move(pos, cur_history.en_pessant + PAWN_DIRECTION_LOOKUP[chess_board.turn], EN_PESSANT);
            // check if it results in a check
            chess_board.play_move(en_pessant_move);
            BB check_ray_copy = chess_board.check_ray;
            BB cur_attacks = generate_attacks(chess_board);

            if (!(king & cur_attacks)) {
                moves.push_back(en_pessant_move);
            }
            chess_board.check_ray = check_ray_copy;
            chess_board.undo_move(en_pessant_move);
        }
    }
    // knights
    same_team_pieces = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = knight_moves(pos, chess_board.pieces[same_team]) & chess_board.check_ray;

        if (SQUARE_TO_BB[pos] & pins) {
            chess_board.pins[chess_board.turn][pos] = chess_board.pins[chess_board.turn][pos];
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    // bishops
    
    same_team_pieces = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]) & chess_board.check_ray;

        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }
    // rooks
    same_team_pieces = chess_board.pieces[ROOK] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) & chess_board.check_ray;

        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    // queens
    same_team_pieces = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        // moves for rooks and bishops combined
        piece_moves = (get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos])) & chess_board.check_ray;
        
        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }
        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
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
}

void generate_captures(Board &chess_board, std::vector<Move> &moves) noexcept {
    if (chess_board.state != RUNNING) {
        return;
    }
    History cur_history = chess_board.history.back();
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!chess_board.turn];
    bool is_one_quiet_move = false;
    // king
    chess_board.next_turn(); // skip turn
    set_attack_bitboard(chess_board);
    BB pins = get_and_set_pins(chess_board);
    chess_board.next_turn(); // go back
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    int king_pos = lsb(king);
    BB piece_moves = king_moves(king_pos, chess_board.pieces[same_team], chess_board.pieces[OTHER_TEAM_ATTACKS]) & chess_board.pieces[other_team];
    int new_pos = 0;

    while (piece_moves) {
        new_pos = pop_lsb(piece_moves);
        moves.push_back((Move(king_pos, new_pos, NORMAL_MOVE)));
    }

    if (chess_board.is_double_check) {
        if (count_bits(king_moves(king_pos, chess_board.pieces[same_team], chess_board.pieces[OTHER_TEAM_ATTACKS])) == 0) {
            if (king & chess_board.pieces[OTHER_TEAM_ATTACKS]) { // checkmate
                chess_board.state = CHECKMATE;
            }
        }
        return; // if it is double check there is no point in checking other moves
    }

    BB same_team_pieces = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    int pos, rank_;
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        piece_moves = PAWN_MOVES[chess_board.turn + 2][pos] & chess_board.pieces[other_team];
        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
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
        if ((chess_board.pieces[FULL] & SQUARE_TO_BB[cur_history.en_pessant]) && (cur_history.en_pessant != 0) && (abs(cur_history.en_pessant - pos) < 2) && (count_bits((SQUARE_TO_BB[cur_history.en_pessant] | SQUARE_TO_BB[pos]) & EDGES) != 2)) { // if it is one off of the en pessant pawn
            Move en_pessant_move = Move(pos, cur_history.en_pessant + PAWN_DIRECTION_LOOKUP[chess_board.turn], EN_PESSANT);
            chess_board.play_move(en_pessant_move);
            BB check_ray_copy = chess_board.check_ray;
            BB cur_attacks = generate_attacks(chess_board);

            if (!(king & cur_attacks)) {
                moves.push_back(en_pessant_move);
            }
            chess_board.check_ray = check_ray_copy;
            chess_board.undo_move(en_pessant_move);
        }
    }
    // knights
    same_team_pieces = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    
    BB not_other_team_pieces = ~chess_board.pieces[other_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = (knight_moves(pos, chess_board.pieces[same_team]) & chess_board.check_ray);
        is_one_quiet_move |= piece_moves & not_other_team_pieces;
        piece_moves &= chess_board.pieces[other_team];

        if (SQUARE_TO_BB[pos] & pins) {
            chess_board.pins[chess_board.turn][pos] = chess_board.pins[chess_board.turn][pos];
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    // bishops
    
    same_team_pieces = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = (get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]) & chess_board.check_ray);
        is_one_quiet_move |= piece_moves & not_other_team_pieces;
        piece_moves &= chess_board.pieces[other_team];

        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }
    // rooks
    same_team_pieces = chess_board.pieces[ROOK] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        piece_moves = (get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) & chess_board.check_ray);
        is_one_quiet_move |= piece_moves & not_other_team_pieces;
        piece_moves &= chess_board.pieces[other_team];

        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }

        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    // queens
    same_team_pieces = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];

    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        // moves for rooks and bishops combined
        piece_moves = ((get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], (BISHOP_MAGICS)[pos])) & chess_board.check_ray);
        is_one_quiet_move |= piece_moves & not_other_team_pieces;
        piece_moves &= chess_board.pieces[other_team];
        
        if (SQUARE_TO_BB[pos] & pins) {
            piece_moves &= in_between(king_pos, chess_board.pins[chess_board.turn][pos]) | SQUARE_TO_BB[chess_board.pins[chess_board.turn][pos]];
        }
        
        while (piece_moves) {
            new_pos = pop_lsb(piece_moves);
            moves.push_back((Move(pos, new_pos, NORMAL_MOVE)));
        }
    }

    if (moves.size() == 0 && !is_one_quiet_move) {
        if (king & chess_board.pieces[OTHER_TEAM_ATTACKS]) { // checkmate
            chess_board.state = CHECKMATE;
        }
        else { // stalemate
            chess_board.state = DRAW;
        }
    }
}

BB generate_attacks(Board &chess_board) noexcept {
    chess_board.check_ray = MAX_VALUE; // so we can and with normal moves to get all legal moves
    int num_of_checks = 0;
    int other_team = TURN_TO_INDEX_LOOKUP[!chess_board.turn];
    BB other_team_king = chess_board.pieces[KING] & chess_board.pieces[other_team];
    int other_team_king_pos = lsb(other_team_king);
    // if the king is in check, it should not be able to back into check
    chess_board.pieces[FULL] ^= other_team_king;
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    // sliding pieces first because they're easy
    BB same_team_pieces = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    BB attacks = 0ULL;
    BB cur_attacks;
    int pos;
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (BISHOP_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    same_team_pieces = chess_board.pieces[ROOK] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (ROOK_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    same_team_pieces = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); // from
        cur_attacks = get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (ROOK_MAGICS)[pos]) | get_sliding_moves(chess_board.pieces[FULL], 0ULL /*0ULL because we want to include protected squares*/, (BISHOP_MAGICS)[pos]);
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    // knights
    same_team_pieces = chess_board.pieces[KNIGHT] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        cur_attacks = knight_moves(pos, 0ULL); 
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    // pawns
    same_team_pieces = chess_board.pieces[PAWN] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces); 
        cur_attacks = PAWN_MOVES[chess_board.turn+2][pos];
        if (cur_attacks & other_team_king) {
            num_of_checks += 1;
            chess_board.check_ray = in_between(pos, other_team_king_pos) | SQUARE_TO_BB[pos];
        }
        attacks |= cur_attacks;
    }
    // king
    BB king = chess_board.pieces[KING] & chess_board.pieces[same_team];
    attacks |= KING_MOVES[lsb(king)];

    // king can not check the other king so we dont need to consider it
    chess_board.is_double_check = num_of_checks > 1;

    chess_board.pieces[FULL] ^= other_team_king; // undoing the thing from the start of the function

    return attacks;
}

void set_attack_bitboard(Board &chess_board) noexcept {
    chess_board.pieces[OTHER_TEAM_ATTACKS] = generate_attacks(chess_board);
}

BB get_and_set_pins(Board &chess_board) noexcept {
    BB pins = 0ULL;
    int same_team = TURN_TO_INDEX_LOOKUP[chess_board.turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!chess_board.turn];
    BB other_team_king = chess_board.pieces[KING] & chess_board.pieces[other_team];
    int king_pos = lsb(other_team_king);
    BB no_other_team_pieces = ~(chess_board.pieces[other_team] ^ other_team_king) ^ chess_board.pieces[EMPTY];
    int other_team_pins_idx = !chess_board.turn;
    BB same_team_pieces = chess_board.pieces[ROOK] & chess_board.pieces[same_team];
    BB cur_attacks, ray;
    int pos;
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (ROOK_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        if ((cur_attacks & other_team_king) && (count_bits(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking and the piece itself*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray;
            pins |= SQUARE_TO_BB[pos];
            while (ray) {
                chess_board.pins[other_team_pins_idx][pop_lsb(ray)] = pos;
            }
        }
    }
    same_team_pieces = chess_board.pieces[BISHOP] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        if ((cur_attacks & other_team_king) && (count_bits(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray;
            pins |= SQUARE_TO_BB[pos];
            while (ray) {
                chess_board.pins[other_team_pins_idx][pop_lsb(ray)] = pos;
            }
        }
    }

    same_team_pieces = chess_board.pieces[QUEEN] & chess_board.pieces[same_team];
    while (same_team_pieces) {
        pos = pop_lsb(same_team_pieces);
        cur_attacks = get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (ROOK_MAGICS)[pos]) | get_sliding_moves(no_other_team_pieces, chess_board.pieces[same_team], (BISHOP_MAGICS)[pos]);
        ray = in_between(pos, king_pos);
        if ((cur_attacks & other_team_king) && (count_bits(ray & chess_board.pieces[other_team]) < 2 /*only one piece blocking*/ && !(ray & chess_board.pieces[same_team]))) {
            pins |= ray;
            pins |= SQUARE_TO_BB[pos];
            while (ray) {
                chess_board.pins[other_team_pins_idx][pop_lsb(ray)] = pos;
            }
        }
    }
    return pins;
}