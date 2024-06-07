#include "sliding_moves.hpp"

BB blocker_mask_rook(const int position)
{
    BB blocker_mask_up = H_FILE << (position % 8);
    blocker_mask_up &= ~(RANK_1 | RANK_8);
    BB blocker_mask_side = RANK_1 << int(position / 8) * 8;
    blocker_mask_side &= ~(A_FILE | H_FILE);
    BB blocker_mask = blocker_mask_up | blocker_mask_side;
    set_bit_off(blocker_mask, position);

    return blocker_mask;
}

BB blocker_mask_bishop(const int position)
{
    BB bitboard = 0ULL;
    int i;

    for (int dir : {9, 7, -7, -9}) {
        i = position + dir;

        while (is_within_board(i) && ((i % 8) != (((dir == 7) || (dir == -9)) ? 7 : 0))) {
            set_bit_on(bitboard, i);
            i += dir;
        }
    }

    set_bit_off(bitboard, position);
    return bitboard & ~EDGES;
}

std::array<BB, 4096> blocker_boards(const BB &blocker_mask)
{
    std::vector<int> poss;

    for (int i = 0; i < 64; i++) {
        if (get_bit(blocker_mask, i)) {
            poss.push_back(i);
        }
    }
    int max_num = std::min(1 << poss.size(), 4096);

    std::array<BB, 4096> output;
    BB cur_BB = 0ULL;
    int i = 0;
    while (i < max_num)
    {
        cur_BB = 0ULL;
        for (int j = 0; j < poss.size(); j++)
        {
            if (i & (1 << j))
            {
                set_bit_on(cur_BB, poss[j]);
            }
        }
        output[i] = cur_BB;

        i++;
    }

    return output;
}

BB moveboard_bishop(const BB &blocker_board, const int position)
{
    BB moveboard = 0ULL;
    int i;

    for (int dir : {9, 7, -7, -9})
    {
        /*
        A = 9
        B = 7
        C = -9
        D = -7
        A0B
        0X0
        D0C
        */
        i = position + dir;

        while (is_within_board(i) && (i % 8 != (((dir == 7) || (dir == -9)) ? 7 : 0)) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
        if (get_bit(blocker_board, i)) {
            set_bit_on(moveboard, i);
        }
    }

    return moveboard;
}

BB moveboard_rook(const BB &blocker_board, const int position)
{
    BB moveboard = 0ULL;
    int i, bad_rank;

    for (int dir : {8, -8, 1, -1})
    {
        /*
        A = -1
        B = 1
        C = 8
        D = -8
        0C0
        AXB
        0D0
        */
        i = position + dir;
        bad_rank = -1;
        if (dir == -1) {
            bad_rank = 7;
        }
        else if (dir == 1) {
            bad_rank = 0;
        }


        while (is_within_board(i) && ((i % 8) != bad_rank) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
        if (get_bit(blocker_board, i)) {
            set_bit_on(moveboard, i);
        }
    }

    return moveboard;
}