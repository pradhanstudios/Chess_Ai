#include "bit_operations.hpp"

void print_BB(const BB bitboard) {
    int cur;
    for (int i = 63; i >= 0; i--) {
        cur = bitboard >> i;

        if ((i+1) % 8 == 0) {
            std::cout << std::endl;
        }

        if (cur & 1) {
            std::cout << "1 ";
        }

        else {
            std::cout << "0 ";
        }
    }
    std::cout << std::endl;
}