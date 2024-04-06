#include "search.hpp"

uint64_t perft(Board &chess_board, const int &depth) {
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

uint64_t perft(Board &chess_board, const int &depth, const int &original_depth) {
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