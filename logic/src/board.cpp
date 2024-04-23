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

// int turn_to_index(bool turn) { // turn 1 or 0 into 7 (WHITE) or 8 (BLACK)
//     return BLACK - turn;
// }

// int no_color(int piece) {
//     return piece & 7;
// }
void print_vector(const std::vector<int> &v) noexcept {
    for (int i : v) {
        std::cout << i << ",  ";
    }
    std::cout << std::endl;
} 

void print_vector(const std::vector<std::string> &v) noexcept {
    for (std::string i : v) {
        std::cout << i << ",  ";
    }
    std::cout << std::endl;
} 


std::vector<std::string> split(const std::string &s, const char &delim) noexcept {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

History::History(const unsigned int &en_pessant, const unsigned int &castle, const unsigned int &capture, const unsigned int &fifty_move_rule) {
    this->en_pessant = en_pessant;
    this->castle = castle;
    this->capture = capture;
    this->fifty_move_rule = fifty_move_rule;
}

Board::Board(const std::string &fen) noexcept {
    std::fill(this->pieces.begin(), this->pieces.end(), 0ULL);
    // char cur;
    // int i = fen.length() - 1;
    History cur_history = History();
    unsigned int castles = 0u;
    
    std::vector<std::string> fensplit = split(fen, ' ');
    // print_vector(fensplit);
    // std::cout << fensplit[3] << std::endl;
    // std::cout << (7-col_letter_to_num.at(fensplit[3][0]) + (std::stoi(&fensplit[3][1])-1)*8) << std::endl;
    if (fensplit[3][0] != '-'){
        cur_history.en_pessant = (7-col_letter_to_num.at(fensplit[3][0]) + (std::stoi(&fensplit[3][1])-1)*8);
    }
    else {
        cur_history.en_pessant = 0u; // you can never have an en pessant at position 0
    }
    // i--;

    for (char cur : fensplit[2]) {
        if (cur == 'q') {
            castles ^= 1 << 3;
        }
        else if (cur == 'k') {
            castles ^= 1 << 2;
        }
        else if (cur == 'Q') {
            castles ^= 1 << 1;
        }
        else if (cur == 'K') {
            castles ^= 1;
        }
        // std::cout << spaces << std::endl;
        // i--;
    }
    cur_history.castle = castles;
    cur_history.fifty_move_rule = 0u;


    this->turn = fensplit[1][0] == 'w';

    cur_history.en_pessant -= PAWN_DIRECTION_LOOKUP[this->turn];

    this->history.push_back(cur_history);

    int color;
    int board_ptr = 0u;
    // int actual_pos;
    char cur;
    std::vector<std::string> fen_lines = split(fensplit[0], '/');
    std::reverse(fen_lines.begin(), fen_lines.end());
    int row = 0;
    // std::cout << row << std::endl;
    // std::string pieces_fen = fensplit[0];
    for (std::string line : fen_lines) {
        // std::cout << line << std::endl;
        board_ptr = 8*row;
        for (int i = line.length()-1; i > -1; i--) {
            cur = line[i];
            // std::cout << cur << " ";
            if (isdigit(cur))
            {
                // std::cout << board_ptr << std::endl;
                board_ptr += cur - '0'; // convert char to int
            }
            else if (cur == '/')
            {
                break;
            }
            else
            {
                color = isupper(cur) ? 0 : 8;
                // actual_pos = 63-board_ptr;
                color ? fast_reverse_bit(this->pieces[BLACK], board_ptr) : fast_reverse_bit(this->pieces[WHITE], board_ptr);
                cur = char(std::tolower(cur));
                // std::cout << cur << std::endl;
                // std::cout << cur << " " << board_ptr << std::endl;
                if (cur == 'k') {
                    // std::cout << "got here" << std::endl;
                    // print_BB(this->pieces[KING]);
                    fast_reverse_bit(this->pieces[KING], board_ptr);
                    // print_BB(this->pieces[KING]);
                    this->piece_data[board_ptr] = KING + color;
                }
                else if (cur == 'q') {
                    fast_reverse_bit(this->pieces[QUEEN], board_ptr);
                    this->piece_data[board_ptr] = QUEEN + color;
                }
                else if (cur == 'r') {
                    fast_reverse_bit(this->pieces[ROOK], board_ptr);
                    this->piece_data[board_ptr] = ROOK + color;
                }
                else if (cur == 'b') {
                    fast_reverse_bit(this->pieces[BISHOP], board_ptr);
                    this->piece_data[board_ptr] = BISHOP + color;
                }
                else if (cur == 'n') {
                    fast_reverse_bit(this->pieces[KNIGHT], board_ptr);
                    this->piece_data[board_ptr] = KNIGHT + color;
                }
                else if (cur == 'p') {
                    fast_reverse_bit(this->pieces[PAWN], board_ptr);
                    this->piece_data[board_ptr] = PAWN + color;
                }
                board_ptr++;
            }
        }
        row++;
    }

    // print_BB(this->pieces[WHITE]);

    this->update_bitboards();
    
}

void Board::print_square_data() noexcept {
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

// inline bool Board::is_in_check() {
//     return (this->pieces[KING] & this->pieces[turn_to_index[this->turn]]) & this->pieces[OTHER_TEAM_ATTACKS];
// }

// inline void Board::next_turn() {
//     this->turn = !this->turn;
// }

// inline void Board::update_bitboards() { // update bitboards which are dependant on other bitboards
//     this->pieces[FULL] = (this->pieces[WHITE] | this->pieces[BLACK]);
//     this->pieces[EMPTY] = ~this->pieces[FULL];
// }

void Board::play_move(const Move &move) noexcept {
    unsigned int cur_en_pessant = 0u;
    int same_team = turn_to_index[this->turn];
    int other_team = turn_to_index[!this->turn];
    unsigned int from = move.from;
    unsigned int to = move.to;
    unsigned int type = move.type;
    // std::cout << type << std::endl;
    History cur_history = this->history.back();
    unsigned int cur_castle = cur_history.castle;
    unsigned int fifty_move_rule = cur_history.fifty_move_rule + 1;
    History new_history = History();
    new_history.capture = this->piece_data[to];
    // print_Move_bits(move);

    if (type == NORMAL_MOVE) { // normal move
        // std::cout << to << "\t" << from << std::endl;
        // piece = type >> 1;
        // int no_color_capture_piece = no_color(this->piece_data[to]);
        BB capture = 1ULL << to;
        int piece = no_color(this->piece_data[from]);
        BB t = capture ^ (SQUARE_TO_BB[from]);
        // std::cout << ind << std::endl;
        // print_BB((1 << to) ^ (1 << from));
        this->pieces[piece] ^= t;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;
        if ((this->piece_data[to] & 7) == ROOK) { // rook is captured
            // std::cout << "got here" << std::endl;
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
        if (this->piece_data[to]) { // capture
            fifty_move_rule = 0;
            this->pieces[EMPTY] ^= capture;
            this->pieces[FULL] ^= capture;
            this->pieces[other_team] ^= capture;
            this->pieces[this->piece_data[to] & 7] ^= capture;
        }
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        if ((this->piece_data[to] & 7) == ROOK) {
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
        else if ((this->piece_data[to] & 7) == PAWN) {
            fifty_move_rule = 0;
            if (abs(from - to) == 16) { // if it moved two rows
                // std::cout << "got here " << from << " " << to << std::endl; 
                cur_en_pessant = to;
                // std::cout << "original en_pessant: " << cur_en_pessant << "\t" << "to: " << to << "\tfrom: " << from << "\t";
            }
        }
        else if ((this->piece_data[to] & 7) == KING) {
            cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
        }
        // std::cout << piece << std::endl;
    }

    else if (type & PROMOTION) {
        fifty_move_rule = 0;
        // int no_color_capture_piece = no_color(this->piece_data[to]);
        // std::cout << "got here" << std::endl;
        int piece = type >> 1; // promotion piece
        // std::cout << piece << std::endl;
        fast_reverse_bit(this->pieces[PAWN], from); // off
        fast_reverse_bit(this->pieces[same_team], from);
        fast_reverse_bit(this->pieces[same_team], to); // on
        fast_reverse_bit(this->pieces[piece], to);
        this->pieces[FULL] ^= (from | to);
        this->pieces[EMPTY] ^= (from | to);
        if ((this->piece_data[to] & 7) == ROOK) { // rook is captured
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
        if (piece_data[to]) { // capture
            BB capture = SQUARE_TO_BB[to];
            this->pieces[EMPTY] ^= capture;
            this->pieces[FULL] ^= capture;
            // std::cout << "capture" << std::endl;
            // print_BB(this->pieces[other_team]);
            this->pieces[other_team] ^= capture;
            this->pieces[piece_data[to] & 7] ^= capture;
        }
        // std::cout << piece_data[to] << std::endl;
        this->piece_data[to] = piece + (!this->turn * 8);
        this->piece_data[from] = EMPTY;
    }

    else if (type & CASTLE) {
        // std::cout << "Here" << std::endl;
        int side = type >> 3; // 1 is kingside 0 is queenside
        cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
        int color_to_pos = (!this->turn * 56);

        BB t = (side ? 0b1111ULL : 0b10111000ULL) << color_to_pos;

        // starting positions
        int rookStart = (!side * 7 + color_to_pos);
        int kingStart = 3 + color_to_pos;
        int rookEnd = 2 + (!side << 1) + color_to_pos;
        int kingEnd = 1 + (!side << 2) + color_to_pos; // x << 1 == x * 2
        // int kingEnd = 6 - (piece * 4) + (!this->turn * 56);
        
        fast_reverse_bit(this->pieces[ROOK], rookStart); // off
        fast_reverse_bit(this->pieces[KING], kingStart); // if its black add 56
        fast_reverse_bit(this->pieces[ROOK], rookEnd); // on x << 1 == x * 2
        fast_reverse_bit(this->pieces[KING], kingEnd);
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t; // we do not have any conditions because castling can not remove a piece
        this->piece_data[rookEnd] = this->piece_data[rookStart];
        this->piece_data[kingEnd] = this->piece_data[kingStart];
        this->piece_data[rookStart] = EMPTY;
        this->piece_data[kingStart] = EMPTY;
    }

    else if (type == EN_PESSANT) {
        fifty_move_rule = 0;
        // int N = this->enpessent_history.size();
        int enpessant = cur_history.en_pessant;
        // std::cout << enpessant << std::endl;
        // print_vector(this->enpessent_history);
        // std::cout << enpessent << std::endl;
        // std::cout << this->enpessent_history[N-1] << "\t" << this->enpessent_history[N-2] << std::endl;
        BB t = SQUARE_TO_BB[from] ^ SQUARE_TO_BB[to];
        BB pawn_mask = t | SQUARE_TO_BB[enpessant];
        // print_BB((BB)(1ULL << from));
        // std::cout << to << std::endl;
        // print_BB(1ULL << to);
        this->pieces[PAWN] ^= pawn_mask;
        // std::cout << same_team << std::endl;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= pawn_mask;
        this->pieces[EMPTY] ^= pawn_mask;
        fast_reverse_bit(this->pieces[turn_to_index[!turn]], enpessant);
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        this->piece_data[enpessant] = EMPTY;
        cur_en_pessant = enpessant;
    }

    else {
        std::cout << "something went wrong..." << std::endl;
        print_move_fancy(move);
    }

    // std::cout << cur_en_pessant << std::endl;
    if (fifty_move_rule == 50) {
        this->state = DRAW;
    }
    new_history.fifty_move_rule = fifty_move_rule;
    new_history.castle = cur_castle;
    new_history.en_pessant = cur_en_pessant;
    history.push_back(new_history);
    // this->update_bitboards();
    this->next_turn();
}

void Board::undo_move(const Move &move) noexcept {
    this->next_turn(); // technically previous turn
    History old_history = this->history.back();
    this->history.pop_back();
    int cur_en_pessant = old_history.en_pessant;
    int capture = old_history.capture; 
    this->state = RUNNING;
    int same_team = turn_to_index[this->turn];
    int other_team = turn_to_index[!this->turn];
    unsigned int from = move.from;
    unsigned int to = move.to;
    unsigned int type = move.type;

    if (type == NORMAL_MOVE) {
        BB capture_place = SQUARE_TO_BB[to];
        BB t = (SQUARE_TO_BB[from]) ^ capture_place;
        this->pieces[this->piece_data[to] & 7] ^= t;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;
        if (capture) {
            // print_BB(capture_place);
            this->pieces[FULL] ^= capture_place;
            this->pieces[EMPTY] ^= capture_place;
            this->pieces[other_team] ^= capture_place;
            this->pieces[capture & 7] ^= capture_place;
        }
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[to] = capture;
    }
    else if (type & PROMOTION) {
        BB full_mask = (SQUARE_TO_BB[from] | SQUARE_TO_BB[to]);

        this->pieces[PAWN] ^= SQUARE_TO_BB[from];
        this->pieces[same_team] ^= full_mask;
        this->pieces[type >> 1] ^= SQUARE_TO_BB[to];
        this->pieces[FULL] ^= full_mask;
        this->pieces[EMPTY] ^= full_mask;
        // std::cout << capture << std::endl;
        if (capture) {
            // std::cout << "here" << std::endl;
            // print_BB(this->pieces[other_team]);
            this->pieces[EMPTY] ^= SQUARE_TO_BB[to];
            this->pieces[FULL] ^= SQUARE_TO_BB[to];
            this->pieces[other_team] ^= SQUARE_TO_BB[to];
            // print_BB(this->pieces[other_team]);
            // std::cout << ((capture & 7) == BISHOP) << " " << ((capture & 7) == QUEEN) << " " << QUEEN << " " << ((capture & 7)) << std::endl;
            this->pieces[capture & 7] ^= SQUARE_TO_BB[to];
        }
        // std::cout << to << std::endl;
        this->piece_data[from] = PAWN + (!this->turn*8);
        this->piece_data[to] = capture;
    }
    else if (type == EN_PESSANT) {
        BB t = (SQUARE_TO_BB[from]) ^ (SQUARE_TO_BB[to]);
        BB pawn_mask = t | SQUARE_TO_BB[cur_en_pessant];
        // BB c = SQUARE_TO_BB[cur_en_pessant];
        // std::cout << cur_en_pessant << std::endl;
        this->piece_data[cur_en_pessant] = PAWN + (this->turn*8);
        // std::cout << (PAWN + !this->turn ? 0 : 8) << std::endl;
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[to] = EMPTY;
        this->pieces[PAWN] ^= pawn_mask;
        this->pieces[FULL] ^= pawn_mask;
        this->pieces[EMPTY] ^= pawn_mask;
        this->pieces[same_team] ^= t;
        this->pieces[other_team] ^= SQUARE_TO_BB[cur_en_pessant];
    }
    else if (type & CASTLE) {
        // std::cout << "here" << std::endl;
        bool is_queenside_castle = !(type >> 3);
        // std::cout << "is queenside castle:\t" << is_queenside_castle << std::endl;
        int color_to_pos = !this->turn * 56;
        BB t = (is_queenside_castle ? 0b10111000ULL : 0b1111ULL) << (color_to_pos); // checking if it is a queenside or kingside castle
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;

        int rookStart = (is_queenside_castle) ? (7 + (color_to_pos)) : (color_to_pos); // (piece * 7) + (color_to_pos)
        // int kingStart = 3 + (color_to_pos);
        int rookEnd = 2 + (is_queenside_castle << 1) + (color_to_pos);
        // int kingEnd = 1 + (is_queenside_castle << 2) + (color_to_pos); // x << 1 == x * 2
        // std::cout << "kingstart:\t" << kingStart; 
        fast_reverse_bit(this->pieces[ROOK], rookStart); // off
        fast_reverse_bit(this->pieces[KING], from);
        fast_reverse_bit(this->pieces[ROOK], rookEnd);
        fast_reverse_bit(this->pieces[KING], to);
        this->piece_data[rookStart] = this->piece_data[rookEnd];
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[rookEnd] = EMPTY;
        this->piece_data[to] = EMPTY;
    }

    // this->update_bitboards();
}