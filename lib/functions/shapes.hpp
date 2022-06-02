#pragma once
// basic shapes

// rectangle
void drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint32_t color) {
    line(x, y, x+width, y, color);
    line(x, y+height, x+width, y+height, color);
    line(x, y, x, y+height, color);
    line(x+width, y, x+width, y+height, color);
}
// filled rectangle
void drawFilledRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint32_t color) {
    for (int16_t i = x; i < x+width; i++) {
        for (int16_t j = y; j < y+height; j++) {
            setPixel(i, j, color);
        }
    }
}
