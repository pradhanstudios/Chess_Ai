#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <map>

/*

tuple to position converter funciotn

(1, 5) -> 4

(2, 4) -> 10

*/

struct Move
{
    int Piece;
    int start;
    int end;
};

enum Pieces
{
    None = 0,
    Pawn = 1,
    King = 2,
    Knight = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,
    White = 8,
    Black = 16
};

std::map<std::string, int> directional_movement = {
    {"up", -8},
    {"down", 8},
    {"left", -1},
    {"right", 1},
    {"up-right", -7},
    {"up-left", -9},
    {"down-right", 9},
    {"down-left", 7},
};

std::vector<int> direction_offsets = {1, -1, 8, -8, 7, -7, 9, -9};

std::vector<Move> all_moves;

std::map<char, int> fen_to_piece = {
    {'p', Pawn},
    {'r', Rook},
    {'n', Knight},
    {'b', Bishop},
    {'q', Queen},
    {'k', King}};

std::map<int, char> piece_to_fen = {
    {None, '-'},
    {Pawn, 'p'},
    {Rook, 'r'},
    {Knight, 'n'},
    {Bishop, 'b'},
    {Queen, 'q'},
    {King, 'k'}};

void reset_values(std::vector<int> &board)
{
    board.clear();
    for (int i = 0; i < 64; i++)
    {
        board.push_back(0);
    }
}

bool move_eq(Move m1, Move m2) {
    return (m1.end == m2.end) && (m1.Piece == m2.Piece) && (m1.start == m2.start);
}

void print_board(std::vector<int> board)
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
 * @brief Moves the piece specified at the piece_pos on the board based on direction and distance.
 *
 * @param board board representation
 * @param piece_pos position of piece that will be moved
 * @param direction direction that the piece will be moved
 * @param distance how far the piece will move in the specified direction
 */
void move_piece(std::vector<int> &board, Move move)
{
    board[move.start] = None;
    board[move.end] = move.Piece;
    all_moves.push_back(move);
}

void undo_move(std::vector<int> board) {
    Move last_move = all_moves[-1];
    all_moves.pop_back();
    board[last_move.end] = None;
    board[last_move.start] = last_move.Piece;

}

bool in_bound(int pos) {
    return (pos >= 0 && pos < 64);
}

int get_color(int piece) {
    return piece & 8 ? White : Black;
}

bool results_in_check(std::vector<int> board, Move move) {
    return false; // for now
}

std::vector<Move> get_piece_moves(std::vector<int> board, std::vector<int> offsets, int piece_pos, int piece, int range=8) {
    std::vector<Move> moves;
    int org_color = get_color(piece);
    for (int dir : offsets) {
        for (int i = 1; i < range; i++) {
            if (in_bound(dir) == false) { // idk how to do it a better way lol
                break;
            }
            int cur_piece = board[piece_pos + dir * i];
            int cur_color = get_color(cur_piece);
            if (cur_color == org_color) {
                break;
            }
            Move cur_move = (Move){piece, piece_pos, piece_pos + dir * i};

            if (results_in_check(board, cur_move) == false) {
                moves.push_back((Move){piece, piece_pos, piece_pos + dir * i});
            }
            if ((cur_color != org_color) && (cur_piece != None)) {
                break;
            }
        }
    }
    return moves;
}

std::vector<Move> get_legal_moves_piece(std::vector<int> board, int piece, int pos) {
    if (piece == Queen) {
        return get_piece_moves(board, direction_offsets, pos, piece); // all moves
    }

    if (piece == Rook) {
        std::vector<int> movements(direction_offsets.begin(), direction_offsets.begin() + 4); // first 4 moves
      
        return get_piece_moves(board, movements, pos, piece);
    }
    if (piece == Bishop) {
        std::vector<int> movements(direction_offsets.begin() + 4, direction_offsets.begin() + 8); // last 4 moves
        return get_piece_moves(board, movements, pos, piece);
    }

    if (piece == King) {
        return get_piece_moves(board, direction_offsets, pos, piece, 2);
    }

    if (piece == Knight) {
        std::vector<int> movements = {-17, -15, -10, -6, 17, 15, 10, 6}; // knight offsets
        return get_piece_moves(board, movements, pos, piece);
    }
}

bool is_legal_move(Move move) {
    for (Move m : all_moves) {
        if (move_eq(m, move)) {
            return true;
        }
    }
    return false;
}

int main(void)
{
    std::vector<int> board;
    // std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    std::string starting_fen = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";

    reset_values(board);

    open_fen(board, starting_fen);

    print_board(board);

    // testing move piece function
    // std::cout << "move pawn down-right 3 spaces" << std::endl;
    // move_piece();
    // print_board(board);
    std::cout << is_legal_move((Move){Queen, 56, 57}) << "\n";

    // for (int i = 0; i < 64; i++)
    // {
    //     board[i] = 0;
    //     std::cout << board[i] << std::endl;
    // }

    // std::cout << board[1] << std::endl;
    return 0;
}
