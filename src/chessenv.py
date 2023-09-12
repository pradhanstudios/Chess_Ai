# the chess environment for our ai
import chess
import pygame
from vars import *
from helpers import *


class ChessEnv:
    def __init__(self, start_fen: str = ""):
        # pygame stuff
        # surface
        self.surface = pygame.surface.Surface((BOARD_SIZE, BOARD_SIZE))
        self.rect = self.surface.get_rect(
            x=(WIDTH // 120), y=((HEIGHT // 2) - (BOARD_SIZE // 2))
        )
        # board
        self.board_img = pygame.image.load(BOARD_IMG).convert_alpha()
        self.board_img = pygame.transform.scale(
            self.board_img, (BOARD_SIZE, BOARD_SIZE)
        )
        self.piece_sprites = {}
        for k, piece_file in FILE_NAMES.items():
            self.piece_sprites[k] = pygame.image.load(
                ASSET_FILENAME + piece_file + ASSET_ENDNAME
            ).convert_alpha()
            self.piece_sprites[k] = pygame.transform.scale(
                self.piece_sprites[k], (PIECE_SIZE * 0.75, PIECE_SIZE * 0.75)
            )
        # print(self.piece_sprites)

        self.start = (-1, -1)
        self.dest = (-1, -1)
        # self.turn = 0
        # actual board
        if not start_fen:
            self.board = chess.Board()
        else:
            self.board = chess.Board(start_fen)
        self.legal_moves = self.board.legal_moves

        self.pieces = self.get_all_pieces_2D()

    def __call__(self):
        print(self.board)

    def update_legal_moves(self):
        self.legal_moves = self.board.legal_moves

    def get_move(self):
        return not len(self.board.move_stack) % 2

    def move_piece(self, start_cell, end_cell):
        uci = t_to_uci(start_cell, end_cell)

        move = chess.Move.from_uci(uci)

        if self.is_promotion(uci, self.get_move()):
            print("PROMOTION")
            return True, uci

        if move in self.board.legal_moves:
            self.board.push(move)
        else:
            print("Illegal move")

        return False

    def outcome(self):
        return self.board.outcome()

    def get_all_pieces_2D(self):
        piece_map = self.board.piece_map()
        output = [BLANK for _ in range(64)]
        # print(output)
        for i in range(64):
            if i in piece_map:
                inserting = str(piece_map[i])
                if inserting != "":
                    output[i] = inserting

        return list_split(output, 8)

    def draw(self, screen):
        offset = 0
        screen.blit(self.surface, self.rect)
        self.surface.fill(BG_COLOR)

        # draw board
        self.surface.blit(self.board_img, (0, 0))

        # draw pieces based on self.board
        for r in range(len(self.pieces)):
            for c in range(len(self.pieces[r])):
                piece = self.pieces[r][c]
                if piece != BLANK:
                    if (c, r) == self.start:
                        offset = 5
                    self.surface.blit(
                        self.piece_sprites[piece],
                        self.piece_sprites[piece].get_rect(
                            center=(
                                c * PIECE_SIZE + PIECE_SIZE / 2,
                                r * PIECE_SIZE + PIECE_SIZE / 2 - offset,
                            )
                        ),
                    )
                    offset = 0

    def update_squares(self, tup):
        if self.rect.collidepoint(tup):
            tupx = tup[0] - self.rect.x
            tupy = tup[1] - self.rect.y

            i = tupx // PIECE_SIZE
            j = tupy // PIECE_SIZE

            # print(f"{tup} -> ({tupx}, {tupy}) -> ({i}, {j})")

            print(f"\n=== MOVE: ({i}, {j}) ===")

            print(f"START:\t{self.start} and {self.dest}")

            if self.start == (-1, -1):
                print("start update")
                if self.pieces[j][i] != BLANK:
                    self.start = (i, j)
            elif self.dest == (-1, -1) and self.start != (i, j):
                print("dest update")
                self.dest = (i, j)

                # print(
                #     f"{self.start} -> {self.dest} -> {t_to_uci(self.start, self.dest)}"
                # )
                log = self.move_piece(self.start, self.dest)

                # print(self.board)

                self.start = (-1, -1)
                self.dest = (-1, -1)
                # updating
                self.legal_moves = self.update_legal_moves()
                self.pieces = self.get_all_pieces_2D()
                if log:
                    return True, log[1]
            else:
                self.start = (i, j)
                self.dest = (-1, -1)

            print(f"END:\t{self.start} and {self.dest}")
            return False

    def game_over(self, cur_player_clock) -> str:
        # return (
        #     self.board.is_checkmate()  # checkmate
        #     or self.board.is_stalemate()  # stalemate
        #     or self.board.is_insufficient_material()  # insufficient matierial
        #     or self.board.can_claim_threefold_repetition()  # if the same moves have been repeated 3 times in a row
        #     or self.board.can_claim_fifty_moves()  # fifty moves after a pawn has movesd
        # )
        if self.board.is_checkmate():
            return f"checkmate; {COLORS[not self.get_move()]} wins."
        if self.board.is_stalemate():
            return "stalemate; it is a tie."
        if self.board.is_insufficient_material():
            return "insufficient material; it is a tie."
        if self.board.can_claim_threefold_repetition():
            return "threefold repetition; it is a tie."
        if self.board.can_claim_fifty_moves():
            return "fifty moves rule; it is a tie."
        if cur_player_clock.done():
            return f"timeout; {COLORS[not self.get_move()]} wins."

    def update(self):
        if pygame.mouse.get_pressed()[0]:
            hit = pygame.mouse.get_pos()
            # print(hit)
            if self.rect.collidepoint(hit):
                hitx = hit[0] - self.rect.x
                hity = hit[1] - self.rect.y

                i = hitx // PIECE_SIZE
                j = hity // PIECE_SIZE

                # print(f"{pygame.mouse.get_pos()} --> ({hitx}, {hity}) --> [{i}][{j}]")

                if self.start == (-1, -1):
                    if self.pieces[i][j] != BLANK:
                        self.start = (i, j)

                if (
                    (i, j) != self.start
                    and self.start != (-1, -1)
                    and self.dest == (-1, -1)
                ):
                    self.dest = (i, j)

                    # print(
                    #     f"{self.start}: {self.board[self.start[1]][self.start[0]]}\t-->\t{self.dest}: {self.board[j][i]}\n{get_algebraic_notation(self.board, self.start, self.dest)}"
                    # )

                    self.move_piece(self.start, self.dest)

                    self.start = (-1, -1)
                    self.dest = (-1, -1)
                    # updating
                    self.legal_moves = self.update_legal_moves()
                    self.pieces = self.get_all_pieces_2D()
                    # turn = (turn + 1) %

    def is_promotion(self, uci, color):
        # print(self.legal_moves)
        self.update_legal_moves()
        return (
            chess.Move.from_uci(uci + "q") in self.legal_moves
            or chess.Move.from_uci(uci + "r") in self.legal_moves
            or chess.Move.from_uci(uci + "b") in self.legal_moves
            or chess.Move.from_uci(uci + "n") in self.legal_moves
        )

    # def draw_board(self)


if __name__ == "__main__":
    envchess = ChessEnv()
    envchess()
