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
};

const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::map<char, int> col_letter_to_num = {
    {'a', 0},
    {'b', 1},
    {'c', 2},
    {'d', 3},
    {'e', 4},
    {'f', 5},
    {'g', 6},
    {'h', 7},
};

int turn_to_index(bool turn) { // turn 1 or 0 into 7 (WHITE) or 8 (BLACK)
    return BLACK - turn;
}

class Board {
    public:
        std::array<BB, 9> pieces;
        std::array<bool, 4> castles;
        int enpessant;
        bool turn; // true for white false for black
        Board(std::string fen) {
            char cur;
            int board_ptr = 0;
            int i = fen.length() - 1;
            int spaces = 0;
            // https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
            while (spaces < 3) { // skip to castling information
                spaces += (fen[i] == ' '); // if it is a space, add 1 (true)
                i--;
            }

            while (spaces < 4) {
                if (fen[i] == 'q') {
                    castles[3] = true;
                }
                else if (fen[i] == 'k') {
                    castles[2] = true;
                }
                else if (fen[i] == 'Q') {
                    castles[1] = true;
                }
                else if (fen[i] == 'K') {
                    castles[0] = true;
                }
                spaces += (fen[i] == ' '); // if it is a space, add 1 (true)
                i--;
            }

            this->turn = (fen[i] == 'w');
            i--;

            for (; i > -1; i--)
            {
                cur = fen[i];

                if (isdigit(cur))
                {
                    board_ptr += std::stoi(&cur);
                }
                else if (cur == '/')
                {
                    continue;
                }
                else
                {
                    isupper(cur) ? set_bit_on(this->pieces[WHITE], board_ptr) : set_bit_on(this->pieces[BLACK], board_ptr);
                    cur = char(std::tolower(cur));
                    if (cur == 'k') {
                        set_bit_on(this->pieces[KING], board_ptr);
                    }
                    else if (cur == 'q') {
                        set_bit_on(this->pieces[QUEEN], board_ptr);
                    }
                    else if (cur == 'r') {
                        set_bit_on(this->pieces[ROOK], board_ptr);
                    }
                    else if (cur == 'b') {
                        set_bit_on(this->pieces[BISHOP], board_ptr);
                    }
                    else if (cur == 'n') {
                        set_bit_on(this->pieces[KNIGHT], board_ptr);
                    }
                    else if (cur == 'p') {
                        set_bit_on(this->pieces[PAWN], board_ptr);
                    }

                    board_ptr++;
                }
            }
            this->pieces[EMPTY] = ~(this->pieces[WHITE] | this->pieces[BLACK]);
            }

        void next_turn() {
            this->turn = !this->turn;
        }

        void update_empties() { // update bitboards which are dependant on other bitboards
            this->pieces[EMPTY] = ~(this->pieces[WHITE] | this->pieces[BLACK]);
        }
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