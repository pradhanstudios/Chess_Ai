# from scripts.vars import *
from vars import *


def _get_moves(board, cur_pos, dirx, diry, rng=8):
    # ccur_pos = cur_pos
    output = []
    color, _, piece = board[cur_pos[0]][cur_pos[1]].partition("_")
    hit_piece = False

    for i in range(rng):
        if hit_piece:
            break
        # new_pos = (cur_pos[0] + dirx*i, cur_pos[1] + diry*i)
        # print(cur_pos[0], new_pos[1], i)
        # cur_square = board[new_pos[0]][new_pos[1]]
        # print(1 <= new_pos[1] < 7)

        if 0 <= cur_pos[0] + dirx * i < 8 and 0 <= cur_pos[1] + diry * i < 8:
            # print("here")
            if board[cur_pos[0] + dirx * i][cur_pos[1] + diry * i] == BLANK:
                output.append((cur_pos[0] + dirx * i, cur_pos[1] + diry * i))

            if (
                board[cur_pos[0] + dirx * i][cur_pos[1] + diry * i].split("_")[0]
                != color
                and board[cur_pos[0] + dirx * i][cur_pos[1] + diry * i].split("_")[0]
                != BLANK
            ):
                output.append((cur_pos[0] + dirx * i, cur_pos[1] + diry * i))
                hit_piece = True

            # print(cur_pos[0] + dirx*i, cur_pos[1] + diry*i, board[cur_pos[0] + dirx*i][cur_pos[1] + diry*i].split("_")[0] == color, cur_pos)
            hit_piece = (
                board[cur_pos[0] + dirx * i][cur_pos[1] + diry * i].split("_")[0]
                == color
                and (cur_pos[0] + dirx * i, cur_pos[1] + diry * i) != cur_pos
            )
        else:
            break

    return output


def _pawn_moves(board, cur_pos):
    color, _, _ = board[cur_pos[0]][cur_pos[1]].partition("_")
    move = PAWN_MOVES[color]
    output = []

    if board[cur_pos[0] + move][cur_pos[1]] == BLANK:
        output.append((cur_pos[0] + move, cur_pos[1]))

    if color == "b" and cur_pos[1] == 1 and cur_pos[1] + move * 2 == BLANK:
        print("THIS")
        output.append((cur_pos[0], cur_pos[1] + move*2))

    # if color == "b" and cur_pos[0] == 1 and cur_pos[1] + move * 2 == BLANK:
    #     output.append((cur_pos[0], cur_pos[1] + move*2))

    if (0 < cur_pos[0] + move < 7) and (0 < cur_pos[1] + move < 7):

        if (
            board[cur_pos[0] + move][cur_pos[1] + move] != BLANK
            and board[cur_pos[0] + move][cur_pos[1] + move].split("_")[0] != color
        ):
            output.append((cur_pos[0] + move, cur_pos[1] + move))

    if (0 < cur_pos[0] + move < 7) and (0 < cur_pos[1] - move < 7):

        if (
            board[cur_pos[0] + move][cur_pos[1] - move] != BLANK
            and board[cur_pos[0] + move][cur_pos[1] - move].split("_")[0] != color
        ):
            output.append((cur_pos[0] + move, cur_pos[1] - move))

    return output


def get_possible_piece(board, cur_pos):
    color, _, piece = board[cur_pos[0]][cur_pos[1]].partition("_")
    # print(piece)
    piece_position = []

    if piece.lower() == "b":  # bishop
        for i in (-1, 1):
            for j in (-1, 1):
                piece_position.extend(_get_moves(board, cur_pos, i, j))

    if piece.lower() == "r":
        for i, j in zip((1, -1, 0, 0), (0, 0, 1, -1)):
            piece_position.extend(_get_moves(board, cur_pos, i, j))

    if piece.lower() == "q":  # queen
        for i in (-1, 1):
            for j in (-1, 1):
                piece_position.extend(_get_moves(board, cur_pos, i, j))
        for i, j in zip((1, -1, 0, 0), (0, 0, 1, -1)):
            piece_position.extend(_get_moves(board, cur_pos, i, j))
        # print(piece_position)

    if piece.lower() == "n":  # knight
        # (2, 1) (2, -1), (-2, 1), (-2, -1), (1, 2), (1, -2) (-1, 2), (-1, -2)
        for i, j in zip((2, 2, -2, -2, 1, 1, -1, -1), (1, -1, 1, -1, 2, -2, 2, -2)):
            # print(i, j)
            piece_position.extend(_get_moves(board, cur_pos, i, j, 2))

    if piece.lower() == "p":
        piece_position.extend(_pawn_moves(board, cur_pos))

    return piece_position

def get_algebraic_notation(board, start_pos, end_pos):
    ex = False
    color, _, piece = board[start_pos[1]][start_pos[0]]
    for c in range(len(board)):
        for r in range(len(board[c])):
            if board[r][c] == "{color}_{piece}":
                if end_pos in get_possible_piece(board, start_pos):
                    ex = True

    output = ""
    if piece.lower() != "p":
        output += piece.lower()
    if ex:
        output += NUMS_TO_LETTERS[start_pos[0]]
    # psuedo code
    # if toook piece:
    #     output += 'x'
    output += NUMS_TO_LETTERS[end_pos[0]+1] + str(end_pos[1]+1)
    # psuedo code
    # if check:
    #     output += '+'
    # psuedo code
    # if check mate:
    #     output += '#'

    return output
    
