import pygame

# from scripts.vars import *
# from scripts.legal_moves import get_possible_piece, get_algebraic_notation
from legal_moves import *
from vars import *


class Chess:
    def __init__(self):
        # surface
        self.surface = pygame.surface.Surface((BOARD_SIZE, BOARD_SIZE))
        self.rect = self.surface.get_rect(
            x=(WIDTH // 120), y=((HEIGHT // 2) - (BOARD_SIZE // 2))
        )

        # board
        self.board_img = pygame.image.load(BOARD_IMG)
        self.board_img = pygame.transform.scale(
            self.board_img, (BOARD_SIZE, BOARD_SIZE)
        )

        # pieces
        self.piece_sprites = {}
        for piece in pieces:
            self.piece_sprites[piece] = pygame.image.load(
                "assets/sprites/" + pieces[piece]
            )
            self.piece_sprites[piece] = pygame.transform.scale(
                self.piece_sprites[piece], (PIECE_SIZE * 0.75, PIECE_SIZE * 0.75)
            )

        # vars to move pieces
        self.start = (-1, -1)
        self.dest = (-1, -1)

        self.board = [[BLANK for _ in range(8)] for _ in range(8)]

        self.parse_FEN(START_FEN)

        # self.print_board()

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
                if piece != BLANK:
                    self.surface.blit(
                        self.piece_sprites[piece],
                        self.piece_sprites[piece].get_rect(
                            center=(
                                c * PIECE_SIZE + PIECE_SIZE / 2,
                                r * PIECE_SIZE + PIECE_SIZE / 2,
                            )
                        ),
                    )

    def update(self):
        if pygame.mouse.get_pressed()[0]:
            hit = pygame.mouse.get_pos()
            if self.rect.collidepoint(hit):
                hitx = hit[0] - self.rect.x
                hity = hit[1] - self.rect.y

                i = hitx // PIECE_SIZE
                j = hity // PIECE_SIZE

                # print(f"{pygame.mouse.get_pos()} --> ({hitx}, {hity}) --> [{i}][{j}]")

                if self.start == (-1, -1):
                    if self.board[j][i] != BLANK:
                        print("HERE")
                        self.start = (i, j)

                if (
                    (i, j) != self.start
                    and self.start != (-1, -1)
                    and self.dest == (-1, -1)
                ):
                    self.dest = (i, j)

                    print(
                        f"{self.start}: {self.board[self.start[1]][self.start[0]]}\t-->\t{self.dest}: {self.board[j][i]}\n{get_algebraic_notation(self.board, self.start, self.dest)}"
                    )

                    self.move_piece(self.start, self.dest)

                    self.start = (-1, -1)
                    self.dest = (-1, -1)

    def move_piece(self, start, dest):
        start_y, start_x = start
        dest_y, dest_x = dest
        # cboard = self.board

        cboard = []
        for r in range(8):
            new_row = []
            for c in range(8):
                new_row.append(self.board[r][c])
            cboard.append(new_row)

        piece = self.board[start_x][start_y]

        if "q" in piece or "Q" in piece:
            print(get_possible_piece(cboard, (start_x, start_y)))

        if (dest_x, dest_y) in get_possible_piece(cboard, (start_x, start_y)):
            cboard[dest_x][dest_y] = piece
            cboard[start_x][start_y] = BLANK
            # print(f"flag: {cboard == self.board}")

        if not is_check(
            cboard, find_king(cboard, (cboard[dest_x][dest_y]).split("_")[0])
        ):
            # print("this")
            self.board = cboard

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


if __name__ == "__main__":
    chess = Chess()
    print(get_all_possible(chess.board))
