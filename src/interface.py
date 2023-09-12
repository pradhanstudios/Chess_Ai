import pygame
from vars import *
import time


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
        if self.active:
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
    def __init__(
        self,
        font,
        text,
        x,
        y,
        w,
        h,
        active=True,
        color="black",
        background_color="white",
    ):
        self.font = font
        self.text = text
        self.button_text = self.font.render(self.text, True, color, background_color)
        super().__init__(self.button_text, x, y, w, h, active)


class Clock:
    def __init__(
        self,
        font,
        starting_time,
        x,
        y,
        tick=1 / FPS,
        color="black",
        background_color="white",
    ):
        self.starting_time = starting_time
        self.t = tick
        self.x = x
        self.y = y
        self.font = font
        self.color = color
        self.background_color = background_color
        self.text = self._render_font()
        self.text_area = self.text.get_rect()
        self.text_area.topleft = (x, y)

    def _format_time(self):
        return "{:.0f}:{:.2f}".format(self.starting_time // 60, self.starting_time % 60)

    def _render_font(self):
        return self.font.render(
            self._format_time(), True, self.color, self.background_color
        )

    def tick(self):
        self.starting_time -= self.t

    def draw(self, screen):
        self.text = self._render_font()
        screen.blit(self.text, self.text_area)

    def done(self):
        return self.starting_time <= 0


class Text:
    def __init__(self, font, text, x, y, color="black", background_color="white"):
        self.font = font
        self.text = text
        self.x = x
        self.y = y
        self.color = color
        self.background_color = background_color
        self.rect = self._render_text().get_rect(topleft=(x, y))

    def _render_text(self):
        return self.font.render(self.text, True, self.color, self.background_color)

    def draw(self, screen):
        screen.blit(self._render_text(), self.rect)


def is_active(obj):
    return obj.active


def is_active_dict(obj):
    output = []
    for k, v in obj:
        output.append(is_active(v))
    return not all(not i for i in output)
