#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef unsigned long long BB;

enum {
    A_FILE = 0x8080808080808080ULL, // represent a file
    B_FILE = A_FILE >> 1,
    C_FILE = A_FILE >> 2,
    D_FILE = A_FILE >> 3,
    E_FILE = A_FILE >> 4,
    F_FILE = A_FILE >> 5,
    G_FILE = A_FILE >> 6,
    H_FILE = A_FILE >> 7,
};

enum {
    RANK_1 = 0x00000000000000FFULL, // represent first rank
    RANK_2 = RANK_1 << 8,
    RANK_3 = RANK_1 << 16,
    RANK_4 = RANK_1 << 24,
    RANK_5 = RANK_1 << 32,
    RANK_6 = RANK_1 << 40,
    RANK_7 = RANK_1 << 48,
    RANK_8 = RANK_1 << 56,
};

static inline int file(const int pos) {
    return pos & 7; // last three bits
}

static inline int rank(const int pos) {
    return pos >> 3; // first three bits
}

// bitboard operations
#ifndef __GNUC__
const int debruijn_index[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

const BB debruijn_constant = 0x03f79d71b4cb0a89ULL;
#endif

static inline int ls1b(BB bitboard) {
    #ifndef __GNUC__
    return debruijn_index[((bitboard & -bitboard) * debruijn_constant) >> 58];
    #else
    return __builtin_ctzll(bitboard);
    #endif
}

static inline int pop_lsb(BB *bitboard) {
    int least_significant_one_bit = ls1b(*bitboard);
    *bitboard &= *bitboard - 1;
    return least_significant_one_bit;
}

static inline void set(BB *bitboard, int square) {
    *bitboard |= 1ULL << square;
}

static inline void clear(BB *bitboard, int square) {
    *bitboard &= ~(1ULL << square);
}

static inline void flip(BB *bitboard, int square) {
    *bitboard ^= 1ULL << square;
}

static inline int get_bit(BB bitboard, int square) {
    return bitboard & (1ULL << square);
}

static inline int bbcount(BB bitboard) {
    #ifndef __GNUC__
    int count = 0;
    for (; bitboard; count++) bitboard &= bitboard - 1;
    return count;
    #else
    return __builtin_popcountll(bitboard);
    #endif
}

void print_BB(BB bitboard) {
    int cur;
    for (int i = 63; i >= 0; i--) {
        cur = bitboard >> i;

        if ((i+1) % 8 == 0) {
            printf("\n");
        }

        if (cur & 1) {
            printf("1 ");
        }

        else {
            printf("0 ");
        }
    }
    printf("\n");
}

// move generation
// pins (might be used later)
// BB _in_between_lookup[64*64];
// BB _in_between(const int pos1, const int pos2) { // I copied from https://www.chessprogramming.org/Square_Attacked_By#Pure_Calculation
//    const BB a2a7 = 0x0001010101010100ULL;
//    const BB b2g7 = 0x0040201008040200ULL;
//    const BB h1b7 = 0x0002040810204080ULL; 
//    BB btwn, line, rank, file;

//    btwn  = (MAX_VALUE << pos1) ^ (MAX_VALUE << pos2);
//    file  =   (pos2 & 7) - (pos1   & 7);
//    rank  =  ((pos2 | 7) -  pos1) >> 3 ;
//    line  =      (   (file  &  7) - 1) & a2a7; /* a2a7 if same file */
//    line += 2 * ((   (rank  &  7) - 1) >> 58); /* b1g1 if same rank */
//    line += (((rank - file) & 15) - 1) & b2g7; /* b2g7 if same diagonal */
//    line += (((rank + file) & 15) - 1) & h1b7; /* h1b7 if same antidiag */
//    line *= btwn & -btwn; /* mul acts like shift by smaller square */
//    return line & btwn;   /* return the bits on that line in-between */
// }

// static inline BB in_between(const int pos1, const int pos2) {
//     return _in_between_lookup[(pos1 << 5) + pos2];
// }

// static inline void _in_between_init() {
//     for (int i = 0; i < 64; i++) for (int j = 0; j < 64; j++) _in_between_lookup[(i << 5) + j] = _in_between(i, j);
// }


// https://www.chessprogramming.org/Kogge-Stone_Algorithm#Occluded_Fill
// kogge stone is decently fast, and it is way easier to set up than magic bitboards
static inline BB south_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move >>  8);
    empties &=       (empties >>  8);
    pieces_to_move |= empties & (pieces_to_move >> 16);
    empties &=       (empties >> 16);
    pieces_to_move |= empties & (pieces_to_move >> 32);
    return pieces_to_move | (pieces_to_move >> 8);
}

