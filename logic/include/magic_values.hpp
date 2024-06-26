#ifndef MAGIC_VALUES_HPP
#define MAGIC_VALUES_HPP

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
} magic_structure;

inline std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;

void load_magics();

constexpr BB get_sliding_moves(BB current_position, BB friendlies, const magic_structure &magic) noexcept {
    return magic.attacks[((current_position & magic.mask) * magic.magic) >> magic.shift] & ~friendlies;
}

constexpr BB bishop_moves(BB current_position, BB friendlies, int pos) noexcept {
    return get_sliding_moves(current_position, friendlies, BISHOP_MAGICS[pos]);
}

constexpr BB rook_moves(BB current_position, BB friendlies, int pos) noexcept {
    return get_sliding_moves(current_position, friendlies, ROOK_MAGICS[pos]);
}

constexpr BB queen_moves(BB current_position, BB friendlies, int pos) noexcept {
    return bishop_moves(current_position, friendlies, pos) | rook_moves(current_position, friendlies, pos);
}
// extern std::array<magic_structure, 64> ROOK_MAGICS;
// extern const std::array<BB, 64> ROOK_SHIFTS;
// extern std::array<magic_structure, 64> BISHOP_MAGICS;
// extern std::array<BB, 64> BISHOP_SHIFTS;
// BB random_BB_helper();
// BB random_BB();
// int calculate_index(BB input_bitboard, BB magic, int shift);
// magic_structure _find_magics_rook(int pos);
// magic_structure _find_magics_bishop(int pos);
// void _write_magic_structure(magic_structure structure, std::ofstream &file);
// void generate_magics_and_save();
#endif // MAGIC_VALUES_HPP