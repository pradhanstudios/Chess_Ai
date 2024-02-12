/*
-------------------------------
--skip to 230 for actual code--
-------------------------------
*/
#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

// #define RSEED 42069

typedef uint64_t BB; // short for Bitboard

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

BB shift_back(BB bitboard, int back)
{
    return back >= 0 ? (bitboard << back) : (bitboard >> -back);
}

void set_bit_on(BB &bitboard, int index)
{
    bitboard |= shift_back(1ULL, index);
}

void set_bit_off(BB &bitboard, int index)
{
    bitboard &= ~shift_back(1ULL, index);
}

BB get_bit(BB bitboard, int index)
{
    return bitboard & shift_back(1ULL, index);
}

int zeroes_end(BB bitboard)
{
    return __builtin_clzll(bitboard);
}

int zeroes_start(BB bitboard)
{
    return __builtin_ctzll(bitboard);
}

int count_bits(BB bitboard)
{
    return __builtin_popcount(bitboard);
}

int real_count(BB bitboard) {
    int count;
    for (count = 0; bitboard != 0; count++, bitboard &= bitboard-1);
    return count;
}

bool is_within_board(int pos)
{
    return pos >= 0 && pos < 64;
}

void print_BB(BB bitboard)
{
    for (int i = 63; i >= 0; i--)
    {
        int cur = bitboard >> i;

        if ((i + 1) % 8 == 0)
        {
            std::cout << std::endl;
        }

        if (cur & 1)
        {
            std::cout << "1 ";
        }

        else
        {
            std::cout << "0 ";
        }
    }
    std::cout << std::endl;
}

BB blocker_mask_rook(int position)
{
    BB blocker_mask_up = H_FILE << (position % 8);
    blocker_mask_up &= ~(RANK_1 | RANK_8);
    BB blocker_mask_side = RANK_1 << int(position / 8) * 8;
    blocker_mask_side &= ~(A_FILE | H_FILE);
    BB blocker_mask = blocker_mask_up | blocker_mask_side;
    // print_BB(blocker_mask);
    set_bit_off(blocker_mask, position);

    return blocker_mask;
}

BB blocker_mask_bishop(int position)
{
    BB bitboard;
    int i = position;
    while ((i % 8 != 0) && (i < 56))
    {
        set_bit_on(bitboard, i);
        i += 7;
    }
    i = position;
    while ((i % 8 != 0) && (i > 7))
    {
        set_bit_on(bitboard, i);
        i -= 9;
    }
    i = position;
    while ((i > 7) && (i % 8 != 7))
    {
        set_bit_on(bitboard, i);
        i -= 7;
    }
    i = position;
    while ((i < 56) && (i % 8 != 7))
    {
        set_bit_on(bitboard, i);
        i += 9;
    }
    set_bit_off(bitboard, position);
    return bitboard;
}

std::array<BB, 4096> blocker_boards(BB blocker_mask)
{
    std::vector<int> poss;
    BB noedge = blocker_mask;

    for (int i = 0; i < 64; i++)
    {
        if (get_bit(noedge, i))
        {
            poss.push_back(i);
        }
    }
    // std::cout << poss.size();
    int max_num = (1 << poss.size()) - 1;

    std::array<BB, 4096> output;
    BB cur_BB = 0ULL;
    int i = 0;
    while (i <= max_num)
    {
        cur_BB = 0ULL;
        for (int j = 0; j < poss.size(); j++)
        {
            if (i & (1 << j))
            {
                set_bit_on(cur_BB, poss[j]);
                // std::cout << "here" << std::endl;
            }
        }
        // std::cout << cur_BB << std::endl;
        output[i] = cur_BB;

        i++;
    }
    // std::cout << output[20] << std::endl;

    return output;
}

BB moveboard_bishop(BB blocker_board, int position)
{
    BB moveboard = 0ULL;

    for (int dir : {9, 7, -7, -9})
    {
        /*
        A = 9
        B = 7
        C = -9
        D = -7
        A0B
        0X0
        D0C
        */
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir == (7 || -9)) ? 7 : 0)) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}

