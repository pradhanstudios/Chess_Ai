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

void print_vector(const std::vector<int> &v) noexcept {
    for (int i : v) {
        std::cout << i << ",  ";
    }
    std::cout << std::endl;
}

uint64_t prandom() {
    return random() | (random() << 32);
}

void print_vector(const std::vector<std::string> &v) noexcept {
    for (std::string i : v) {
        std::cout << i << ",  ";
    }
    std::cout << std::endl;
} 


std::vector<std::string> split(const std::string &s, const char delim) noexcept {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

History::History(const unsigned int en_pessant, const unsigned int castle, const unsigned int capture, const unsigned int fifty_move_rule) {
    this->en_pessant = en_pessant;
    this->castle = castle;
    this->capture = capture;
    this->fifty_move_rule = fifty_move_rule;
}

Zobrist::Zobrist() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 64; j++) {
            this->pieces[i][j] = prandom();
        }
    }

    for (int i = 0; i < 16; i++) {
        this->castles[i] = prandom();
    }

    for (int i = 0; i < 64; i++) {
        this->en_pessants[i] = prandom();
    }

    this->black_turn = prandom();
}

Board::Board(const std::string &fen) noexcept {
    std::fill(this->pieces.begin(), this->pieces.end(), 0ULL);
    
    History cur_history = History();
    unsigned int castles = 0u;
    
    std::vector<std::string> fensplit = split(fen, ' ');
    
    if (fensplit[3][0] != '-'){
        cur_history.en_pessant = (7-col_letter_to_num.at(fensplit[3][0]) + (std::stoi(&fensplit[3][1])-1)*8);
    }
    else {
        cur_history.en_pessant = 0u; // you can never have an en pessant at position 0
    }

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
    }
    cur_history.castle = castles;
    cur_history.fifty_move_rule = 0u;


    this->turn = fensplit[1][0] == 'w';
    zobrist_key = this->turn ? 0 : zobrist.black_turn;

    cur_history.en_pessant -= PAWN_DIRECTION_LOOKUP[this->turn];

    this->history.push_back(cur_history);

    int color;
    int board_ptr = 0u;
    char cur;
    std::vector<std::string> fen_lines = split(fensplit[0], '/');
    std::reverse(fen_lines.begin(), fen_lines.end());
    int row = 0;
    
    for (std::string line : fen_lines) {
        board_ptr = 8*row;
        for (int i = line.length()-1; i > -1; i--) {
            cur = line[i];
            if (isdigit(cur))
            {
                board_ptr += cur - '0'; // convert char to int
            }
            else if (cur == '/')
            {
                break;
            }
            else
            {
                color = isupper(cur) ? 0 : 8;
                color ? toggle_bit(this->pieces[BLACK], board_ptr) : toggle_bit(this->pieces[WHITE], board_ptr);
                cur = char(std::tolower(cur));
                if (cur == 'k') {
                    toggle_bit(this->pieces[KING], board_ptr);
                    this->piece_data[board_ptr] = KING + color;
                }
                else if (cur == 'q') {
                    toggle_bit(this->pieces[QUEEN], board_ptr);
                    this->piece_data[board_ptr] = QUEEN + color;
                }
                else if (cur == 'r') {
                    toggle_bit(this->pieces[ROOK], board_ptr);
                    this->piece_data[board_ptr] = ROOK + color;
                }
                else if (cur == 'b') {
                    toggle_bit(this->pieces[BISHOP], board_ptr);
                    this->piece_data[board_ptr] = BISHOP + color;
                }
                else if (cur == 'n') {
                    toggle_bit(this->pieces[KNIGHT], board_ptr);
                    this->piece_data[board_ptr] = KNIGHT + color;
                }
                else if (cur == 'p') {
                    toggle_bit(this->pieces[PAWN], board_ptr);
                    this->piece_data[board_ptr] = PAWN + color;
                }
                board_ptr++;
            }
        }
        row++;
    }

    this->update_bitboards();


    BB piecesfull = this->pieces[FULL];
    int pos;
    while (piecesfull) {
        pos = pop_lsb(piecesfull);
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[pos])-1][pos];
    }

    this->zobrist_key ^= zobrist.castles[this->history.back().castle];
}

