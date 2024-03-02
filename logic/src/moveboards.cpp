#include "moveboards.hpp"

void init_moveboards(std::array<BB, 64> &rook_masks, std::array<BB, 64> &bishop_masks, std::array<std::array<BB, 4096>, 64> &rook_moves, std::array<std::array<BB, 4096>, 64> &bishop_moves) { // litterally as soon as I try to store the arrays somewhere else, I get a segfault
    // _Moves m;
    // std::array<BB, 64> rook_masks, bishop_masks;
    int i;
    for (i = 0; i < 64; i++) {
        // std::cout << i << std::endl;
        rook_masks[i] = blocker_mask_rook(i);
        bishop_masks[i] = blocker_mask_bishop(i);
    }

    // save<std::array<BB, 64>>(rook_masks, "ROOK_MASKS.bin");
    // save<std::array<BB, 64>>(bishop_masks, "BISHOP_MASKS.bin");

    // std::array<std::array<BB, 4096>, 64> rook_moves, bishop_moves;
    std::array<BB, 4096> rook_blockerboards, bishop_blockerboards;
    // std::array<std::array<BB, 4096>, 64> rook_blockerboards, bishop_blockerboards;
    int j;

    for (i = 0; i < 64; i++) {
        // std::cout << "Big loop " << i << std::endl;
        rook_blockerboards = blocker_boards(rook_masks[i]);
        bishop_blockerboards = blocker_boards(bishop_masks[i]);

        // if (rook_blockerboards[5] == 0) {
        //     std::cout << i << " ";
        // }
        // print_BB(bishop_blockerboards[2]);

        for (j = 0; j < (1 << real_count(rook_masks[i])) && j < 4096; j++) {
            rook_moves[i][j] = moveboard_rook(rook_blockerboards[j], i);
        }
        // if (i == 5) {
        //     j = 0;
        //     for (BB b : rook_moves[5]) {
        //         if (b == 0) {
        //             std::cout << j << ", ";
        //             j++;
        //         }
        //     }
        //     std::cout << std::endl;
        //     // print_BB(rook_blockerboards[34]);
        // }
        // std::cout << "\tFirst loop passed" << std::endl;

        for (j = 0; j < (1 << real_count(bishop_masks[i])) && j < 4096; j++) {
            bishop_moves[i][j] = moveboard_bishop(bishop_blockerboards[j], i);
        }
        // print_BB(bishop_moves[i][3]);
        // std::cout << "\tSecond loop passed" << std::endl;
    }
}