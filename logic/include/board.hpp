#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"
#include "move.hpp"

enum PIECE {
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6,
    WHITE = 7,
    BLACK = 8,
    FULL = 9
};

extern const std::string DEFAULT_FEN;
extern const std::map<char, int> col_letter_to_num;

int turn_to_index(bool turn);
int no_color(int piece);
void print_vector(std::vector<int> v);

std::vector<std::string> split(std::string s, char delim);

class Board {
    public:
        std::array<int, 64> piece_data;
        std::array<BB, 10> pieces;
        // int castles;
        std::vector<int> enpessent_history, castle_history, capture_history;
        bool turn; // true for white false for black
        Board(std::string fen);
        void print_square_data();
        void next_turn();
        void update_bitboards();
        void play_move(Move move);
        void undo_move(Move move);
        // this needs some more thinking
//         void play_move(Move move) { // this is assuming that the moves are legal
//             int piece;
//             int to = (move & MOVE_TO) >> 6;
//             int from = move & MOVE_FROM;
//             if (move & NORMAL_MOVE) {
//                 BB t;
//                 piece = (move >> 13);
//                 int turn_to_ind = turn_to_index(this->turn);
//                 if (piece == KING) { // if we move the king we can not castle
//                     castles[(2 * !turn)] = 0;
//                     castles[1 + (2 * !turn)] = 0;
//                 }
//                 t = (1ULL << from) ^ (1ULL << to);
//                 this->pieces[piece] ^= t;
//                 this->pieces[turn_to_ind] ^= t;
//             }

//             else if (move & PROMOTION) {
//                 piece = (move >> 13);
//                 int turn_to_ind = turn_to_index(this->turn);
//                 BB t, f;
//                 t = 1ULL << to;
//                 f = 1ULL << from;
//                 this->pieces[PAWN] ^= f;
//                 this->pieces[turn_to_ind] ^= f;
//                 this->pieces[piece] ^= t;
//                 this->pieces[turn_to_ind] ^= t;
//             }
            
//             else if (move & CASTLE) {
//                 bool side = (move >> 15);
//                 castles[side + (2 * !this->turn)] = 0; // ex 3 = 1(queenside) + (2*!0(!0 = 1, 0 is for black)) castles[3] == black queen
//                 castles[!side + (2 * !this->turn)] = 0; // if we castled once, we can not castle again
//                 // too inconvient to xor here
//                 set_bit_off(this->pieces[ROOK], (side * 7) + (!this->turn * 56));
//                 set_bit_off(this->pieces[KING], 3 + (!this->turn * 56));
//                 set_bit_on(this->pieces[ROOK], 2 + (side * 3) + (!this->turn * 56));
//                 set_bit_on(this->pieces[KING], 1 + (5 * side) + (!this->turn * 56));
//             }

//             else if (move & EN_PESSANT) {
//                 int turn_to_ind = turn_to_index(this->turn);
//                 BB t, f, e;
//                 f = 1ULL << (move & MOVE_FROM);
//                 t = 1ULL << ((move & MOVE_TO) >> 6);
//                 e = 1ULL << (this->enpessant);
//                 this->pieces[PAWN] ^= f;
//                 this->pieces[turn_to_ind] ^= f;
//                 this->pieces[PAWN] ^= e;
//                 this->pieces[turn_to_index(!this->turn)] ^= e;
//                 this->pieces[PAWN] ^= t;
//                 this->pieces[turn_to_ind] ^= t;
//             }

//         }
};