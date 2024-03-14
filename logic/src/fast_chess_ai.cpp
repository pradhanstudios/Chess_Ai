#include <iostream>

#include "constants.hpp"
#include "bit_operations.hpp"
#include "sliding_moves.hpp"
#include "non_sliding_moves.hpp"
#include "moveboards.hpp"
#include "magic_values.hpp"
#include "board.hpp"
#include "move.hpp"
#include "generate_moves.hpp"

int main() {
    // std::array<BB, 64> ROOK_MASKS, BISHOP_MASKS;
    // std::array<std::array<BB, 4096>, 64> ROOK_MOVES, BISHOP_MOVES;
    // init_moveboards(ROOK_MASKS, BISHOP_MASKS, ROOK_MOVES, BISHOP_MOVES);
    // // int i = 0;
    // int count = 0;
    // print_BB(ROOK_MOVES[0][90]);
    // std::cout << "Empty bb's: " << std::endl;
    // for (BB bitboard : ROOK_MOVES[2]) {
    //     if (!bitboard) {
    //         // if (i < 100) {
    //         // print_BB(bitboard);
    //         // std::cout << i << ", ";
    //         // }
    //         count++;
    //     }
    //     i++;
    // }
    // std::cout << std::endl << "Count: " << count << "/" << i << std::endl;

    // BB rook_blck = 0b110ULL;

    // BB newbb = moveboard_rook(rook_blck, 0);

    // print_BB(newbb);

    // generate_magics_and_save();

    // print_BB(ROOK_MOVES[0][26]);
    // std::vector<Move> moves;
    // moves.reserve(MAX_LEGAL_MOVES);
    // std::array<magic_structure, 64> t;
    // Board chess_board = Board("r4rk1/p3bp1p/1qb1pnp1/1p1pB3/3P4/2PB1N1P/PPQ2PP1/R4RK1 w - - 1 15");
    // std::cout << chess_board.piece_data[11];
    // std::cout << "\n" << chess_board.piece_data[63] << std::endl;
    load_magics();
    std::cout << ROOK_MAGICS[20].magic << std::endl;
    // chess_board.print_square_data();
    // generate_legal_moves(chess_board, moves);
    // for (Move m : moves) {
        // print_move_fancy(m);
    // }
    // print_BB(blocker_mask_bishop(0));
    // generate_magics_and_save();
    // extern std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
    // std::cout << ROOK_MAGICS[0].magic << std::endl;
    // int *temp;
    // std::cout << "ok" << std::endl;
    // int temp2 = temp[3];
    // std::cout << (7 & temp[2]) << std::endl;
    // chess.print_square_data();
    // std::cout << "here";
    // t = ROOK_MAGICS;
    // std::cout << chess.pieces[WHITE] << std::endl;
    // generate_legal_moves(chess_board, ROOK_MAGICS, BISHOP_MAGICS, moves);
    
    // std::cout << moves.size() << std::endl;
    // std::cout << ROOK_MAGICS[0].magic;
    // print_BB(chess_board.pieces[FULL]);
    // print_BB(get_sliding_moves(chess_board.pieces[FULL], chess_board.pieces[WHITE], 20, BISHOP_MAGICS[20]));
    // Searcher s;
    // s.ROOK_MAGICS = ROOK_MAGICS;
    // s.BISHOP_MAGICS = BISHOP_MAGICS;
    // s->ROOK_MAGICS = &ROOK_MAGICS;
    // s->BISHOP_MAGICS = &BISHOP_MAGICS;

    // s.generate_legal_moves(chess_board, moves);
    return 0;
}