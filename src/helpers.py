from vars import *
import chess

t_opposite = lambda t: (t[0] * -1, t[1] * -1)


def list_split(l, skip=1):
    return [l[i : i + skip] for i in range(0, len(l), skip)]


piece_indices = list_split(PIECE_INDICES, 8)


def t_to_uci(s_t: tuple, e_t: tuple):
    output = ""
    # print(piece_indices)
    # print(s_t, e_t)
    # print(len(piece_indices), len(piece_indices[0]))
    output += piece_indices[s_t[0]][-s_t[1] - 1]
    output += piece_indices[e_t[0]][-e_t[1] - 1]
    # print(output)
    return output


def uci_to_san(board, uci_str: str):
    move = chess.Move.from_uci(uci_str)
    return board.san(move)


def get_piece_properties(piece: str):
    return piece.isupper(), piece


if __name__ == "__main__":
    print(list_split([1, 2, 3, 4, 5, 6], 2))
