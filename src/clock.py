import time
from vars import *


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
