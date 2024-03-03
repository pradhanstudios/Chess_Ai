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
#include "sliding_moves.hpp"
#include "constants.hpp"
#include "magic_values.hpp"

// int calculate_index(BB input_bitboard, BB magic, int shift) {
//     BB out = input_bitboard;
//     out *= magic;
//     out >>= shift;
//     return out;
// }

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
    // int tmax;
    // BB best;
    // BB min = INFINITY;
    // std::cout << "";
    for (long k = 0; k < 480000; k++){
        used = {};
        ismagic = 1;
        // tmax = 0;
        magic = random_BB();
        // std::cout << magic << std::endl;
        for (int i = 0; (i < size) && ismagic; i++) {
            j = calculate_index(blockerboard[i], magic, 64-real_count(magic));
            if (j > size) {
                ismagic = 0;
                break;
            }
            // std::cout << j << std::endl;
            // tmax = std::max(tmax, j);
            if (used.find(j) == used.end()) {
                used[j] = moveboard[i];
            }
            else if (used[j] != moveboard[i]) {
                ismagic = 0;
                // break;
            }
        }
        if (ismagic) {
            return magic;
        }

    }
    return 0;
    // std::cout << best;
    // return best;
}

int main()
{
    int i;
    BB r_magics[64], b_magics[64];
    std::cout << "ROOK MAGIC NUMBERS{\n"; 
    for (i = 0; i < 64; i++) {
        r_magics[i] = find_magic_num(i, 0);
        std::cout << r_magics[i] << ", ";
        // std::cout.flush();
    }
    std::cout << "}\nROOK SHIFT NUMBERS\n{" << std::endl;
    // std::cout.flush();
    for (BB magic : r_magics) {
        std::cout << 64-real_count(magic) << ", ";
    }
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