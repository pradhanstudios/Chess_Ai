import pygame
from vars import *


class Board:
    def __init__(self):
        self.surface = pygame.surface.Surface((BOARD_SIZE, BOARD_SIZE))
        self.rect = self.surface.get_rect(
            x=(WIDTH // 120), y=((HEIGHT // 2) - (BOARD_SIZE // 2))
        )

        self.board_img = pygame.image.load(BOARD_IMG)
        self.board_img = pygame.transform.scale(
            self.board_img, (BOARD_SIZE, BOARD_SIZE)
        )

        self.piece_sprites = {}
        for piece in pieces:
            self.piece_sprites[piece] = pygame.image.load(
                "assets/sprites/" + pieces[piece]
            )
            self.piece_sprites[piece] = pygame.transform.scale(
                self.piece_sprites[piece], (PIECE_SIZE * 0.75, PIECE_SIZE * 0.75)
            )

        print(self.piece_sprites)

        self.board = [["   " for _ in range(8)] for _ in range(8)]

        self.parse_FEN(START_FEN)

        self.print_board()

    def draw(self, screen):
        # draw game surface onto window
        screen.blit(self.surface, self.rect)
        self.surface.fill(BG_COLOR)

        # draw board
        self.surface.blit(self.board_img, (0, 0))

        # draw pieces based on self.board
        for r in range(len(self.board)):
            for c in range(len(self.board[r])):
                piece = self.board[r][c]
                if piece != "   ":
                    # print(
                    #     f"({r}, {c}) -- {piece} : ({r * PIECE_SIZE}, {c * PIECE_SIZE})"
                    # )
                    self.surface.blit(
                        self.piece_sprites[piece],
                        self.piece_sprites[piece].get_rect(
                            center=(
                                c * PIECE_SIZE + PIECE_SIZE / 2,
                                r * PIECE_SIZE + PIECE_SIZE / 2,
                            )
                        ),
                    )

    def print_board(self):
        for row in self.board:
            print(row)

    def parse_FEN(self, fen):
        rows = fen.split("/")

        w = "w_"
        b = "b_"

        for r in range(len(rows)):
            col_ptr = 0
            for c in rows[r]:
                if c.isnumeric():
                    col_ptr += int(c)

                elif c.isupper():
                    self.board[r][col_ptr] = w + c
                    col_ptr += 1

                else:
                    self.board[r][col_ptr] = b + c
                    col_ptr += 1

                if col_ptr > 8:
                    break
