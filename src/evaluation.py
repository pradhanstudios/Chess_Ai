from vars import *
from helpers import get_piece_properties
import sys
sys.setrecursionlimit(10000)


def evaluate(position, turn, depth=4):
    return minimax(position, turn, depth, NEGATIVE_INFINITY, INFINITY, depth)
    # return evaluate_weights(position.get_all_pieces_2D())

def evaluate_weights(board: list[list]):
    evaluation = 0.0
    for i in range(len(board)):
        for j in range(len(board[i])):
            color, piece = get_piece_properties(board[i][j])
            if piece != BLANK:
                # print(color, piece, i, j)
                multi = (EVAL_DICT[color][piece][i][j]) / 100
                multi = multi if color else -multi
                piece_eval = PIECE_VALUES[piece.lower()]
                piece_eval = piece_eval if color else -piece_eval
                # print(piece_eval)
                evaluation += multi + piece_eval
                # print(piece_eval)
                # evaluation += piece_eval
    return round(evaluation, 2)

best_move = [0, 0] # second one should be a move
                
def minimax(position, maximize, depth, alpha, beta, origdepth):
    # print(depth)
    # print(depth == 0)
    global best_move
    best_move = [NEGATIVE_INFINITY if maximize else INFINITY, 0] # second one should be a move

    def m_max(position, maximize, depth, alpha, beta):
        global best_move
        if depth == 0:
            return evaluate_weights(position.get_all_pieces_2D())
        if position.board.is_checkmate():
            won = position.board.outcome()
            if won.winner == True:
                return INFINITY if maximize else NEGATIVE_INFINITY
            if won.winner == False:
                return NEGATIVE_INFINITY if maximize else INFINITY
        if position.board.is_stalemate():
            return 0
        if maximize:
            max_eval = NEGATIVE_INFINITY
            for move in position.board.legal_moves:
                position.board.push(move)
                evaluation = m_max(position, False, depth - 1, alpha, beta)
                position.board.pop()
                max_eval = max(max_eval, evaluation)
                alpha = max(alpha, evaluation)
                if evaluation > best_move[0] and depth == origdepth:
                    # print("here")
                    best_move[0] = evaluation
                    best_move[1] = move
                if alpha >= beta:
                    break
            return max_eval
        else:
            min_eval = INFINITY
            for move in position.board.legal_moves:
                position.board.push(move)
                evaluation = m_max(position, True, depth - 1, alpha, beta)
                position.board.pop()
                min_eval = min(min_eval, evaluation)
                beta = min(beta, evaluation)
                if alpha >= beta:
                    break
            return min_eval
    m_max(position, maximize, depth, alpha, beta)
    return best_move