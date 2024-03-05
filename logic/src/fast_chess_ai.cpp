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



    // print_BB(ROOK_MOVES[0][26]);
    std::vector<Move> legal_moves;
    legal_moves.reserve(MAX_LEGAL_MOVES);
    Board chess = Board("r4rk1/p3bp1p/1qb1pnp1/1p1pB3/3P4/2PB1N1P/PPQ2PP1/R4RK1 w - - 1 15");
    std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
    load_magics(ROOK_MAGICS, BISHOP_MAGICS);
    // int *temp;
    // std::cout << "ok" << std::endl;
    // int temp2 = temp[3];
    // std::cout << (7 & temp[2]) << std::endl;
    // chess.print_square_data();
    // std::cout << "here";
    std::cout << chess.pieces[WHITE] << std::endl;
    generate_legal_moves(chess, ROOK_MAGICS, BISHOP_MAGICS, legal_moves);
    // std::cout << legal_moves.size() << std::endl;
    // std::cout << ROOK_MAGICS[0].magic;
    // print_BB(get_sliding_moves(chess.pieces[FULL], chess.pieces[WHITE], 20, BISHOP_MAGICS[20]));
    // chess.print_square_data();
    // print_BB();
    // generate_legal_moves(chess, legal_moves);
    // for (Move m : legal_moves) {
    //     print_move_fancy(m);
    // }
    // BB pos = 0b01000010ULL;
    // std::cout << pop_first_one(pos) << " " << pop_first_one(pos) << std::endl;
    // print_BB(chess.pieces[KNIGHT] & chess.pieces[BLACK]);
    // std::cout << chess.turn << std::endl;
    // Move move01 = generate_move(8, 24, CASTLE, EMPTY, 0);
    // Move move01 = generate_move(8, 24, NORMAL_MOVE);
    // Move move02 = generate_move(55, 47, NORMAL_MOVE);
    // Move move03 = generate_move(24, 32, NORMAL_MOVE);
    // Move move04 = generate_move(49, 33, NORMAL_MOVE);
    // Move move05 = generate_move(32, 41, EN_PESSANT);
    // Move move06 = generate_move(47, 39, NORMAL_MOVE);
    // Move move07 = generate_move(41, 49, NORMAL_MOVE);
    // Move move08 = generate_move(39, 31, NORMAL_MOVE);
    // Move move09 = generate_move(49, 56, PROMOTION, QUEEN); 
    // Move move10 = generate_move(31, 23, NORMAL_MOVE);
    // Move move11 = generate_move(11, 19, NORMAL_MOVE);
    // Move move12 = generate_move(54, 46, NORMAL_MOVE);
    // Move move13 = generate_move(3, 12, NORMAL_MOVE);
    // Move move14 = generate_move(46, 38, NORMAL_MOVE);
    // Move move15 = generate_move(1, 18, NORMAL_MOVE);
    // Move move16 = generate_move(38, 30, NORMAL_MOVE);
    // Move move17 = generate_move(4, 1, CASTLE, EMPTY, 0);
    // 0 is kingside 1 is queenside
    // init_moveboards();
    // chess.play_move(move01);
    // chess.play_move(move02);
    // chess.play_move(move03);
    // chess.play_move(move04);
    // // std::cout << abs(33-49) << "should bve 16" << std::endl;
    // // std::cout << chess.enpessents.back() << std::endl;
    // // print_vector(chess.enpessents);
    // chess.play_move(move05);
    // chess.play_move(move06);
    // chess.play_move(move07);
    // chess.play_move(move08);
    // chess.play_move(move09);
    // chess.play_move(move09); // PROMO
    // chess.play_move(move10); // waste move
    // chess.play_move(move11); // move knight
    // chess.play_move(move12); // waste move
    // chess.play_move(move13); // move pawn
    // chess.play_move(move14); // waste move
    // chess.play_move(move15); // move bishop
    // chess.play_move(move16); // waste move
    // chess.play_move(move17); // castle
    // chess.print_square_data();

    // print_BB(chess.pieces[WHITE] | chess.pieces[BLACK]);
    
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