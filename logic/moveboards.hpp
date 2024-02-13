#pragma once
#include "constants.hpp"

// similar to implementation from https://github.com/Usama-Azad/Pickle-in-Cpp
template<typename T>
void _load_masks(std::string filename, T &object) {
    std::ifstream file(filename, std::ios::binary);
    file.read((char *)(&(object[0])), sizeof(object));
    file.close();
}

template<typename T>
void _load_moves(std::string filename, T &object) {
    std::ifstream file(filename, std::ios::binary);
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < object[i].size(); j++) {       
            file.read((char *)(&(object[i][j])), sizeof(object[i][j]));
        }
    }
    file.close();
}

std::array<BB, 64> ROOK_MASKS, BISHOP_MASKS;
std::array<std::array<BB, 4096>, 64> ROOK_MOVES, BISHOP_MOVES;

void init_moveboards() {
    _load_masks<std::array<BB, 64>>("moves/ROOK_MASKS.bin", ROOK_MASKS);
    _load_masks<std::array<BB, 64>>("moves/BISHOP_MASKS.bin", BISHOP_MASKS);
    _load_moves<std::array<std::array<BB, 4096>, 64>>("moves/ROOK_MOVES.bin", ROOK_MOVES);
    _load_moves<std::array<std::array<BB, 4096>, 64>>("moves/BISHOP_MOVES.bin", BISHOP_MOVES);
}