BB moveboard_rook(BB blocker_board, int position)
{
    BB moveboard = 0ULL;

    for (int dir : {8, -8, 1, -1})
    {
        /*
        A = -1
        B = 1
        C = 8
        D = -8
        0C0
        AXB
        0D0
        */
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir < 0)) ? 7 : 0) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}

int calculate_index(BB input_bitboard, BB magic, int shift) {
    BB out = input_bitboard;
    out *= magic;
    out >>= shift;
    return out;
}

BB _random_BB_helper()
{
    return random() | (random() << 32);
}

BB random_BB()
{
    return _random_BB_helper() & _random_BB_helper() & _random_BB_helper();
}

BB find_magic_num(int pos, int piece)
{ // rook: 0 bishop: 1
    BB mask, magic;
    mask = piece ? blocker_mask_bishop(pos) : blocker_mask_rook(pos);
    // std::cout << mask << std::endl;
    // int s = 1 << real_count(mask);
    std::map<int, BB> used;
    std::array<BB, 4096> blockerboard, moveboard;
    blockerboard = blocker_boards(mask);
    int size = 1 << real_count(mask);
    // std::cout << blockerboard[20] << std::endl;
    for (int i = 0; i < size; i++)
    {
        moveboard[i] = piece ? moveboard_bishop(blockerboard[i], pos) : moveboard_rook(blockerboard[i], pos);
    }
    int ismagic, j; 
    // std::cout << size << std::endl;
    int tmax;
    BB best;
    BB min = INFINITY;
    // std::cout << "";
    for (long k = 0; k < 65535; k++){
        used = {};
        ismagic = 1;
        tmax = 0;
        magic = random_BB();
        // std::cout << magic << std::endl;
        for (int i = 0; (i < size) && ismagic; i++) {
            j = calculate_index(blockerboard[i], magic, 64-real_count(magic));
            if (j > 10000) {
                ismagic = 0;
                break;
            }
            // std::cout << j << std::endl;
            tmax = std::max(tmax, j);
            if (used.find(j) == used.end()) {
                used[j] = moveboard[i];
            }
            else if (used[j] != moveboard[i]) {
                ismagic = 0;
                // break;
            }
        }
        if (ismagic && (tmax < min)) {
            // std::cout << "got here " << k << std::endl;
            best = magic;
            // // std::cout << best << '\n';
            min = tmax;
            // return magic;
        }

    }
    // std::cout << best;
    return best;
}

int main()
{
    int i;
    BB r_magics[64], b_magics[64];
    // std::cout << "ROOK MAGIC NUMBERS{\n";
    // for (i = 0; i < 64; i++) {
    //     r_magics[i] = find_magic_num(i, 0);
    //     std::cout << r_magics[i] << ", ";
    //     // std::cout.flush();
    // }
    // std::cout << "}\nROOK SHIFT NUMBERS\n{" << std::endl;
    // // std::cout.flush();
    // for (BB magic : r_magics) {
    //     std::cout << 64-real_count(magic) << ", ";
    // }
    // srand(RSEED);
    std::cout << "}\nBISHOP MAGIC NUMBERS\n{";
    std::cout.flush();
    for (i = 0; i < 64; i++) {
        b_magics[i] = find_magic_num(i, 1);
        std::cout << b_magics[i] << ", ";
        std::cout.flush();
  }
    std::cout << "}BISHOP SHIFT NUMBERS\n{";
    std::cout.flush();

    for (BB magic : b_magics) {
        std::cout << 64-real_count(magic) << ", ";
        std::cout.flush();
    }
    std::cout << "}" << std::endl;
    std::cout.flush();
    // BB mask = blocker_mask_rook(0);
    // std::array<BB, 4096> b = blocker_boards(mask);
    // print_BB(mask);
    // std::cout << b[1];
    // std::cout << find_magic_num(25, 1) << std::endl;

    return 0;
}