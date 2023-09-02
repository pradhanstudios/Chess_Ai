import pygame, sys
sys.path.append("scripts/")

from pygame.locals import *


from vars import *
from chess import Chess


class Game:
    def __init__(self):
        #########
        # setup #
        #########
        pygame.init()

        # window
        self.window = pygame.display.set_mode((WIDTH, HEIGHT))
        pygame.display.set_caption("Chess")

        # clock
        self.clock = pygame.time.Clock()

        # fonts

        # screens

        ################
        # game objects #
        ################
        self.window_bg = pygame.image.load(BG_IMG).convert()
        self.window_bg = pygame.transform.scale(self.window_bg, (WIDTH, HEIGHT))
        self.chess = Chess()

    def run(self):
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
            self.window.blit(self.window_bg, (0, 0))

            # update
            self.chess.update()

            # render
            self.chess.draw(self.window)

            # draw
            pygame.display.flip()
            self.clock.tick(FPS)

        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    Game().run()
