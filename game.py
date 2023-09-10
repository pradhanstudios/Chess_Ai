import pygame, sys
from pygame.locals import *

sys.path.append("src/")
from chessenv import ChessEnv
from vars import *
from interface import *


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

        # buttons
        self.knight_button = Button(
            f"{ASSET_FILENAME}w_knight{ASSET_ENDNAME}", 1000, 350, 100, 100
        )
        ################
        # game objects #
        ################
        self.window_bg = pygame.image.load(BG_IMG).convert()
        self.window_bg = pygame.transform.scale(self.window_bg, (WIDTH, HEIGHT))
        self.chess = ChessEnv()

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
                if event.type == MOUSEBUTTONDOWN:
                    if event.button == 1:
                        hit = pygame.mouse.get_pos()
                        # print(hit)
                        self.chess.update_squares(hit)
                    if self.knight_button.rect.collidepoint(pygame.mouse.get_pos()):
                        print("NOICE")

            # clear last frame
            self.window.blit(self.window_bg, (0, 0))
            self.knight_button.draw(self.window)

            # update
            if self.chess.game_over():
                running = False

            # render
            self.chess.draw(self.window)

            # draw
            pygame.display.flip()
            self.clock.tick(FPS)
        outcome = self.chess.outcome()
        if outcome:
            print(f"The game ends due to: {str(outcome.termination)[12:]}!")

        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    Game().run()
