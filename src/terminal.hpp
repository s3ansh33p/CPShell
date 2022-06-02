#pragma once

#include "../calc.hpp"
#include "../lib/functions/shapes.hpp"

// Terminal Class
class Terminal {
	public: 
		void ClearBuffer();
		void WriteBuffer(char c);
		void WriteChars(char* charArray);
		void RemoveLast();
		void SetFont(uint8_t* font);
		// buffer for command input
		char bufferIn[BUF_SIZE];
		int8_t bufferInPos = 0;
		// buffer cursor position
		int16_t bufferCX = 0;
		int16_t bufferCY = 0;
		int16_t bufferOffsetX = 8;
		int16_t bufferOffsetY = 8;
		int16_t xmargin = 10;
		int16_t ymargin = 10;
		uint8_t* font;
		int16_t termWidth = width - xmargin * 2;
		int16_t termHeight = 40;
		int16_t xmax = termWidth / xmargin;
};

void Terminal::SetFont(uint8_t* font) {
	this->font = font;
}

void Terminal::ClearBuffer() {
	this->bufferInPos = 0;
	this->bufferCX = 0;
}

void Terminal::WriteBuffer(char c) {
	// check if '\n'
	if (c == '\n') {
		this->bufferCY++;
		this->bufferCX = 0;
		this->ClearBuffer();
		return;
	}

	if (this->bufferInPos < (BUF_SIZE - 1)) {
		// add the character to the buffer
		this->bufferIn[this->bufferInPos] = c;
		this->bufferInPos++;

		// buf of chars array that will have c in it
		char buf[2] = {c, '\0'};

		// draw the character
		DRAW_FONT(this->font, buf, this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, color(255,255,255), this->termWidth);

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
void Terminal::WriteChars(char *charArray) {
	int len = strlen(charArray);
	for (int i = 0; i < len; i++) {
		this->WriteBuffer(charArray[i]);
	}
	// clear the buffer
	this->ClearBuffer();
}

void Terminal::RemoveLast() {
	if (this->bufferInPos == 0) return;
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
