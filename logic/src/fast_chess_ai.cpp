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

/*
test 1 passed
test 2 passed
test 3 passed
test 4 passed
test 5 passed
test 6 passed
test 7 passed
test 8 failed; engine: 62381; correct answer: 62379; rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8; 3
test 9 passed
test 10 passed
test 11 passed
test 12 passed
test 13 passed
test 14 passed
test 15 failed; engine: 1274207; correct answer: 1274206; r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1; 4
test 16 passed
test 17 passed
test 18 failed; engine: 1005172; correct answer: 1004658; 8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1; 5
test 19 passed
test 20 passed
test 21 passed
test 22 passed
test 23 failed; engine: 23612; correct answer: 23527; 8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1; 4
*/
// f2d3 6
int main() {
    initialize_engine();
    assert(ENGINE_INITIALIZED);
    // Board b = Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    // b.print_square_data();
    run_test_suite();
    // Board b = Board("rnN2k1r/pp2bppp/2p5/8/2B5/8/PPP1NnPP/RNBqK2R w KQ - 0 9");
    // print_BB(generate_attacks(b));
    // std::vector<Move> l;
    // generate_legal_moves(b, l);
    // std::cout << l.size() << std::endl;
    // print_BB(get_sliding_moves(b.pieces[FULL], b.pieces[BLACK], BISHOP_MAGICS[61]));
    // std::cout << sizeof(History) << std::endl;
    // interface();

    return 0;
}