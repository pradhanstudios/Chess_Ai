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
#include "evaluation.hpp"

int main() {
    initialize_engine();
    assert(ENGINE_INITIALIZED);
    // run_test_suite();
    // interface();
    // h8h7
    // std::cout << perft(b, 6) << std::endl;
   /*
    105
    b1c3
    ./fast_chess_ai  6.44s user 0.00s system 99% cpu 6.459 total
   */
    Board b = Board(DEFAULT_FEN);
    Searcher s = Searcher();
    s.run_negamax_search(b, 7, 0, NEGINF, INF);
    std::cout << s.best_eval << std::endl;
    std::cout << move_to_uci(s.best_move) << std::endl;
    // std::cout << (multiply_based_on_color((PIECE_TABLES[no_color(BLACK | QUEEN)-1][orient(BLACK | QUEEN, 60)]), (b.pieces[60]))) << std::endl;
    // Searcher s = Searcher();
    // int eval = s.run_negamax_search(b, 5, 0, NEGINF, INF);
    // b.play_move(s.best_move);
    // std::cout << move_to_uci(s.best_move) << std::endl;
    // pretty_eval(s);
    // eval = s.run_negamax_search(b, 5, 0, NEGINF, INF);
    // // b.play_move(s.best_move);
    // std::cout << move_to_uci(s.best_move) << std::endl;
    // pretty_eval(s);
    // s.run_negamax_search(b, 5, 0, NEGINF, INF);
    // std::cout << move_to_uci(s.best_move) << std::endl;
    // b.play_move(s.best_move);
    // std::cout << s.nodes << std::endl;
    // Board b = Board("rnbqkbnr/ppppppp1/8/7p/5P2/P7/1PPPP1PP/RNBQKBNR w KQkq - 0 3");
    // b.play_move(uci_to_move("g1h3", b));
    // b.undo_move(uci_to_move("g1h3", b));
    // b.play_move(uci_to_move("g1h3", b));
    // b.print_square_data();
    // print_BB(b.pieces[FULL]);
    // print_BB(b.pieces[BLACK]);
    // print_BB(b.pieces[WHITE]);
    // print_BB(b.pieces[ROOK]);

    // std::cout << perft(b, 5, 5) << std::endl;

    return 0;
}