#pragma once

#include "../calc.hpp"

// Terminal Class
class Terminal {
	public: 
		void ClearBuffer();
		void OutputBuffer();
		void WriteBuffer(char c, bool removeLast = false);
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
	bufferInPos = 0;
}

void Terminal::WriteBuffer(char c, bool removeLast) {
	// check if '\n'
	if (c == '\n') {
		bufferCY++;
		bufferCX = 0;
		return;
	}

	char newBuffer[BUF_SIZE] = {0};
	for (int i = 0; i < bufferInPos; i++) {
		newBuffer[i] = bufferIn[i];
	}
	if (bufferInPos < (BUF_SIZE - 1)) {
		// previous buffer to refresh
		DRAW_FONT(this->font, newBuffer, this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, color(0,0,0), this->termWidth);
		if (removeLast) {
			bufferInPos--;
		} else {
			bufferIn[bufferInPos] = c;
			newBuffer[bufferInPos] = c;
			bufferInPos++;
		}
		// render new buffer
		DRAW_FONT(this->font, newBuffer, this->bufferOffsetX + this->bufferCX * this->xmargin, this->bufferOffsetY + this->bufferCY * this->ymargin, color(255,255,255), this->termWidth);
		// render
	} else {
		Debug_Printf(1,1,true,0,"Buffer overflow!");
	}
}

void Terminal::OutputBuffer() {
	// using custom font 
	// copy up to the buffer in pos to new buffer
	char newBuffer[BUF_SIZE]; // assuming 64 chars max in a line
	for (int i = 0; i < bufferInPos; i++) {
		newBuffer[i] = bufferIn[i];
	}

	DRAW_FONT(font, newBuffer, this->bufferOffsetX + this->bufferCX, this->bufferOffsetY + this->bufferCY * this->ymargin, color(255,255,255), this->termWidth);
	// update the buffer cursor position
	this->bufferCX = 0;
	this->bufferCY++;
	// will need to shift vram up if we go past the bottom
	if (this->bufferCY > this->termHeight) {
		// move vram up by ymargin amount
		// vram + ymargin -> vram
		for (int i = 0; i < this->ymargin; i++) {
			for (int j = 0; j < this->termWidth; j++) {
				vram[j + i * this->termWidth] = vram[j + (i + this->ymargin) * this->termWidth];
			}
		}
	}

}
