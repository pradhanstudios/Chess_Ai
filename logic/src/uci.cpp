#include "uci.hpp"

const std::array<char, 8> num_to_col = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const std::array<char, 6> num_to_piece = {'F', 'F', 'n', 'b', 'r', 'q'};

std::string move_to_uci(const Move &move) {
    int from = move.from;
    int to = move.to;
    int type = move.type;
    std::string output = "";
    output += num_to_col[7 - (from & 7)];
    output += std::to_string(int(from / 8) + 1);
    output += num_to_col[7 - (to & 7)];
    output += std::to_string(int(to / 8) + 1);
    if (type & PROMOTION) {
        output += num_to_piece[type >> 1];
    }
    // else if (type & CASTLE) {
    //     output += (type >> 3) ? 'k' : 'q';
    // }
    return output;
}

Move uci_to_move(const std::string &uci, const Board &chess_board) {
    // std::cout << (8*(std::stoi(&uci[3]) - 1)) << std::endl;
    int from = (7 - col_letter_to_num.at(uci[0])) + 8*(std::stoi(&uci[1]) - 1);
    int to = (7 - col_letter_to_num.at(uci[2])) + 8*(std::stoi(&uci[3]) - 1);
    int piece = no_color(chess_board.piece_data[from]);
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
        int castle_side = ((from - to) > 0) ? 1 : 0;
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

// Move san_to_move(const std::string &san, const Board &chess_board) {
//     Move new_move = Move();
//     if (san == "O-O") { // short castle
//         int color_to_position = !chess_board.turn * 56;
//         new_move.from = 3 + color_to_position;
//         new_move.to = 1 + color_to_position;
//         return new_move;
//     }

//     else if (san == "O-O-O") { // long castle
//         int color_to_position = !chess_board.turn * 56;
//         new_move.from = 3 + color_to_position;
//         new_move.to = 5 + color_to_position;
//         return new_move;
//     }

//     else if (san[0] == 'N') { // knight

//     }
// };