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
    // Board b = Board(TEST_SUITE_FENS[9]);
    // std::cout << TEST_SUITE_FENS[9] << std::endl;
    // std::cout << TEST_SUITE_DEPTHS[9] << std::endl;
    // Board b = Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    // b.play_move(Move(d7, c8, PROMOTION, QUEEN));

    // Board b2 = Board("rnQq1k1r/pp2bppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R b KQ - 0 8");
    // print_BB(b.pieces[FULL]);
    // print_BB(b2.pieces[FULL]);

    // std::cout << (b.pieces[FULL] == b2.pieces[FULL]) << std::endl;
    // std::cout << (b.pieces[EMPTY] == b2.pieces[EMPTY]) << std::endl;
    // std::cout << (b.pieces == b2.pieces) << std::endl;
    // int depth = TEST_SUITE_DEPTHS[9]
    // ;
    // std::cout << perft(b, 4, 4) << std::endl;
    // std::cout << TEST_SUITE_NODE_COUNT[9] << std::endl;
    // ---------------------
    return 0;
}