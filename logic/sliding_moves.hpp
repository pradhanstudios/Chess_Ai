#pragma once

#include "constants.hpp"
#include "bit_operations.hpp"

BB blocker_mask_rook(int position)
{
    BB blocker_mask_up = H_FILE << (position % 8);
    blocker_mask_up &= ~(RANK_1 | RANK_8);
    BB blocker_mask_side = RANK_1 << int(position / 8) * 8;
    blocker_mask_side &= ~(A_FILE | H_FILE);
    BB blocker_mask = blocker_mask_up | blocker_mask_side;
    // print_BB(blocker_mask);
    set_bit_off(blocker_mask, position);

    return blocker_mask;
}

BB blocker_mask_bishop(int position)
{
    BB bitboard;
    int i = position;
    while ((i % 8 != 0) && (i < 56))
    {
        set_bit_on(bitboard, i);
        i += 7;
    }
    i = position;
    while ((i % 8 != 0) && (i > 7))
    {
        set_bit_on(bitboard, i);
        i -= 9;
    }
    i = position;
    while ((i > 7) && (i % 8 != 7))
    {
        set_bit_on(bitboard, i);
        i -= 7;
    }
    i = position;
    while ((i < 56) && (i % 8 != 7))
    {
        set_bit_on(bitboard, i);
        i += 9;
    }
    set_bit_off(bitboard, position);
    return bitboard;
}

std::array<BB, 4096> blocker_boards(BB blocker_mask)
{
    std::vector<int> poss;
    BB noedge = blocker_mask;

    for (int i = 0; i < 64; i++)
    {
        if (get_bit(noedge, i))
        {
            poss.push_back(i);
        }
    }
    // std::cout << poss.size();
    int max_num = (1 << poss.size()) - 1;

    std::array<BB, 4096> output;
    BB cur_BB = 0ULL;
    int i = 0;
    while (i <= max_num)
    {
        cur_BB = 0ULL;
        for (int j = 0; j < poss.size(); j++)
        {
            if (i & (1 << j))
            {
                set_bit_on(cur_BB, poss[j]);
                // std::cout << "here" << std::endl;
            }
        }
        // std::cout << cur_BB << std::endl;
        output[i] = cur_BB;

        i++;
    }
    // std::cout << output[20] << std::endl;

    return output;
}

BB moveboard_bishop(BB blocker_board, int position)
{
    BB moveboard = 0ULL;

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
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir == (7 || -9)) ? 7 : 0)) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}

BB moveboard_rook(BB blocker_board, int position)
{
    BB moveboard = 0ULL;

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
        int i = position + dir;

        while (is_within_board(i) && (i % 8 != ((dir < 0)) ? 7 : 0) && (!get_bit(blocker_board, i)))
        {
            set_bit_on(moveboard, i);
            i += dir;
        }
    }

    set_bit_off(moveboard, position);
    return moveboard;
}