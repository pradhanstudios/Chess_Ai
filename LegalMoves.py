from vars import *

opposites = {"w": "b", "b": "w"}

def isLegalMove(board, piece, move_coords: tuple):

    move_sqare = board[move_coords[0]][move_coords[1]]



    if move_sqare == BLANK:
        return True
    


