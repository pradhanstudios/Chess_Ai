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

BG_COLOR = (249, 172, 113)  # light brown
BG_IMG = "assets/backgrounds/bg_wood.jpg"

BOARD_IMG = "assets/backgrounds/chess_board.png"
BOARD_SIZE = (HEIGHT // 100) * 100 if HEIGHT <= WIDTH else (WIDTH // 100) * 100

BLANK = "   "
PIECE_SIZE = BOARD_SIZE // 8

COLORS = {True: "white", False: "black"}