static inline BB north_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move <<  8);
    empties &=       (empties <<  8);
    pieces_to_move |= empties & (pieces_to_move << 16);
    empties &=       (empties << 16);
    pieces_to_move |= empties & (pieces_to_move << 32);
    return pieces_to_move | (pieces_to_move << 8);
}

static inline BB west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move << 1);
    empties &=       (empties << 1);
    pieces_to_move |= empties & (pieces_to_move << 2);
    empties &=       (empties << 2);
    pieces_to_move |= empties & (pieces_to_move << 4);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 1);
}

static inline BB north_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  9);
    empties &=       (empties <<  9);
    pieces_to_move |= empties & (pieces_to_move << 18);
    empties &=       (empties << 18);
    pieces_to_move |= empties & (pieces_to_move << 36);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 9);
}

static inline BB south_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  7);
    empties &=       (empties >>  7);
    pieces_to_move |= empties & (pieces_to_move >> 14);
    empties &=       (empties >> 14);
    pieces_to_move |= empties & (pieces_to_move >> 28);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) >> 7);
}

static inline BB east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >> 1);
    empties &=       (empties >> 1);
    pieces_to_move |= empties & (pieces_to_move >> 2);
    empties &=       (empties >> 2);
    pieces_to_move |= empties & (pieces_to_move >> 4);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 1);
}

static inline BB south_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  9);
    empties &=       (empties >>  9);
    pieces_to_move |= empties & (pieces_to_move >> 18);
    empties &=       (empties >> 18);
    pieces_to_move |= empties & (pieces_to_move >> 36);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 9);
}

static inline BB north_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  7);
    empties &=       (empties <<  7);
    pieces_to_move |= empties & (pieces_to_move << 14);
    empties &=       (empties << 14);
    pieces_to_move |= empties & (pieces_to_move << 28);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) << 7);
}

static inline BB pawn_moves_white(BB white_pawns, BB other_team, BB empties) {
    // single pushes
    BB moves = (white_pawns << 8) & empties;
    // double pushes  
    moves |= ((moves & (RANK_3)) << 8) & empties;
    // attacks
    moves |= (
        ((white_pawns & ~(A_FILE)) << 9) |
        ((white_pawns & ~(H_FILE)) << 7)
    ) & other_team;

    return moves;
}

static inline BB pawn_attacks_white(BB white_pawns) {
    return (
        ((white_pawns & ~(A_FILE)) << 9) |
        ((white_pawns & ~(H_FILE)) << 7)
    );
}

static inline BB pawn_moves_black(BB black_pawns, BB other_team, BB empties) {
    // single pushes
    BB moves = (black_pawns >> 8) & empties;
    // double pushes  
    moves |= ((moves & (RANK_6)) >> 8) & empties;
    // attacks
    moves |= (
        ((black_pawns & ~(H_FILE)) >> 9) |
        ((black_pawns & ~(A_FILE)) >> 7)
    ) & other_team;

    return moves;
}

static inline BB pawn_attacks_black(BB black_pawns) {
    return (
        ((black_pawns & ~(H_FILE)) >> 9) |
        ((black_pawns & ~(A_FILE)) >> 7)
    );
}

static inline BB knight_moves(BB knights, BB same_team) {
    return (
        ((knights & ~(RANK_1 | A_FILE | B_FILE)) >> 6) |
        ((knights & ~(RANK_1 | G_FILE | H_FILE)) >> 10) |
        ((knights & ~(RANK_8 | G_FILE | H_FILE)) << 6) |
        ((knights & ~(RANK_8 | A_FILE | B_FILE)) << 10) | 
        ((knights & ~(RANK_1 | RANK_2 | A_FILE)) >> 15) | 
        ((knights & ~(RANK_1 | RANK_2 | H_FILE)) >> 17) |
        ((knights & ~(RANK_8 | RANK_7 | H_FILE)) << 15) |
        ((knights & ~(RANK_8 | RANK_7 | A_FILE)) << 17)
        ) & ~same_team;
}

static inline BB bishop_moves(BB bishops, BB same_team, BB empties) {
    return (
        north_east_ray(bishops, empties) |
        north_west_ray(bishops, empties) |
        south_east_ray(bishops, empties) |
        south_west_ray(bishops, empties)
    ) & ~same_team;
}

static inline BB rook_moves(BB rooks, BB same_team, BB empties) {
    return (
        north_ray(rooks, empties) |
        south_ray(rooks, empties) |
        east_ray(rooks, empties) |
        west_ray(rooks, empties)
    ) & ~same_team;
}

