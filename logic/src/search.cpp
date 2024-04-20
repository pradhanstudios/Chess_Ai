#include "search.hpp"

void run_test_suite() noexcept {
    Board b = Board(TEST_SUITE_FENS[0]);
    // std::cout << TEST_SUITE_FENS[0] << std::endl;
    // print_BB(b.pieces[FULL]);
    uint64_t nodes = perft(b, TEST_SUITE_DEPTHS[0]);
    if (nodes == TEST_SUITE_NODE_COUNT[0]) {
        std::cout << "test 1 passed" << std::endl;
    }
    else {
        std::cout << "test 1 failed; engine: " << nodes << "; correct answer: " << TEST_SUITE_NODE_COUNT[0] << "; " << TEST_SUITE_FENS[0] << "; " << TEST_SUITE_DEPTHS[0] << std::endl; 
    }
    for (int i = 1; i < 23; i++) {
        b = Board(TEST_SUITE_FENS[i]);
        nodes = perft(b, TEST_SUITE_DEPTHS[i]);
        if (nodes == TEST_SUITE_NODE_COUNT[i]) {
            std::cout << "test " << (i+1) << " passed" << std::endl;
        }
        else {
            std::cout << "test " << (i+1) << " failed; engine: " << nodes << "; correct answer: " << TEST_SUITE_NODE_COUNT[i] << "; " << TEST_SUITE_FENS[i] << "; " << TEST_SUITE_DEPTHS[i] << std::endl; 
        }
    }
}

uint64_t perft(Board &chess_board, const int &depth) noexcept {
    if (depth == 0) {
        return 1ULL;
    }

    std::vector<Move> moves;
    moves.reserve(MAX_LEGAL_MOVES);
    uint64_t nodes = 0ULL;

    generate_legal_moves(chess_board, moves);
    for (const Move &move : moves) {
        chess_board.play_move(move);
        nodes += perft(chess_board, depth - 1);
        chess_board.undo_move(move);
    }
    return nodes;
}

uint64_t perft(Board &chess_board, const int &depth, const int &original_depth) noexcept {
    // std::cout << "depth " << depth << std::endl;
    // chess_board.print_square_data();
    if (depth == 0) {
        return 1ULL;
    }
    
    std::vector<Move> moves;
    moves.reserve(MAX_LEGAL_MOVES);
    uint64_t cur_node_count = 0ULL;
    uint64_t nodes = 0ULL;

    generate_legal_moves(chess_board, moves);
    for (const Move &move : moves) {
        chess_board.play_move(move);
        cur_node_count = perft(chess_board, depth - 1, depth);
        nodes += cur_node_count;
        if (depth == original_depth) {
            std::cout << move_to_uci(move) << ": " << cur_node_count << std::endl;
        }
        chess_board.undo_move(move);
        
        // break;
    }
    return nodes;
}

Searcher::Searcher() :
    best_move(NULL_MOVE), 
    best_eval(NEGINF) {}
 
int Searcher::negamax_search(Board &chess_board, const int &depth, const int &depth_from_start, int alpha, int beta) noexcept {
    if (depth == 0) {
        return simple_eval(chess_board); // eval
    }

    else if (chess_board.state == DRAW) {
        return 0;
    }

    else if (chess_board.state == CHECKMATE){
        return NEGINF + depth_from_start; // better to lose in the most amount of time
    }

    std::vector<Move> moves;
    moves.reserve(MAX_LEGAL_MOVES);
    int cur_eval = NEGINF;

    generate_legal_moves(chess_board, moves);
    for (const Move &move : moves) {
        chess_board.play_move(move);
        cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, -beta, -alpha);
        alpha = std::max(cur_eval, alpha);
        chess_board.undo_move(move);
        if (cur_eval >= this->best_eval && depth_from_start == 0) {
            this->best_move = move;
            this->best_eval = cur_eval;
        }
        if (beta <= alpha) {
            return beta;
        }
    }
    return alpha;
}

int Searcher::run_negamax_search(Board &chess_board, const int &depth, const int &depth_from_start, const int &alpha, const int &beta) noexcept  {
    this->best_move = NULL_MOVE;
    this->best_eval = NEGINF;
    
    return this->negamax_search(chess_board, depth, depth_from_start, alpha, beta);
}