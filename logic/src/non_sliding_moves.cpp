#include "non_sliding_moves.hpp"

BB pawn_moves(BB pawns_to_move, BB opposite_team, BB empties, int direction) {

    BB single_moves = shift_back(pawns_to_move, direction) & empties;

    BB starting_rank = direction < 0 ? RANK_7 : RANK_2;

    BB double_moves = shift_back(shift_back(pawns_to_move & starting_rank & ~(opposite_team | empties), direction) & empties, direction);

    BB captures = (shift_back(pawns_to_move, direction+1) | shift_back(pawns_to_move, direction-1)) & opposite_team;

    return single_moves | double_moves | captures;
}

BB knight_moves(BB knights_to_move, BB same_team) {
    /*
    A>>6;B<<6;C>>10;D<<10;E>>15;F<<15;G>>17;H<<17;
    00H0F000
    0D000B00
    00010000
    0A000C00
    00E0G000
    00000000
    00000000
    00000000
    */
//    print_BB((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) << 15);
//    print_BB(same_team);

    BB legal_moves = 0ULL;

    BB not_same_team = ~same_team;
    // https://www.chess.com/analysis?tab=analysis
    BB moves =
            ((knights_to_move & ~(RANK_1 | A_FILE | B_FILE)) >> 6) |
            ((knights_to_move & ~(RANK_1 | G_FILE | H_FILE)) >> 10) |
            ((knights_to_move & ~(RANK_8 | G_FILE | H_FILE)) << 6) |
            ((knights_to_move & ~(RANK_8 | A_FILE | B_FILE)) << 10) | 
            ((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) >> 15) | 
            ((knights_to_move & ~(RANK_1 | RANK_2 | H_FILE)) >> 17) |
            ((knights_to_move & ~(RANK_8 | RANK_7 | A_FILE)) << 15) |
            ((knights_to_move & ~(RANK_8 | RANK_7 | H_FILE)) << 17);
    
    legal_moves = moves & not_same_team;

    return legal_moves;
}