#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <bitset>

/*

tuple to position converter function
(1, 5) -> 4
(2, 4) -> 10

8 rows x 8 columns

COL:
1st column: 0, 8, ... <=> (pos % 8 == 0)
2nd column: 1, 9, ... <=> (pos % 8 == 1)
... column: ..... ... <=> (pos % 8 == ...)

ROW:
1st row: 0-7  <=> (pos // 8 == 0)
2nd row: 8-15 <=> (pos // 8 == 1)
... row: .... <=> (pos // 8 == ...)


*/

/***********/
/* Structs */
/***********/

/**
 * @brief
 *
 * @param int Piece
 * @param int start
 * @param int end
 */
struct Move
{
    int Piece;
    int start;
    int end;
};

enum Piece
{
    None = 0,   // 00000
    Pawn = 1,   // 00001
    King = 2,   // 00010
    Knight = 3, // 00011
    Bishop = 4, // 00100
    Rook = 5,   // 00101
    Queen = 6,  // 00110
    White = 8,  // 01000
    Black = 16, // 10000
    Both = 24,  // 11000
};

/*************/
/* Variables */
/*************/

std::map<int, std::string> piece_num_to_name = {
    {0, "None"},
    {1, "Pawn"},
    {2, "King"},
    {3, "Knight"},
    {4, "Bishop"},
    {5, "Rook"},
    {6, "Queen"},
};

std::map<std::string, int>
    directional_movement = {
        {"up", -8},
        {"down", 8},
        {"left", -1},
        {"right", 1},
        {"up-right", -7},
        {"up-left", -9},
        {"down-right", 9},
        {"down-left", 7},
};

std::map<int, char> col_num_to_letter = {
    {0, 'A'},
    {1, 'B'},
    {2, 'C'},
    {3, 'D'},
    {4, 'E'},
    {5, 'F'},
    {6, 'G'},
    {7, 'H'},
};

std::vector<int> direction_offsets = {1, -1, 8, -8, 7, -7, 9, -9};

std::vector<Move> all_moves;

std::map<char, int> fen_to_piece = {
    {'p', Pawn},
    {'r', Rook},
    {'n', Knight},
    {'b', Bishop},
    {'q', Queen},
    {'k', King},
};

std::map<int, char> piece_to_fen = {
    {None, '-'},
    {Pawn, 'p'},
    {Rook, 'r'},
    {Knight, 'n'},
    {Bishop, 'b'},
    {Queen, 'q'},
    {King, 'k'},
};

std::bitset<64> white_pieces;
std::bitset<64> black_pieces;
std::bitset<64> kings;
std::bitset<64> queens;
std::bitset<64> rooks;
std::bitset<64> bishops;
std::bitset<64> knights;
std::bitset<64> pawns;

std::map<int, std::bitset<64>> bitboards = {
    {Both, (white_pieces | black_pieces)}, // all of the pieces
    {White, white_pieces},
    {Black, black_pieces},
    {King, kings},
    {Queen, queens},
    {Rook, rooks},
    {Bishop, bishops},
    {Knight, knights},
    {Pawn, pawns},
};

/*************/
/* Functions */
/*************/

/**
 * @brief Get the column of the given position
 *
 * @param position
 * @return int
 */
int get_column(int position)
{
    return position % 8;
}

/**
 * @brief Get the row of the given position
 *
 * @param position
 * @return int
 */
int get_row(int position)
{
    return (int)(position / 8);
}

/**
 * @brief Reset values on the board
 *
 * @param board
 */
void reset_values(std::vector<int> &board)
{
    board.clear();
    for (int i = 0; i < 64; i++)
    {
        board.push_back(0);
    }
}

/**
 * @brief Print the board to the CLI
 *
 * @param board
 */
void print_board(std::vector<int> &board)
{
    std::cout << "-----------------------" << std::endl;
    std::cout << "|     A B C D E F G H |" << std::endl;
    std::cout << "|                     |" << std::endl;
    std::cout << "| 1   ";
    for (int i = 0; i < board.size(); i++)
    {
        if (board[i] & White)
        {
            std::cout << char(std::toupper(piece_to_fen[board[i] ^ White])) << " ";
        }
        else if (board[i] & Black)
        {
            std::cout << piece_to_fen[board[i] ^ Black] << " ";
        }
        else
        {
            std::cout << piece_to_fen[None] << " ";
        }
        if ((i + 1) % 8 == 0)
        {
            if (int(i / 8) != 7)
            {
                std::cout << "|" << std::endl
                          << "| "
                          << int(i / 8) + 2 << "   ";
            }
        }
    }
    std::cout << "|" << std::endl;
    std::cout << "-----------------------" << std::endl;
}