static inline BB queen_moves(BB queens, BB same_team, BB empties) {
    return (
        north_east_ray(queens, empties) |
        north_west_ray(queens, empties) |
        north_ray(queens, empties) |
        south_east_ray(queens, empties) |
        south_west_ray(queens, empties) |
        south_ray(queens, empties) |
        east_ray(queens, empties) |
        west_ray(queens, empties)
    ) & ~same_team;
}

static inline BB king_moves(BB kings, BB same_team, BB other_team_attacks) {
    return (
        (kings & ~(A_FILE)) << 9 |
        (kings & ~(A_FILE)) << 1 |
        (kings & ~(A_FILE)) >> 7 |
        (kings & ~(H_FILE)) << 7 |
        (kings & ~(H_FILE)) >> 1 |
        (kings & ~(H_FILE)) >> 9 |
        (kings            ) << 8 |
        (kings            ) >> 8  
    ) & ~(other_team_attacks | same_team);
}

// passed pawn masks
BB passed_pawn_mask_white[8] = {0xFFFFFFFFFFFFFF00, 0xFFFFFFFFFFFF0000, 0xFFFFFFFFFF000000, 0xFFFFFFFF00000000, 0xFFFFFF0000000000, 0xFFFF000000000000, 0xFF00000000000000, 0x0000000000000000};
BB passed_pawn_mask_black[8] = {0x0000000000000000, 0x00000000000000FF, 0x000000000000FFFF, 0x0000000000FFFFFF, 0x00000000FFFFFFFF, 0x000000FFFFFFFFFF, 0x0000FFFFFFFFFFFF, 0x00FFFFFFFFFFFFFF};


// parsing

FILE *book;
long count = 0L;

void open_book() {
    book = fopen("lichess-big3-resolved.book", "r");
    printf("opened book\n");
    // _in_between_init();
    // printf("move generation data successfully loaded\n");
}

#define DATA_SIZE 7153653
#define BATCH_SIZE 65536

// indices in features array
#define PAWN_PST 0
#define KNIGHT_PST 48
#define BISHOP_PST 112
#define ROOK_PST 176
#define QUEEN_PST 240
#define KING_PST 304
#define KING_RING_ATTACKS 368
#define KING_RING_DEFENSES 374
// RPT = Piece-Rank-Table
#define PASSED_PAWN_RANK_RPT 379
#define PROTECTED_PAWN_RANK_RPT 385
#define THREAT_BY_MINOR_PIECE 391
#define THREAT_BY_ROOK 397
#define PAWN_SHIELD_1_BONUS 403
#define PAWN_SHIELD_2_BONUS 404
#define PAWN_SHIELD_3_BONUS 405
#define ISOLATED_PAWN_PENALTY 406
#define DOUBLED_PAWN_PENALTY 407
#define ROOK_ON_OPEN_FILE_BONUS 408
#define ROOK_ON_SEMI_OPEN_FILE_BONUS 409
#define KING_ON_OPEN_FILE_PENALTY 410
#define KING_ON_SEMI_OPEN_FILE_PENALTY 411
#define BISHOP_PAIR_BONUS 412
#define TEMPO 413
#define BLACK 414
#define FEATURE_SIZE 828

/*
pawn pst 48 /
knight pst 64 /
bishop pst 64 /
rook pst 64 /
queen pst 64 /
king pst 64 /
king ring attacking pieces 6  /
king ring defending pieces 5 /
passed pawn rank rpt 6 /
protected pawn rank rpt 6 /
threat by minor piece 6 /
threat by rook 6 /
pawn shields infront of king 3 /
isolated pawn 1 /
doubled pawn 1 /
rook on open file 1 /
rook on half open file 1 /
king on open file 1 /
king on half open file 1 /
bishop pair 1 /
tempo 1 /
TODO: add more king safety

*/

