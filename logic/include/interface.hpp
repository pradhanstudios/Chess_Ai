#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "uci.hpp"
#include "search.hpp"
#include "board.hpp"
#include "search.hpp"

class Interface {
    
    Board *chess_board = new Board(DEFAULT_FEN);
    Searcher searcher = Searcher();
    std::vector<std::string> args;
    std::string last;
    std::string args_str;
    int N;
    bool running = true;
    public:
        void quit();
        void uci();
        void ucinewgame();
        void isready();
        void position();
        void go();
        void eval();
        void show();
        void run_arguments(const std::string &arguments);
        void run();
};

#endif // INTERFACE_HPP