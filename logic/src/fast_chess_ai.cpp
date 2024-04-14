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

int main() {
    initialize_engine();
    assert(ENGINE_INITIALIZED);
    // run_test_suite();

    // interface();
    Board b = Board(DEFAULT_FEN);
    std::cout << perft(b, 6, 6) << std::endl;

    return 0;
}