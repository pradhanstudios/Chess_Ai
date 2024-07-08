#ifndef BOARD_HPP
#define BOARD_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "non_sliding_moves.hpp"
#include "magic_values.hpp"

enum PIECE { // for pieces, white is implied
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6,
    WHITE = 7,
    BLACK = 8,
    OTHER_TEAM_ATTACKS = 9,
    FULL = 10,
    BLACK_PAWN = BLACK | PAWN,
    BLACK_KNIGHT = BLACK | KNIGHT,
    BLACK_BISHOP = BLACK | BISHOP,
    BLACK_ROOK = BLACK | ROOK,
    BLACK_QUEEN = BLACK | QUEEN,
    BLACK_KING = BLACK | KING,
};

enum CASTLE_TYPE {
    QUEENSIDE_CASTLE = 0,
    KINGSIDE_CASTLE = 1
};

enum BOARD_STATE {
    RUNNING = 0,
    DRAW = 1,
    CHECKMATE = 2
};

extern const std::string DEFAULT_FEN;
extern const std::map<char, int> col_letter_to_num;

inline std::array<PIECE, 2> TURN_TO_INDEX_LOOKUP = {BLACK, WHITE};

constexpr bool _INDEX_TO_TURN(int index) {
    return index ^ BLACK;
}

constexpr int no_color(const int piece) noexcept {
    return piece & 7;
}

uint64_t prandom();
void print_vector(const std::vector<int> &v) noexcept;
void print_vector(const std::vector<std::string> &v) noexcept;

std::vector<std::string> split(const std::string &s, const char delim) noexcept;

#pragma pack(1)

struct History {
    unsigned int castle : 4;
    unsigned int capture : 4;
    unsigned int en_pessant : 6;
    unsigned int fifty_move_rule : 6;

    History(const unsigned int en_pessant=0u, const unsigned int castle=0b0000u, const unsigned int capture=0u, const unsigned int fifty_move_rule=0u);
};

#pragma pack()

struct Zobrist {
    std::array<std::array<BB, 64>, 6> pieces;
    std::array<BB, 64> en_pessants;
    std::array<BB, 16> castles;
    BB black_turn;
    Zobrist();
};

const inline Zobrist zobrist = Zobrist();

class Board {
    public:
        std::array<int, 64> piece_data = {EMPTY};
        std::array<BB, 11> pieces = {EMPTY};
        std::array<std::array<int, 64>, 2> pins;
        bool is_double_check;
        BB check_ray;
        BOARD_STATE state;
        uint64_t zobrist_key;
        // unsigned int cur_en_pessant;
        // int castles;
        std::vector<History> history;
        bool turn; // true for white, false for black
        Board(const std::string &fen) noexcept;
        void print_square_data() const noexcept;
        void play_move(const Move move) noexcept;
        void undo_move(const Move move) noexcept;


        constexpr BB get_color(PIECE color) const noexcept {
            return this->pieces[color];
        }

        constexpr BB get_piece_bitboard(PIECE piece) const noexcept {
            return this->pieces[piece];
        }
        constexpr BB get_piece_bitboard(PIECE color, PIECE piece) const noexcept {
            return this->get_color(color) & this->get_piece_bitboard(piece);
        }

        constexpr BB get_same_team_color() const noexcept {
            return this->get_color(TURN_TO_INDEX_LOOKUP[this->turn]);
        }

        constexpr BB get_same_team_color(PIECE piece) const noexcept {
            return this->get_color(TURN_TO_INDEX_LOOKUP[this->turn]) & this->get_piece_bitboard(piece);
        }

        constexpr BB get_other_team_color() const noexcept {
            return this->get_color(TURN_TO_INDEX_LOOKUP[!this->turn]);
        }

        constexpr BB get_other_team_color(PIECE piece) const noexcept {
            return this->get_color(TURN_TO_INDEX_LOOKUP[!this->turn]) & this->get_piece_bitboard(piece);
        }

        constexpr BB get_full_bitboard() const noexcept {
            return this->pieces[FULL];
        }

        constexpr BB get_empty_bitboard() const noexcept {
            return this->pieces[EMPTY];
        }

        inline History get_last_history() const noexcept {
            return this->history.back();
        }

        inline BB square_attacks(int square, BB same_team, bool other_team=0) const noexcept {
            BB pawn_attacks_from_square = PAWN_MOVES[(this->turn^other_team) + 2][square] & ~same_team;
            BB knight_attacks_from_square = KNIGHT_MOVES[square] & ~same_team;
            BB bishop_attacks_from_square = bishop_moves(this->pieces[FULL], same_team, square);
            BB rook_attacks_from_square = rook_moves(this->pieces[FULL], same_team, square);
            BB attacks = 
                (pawn_attacks_from_square & (this->pieces[PAWN])) |
                (knight_attacks_from_square & (this->pieces[KNIGHT])) |
                (bishop_attacks_from_square & ((this->pieces[BISHOP] | this->pieces[QUEEN]))) |
                (rook_attacks_from_square & ((this->pieces[ROOK]     | this->pieces[QUEEN])));  

            return attacks;
        }

        constexpr BB possibly_pinned_pieces_from_square(int square, BB same_team) const noexcept {
            return queen_moves(this->pieces[FULL], 0ULL, square) & same_team;
        }

        inline bool is_move_legal(const Move move, BB possibly_pinned, BB same_team_king) noexcept {
            bool is_legal = true;
            if (
                (SQUARE_TO_BB[move.from] & (possibly_pinned | same_team_king)) ||
                (move.type == EN_PESSANT)
            ) {
                this->play_move(move);
                
                if (square_attacks(lsb(this->get_other_team_color(KING)), this->get_other_team_color(), 1)) {
                    is_legal = false;
                }
                
                this->undo_move(move);
            }

            return is_legal;
        }

        constexpr bool is_open_file(const int file_) const noexcept {
            return !(this->pieces[PAWN] & (H_FILE << file_));
        }

        constexpr bool is_semi_open_file(const int file_, const PIECE color, const PIECE other_color) const noexcept {
            BB cur_file = H_FILE << file_;
            return !((this->pieces[color] & this->pieces[PAWN]) & cur_file) && ((this->pieces[other_color] & this->pieces[PAWN]) & cur_file);
        }

        inline bool is_in_check() const noexcept {
            return this->square_attacks(lsb(this->get_same_team_color(KING)), this->get_same_team_color());
        };

        inline void next_turn() noexcept {
            this->zobrist_key ^= zobrist.black_turn;
            this->turn = !this->turn;
        };
        
        constexpr void update_bitboards() noexcept {
            this->pieces[FULL] = (this->pieces[WHITE] | this->pieces[BLACK]);
            this->pieces[EMPTY] = ~this->pieces[FULL];
        };
};
#endif // BOARD_HPP