/**
 * @brief Sets the board to the position specified by the FEN
 *
 * @param board
 * @param fen
 */
void open_fen(std::vector<int> &board, std::string fen)
{
    char cur;
    int board_ptr = 0;
    for (int i = 0; i < fen.length(); i++)
    {
        cur = fen[i];

        if (isdigit(cur))
        {
            // skip squares
            board_ptr += std::stoi(&cur);
        }
        else if (cur == '/')
        {
            // next row
            continue;
        }
        else
        {
            // place piece
            board[board_ptr] = (islower(cur) ? Black : White) | (fen_to_piece[std::tolower(cur)]);
            board_ptr++;
        }
    }
}

/**
 * @brief Moves the piece specified at the piece_pos on the board based on direction and distance
 *
 * @param board board representation
 * @param piece_pos position of piece that will be moved
 * @param direction direction that the piece will be moved
 * @param distance how far the piece will move in the specified direction
 */
void move_piece(std::vector<int> &board, Move move)
{
    std::cout << "Board address: " << &board << std::endl;
    std::cout << piece_num_to_name[move.Piece] << " " << move.start << " " << move.end << std::endl;

    board[move.start] = None;
    board[move.end] = move.Piece;

    std::cout << move.end << ": " << piece_num_to_name[board[move.end]] << std::endl;

    all_moves.push_back(move);
}

/**
 * @brief Undo the last move
 *
 * @param board
 */
void undo_move(std::vector<int> board)
{
    Move last_move = all_moves[-1];
    all_moves.pop_back();
    board[last_move.end] = None;
    board[last_move.start] = last_move.Piece;
}

/**
 * @brief Checks if the position is in the board
 *
 * @param pos
 * @return true
 * @return false
 */
bool in_bound(int pos)
{
    return (pos >= 0 && pos < 64);
}

/**
 * @brief Get the color of the piece
 *
 * @param piece
 * @return int
 */
int get_color(int piece)
{
    if (piece == None)
    {
        return None;
    }
    return piece & 8 ? White : Black;
}

/**
 * @brief Checks whether the given move results in a check on the board --
 * TODO: create the function
 *
 * @param board
 * @param move
 * @return true
 * @return false
 */
bool results_in_check(std::vector<int> board, Move move)
{
    return false; // for now
}

/**
 * @brief
 *
 * @param board
 * @param offsets
 * @param piece_pos
 * @param piece
 * @param range
 * @return std::vector<Move>
 */
std::vector<Move> get_piece_moves_helper(std::vector<int> board, std::vector<int> offsets, int piece_pos, int piece, int range = 8)
{
    std::vector<Move> moves;
    int org_color = get_color(piece);
    for (int dir : offsets)
    {
        for (int i = 1; i < range; i++)
        {
            if (in_bound(dir) == false)
            { // idk how to do it a better way lol
                // std::cout << "broke out of with: " << dir << "\n";

                break;
            }
            int cur_piece = board[piece_pos + dir * i];
            int cur_color = get_color(cur_piece);
            // std::cout << piece << " " << piece_pos << " " << piece_pos + dir * i << "\n";
            if (cur_color == org_color)
            {
                // std::cout << cur_color << "\n";
                break;
            }
            if (abs(get_row(piece_pos + dir * (i - 1)) - get_row(piece_pos + dir * i)) > 3)
            {
                break;
            }
            Move cur_move = (Move){piece, piece_pos, piece_pos + dir * i};
            // if (results_in_check(board, cur_move) == false) {
            // std::cout << "got here" << "\n";
            moves.push_back((Move){piece, piece_pos, piece_pos + dir * i});
            // }
            if ((cur_color != org_color) && (cur_piece != None))
            {
                break;
            }
        }
    }
    return moves;
}

/**
 * @brief Gets the legal moves of the given piece
 * @param board
 * @param piece
 * @param pos
 * @return std::vector<Move>
 */
