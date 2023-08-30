import pygame
from vars import *


class Board:
    def __init__(self, w, h):
        self.side_len = BOARD_SIZE
        self.surface = pygame.surface.Surface((self.side_len, self.side_len))
        self.rect = self.surface.get_rect(
            x=(w // 120), y=((h // 2) - (self.side_len // 2))
        )

        self.board = [["" for _ in range(8)] for _ in range(8)]

    def draw(self, screen):
        self.surface.fill((103, 51, 20))

        screen.blit(self.surface, self.rect)

    def print_board(self):
        for row in self.board:
            print(row)
