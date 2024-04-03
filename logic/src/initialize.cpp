#include "initialize.hpp"

BB _in_between(int sq1, int sq2) { // I copied from https://www.chessprogramming.org/Square_Attacked_By#Pure_Calculation
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


void initialize_engine() {
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
}