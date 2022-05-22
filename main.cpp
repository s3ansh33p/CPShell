#include <appdef.hpp>

#include "calc.hpp"
#include "draw_functions.hpp"
#include "lib/core/exceptions.hpp"
#include "lib/core/event_handler.hpp"
#include "lib/functions/random.hpp"

#ifndef PC
	APP_NAME("CP Shell")
	APP_DESCRIPTION("A calculator shell, with file handling, memory management, breakpoints, and a lot more.")
	APP_AUTHOR("s3ansh33p")
	APP_VERSION("0.0.1")
#endif

// Terminal Class
class Terminal {
	public: 
		void ClearBuffer();
		void OutputBuffer();
		void WriteBuffer(char c, bool removeLast = false);
		void SetFont(uint8_t* font);
		// buffer for command input
		char bufferIn[64];
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
	bufferCX = 0;
}

void Terminal::WriteBuffer(char c, bool removeLast) {
	char newBuffer[64]; // assuming 64 chars max in a line
	for (int i = 0; i < bufferInPos; i++) {
		newBuffer[i] = bufferIn[i];
	}
	if (bufferInPos < 63) {
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
	char newBuffer[64]; // assuming 64 chars max in a line
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

// KeyBoard Class
class VirtualKeyboard {
    public:
        void Render();
		void SetFont(uint8_t* font);
		void Highlight(uint8_t x, uint8_t y, uint32_t highlightColor = color(255,255,0), bool hide = false);
		void ToggleShift();
        bool caps = false;
		int16_t x = 12;
		int16_t y = 400;
		int16_t xmargin = 10;
		int16_t ymargin = 10;
		uint8_t* font;
		int8_t xcursor = 0;
		int8_t ycursor = 0;
		int8_t yfix = 0;
		const char lower[27] = "abcdefghijklmnopqrstuvwxyz";
		const char characters[67] = "1234567890=!@#$%^&*_+-ABCDEFGHIJKLM\\|;:',./?NOPQRSTUVWXYZ[]{}()\"~`";
		const char specials[23] = "[SPACE] [BACK] [ENTER]";
		char activeKey[2] = "";
		int32_t keyColor = color(255, 255, 255);
};

void VirtualKeyboard::ToggleShift() {
	// call render to update the keyboard
	// will need something to only update the qwerty keys
	
	// set the color to black so that we cannot see the text
	this->keyColor = color(0, 0, 0);
	this->Render();
	this->caps = !this->caps;
	// set back to white
	this->keyColor = color(255, 255, 255);
	this->Render();
	// render highlighted key
	this->Highlight(this->xcursor, this->ycursor);

}

// x is between 0 and 21 (inclusive)
// y is between 0 and 2 (inclusive)
void VirtualKeyboard::Highlight(uint8_t xOffset = 0, uint8_t yOffset = 0, uint32_t highlightColor, bool hide) {
	// KEYBOARD
	// 1 2 3 4 5 6 7 8 9 0 = ! @ # $ % ^ & * _ + -  
	// A B C D E F G H I J K L M \ | ; : ' , . / ?   
	// N O P Q R S T U V W X Y Z [ ] { } ( ) " ~ `
	// [ S P A C E ]   [ B A C K ]   [ E N T E R ]

	// get active key
	// check if within first 13 chars on y=1,2
	if (xOffset < 13 && !this->caps && yOffset != 0 && yOffset != 3) {
		if (yOffset == 1) {
			this->activeKey[0] = this->lower[xOffset];
		} else {
			this->activeKey[0] = this->lower[xOffset + 13];
		}
	} else {
		this->activeKey[0] = this->characters[xOffset + (yOffset * 22)];
	}

	if (yOffset != 3) {
		DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * xOffset, this->y + this->ymargin * yOffset, highlightColor, 0);

		// draw rectangle around the character

		if (hide) {
			highlightColor = color(0, 0, 0);
		}
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * yOffset - 2, highlightColor);
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
		line(this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * yOffset - 2, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
		line(this->x + this->xmargin * xOffset - 2, this->y + this->ymargin * (yOffset + 1) - 1, this->x + this->xmargin * (xOffset + 1) - 1, this->y + this->ymargin * (yOffset + 1) - 1, highlightColor);
	} else {
	// else specials
		char specialChar[2] = "";

		if (xOffset == 0) {
			for (int i = 0; i < 7; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		} else if (xOffset == 1) {
			for (int i = 8; i < 14; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i - 1), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		} else if (xOffset == 2) {
			for (int i = 15; i < 22; i++) {
				specialChar[0] = this->specials[i];
				DRAW_FONT(this->font, specialChar, this->x + this->xmargin * (xOffset + i - 2), this->y + this->ymargin * yOffset, highlightColor, 0);
			}
		}
	}
}

void VirtualKeyboard::SetFont(uint8_t* font) {
	this->font = font;
}

void VirtualKeyboard::Render() {

	// render normals
	for (int i = 0; i < 66; i++) {
		this->activeKey[0] = this->characters[i];
		// if in top row
		if (i < 22) {
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * i, this->y, this->keyColor, 0);
		// if in 2nd row
		} else if (i < 44) {
			// check for left 13 keys
			if (i < 35 && !this->caps) {
				this->activeKey[0] = this->lower[i - 22];
			}
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * (i - 22), this->y + this->ymargin, this->keyColor, 0);
		// 3rd
		} else {
			// check for left 13 keys
			if (i < 57 && !this->caps) {
				this->activeKey[0] = this->lower[i - 44 + 13];
			}
			DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * (i - 44), this->y + this->ymargin * 2, this->keyColor, 0);
		}
	}
	// render specials
	for (int i = 0; i < 23; i++) {
		this->activeKey[0] = this->specials[i];
		DRAW_FONT(this->font, this->activeKey, this->x + this->xmargin * i, this->y + this->ymargin * 3, this->keyColor, 0);
	}

}	