void Board::print_square_data() const noexcept {
    std::string cur;
    int entry, cur_piece;
    for (int i = 63; i > -1; i--) {
        if(((i + 1) & 7) == 0) {
            std::cout << "\n";
        }
        entry = this->piece_data[i];
        cur_piece = no_color(entry);
        cur = "";

        if (cur_piece == EMPTY) {
            cur = " 0";
        } else {
            cur += (entry & BLACK) ? "B" : "W";

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
}

void Board::play_move(const Move move) noexcept {
    // variable initialization
    unsigned int cur_en_pessant = 0u;
    int same_team = TURN_TO_INDEX_LOOKUP[this->turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!this->turn];
    unsigned int from = move.from;
    unsigned int to = move.to;
    unsigned int type = move.type;
    History cur_history = this->history.back();
    unsigned int cur_castle = cur_history.castle;
    unsigned int fifty_move_rule = cur_history.fifty_move_rule + 1;
    History new_history = History();
    new_history.capture = this->piece_data[to];

    if (type == NORMAL_MOVE) { // normal move
        int piece = no_color(this->piece_data[from]);
        this->zobrist_key ^= zobrist.pieces[piece-1][from];
        BB capture = SQUARE_TO_BB[to];
        BB t = capture ^ (SQUARE_TO_BB[from]);
        this->pieces[piece] ^= t;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;
        if (no_color((this->piece_data[to])) == ROOK) { // rook is captured
            this->zobrist_key ^= zobrist.castles[cur_castle];
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
            this->zobrist_key ^= zobrist.castles[cur_castle];
        }

        if (this->piece_data[to]) { // capture
            fifty_move_rule = 0;
            this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
            this->pieces[EMPTY] ^= capture;
            this->pieces[FULL] ^= capture;
            this->pieces[other_team] ^= capture;
            this->pieces[no_color(this->piece_data[to])] ^= capture;
        }

        this->piece_data[to] = this->piece_data[from];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
        this->piece_data[from] = EMPTY;
        if (no_color((this->piece_data[to])) == ROOK) {
            this->zobrist_key ^= zobrist.castles[cur_castle];
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
            this->zobrist_key ^= zobrist.castles[cur_castle];
        }
        else if (no_color((this->piece_data[to])) == PAWN) {
            fifty_move_rule = 0;
            if (abs(from - to) == 16) { // if it moved two rows
                cur_en_pessant = to;
                this->zobrist_key ^= zobrist.en_pessants[cur_en_pessant];
            }
        }
        else if (no_color((this->piece_data[to])) == KING) {
            this->zobrist_key ^= zobrist.castles[cur_castle];
            cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
            this->zobrist_key ^= zobrist.castles[cur_castle];
        }
    }

    else if (type & PROMOTION) {
        fifty_move_rule = 0;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[from])-1][from];
        int piece = type >> 1; // promotion piece

        toggle_bit(this->pieces[PAWN], from); // off
        toggle_bit(this->pieces[same_team], from);
        toggle_bit(this->pieces[same_team], to); // on
        toggle_bit(this->pieces[piece], to);
        toggle_bit(this->pieces[FULL], from);
        toggle_bit(this->pieces[FULL], to);
        toggle_bit(this->pieces[EMPTY], from);
        toggle_bit(this->pieces[EMPTY], to);

        if (no_color((this->piece_data[to])) == ROOK) { // rook is captured
            this->zobrist_key ^= zobrist.castles[cur_castle];
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
            this->zobrist_key ^= zobrist.castles[cur_castle];
        }
        if (piece_data[to]) { // capture
            this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
            toggle_bit(this->pieces[EMPTY], to);
            toggle_bit(this->pieces[FULL], to);
            toggle_bit(this->pieces[other_team], to);
            toggle_bit(this->pieces[no_color(piece_data[to])], to);
        }
        
        this->piece_data[to] = piece + (!this->turn * 8);
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
        this->piece_data[from] = EMPTY;
    }

    else if (type & CASTLE) {
        int side = type >> 3; // 1 is kingside 0 is queenside
        this->zobrist_key ^= zobrist.castles[cur_castle];
        cur_castle &= 0b0011 ^ (0b1111 * this->turn); // if u castle once u cant castle again
        this->zobrist_key ^= zobrist.castles[cur_castle];
        int color_to_pos = (!this->turn * 56);

        BB t = (side ? 0b1111ULL : 0b10111000ULL) << color_to_pos;

        // starting positions
        int rookStart = (!side * 7 + color_to_pos);
        int kingStart = 3 + color_to_pos;
        int rookEnd = 2 + (!side << 1) + color_to_pos;
        int kingEnd = 1 + (!side << 2) + color_to_pos; // x << 1 == x * 2
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[rookStart])-1][rookStart];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[kingStart])-1][kingStart];
        
        toggle_bit(this->pieces[ROOK], rookStart); // off
        toggle_bit(this->pieces[KING], kingStart);
        toggle_bit(this->pieces[ROOK], rookEnd); // on
        toggle_bit(this->pieces[KING], kingEnd);
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t; // we do not have any conditions because castling can not remove a piece
        this->piece_data[rookEnd] = this->piece_data[rookStart];
        this->piece_data[kingEnd] = this->piece_data[kingStart];
        this->piece_data[rookStart] = EMPTY;
        this->piece_data[kingStart] = EMPTY;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[rookEnd])-1][rookEnd];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[kingEnd])-1][kingEnd];
    }

    else if (type == EN_PESSANT) {
        fifty_move_rule = 0;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[from])-1][from];
        int enpessant = cur_history.en_pessant;
        BB t = SQUARE_TO_BB[from] ^ SQUARE_TO_BB[to];
        BB pawn_mask = t | SQUARE_TO_BB[enpessant];
        this->pieces[PAWN] ^= pawn_mask;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= pawn_mask;
        this->pieces[EMPTY] ^= pawn_mask;
        toggle_bit(this->pieces[other_team], enpessant);
        this->piece_data[to] = this->piece_data[from];
        this->piece_data[from] = EMPTY;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[enpessant])-1][enpessant];
        this->piece_data[enpessant] = EMPTY;
        cur_en_pessant = enpessant;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
    }

    if (fifty_move_rule == 50) {
        this->state = DRAW;
    }

    int white_knights = count_bits(this->pieces[KNIGHT] & this->pieces[WHITE]);
    int black_knights = count_bits(this->pieces[KNIGHT] & this->pieces[BLACK]);
    int white_bishops = count_bits(this->pieces[BISHOP] & this->pieces[WHITE]);
    int black_bishops = count_bits(this->pieces[BISHOP] & this->pieces[BLACK]);
    int non_minor_pieces = count_bits(
        this->pieces[PAWN] |
        this->pieces[ROOK] |
        this->pieces[QUEEN]
    );
    bool is_draw = 
    !non_minor_pieces && 
    (
        (white_knights <= 1 && black_knights <= 1 && white_bishops <= 1 && black_bishops <= 1) || // not enough pieces on board
        // white can checkmate
        (white_knights >= 1 && white_bishops >= 1) || // bishop and knight
        (white_bishops >= 2) || // 2 bishops
        // black can checkmate
        (black_knights >= 1 && black_bishops >= 1) ||
        (black_knights >= 2)
    );

    if (is_draw) {
        this->state = DRAW;
    }

    new_history.fifty_move_rule = fifty_move_rule;
    new_history.castle = cur_castle;
    new_history.en_pessant = cur_en_pessant;
    history.push_back(new_history);
    this->next_turn();
}

