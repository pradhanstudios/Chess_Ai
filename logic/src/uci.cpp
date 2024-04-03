#include "uci.hpp"

const std::array<char, 8> num_to_col = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const std::array<char, 6> num_to_piece = {'F', 'F', 'n', 'b', 'r', 'q'};

std::string move_to_uci(Move move) {
    int from, to, type;
    from = move.from;
    to = move.to;
    type = move.type;
    std::string output = "";
    output += num_to_col[7 - (from & 7)];
    output += std::to_string(int(from / 8) + 1);
    output += num_to_col[7 - (to & 7)];
    output += std::to_string(int(to / 8) + 1);
    if (type & PROMOTION) {
        output += num_to_piece[type >> 1];
    }
    else if (type & CASTLE) {
        output += (type >> 3) ? 'k' : 'q';
    }
    return output;
}

Move uci_to_move(std::string uci, Board chess_board) {
    int from, to, piece;
    // std::cout << (8*(std::stoi(&uci[3]) - 1)) << std::endl;
    from = (7 - col_letter_to_num.at(uci[0])) + 8*(std::stoi(&uci[1]) - 1);
    to = (7 - col_letter_to_num.at(uci[2])) + 8*(std::stoi(&uci[3]) - 1);
    piece = no_color(chess_board.piece_data[from]);
    char back = uci.back();
    if (!isdigit(back)) { // promotion
        int promotion_piece = 0;
        switch (back) {
        case 'n':
            promotion_piece = KNIGHT;
            break;
            
        case 'b':
            promotion_piece = BISHOP;
            break;

        case 'r':
            promotion_piece = ROOK;
            break;

        case 'q':
            promotion_piece = QUEEN;
            break;
        }
        return Move(from, to, PROMOTION, promotion_piece);
    }

    else if (piece == KING && abs(from - to) == 2) { // king has moved two spaces = castle
        int castle_side = (from - to) > 0 ? 1 : 0;
        return Move(from, to, CASTLE, EMPTY, castle_side);
    }
    // std::cout << "here";
    else if (piece == PAWN && chess_board.piece_data[to] == EMPTY && ((from & 7) != (to & 7)) /*capture*/) { // pawn capture and the to is nothing, which means en pessant
        // std::cout << "here" << std::endl;
        return Move(from, to, EN_PESSANT);
    }
    // std::cout << (piece == PAWN) << std::endl;

    return Move(from, to, NORMAL_MOVE);

}