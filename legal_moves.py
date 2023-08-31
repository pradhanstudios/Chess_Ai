from vars import *

def _get_moves(board, cur_pos, dirx, diry, rng=8):
    output = []
    color, _, piece = board[cur_pos[0]][cur_pos[1]].partition("_")

    for i in range(rng):
        new_pos = (cur_pos[0] + dirx*i, cur_pos[1] + diry*i)
        # print(cur_pos[0], new_pos[1], i)
        # cur_square = board[new_pos[0]][new_pos[1]]
        # print(1 <= new_pos[1] < 7)

        if 0 <= cur_pos[0] + dirx*i < 8 and 0 <= cur_pos[1] + diry*i < 8:
            # print("here")
            if board[cur_pos[0] + dirx*i][cur_pos[1] + diry*i] == BLANK:
                output.append((cur_pos[0] + dirx*i, cur_pos[1] + diry*i))

            elif board[cur_pos[0] + dirx*i][cur_pos[1] + diry*i].split("_")[0] != color:
                output.append((cur_pos[0] + dirx*i, cur_pos[1] + diry*i))
        else:
            break

    return output


def get_possible_piece(board, cur_pos):

    color, _, piece = board[cur_pos[0]][cur_pos[1]].partition("_")
    piece_position = []

    if piece.lower() == "b": # bishop
        for i in (-1, 1):
            for j in (-1, 1):
                piece_position.extend(_get_moves(board, cur_pos, i, j))


    return piece_position


                
