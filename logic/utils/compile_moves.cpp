#include "../sliding_moves.hpp"
#include <iostream>

const std::string FOLDER_PATH = "../moves/";

// similar to implementation from https://github.com/Usama-Azad/Pickle-in-Cpp

template<typename T>
void save(T object, std::string filename) {
    std::ofstream file(FOLDER_PATH + filename, std::ios::binary);
    file.write((const char *)(&object), sizeof(object));
    file.close();
}

int main() {

    std::array<BB, 64> rook_masks, bishop_masks;
    int i;
    for (i = 0; i < 64; i++) {
        rook_masks[i] = blocker_mask_rook(i);
        bishop_masks[i] = blocker_mask_bishop(i);
    }

    save<std::array<BB, 64>>(rook_masks, "ROOK_MASKS.bin");
    save<std::array<BB, 64>>(bishop_masks, "BISHOP_MASKS.bin");

    std::array<std::array<BB, 4096>, 64> rook_moves, bishop_moves;
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
    
    save<std::array<std::array<BB, 4096>, 64>>(rook_moves, "ROOK_MOVES.bin");
    save<std::array<std::array<BB, 4096>, 64>>(bishop_moves, "BISHOP_MOVES.bin");

    return 0;
}