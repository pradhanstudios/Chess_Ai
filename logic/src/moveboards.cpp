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
    int j;

    for (i = 0; i < 64; i++) {
        rook_blockerboards = blocker_boards(rook_masks[i]);
        bishop_blockerboards = blocker_boards(bishop_masks[i]);
        for (j = 0; j < (1 << real_count(rook_masks[i])); j++) {
            rook_moves[i][j] = moveboard_rook(rook_blockerboards[j], i);
            bishop_moves[i][j] = moveboard_bishop(bishop_blockerboards[j], i);
        }
    }
    // print_BB(bishop_masks[32]);
    
    // std::cout << rook_masks_t[12] << std::endl;
    // struct _Moves {rook_masks_t, bishop_masks_t, rook_moves_t, bishop_moves_t};
    // m.ROOK_MASKS = rook_masks_t;
    // m.BISHOP_MASKS = bishop_masks_t;
    // m.ROOK_MOVES = rook_moves_t;
    // m.BISHOP_MOVES = bishop_moves_t;
    // return m;
}