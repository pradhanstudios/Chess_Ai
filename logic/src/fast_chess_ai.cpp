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
    Board b = Board("7k/5K2/8/8/8/4R3/8/8 b - - 0 1");
    Searcher s = Searcher();
    s.run_negamax_search(b, 3);
    std::cout << move_to_uci(s.best_move) << std::endl;
    b.play_move(s.best_move);
    s.run_negamax_search(b, 3);
    std::cout << move_to_uci(s.best_move) << std::endl;
    // Board b = Board("rnbqkbnr/ppppppp1/8/7p/5P2/P7/1PPPP1PP/RNBQKBNR w KQkq - 0 3");
    // b.play_move(uci_to_move("g1h3", b));
    // b.undo_move(uci_to_move("g1h3", b));
    // b.play_move(uci_to_move("g1h3", b));
    // b.print_square_data();
    // print_BB(b.pieces[FULL]);
    // print_BB(b.pieces[BLACK]);
    // print_BB(b.pieces[WHITE]);
    // print_BB(b.pieces[ROOK]);

    // std::cout << perft(b, 6, 6) << std::endl;

    return 0;
}