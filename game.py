import pygame, sys
from pygame.locals import *

sys.path.append("src/")
from chessenv import ChessEnv
from vars import *
from interface import *
from clock import *


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
        self.font = pygame.font.SysFont("arial", FONT_SIZE)

        # buttons
        self.buttons = {}
        print(FILE_NAMES.items())
        for key, value in FILE_NAMES.items():
            self.buttons[key] = ImageButton(
                f"{ASSET_FILENAME}{value}{ASSET_ENDNAME}",
                HEIGHT,
                PIECE_POS[key.lower()],
                PIECE_SIZE,
                PIECE_SIZE,
                active=False,
            )

        ################
        # game objects #
        ################
        self.window_bg = pygame.image.load(BG_IMG).convert()
        self.window_bg = pygame.transform.scale(self.window_bg, (WIDTH, HEIGHT))
        self.chess = ChessEnv()
        self.cur_uci = None
        self.player_1_clock = Clock(
            font=self.font,
            starting_time=600,
            x=HEIGHT + 200,
            y=250,
            tick=1 / FPS,
            color="black",
        )
        self.player_2_clock = Clock(
            font=self.font,
            starting_time=600,
            x=HEIGHT + 200,
            y=400,
            tick=1 / FPS,
            color="black",
        )

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
                    if event.button == 1 and not self.cur_uci:
                        hit = pygame.mouse.get_pos()
                        # print(hit)
                        l = self.chess.update_squares(hit)
                        if l:
                            self.cur_uci = l[1]
                            if self.chess.get_move():  # white
                                for k, v in self.buttons.items():
                                    if k.isupper():
                                        v.active = True
                            else:
                                for k, v in self.buttons.items():
                                    if k.islower():
                                        v.active = True
                    else:
                        mouse_pos = pygame.mouse.get_pos()
                        for k, v in self.buttons.items():
                            if v.is_clicked(mouse_pos):
                                flag = True
                                print(self.cur_uci, k.lower())
                                self.cur_uci += k.lower()
                                move = chess.Move.from_uci(self.cur_uci)
                                self.chess.board.push(move)
                                self.cur_uci = None
                                self.chess.update_legal_moves()
                                self.chess.pieces = self.chess.get_all_pieces_2D()
                        if flag:
                            for k, v in self.buttons.items():
                                v.active = False

            # clear last frame
            self.window.blit(self.window_bg, (0, 0))
            # print(self.buttons.items())
            for k, v in self.buttons.items():
                if v.active:
                    v.draw(self.window)

            # update
            if end := self.chess.game_over(self.player_1_clock):
                running = False
            if self.chess.get_move():
                self.player_1_clock.tick()
            else:
                self.player_2_clock.tick()
            self.player_1_clock.draw(self.window)
            self.player_2_clock.draw(self.window)

            # render
            self.chess.draw(self.window)

            # draw
            pygame.display.flip()
            self.clock.tick(FPS)
        if end:
            print("#############################")
            print("the game ends due to: " + end)

        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    Game().run()