// Tracks the main Shell loop
bool shell_running = true;

// RNG pointer
RandomGenerator* rng;

// Virtual Keyboard pointer
VirtualKeyboard* keyboard;

// Terminal pointer
Terminal* terminal;

// Ends the Shell and is called by the event handler
void endShell() {
	shell_running = false;
}

// Updates the random number
void updateRNG() {
	rng->Generate(50);
}

// Kayboard Pseudo functions
void kbShift() {
	keyboard->ToggleShift();
}
void kbLeft() {
	if (keyboard->xcursor > 0) {
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor, keyboard->keyColor, true);
		keyboard->xcursor--;
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
	}
}
void kbRight() {
	if (keyboard->xcursor < 21 || (keyboard->xcursor < 1 && keyboard->ycursor == 3)) {
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor, keyboard->keyColor, true);
		keyboard->xcursor++;
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
	}
}
void kbUp() {
	if (keyboard->ycursor > 0) {
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor, keyboard->keyColor, true);
		if (keyboard->yfix > 4) {
			// check if going from specials to normal
			if (keyboard->ycursor == 3) {
				if (keyboard->xcursor == 2) {
					keyboard->xcursor = 15;
				} else if (keyboard->xcursor == 1) {
					keyboard->xcursor = 8;
				} else {
					keyboard->xcursor = 0;
				}
			}
			keyboard->ycursor--;
			keyboard->yfix = 0;
		} else {
			keyboard->yfix++;
		}
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
	}
}
void kbDown() {
	if (keyboard->ycursor < 3) {
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor, keyboard->keyColor, true);
		if (keyboard->yfix > 4) {
			keyboard->ycursor++;
			keyboard->yfix = 0;
			// check if going to specials
			if (keyboard->ycursor == 3) {
				if (keyboard->xcursor < 8) {
					keyboard->xcursor = 0;
				} else if (keyboard->xcursor < 15) {
					keyboard->xcursor = 1;
				} else {
					keyboard->xcursor = 2;
				}
			}
		} else {
			keyboard->yfix++;
		}
		keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
	}
}
void kbEnter() {
	// write to the char buffer
	char activeKey = keyboard->activeKey[0];
	// checks for specials
	if (keyboard->ycursor == 3) {
		if (keyboard->xcursor == 2) {
			// enter
			// store buffer
			char buf[64];
			for (int i = 0; i < terminal->bufferInPos; i++) {
				buf[i] = terminal->bufferIn[i];
			}
			terminal->bufferCY++;

			// read current buffer
			// check for command "help"
			// check for command "clear"
			// else print error
			
			if (terminal->bufferInPos == 4) {
				if ((buf[0] == 'h' || buf[0] == 'H') && (buf[1] == 'e' || buf[1] == 'E') && (buf[2] == 'l' || buf[2] == 'L') && (buf[3] == 'p' || buf[3] == 'P')) {
					char help[31] = "help: prints this help message";
					terminal->bufferInPos = 30;
					for (int i = 0; i < 31; i++) {
						terminal->bufferIn[i] = help[i];
					}
					terminal->OutputBuffer();
				}
			} else if (terminal->bufferInPos == 5) {
				if ((buf[0] == 'c' || buf[0] == 'C') && (buf[1] == 'l' || buf[1] == 'L') && (buf[2] == 'e' || buf[2] == 'E') && (buf[3] == 'a' || buf[3] == 'A') && (buf[4] == 'r' || buf[4] == 'R')) {
					// TBA
				}
			} else {
				char error[27] = "error: command not found\n";
				terminal->bufferInPos = 26;
				for (int i = 0; i < 26; i++) {
					terminal->bufferIn[i] = error[i];
				}
				terminal->OutputBuffer();
			}

			terminal->ClearBuffer();

		} else if (keyboard->xcursor == 1) {
			// back
			// hex for space is 20
			terminal->WriteBuffer(0x20, true);
		} else {
			// space
			terminal->WriteBuffer(0x20);
		}
	} else {
		terminal->WriteBuffer(activeKey);
	}
}



//The acutal main
void main2() {

	// load the textures and fonts
	LOAD_FONT_PTR("fnt\\7x8", f_7x8);
	
	fillScreen(color(0, 0, 0));

	RandomGenerator rngp;
	rng = &rngp;
	rng->SetSeed(1337);

	VirtualKeyboard keyboardp;
	keyboard = &keyboardp;
	keyboard->SetFont(f_7x8);
	keyboard->Render();
	keyboard->Highlight(); // 0,0

	Terminal terminalp;
	terminal = &terminalp;
	terminal->SetFont(f_7x8);
	Debug_Printf(20,29,true,0,"Max X: %i", terminal->xmax);
	Debug_Printf(20,30,true,0,"W x H: %i x %i", terminal->termWidth, terminal->termHeight);


	// Add event listeners
	addListener(KEY_CLEAR, endShell); // end the shell
	addListener(KEY_BACKSPACE, updateRNG); // update the random number
	addListener(KEY_SHIFT, kbShift); // toggle Shift 

	// Keyboard Listeners
	addListener(KEY_LEFT, kbLeft); // left cursor
	addListener(KEY_RIGHT, kbRight); // right cursor
	addListener2(KEY_UP, kbUp); // up cursor
	addListener2(KEY_DOWN, kbDown); // down cursor
	addListener(KEY_EXE, kbEnter); // send the key


	LCD_Refresh();

	while (shell_running) {
		checkEvents();
		

		
		Debug_Printf(20,31,true,0,"RNG: %i",rng->m_x);

		LCD_Refresh();
	}

	// free memory
	// free(f_7x8);
}
