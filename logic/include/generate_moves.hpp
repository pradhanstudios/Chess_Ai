#ifndef GENERATE_MOVES_HPP
#define GENERATE_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "non_sliding_moves.hpp"
#include "sliding_moves.hpp"
#include "move.hpp"
#include "board.hpp"
#include "magic_values.hpp"

enum MOVEGEN_TYPE {
    CAPTURE = 0b1,
    QUIET = 0b10,
    ALL = 0b11
};

inline std::array<BB, 4096> in_between_lookup;
inline BB in_between(const int i, const int j) noexcept {
    return in_between_lookup[(i<<6) + j];
}

constexpr void bitloop(BB bitboard, int original_pos, std::vector<Move> &moves) noexcept {
    while (bitboard) {
        int pos = pop_lsb(bitboard);
        moves.push_back(Move(original_pos, pos, NORMAL_MOVE));
    }
}

template<PIECE piece>
constexpr void generate_non_pawn_moves_one_piece(Board &chess_board, int same_team, int pos, BB mask, std::vector<Move> &moves) {
    static_assert(no_color(piece) != PAWN);

    if constexpr (piece == KNIGHT) {
        BB knight_moves_ = knight_moves(pos, chess_board.pieces[same_team]) & mask;
        bitloop(knight_moves_, pos, moves);
    }

    else if constexpr (piece == BISHOP) {
        BB bishop_moves_ = bishop_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos) & mask;
        bitloop(bishop_moves_, pos, moves);
    }

    else if constexpr (piece == ROOK) {
        BB rook_moves_ = rook_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos) & mask;
        bitloop(rook_moves_, pos, moves);
    }

    else if constexpr (piece == QUEEN) {
        BB queen_moves_ = queen_moves(chess_board.pieces[FULL], chess_board.pieces[same_team], pos) & mask;
        bitloop(queen_moves_, pos, moves);
    }

    else if constexpr (piece == KING) {
        BB king_moves_ = (KING_MOVES[pos] & ~chess_board.pieces[same_team]) & mask;
        bitloop(king_moves_, pos, moves);
    }
}

template <PIECE piece>
constexpr void generate_non_pawn_moves(Board &chess_board, int same_team, BB pieces, BB mask, std::vector<Move> &moves) {
    while (pieces) {
        int pos = pop_lsb(pieces);
        generate_non_pawn_moves_one_piece<piece>(chess_board, same_team, pos, mask, moves);
    }
}

constexpr void generate_en_pesssant_moves(Board &chess_board, BB same_team_pawns, History &cur_history, std::vector<Move> &moves) {
    while (same_team_pawns) {
        int pos = pop_lsb(same_team_pawns);
        if ((chess_board.pieces[FULL] & SQUARE_TO_BB[cur_history.en_pessant]) && (cur_history.en_pessant != 0) && (abs(cur_history.en_pessant - pos) < 2) && (count_bits((SQUARE_TO_BB[cur_history.en_pessant] | SQUARE_TO_BB[pos]) & EDGES) != 2)) { // if it is one off of the en pessant pawn
            moves.push_back(Move(pos, cur_history.en_pessant + PAWN_DIRECTION_LOOKUP[chess_board.turn], EN_PESSANT));
        }
    }
}

template <MOVEGEN_TYPE movegen_type>
constexpr void generate_promotions(Board &chess_board, BB same_team_pawns, BB other_team, BB mask, std::vector<Move> &moves) {
    same_team_pawns &= PAWN_ALMOST_PROMOTION_RANK_LOOKUP[chess_board.turn];
    BB attacks;
    while (same_team_pawns) {
        int pos = pop_lsb(same_team_pawns);
        if constexpr (movegen_type & CAPTURE) {
            attacks = PAWN_MOVES[chess_board.turn + 2][pos] & other_team & mask;
            while (attacks) {
                int new_pos = pop_lsb(attacks);
                moves.push_back((Move(pos, new_pos, PROMOTION, QUEEN)));
                moves.push_back((Move(pos, new_pos, PROMOTION, ROOK)));
                moves.push_back((Move(pos, new_pos, PROMOTION, KNIGHT)));
                moves.push_back((Move(pos, new_pos, PROMOTION, BISHOP)));
            }
        }

        if constexpr (movegen_type & QUIET) {
            attacks = PAWN_MOVES[chess_board.turn][pos] & chess_board.pieces[EMPTY] & mask;
            while (attacks) {
                int new_pos = pop_lsb(attacks);
                moves.push_back((Move(pos, new_pos, PROMOTION, QUEEN)));
                moves.push_back((Move(pos, new_pos, PROMOTION, ROOK)));
                moves.push_back((Move(pos, new_pos, PROMOTION, KNIGHT)));
                moves.push_back((Move(pos, new_pos, PROMOTION, BISHOP)));
            }
        }
    }
}

template <MOVEGEN_TYPE movegen_type>
constexpr void generate_normal_pawn_moves(Board &chess_board, BB same_team_pawns, BB other_team, BB mask, std::vector<Move> &moves) {
    same_team_pawns &= ~PAWN_ALMOST_PROMOTION_RANK_LOOKUP[chess_board.turn];
    BB attacks;
    while (same_team_pawns) {
        int pos = pop_lsb(same_team_pawns);
        if constexpr (movegen_type & CAPTURE) {
            attacks = pawn_capture_moves(pos, other_team, chess_board.turn) & mask;
            bitloop(attacks, pos, moves);
        }

        if constexpr (movegen_type & QUIET) {
            attacks = pawn_non_capture_moves(pos, chess_board.pieces[EMPTY], chess_board.turn) & mask;
            bitloop(attacks, pos, moves);
        }  
    }
}

constexpr void generate_castles(Board &chess_board, BB same_team_king, BB king_attackers, History &cur_history, std::vector<Move> &moves) {
    BB not_king = (chess_board.pieces[FULL] ^ same_team_king);
    int shift = !chess_board.turn << 1;
    int color_to_position = !chess_board.turn * 56; 
    if (!king_attackers && !((0b1110ULL << color_to_position) & not_king) && (cur_history.castle & SQUARE_TO_BB[shift]) && !(chess_board.square_attacks(color_to_position + 2, TURN_TO_INDEX_LOOKUP[chess_board.turn]))) {
        moves.push_back((Move(3 + color_to_position, 1 + color_to_position, CASTLE, EMPTY, KINGSIDE_CASTLE)));
    }

    // queenside castle
    shift++; // because queenside castles are after in the mask
    
    if (!king_attackers && !((0b01110000ULL << color_to_position) & not_king) && (cur_history.castle & SQUARE_TO_BB[shift]) && !(chess_board.square_attacks(color_to_position + 4, TURN_TO_INDEX_LOOKUP[chess_board.turn]))) {
        moves.push_back((Move(3 + color_to_position, 5 + color_to_position, CASTLE, EMPTY, QUEENSIDE_CASTLE)));
    }
}

void generate_psuedo_legal_moves(Board &chess_board, std::vector<Move> &movelist, bool legal);
void filter_illegal_moves(Board &chess_board, std::vector<Move> &psuedo_legal, std::vector<Move> &legal);
void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) noexcept;
void generate_captures(Board &chess_board, std::vector<Move> &moves) noexcept;
BB generate_attacks(Board &chess_board) noexcept;
void set_attack_bitboard(Board &chess_board) noexcept;
BB get_and_set_pins(Board &chess_board) noexcept;
#endif // GENERATE_MOVES_HPP