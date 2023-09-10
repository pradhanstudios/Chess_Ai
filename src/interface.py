import pygame


class Button:
    def __init__(self, preloaded_content, x, y, w, h, active=True):
        self.x = x
        self.y = y
        self.rect = pygame.rect.Rect(x, y, w, h)
        self.rect.x = self.x
        self.rect.y = self.y
        self.active = active
        self.preloaded_content = preloaded_content

    def draw(self, screen):
        screen.blit(self.preloaded_content, (self.x, self.y))

    def is_clicked(self, mouse_pos):
        if self.rect.collidepoint(mouse_pos) and self.active:
            return True
        return False


class ImageButton(Button):
    def __init__(self, content_fname: str, x, y, w, h, active=True):
        self.content = pygame.image.load(content_fname)
        self.content = pygame.transform.scale(self.content, (w, h))
        super().__init__(self.content, x, y, w, h, active)


class TextButton(Button):
    def __init__(self, font, text, x, y, w, h, active=True, color="black"):
        self.font = font
        self.text = text
        self.button_text = self.font.render(self.text, True, color)
        super().__init__(self.button_text, x, y, w, h, active)
