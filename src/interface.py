import pygame


class Button:
    def __init__(self, x, y, w, h, active=True):
        self.x = x
        self.y = y
        self.rect = pygame.rect.Rect(x, y, w, h)
        self.rect.x = self.x
        self.rect.y = self.y
        self.active = active

    def is_clicked(self, mouse_pos):
        if self.rect.collidepoint(mouse_pos) and self.active:
            return True
        return False


class ImageButton(Button):
    def __init__(self, content_fname: str, x, y, w, h, active=True):
        super().__init__(x, y, w, h, active)
        self.content = pygame.image.load(content_fname)
        self.content = pygame.transform.scale(self.content, (w, h))

    def draw(self, screen):
        screen.blit(self.content, (self.x, self.y))


class TextButton(Button):
    def __init__(self, font, text, x, y, w, h, active=True, color="black"):
        super().__init__(x, y, w, h, active)
        self.font = font
        self.text = text
        self.button_text = self.font.render(self.text, True, color)

    def draw(self, screen):
        screen.blit(self.button_text, (self.x, self.y))
