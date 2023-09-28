from vars import *
from helpers import get_piece_properties


def evaluate(board: list[list]):
    evaluation = 0.0
    for i in range(len(board)):
        for j in range(len(board[i])):
            color, piece = get_piece_properties(board[i][j])
            if piece != BLANK:
                # print(color, piece, i, j)
                multi = 1 + EVAL_DICT[color][piece][i][j] / 200
                piece_eval = PIECE_VALUES[piece.lower()]
                if color:
                    piece_eval *= multi
                else:
                    piece_eval *= -multi
                # print(piece_eval)
                evaluation += piece_eval
    return round(evaluation, 2)
                
