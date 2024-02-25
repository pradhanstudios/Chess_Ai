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

void print_vector(std::vector<int> v) {
    for (int i : v) {
        std::cout << i << ",  ";
    }
    std::cout << std::endl;
} 

std::vector<std::string> split(std::string s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

Board::Board(std::string fen) {
    // char cur;
    int board_ptr = 0;
    int i = fen.length() - 1;
    
    std::vector<std::string> fensplit = split(fen, ' ');

    if (fensplit[3][0] != '-'){
        this->enpessents.push_back(col_letter_to_num.at(fensplit[3][0]) + std::stoi(&fensplit[3][1])-1);
    }
    else {
        this->enpessents.push_back(-1);
    }
    i--;

    for (char cur : fensplit[2]) {
        if (cur == 'q') {
            this->castles ^= 1 << 4;
        }
        else if (cur == 'k') {
            this->castles ^= 1 << 3;
        }
        else if (cur == 'Q') {
            this->castles ^= 1 << 2;
        }
        else if (cur == 'K') {
            this->castles ^= 1;
        }
        // std::cout << spaces << std::endl;
        // i--;
    }

    this->turn = fensplit[1][0] == 'w';

    int color;
    for (char cur : fensplit[0])
    {

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
            color = isupper(cur) ? 8 : 0;
            color ? set_bit_on(this->pieces[BLACK], board_ptr) : set_bit_on(this->pieces[WHITE], board_ptr);
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

    this->update_bitboards();
    
}

void Board::print_square_data() {
    std::string cur;
    int entry;
    for (int i = 64; i > -1; i--) {
        if ((i+1) % 8 == 0) {
            std::cout << "\n";
        }
        entry = this->piece_data[i];
        cur = "";
        cur += entry < BLACK ? "W" : "B"; 
        if (no_color(entry) == EMPTY) {
            cur = " 0";
        }

        else if (no_color(entry) == PAWN) {
            cur += "P";
        }

        else if (no_color(entry) == KNIGHT) {
            cur += "N";
        }

        else if (no_color(entry) == BISHOP) {
            cur += "B";
        }

        else if (no_color(entry) == ROOK) {
            cur += "R";
        }

        else if (no_color(entry) == QUEEN) {
            cur += "Q";
        }

        else if (no_color(entry) == KING) {
            cur += "K";
        }
        std::cout << cur << " ";
    }
}

void Board::next_turn() {
    this->turn = !this->turn;
}

void Board::update_bitboards() { // update bitboards which are dependant on other bitboards
    this->pieces[FULL] = (this->pieces[WHITE] | this->pieces[BLACK]);
    this->pieces[EMPTY] = ~this->pieces[FULL];
}

void Board::play_move(Move move) {
    Move from, to, type;
    int ind, piece;
    int cur_en_pessant = -1;
    ind = turn_to_index(this->turn);
    from = move & FIRST_SIX;
    to = (move >> 6) & FIRST_SIX;
    type = (move >> 12);
    // print_Move_bits(move);

    if (type == NORMAL_MOVE) { // normal move
        // std::cout << to << "\t" << from << std::endl;
        // piece = type >> 1;
        piece = no_color(this->piece_data[from]);
        BB t = (1ULL << to) ^ (1ULL << from);
        // std::cout << ind << std::endl;
        // print_BB((1 << to) ^ (1 << from));
        this->pieces[piece] ^= t;
        this->pieces[ind] ^= t;
        if (piece_data[to] == ROOK) { // rook is captured
            if (to == 63) { // blck qside
                castles &= 0b0111;
            }

            else if (to == 56) { // blck kngside 
                castles &= 0b1011;
            }

            else if (to == 7) { // wht qside
                castles &= 0b1101;
            }

            else if (to == 0) { // wht kngside
                castles &= 0b1110;
            }
        }
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        if (piece == ROOK) {
            if (from == 63) { // blck qside
                castles &= 0b0111;
            }

            else if (from == 56) { // blck kngside 
                castles &= 0b1011;
            }

            else if (from == 7) { // wht qside
                castles &= 0b1101;
            }

            else if (from == 0) { // wht kngside
                castles &= 0b1110;
            }
        }
        else if (piece == PAWN) {
            if (abs(from - to) == 16) { // if it moved two rows
                cur_en_pessant = to;
                // std::cout << "original en_pessant: " << cur_en_pessant << "\t" << "to: " << to << "\tfrom: " << from << "\t";
            }
        }
        else if (piece == KING) {
            castles &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
        }
        // std::cout << piece << std::endl;
    }

    else if (type & PROMOTION) {
        piece = type >> 1; // promotion piece
        fast_reverse_bit(this->pieces[PAWN], from); // off
        fast_reverse_bit(this->pieces[ind], from);
        fast_reverse_bit(this->pieces[ind], to); // on
        fast_reverse_bit(this->pieces[piece], to);
        if (piece_data[to] == ROOK) { // rook is captured
            if (to == 63) { // blck qside
                castles &= 0b0111;
            }

            else if (to == 56) { // blck kngside 
                castles &= 0b1011;
            }

            else if (to == 7) { // wht qside
                castles &= 0b1101;
            }

            else if (to == 0) { // wht kngside
                castles &= 0b1110;
            }
        }
        this->piece_data[to] = piece + (this->turn * 8);
        this->piece_data[from] = EMPTY;
    }

    else if (type & CASTLE) {
        // std::cout << "Here" << std::endl;
        piece = type >> 3; // more like side; 0 is kingside 1 is queenside
        castles &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again

        // starting positions
        int rookStart = (piece == 0) ? (7 + (!this->turn * 56)) : (0 + (!this->turn * 56)); // (piece * 7) + (!this->turn * 56)
        int kingStart = 4 + (!this->turn * 56);
        int rookEnd = 3 + (!piece << 1) + (!this->turn * 56); // x << 1 == x * 2
        int kingEnd = 6 - (piece * 4) + (!this->turn * 56);
        
        fast_reverse_bit(this->pieces[ROOK], rookStart); // off
        fast_reverse_bit(this->pieces[KING], kingStart); // if its black add 56
        fast_reverse_bit(this->pieces[ROOK], rookEnd); // on x << 1 == x * 2
        fast_reverse_bit(this->pieces[KING], kingEnd);
        this->piece_data[rookEnd] = this->piece_data[rookStart];
        this->piece_data[kingEnd] = this->piece_data[kingStart];
        this->piece_data[rookStart] = EMPTY;
        this->piece_data[kingStart] = EMPTY;
    }

    else if (type == EN_PESSANT) {
        // int N = this->enpessents.size();
        int enpessent = this->enpessents.back();
        // print_vector(this->enpessents);
        // std::cout << enpessent << std::endl;
        // std::cout << this->enpessents[N-1] << "\t" << this->enpessents[N-2] << std::endl;
        BB t = (1ULL << from) | (1ULL << to);
        // print_BB((BB)(1ULL << from));
        std::cout << to << std::endl;
        print_BB(1ULL << to);
        this->pieces[PAWN] ^= t;
        // std::cout << ind << std::endl;
        this->pieces[ind] ^= t;
        fast_reverse_bit(this->pieces[PAWN], enpessent);
        fast_reverse_bit(this->pieces[turn_to_index(!turn)], enpessent);
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        this->piece_data[enpessent] = EMPTY;
    }

    else {
        std::cout << "something went wrong..." << std::endl;
        print_Move_bits(move);
    }

    // std::cout << cur_en_pessant << std::endl;
    this->enpessents.push_back(cur_en_pessant);
    this->update_bitboards();
    this->next_turn();
}