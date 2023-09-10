from vars import *
import chess


def list_split(l, skip=1):
    return [l[i : i + skip] for i in range(0, len(l), skip)]


piece_indexes = list_split(PIECE_INDICES, 8)


def t_to_uci(s_t: tuple, e_t: tuple):
    output = ""
    # print(piece_indexes)
    output += piece_indexes[s_t[0]][s_t[1]]
    output += piece_indexes[e_t[0]][e_t[1]]
    print(output)
    return output


def uci_to_san(board, uci_str: str):
    move = chess.Move.from_uci(uci_str)
    return board.san(move)


if __name__ == "__main__":
    print(list_split([1, 2, 3, 4, 5, 6], 2))
