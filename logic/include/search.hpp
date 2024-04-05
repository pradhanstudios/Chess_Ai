#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "generate_moves.hpp"
#include "uci.hpp"

uint64_t perft(Board &chess_board, const int depth);
uint64_t perft(Board &chess_board, const int depth, const int original_depth);

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