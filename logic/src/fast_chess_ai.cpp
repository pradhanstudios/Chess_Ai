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
#include "search.hpp"
#include "initialize.hpp"
#include "interface.hpp"

int main() { // http://www.rocechess.ch/perft.html n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1 
    // 8/8/6p1/K4pPr/8/8/8/8 w - f5 0 1
    // 1k1R4/1pp5/p7/8/8/8/8/8 w - - 0 1
    initialize_engine();
    if (!ENGINE_INITIALIZED) {
        std::cerr << "ENGINE HAS NOT BEEN INITIALIZED, THEREFORE IT WILL NOT WORK PROPERLY" << std::endl;
        std::cerr << "TO INITIALIZE, AT THE START OF MAIN(), RUN 'initialize_engine();'" << std::endl;
        return 1;
    }
    // 8/8/7p/6pP/8/8/5kr1/7K w - g5 0 1
    // print_BB(MAX_VALUE);
    // print_BB(SQUARE_TO_BB[30]);
    // 
    // rnbqkbnr/ppppppp1/8/7p/8/7P/PPPPPPP1/RNBQKBNR w KQkq - 0 2
    // r3k2r/p1ppqpb1/bn2pQp1/3PN3/1p2P3/2N4p/PPPBBPPP/R3K2R b KQkq - 0 1
    // rnbqkbnr/pppp1pp1/8/4p2p/P7/8/1PPPPPPP/RNBQKBNR w KQkq - 0 3
    // std::cout << "here" << std::endl;
    interface();
    // std::cout << sizeof(Move) << std::endl;
    // Board b = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"); //
    // // Move m1 = uci_to_move("e1c1", b);
    // // b.play_move(m1);
    // // b.undo_move(m1);
    // // print_BB(b.pieces[FULL]);
    // std::cout << perft(b, 3, 3) << std::endl;
    // b.next_turn();
    // set_attack_bitboard(b);
    // BB pins = get_and_set_pins(b);
    // b.next_turn();
    // test(b, pins);
    
    // print_BB(b.pieces[PAWN]);
    // print_BB(b.pieces[FULL]);
    // std::cout << perft(b, 6);
    // b.print_square_data();
    // print_BB(b.pieces[FULL]);
    // b.undo_move(m1);
    // b.print_square_data();
    // print_BB(b.pieces[PAWN]);
    // print_BB(pawn_moves(b.pieces[FULL] & SQUARE_TO_BB[8], b.pieces[BLACK], b.pieces[EMPTY], 8));
    
    // std::cout << b.state << std::endl;
    // BB pawns_to_move = b.pieces[WHITE] & b.pieces[PAWN];
    // print_BB(shift_back(pawns_to_move & ~H_FILE, 8-1));
    // b.next_turn();
    // print_BB(get_and_set_pins(b));
    // b.next_turn();
    // std::cout << perft(b, 3, 3) << std::endl;
    // generate_legal_moves(b, moves);
    // for (Move move : moves) {
    //     std::cout << move_to_uci(move) << std::endl;
    // }
    // std::cout << moves.size() << std::endl;

    // std::cout << b.is_double_check << std::endl;
    // b.play_move(uci_to_move("h2h4", b));
    // b.play_move(uci_to_move("g7g5", b));
    // print_move_fancy(uci_to_move("h2h4", b));
    // b.print_square_data();
    // std::cout << perft(b, 4, 4) << std::endl;
    // print_BB(b.pieces[PAWN]);
    // Move m = uci_to_move("g2h1r", b);
    // Move m2 = Move(9, 0, PROMOTION, ROOK);
    // // std::cout << (m == m2) << std::endl;
    // b.play_move(m);
    // b.undo_move(m);
    // print_BB(b.pieces[KNIGHT] & b.pieces[WHITE]);
    return 0;
}