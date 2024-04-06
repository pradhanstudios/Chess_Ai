#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "generate_moves.hpp"
#include "uci.hpp"

// https://gist.githubusercontent.com/peterellisjones/8c46c28141c162d1d8a0f0badbc9cff9/raw/b11af5a3dd978724ac2d4a531217133b9a3ad9ba/Chess%2520Perft%2520test%2520positions
const inline std::array<std::string, 23> TEST_SUITE_FENS = {"r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2", "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3", "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2", "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2", "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2", "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9", "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", "5k2/8/8/8/8/8/8/4K2R w K - 0 1", "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", "4k3/1P6/8/8/8/8/K7/8 w - - 0 1", "8/P1k5/K7/8/8/8/8/8 w - - 0 1", "K1k5/8/P7/8/8/8/8/8 w - - 0 1", "8/k1P5/8/1K6/8/8/8/8 w - - 0 1", "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"};
const inline std::array<int, 23> TEST_SUITE_DEPTHS = {1, 1, 1, 1, 1, 1, 1, 3, 3, 6, 6, 6, 6, 6, 4, 4, 6, 5, 6, 6, 6, 7, 4};
const inline std::array<int, 23> TEST_SUITE_NODE_COUNT = {8, 8, 19, 5, 44, 39, 9, 62379, 89890, 1134888, 1015133, 1440467, 661072, 803711, 1274206, 1720476, 3821001, 1004658, 217342, 92683, 2217, 567584, 23527};

void run_test_suite();

uint64_t perft(Board &chess_board, const int &depth);
uint64_t perft(Board &chess_board, const int &depth, const int &original_depth);

// https://www.chessprogramming.org/Perft_Results

/*
stockfish:
a2a3: 380
b2b3: 420
c2c3: 420
d2d3: 539
e2e3: 599
f2f3: 380
g2g3: 420
h2h3: 380
a2a4: 420
b2b4: 421
c2c4: 441
d2d4: 560
e2e4: 600
f2f4: 401
g2g4: 421
h2h4: 420
b1a3: 400
b1c3: 440
g1f3: 440
g1h3: 400

mine: (not matching)
a2a3 323
a2a4 320
b2b3 447
b2b4 425
c2c3 480
c2c4 357
d2d3 519
d2d4 472
e2e3 598
e2e4 465
f2f3 924
f2f4 957
g2g3 932
g2g4 779
h2h3 989
h2h4 855
b1a3 1021
b1c3 860
g1f3 831
g1h3 863

*/
#endif // SEARCH_HPP