import pygame
from pygame.locals import *
from consts import *

#########
# setup #
#########
pygame.init()

# window
window = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Chess")

# clock
clock = pygame.time.Clock()

# fonts

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
    window.fill("black")

    # update

    # render

    # draw
    pygame.display.flip()
    clock.tick(FPS)

pygame.quit()
