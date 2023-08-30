import pygame
from pygame.locals import *
from consts import *
from board import Board

#########
# setup #
#########
pygame.init()

# window
window = pygame.display.set_mode((WIDTH, HEIGHT))  # 1280 x 720
pygame.display.set_caption("Chess")

# clock
clock = pygame.time.Clock()

# fonts

# screens

################
# game objects #
################
window_bg = pygame.image.load(BG_IMG)
board = Board(WIDTH, HEIGHT)

#############
# game loop #
#############
running = True
while running:
    # event loop
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    # clear last frame
    # window.fill(BG_COLOR)
    window.blit(window_bg, (0, 0))

    # update

    # render
    board.draw(window)

    # draw
    pygame.display.flip()
    clock.tick(FPS)

pygame.quit()
