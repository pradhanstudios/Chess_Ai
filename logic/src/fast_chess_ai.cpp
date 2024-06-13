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
    // Interface uci = Interface();
    // uci.run();
    // print_BB(PIECE_SQUARE_TABLES[320 + 0]);
    // printf("eg %i mg %i\n", (PIECE_SQUARE_TABLES[320 + 0] & 0xFFFF) ^ 0xFFFF, PIECE_SQUARE_TABLES[320 + 0] >> 16);
    // printf("%i\n", int16_t(-65));
    // print_BB(0x8000);
    // run_test_suite();
    Board b = Board("r2q1rk1/3n1p2/2pp3p/1pb1p1p1/p3P3/P1NP1N1P/RPP2PP1/5QK1 b - - 0 2");
    Interface i = Interface();
    i.run();
    // Board b = Board(DEFAULT_FEN);
    // printf("%i\n", int(-5 << 16));
    // printf("%i\n", int(-5 * 0x10000));
    // b.print_square_data();
    // std::cout << evaluate(b) << std::endl;
    // Searcher s = Searcher();
    // std::cout << evaluate(b) << std::endl;
    // std::cout << s.quiescence_search(b, NEGINF, INF, false) << std::endl;
    // s.run_iterative_deepening(b, 3000, 7);
    // s.search_until = 4000 + current_time();
    // s.run_negamax_search(b, 6, 0, NEGINF, INF);
    // s.run_iterative_deepening(b, 3000);
    // b.print_square_data();
    // std::vector<Move> moves;
    // moves.reserve(MAX_LEGAL_MOVES);
    // generate_captures(b, moves);
    // std::cout << perft(b, 6, 6) << std::endl;
    // std::cout <<  get_perspective_eval(s.best_eval, b.turn) << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;
    // std::cout << perft(b, 6, 6) << std::endl;
    // b.play_move(s.best_move);
    // s.run_iterative_deepening(b, 3000);
    // std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << " " << s.nodes << std::endl;
    return 0;
}