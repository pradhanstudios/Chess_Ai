from vars import *
from helpers import get_piece_properties
import sys
sys.setrecursionlimit(10000)


def evaluate(position, turn, depth=3):
    return minimax(position, turn, depth, NEGATIVE_INFINITY, INFINITY)
    # return evaluate_weights(position.get_all_pieces_2D())

def evaluate_weights(board: list[list]):
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

                
def minimax(position, maximize, depth, alpha, beta):
    # print(depth)
    # print(depth == 0)
    if depth == 0 or position.eval_game_over():
        return evaluate_weights(position.get_all_pieces_2D())

    if maximize:
        max_eval = NEGATIVE_INFINITY
        for move in position.board.legal_moves:
            position.board.push(move)
            evaluation = minimax(position, False, depth - 1, alpha, beta)
            position.board.pop()
            max_eval = max(max_eval, evaluation)
            alpha = max(alpha, evaluation)
            if beta <= alpha:
                break
        return max_eval
    else:
        min_eval = INFINITY
        for move in position.board.legal_moves:
            position.board.push(move)
            evaluation = minimax(position, True, depth - 1, alpha, beta)
            position.board.pop()
            min_eval = min(min_eval, evaluation)
            beta = min(beta, evaluation)
            if beta <= alpha:
                break
        return min_eval