void parse_batch(float *bitboards, float *phases, float *ys) {
    // printf("%li\n", count);
    // go to the current batch
    fseek(book, count, SEEK_SET);
    // buffer information
    // parse file
    int j = 0;
    char buf[100];
    while (j < BATCH_SIZE && fgets(buf, 100, book)) {
        count += (long)strlen(buf);
        __uint64_t index = j * FEATURE_SIZE;

        int str_ptr = 0, board_ptr = 0, cur_phase = 0, slashes = 0, bishop_count_white = 0, bishop_count_black = 0, king_pos_white = 0, king_pos_black = 0, black_shields_ok = 0, white_shields_ok = 0;

        BB pawns = 0ULL, knights = 0ULL, bishops = 0ULL, rooks = 0ULL, queens = 0ULL, kings = 0ULL, white = 0ULL, black = 0ULL;

        // fen parsing and psts
        while (buf[str_ptr] != ' ' && buf[str_ptr]) {
            char cur = buf[str_ptr++];
            if (isdigit(cur)) {
                board_ptr += cur - '0'; // convert char to int
            }

            else if (cur == '/') {
                board_ptr = ++slashes << 3; // slashes * 8
            }

            else {
                int is_black = (islower(cur) != 0); 
                int black_orient = is_black * 56;
                int board_ptr_rep = board_ptr ^ 63 ^ black_orient;
                int add_bitboard_offset = is_black * BLACK; // if color is black, then add by 384
                cur = tolower(cur);
                if (is_black) set(&black, board_ptr_rep ^ black_orient);
                else set(&white, board_ptr_rep);

                if (cur == 'p') {
                    *(bitboards + index + (board_ptr_rep - 8) + PAWN_PST + add_bitboard_offset) = 1.0f;
                    set(&pawns, board_ptr_rep ^ black_orient);
                }

                else if (cur == 'n') {
                    *(bitboards + index + board_ptr_rep + add_bitboard_offset + KNIGHT_PST) = 1.0f;
                    set(&knights, board_ptr_rep ^ black_orient);
                    cur_phase += 1;
                }

                else if (cur == 'b') {
                    *(bitboards + index + board_ptr_rep + add_bitboard_offset + BISHOP_PST) = 1.0f;
                    set(&bishops, board_ptr_rep ^ black_orient);
                    cur_phase += 1;
                    is_black ? bishop_count_black++ : bishop_count_white++;
                }

                else if (cur == 'r') {
                    *(bitboards + index + board_ptr_rep + add_bitboard_offset + ROOK_PST) = 1.0f;
                    set(&rooks, board_ptr_rep ^ black_orient);
                    cur_phase += 2;
                }

                else if (cur == 'q') {
                    *(bitboards + index + board_ptr_rep + add_bitboard_offset + QUEEN_PST) = 1.0f;
                    set(&queens, board_ptr_rep ^ black_orient);
                    cur_phase += 4;
                }

                else if (cur == 'k') {
                    *(bitboards + index + board_ptr_rep + add_bitboard_offset + KING_PST) = 1.0f;
                    set(&kings, board_ptr_rep ^ black_orient);
                    is_black ? (king_pos_black = board_ptr_rep ^ black_orient) : (king_pos_white = board_ptr_rep);
                }

                board_ptr++;
            }
        }

        BB empties = ~(white | black);

        // features array 

        // bishop pair bonus
        *(bitboards + index + BISHOP_PAIR_BONUS) = (float) (bishop_count_white > 1);
        *(bitboards + index + BISHOP_PAIR_BONUS + BLACK) = (float) (bishop_count_black > 1);

        // pawn shields
        white_shields_ok = king_pos_white != 3 && king_pos_white < 54;
        black_shields_ok = king_pos_black != 59 && king_pos_black > 9;
        BB white_pawns = white & pawns;
        BB black_pawns = black & pawns;

        *(bitboards + index + PAWN_SHIELD_1_BONUS) = (float) (white_shields_ok && (get_bit(white_pawns, king_pos_white+9) != 0));
        *(bitboards + index + PAWN_SHIELD_2_BONUS) = (float) (white_shields_ok && (get_bit(white_pawns, king_pos_white+8) != 0));
        *(bitboards + index + PAWN_SHIELD_3_BONUS) = (float) (white_shields_ok && (get_bit(white_pawns, king_pos_white+7) != 0));
        *(bitboards + index + PAWN_SHIELD_1_BONUS + BLACK) = (float) (black_shields_ok && (get_bit(black_pawns, king_pos_black-7) != 0));
        *(bitboards + index + PAWN_SHIELD_2_BONUS + BLACK) = (float) (black_shields_ok && (get_bit(black_pawns, king_pos_black-8) != 0));
        *(bitboards + index + PAWN_SHIELD_3_BONUS + BLACK) = (float) (black_shields_ok && (get_bit(black_pawns, king_pos_black-9) != 0));
        // more pawn data
        BB white_pawns_c = white_pawns;
        BB black_minor_piece_attacks = knight_moves(black & knights, 0ULL) | bishop_moves(black & bishops, 0ULL, empties);
        BB black_rook_attacks = rook_moves(black & rooks, 0ULL, empties);
        while (white_pawns_c) {
            int pos = pop_lsb(&white_pawns_c);
            int pos_file = file(pos);
            int pos_rank = rank(pos);
            BB sides = ((pos_file < 7) * (H_FILE << (pos_file + 1))) |
                       ((pos_file > 0) * (H_FILE << (pos_file - 1)));
            BB same_file = H_FILE << pos_file;
            // is doubled pawn
            *(bitboards + index + DOUBLED_PAWN_PENALTY) += (float) (((same_file & passed_pawn_mask_white[pos_rank]) & (white_pawns ^ (1ULL << pos))) != 0ULL);
            // is isolated pawn
            *(bitboards + index + ISOLATED_PAWN_PENALTY) += (float)(!(white_pawns & sides));
            // is protected pawn
            *(bitboards + index + PROTECTED_PAWN_RANK_RPT + pos_rank - 1) += (float) ((white_pawns & pawn_attacks_black(1ULL << pos) /*check the opposite direction*/) != 0ULL);
            // is passed pawn
            *(bitboards + index + PASSED_PAWN_RANK_RPT + pos_rank - 1) += (float) !((passed_pawn_mask_white[pos_rank] & (sides | same_file)) & black_pawns);
            // is attacked by minor piece
            int is_not_defended_by_pawn = !(pawn_attacks_black(1ULL << pos) & white_pawns);
            *(bitboards + index + THREAT_BY_MINOR_PIECE) += (float) (((1ULL << pos) & black_minor_piece_attacks) && is_not_defended_by_pawn);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK) += (float) (((1ULL << pos) & black_rook_attacks) && is_not_defended_by_pawn);
        }
        // black
        BB black_pawns_c = black_pawns;
        BB white_minor_piece_attacks = knight_moves(white & knights, 0ULL) | bishop_moves(white & bishops, 0ULL, empties);
        BB white_rook_attacks = rook_moves(white & rooks, 0ULL, empties);
        while (black_pawns_c) {
            int pos = pop_lsb(&black_pawns_c);
            int pos_file = file(pos);
            int pos_rank = rank(pos);
            BB sides = (pos_file < 7) * (H_FILE << (pos_file + 1)) |
                       (pos_file > 0) * (H_FILE << (pos_file - 1));
            BB same_file = H_FILE << pos_file;
            // is doubled pawn
            *(bitboards + index + DOUBLED_PAWN_PENALTY + BLACK) += (float) (((same_file & passed_pawn_mask_black[pos_rank]) & (black_pawns ^ (1ULL << pos))) != 0ULL);
            // is isolated pawn
            *(bitboards + index + ISOLATED_PAWN_PENALTY + BLACK) += (float) !(black_pawns & sides);
            // is protected pawn
            *(bitboards + index + PROTECTED_PAWN_RANK_RPT + (6 - pos_rank) + BLACK) += (float) ((black_pawns & pawn_attacks_white(1ULL << pos) /*check the opposite direction*/) != 0ULL);
            // is passed pawn
            *(bitboards + index + PASSED_PAWN_RANK_RPT + (6 - pos_rank) + BLACK) += (float) !((passed_pawn_mask_black[pos_rank] & (sides | same_file)) & white_pawns);
            // is attacked by minor piece
            int is_not_defended_by_pawn = !(pawn_attacks_white(1ULL << pos) & black_pawns);
            *(bitboards + index + THREAT_BY_MINOR_PIECE + BLACK) += (float) (((1ULL << pos) & white_minor_piece_attacks) && is_not_defended_by_pawn);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + BLACK) += (float) (((1ULL << pos) & white_rook_attacks) && is_not_defended_by_pawn);
        }

        // knight info
        BB white_knights_c = white & knights;
        while (white_knights_c) {
            BB pos = white_knights_c & -white_knights_c;
            white_knights_c &= white_knights_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 1) += (float) ((pos & black_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            int is_not_defended_by_pawn = !(pawn_attacks_black(pos) & white_pawns);
            *(bitboards + index + THREAT_BY_ROOK + 1) += (float) ((pos & black_rook_attacks) && is_not_defended_by_pawn);
        }

        BB black_knights_c = black & knights;
        while (black_knights_c) {
            BB pos = black_knights_c & -black_knights_c;
            black_knights_c &= black_knights_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 1 + BLACK) += (float) ((pos & white_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            int is_not_defended_by_pawn = !(pawn_attacks_white(pos) & black_pawns);
            *(bitboards + index + THREAT_BY_ROOK + 1 + BLACK) += (float) ((pos & white_rook_attacks) && is_not_defended_by_pawn);
        }

        // bishop info
        BB white_bishops_c = white & bishops;
        while (white_bishops_c) {
            BB pos = white_bishops_c & -white_bishops_c;
            white_bishops_c &= white_bishops_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 2) += (float) ((pos & black_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            int is_not_defended_by_pawn = !(pawn_attacks_black(pos) & white_pawns);
            *(bitboards + index + THREAT_BY_ROOK + 2) += (float) ((pos & black_rook_attacks) && is_not_defended_by_pawn);
        }


        BB black_bishops_c = black & bishops;
        while (black_bishops_c) {
            BB pos = black_bishops_c & -black_bishops_c;
            black_bishops_c &= black_bishops_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 2 + BLACK) += (float) ((pos & white_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            int is_not_defended_by_pawn = !(pawn_attacks_white(pos) & black_pawns);
            *(bitboards + index + THREAT_BY_ROOK + 2 + BLACK) += (float) ((pos & white_rook_attacks) && is_not_defended_by_pawn);
        }

        // rook info
        BB white_rooks_c = rooks & white;
        while (white_rooks_c) {
            int pos = pop_lsb(&white_rooks_c);
            BB same_file = H_FILE << file(pos);
            // rook on semi open file
            *(bitboards + index + ROOK_ON_SEMI_OPEN_FILE_BONUS) += (float) ((same_file & black_pawns) && !(same_file & white_pawns));
            // rook on open file
            *(bitboards + index + ROOK_ON_OPEN_FILE_BONUS) += (float) !((same_file & black_pawns) | (same_file & white_pawns));
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 3) += (float) (((1ULL << pos) & black_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 3) += (float) (((1ULL << pos) & black_rook_attacks) != 0ULL);
        }
        
        // black
        BB black_rooks_c = rooks & black;
        while (black_rooks_c) {
            int pos = pop_lsb(&black_rooks_c);
            BB same_file = H_FILE << file(pos);
            // rook on semi open file
            *(bitboards + index + ROOK_ON_SEMI_OPEN_FILE_BONUS + BLACK) += (float) ((same_file & white_pawns) && !(same_file & black_pawns));
            // rook on open file
            *(bitboards + index + ROOK_ON_OPEN_FILE_BONUS + BLACK) += (float) !((same_file & black_pawns) | (same_file & white_pawns));
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 3 + BLACK) += (float) (((1ULL << pos) & white_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 3 + BLACK) += (float) (((1ULL << pos) & white_rook_attacks) != 0ULL);
        }

        // rook info
        BB white_queens_c = queens & white;
        while (white_queens_c) {
            BB pos = white_queens_c & -white_queens_c;
            white_queens_c &= white_queens_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 4) += (float) ((pos & black_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 4) += (float) ((pos & black_rook_attacks) != 0ULL);
        }

        // black
        BB black_queens_c = queens & black;
        while (black_queens_c) {
            BB pos = black_queens_c & -black_queens_c;
            black_queens_c &= black_queens_c - 1;
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 4 + BLACK) += (float) ((pos & white_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 4 + BLACK) += (float) ((pos & white_rook_attacks) != 0ULL);
        }


        // no while loop because there can only be one king
        // king info
        BB white_king_c = kings & white;
        while (white_king_c) {
            int pos = pop_lsb(&white_king_c);
            BB same_file = H_FILE << file(pos);
            // king on semi open file
            *(bitboards + index + KING_ON_SEMI_OPEN_FILE_PENALTY) += (float) ((same_file & black_pawns) && !(same_file & white_pawns));
            // king on open file
            *(bitboards + index + KING_ON_OPEN_FILE_PENALTY) += (float) !((same_file & black_pawns) | (same_file & white_pawns));
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 5) += (float) (((1ULL << pos) & black_minor_piece_attacks) != 0ULL);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 5) += (float) (((1ULL << pos) & black_rook_attacks) != 0ULL);
            BB king_ring_white = king_moves(1ULL << pos, 0ULL, 0ULL);
            // king attacks
            BB pawn_attacks_white_ = pawn_attacks_white(king_ring_white) | king_ring_white;
            BB knight_attacks_white = knight_moves(king_ring_white, 0ULL) | king_ring_white;
            BB bishop_attacks_white = bishop_moves(king_ring_white, 0ULL, empties);
            BB rook_attacks_white = rook_moves(king_ring_white, 0ULL, empties);
            BB queen_attacks_white = queen_moves(king_ring_white, 0ULL, empties);
            BB king_attacks_white = king_moves(king_ring_white, 0ULL, 0ULL) | king_ring_white;
            *(bitboards + index + KING_RING_ATTACKS    ) = (float) bbcount((black & pawns) & pawn_attacks_white_);
            *(bitboards + index + KING_RING_ATTACKS + 1) = (float) bbcount((black & knights) & knight_attacks_white);
            *(bitboards + index + KING_RING_ATTACKS + 2) = (float) bbcount((black & bishops) & bishop_attacks_white);
            *(bitboards + index + KING_RING_ATTACKS + 3) = (float) bbcount((black & rooks) & rook_attacks_white);
            *(bitboards + index + KING_RING_ATTACKS + 4) = (float) bbcount((black & queens) & queen_attacks_white);
            *(bitboards + index + KING_RING_ATTACKS + 5) = (float) (((black & kings) & king_attacks_white) != 0ULL);
            // defense
            *(bitboards + index + KING_RING_DEFENSES    ) = (float) bbcount((white & pawns) & pawn_attacks_white_);
            *(bitboards + index + KING_RING_DEFENSES + 1) = (float) bbcount((white & knights) & knight_attacks_white);
            *(bitboards + index + KING_RING_DEFENSES + 2) = (float) bbcount((white & bishops) & bishop_attacks_white);
            *(bitboards + index + KING_RING_DEFENSES + 3) = (float) bbcount((white & rooks) & rook_attacks_white);
            *(bitboards + index + KING_RING_DEFENSES + 4) = (float) bbcount((white & queens) & queen_attacks_white);
        }

        // black
        BB black_king_c = kings & black;
        while (black_king_c) {
            int pos = pop_lsb(&black_king_c);
            BB same_file = H_FILE << file(pos);
            // king on semi open file
            *(bitboards + index + KING_ON_SEMI_OPEN_FILE_PENALTY + BLACK) += (float) ((same_file & white_pawns) && !(same_file & black_pawns));
            // king on open file
            *(bitboards + index + KING_ON_SEMI_OPEN_FILE_PENALTY + BLACK) += (float) !((same_file & black_pawns) | (same_file & white_pawns));
            // is attacked by minor piece
            *(bitboards + index + THREAT_BY_MINOR_PIECE + 5 + BLACK) += (float) ((1ULL << pos) & white_minor_piece_attacks);
            // is attacked by rook
            *(bitboards + index + THREAT_BY_ROOK + 5 + BLACK) += (float) ((1ULL << pos) & white_rook_attacks);
            BB king_ring_black = king_moves(1ULL << pos, 0ULL, 0ULL);
            // king attacks
            BB pawn_attacks_black_ = pawn_attacks_white(king_ring_black) | king_ring_black;
            BB knight_attacks_black = knight_moves(king_ring_black, 0ULL) | king_ring_black;
            BB bishop_attacks_black = bishop_moves(king_ring_black, 0ULL, empties);
            BB rook_attacks_black = rook_moves(king_ring_black, 0ULL, empties);
            BB queen_attacks_black = queen_moves(king_ring_black, 0ULL, empties);
            BB king_attacks_black = king_moves(king_ring_black, 0ULL, 0ULL) | king_ring_black;
            *(bitboards + index + KING_RING_ATTACKS     + BLACK) = (float) bbcount((white & pawns) & pawn_attacks_black_);
            *(bitboards + index + KING_RING_ATTACKS + 1 + BLACK) = (float) bbcount((white & knights) & knight_attacks_black);
            *(bitboards + index + KING_RING_ATTACKS + 2 + BLACK) = (float) bbcount((white & bishops) & bishop_attacks_black);
            *(bitboards + index + KING_RING_ATTACKS + 3 + BLACK) = (float) bbcount((white & rooks) & rook_attacks_black);
            *(bitboards + index + KING_RING_ATTACKS + 4 + BLACK) = (float) bbcount((white & queens) & queen_attacks_black);
            *(bitboards + index + KING_RING_ATTACKS + 5 + BLACK) = (float) (((white & kings) & king_attacks_black) != 0ULL);
            // defense
            *(bitboards + index + KING_RING_DEFENSES     + BLACK) = (float) bbcount((black & pawns) & pawn_attacks_black_);
            *(bitboards + index + KING_RING_DEFENSES + 1 + BLACK) = (float) bbcount((black & knights) & knight_attacks_black);
            *(bitboards + index + KING_RING_DEFENSES + 2 + BLACK) = (float) bbcount((black & bishops) & bishop_attacks_black);
            *(bitboards + index + KING_RING_DEFENSES + 3 + BLACK) = (float) bbcount((black & rooks) & rook_attacks_black);
            *(bitboards + index + KING_RING_DEFENSES + 4 + BLACK) = (float) bbcount((black & queens) & queen_attacks_black);
        }

        // get the tempo
        *(bitboards + index + TEMPO) = (float) (buf[++str_ptr] == 'w');
        *(bitboards + index + TEMPO + BLACK) = !*(bitboards + index + TEMPO);

        // the phase
        *(phases + j) = (float)cur_phase / 24.0f;
        // *(phases + j) = (((float)((24 - cur_phase) * 256 + 12)) / 6144.0f);

        // move pointer to square bracket
        while (buf[++str_ptr] != '[');
        char char_eval[4];
        char_eval[0] = buf[str_ptr + 1];
        char_eval[1] = buf[str_ptr + 2];
        char_eval[2] = buf[str_ptr + 3];
        char_eval[3] = '\0';

        // get who won
        *(ys + j) = (strtof(char_eval, NULL));

        j++;
    }
}

