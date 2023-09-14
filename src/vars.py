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
