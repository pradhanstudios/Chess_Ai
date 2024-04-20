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

    // magic_structure(int size) : attacks(new BB[size]) {}
} magic_structure;

/*
mask
magic
{all values for attacks}
shift
\
repeat
*/

// extern std::array<magic_structure, 64> ROOK_MAGICS;
// // extern const std::array<BB, 64> ROOK_SHIFTS;
// extern std::array<magic_structure, 64> BISHOP_MAGICS;
// extern std::array<BB, 64> BISHOP_SHIFTS;
inline std::array<magic_structure, 64> ROOK_MAGICS, BISHOP_MAGICS;
// BB random_BB_helper();
// BB random_BB();
// int calculate_index(BB input_bitboard, BB magic, int shift);
// magic_structure _find_magics_rook(int pos);
// magic_structure _find_magics_bishop(int pos);
// // void _write_magic_structure(magic_structure structure, std::ofstream &file);
// void generate_magics_and_save();
// void _save(std::array<magic_structure, 64> array, std::string filename);
void load_magics();
constexpr BB get_sliding_moves(const BB &current_position, const BB &friendlies, const magic_structure &magic) noexcept {
    // fast_reverse_bit(current_position, pos); // get rid of the piece itself
    // for (BB b : magic.attacks) {
    //     if (b) {
    //         return b;
    //     }
    // }
    // print_BB(current_position);
    // print_BB((current_position ^ (1 << pos)));
    return magic.attacks[((current_position & magic.mask) * magic.magic) >> magic.shift] & ~friendlies;
}
#endif // MAGIC_VALUES_HPP