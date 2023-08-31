import os

WIDTH, HEIGHT = 1280, 720
FPS = 60

BG_COLOR = (249, 172, 113)  # light brown
BG_IMG = "assets/backgrounds/bg_wood.jpg"

BOARD_IMG = "assets/backgrounds/chess_board.png"
BOARD_SIZE = (HEIGHT // 100) * 100 if HEIGHT <= WIDTH else (WIDTH // 100) * 100

START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

PIECE_SIZE = BOARD_SIZE // 8

BLANK = "   "


pieces = {}
for fname in os.listdir("assets/sprites"):
    str_parts = fname.split("_")

    upper_bool = False
    val = ""

    if str_parts[0] == "b":
        val += "b_"
    else:
        val += "w_"
        upper_bool = True

    next_char = ""

    if str_parts[1][0] == "k":
        if str_parts[1][1] == "i":
            next_char = "k"
        else:
            next_char = str_parts[1][1]
    else:
        next_char = str_parts[1][0]

    next_char = next_char.upper() if upper_bool else next_char

    val += next_char

    pieces[val] = fname
