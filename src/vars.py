import os
import chess

WIDTH, HEIGHT = 1280, 720
FPS = 60

WHO_TURN = {0: "white", 1: "black"}
turn = 0
PIECE_INDICES = [i + j for i in chess.FILE_NAMES for j in chess.RANK_NAMES]

ASSET_FILENAME = "assets/sprites/"
ASSET_ENDNAME = "_png_1024px.png"
FILE_NAMES = {
    "b": "b_bishop",
    "k": "b_king",
    "n": "b_knight",
    "p": "b_pawn",
    "q": "b_queen",
    "r": "b_rook",
    "B": "w_bishop",
    "K": "w_king",
    "N": "w_knight",
    "P": "w_pawn",
    "Q": "w_queen",
    "R": "w_rook",
}

PIECE_VALUES = {
    "b": 3,
    "k": 0,
    "n": 3,
    "p": 1,
    "q": 9,
    "r": 5,
}

BG_COLOR = (249, 172, 113)  # light brown
BG_IMG = "assets/backgrounds/bg_wood.jpg"

BOARD_IMG = "assets/backgrounds/chess_board.png"
BOARD_SIZE = (HEIGHT // 100) * 100 if HEIGHT <= WIDTH else (WIDTH // 100) * 100

BLANK = "   "
PIECE_SIZE = BOARD_SIZE // 8

COLORS = {True: "white", False: "black"}

FONT_SIZE = 24

PIECE_POS = {"q": 10, "r": 110, "b": 210, "n": 310, "p": 1000, "k": 1000}
PAWN_END = {True: 8, False: 1}
PAWN_MOVES = {True: 1, False: -1}
##############
# EVAL STUFF #
##############
# credit to https://www.chessprogramming.org/Simplified_Evaluation_Function for values
W_PAWN = [
[0,  0,  0,  0,  0,  0,  0,  0],
[50, 50, 50, 50, 50, 50, 50, 50],
[10, 10, 20, 30, 30, 20, 10, 10],
 [5,  5, 10, 25, 25, 10,  5,  5],
 [0,  0,  0, 20, 20,  0,  0,  0],
 [5, -5,-10,  0,  0,-10, -5,  5],
 [5, 10, 10,-20,-20, 10, 10,  5],
 [0,  0,  0,  0,  0,  0,  0,  0]]
W_KNIGHT =[[ -50,-40,-30,-30,-30,-30,-40,-50,],
[-40,-20,  0,  0,  0,  0,-20,-40,],
[-30,  0, 10, 15, 15, 10,  0,-30,],
[-30,  5, 15, 20, 20, 15,  5,-30,],
[-30,  0, 15, 20, 20, 15,  0,-30,],
[-30,  5, 10, 15, 15, 10,  5,-30,],
[-40,-20,  0,  5,  5,  0,-20,-40,],
[-50,-40,-30,-30,-30,-30,-40,-50,]]
W_BISHOP = [[-20,-10,-10,-10,-10,-10,-10,-20,],
[-10,  0,  0,  0,  0,  0,  0,-10,] ,
[-10,  0,  5, 10, 10,  5,  0,-10,],
[-10,  5,  5, 10, 10,  5,  5,-10,],
[-10,  0, 10, 10, 10, 10,  0,-10,],
[-10, 10, 10, 10, 10, 10, 10,-10,],
[-10,  5,  0,  0,  0,  0,  5,-10,],
[-20,-10,-10,-10,-10,-10,-10,-20,]]
W_ROOK =  [[0,  0,  0,  0,  0,  0,  0,  0,],
  [5, 10, 10, 10, 10, 10, 10,  5,],
 [-5,  0,  0,  0,  0,  0,  0, -5,],
 [-5,  0,  0,  0,  0,  0,  0, -5,],
 [-5,  0,  0,  0,  0,  0,  0, -5,],
 [-5,  0,  0,  0,  0,  0,  0, -5,],
 [-5,  0,  0,  0,  0,  0,  0, -5,],
[  0,  0,  0,  5,  5,  0,  0,  0]]
W_QUEEN = [[-20,-10,-10, -5, -5,-10,-10,-20],
[-10,  0,  0,  0,  0,  0,  0,-10,],
[-10,  0,  5,  5,  5,  5,  0,-10,],
[ -5,  0,  5,  5,  5,  5,  0, -5,],
[  0,  0,  5,  5,  5,  5,  0, -5,],
[-10,  5,  5,  5,  5,  5,  0,-10,],
[-10,  0,  5,  0,  0,  0,  0,-10,],
[-20,-10,-10, -5, -5,-10,-10,-20]]
W_KING_MIDDLE_GAME = -[[30,-40,-40,-50,-50,-40,-40,-30,],
[-30,-40,-40,-50,-50,-40,-40,-30,],
[-30,-40,-40,-50,-50,-40,-40,-30,],
[-30,-40,-40,-50,-50,-40,-40,-30,],
[-20,-30,-30,-40,-40,-30,-30,-20,],
[-10,-20,-20,-20,-20,-20,-20,-10,],
[ 20, 20,  0,  0,  0,  0, 20, 20,],
[ 20, 30, 10,  0,  0, 10, 30, 20]]
W_KING_END_GAME = [[-50,-40,-30,-20,-20,-30,-40,-50,],
[-30,-20,-10,  0,  0,-10,-20,-30,],
[-30,-10, 20, 30, 30, 20,-10,-30,],
[-30,-10, 30, 40, 40, 30,-10,-30,],
[-30,-10, 30, 40, 40, 30,-10,-30,],
[-30,-10, 20, 30, 30, 20,-10,-30,],
[-30,-30,  0,  0,  0,  0,-30,-30,],
[-50,-30,-30,-30,-30,-30,-30,-50]]
# just reverse the white values to get the black ones
B_PAWN = W_PAWN[::-1]
B_KNIGHT = W_KNIGHT[::-1]
B_BISHOP = W_BISHOP[::-1]
B_ROOK = W_ROOK[::-1]
B_QUEEN = W_QUEEN[::-1]
B_KING_MIDDLE_GAME = W_KING_MIDDLE_GAME[::-1]
B_KING_END_GAME = W_KING_END_GAME[::-1]
WHITE_DICT = {
    "p": W_PAWN,
    "n": W_KNIGHT,
    "b": W_BISHOP,
    "r": W_ROOK,
    "q": W_QUEEN,
    "km": W_KING_MIDDLE_GAME,
    "ke": W_KING_END_GAME,
}
BLACK_DICT = {
    "P": B_PAWN,
    "N": B_KNIGHT,
    "B": B_BISHOP,
    "R": B_ROOK,
    "Q": B_QUEEN,
    "KM": B_KING_MIDDLE_GAME,
    "KE": B_KING_END_GAME,
}
EVAL_DICT = {True: WHITE_DICT, False: BLACK_DICT}
