#ifndef GENERATE_MOVES_HPP
#define GENERATE_MOVES_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "non_sliding_moves.hpp"
#include "sliding_moves.hpp"
#include "move.hpp"
#include "board.hpp"
#include "magic_values.hpp"

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

void generate_psuedo_legal_moves(Board &chess_board, std::vector<Move> &movelist);
void generate_legal_moves(Board &chess_board, std::vector<Move> &moves) noexcept;
void generate_captures(Board &chess_board, std::vector<Move> &moves) noexcept;
BB generate_attacks(Board &chess_board) noexcept;
void set_attack_bitboard(Board &chess_board) noexcept;
BB get_and_set_pins(Board &chess_board) noexcept;
#endif // GENERATE_MOVES_HPP