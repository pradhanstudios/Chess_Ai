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
        // b.change_fen(TEST_SUITE_FENS[i]);
        b = Board(TEST_SUITE_FENS[i]);
        print_BB(b.state);
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
        if (depth != 1) {
        chess_board.play_move(move);
        nodes += perft(chess_board, depth - 1);
        chess_board.undo_move(move);
        }
        else nodes++;
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
        if (depth != 1 || depth == original_depth) {
            chess_board.play_move(move);
            cur_node_count = perft(chess_board, depth - 1, depth);
            nodes += cur_node_count;
            if (depth == original_depth) {
                std::cout << move_to_uci(move) << ": " << cur_node_count << std::endl;
            }
            chess_board.undo_move(move);
        }
        else nodes++;
        
        // break;
    }
    return nodes;
}

Searcher::Searcher() :
    best_move(NULL_MOVE), 
    best_eval(NEGINF),
    killer_moves_idx(0),
    search_over(false),
    nodes(0ULL) {}

void Searcher::order_moves(std::vector<Move> &moves, const Board &chess_board) noexcept {
    for (int i = 0; i < moves.size(); i++) { // give each move a score
        moves[i].score = 
        (moves[i] == this->previous_best_move) * 17000 + // pv node
        (std::find(this->killers.begin(), this->killers.end(), moves[i]) != this->killers.end()) * 5500 + // killers
        ((moves[i].type & PROMOTION) * (moves[i].type & 0b1110)) * 1000 + // promotion (more score based on piece values)
        (bool(SQUARE_TO_BB[moves[i].to] & chess_board.pieces[FULL]) * (piece_values_mg[no_color(chess_board.piece_data[moves[i].to])] - piece_values_mg[no_color(chess_board.piece_data[moves[i].from])])) // MVV-LVA
        ;
        // moves[i].score = 1;
        // std::cout << moves[i].score << " ";
        // std::cout <<  ((bool(SQUARE_TO_BB[moves[i].to] & chess_board.pieces[FULL])) * (piece_values[no_color(chess_board.piece_data[moves[i].to])] - piece_values[no_color(chess_board.piece_data[moves[i].from])]))
        // << " ";
        // std::cout << ((moves[i] == this->previous_best_move) * 20000 + // pv node
        // (std::find(this->killers.begin(), this->killers.end(), moves[i]) != this->killers.end()) * 2000 + // killers
        // ((moves[i].type & PROMOTION) * (moves[i].type & 0b1110)) * 1000 + // promotion (more score based on piece values)
        // ((SQUARE_TO_BB[moves[i].to] & chess_board.pieces[FULL]) * (piece_values[no_color(chess_board.piece_data[moves[i].to])] - piece_values[no_color(chess_board.piece_data[moves[i].from])])) // MVV-LVA
        // )<< " ";
    }
    // std::cout << std::endl;
    // std::sort(moves.begin(), moves.end(), [](const Move &m1, const Move &m2) { return m1.score > m2.score; });

    // std::sort is slow on small vectors (I tried it)
    // https://www.geeksforgeeks.org/cpp-program-for-insertion-sort/
    // insertion sort
    int i, j;
    Move key;
    for (i = 1; i < moves.size(); i++) {
        key = moves[i];
        j = i - 1;

        while (j >= 0 && moves[j].score > key.score) {
            moves[j + 1] = moves[j];
            j = j - 1;
        }
        moves[j + 1] = key;
    }
    std::reverse(moves.begin(), moves.end());
}

void Searcher::order_captures(std::vector<Move> &moves, const Board &chess_board) noexcept {
    for (int i = 0; i < moves.size(); i++) { // give each move a score
        moves[i].score = 
        ((SQUARE_TO_BB[moves[i].to] & chess_board.pieces[FULL]) * (no_color(chess_board.piece_data[moves[i].to]) - no_color(chess_board.piece_data[moves[i].from]))) // MVV-LVA
        ;
    }
    int i, j;
    Move key;
    for (i = 1; i < moves.size(); i++) {
        key = moves[i];
        j = i - 1;

        while (j >= 0 && moves[j].score > key.score) {
            moves[j + 1] = moves[j];
            j = j - 1;
        }
        moves[j + 1] = key;
    }
    std::reverse(moves.begin(), moves.end());
}

int Searcher::quiescence_search(Board &chess_board, int alpha, int beta, const bool &promotion) noexcept {
    nodes++;
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
    // delta pruning
    int delta = 900; // queen value
    if (promotion) {
        delta += 800;
    }

    if (eval < alpha - delta) {
        return alpha;
    }

    if (alpha < eval) {
        alpha = eval;
    }
    std::vector<Move> moves;
    moves.reserve(MAX_CAPTURE_MOVES);
    generate_captures(chess_board, moves);
    this->order_captures(moves, chess_board);
    // this->order_moves(moves, chess_board);

    for (const Move &move : moves) {
        chess_board.play_move(move);
        eval = -this->quiescence_search(chess_board, -beta, -alpha, move.type & PROMOTION);
        chess_board.undo_move(move);

        if (eval >= beta) {
            return beta;
        }

        alpha = std::max(alpha, eval);
    }
    return alpha;
}

