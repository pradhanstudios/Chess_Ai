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
    best_eval(NEGINF),
    killer_moves_idx(0),
    search_over(false) {}
    // nodes(0ULL) {}

void Searcher::order_moves(std::vector<Move> &moves, const Board &chess_board) noexcept {
    for (int i = 0; i < moves.size(); i++) { // give each move a score
        moves[i].score = 
        (moves[i] == this->previous_best_move) * 1e7 + // pv node
        ((moves[i].type & PROMOTION) * (moves[i].type & 0b1110)) * 1e5 + // promotion
        (std::find(this->killers.begin(), this->killers.end(), moves[i]) != this->killers.end()) * 1e4 +
        ((SQUARE_TO_BB[moves[i].to] & chess_board.pieces[FULL]) * (no_color(chess_board.piece_data[moves[i].to]) - no_color(chess_board.piece_data[moves[i].from]))) * 1e3 // MVV-LVA
        ;
    }
    std::sort(moves.begin(), moves.end(), [](const Move &m1, const Move &m2) { return m1.score > m2.score; });
}

int Searcher::quiescence_search(Board &chess_board, int alpha, int beta) noexcept {
    this->search_over = current_time() > search_until;
    if (this->search_over) {
        return 0;
    }
    if (chess_board.state == CHECKMATE) {
        return NEGINF;
    }
    if (chess_board.state == DRAW) {
        return 0;
    }
    int eval = get_perspective_eval((evaluate(chess_board)), chess_board.turn);

    if (eval >= beta) {
        return beta;
    }

    if (alpha < eval) {
        alpha = eval;
    }
    std::vector<Move> moves;
    moves.reserve(MAX_CAPTURE_MOVES);
    generate_captures(chess_board, moves);

    for (const Move &move : moves) {
        chess_board.play_move(move);
        eval = -this->quiescence_search(chess_board, -beta, -alpha);
        chess_board.undo_move(move);

        if (eval >= beta) {
            return beta;
        }

        alpha = std::max(alpha, eval);
    }
    return alpha;
}

int Searcher::negamax_search(Board &chess_board, const int &depth, const int &depth_from_start, int alpha, int beta) noexcept {
    // nodes++;
    this->search_over = current_time() > search_until;
    if (this->search_over) {
        return 0;
    }
    std::vector<Move> moves;
    moves.reserve(MAX_LEGAL_MOVES);
    generate_legal_moves(chess_board, moves);
    // std::cout << "here" << std::endl;
    this->order_moves(moves, chess_board);
    
    if (chess_board.state == CHECKMATE) {
        // std::cout << "gothere" << std::endl;
        return (NEGINF + depth_from_start); // better to lose in the most amount of time
    }
    
    else if (chess_board.state == DRAW) {
        return 0;
    }
    
    else if (depth == 0) {
        return this->quiescence_search(chess_board, alpha, beta); // eval
    }

    int cur_eval = NEGINF;
    bool search_pv = true;

    for (const Move &move : moves) {
        chess_board.play_move(move);
        // cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, negbeta, negalpha);
        if (search_pv) {
            cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, -beta, -alpha);
        }
        else {
            cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, -alpha-1, -alpha);
            if (cur_eval > alpha && cur_eval < beta) {
                cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, -beta, -alpha);
            }
        }
        
        chess_board.undo_move(move);

        if (this->search_over) {
            return 0;
        }

        if ((cur_eval > best_eval || this->best_move == NULL_MOVE) && depth_from_start == 0) {
            this->best_eval = cur_eval;
            this->best_move = move;
        }

        if (cur_eval >= beta) {

            if (!chess_board.piece_data[move.to]) { // not capture
                this->killers[++this->killer_moves_idx] = move;
                this->killer_moves_idx &= 7;
            }

            return beta;
        }
        

        if (cur_eval > alpha) {
            alpha = cur_eval;
            search_pv = false;
        }

    }
    return alpha;
}

int Searcher::run_negamax_search(Board &chess_board, const int &depth, const int &depth_from_start, int alpha, int beta) noexcept {
    // this->nodes = 0ULL;
    this->best_move = NULL_MOVE;
    this->best_eval = NEGINF;
    // std::cout << "here" << std::endl;
    
    return this->negamax_search(chess_board, depth, depth_from_start, alpha, beta);
}

void Searcher::run_iterative_deepening(Board &chess_board, const int &time, const int &max_depth) noexcept {
    this->search_until = current_time() + time;
    this->best_move = NULL_MOVE;
    this->best_eval = NEGINF;
    int m_depth = 0;

    for (int i = 0; i <= max_depth; i++) {
        this->search_over = false;
        this->previous_best_move = this->best_move;
        this->previous_best_eval = this->best_eval;
        // aspiration windows
        // if (abs(this->previous_best_eval - this->run_negamax_search(chess_board, i, 0, this->previous_best_eval - 20, this->previous_best_eval + 20)) >= 20) {
        //     this->run_negamax_search(chess_board, i, 0, NEGINF, INF);
        // }
        // for some reason, aspiration windows seem to be making my program slower
        this->run_negamax_search(chess_board, i, 0, NEGINF, INF);
        m_depth = i;
        // TODO: add: if the searcher found a checkmate, the search stops
        if (search_over) {
            if (m_depth > 1) {
                this->best_move = this->previous_best_move;
                this->best_eval = this->previous_best_eval;
            }
            m_depth--;
            // we do not want moves from incomplete search
            break;
        }
    }
    std::cout << "ran to depth: " << m_depth << std::endl;
    // if (this->best_move == NULL_MOVE) {
    //     this->best_move = this->previous_best_move;
    // }
    if (this->best_move == NULL_MOVE) { // if the previous move was also null
        std::vector<Move> moves;
        moves.reserve(MAX_LEGAL_MOVES);
        generate_legal_moves(chess_board, moves);
        this->best_move = moves[0]; // pick a random move
    }
}