void Board::undo_move(const Move move) noexcept {
    // variable initialization
    this->next_turn(); // technically previous turn
    History old_history = this->history.back();
    this->zobrist_key ^= zobrist.castles[old_history.castle];
    this->history.pop_back();
    int cur_en_pessant = old_history.en_pessant;
    this->zobrist_key ^= zobrist.en_pessants[cur_en_pessant];
    int capture = old_history.capture; 
    this->state = RUNNING;
    int same_team = TURN_TO_INDEX_LOOKUP[this->turn];
    int other_team = TURN_TO_INDEX_LOOKUP[!this->turn];
    unsigned int from = move.from;
    unsigned int to = move.to;
    unsigned int type = move.type;

    if (type == NORMAL_MOVE) {
        BB capture_place = SQUARE_TO_BB[to];
        BB t = (SQUARE_TO_BB[from]) ^ capture_place;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
        this->pieces[no_color(this->piece_data[to])] ^= t;
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;
        if (capture) {
            this->pieces[FULL] ^= capture_place;
            this->pieces[EMPTY] ^= capture_place;
            this->pieces[other_team] ^= capture_place;
           this->pieces[no_color(capture)] ^= capture_place;
            this->zobrist_key ^= zobrist.pieces[no_color(capture)-1][to];
        }

        this->piece_data[from] = this->piece_data[to];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[from])-1][from];
        this->piece_data[to] = capture;
    }

    else if (type & PROMOTION) {
        BB full_mask = (SQUARE_TO_BB[from] | SQUARE_TO_BB[to]);

        toggle_bit(this->pieces[PAWN], from);
        this->pieces[same_team] ^= full_mask;
        toggle_bit(this->pieces[type >> 1], to);
        this->pieces[FULL] ^= full_mask;
        this->pieces[EMPTY] ^= full_mask;
        if (capture) {
            toggle_bit(this->pieces[EMPTY], to);
            toggle_bit(this->pieces[FULL], to);
            toggle_bit(this->pieces[other_team], to);
            toggle_bit(this->pieces[no_color(capture)], to);
            this->zobrist_key ^= zobrist.pieces[no_color(capture)-1][to];
        }

        this->piece_data[from] = PAWN + (!this->turn*8);
        this->piece_data[to] = capture;
    }

    else if (type == EN_PESSANT) {
        BB t = (SQUARE_TO_BB[from]) ^ (SQUARE_TO_BB[to]);
        BB pawn_mask = t | SQUARE_TO_BB[cur_en_pessant];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
        this->piece_data[cur_en_pessant] = PAWN + (this->turn*8);
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[cur_en_pessant])-1][cur_en_pessant];
        this->piece_data[from] = this->piece_data[to];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[from])-1][from];
        this->piece_data[to] = EMPTY;
        this->pieces[PAWN] ^= pawn_mask;
        this->pieces[FULL] ^= pawn_mask;
        this->pieces[EMPTY] ^= pawn_mask;
        this->pieces[same_team] ^= t;
        toggle_bit(this->pieces[other_team], cur_en_pessant);
    }

    else if (type & CASTLE) {
        bool is_queenside_castle = !(type >> 3);
        int color_to_pos = !this->turn * 56;
        BB t = (is_queenside_castle ? 0b10111000ULL : 0b1111ULL) << (color_to_pos); // checking if it is a queenside or kingside castle
        this->pieces[same_team] ^= t;
        this->pieces[FULL] ^= t;
        this->pieces[EMPTY] ^= t;

        int rookStart = (is_queenside_castle) ? (7 + (color_to_pos)) : (color_to_pos);
        int rookEnd = 2 + (is_queenside_castle << 1) + (color_to_pos);
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[rookEnd])-1][rookEnd];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[to])-1][to];
        toggle_bit(this->pieces[ROOK], rookStart); // off
        toggle_bit(this->pieces[KING], from);
        toggle_bit(this->pieces[ROOK], rookEnd);
        toggle_bit(this->pieces[KING], to);
        this->piece_data[rookStart] = this->piece_data[rookEnd];
        this->piece_data[from] = this->piece_data[to];
        this->piece_data[rookEnd] = EMPTY;
        this->piece_data[to] = EMPTY;
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[from])-1][from];
        this->zobrist_key ^= zobrist.pieces[no_color(this->piece_data[rookStart])-1][rookStart];
    }
}