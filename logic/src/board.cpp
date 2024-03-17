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
    if (piece) {
        return piece & 7;
    }
    else {
        return 0;
    }
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
    int i = fen.length() - 1;
    int castles = 0;
    
    std::vector<std::string> fensplit = split(fen, ' ');

    if (fensplit[3][0] != '-'){
        this->enpessent_history.push_back(col_letter_to_num.at(fensplit[3][0]) + std::stoi(&fensplit[3][1])-1);
    }
    else {
        this->enpessent_history.push_back(-1);
    }
    i--;

    for (char cur : fensplit[2]) {
        if (cur == 'q') {
            castles ^= 1 << 4;
        }
        else if (cur == 'k') {
            castles ^= 1 << 3;
        }
        else if (cur == 'Q') {
            castles ^= 1 << 2;
        }
        else if (cur == 'K') {
            castles ^= 1;
        }
        // std::cout << spaces << std::endl;
        // i--;
    }
    this->castle_history.push_back(castles);

    this->turn = fensplit[1][0] == 'w';

    int color;
    int board_ptr = 0;
    // int actual_pos;
    char cur;
    std::vector<std::string> fen_lines = split(fensplit[0], '/');
    std::reverse(fen_lines.begin(), fen_lines.end());
    int row = 0;
    // std::string pieces_fen = fensplit[0];
    for (std::string line : fen_lines) {
        // std::cout << line << std::endl;
        board_ptr = 8*row;
        for (int i = line.length()-1; i > -1; i--) {
            cur = line[i];
            // std::cout << cur << " ";
            if (isdigit(cur))
            {
                board_ptr += std::stoi(&cur);
            }
            else if (cur == '/')
            {
                break;
            }
            else
            {
                color = isupper(cur) ? 0 : 8;
                // actual_pos = 63-board_ptr;
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
        row++;
    }

    this->update_bitboards();
    
}

void Board::print_square_data() {
    std::string cur;
    int entry, cur_piece;
    for (int i = 63; i > -1; i--) {
        if (((i + 1) & 7) == 0) {
            std::cout << "\n";
        }
        entry = this->piece_data[i];
        cur_piece = no_color(entry);
        cur = "";

        if (cur_piece == EMPTY) {
            cur = " 0";
        } else {
            cur += (entry & BLACK) ? "B" : "W";
            // std::cout << (cur_piece & BLACK) << std::endl;

            switch (cur_piece) {
                case PAWN:
                    cur += "P";
                    break;
                case KNIGHT:
                    cur += "N";
                    break;
                case BISHOP:
                    cur += "B";
                    break;
                case ROOK:
                    cur += "R";
                    break;
                case QUEEN:
                    cur += "Q";
                    break;
                case KING:
                    cur += "K";
                    break;
                default:
                    std::cout << "got here\t" << cur_piece << "\t";
                    break;
            }
        }
        std::cout << cur << " ";

    }
    std::cout << std::endl;

    // std::cout << "Done printing square data." << std::endl;
}


inline void Board::next_turn() {
    this->turn = !this->turn;
}

inline void Board::update_bitboards() { // update bitboards which are dependant on other bitboards
    this->pieces[FULL] = (this->pieces[WHITE] | this->pieces[BLACK]);
    this->pieces[EMPTY] = ~this->pieces[FULL];
}

void Board::play_move(Move move) {
    Move from, to, type;
    int same_team, piece;
    int cur_en_pessant = -1;
    same_team = turn_to_index(this->turn);
    from = move & FIRST_SIX;
    to = (move >> 6) & FIRST_SIX;
    type = (move >> 12);
    int cur_castle = this->castle_history.back();
    this->capture_history.push_back(this->piece_data[to]);
    // print_Move_bits(move);

    if (type == NORMAL_MOVE) { // normal move
        // std::cout << to << "\t" << from << std::endl;
        // piece = type >> 1;
        piece = no_color(this->piece_data[from]);
        BB t = (1ULL << to) ^ (1ULL << from);
        // std::cout << ind << std::endl;
        // print_BB((1 << to) ^ (1 << from));
        this->pieces[piece] ^= t;
        this->pieces[same_team] ^= t;
        if (piece_data[to] == ROOK) { // rook is captured
            if (to == 63) { // blck qside
                cur_castle &= 0b0111;
            }

            else if (to == 56) { // blck kngside 
                cur_castle &= 0b1011;
            }

            else if (to == 7) { // wht qside
                cur_castle &= 0b1101;
            }

            else if (to == 0) { // wht kngside
                cur_castle &= 0b1110;
            }
        }
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        if (piece == ROOK) {
            if (from == 63) { // blck qside
                cur_castle &= 0b0111;
            }

            else if (from == 56) { // blck kngside 
                cur_castle &= 0b1011;
            }

            else if (from == 7) { // wht qside
                cur_castle &= 0b1101;
            }

            else if (from == 0) { // wht kngside
                cur_castle &= 0b1110;
            }
        }
        else if (piece == PAWN) {
            if (abs(from - to) == 16) { // if it moved two rows
                cur_en_pessant = to;
                // std::cout << "original en_pessant: " << cur_en_pessant << "\t" << "to: " << to << "\tfrom: " << from << "\t";
            }
        }
        else if (piece == KING) {
            cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
        }
        // std::cout << piece << std::endl;
    }

    else if (type & PROMOTION) {
        piece = type >> 1; // promotion piece
        fast_reverse_bit(this->pieces[PAWN], from); // off
        fast_reverse_bit(this->pieces[same_team], from);
        fast_reverse_bit(this->pieces[same_team], to); // on
        fast_reverse_bit(this->pieces[piece], to);
        if (piece_data[to] == ROOK) { // rook is captured
            if (to == 63) { // blck qside
                cur_castle &= 0b0111;
            }

            else if (to == 56) { // blck kngside 
                cur_castle &= 0b1011;
            }

            else if (to == 7) { // wht qside
                cur_castle &= 0b1101;
            }

            else if (to == 0) { // wht kngside
                cur_castle &= 0b1110;
            }
        }
        this->piece_data[to] = piece + (!this->turn * 8);
        this->piece_data[from] = EMPTY;
    }

    else if (type & CASTLE) {
        // std::cout << "Here" << std::endl;
        piece = type >> 3; // more like side; 1 is kingside 0 is queenside
        cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again

        BB t = (piece ? 0b10111000ULL : 0b1111) << (!this->turn * 56);

        // starting positions
        int rookStart = (piece == 0) ? (7 + (!this->turn * 56)) : (0 + (!this->turn * 56)); // (piece * 7) + (!this->turn * 56)
        int kingStart = 3 + (!this->turn * 56);
        int rookEnd = 2 + (!piece << 1) + (!this->turn * 56);
        int kingEnd = 1 + (!piece << 2) + (!this->turn * 56); // x << 1 == x * 2
        // int kingEnd = 6 - (piece * 4) + (!this->turn * 56);
        
        fast_reverse_bit(this->pieces[ROOK], rookStart); // off
        fast_reverse_bit(this->pieces[KING], kingStart); // if its black add 56
        fast_reverse_bit(this->pieces[ROOK], rookEnd); // on x << 1 == x * 2
        fast_reverse_bit(this->pieces[KING], kingEnd);
        this->pieces[same_team] ^= t;
        this->piece_data[rookEnd] = this->piece_data[rookStart];
        this->piece_data[kingEnd] = this->piece_data[kingStart];
        this->piece_data[rookStart] = EMPTY;
        this->piece_data[kingStart] = EMPTY;
    }

    else if (type == EN_PESSANT) {
        // int N = this->enpessent_history.size();
        int enpessent = this->enpessent_history.back();
        // print_vector(this->enpessent_history);
        // std::cout << enpessent << std::endl;
        // std::cout << this->enpessent_history[N-1] << "\t" << this->enpessent_history[N-2] << std::endl;
        BB t = (1ULL << from) ^ (1ULL << to);
        // print_BB((BB)(1ULL << from));
        // std::cout << to << std::endl;
        // print_BB(1ULL << to);
        this->pieces[PAWN] ^= t;
        // std::cout << same_team << std::endl;
        this->pieces[same_team] ^= t;
        fast_reverse_bit(this->pieces[PAWN], enpessent);
        fast_reverse_bit(this->pieces[turn_to_index(!turn)], enpessent);
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        this->piece_data[enpessent] = EMPTY;
        cur_en_pessant = enpessent;
    }

    else {
        std::cout << "something went wrong..." << std::endl;
        print_move_fancy(move);
    }

    // std::cout << cur_en_pessant << std::endl;
    this->castle_history.push_back(cur_castle);
    this->enpessent_history.push_back(cur_en_pessant);
    this->update_bitboards();
    this->next_turn();
}

void Board::undo_move(Move move) {
    this->next_turn(); // technically previous turn, but whatever
    Move from, to, type;
    int same_team, other_team, piece, cur_en_pessant, capture, castle;
    cur_en_pessant = this->enpessent_history.back();
    capture = this->capture_history.back();
    castle = this->castle_history.back();
    this->enpessent_history.pop_back();
    this->capture_history.back();
    this->castle_history.pop_back();
    same_team = turn_to_index(this->turn);
    other_team = turn_to_index(!this->turn);
    from = move & FIRST_SIX;
    to = (move >> 6) & FIRST_SIX;
    type = (move >> 12);

    if (type == NORMAL_MOVE) {
        BB t = (1ULL << from) ^ (1ULL << to);
        this->pieces[this->piece_data[from]] ^= t;
        this->pieces[same_team] ^= t;
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[to] = capture;
    }
    else if (type & PROMOTION) {
        fast_reverse_bit(this->pieces[PAWN], from);
        fast_reverse_bit(this->pieces[same_team], from);

        this->piece_data[from] = PAWN + (this->turn ? 0 : 8);
        this->piece_data[to] = capture;
    }
    else if (type == EN_PESSANT) {
        BB t = (1ULL << from) ^ (1ULL << to);
        // std::cout << cur_en_pessant << std::endl;
        this->piece_data[cur_en_pessant] = PAWN + (!this->turn ? 0 : 8);
        // std::cout << (PAWN + !this->turn ? 0 : 8) << std::endl;
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[to] = EMPTY;
        this->pieces[PAWN] ^= t;
        this->pieces[same_team] ^= t;
        fast_reverse_bit(this->pieces[PAWN], cur_en_pessant);
        fast_reverse_bit(this->pieces[other_team], cur_en_pessant);
    }
    else if (type & CASTLE) {
        BB t = (((castle & (this->turn ? 0b0011 : 0b1100)) & 0b1010) ? 0b10111000ULL : 0b1111) << (!this->turn * 56);
        this->pieces[same_team] ^= t;
        int rookStart = (piece == 0) ? (7 + (!this->turn * 56)) : (0 + (!this->turn * 56)); // (piece * 7) + (!this->turn * 56)
        int kingStart = 3 + (!this->turn * 56);
        int rookEnd = 2 + (!piece << 1) + (!this->turn * 56);
        int kingEnd = 1 + (!piece << 2) + (!this->turn * 56); // x << 1 == x * 2
        fast_reverse_bit(this->pieces[ROOK], rookStart); // off
        fast_reverse_bit(this->pieces[KING], kingStart); // if its black add 56
        fast_reverse_bit(this->pieces[ROOK], rookEnd); // on x << 1 == x * 2
        fast_reverse_bit(this->pieces[KING], kingEnd);
        this->piece_data[rookStart] = this->piece_data[rookEnd];
        this->piece_data[kingStart] = this->piece_data[kingEnd];
        this->piece_data[rookEnd] = EMPTY;
        this->piece_data[kingEnd] = EMPTY;
    }

    this->update_bitboards();
}