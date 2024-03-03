#pragma once

#include "constants.hpp"
#include "sliding_moves.hpp"

// this is using a fixed shift fancy magic bitboard technique
// https://www.chessprogramming.org/Magic_Bitboards

// inspired from https://github.com/Tearth/Fast-Magic-Bitboards/blob/master/FastMagicBitboards/Include/MagicStructure.hpp
typedef struct magic_structure {
    BB mask;
    BB magic;
    std::array<BB, 4096> attacks;
    int shift;

    // magic_structure(int size) : attacks(new BB[size]) {}
} magic_structure;

// extern const std::array<BB, 64> ROOK_MAGICS;
// extern const std::array<BB, 64> ROOK_SHIFTS;
// extern const std::array<BB, 64> BISHOP_MAGICS;
// extern const std::array<BB, 64> BISHOP_SHIFTS;
// extern std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
BB random_BB_helper();
BB random_BB();
int calculate_index(BB input_bitboard, BB magic, int shift);
magic_structure _find_magics_rook(int pos);
magic_structure _find_magics_bishop(int pos);
void generate_magics_and_save();
// void _save(std::array<magic_structure, 64> array, std::string filename);
void load_magics(std::array<magic_structure, 64> &ROOK_MAGICS, std::array<magic_structure, 64> &BISHOP_MAGICS);
BB get_sliding_moves(BB current_position, BB friendlies, int pos, magic_structure magic);
