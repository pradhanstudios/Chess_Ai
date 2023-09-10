import pygame


class Button:
    def __init__(self, content_fname: str, x, y, w, h):
        self.content = pygame.image.load(content_fname)
        self.content = pygame.transform.scale(self.content, (w, h))
        self.x = x
        self.y = y
        self.rect = self.content.get_rect()
        self.rect.x = self.x
        self.rect.y = self.y

    def draw(self, screen):
        screen.blit(self.content, (self.x, self.y))
