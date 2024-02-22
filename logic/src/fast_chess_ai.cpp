#include <iostream>

#include "constants.hpp"
#include "bit_operations.hpp"
#include "sliding_moves.hpp"
#include "non_sliding_moves.hpp"
#include "moveboards.hpp"
#include "magic_values.hpp"
#include "board.hpp"
#include "move.hpp"

int main() {
    // std::array<BB, 64> ROOK_MASKS, BISHOP_MASKS;
    // std::array<std::array<BB, 4096>, 64> ROOK_MOVES, BISHOP_MOVES;
    // init_moveboards(ROOK_MASKS, BISHOP_MASKS, ROOK_MOVES, BISHOP_MOVES);
    // Board chess = Board(DEFAULT_FEN);
    // print_BB(chess.pieces[KNIGHT] & chess.pieces[BLACK]);
    print_Move_bits(generate_move(63, 54, BISHOP, NORMAL_MOVE));
    
    // open_fen(starting_fen); // 51
    // print_BB(black | white);
    // print_BB(BISHOP_MASKS[51]);
    // print_BB(ROOK_MASKS[51]);
    // print_BB(BISHOP_MOVES[51][calculate_index((black | white) & BISHOP_MASKS[51], BISHOP_MAGICS[51], BISHOP_SHIFTS[51])]);
    // std::vector<BB> blocker_board = blocker_boards(blocker_mask_rook(23));
    // for (int i = 0; i < blocker_board.size(); i++) {
    //     print_BB(blocker_board[i]);
    // }
    // BB blockermask = blocker_mask_rook(23);
    // std::cout << "example blockermask" << '\n';
    // print_BB(blockermask);
    // std::cout << "example blockerboard" << '\n';
    // print_BB(blockerboards[45]);
    // std::cout << blockerboards.size() << '\n';
    // print_BB(moveboard_rook(blockerboards[45], 23));
    return 0;
}