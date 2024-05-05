#include "interface.hpp"

void interface() {
    Searcher s = Searcher();
    Board b = Board(DEFAULT_FEN);
    int N = 0;
    std::string cur_input, fen;
    std::vector<std::string> cur_input_args;
    bool running = true;
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, cur_input);
        if (cur_input == "") {
            continue;
        }
        cur_input_args = split(cur_input, ' ');
        N = cur_input_args.size();
        if (cur_input_args[0] == "quit") {
            running = false;
        }

        else if (cur_input_args[0] == "position") {
            if (cur_input_args[1] == "fen") {
                if (cur_input_args[2] != "startpos") {
                    fen = std::accumulate(
                    std::next(cur_input_args.begin()+2), 
                    cur_input_args.end(), 
                    cur_input_args[2], 
                    [](std::string a, std::string b) {
                        return a + " " + b;
                    }
                    ); // https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
                    std::cout << fen << std::endl;
                    b = Board(fen);
                }
                else {
                    b = Board(DEFAULT_FEN);
                }
            }
            else if (cur_input_args[1] == "play") {
                b.play_move(uci_to_move(cur_input_args[2], b));
                print_move_fancy(uci_to_move(cur_input_args[2], b));
            }

            else if (cur_input_args[1] == "undo") {
                b.undo_move(uci_to_move(cur_input_args[2], b));
            }
        }
        else if (cur_input_args[0] == "go") {
            if (cur_input_args[1] == "perft") {
                if (N < 3) {
                    std::cout << "nah cant do infinite perft, my program is too slow" << std::endl;
                }
                else {
                    std::cout << perft(b, std::stoi(cur_input_args[2]), std::stoi(cur_input_args[2])) << " total nodes" << std::endl; 
                }
            }
            // else if (cur_input_args[1] == "depth") {
            //     s.run_negamax_search(b, std::stoi(cur_input_args[2]), 0, NEGINF, INF);
            //     std::cout << "bestmove " << move_to_uci(s.best_move) << "\nevaluation: " << s.best_eval << std::endl;
            // }

            else if (cur_input_args[1] == "maxtime") {
                s.run_iterative_deepening(b, std::stoi(cur_input_args[2]));
                std::cout << "bestmove " << move_to_uci(s.best_move) << "\nevaluation: " << s.best_eval << std::endl;
            }
        }
        else if (cur_input_args[0] == "show") {
            if (N == 1) {
                b.print_square_data();
            }
            else if (cur_input_args[1] == "bitboard") {
                if (N == 2) {
                    print_BB(b.pieces[FULL]);
                }
                else if (cur_input_args[2] == "white") {
                    print_BB(b.pieces[WHITE]);
                }
                else if (cur_input_args[2] == "black") {
                    print_BB(b.pieces[BLACK]);
                }
                else if (cur_input_args[2] == "pawns") {
                    print_BB(b.pieces[PAWN]);
                }
                else if (cur_input_args[2] == "knights") {
                    print_BB(b.pieces[KNIGHT]);
                }
                else if (cur_input_args[2] == "bishops") {
                    print_BB(b.pieces[BISHOP]);
                }
                else if (cur_input_args[2] == "rooks") {
                    print_BB(b.pieces[ROOK]);
                }
                else if (cur_input_args[2] == "queens") {
                    print_BB(b.pieces[QUEEN]);
                }
                else if (cur_input_args[2] == "kings") {
                    print_BB(b.pieces[KING]);
                }
            }
        }

        else if (cur_input_args[0] == "fullsuite") {
            run_test_suite();
        }

        else if (cur_input_args[0] == "eval") {
            std::cout << s.quiescence_search(b, NEGINF, INF) << std::endl;
        }
    }
}