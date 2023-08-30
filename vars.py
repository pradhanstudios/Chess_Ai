import os

WIDTH, HEIGHT = 1280, 720
FPS = 60

BG_COLOR = (249, 172, 113)  # light brown
BG_IMG = "assets/backgrounds/bg_wood.jpg"

BOARD_IMG = "assets/backgrounds/chess_board.png"
BOARD_SIZE = (HEIGHT // 100) * 100 if HEIGHT <= WIDTH else (WIDTH // 100) * 100

PIECE_SIZE = (BOARD_SIZE // 8, BOARD_SIZE // 8)

pieces = {}
for fname in os.listdir("assets/sprites"):
    str_parts = fname.split("_")
    pieces[str_parts[0] + "_" + str_parts[1]] = fname
