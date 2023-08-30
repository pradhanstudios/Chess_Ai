import pygame
from vars import *


class Board:
    def __init__(self):
        self.surface = pygame.surface.Surface((BOARD_SIZE, BOARD_SIZE))
        self.rect = self.surface.get_rect(
            x=(WIDTH // 120), y=((HEIGHT // 2) - (BOARD_SIZE // 2))
        )

        # sub-surfaces
        self.border_factor = 20
        self.chess_board = pygame.surface.Surface(
            (
                (BOARD_SIZE - (BOARD_SIZE // self.border_factor)),
                (BOARD_SIZE - (BOARD_SIZE // self.border_factor)),
            )
        )

        self.board_img = pygame.image.load(BOARD_IMG)
        self.board_img = pygame.transform.scale(
            self.board_img,
            (
                BOARD_SIZE - (BOARD_SIZE // self.border_factor),
                BOARD_SIZE - (BOARD_SIZE // self.border_factor),
            ),
        )

        self.board = [["" for _ in range(8)] for _ in range(8)]

    def draw(self, screen):
        screen.blit(self.surface, self.rect)
        self.surface.fill(BG_COLOR)

        self.surface.blit(
            self.chess_board,
            (
                ((BOARD_SIZE // self.border_factor) // 2),
                ((BOARD_SIZE // self.border_factor) // 2),
            ),
        )

        self.chess_board.blit(self.board_img, (0, 0))

    def print_board(self):
        for row in self.board:
            print(row)
