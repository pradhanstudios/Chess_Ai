from vars import *
from helpers import get_piece_properties


def evaluate(board: list[list]):
    evaluation = 0.0
    for i in range(len(board)):
        for j in range(len(board[i])):
            color, piece = get_piece_properties(board[i][j])
            additive = EVAL_DICT[color][piece][i][j]
            evaluation