std::vector<Move> get_piece_moves(std::vector<int> board, int piece, int pos)
{
    if (piece == Queen)
    {
        return get_piece_moves_helper(board, direction_offsets, pos, piece); // all moves
    }

    if (piece == Rook)
    {
        std::vector<int> movements(direction_offsets.begin(), direction_offsets.begin() + 4); // first 4 moves

        return get_piece_moves_helper(board, movements, pos, piece);
    }
    if (piece == Bishop)
    {
        std::vector<int> movements(direction_offsets.begin() + 4, direction_offsets.begin() + 8); // last 4 moves
        return get_piece_moves_helper(board, movements, pos, piece);
    }

    if (piece == King)
    {
        return get_piece_moves_helper(board, direction_offsets, pos, piece, 2);
    }

    if (piece == Knight)
    {
        std::vector<int> movements = {-17, -15, -10, -6, 17, 15, 10, 6}; // knight offsets
        return get_piece_moves_helper(board, movements, pos, piece);
    }

    // default return value if reaches end
    Move def_move = (Move){None, None, None};
    std::vector<Move> def_vector = {def_move};
    return def_vector;
}

/**
 * @brief Checks if the given move is legal
 *
 * @param board
 * @param move
 * @return bool
 */
bool is_legal_move(std::vector<int> board, Move move)
{
    for (Move m : get_piece_moves(board, move.Piece, move.start))
    {
        // std::cout << m.end << " " << move.end << "\n";
        if (m.end == move.end)
        {
            return true;
        }
    }
    return false;
}

void print_readable_position(int position)
{
    std::cout << col_num_to_letter[get_column(position)] << get_row(position) + 1;
}

int get_piece(std::vector<int> board, int position)
{
    return board[position] % 8;
}

void update_bitboards(std::vector<int> board)
{
    for (int i = 0; i < 64; i++)
    {
        int cur_piece = board[i];
        if (cur_piece == None)
        {
            white_pieces[i] = 0;
            black_pieces[i] = 0;
            kings[i] = 0;
            queens[i] = 0;
            rooks[i] = 0;
            bishops[i] = 0;
            knights[i] = 0;
            pawns[i] = 0;
            continue;
        }
        else if (get_color(cur_piece) == White)
        {
            white_pieces[i] = 1;
        }
        else
        { // color is black
            black_pieces[i] = 1;
        }

        cur_piece ^= get_color(cur_piece) == White ? White : Black; // get just the piece

        if (cur_piece == King)
        {
            kings[i] = 1;
        }
        else if (cur_piece == Queen)
        {
            queens[i] = 1;
        }
        else if (cur_piece == Rook)
        {
            rooks[i] = 1;
        }
        else if (cur_piece == Knight)
        {
            knights[i] = 1;
        }
        else if (cur_piece == Pawn)
        {
            pawns[i] = 1;
        }
        else if (cur_piece == Bishop)
        {
            bishops[i] = 1;
        }
    }

    bitboards = {
        {Both, (white_pieces | black_pieces)}, // all of the pieces
        {White, white_pieces},
        {Black, black_pieces},
        {King, kings},
        {Queen, queens},
        {Rook, rooks},
        {Bishop, bishops},
        {Knight, knights},
        {Pawn, pawns},
    };
}

void print_bitboard(std::bitset<64> bitboard)
{
    for (int i = 0; i < 64; i++)
    {
        std::cout << bitboard[i] << " ";

        if ((i + 1) % 8 == 0)
        {
            std::cout << std::endl;
        }
    }
}

int main(void)
{
    // variable to store the board
    std::vector<int> board;

    // FEN that specifies board position to start with
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    // std::string starting_fen = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";

    // reset the values on the board
    reset_values(board);

    // set the board position to the one in the starting fen
    open_fen(board, starting_fen);

    // print the board
    print_board(board);

    update_bitboards(board);

    print_bitboard(bitboards[White | Black]);

    // testing move piece function
    // std::cout << "move Pawn on ";
    // print_readable_position(50);
    // std::cout << " to ";
    // print_readable_position(42);
    // std::cout << std::endl;
    // std::cout << "Board address: " << &board << std::endl;
    // move_piece(board, (Move){Pawn, 50, 42});
    // print_board(board);

    // std::cout << "Is Knight on ";
    // print_readable_position(16);
    // std::cout << " --> ";
    // print_readable_position(33);
    // std::cout << " a legal move? " << (is_legal_move(board, (Move){Knight, 16, 33}) ? "Yes" : "No") << std::endl;

    // std::cout << "Piece on pos ";
    // print_readable_position(16);
    // std::cout << ": " << piece_num_to_name[get_piece(board, 16)] << std::endl;

    // for (int i = 0; i < 64; i++)
    // {
    //     board[i] = 0;
    //     std::cout << board[i] << std::endl;
    // }

    // std::cout << board[1] << std::endl;
    return 0;
}
