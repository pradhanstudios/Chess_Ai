import pygame
from pygame.locals import *
from random import choice

##########
# consts #
##########
WIDTH, HEIGHT = 700, 800
UP, DOWN = -1, 1
LEFT, RIGHT = -1, 1


###########
# classes #
###########
class Player:
    def __init__(self):
        self.bullet_list = []

        self.lives = 3
        self.speed = 5

        self.surface = pygame.surface.Surface((60, 20))
        self.surface.fill("green")

        self.rect = self.surface.get_rect(center=(WIDTH // 2, 650))

    def move(self, keys) -> None:
        if keys[K_a] or keys[K_LEFT]:
            self.rect.x -= self.speed
        if keys[K_d] or keys[K_RIGHT]:
            self.rect.x += self.speed

        if self.rect.left < 0:
            self.rect.left = 0
        if self.rect.right > WIDTH:
            self.rect.right = WIDTH

        for bullet in self.bullet_list:
            bullet.move()

            if bullet.rect.bottom < 0:
                self.rm_bullet(bullet)

    def rm_bullet(self, bullet):
        self.bullet_list.remove(bullet)
        # del bullet

    def shoot(self) -> None:
        if len(self.bullet_list) == 0:
            self.bullet_list.append(Bullet(self.rect.midtop, UP))

    def draw_bullets(self, surface):
        for bullet in self.bullet_list:
            bullet.draw(surface)


class Bullet:
    def __init__(self, start_pos: tuple, direction: int):
        self.start_pos = start_pos
        self.direction = direction
        self.speed = 10

        self.surface = pygame.surface.Surface((20, 15))
        if self.direction == UP:
            self.surface.fill("green")
        elif self.direction == DOWN:
            self.surface.fill("white")
        else:
            self.surface.fill("red")

        self.rect = self.surface.get_rect(center=start_pos)

    def move(self):
        self.rect.y += self.direction * self.speed

    def draw(self, surface):
        surface.blit(self.surface, self.rect)


class Enemy:
    def __init__(self, start_pos: tuple, direction: int):
        self.start_row = True
        self.start_pos = start_pos
        self.direction = direction
        self.speed = 10

        self.range = 250
        self.start_col = start_pos[0]
        self.end_col = start_pos[0] + self.range

        self.surface = pygame.surface.Surface((40, 20))
        self.surface.fill("white")

        self.rect = self.surface.get_rect(center=start_pos)

    def move(self):
        if not self.start_row:
            if self.rect.centerx >= self.end_col or self.rect.centerx <= self.start_col:
                self.rect.y += 40
                self.direction *= -1
                self.speed += 1

            self.rect.centerx += self.speed * self.direction
        else:
            if self.rect.centerx >= self.end_col:
                self.rect.y += 40
                self.direction *= -1
                self.start_row = False
                self.speed += 1

            self.rect.centerx += self.speed * self.direction

    def shoot(self):
        enemy_bullet_list.append(Bullet(self.rect.midbottom, DOWN))


#########
# setup #
#########
pygame.init()

# window
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Space Invaders")

# clock
clock = pygame.time.Clock()

# fonts
font1 = pygame.font.Font("assets/fonts/PressStart2P.ttf", 20)
font2 = pygame.font.Font("assets/fonts/PressStart2P.ttf", 14)

################
# game objects #
################

# game info
score = 0

# surfaces
status_bar = pygame.surface.Surface((WIDTH, 50))
game_screen = pygame.surface.Surface((WIDTH, 700))
instr_bar = pygame.surface.Surface((WIDTH, 50))

# player
p1 = Player()

# list of enemies
enemies = [
    Enemy((x, y), RIGHT) for x in range(50, 450, 50) for y in range(100, 300, 40)
]
enemy_bullet_list = []

#############
# game loop #
#############
frame_counter = 0
running = True
while running:
    ##############
    # event loop #
    ##############
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    # clear last frame

    ##################
    # update objects #
    ##################
    pressed_keys = pygame.key.get_pressed()

    now = pygame.time.get_ticks()
    p1.move(pressed_keys)
    if p1.lives <= 0:
        running = False

    if pressed_keys[K_SPACE]:
        p1.shoot()
    # print(len(enemy_bullet_list))
    if frame_counter % 120 == 0 and len(enemy_bullet_list) < 3:
        choice(enemies).shoot()

    if frame_counter % 40 == 0:
        for enemy in enemies:
            if enemy:
                if enemy.rect.bottom >= 700:
                    running = False
                # print(enemy.rect.bottom)
                enemy.move()

    ########
    # draw #
    ########

    # status bar
    screen.blit(status_bar, (0, 0))
    status_bar.fill("black")
    score_text = font1.render(f"SCORE: {score}", True, "white")
    status_bar.blit(score_text, (10, 15))
    lives_text = font1.render(f"LIVES: {p1.lives}", True, "white")
    status_bar.blit(lives_text, (WIDTH - font1.size(f"LIVES: {p1.lives}")[0] - 10, 15))
    pygame.draw.aaline(status_bar, "white", (0, 49), (WIDTH, 49))

    # game screen
    screen.blit(game_screen, (0, 50))
    game_screen.fill("black")

    # player
    game_screen.blit(p1.surface, p1.rect)
    # bullets
    for bullet in enemy_bullet_list:
        bullet.move()
        bullet.draw(game_screen)
    p1.draw_bullets(game_screen)

    for enemy in enemies:
        game_screen.blit(enemy.surface, enemy.rect)

    if not enemies:
        running = False

    # print(p1.rect.collidelistall(enemies))
    if p1.rect.collidelistall(enemies):
        running = False

    if len(p1.bullet_list) > 0:
        for enemy in p1.bullet_list[0].rect.collidelistall(enemies):
            # enemies.remove(enemy)
            del enemies[enemy]
            p1.rm_bullet(p1.bullet_list[0])

    for bul in enemy_bullet_list:
        if bul.rect.colliderect(p1.rect):
            enemy_bullet_list.remove(bul)
            p1.lives -= 1
        if bul.rect.y >= HEIGHT:
            enemy_bullet_list.remove(bul)
        for bullet in p1.bullet_list:
            if bullet.rect.colliderect(bul.rect):
                p1.bullet_list.remove(bullet)
                enemy_bullet_list.remove(bul)

    # instructions bar
    screen.blit(instr_bar, (0, 750))
    instr_bar.fill("black")
    pygame.draw.aaline(instr_bar, "white", (0, 0), (WIDTH, 0))
    move_text = font2.render(
        "Use <- and -> or A and D to move left and right", True, "white"
    )
    instr_bar.blit(
        move_text,
        (
            (WIDTH // 2)
            - (font2.size("Use <- and -> or A and D to move left and right")[0] // 2),
            5,
        ),
    ),
    shoot_text = font2.render("Use SPACEBAR to shoot", True, "white")
    instr_bar.blit(
        shoot_text,
        ((WIDTH // 2) - (font2.size("Use SPACEBAR to shoot")[0] // 2), 28),
    )

    ##########
    # render #
    ##########
    pygame.display.flip()

    frame_counter += 1
    if frame_counter == 80:
        frame_counter = 0
    clock.tick(60)


pygame.quit()