int Searcher::negamax_search(Board &chess_board, int depth, const int &depth_from_start, int alpha, int beta, const bool &is_pvs_node) noexcept {
    nodes++;
    // TODO: if move is a check, add an extension

    alpha = std::max(alpha, NEGINF + depth_from_start);
    beta = std::min(beta, INF - depth_from_start);

    if (alpha >= beta) {
        return alpha;
    }

    this->search_over = current_time() > search_until;
    if (this->search_over) {
        return 0;
    }

    if (depth <= 0) {
        return this->quiescence_search(chess_board, alpha, beta); // eval
    }
    std::vector<Move> moves;
    moves.reserve(MAX_LEGAL_MOVES);
    generate_legal_moves(chess_board, moves);
    // std::cout << "here" << std::endl;
    
    if (chess_board.state == CHECKMATE) {
        // std::cout << "gothere" << std::endl;
        return (NEGINF + depth_from_start);
    }
    
    if (chess_board.state == DRAW) {
        return 0;
    }

    this->order_moves(moves, chess_board);
    
    bool not_move_is_capture, lmr = false;
    bool is_check = (chess_board.pieces[turn_to_index[chess_board.turn]] & chess_board.pieces[KING]) & chess_board.pieces[OTHER_TEAM_ATTACKS];
    int cur_eval = NEGINF;
    int R = 0;

    // internal iterative reductions
    depth -= moves[0].score < 17000 && !is_pvs_node; // if the pv is not there, then reduce the depth by one 

    if (!is_check) {
        // reverse futility pruning
        int eval = evaluate(chess_board);
        // move is too good
        if (depth < 6 && (eval - (100 * depth)) >= beta) {
            return eval;
        }
        // null move pruning
        if (!is_pvs_node && eval >= beta && beta > BLACK_MATE_SCORE /*don't accidently prune checkmate*/ && depth > 1) {
            chess_board.next_turn(); // null move

            cur_eval = -this->negamax_search(chess_board, depth - 4, depth_from_start + 1, -beta, -alpha, is_pvs_node);

            chess_board.next_turn(); // undo null move

            if (cur_eval >= beta) {
                return cur_eval;
            }
        }
        // extended futility pruning does nothing in this engine

        // move is really bad (extended futility pruning)
        // extended_futility_pruning_allowed = depth < 5 && (eval + (depth * 106)) <= alpha;    
        // extended_futility_pruning_allowed = false;
    }

    bool search_pv = true;

    for (int i = 0; i < moves.size(); i++) {
        not_move_is_capture = chess_board.piece_data[moves[i].to] == 0;
        chess_board.play_move(moves[i]);
        // cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, negbeta, negalpha);
        if (search_pv) {
            cur_eval = -this->negamax_search(chess_board, depth - 1, depth_from_start + 1, -beta, -alpha, true);
        }
        else {
            cur_eval = -this->negamax_search(chess_board, depth - R - 1, depth_from_start + 1, -alpha-1, -alpha, is_pvs_node);
            if (cur_eval > alpha && cur_eval < beta) {
                // late move reductions
                lmr = depth >= 3 && moves[i].type == NORMAL_MOVE && not_move_is_capture && i > 4;
                if (lmr) {
                    cur_eval = -this->negamax_search(chess_board, depth - 2, depth_from_start + 1, -beta, -alpha, is_pvs_node);
                }
                if (!lmr || (cur_eval > alpha)) {
                    cur_eval = -this->negamax_search(chess_board, depth - R - 1, depth_from_start + 1, -beta, -alpha, is_pvs_node);
                }
            }
        }
        
        chess_board.undo_move(moves[i]);

        if (this->search_over) {
            return 0;
        }

        if ((cur_eval > best_eval || this->best_move == NULL_MOVE) && depth_from_start == 0) {
            // std::cout << "got here " << move_to_uci(moves[i]) << " " << cur_eval << " " << depth << " Move ordering: ";
            // for (const Move &move : moves) {
            //     std::cout << move_to_uci(move) << ", ";
            // }
            // std::cout << " || " << move_to_uci(this->previous_best_move) << std::endl;
            this->best_eval = cur_eval;
            this->best_move = moves[i];
        }

        if (cur_eval >= beta) {

            if (!chess_board.piece_data[moves[i].to]) { // not capture
                this->killers[++this->killer_moves_idx] = moves[i];
                this->killer_moves_idx &= 1;
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
    nodes = 0ULL;
    this->search_until = current_time() + time;
    this->best_move = NULL_MOVE;
    this->best_eval = NEGINF;
    int m_depth = 0;
    int soft_time_limit = current_time() + time / 11 /*around how much longer each depth takes*/;
    // const int aspiration_window = 30;
    // int aspiration_score;

    for (int i = 1; i <= max_depth; i++) {
        if (current_time() > soft_time_limit && i > 1) { // soft time limit reached
            break;
        }
        this->search_over = false;
        this->previous_best_move = this->best_move;
        this->previous_best_eval = this->best_eval;
        // aspiration windows
        // TODO: test this since aspiration windows are very inconsitent
        // if (i > 5) {
            // aspiration_score = this->run_negamax_search(chess_board, i, 0, this->previous_best_eval - aspiration_window, this->previous_best_eval + aspiration_window);
            // if (abs(this->previous_best_eval - aspiration_score) >= aspiration_window) {
                // this->run_negamax_search(chess_board, i, 0, NEGINF, INF);
            // }
        // }
        // else {
        // aspiration windows do nothing!
        this->run_negamax_search(chess_board, i, 0, NEGINF, INF);
        // }
        m_depth = i;
        if (this->search_over) {
            // std::cout << "got here" << std::endl;
            if (this->best_move == NULL_MOVE) {
                this->best_move = this->previous_best_move;
                this->best_eval = this->previous_best_eval;
            }
            // we do not want moves from incomplete search
            break;
        }
        // if (this->best_eval <= BLACK_MATE_SCORE || this->best_eval >= WHITE_MATE_SCORE) {
        //     // std::cout << "got here" << std::endl;
        //     break;
        // }
    }
    std::cout << "ran to depth: " << m_depth << std::endl;
    // if (this->best_move == NULL_MOVE) {
    //     this->best_move = this->previous_best_move;
    // }
}