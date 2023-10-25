#include <stdint.h>
#include <string>
#include <iostream>

typedef uint64_t BB; // short for Bitboard


// constants
const BB A_FILE = 0x8080808080808080ULL; // represent a file
const BB RANK_1 = 0xFF00000000000000ULL; // represent first rank
const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"; // starting fen

// bitwise functions
void set_bit_on(BB &bitboard, int index) {
    bitboard |= 1ULL << index;
}

void set_bit_off(BB &bitboard, int index) {
    bitboard &= ~(1ULL << index);
}

BB get_bit(BB bitboard, int index) {
    return bitboard & (1ULL << index);
}

// bitboards
BB empties;
BB white;
BB black;
BB kings;
BB queens;
BB rooks;
BB bishops;
BB knights;
BB pawns;

void open_fen(std::string fen) {
    char cur;
    int board_ptr = 0;
    for (int i = 0; i < fen.length(); i++)
    {
        cur = fen[i];

        if (isdigit(cur))
        {
            // skip squares
            board_ptr += std::stoi(&cur);
        }
        else if (cur == '/')
        {
            // next row
            continue;
        }
        else
        {
            // // place piece
            // board[board_ptr] = (islower(cur) ? Black : White) | (fen_to_piece[std::tolower(cur)]);
            // board_ptr++;
            isupper(cur) ? set_bit_on(white, board_ptr) : set_bit_on(black, board_ptr);
            cur = char(std::tolower(cur));
            if (cur == 'k') {
                set_bit_on(kings, board_ptr);
            }
            else if (cur == 'q') {
                set_bit_on(queens, board_ptr);
            }
            else if (cur == 'r') {
                set_bit_on(rooks, board_ptr);
            }
            else if (cur == 'b') {
                set_bit_on(bishops, board_ptr);
            }
            else if (cur == 'n') {
                set_bit_on(knights, board_ptr);
            }
            else if (cur == 'p') {
                set_bit_on(pawns, board_ptr);
            }

            board_ptr++;
        }
    }
    empties = ~(white | black);
}

int main() {
    open_fen(starting_fen);
    std::cout << empties << std::endl;
    return 0;
}