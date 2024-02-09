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

const BB EDGES = A_FILE | H_FILE | RANK_1 | RANK_8;

const BB TOPLEFTTORIGHT = 9241421688590303745ULL;
const BB BOTLEFTTORIGHT = 72624976668147840ULL;

// const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"; // starting fen
const std::string starting_fen = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";

// bitwise functions

BB shift_back(BB bitboard, int back) {
    return back >= 0 ? (bitboard << back): (bitboard >> -back);
}

void set_bit_on(BB &bitboard, int index) {
    bitboard |= shift_back(1ULL, index);
}

void set_bit_off(BB &bitboard, int index) {
    bitboard &= ~shift_back(1ULL, index);
}

BB get_bit(BB bitboard, int index) {
    return bitboard & shift_back(1ULL, index);
}

int zeroes_end(BB bitboard) {
    return __builtin_clzll(bitboard);
}

int zeroes_start(BB bitboard) {
    return __builtin_ctzll(bitboard);
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
            std::cout << "1 ";
        }

        else {
            std::cout << "0 ";
        }
    }
    std::cout << std::endl;
}

BB show_index(int index) {
    BB output = 0ULL;
    set_bit_on(output, index);
    return output;
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



// direction should be 8 (white), or -8 (black) for
BB pawn_moves(BB pawns_to_move, BB opposite_team, int direction) { // TODO: add en passent

    BB single_moves = shift_back(pawns_to_move, direction) & empties;

    BB starting_rank = direction <= 0 ? RANK_7 : RANK_2;

    BB double_moves = shift_back(shift_back(pawns_to_move & starting_rank & ~(opposite_team | empties), direction) & empties, direction);

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
   print_BB((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) << 15);
   print_BB(same_team);
    
    return (((knights_to_move & ~(RANK_8 | G_FILE | H_FILE)) << 6) |
    ((knights_to_move & ~(RANK_1 | A_FILE | B_FILE)) >> 6) |
    ((knights_to_move & ~(RANK_1 | G_FILE | H_FILE)) >> 10) |
    ((knights_to_move & ~(RANK_8 | A_FILE | B_FILE)) << 10) | 
    ((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) << 15) | 
    ((knights_to_move & ~(RANK_8 | RANK_7 | A_FILE)) >> 15) |
    ((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) << 17) |
    ((knights_to_move & ~(RANK_8 | RANK_7 | A_FILE)) >> 17)) & ~same_team;
}

BB blocker_mask_rook(int position) {
    BB blocker_mask = H_FILE << (position % 8);
    blocker_mask |= RANK_1 << int(position / 8) * 8;
    // print_BB(blocker_mask);
    set_bit_off(blocker_mask, position);

    return blocker_mask & ~EDGES;
}

BB blocker_mask_bishop(int position) {
    BB bitboard;
    int i = position; 
    while ((i % 8 != 0) && (i < 56)) {
        set_bit_on(bitboard, i);
        i += 7;
    }
    i = position;
    while ((i % 8 != 0) && (i > 7)) {
        set_bit_on(bitboard, i);
        i -= 9;
    }
    i = position;
    while ((i > 7) && (i % 8 != 7)) {
        set_bit_on(bitboard, i);
        i -= 7;
    }
    i = position;
    while ((i < 56) && (i % 8 != 7)) {
        set_bit_on(bitboard, i);
        i += 9;
    }
    set_bit_off(bitboard, position);
    return bitboard;
}

std::vector<BB> blocker_boards(BB blocker_mask) {
    std::vector<int> poss;
    BB noedge = blocker_mask & ~EDGES;

    for (int i = 0; i < 64; i++) {
        if (get_bit(noedge, i)) {
            poss.push_back(i);
        }
    }

    int max_num = (1 << poss.size()) - 1;
    std::vector<int> nums;
    for (int i = 0; i <= max_num; i++) {
        nums.push_back(i);
    }

    std::vector<BB> output;

    for (int i = 0; i < nums.size(); i++) {
        int cur = nums[i];
        BB cur_BB = 0ULL;
        for (int i = 0; i < poss.size(); i++) {
            if (cur & (1 << i)) {
                set_bit_on(cur_BB, poss[i]);
            }
        }

        output.push_back(cur_BB);

    }

    return output;
}

BB moveboard(BB blocker_mask, BB blocker_board, int position) {
    BB moveboard;
    // top
    moveboard = blocker_mask & blocker_board;
    BB t = moveboard >> (position + 1);
    int first = zeroes_start(t) + 1;
    t = (1 << first) - 1;
    moveboard |= (t << (position + 1)) & blocker_mask;



    return moveboard;
}


int main() {
    // open_fen(starting_fen);
    // std::vector<BB> blocker_board = blocker_boards(blocker_mask_rook(23));
    // for (int i = 0; i < blocker_board.size(); i++) {
    //     print_BB(blocker_board[i]);
    // }
    print_BB(blocker_mask_bishop(23));
    return 0;
}