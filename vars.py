import os

WIDTH, HEIGHT = 1200, 820
FPS = 60

BG_COLOR = (249, 172, 113)  # light brown
BG_IMG = "assets/backgrounds/bg_wood.jpg"

BOARD_SIZE = 800

PIECE_SIZE = (BOARD_SIZE // 8, BOARD_SIZE // 8)

pieces = {}
for fname in os.listdir("assets/sprites"):
    str_parts = fname.split("_")
    pieces[str_parts[0] + "_" + str_parts[1]] = fname
