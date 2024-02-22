#include "board.hpp"

const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::map<char, int> col_letter_to_num = {
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

int no_color(int piece) {
    return piece & 7;
}

Board::Board(std::string fen) {
    char cur;
    int board_ptr = 0;
    int i = fen.length() - 1;
    int spaces = 0;
    // https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
    while (spaces < 2) { // skip to enpessent information
        spaces += (fen[i] == ' '); // if it is a space, add 1 (true)
        i--;
    }
    spaces++;
    if (fen[i] != '-'){
        this->enpessents.push_back(col_letter_to_num.at(fen[i-1]) + std::stoi(&fen[i]));
    }
    i--;

    while (spaces < 4) {
        if (fen[i] == 'q') {
            castles ^= 1 << 4;
        }
        else if (fen[i] == 'k') {
            castles ^= 1 << 3;
        }
        else if (fen[i] == 'Q') {
            castles ^= 1 << 2;
        }
        else if (fen[i] == 'K') {
            castles ^= 1;
        }
        spaces += (fen[i] == ' '); // if it is a space, add 1 (true)
        i--;
    }

    this->turn = (fen[i] == 'w');
    i--;
    int color;
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
            int color = !isupper(cur) ? 0 : 8;
            color ? set_bit_on(this->pieces[WHITE], board_ptr) : set_bit_on(this->pieces[BLACK], board_ptr);
            cur = char(std::tolower(cur));
            if (cur == 'k') {
                set_bit_on(this->pieces[KING], board_ptr);
                this->piece_data[board_ptr] = KING + color;
            }
            else if (cur == 'q') {
                set_bit_on(this->pieces[QUEEN], board_ptr);
                this->piece_data[board_ptr] = QUEEN + color;
            }
            else if (cur == 'r') {
                set_bit_on(this->pieces[ROOK], board_ptr);
                this->piece_data[board_ptr] = ROOK + color;
            }
            else if (cur == 'b') {
                set_bit_on(this->pieces[BISHOP], board_ptr);
                this->piece_data[board_ptr] = BISHOP + color;
            }
            else if (cur == 'n') {
                set_bit_on(this->pieces[KNIGHT], board_ptr);
                this->piece_data[board_ptr] = KNIGHT + color;
            }
            else if (cur == 'p') {
                set_bit_on(this->pieces[PAWN], board_ptr);
                this->piece_data[board_ptr] = PAWN + color;
            }

            board_ptr++;
        }
    }
    this->pieces[EMPTY] = ~(this->pieces[WHITE] | this->pieces[BLACK]);
}

void Board::next_turn() {
    this->turn = !this->turn;
}

void Board::update_empties() { // update bitboards which are dependant on other bitboards
    this->pieces[EMPTY] = ~(this->pieces[WHITE] | this->pieces[BLACK]);
}

void Board::play_move(Move move) {
    Move from, to, type;
    int ind, piece;
    BB t;
    ind = turn_to_index(this->turn);
    from = move & FIRST_SIX;
    to = (move >> 6) & FIRST_SIX;
    type = (move >> 12);

    if (type & PROMOTION) {
        piece = type >> 1;
        fast_reverse_bit(this->pieces[PAWN], from); // off
        fast_reverse_bit(this->pieces[ind], from);
        fast_reverse_bit(this->pieces[ind], to); // on
        fast_reverse_bit(this->pieces[piece], to);
        this->piece_data[from] = EMPTY;
        this->piece_data[to] = piece + (!turn * 8);
    }

    else if (type & CASTLE) {
        piece = type >> 3; // more like side; 0 is kingside 1 is queenside
        castles &= 0b0011 ^ (0b1111 * this->turn);
        fast_reverse_bit(this->pieces[ROOK], (piece * 7) + (!this->turn * 56)); // off
        fast_reverse_bit(this->pieces[KING], 3 + (!this->turn * 56)); // if its black add 56
        fast_reverse_bit(this->pieces[ROOK], 2 + (piece * 3) + (!this->turn * 56)); // on
        fast_reverse_bit(this->pieces[KING], 1 + (5 * piece) + (!this->turn * 56));
        this->piece_data[(piece * 7) + (!this->turn * 56)] = EMPTY;
        this->piece_data[3 + (!this->turn * 56)] = EMPTY;
        this->piece_data[2 + (piece * 3) + (!this->turn * 56)] = ROOK + (!this->turn * 8);
        this->piece_data[1 + (5 * piece) + (!this->turn * 56)] = KING + (!this->turn * 8);
    }

    else if (type & EN_PESSANT) {
        int enpessent = enpessents.back();
        t ^= (1 << from) ^ (1 << to);
        this->pieces[PAWN] ^= t;
        this->pieces[ind] ^= t;
        fast_reverse_bit(this->pieces[PAWN], enpessent);
        fast_reverse_bit(this->pieces[turn_to_index(!turn)], enpessent);
        this->piece_data[from] = EMPTY;
        this->piece_data[enpessent] = EMPTY;
        this->piece_data[to] = piece + (!this->turn * 8);
    }


    else { // normal move
        piece = type >> 1;
        t ^= (1 << to) ^ (1 << from);
        this->pieces[piece] ^= t;
        this->pieces[ind] ^= t;
        this->piece_data[from] = EMPTY;
        this->piece_data[to] = piece + (!this->turn * 8); // add 8 if it is black
    }




    this->next_turn();
}