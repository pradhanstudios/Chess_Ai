import sys
import json
from collections import deque

jsonfile = sys.argv[1]

with open(jsonfile, "r") as fh:
    data = json.load(fh)

mg_data = deque(data["mg_values.weight"][0])
eg_data = deque(data["eg_values.weight"][0])

def float_to_val(val):
    return round(val * 100)

def print_comment(comment):
    print(f"\t// {comment}")

def print_array(length, ends=False):
    average_mg = average_eg = 0
    for i in range(length):
        if (i % 8 == 0):
            print("\n\t", end="")
        mg = float_to_val(mg_data.popleft())
        average_mg += mg
        eg = float_to_val(eg_data.popleft())
        average_eg += eg
        print(f"S({mg}, {eg})", end=", ")
    if ends:
        print("\n};")
    else:
        print()
    average_mg /= length
    average_eg /= length
    return round(average_mg), round(average_eg)
def print_one():
    print(f"S({float_to_val(mg_data.popleft())}, {float_to_val(eg_data.popleft())});")

pawn_score_mg = pawn_score_mg = knight_score_mg = knight_score_eg = bishop_score_mg = bishop_score_eg = rook_score_mg = rook_score_eg = queen_score_mg = queen_score_eg = king_score_mg = king_score_eg = 0

print("#define S(mg, eg) ((mg * 0x10000) + eg) // compiler complains about bitwise operations on negative values\n\n")
print("const int PIECE_SQUARE_TABLES[384] = {")
print_comment("pawn\n")
print("\t" + "S(0, 0), " * 8, end="")
pawn_score_mg, pawn_score_eg = print_array(48)
print("\t" + "S(0, 0), " * 8)
print_comment("knight")
knight_score_mg, knight_score_eg = print_array(64)
print_comment("bishop")
bishop_score_mg, bishop_score_eg = print_array(64)
print_comment("rook")
rook_score_mg, rook_score_eg = print_array(64)
print_comment("queen")
queen_score_mg, queen_score_eg =  print_array(64)
print_comment("king")
king_score_mg, king_score_eg = print_array(64, ends=True)
print("\nconst int KING_RING_ATTACKS[6] = {", end="")
print_array(6, ends=True)
print("\nconst int KING_RING_DEFENSES[6] = {", end="")
print_array(5, ends=True)
print("\nconst int PASSED_PAWN_RANK[6] = {", end="")
print_array(6, ends=True)
print("\nconst int PROTECTED_PAWN_RANK[6] = {", end="")
print_array(6, ends=True)
print("\nconst int THREAT_BY_MINOR_PIECE[6] = {", end="")
print_array(6, ends=True)
print("\nconst int THREAT_BY_ROOK[6] = {", end="")
print_array(6, ends=True)
print("\nconst int PAWN_SHIELDS[3] = {", end="")
print_array(3, ends=True)
print("\nconst int ISOLATED_PAWN = ", end="")
print_one()
print("\nconst int DOUBLED_PAWN = ", end="")
print_one()
print("\nconst int ROOK_ON_OPEN_FILE = ", end="")
print_one()
print("\nconst int ROOK_ON_SEMI_OPEN_FILE = ", end="")
print_one()
print("\nconst int KING_ON_OPEN_FILE = ", end="")
print_one()
print("\nconst int KING_ON_SEMI_OPEN_FILE = ", end="")
print_one()
print("\nconst int BISHOP_PAIR = ", end="")
print_one()
print("\nconst int TEMPO = ", end="")
print_one()
print(f"\nconst int PIECE_VALUES[7] = {'{'}\n\tS(0, 0), S({pawn_score_mg}, {pawn_score_eg}), S({knight_score_mg}, {knight_score_eg}), S({bishop_score_mg}, {bishop_score_eg}), S({rook_score_mg}, {rook_score_eg}), S({queen_score_mg}, {queen_score_eg}), S(0, 0)\n{'}'};")
print("\n\n#undef S")