// go back to the start of the file
void reread() {
    count = 0ULL;
}

// close the file to avoid memory leaks (of over 300mb each)
void close_book() {
    fclose(book);
    printf("closed book\n");
}

void print_features(float *bitboards, int i) {
    int index = i*FEATURE_SIZE;
    int sep = 0;
    for (int i = 0; i < FEATURE_SIZE; i++) {
        if ((++sep % 8) == 0) {
            printf("\n");
        }
        switch (i % BLACK) {
            case PAWN_PST: printf("\n\nPAWN_PST"); goto HIT;
            case KNIGHT_PST: printf("\n\nKNIGHT_PST"); goto HIT;
            case BISHOP_PST: printf("\n\nBISHOP_PST"); goto HIT;
            case ROOK_PST: printf("\n\nROOK_PST"); goto HIT;
            case QUEEN_PST: printf("\n\nQUEEN_PST"); goto HIT;
            case KING_PST: printf("\n\nKING_PST"); goto HIT;
            case KING_RING_ATTACKS: printf("\n\nKING_RING_ATTACKS"); goto HIT;
            case KING_RING_DEFENSES: printf("\n\nKING_RING_DEFENSES"); goto HIT;
            case PASSED_PAWN_RANK_RPT: printf("\n\nPASSED_PAWN_RANK"); goto HIT;
            case PROTECTED_PAWN_RANK_RPT: printf("\n\nPROTECTED_PAWN_RANK"); goto HIT;
            case THREAT_BY_MINOR_PIECE: printf("\n\nTHREAT_BY_MINOR_PIECE"); goto HIT;
            case THREAT_BY_ROOK: printf("\n\nTHREAT_BY_ROOK"); goto HIT;
            case PAWN_SHIELD_1_BONUS: printf("\n\nPAWN_SHIELDS"); goto HIT;
            case ISOLATED_PAWN_PENALTY: printf("\n\nISOLATED_PAWN"); goto HIT;
            case DOUBLED_PAWN_PENALTY: printf("\n\nDOUBLED_PAWN"); goto HIT;
            case ROOK_ON_OPEN_FILE_BONUS: printf("\n\nROOK_ON_OPEN_FILE"); goto HIT;
            case ROOK_ON_SEMI_OPEN_FILE_BONUS: printf("\n\nROOK_ON_SEMI_OPEN_FILE"); goto HIT;
            case KING_ON_OPEN_FILE_PENALTY: printf("\n\nKING_ON_OPEN_FILE"); goto HIT;
            case KING_ON_SEMI_OPEN_FILE_PENALTY: printf("\n\nKING_ON_SEMI_OPEN_FILE"); goto HIT;
            case BISHOP_PAIR_BONUS:  printf("\n\nBISHOP_PAIR"); goto HIT;
            case TEMPO: printf("\n\nTEMPO"); goto HIT;
            HIT:
                printf("\n");
                sep = 0;
        }

        printf("%.0f ", *(bitboards + index + i));
    }
    printf("\n");
}

int main() {
    // print_BB(H_FILE);
    // print_BB(south_east_ray((1ULL << 55) | (1ULL << 48), ~0ULL));
    // BB empties = ~2ULL;
    // print_BB(bbcount(pawn_attacks_white(1ULL << 10)));
    float *bitboards = malloc(BATCH_SIZE * FEATURE_SIZE * sizeof(float));
    float *phases = malloc(BATCH_SIZE * sizeof(float));
    float *ys = malloc(BATCH_SIZE * sizeof(float));
    memset(bitboards, 0, BATCH_SIZE*FEATURE_SIZE*sizeof(*bitboards));
    memset(phases, 0, BATCH_SIZE*sizeof(*phases));
    memset(ys, 0, BATCH_SIZE*sizeof(*ys));
    open_book();
    // int stop = DATA_SIZE / BATCH_SIZE;
    // for (int i = 0; i < stop; i++) {
        parse_batch(bitboards, phases, ys);
        // printf("%f\n", *phases);
    // }
    print_features(bitboards, 0);
    close_book();
    free(bitboards);
    free(phases);
    free(ys);

    return 0;
}