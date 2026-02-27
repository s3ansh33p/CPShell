#pragma once

#include "../calc.hpp"
#include "../lib/functions/shapes.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include "internal.hpp"

// Terminal Class
class Terminal {
	public: 
		void ClearBuffer();
		void WriteBuffer(char c, bool hideCursor = true);
		void WriteChars(const char* charArray, bool skipClear = false);
		void RemoveLast();
		void ShowCursor();
		void HideCursor();
		void SetFont(uint8_t* font);
		void SetColor(uint32_t newColor);
		void SetCursorColor(uint32_t newColor);
		void Render(); // Redraw visible lines
		void Scroll(int lines);
		void ScrollToBottom();
		void UpdateLayout();

		// buffer for command input
		char bufferIn[BUF_SIZE];
		int8_t bufferInPos = 0;

		// History Buffer
		std::vector<std::string> history;

		// View State
		int scrollOffset = 0; // 0 means showing the top. Or maybe 0 means bottom?
		// Let's say scrollOffset is the index of the first visible line.
		// But we want auto-scroll to end.
		// Maybe scrollOffset is "lines from bottom"?
		// Let's use: scrollY = index of top visible line.
		int scrollY = 0;

		// Dimensions
		int16_t xmargin = 8; // character width
		int16_t ymargin = 10; // character height (7x8 font + spacing)
		int16_t termWidth = width - xmargin * 2;
		// termHeight depends on keyboard visibility?
		// User said: "prompt would always be visible".
		// When keyboard is visible, terminal area is smaller.
		// When hidden, full screen.
		int16_t termHeight = height - 40; // Default full screen minus some margin?
		int16_t xmax = termWidth / xmargin;
		int16_t ymax; // Calculated based on height

		uint8_t* font;
		uint32_t color = 0xFFFF; // white
		uint32_t cursorColor = 0xFFFF; // white

		bool keyboardVisible = true;

		Terminal() {
			history.push_back(""); // Start with one empty line
			UpdateLayout();
		}
};

void Terminal::UpdateLayout() {
	int kbdH = keyboardVisible ? KBD_H : 0;
	termHeight = height - kbdH;
	ymax = termHeight / ymargin;
	ScrollToBottom();
}

void Terminal::SetFont(uint8_t* font) {
	this->font = font;
}

void Terminal::ClearBuffer() {
	this->bufferInPos = 0;
	// No, we don't clear history here. Just the current input line buffer.
	// But Wait, WriteBuffer writes to `bufferIn` AND history?
	// `bufferIn` tracks the *current command being typed*.
	// The history vector tracks *displayed lines*.
	// When user types, we update the last line of history?
	// Or we treat `bufferIn` separate?
	// Standard terminal: history contains committed lines + current line being edited.
	// Let's sync them.
}

void Terminal::Scroll(int lines) {
	scrollY += lines;
	int maxScroll = std::max(0, (int)history.size() - ymax);
	if (scrollY < 0) scrollY = 0;
	if (scrollY > maxScroll) scrollY = maxScroll;
	Render();
}

void Terminal::ScrollToBottom() {
	int maxScroll = std::max(0, (int)history.size() - ymax);
	scrollY = maxScroll;
}

void Terminal::WriteBuffer(char c, bool hideCursor) {
	if (hideCursor) this->HideCursor();

	std::string& currentLine = history.back();

	if (c == '\n') {
		history.push_back("");
		bufferInPos = 0;
		ScrollToBottom();
		Render(); // Need to redraw to show scroll
		return;
	}

	// Handle Backspace (passed as special char or handled in RemoveLast?)
	// RemoveLast calls this? No.

	if (c >= 32 && c <= 126) {
		currentLine += c;
		bufferIn[bufferInPos++] = c; // Keep bufferIn for command processing

		// Wrap text
		if (currentLine.length() >= (size_t)xmax) {
			// Move excess to next line?
			// Simple wrapping: just let it grow and Renderer handles it?
			// Or split?
			// Splitting is hard. Let's just break line.
			std::string remainder = currentLine.substr(xmax);
			currentLine = currentLine.substr(0, xmax);
			history.push_back(remainder);
		}
	}

	ScrollToBottom();
	Render();
}

void Terminal::WriteChars(const char *charArray, bool skipClear) {
	int len = strlen(charArray);
	for (int i = 0; i < len; i++) {
		this->WriteBuffer(charArray[i], false);
	}
	if (!skipClear) {
		bufferInPos = 0; // Reset input buffer, but history persists
		// memset(bufferIn, 0, BUF_SIZE);
	}
}

void Terminal::RemoveLast() {
	std::string& currentLine = history.back();
	if (currentLine.length() > 0) {
		currentLine.pop_back();
		if (bufferInPos > 0) bufferInPos--;
		Render();
	} else {
		// Merge with previous line if empty?
		if (history.size() > 1) {
			history.pop_back();
			// bufferInPos needs to be restored?
			// This is complex if we allow backspacing over newlines.
			// For now, simple backspace on current line.
			Render();
		}
	}
}

void Terminal::Render() {
	// Clear visible area
	int visibleH = keyboardVisible ? (height - KBD_H) : height;
	drawFilledRectangle(0, 0, width, visibleH, 0x0000); // Black bg

	if (!font) return;

	int startLine = scrollY;
	int endLine = std::min((int)history.size(), startLine + ymax + 1);

	for (int i = startLine; i < endLine; i++) {
		int screenY = (i - startLine) * ymargin;
		if (screenY >= visibleH) break;

		const std::string& line = history[i];
		// const char* text = line.c_str(); // DRAW_FONT takes char*? const char* fix applied.
		// Need mutable for DRAW_FONT? No, fixed to const char*.

		DRAW_FONT(font, line.c_str(), 2, screenY, color, width);
	}

	// Draw cursor if at bottom?
	if (scrollY >= (int)history.size() - ymax) {
		ShowCursor();
	}
}

// Cursor Show
void Terminal::ShowCursor() {
	// Find cursor position
	// It's at the end of the last line
	if (history.empty()) return;

	int lineIdx = history.size() - 1;
	// Is it visible?
	if (lineIdx < scrollY || lineIdx >= scrollY + ymax) return;

	int screenY = (lineIdx - scrollY) * ymargin;
	int screenX = 2 + history.back().length() * xmargin;

	drawFilledRectangle(screenX, screenY, xmargin, ymargin, cursorColor);
}

// Cursor Hide
void Terminal::HideCursor() {
	// Find cursor position and clear it
	if (history.empty()) return;
	int lineIdx = history.size() - 1;
	if (lineIdx < scrollY || lineIdx >= scrollY + ymax) return;

	int screenY = (lineIdx - scrollY) * ymargin;
	int screenX = 2 + history.back().length() * xmargin;

	drawFilledRectangle(screenX, screenY, xmargin, ymargin, 0x0000);
}

// Set Color
void Terminal::SetColor(uint32_t newColor) {
	this->color = newColor;
}

// Set Cursor Color
void Terminal::SetCursorColor(uint32_t newColor) {
	this->cursorColor = newColor;
}
