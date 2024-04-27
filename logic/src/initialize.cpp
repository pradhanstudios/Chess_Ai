#include "initialize.hpp"

BB _in_between(const int &sq1, const int &sq2) { // I copied from https://www.chessprogramming.org/Square_Attacked_By#Pure_Calculation
   const BB a2a7 = BB(0x0001010101010100);
   const BB b2g7 = BB(0x0040201008040200);
   const BB h1b7 = BB(0x0002040810204080); 
   BB btwn, line, rank, file;

   btwn  = (MAX_VALUE << sq1) ^ (MAX_VALUE << sq2);
   file  =   (sq2 & 7) - (sq1   & 7);
   rank  =  ((sq2 | 7) -  sq1) >> 3 ;
   line  =      (   (file  &  7) - 1) & a2a7; /* a2a7 if same file */
   line += 2 * ((   (rank  &  7) - 1) >> 58); /* b1g1 if same rank */
   line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
   line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
   line *= btwn & -btwn; /* mul acts like shift by smaller square */
   return line & btwn;   /* return the bits on that line in-between */
}


void initialize_engine() noexcept {
    srand(6942042069LL);
    load_magics();
    for (int i = 0; i < 64; i++) {
        SQUARE_TO_BB[i] = 1ULL << i;
    }
    ENGINE_INITIALIZED = true;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            in_between_lookup[i*64 + j] = _in_between(i, j);
        }
    }

    int direction;
    for (int i = 0; i < 64; i++) {
        
        for (int j = 0; j < 2; j++) {
            direction = -8 + 16*(j); /*-8 for black 8 for white*/ 
            PAWN_MOVES[j][i] = (shift_up(SQUARE_TO_BB[i], direction));
        }

        for (int j = 2; j < 4; j++) {
            direction = -8 + 16*(j-2); /*-8 for black 8 for white*/ 
            PAWN_MOVES[j][i] = (shift_up(SQUARE_TO_BB[i] & ~A_FILE, direction+1) | shift_up(SQUARE_TO_BB[i] & ~H_FILE, direction-1));
        }
        KNIGHT_MOVES[i] = (
        ((SQUARE_TO_BB[i] & ~(RANK_1 | A_FILE | B_FILE)) >> 6) |
        ((SQUARE_TO_BB[i] & ~(RANK_1 | G_FILE | H_FILE)) >> 10) |
        ((SQUARE_TO_BB[i] & ~(RANK_8 | G_FILE | H_FILE)) << 6) |
        ((SQUARE_TO_BB[i] & ~(RANK_8 | A_FILE | B_FILE)) << 10) | 
        ((SQUARE_TO_BB[i] & ~(RANK_1 | RANK_2 | A_FILE)) >> 15) | 
        ((SQUARE_TO_BB[i] & ~(RANK_1 | RANK_2 | H_FILE)) >> 17) |
        ((SQUARE_TO_BB[i] & ~(RANK_8 | RANK_7 | H_FILE)) << 15) |
        ((SQUARE_TO_BB[i] & ~(RANK_8 | RANK_7 | A_FILE)) << 17)
        );
        
        KING_MOVES[i] = (
        ((SQUARE_TO_BB[i] & ~(RANK_8 | A_FILE)) << 9) |
        ((SQUARE_TO_BB[i] & ~(RANK_8)) << 8) |
        ((SQUARE_TO_BB[i] & ~(RANK_8 | H_FILE)) << 7) |
        ((SQUARE_TO_BB[i] & ~(A_FILE)) << 1) |
        ((SQUARE_TO_BB[i] & ~(H_FILE)) >> 1) |
        ((SQUARE_TO_BB[i] & ~(RANK_1 | A_FILE)) >> 7) |
        ((SQUARE_TO_BB[i] & ~(RANK_1)) >> 8) |
        ((SQUARE_TO_BB[i] & ~(RANK_1 | H_FILE)) >> 9)
        );
    }
}