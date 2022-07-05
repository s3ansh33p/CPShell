/**
 * @file scrollbar.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief A vertical scrollbar class for CPShell, may merge with terminal class later.
 * @version 1.0
 * @date 2022-07-05
 */

#pragma once

#include "../calc.hpp"
#include "../lib/functions/shapes.hpp"

// Scrollbar Class
class Scrollbar {
    public: 
        // x and y
        int16_t x = (width - 13);
        int16_t y = 0;
        // width and height
        int16_t w = 12;
        int16_t h = (height - 1);
        // scrollbar colors
        uint32_t backgroundColor;
        uint32_t handleColor;
        // scrollbar y position
        int16_t position = 10;
        // scrollbar size (height)
        int16_t size = 100;
        // previous position vars
        int16_t prevScrollOffset = 0;
        void Render();
        void SetPosition(int16_t newY);
        void SetSize(int16_t sz);
        void SetColors(uint32_t bg, uint32_t fg);
};

void Scrollbar::Render() {
    drawRectangle(this->x, this->y, this->w, this->h, this->backgroundColor);
    drawFilledRectangle(this->x + 2, this->y + this->position, this->w-2, this->size, this->handleColor);   
}

void Scrollbar::SetPosition(int16_t newY) {
    this->position = newY;
    if (this->position < 0) {
        this->position = 0;
    } else if (this->position > this->h - this->size) {
        this->position = this->h - this->size;
    }
}

void Scrollbar::SetSize(int16_t sz) {
    this->size = sz;
    if (this->size < 0) {
        this->size = 0;
    } else if (this->size > this->h) {
        this->size = this->h;
    }
}

void Scrollbar::SetColors(uint32_t bg, uint32_t fg) {
    this->backgroundColor = bg;
    this->handleColor = fg;
}
