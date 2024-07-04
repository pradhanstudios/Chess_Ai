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
    run_test_suite();
    // Board b = Board("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2");
    // print_BB(pawn_non_capture_moves(c7, b.pieces[EMPTY], b.turn));
    // std::cout << TEST_SUITE_FENS[9] << std::endl;
    // std::cout << TEST_SUITE_DEPTHS[9] << std::endl;
    // b.play_move(Move(d7, c8, PROMOTION, QUEEN));

    // Board b2 = Board("rnQq1k1r/pp2bppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R b KQ - 0 8");
    // print_BB(b.pieces[FULL]);
    // print_BB(b2.pieces[FULL]);

    // std::cout << (b.pieces[FULL] == b2.pieces[FULL]) << std::endl;
    // std::cout << (b.pieces[EMPTY] == b2.pieces[EMPTY]) << std::endl;
    // std::cout << (b.pieces == b2.pieces) << std::endl;
    // int depth = TEST_SUITE_DEPTHS[9]
    // ;
    // Board b = Board(DEFAULT_FEN);
    // std::cout << perft(b, 1, 1) << std::endl;
    // std::cout << TEST_SUITE_NODE_COUNT[9] << std::endl;
    // ---------------------
    return 0;
}