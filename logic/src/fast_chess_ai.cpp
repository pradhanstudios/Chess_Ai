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
    // Board b = Board("rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1");
    Board b = Board("8/7k/5K2/4Q3/8/8/8/8 w - - 0 1");

    // h8h7
    // interface();
    // print_BB(SQUARE_TO_BB[h8]);
    // Board b = Board("r3k2r/pppp2pp/4p3/5pP1/8/8/PPPPPP1P/R3K2R w KQkq f6 0 1");
    // Move m1 = Move(h2, h4, NORMAL_MOVE);
    // Board b = Board("r3k2r/pppp2pp/4p3/5pP1/8/8/PPPPPP1P/R3K2R w KQkq f6 0 1");
    // BB orig = b.zobrist_key;
    // BB expected = b.zobrist_key ^ zobrist.pieces[PAWN-1][h2] ^ zobrist.pieces[PAWN-1][h2];
    // print_BB(expected);
    // b.play_move(m1);
    // print_BB(b.zobrist_key);
    // std::cout << (b.zobrist_key == expected) << std::endl;
    // b.undo_move(m1);
    // std::cout << (b.zobrist_key == orig) << std::endl;
    Searcher s = Searcher();
    s.run_iterative_deepening(b, 10000, 6);
    // print_BB(b.zobrist_key);
    // s.run_iterative_deepening(b, 2000);
    std::cout << move_to_uci(s.best_move) << " " << s.best_eval << " " << s.nodes << std::endl;
    // std::cout << s.quiescence_search(b, NEGINF, INF) << std::endl;
    // std::vector<Move> moves;
    // moves.reserve(MAX_CAPTURE_MOVES);
    // // generate_captures(b, moves);

    // for (const Move &m : moves) {
    //     std::cout << move_to_uci(m) << " ";
    // }
    // std::cout << std::endl;
    // s.order_moves(moves, b);
    // for (const Move &m : moves) {
    //     std::cout << move_to_uci(m) << " ";
    // }
    // std::cout << std::endl;
    // s.run_iterative_deepening(b, 2500);

    // // s.run_iterative_deepening(b, 1000); // it doesnt work when set to 2000
    // std::cout << s.best_eval << std::endl;
    // std::cout << move_to_uci(s.best_move) << std::endl;
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