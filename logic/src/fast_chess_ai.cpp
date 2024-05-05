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


    // interface();
    Board b = Board("r2qkbnr/ppp2ppp/2n1b3/1B2p3/4p3/5N2/PPPPQPPP/RNB1K1R1 w Qkq - 0 6");
    Searcher s = Searcher();
    // std::cout << s.quiescence_search(b, NEGINF, INF) << std::endl;
    s.run_iterative_deepening(b, 3000);
    std::cout <<  s.best_eval << " " << move_to_uci(s.best_move) << std::endl;

    return 0;
}