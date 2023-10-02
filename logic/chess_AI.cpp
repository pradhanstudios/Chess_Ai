#include <iostream>
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
    int piece_pos;
    int direction;
    int distance;
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

int direction_offsets[8] = {1, -1, 8, -8, 7, -7, 9, -9};

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
void move_piece(std::vector<int> &board, int piece_pos, std::string direction, int distance)
{
    int cur = board[piece_pos];
    int target = piece_pos + (directional_movement[direction] * distance);
    board[piece_pos] = None;
    board[target] = cur;
}

// bool is_legal_move(std::vector<int> board, Move move)
// {
// TODO: implement function
// }

int main(void)
{
    std::vector<int> board;
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    reset_values(board);

    open_fen(board, starting_fen);

    print_board(board);

    // testing move piece function
    std::cout << "move pawn down-right 3 spaces" << std::endl;
    move_piece(board, 11, "down-right", 3);
    print_board(board);

    // for (int i = 0; i < 64; i++)
    // {
    //     board[i] = 0;
    //     std::cout << board[i] << std::endl;
    // }

    // std::cout << board[1] << std::endl;
    return 0;
}
