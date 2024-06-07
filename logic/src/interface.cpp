#include "interface.hpp"

void Interface::quit() {
    this->running = false;
}

void Interface::uci() {
    std::printf("uciok\n");
}
void Interface::ucinewgame() {
    return;
}
void Interface::isready() {
    std::printf("readyok\n");
}

void Interface::position() {
    if (this->args[1] == "startpos") {
        this->chess_board = Board(DEFAULT_FEN);
        this->chess_board.state = RUNNING;
        if (this->N > 2) {
            for (int i = 2; i < this->N; i++) {
                this->chess_board.play_move(uci_to_move(this->args[i], this->chess_board));
            }
        }
    }
    else if (this->args[1] == "fen") {
        this->chess_board = Board(
            std::accumulate(
            std::next(this->args.begin()+2), 
            this->args.begin()+8, 
            this->args[2], 
            [](std::string a, std::string b) {
                return a + " " + b;
            }
            ));
        this->chess_board.state = RUNNING;
        if (N > 8) {
            for (int i = 8; i < this->N; i++) {
                this->chess_board.play_move(uci_to_move(this->args[i], this->chess_board));
            }
        }
    }
    else if (this->args[1] == "show") {
        this->chess_board.print_square_data();
    }
}

void Interface::go() {
    // TODO: implement ponder
    if (this->args[1] == "wtime") {
        if (this->args[3] == "btime") {
            // TODO: implement winc and binc
            this->searcher.run_iterative_deepening(
                this->chess_board, 
                this->chess_board.turn ? std::stoi(this->args[2]) / 10 : std::stoi(this->args[4]) / 10
            );
            // h1h1 is a null move
            std::printf("bestmove %s ponder h1h1\n", move_to_uci(this->searcher.best_move).c_str());
        }
    }
    if (this->args[1] == "depth") {
        this->searcher.run_iterative_deepening(
            this->chess_board, 
            2147483647,
            std::stoi(this->args[2])
        );
        std::printf("bestmove %s ponder h1h1 nodes %lu eval %i\n", move_to_uci(this->searcher.best_move).c_str(), this->searcher.nodes, get_perspective_eval(this->searcher.best_eval, chess_board.turn));
    }
}

void Interface::eval() {
    std::printf("static evaluation of position: %i\n ", evaluate(this->chess_board));
}

void Interface::show() {
    this->chess_board.print_square_data();
}

void Interface::run_arguments(const std::string &arguments) {
    this->args = split(arguments, ' ');
    this->N = args.size();
    if (this->args[0] == "quit") {
            this->quit();
        }

    else if (this->args[0] == "uci") {
        this->uci();
    }

    else if (this->args[0] == "ucinewgame") {
        this->ucinewgame();
    }

    else if (this->args[0] == "isready") {
        this->isready();
    }

    else if (this->args[0] == "position") {
        this->position();
    }

    else if (this->args[0] == "go") {
        this->go();
    }
    // just for debug, not part of uci
    else if (this->args[0] == "eval") {
        this->eval();
    }

    else if (this->args[0] == "show") {
        this->show();
    }
}

void Interface::run() {
    printf("> ");
    while (running && getline(std::cin, args_str)) {
        if (args_str[0] == '_') {
            std::printf("%s\n", (last + args_str.substr(1)).c_str());
            this->run_arguments(last + args_str.substr(1));
        }
        else {
            this->run_arguments(args_str);
        }

        if (this->args[N-1] != "!") { // copy to history
            last = args_str;
        }
        printf("> ");
    }
}

// deprecated
// void interface() {
//     Searcher s = Searcher();
//     Board b = Board(DEFAULT_FEN);
//     int N = 0;
//     std::string cur_input, fen;
//     std::vector<std::string> cur_input_args;
//     bool running = true;
//     while (running) {
//         std::cout << "> ";
//         std::getline(std::cin, cur_input);
//         if (cur_input == "") {
//             continue;
//         }
//         cur_input_args = split(cur_input, ' ');
//         N = cur_input_args.size();
//         if (cur_input_args[0] == "quit") {
//             running = false;
//         }

//         else if (cur_input_args[0] == "position") {
//             if (cur_input_args[1] == "fen") {
//                 if (cur_input_args[2] != "startpos") {
//                     fen = std::accumulate(
//                     std::next(cur_input_args.begin()+2), 
//                     cur_input_args.end(), 
//                     cur_input_args[2], 
//                     [](std::string a, std::string b) {
//                         return a + " " + b;
//                     }
//                     ); // https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
//                     std::cout << fen << std::endl;
//                     b = Board(fen);
//                 }
//                 else {
//                     b = Board(DEFAULT_FEN);
//                 }
//             }
//             else if (cur_input_args[1] == "play") {
//                 b.play_move(uci_to_move(cur_input_args[2], b));
//                 print_move_fancy(uci_to_move(cur_input_args[2], b));
//             }

//             else if (cur_input_args[1] == "undo") {
//                 b.undo_move(uci_to_move(cur_input_args[2], b));
//             }
//         }
//         else if (cur_input_args[0] == "go") {
//             if (cur_input_args[1] == "perft") {
//                 if (N < 3) {
//                     std::cout << "nah cant do infinite perft, my program is too slow" << std::endl;
//                 }
//                 else {
//                     std::cout << perft(b, std::stoi(cur_input_args[2]), std::stoi(cur_input_args[2])) << " total nodes" << std::endl; 
//                 }
//             }
//             // else if (cur_input_args[1] == "depth") {
//             //     s.run_negamax_search(b, std::stoi(cur_input_args[2]), 0, NEGINF, INF);
//             //     std::cout << "bestmove " << move_to_uci(s.best_move) << "\nevaluation: " << s.best_eval << std::endl;
//             // }

//             else if (cur_input_args[1] == "maxtime") {
//                 s.run_iterative_deepening(b, std::stoi(cur_input_args[2]));
//                 std::cout << "bestmove " << move_to_uci(s.best_move) << "\nevaluation: " << s.best_eval << std::endl;
//             }
//         }
//         else if (cur_input_args[0] == "show") {
//             if (N == 1) {
//                 b.print_square_data();
//             }
//             else if (cur_input_args[1] == "bitboard") {
//                 if (N == 2) {
//                     print_BB(b.pieces[FULL]);
//                 }
//                 else if (cur_input_args[2] == "white") {
//                     print_BB(b.pieces[WHITE]);
//                 }
//                 else if (cur_input_args[2] == "black") {
//                     print_BB(b.pieces[BLACK]);
//                 }
//                 else if (cur_input_args[2] == "pawns") {
//                     print_BB(b.pieces[PAWN]);
//                 }
//                 else if (cur_input_args[2] == "knights") {
//                     print_BB(b.pieces[KNIGHT]);
//                 }
//                 else if (cur_input_args[2] == "bishops") {
//                     print_BB(b.pieces[BISHOP]);
//                 }
//                 else if (cur_input_args[2] == "rooks") {
//                     print_BB(b.pieces[ROOK]);
//                 }
//                 else if (cur_input_args[2] == "queens") {
//                     print_BB(b.pieces[QUEEN]);
//                 }
//                 else if (cur_input_args[2] == "kings") {
//                     print_BB(b.pieces[KING]);
//                 }
//             }
//         }

//         else if (cur_input_args[0] == "fullsuite") {
//             run_test_suite();
//         }

//         else if (cur_input_args[0] == "eval") {
//             std::cout << s.quiescence_search(b, NEGINF, INF) << std::endl;
//         }
//     }
// }