#include "evaluation.hpp"

// uses koggestone for parrelelism
// https://www.chessprogramming.org/Kogge-Stone_Algorithm#Occluded_Fill
constexpr BB _south_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move >>  8);
    empties &=       (empties >>  8);
    pieces_to_move |= empties & (pieces_to_move >> 16);
    empties &=       (empties >> 16);
    pieces_to_move |= empties & (pieces_to_move >> 32);
    return pieces_to_move | (pieces_to_move >> 8);
}

constexpr BB _north_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move <<  8);
    empties &=       (empties <<  8);
    pieces_to_move |= empties & (pieces_to_move << 16);
    empties &=       (empties << 16);
    pieces_to_move |= empties & (pieces_to_move << 32);
    return pieces_to_move | (pieces_to_move << 8);
}

constexpr BB _west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move << 1);
    empties &=       (empties << 1);
    pieces_to_move |= empties & (pieces_to_move << 2);
    empties &=       (empties << 2);
    pieces_to_move |= empties & (pieces_to_move << 4);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 1);
}

constexpr BB _north_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  9);
    empties &=       (empties <<  9);
    pieces_to_move |= empties & (pieces_to_move << 18);
    empties &=       (empties << 18);
    pieces_to_move |= empties & (pieces_to_move << 36);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 9);
}

constexpr BB _south_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  7);
    empties &=       (empties >>  7);
    pieces_to_move |= empties & (pieces_to_move >> 14);
    empties &=       (empties >> 14);
    pieces_to_move |= empties & (pieces_to_move >> 28);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) >> 7);
}

constexpr BB _east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >> 1);
    empties &=       (empties >> 1);
    pieces_to_move |= empties & (pieces_to_move >> 2);
    empties &=       (empties >> 2);
    pieces_to_move |= empties & (pieces_to_move >> 4);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 1);
}

constexpr BB _south_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  9);
    empties &=       (empties >>  9);
    pieces_to_move |= empties & (pieces_to_move >> 18);
    empties &=       (empties >> 18);
    pieces_to_move |= empties & (pieces_to_move >> 36);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 9);
}

constexpr BB _north_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  7);
    empties &=       (empties <<  7);
    pieces_to_move |= empties & (pieces_to_move << 14);
    empties &=       (empties << 14);
    pieces_to_move |= empties & (pieces_to_move << 28);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) << 7);
}

constexpr BB _pawn_attacks_white(const BB white_pawns) {
    return (
        ((white_pawns & ~(A_FILE)) << 9) |
        ((white_pawns & ~(H_FILE)) << 7)
    );
}

constexpr BB _pawn_attacks_black(const BB black_pawns) {
    return (
        ((black_pawns & ~(H_FILE)) >> 9) |
        ((black_pawns & ~(A_FILE)) >> 7)
    );
}

constexpr BB _knight_attacks(const BB knights) {
    return (
        ((knights & ~(A_FILE | B_FILE)) >> 6) |
        ((knights & ~(G_FILE | H_FILE)) >> 10) |
        ((knights & ~(G_FILE | H_FILE)) << 6) |
        ((knights & ~(A_FILE | B_FILE)) << 10) | 
        ((knights & ~(        A_FILE)) >> 15) | 
        ((knights & ~(        H_FILE)) >> 17) |
        ((knights & ~(        H_FILE)) << 15) |
        ((knights & ~(        A_FILE)) << 17)
        );
}

constexpr BB _bishop_attacks(const BB bishops, const BB empties) {
    return (
        _north_east_ray(bishops, empties) |
        _north_west_ray(bishops, empties) |
        _south_east_ray(bishops, empties) |
        _south_west_ray(bishops, empties)
    );
}

constexpr BB _rook_attacks(const BB rooks, const BB empties) {
    return (
        _north_ray(rooks, empties) |
        _south_ray(rooks, empties) |
        _east_ray(rooks, empties) |
        _west_ray(rooks, empties)
    );
}

constexpr BB _queen_attacks(const BB queens, const BB empties) {
    return (
        _north_ray(queens, empties) |
        _south_ray(queens, empties) |
        _east_ray(queens, empties) |
        _west_ray(queens, empties) |
        _north_east_ray(queens, empties) |
        _north_west_ray(queens, empties) |
        _south_east_ray(queens, empties) |
        _south_west_ray(queens, empties)
    );
} 

