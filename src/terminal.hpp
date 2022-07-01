/**
 * @file terminal.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief A terminal class for CPShell, which handles all terminal input and output.
 * @version 1.1
 * @date 2022-06-09
 */

#pragma once

#include "../calc.hpp"
#include "../lib/functions/shapes.hpp"

// Terminal Class
class Terminal {
	public: 
		void ClearBuffer();
		void WriteBuffer(char c, bool hideCursor = true);
		void WriteChars(char* charArray, bool skipClear = false);
		void RemoveLast();
		void ShowCursor();
		void HideCursor();
		void SetFont(uint8_t* font);
		void SetColor(uint32_t newColor);
		void SetCursorColor(uint32_t newColor);
		// buffer for command input
		char bufferIn[BUF_SIZE];
		int8_t bufferInPos = 0;
		// buffer cursor position
		int16_t bufferCX = 0;
		int16_t bufferCY = 0;
		int16_t bufferOffsetX = 2;
		int16_t bufferOffsetY = 2;
		int16_t xmargin = 8; // character width
		int16_t ymargin = 9; // character height
		uint8_t* font;
		int16_t termWidth = width - xmargin * 2;
		int16_t termHeight = 40;
		int16_t xmax = termWidth / xmargin;
		uint32_t color = 0xFFFF; // white
		uint32_t cursorColor = 0xFFFF; // white
};

void Terminal::SetFont(uint8_t* font) {
	this->font = font;
}

void Terminal::ClearBuffer() {
	this->bufferInPos = 0;
	this->bufferCX = 0;
}

void Terminal::WriteBuffer(char c, bool hideCursor) {
	// hideCursor should only be false from WriteChars as writing to vram is slow
	if (hideCursor) this->HideCursor();
	// check if '\n'
	if (c == '\n') {
		this->ClearBuffer();
		this->bufferCY++;
		return;
	}

	if (this->bufferInPos < (BUF_SIZE - 1)) {
		// add the character to the buffer
		this->bufferIn[this->bufferInPos] = c;
		this->bufferInPos++;

		// buf of chars array that will have c in it
		char buf[2] = {c, '\0'};

		// draw the character
		DRAW_FONT(this->font, buf, this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, this->color, this->termWidth);

		// check for overflow on screen
		if (this->bufferCX < this->xmax) {
			this->bufferCX++;
		} else {
			this->bufferCX = 0;
			this->bufferCY++;
		}
	} else {
		Debug_Printf(1,1,true,0,"Buffer overflow!");
	}
}

/*
 * Usage
 * char myMessage[] = "Hello World!\nSupports newlines!";
 * Terminal* terminal;
 * terminal->WriteChars(myMessage);
 */
void Terminal::WriteChars(char *charArray, bool skipClear) {
	int len = strlen(charArray);
	for (int i = 0; i < len; i++) {
		this->WriteBuffer(charArray[i], false);
	}
	// clear the buffer
	if (!skipClear) this->ClearBuffer();
}

void Terminal::RemoveLast() {
	if (this->bufferInPos == 0) return;
	// remove the active cursor
	this->HideCursor();
	// check for wrapping
	if (this->bufferCX == 0) {
		this->bufferCY--;
		this->bufferCX = this->xmax;
	} else {
		this->bufferCX--;
	}

	this->bufferInPos--;
	// remove the last character from vram
	drawFilledRectangle(this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, this->xmargin, this->ymargin, 0); // 0 is always color black / 0,0,0
}

// Cursor Show
void Terminal::ShowCursor() {
	// draw the cursor
	drawFilledRectangle(this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, this->xmargin, this->ymargin, this->cursorColor);
}

// Cursor Hide
void Terminal::HideCursor() {
	// draw the cursor
	drawFilledRectangle(this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, this->xmargin, this->ymargin, 0); // 0 is always color black / 0,0,0
}

// Set Color
void Terminal::SetColor(uint32_t newColor) {
	this->color = newColor;
}

// Set Cursor Color
void Terminal::SetCursorColor(uint32_t newColor) {
	this->cursorColor = newColor;
}
