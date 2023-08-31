from vars import *

opposites = {"w": "b", "b": "w"}
color_moves = {"b": 1, "w": -1}
pawn_startings = {"b": 1, "w": 6}


def tuple_addition(t1, t2):
    return (t1[0] + t2[0], t1[1], t2[1])


def legal_moves(board, start_coords: tuple):
    output = []
    color, _, piece = board[start_coords[0]][start_coords[1]].partition("_")

    if piece.lower() == "p":
        if board[start_coords[0] + pawn_startings[color] * 1][start_coords[1]] == BLANK:
            output.append(
                (start_coords[0] + pawn_startings[color] * 1, start_coords[1])
            )
            if (
                color == "w"
                and start_coords[0] == pawn_startings[color]
                and board[start_coords[0] - 2][start_coords[1]] == BLANK
            ):
                output.append(
                    (start_coords[0] + color_moves[color] * 2, start_coords[1])
                )
                output.append(
                    (start_coords[0] + color_moves[color] * 1, start_coords[1])
                )
        if (
            board[start_coords[0] + pawn_startings[color] * 1][start_coords[1] + 1]
            != BLANK
            and board[start_coords[0] + pawn_startings[color] * 1][start_coords[1] + 1][
                0
            ]
            != color
        ):
            output.append(
                start_coords[0] + pawn_startings[color] * 1,
                start_coords[1] + 1,
            )
        if (
            board[start_coords[0] + pawn_startings[color] * 1][start_coords[1] - 1]
            != BLANK
            and board[start_coords[0] + pawn_startings[color] * 1][start_coords[1] - 1][
                0
            ]
            != color
        ):
            output.append(
                start_coords[0] + pawn_startings[color] * 1,
                start_coords[1] - 1,
            )

    elif piece.lower() == "b":  # bishop
        next_move = (start_coords[0] - 1, start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[1] > len(board[0])
            or next_move[0] < 0
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] - 1, start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] < 0
            or next_move[1] < 0
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] > len(board)
            or next_move[1] > len(board[0])
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])
        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] > len(board)
            or next_move[1] < len(board[0])
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

    elif piece.lower() == "r":  # rook
        next_move = (start_coords[0] - 1, start_coords[1])
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[0] < 0:
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1])

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1])
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[0] < len(board):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1])

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0], start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[1] > len(
            board[0]
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0], start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0], start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])
        while board[next_move[0]][next_move[1]] == BLANK or next_move[1] < 0:
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0], start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

    elif piece.lower() == "q":  # queen
        next_move = (start_coords[0] - 1, start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[1] > len(board[0])
            or next_move[0] < 0
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] - 1, start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] < 0
            or next_move[1] < 0
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] > len(board)
            or next_move[1] > len(board[0])
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])
        while (
            board[next_move[0]][next_move[1]] == BLANK
            or next_move[0] > len(board)
            or next_move[1] < len(board[0])
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] - 1, start_coords[1])
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[0] < 0:
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] - 1, start_coords[1])

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0] + 1, start_coords[1])
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[0] < len(board):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0] + 1, start_coords[1])

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0], start_coords[1] + 1)
        cur_move = (start_coords[0], start_coords[1])

        while board[next_move[0]][next_move[1]] == BLANK or next_move[1] > len(
            board[0]
        ):
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0], start_coords[1] + 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        next_move = (start_coords[0], start_coords[1] - 1)
        cur_move = (start_coords[0], start_coords[1])
        while board[next_move[0]][next_move[1]] == BLANK or next_move[1] < 0:
            output.append(cur_move)
            cur_move = next_move
            next_move = (start_coords[0], start_coords[1] - 1)

        if (
            board[next_move[0]][next_move[1]] != BLANK
            and board[next_move[0]][next_move[1]] != color
        ):
            output.append(next_move)

        if piece.lower() == "n":  # knight
            if (
                board[start_coords[0] + 2][start_coords[1] + 1] == BLANK
                or (
                    board[start_coords[0] + 2][start_coords[1] + 1] != BLANK
                    and board[start_coords[0] + 2][start_coords[1] + 1] != color
                )
            ) and not (
                start_coords[0] + 2 > len(board) or start_coords[1] + 1 > len(board)
            ):
                output.append((start_coords[0] + 2, start_coords[1] + 1))

            if (
                board[start_coords[0] - 2][start_coords[1] + 1] == BLANK
                or (
                    board[start_coords[0] - 2][start_coords[1] + 1] != BLANK
                    and board[start_coords[0] - 2][start_coords[1] + 1] != color
                )
            ) and not (start_coords[0] - 2 < 0 or start_coords[1] + 1 > len(board)):
                output.append((start_coords[0] - 2, start_coords[1] + 1))

            if (
                board[start_coords[0] - 2][start_coords[1] - 1] == BLANK
                or (
                    board[start_coords[0] - 2][start_coords[1] - 1] != BLANK
                    and board[start_coords[0] - 2][start_coords[1] - 1] != color
                )
            ) and not (start_coords[0] - 2 < 0 or start_coords[1] - 1 < 0):
                output.append((start_coords[0] - 2, start_coords[1] - 1))

            if (
                board[start_coords[0] + 2][start_coords[1] - 1] == BLANK
                or (
                    board[start_coords[0] + 2][start_coords[1] - 1] != BLANK
                    and board[start_coords[0] + 2][start_coords[1] - 1] != color
                )
            ) and not (start_coords[0] + 2 > len(board) or start_coords[1] - 1 < 0):
                output.append((start_coords[0] + 2, start_coords[1] - 1))

            if (
                board[start_coords[0] + 1][start_coords[1] + 2] == BLANK
                or (
                    board[start_coords[0] + 1][start_coords[1] + 2] != BLANK
                    and board[start_coords[0] + 1][start_coords[1] + 2] != color
                )
            ) and not (
                start_coords[0] + 1 > len(board) or start_coords[1] + 2 > len(board)
            ):
                output.append((start_coords[0] + 1, start_coords[1] + 2))

            if (
                board[start_coords[0] - 1][start_coords[1] + 2] == BLANK
                or (
                    board[start_coords[0] - 1][start_coords[1] + 2] != BLANK
                    and board[start_coords[0] - 1][start_coords[1] + 2] != color
                )
            ) and not (start_coords[0] - 1 < 0 or start_coords[1] + 2 > len(board)):
                output.append((start_coords[0] - 1, start_coords[1] + 2))

            if (
                board[start_coords[0] - 1][start_coords[1] - 2] == BLANK
                or (
                    board[start_coords[0] - 1][start_coords[1] - 2] != BLANK
                    and board[start_coords[0] - 1][start_coords[1] - 2] != color
                )
            ) and not (start_coords[0] - 1 < 0 or start_coords[1] - 2 < 0):
                output.append((start_coords[0] - 1, start_coords[1] - 2))

            if (
                board[start_coords[0] + 1][start_coords[1] - 2] == BLANK
                or (
                    board[start_coords[0] + 1][start_coords[1] - 2] != BLANK
                    and board[start_coords[0] + 1][start_coords[1] - 2] != color
                )
            ) and not (start_coords[0] + 1 > len(board) or start_coords[1] - 2 < 0):
                output.append((start_coords[0] + 1, start_coords[1] - 2))

    return output
