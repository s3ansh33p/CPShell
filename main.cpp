#include <appdef.hpp>

// main
#include "calc.hpp"
#include "lib/draw_functions.hpp"
#include "lib/core/exceptions.hpp"
#include "lib/core/event_handler.hpp"
#include "lib/functions/random.hpp"

// shell
#include "src/cpshell.cpp"
#include "src/terminal.hpp"
#include "src/virtual_keyboard.hpp"

#ifndef PC
	APP_NAME("CP Shell")
	APP_DESCRIPTION("A calculator shell, with file handling, memory management, breakpoints, and a lot more.")
	APP_AUTHOR("s3ansh33p")
	APP_VERSION("0.0.1")
#endif


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
			char buf[BUF_SIZE];
			for (int i = 0; i < terminal->bufferInPos; i++) {
				buf[i] = terminal->bufferIn[i];
			}
			terminal->HideCursor();
			terminal->bufferCY++;

			// read current buffer
			// check for command "help"
			// check for command "clear"
			// else print error

			// Todo: create a command handler class
			
			bool validCommandFound = false;

			if (terminal->bufferInPos == 4) {
				if ((buf[0] == 'h' || buf[0] == 'H') && (buf[1] == 'e' || buf[1] == 'E') && (buf[2] == 'l' || buf[2] == 'L') && (buf[3] == 'p' || buf[3] == 'P')) {
					// clear buffer
					terminal->ClearBuffer();
					char help[] = "help: prints this help message\nclear: clears the terminal\n";
					terminal->WriteChars(help);
					validCommandFound = true;
				}
			} else if (terminal->bufferInPos == 5) {
				if ((buf[0] == 'c' || buf[0] == 'C') && (buf[1] == 'l' || buf[1] == 'L') && (buf[2] == 'e' || buf[2] == 'E') && (buf[3] == 'a' || buf[3] == 'A') && (buf[4] == 'r' || buf[4] == 'R')) {
					// clear the screen to black
					fillScreen(0);
					// rerender the keyboard
					keyboard->Render();
					// keyboard highlight
					keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
					// clear buffer done later for us
					// reset terminal y
					terminal->bufferCY = 0;
					validCommandFound = true;
				}
			}
			if (!validCommandFound) {
				// clear buffer
				terminal->ClearBuffer();
				char error[27] = "error: command not found\n";
				terminal->WriteChars(error);
			}

			terminal->ClearBuffer();

		} else if (keyboard->xcursor == 1) {
			// back
			terminal->RemoveLast();
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
	
	fillScreen(0); // clear the screen to black (0,0,0)

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
	Debug_Printf(10,29,true,0,"Max X: %i", terminal->xmax);
	Debug_Printf(10,30,true,0,"W x H: %i x %i", terminal->termWidth, terminal->termHeight);


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

	char welcomeMessage[] = "Welcome to CPShell!\nRunning on Classpad OS v2.1.2\nWritten by: CPShell Team\nType 'help' for a list of commands.\n\n\n";
	terminal->WriteChars(welcomeMessage);

	LCD_Refresh();

	// frame counter for updating the blinky cursor
	uint8_t frameCounter = 0;
	bool isCursorShowing = false;

	while (shell_running) {

		// frame counter
		frameCounter++;

		// update the cursor
		if (frameCounter > 10) {
			frameCounter = 0;
			isCursorShowing = !isCursorShowing;
			if (isCursorShowing) {
				terminal->ShowCursor();
			} else {
				terminal->HideCursor();
			}
		}

		checkEvents();
		
		Debug_Printf(10,31,true,0,"RNG: %i",rng->m_x);
		Debug_Printf(10,28,true,0,"T X: %i | Y: %i | PX: %i | PY: %i",terminal->bufferCX, terminal->bufferCY, terminal->bufferCX * terminal->xmargin + terminal->bufferOffsetX, terminal->bufferCY * terminal->ymargin + terminal->bufferOffsetY);

		LCD_Refresh();
	}

	// free memory
	// free(f_7x8);
}