constexpr BB _king_attacks(const BB king) {
    return (
        (king & ~(A_FILE)) << 9 |
        (king & ~(A_FILE)) << 1 |
        (king & ~(A_FILE)) >> 7 |
        (king & ~(H_FILE)) << 7 |
        (king & ~(H_FILE)) >> 1 |
        (king & ~(H_FILE)) >> 9 |
        (king            ) << 8 |
        (king            ) >> 8  
    );
}

int evaluate(const Board &chess_board) noexcept { // neutral evaluation of position
    int evaluation = 0;
    // tempo
    if (chess_board.turn) evaluation += TEMPO;
    else evaluation -= TEMPO; 

    const BB black_pawns = chess_board.pieces[BLACK] & chess_board.pieces[PAWN];
    const BB white_pawns = chess_board.pieces[WHITE] & chess_board.pieces[PAWN];

    int bishop_count = 0;
    BB black_minor_piece_attacks = _bishop_attacks(chess_board.pieces[BLACK] & chess_board.pieces[BISHOP], chess_board.pieces[EMPTY]) | _knight_attacks(chess_board.pieces[BLACK] & chess_board.pieces[KNIGHT]);
    BB black_rook_attacks = _rook_attacks(chess_board.pieces[BLACK] & chess_board.pieces[ROOK], chess_board.pieces[EMPTY]);
    BB white_pieces = chess_board.pieces[WHITE];
    while (white_pieces) {
        int pos = pop_lsb(white_pieces);
        int piece_type = chess_board.piece_data[pos];

        if (piece_type == PAWN) {
            evaluation += PIECE_SQUARE_TABLES[/*0 + */pos];
            int pos_file = file(pos);
            int pos_rank = rank(pos);
            BB sides = ((pos_file < 7) * (H_FILE << (pos_file + 1))) |
                       ((pos_file > 0) * (H_FILE << (pos_file - 1)));
            BB same_file = H_FILE << pos_file;
            // isolated pawn
            evaluation += ISOLATED_PAWN * !(sides & white_pawns);
            // doubled pawn
            evaluation += DOUBLED_PAWN * (((same_file & passed_pawn_mask_white[pos]) & white_pawns) != 0ULL);
            // passed pawn
            evaluation += PASSED_PAWN_RANK[pos_rank-1] * !((passed_pawn_mask_white[pos] & (sides | same_file)) & (black_pawns));
            // protected pawn
            bool is_protected = PAWN_MOVES[2][pos] & (white_pawns);
            evaluation += PROTECTED_PAWN_RANK[pos_rank-1] * is_protected;
            // threat by minor piece, ignore if protected by pawn
            evaluation += THREAT_BY_MINOR_PIECE[0] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) && !is_protected);
            // threat by rook, ignore if protected by pawn
            evaluation += THREAT_BY_ROOK[0] * ((SQUARE_TO_BB[pos] & black_rook_attacks) && !is_protected);
        }

        else if (piece_type == KNIGHT) {
            evaluation += PIECE_SQUARE_TABLES[64 + pos];
            // threat by minor piece
            evaluation += THREAT_BY_MINOR_PIECE[1] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) != 0ULL);
            // threat by rook, ignore if protected by pawn
            evaluation += THREAT_BY_ROOK[1] * ((SQUARE_TO_BB[pos] & black_rook_attacks) && !(white_pawns & PAWN_MOVES[2][pos]));
        }

        else if (piece_type == BISHOP) {
            evaluation += PIECE_SQUARE_TABLES[128 + pos];
            bishop_count++;
            // threat by minor piece
            evaluation += THREAT_BY_MINOR_PIECE[2] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) != 0ULL);
            // threat by rook, ignore if protected by pawn
            evaluation += THREAT_BY_ROOK[2] * ((SQUARE_TO_BB[pos] & black_rook_attacks) && !(white_pawns & PAWN_MOVES[2][pos]));
        }

        else if (piece_type == ROOK) {
            evaluation += PIECE_SQUARE_TABLES[192 + pos];
            int pos_file = file(pos);
            // rook on open file
            evaluation += ROOK_ON_OPEN_FILE * chess_board.is_open_file(pos_file);
            // rook on semi open file
            evaluation += ROOK_ON_SEMI_OPEN_FILE * chess_board.is_semi_open_file(pos_file, WHITE, BLACK);
            // threat by minor piece
            evaluation += THREAT_BY_MINOR_PIECE[3] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) != 0ULL);
            // threat by rook
            evaluation += THREAT_BY_ROOK[3] * ((SQUARE_TO_BB[pos] & black_rook_attacks) != 0ULL);
        }

        else if (piece_type == QUEEN) {
            evaluation += PIECE_SQUARE_TABLES[256 + pos];
            // threat by minor piece
            evaluation += THREAT_BY_MINOR_PIECE[4] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) != 0ULL);
            // threat by rook
            evaluation += THREAT_BY_ROOK[4] * ((SQUARE_TO_BB[pos] & black_rook_attacks) != 0ULL);
        }

        else if (piece_type == KING) {
            evaluation += PIECE_SQUARE_TABLES[320 + pos];
            int pos_file = file(pos);
            // king on open file
            evaluation += KING_ON_OPEN_FILE * chess_board.is_open_file(pos_file);
            // king on semi open file
            evaluation += KING_ON_SEMI_OPEN_FILE * chess_board.is_semi_open_file(pos_file, WHITE, BLACK);
            // threat (check) by minor piece
            evaluation += THREAT_BY_MINOR_PIECE[5] * ((SQUARE_TO_BB[pos] & black_minor_piece_attacks) != 0ULL);
            // threat (check) by rook
            evaluation += THREAT_BY_ROOK[5] * ((SQUARE_TO_BB[pos] & black_rook_attacks) != 0ULL);
            // pawn shields
            int king_pos = lsb(chess_board.pieces[WHITE] & chess_board.pieces[KING]);
            if (king_pos < 54 && king_pos && king_pos != 3) {
                evaluation += PAWN_SHIELDS[0] * (get_bit(white_pawns, pos+9) != 0ULL);
                evaluation += PAWN_SHIELDS[1] * (get_bit(white_pawns, pos+8) != 0ULL);
                evaluation += PAWN_SHIELDS[2] * (get_bit(white_pawns, pos+7) != 0ULL);
            }

            // king ring attacks and defenses
            BB king_ring = KING_MOVES[pos];
            BB pawn_attacks_white_ = _pawn_attacks_white(king_ring);
            BB knight_attacks_white = _knight_attacks(king_ring);
            BB bishop_attacks_white = _bishop_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB rook_attacks_white = _rook_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB queen_attacks_white = _queen_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB king_attacks_white = _king_attacks(king_ring);
            
            // king ring attacks
            evaluation += KING_RING_ATTACKS[0] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[PAWN]) & pawn_attacks_white_); 
            evaluation += KING_RING_ATTACKS[1] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[KNIGHT]) & knight_attacks_white); 
            evaluation += KING_RING_ATTACKS[2] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[BISHOP]) & bishop_attacks_white); 
            evaluation += KING_RING_ATTACKS[3] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[ROOK]) & rook_attacks_white); 
            evaluation += KING_RING_ATTACKS[4] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[QUEEN]) & queen_attacks_white); 
            evaluation += KING_RING_ATTACKS[5] * (((chess_board.pieces[BLACK] & chess_board.pieces[KING]) & king_attacks_white) != 0ULL);

            // king ring defenses
            evaluation += KING_RING_DEFENSES[0] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[PAWN]) & pawn_attacks_white_);
            evaluation += KING_RING_DEFENSES[1] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[KNIGHT]) & knight_attacks_white);
            evaluation += KING_RING_DEFENSES[2] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[BISHOP]) & bishop_attacks_white);
            evaluation += KING_RING_DEFENSES[3] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[ROOK]) & rook_attacks_white);
            evaluation += KING_RING_DEFENSES[4] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[QUEEN]) & queen_attacks_white);
        }
    }

    evaluation += BISHOP_PAIR * (bishop_count > 1);

    bishop_count = 0;
    BB white_minor_piece_attacks = _bishop_attacks(chess_board.pieces[WHITE] & chess_board.pieces[BISHOP], chess_board.pieces[EMPTY]) | _knight_attacks(chess_board.pieces[WHITE] & chess_board.pieces[KNIGHT]);
    BB white_rook_attacks = _rook_attacks(chess_board.pieces[WHITE] & chess_board.pieces[ROOK], chess_board.pieces[EMPTY]);
    BB black_pieces = chess_board.pieces[BLACK];
    while (black_pieces) {
        int pos = pop_lsb(black_pieces);
        int piece_type = no_color(chess_board.piece_data[pos]);

        if (piece_type == PAWN) {
            evaluation -= PIECE_SQUARE_TABLES[/*0 + */pos ^ 56];
            int pos_file = file(pos);
            int pos_rank = rank(pos);
            BB sides = ((pos_file < 7) * (H_FILE << (pos_file + 1))) |
                       ((pos_file > 0) * (H_FILE << (pos_file - 1)));
            BB same_file = H_FILE << pos_file;
            // isolated pawn
            evaluation -= ISOLATED_PAWN * !(sides & black_pawns);
            // doubled pawn
            evaluation -= DOUBLED_PAWN * (((same_file & passed_pawn_mask_black[pos]) & black_pawns) != 0ULL);
            // passed pawn
            evaluation -= PASSED_PAWN_RANK[6-pos_rank] * !((passed_pawn_mask_black[pos] & (sides | same_file)) & white_pawns);
            // protected pawn
            bool is_protected = /*white attacks from square*/PAWN_MOVES[3][pos] & (black_pawns);
            evaluation -= PROTECTED_PAWN_RANK[6-pos_rank] * is_protected;
            // threat by minor piece, ignore if protected by pawn
            evaluation -= THREAT_BY_MINOR_PIECE[0] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) && !is_protected);
            // threat by rook, ignore if protected by pawn
            evaluation -= THREAT_BY_ROOK[0] * ((SQUARE_TO_BB[pos] & white_rook_attacks) && !is_protected);
        }

        else if (piece_type == KNIGHT) {
            evaluation -= PIECE_SQUARE_TABLES[64 + (pos ^ 56)];
            // threat by minor piece
            evaluation -= THREAT_BY_MINOR_PIECE[1] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) != 0ULL);
            // threat by rook, ignore if protected by pawn
            evaluation -= THREAT_BY_ROOK[1] * ((SQUARE_TO_BB[pos] & white_rook_attacks) && !(black_pawns & PAWN_MOVES[3][pos]));
        }

        else if (piece_type == BISHOP) {
            evaluation -= PIECE_SQUARE_TABLES[128 + (pos ^ 56)];
            bishop_count++;
            // threat by minor piece
            evaluation -= THREAT_BY_MINOR_PIECE[2] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) != 0ULL);
            // threat by rook, ignore if protected by pawn
            evaluation -= THREAT_BY_ROOK[2] * ((SQUARE_TO_BB[pos] & white_rook_attacks) && !(black_pawns & PAWN_MOVES[3][pos]));
        }

        else if (piece_type == ROOK) {
            evaluation -= PIECE_SQUARE_TABLES[192 + (pos ^ 56)];
            int pos_file = file(pos);
            // rook on open file
            evaluation -= ROOK_ON_OPEN_FILE * chess_board.is_open_file(pos_file);
            // rook on semi open file
            evaluation -= ROOK_ON_SEMI_OPEN_FILE * chess_board.is_semi_open_file(pos_file, BLACK, WHITE);
            // threat by minor piece
            evaluation -= THREAT_BY_MINOR_PIECE[3] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) != 0ULL);
            // threat by rook
            evaluation -= THREAT_BY_ROOK[3] * ((SQUARE_TO_BB[pos] & white_rook_attacks) != 0ULL);
        }

        else if (piece_type == QUEEN) {
            evaluation -= PIECE_SQUARE_TABLES[256 + (pos ^ 56)];
            // threat by minor piece
            evaluation -= THREAT_BY_MINOR_PIECE[4] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) != 0ULL);
            // threat by rook
            evaluation -= THREAT_BY_ROOK[4] * ((SQUARE_TO_BB[pos] & white_rook_attacks) != 0ULL);
        }

        else if (piece_type == KING) {
            evaluation -= PIECE_SQUARE_TABLES[320 + (pos ^ 56)];
            int pos_file = file(pos);
            // king on open file
            evaluation -= KING_ON_OPEN_FILE * chess_board.is_open_file(pos_file);
            // king on semi open file
            evaluation -= KING_ON_SEMI_OPEN_FILE * chess_board.is_semi_open_file(pos_file, BLACK, WHITE);
            // threat (check) by minor piece
            evaluation -= THREAT_BY_MINOR_PIECE[5] * ((SQUARE_TO_BB[pos] & white_minor_piece_attacks) != 0ULL);
            // threat (check) by rook
            evaluation -= THREAT_BY_ROOK[5] * ((SQUARE_TO_BB[pos] & white_rook_attacks) != 0ULL);
            // pawn shields
            int king_pos = lsb(chess_board.pieces[BLACK] & chess_board.pieces[KING]);
            if (king_pos > 9 && king_pos && king_pos != 59) {
                evaluation -= PAWN_SHIELDS[0] * (get_bit(black_pawns, pos-7) != 0ULL);
                evaluation -= PAWN_SHIELDS[1] * (get_bit(black_pawns, pos-8) != 0ULL);
                evaluation -= PAWN_SHIELDS[2] * (get_bit(black_pawns, pos-9) != 0ULL);
            }

            // king ring attacks and defenses
            BB king_ring = KING_MOVES[pos];
            BB pawn_attacks_black_ = _pawn_attacks_black(king_ring);
            BB knight_attacks_black = _knight_attacks(king_ring);
            BB bishop_attacks_black = _bishop_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB rook_attacks_black = _rook_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB queen_attacks_black = _queen_attacks(king_ring, chess_board.pieces[EMPTY]);
            BB king_attacks_black = _king_attacks(king_ring);
            
            // king ring attacks
            evaluation -= KING_RING_ATTACKS[0] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[PAWN]) & pawn_attacks_black_); 
            evaluation -= KING_RING_ATTACKS[1] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[KNIGHT]) & knight_attacks_black); 
            evaluation -= KING_RING_ATTACKS[2] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[BISHOP]) & bishop_attacks_black); 
            evaluation -= KING_RING_ATTACKS[3] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[ROOK]) & rook_attacks_black); 
            evaluation -= KING_RING_ATTACKS[4] * count_bits((chess_board.pieces[WHITE] & chess_board.pieces[QUEEN]) & queen_attacks_black); 
            evaluation -= KING_RING_ATTACKS[5] * (((chess_board.pieces[WHITE] & chess_board.pieces[KING]) & king_attacks_black) != 0ULL);

            // king ring defenses
            evaluation -= KING_RING_DEFENSES[0] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[PAWN]) & pawn_attacks_black_);
            evaluation -= KING_RING_DEFENSES[1] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[KNIGHT]) & knight_attacks_black);
            evaluation -= KING_RING_DEFENSES[2] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[BISHOP]) & bishop_attacks_black);
            evaluation -= KING_RING_DEFENSES[3] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[ROOK]) & rook_attacks_black);
            evaluation -= KING_RING_DEFENSES[4] * count_bits((chess_board.pieces[BLACK] & chess_board.pieces[QUEEN]) & queen_attacks_black);
        }
    }

    evaluation -= BISHOP_PAIR * (bishop_count > 1);
    // printf("%i %i\n", static_cast<int16_t>(evaluation), static_cast<int16_t>((evaluation + 0x8000) >> 16));
    // linear interpolation based on phase
    //            (count(queens) * 4                               count(rooks) * 2                             (count(bishop)                        +  count(knight)))
    int phase = ((count_bits(chess_board.pieces[QUEEN]) << 2) + (count_bits(chess_board.pieces[ROOK]) << 1) + count_bits(chess_board.pieces[BISHOP] | chess_board.pieces[KNIGHT]));
    return static_cast<int>(static_cast<float>(static_cast<int16_t>((evaluation + 0x8000) >> 16) * phase + (24 - phase) * static_cast<int16_t>(evaluation)) / 24.0f);
}