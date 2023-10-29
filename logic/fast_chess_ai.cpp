#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>

typedef uint64_t BB; // short for Bitboard


// constants
const BB A_FILE = 0x8080808080808080ULL; // represent a file
const BB B_FILE = A_FILE >> 1;
const BB C_FILE = A_FILE >> 2;
const BB D_FILE = A_FILE >> 3;
const BB E_FILE = A_FILE >> 4;
const BB F_FILE = A_FILE >> 5;
const BB G_FILE = A_FILE >> 6;
const BB H_FILE = A_FILE >> 7;

const BB RANK_1 = 0x00000000000000FFULL; // represent first rank
const BB RANK_2 = RANK_1 << 8;
const BB RANK_3 = RANK_1 << 16;
const BB RANK_4 = RANK_1 << 24;
const BB RANK_5 = RANK_1 << 32;
const BB RANK_6 = RANK_1 << 40;
const BB RANK_7 = RANK_1 << 48;
const BB RANK_8 = RANK_1 << 56;

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

BB shift_back(BB bitboard, int back) {
    return back >= 0 ? (bitboard << back): (bitboard >> -back);
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

void print_BB(BB bitboard) {
    for (int i = 63; i >= 0; i--) {
        int cur = bitboard >> i;

        if ((i+1) % 8 == 0) {
            std::cout << std::endl;
        }

        if (cur & 1) {
            std::cout << "1";
        }

        else {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void open_fen(std::string fen) {
    char cur;
    int board_ptr = 0;
    for (int i = fen.length(); i > 0; i--)
    {
        cur = fen[i-1];

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



// direction should be 8 (down), or -8 (up) for
BB pawn_moves(BB pawns_to_move, BB opposite_team, int direction) { // TODO: add en passent

    BB single_moves = shift_back(pawns_to_move, direction) & empties;

    BB double_moves = shift_back(single_moves, direction) & empties;

    BB captures = (shift_back(pawns_to_move, direction+1) | shift_back(pawns_to_move, direction-1)) & opposite_team;

    return single_moves | double_moves | captures;
}

BB knight_moves(BB knights_to_move, BB same_team) {
    /*
    A>>6;B<<6;C>>10;D<<10;E>>15;F<<15;G>>17;H<<17;
    00H0F000
    0D000B00
    00010000
    0A000C00
    00E0G000
    00000000
    00000000
    00000000
    */
    
    return (((knights_to_move << 6) & ~(RANK_8 | G_FILE | H_FILE)) |
    ((knights_to_move >> 6) & ~(RANK_1 | A_FILE | B_FILE)) | 
    ((knights_to_move << 10) & ~(RANK_1 | A_FILE | B_FILE)) |
    ((knights_to_move >> 10) & ~(RANK_8 | A_FILE | B_FILE)) |
    ((knights_to_move << 15) & ~(RANK_1 | RANK_2 | A_FILE)) |
    ((knights_to_move >> 15) & ~(RANK_8 | RANK_7 | H_FILE)) |
    ((knights_to_move << 17) & ~(RANK_1 | RANK_2 | H_FILE)) | 
    ((knights_to_move >> 17) & ~(RANK_8 | RANK_7 | A_FILE))) & ~same_team;
}

int main() {
    open_fen(starting_fen);
    // print_BB(knight_moves(knights & black, black));
    print_BB(knight_moves(white & knights, white));
    return 0;
}