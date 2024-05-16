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
#include "tuner.hpp"

int main() {
    initialize_engine();
    assert(ENGINE_INITIALIZED);
    // run_test_suite();
    // Board b = Board("rnb1kbnr/ppppqppp/8/8/3p4/4K3/PPP1PPPP/RNBQ1BNR w kq - 0 4");
    Board b = Board(DEFAULT_FEN);
    std::cout << perft(b, 7, 7) << std::endl;
    // b.play_move(uci_to_move("g7g5", b));
    // print_BB(b.pieces[FULL]);
    // print_BB(b.pieces[FULL]);
    // printf("\n---------\n");
    // std::cout << perft(b, 1, 1) << std::endl;
    // train(200, 5e-3);
    // Interface interface = Interface();
    // interface.run();
    // Board b = Board("rnbqkbnr/ppp1pppp/8/3p4/8/5N2/PPPPPPPP/RNBQKB1R w KQkq - 0 2");
    // Searcher s = Searcher();
    // // std::cout << s.quiescence_search(b, NEGINF, INF) << std::endl;
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;

    // b.play_move(s.best_move);
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;

    // b.play_move(s.best_move);
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;

    // b.play_move(s.best_move);
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;

    // b.play_move(s.best_move);
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;

    return 0;
}