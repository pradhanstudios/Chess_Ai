from vars import *
from helpers import get_piece_properties


def evaluate(board: list[list]):
    evaluation = 0.0
    for i in range(len(board)):
        for j in range(len(board[i])):
            color, piece = get_piece_properties(board[i][j])
            if piece != BLANK:
                # print(color, piece, i, j)
                additive = EVAL_DICT[color][piece][i][j] / 250
                piece_eval = PIECE_VALUES[piece.lower()]
                if color:
                    piece_eval += additive
                else:
                    piece_eval -= additive
                # print(piece_eval)
                evaluation += piece_eval
    return round(evaluation - 78, 2)
                
