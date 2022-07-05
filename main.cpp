#include <appdef.hpp>

// main
#include "calc.hpp"
#include "lib/draw_functions.hpp"
#include "lib/core/exceptions.hpp"
#include "lib/core/event_handler.hpp"
#include "lib/core/touch_event_handler.hpp"
#include "lib/functions/random.hpp"

// shell
#include "src/internal.hpp" // definitions
#include "src/terminal.hpp"
#include "src/virtual_keyboard.hpp"

// Terminal pointer
Terminal* terminal;

// Virtual Keyboard pointer
VirtualKeyboard* keyboard;

#include "src/cpshell.cpp"

#ifndef PC
	APP_NAME("CP Shell")
	APP_DESCRIPTION("A calculator shell, with file handling, memory management, breakpoints, and a lot more.")
	APP_AUTHOR("s3ansh33p")
	APP_VERSION(CPS_VERSION)
#endif

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
	if (keyboard->xcursor < 2 || (keyboard->xcursor < 21 && keyboard->ycursor != 3)) {
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

void kbBackspace() {
	terminal->RemoveLast();
}

void kbEnter() {
	// write to the char buffer
	char activeKey = keyboard->activeKey[0];
	// checks for specials
	if (keyboard->ycursor == 3) {
		if (keyboard->xcursor == 2) {
			// enter
			// store buffer
			if (terminal->bufferInPos != 0) {
				terminal->HideCursor();
				terminal->bufferCY++;

				// copy bufferIn to callingArgs
				char callingArgs[BUF_SIZE];
				for (int i = 0; i < terminal->bufferInPos; i++) {
					callingArgs[i] = terminal->bufferIn[i];
				}
				// add space to end
				callingArgs[terminal->bufferInPos] = '\0';
				int argc = 0;
				// count number spaces in callingArgs
				for (int i = 0; i < strlen(callingArgs); i++) {
					if (callingArgs[i] == ' ') {
						argc++;
					}
				}
				// check if final space is in callingArgs
				if (callingArgs[strlen(callingArgs) - 1] != ' ') {
					argc++;
				}
				// create instance of argv
				char** argv = new char*[argc];
				// split callingArgs into argv
				int argvIndex = 0;
				
				char currentArg[ARGV_SIZE];
				int currentArgIndex = 0;
				for (int i = 0; i < strlen(callingArgs); i++) {
					if (callingArgs[i] == ' ') {
						argv[argvIndex] = new char[currentArgIndex + 1];
						for (int j = 0; j < currentArgIndex; j++) {
							argv[argvIndex][j] = currentArg[j];
						}
						argv[argvIndex][currentArgIndex] = '\0';
						argvIndex++;
						currentArgIndex = 0;
					} else {
						currentArg[currentArgIndex] = callingArgs[i];
						currentArgIndex++;
					}
				}
				// set last argv to last arg
				argv[argvIndex] = new char[currentArgIndex + 1];
				for (int j = 0; j < currentArgIndex; j++) {
					argv[argvIndex][j] = currentArg[j];
				}
				argv[argvIndex][currentArgIndex] = '\0';

				// call cpshell_main
				// testfunc_main(argc, argv);
				psuedo_main(argc, argv);
				// display host after psuedo_main
				display_host();
			}
			// else ignore as it is a blank line

		} else if (keyboard->xcursor == 1) {
			// back
			kbBackspace();
		} else {
			// space
			terminal->WriteBuffer(0x20);
		}
	} else {
		terminal->WriteBuffer(activeKey);
	}
}

void HandleTouchForKeyboard() {
	keyboard->Highlight(keyboard->xcursor, keyboard->ycursor, keyboard->keyColor, true);
	uint16_t xIn = event.data.touch_single.p1_x;
	uint16_t yIn = event.data.touch_single.p1_y;
	Debug_Printf(1,34,true,0,"%d | %d",xIn,yIn);
	// get x and y to be top left of keyboard
	xIn -= keyboard->x;
	yIn -= keyboard->y;
	// read column / row
	uint16_t xOffset = xIn / keyboard->xmargin;
	uint16_t yOffset = yIn / keyboard->ymargin;
	// to investigate later
	if (yOffset > 3) yOffset = 3;
	// check if on bottom line
	if (yOffset > 2) {
		if (xIn < keyboard->xmargin * 7) {
			xOffset = 0;
		} else if (xIn < keyboard->xmargin * 14) {
			xOffset = 1;
		} else {
			xOffset = 2;
		}
	}
	keyboard->xcursor = xOffset;
	keyboard->ycursor = yOffset;
	keyboard->Highlight(xOffset, yOffset);
	kbEnter();
}

void testTouch() {
	Debug_Printf(10,32,true,0,"Working");
}

//The acutal main
void main2() {

	// load the textures and fonts
	LOAD_FONT_PTR("7x8", f_7x8);
	
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
	// Debug_Printf(10,29,true,0,"Max X: %i", terminal->xmax);
	// Debug_Printf(10,30,true,0,"W x H: %i x %i", terminal->termWidth, terminal->termHeight);


	// Add event listeners
	addListener(KEY_CLEAR, endShell); // end the shell - cmd now
	addListener(KEY_BACKSPACE, kbBackspace); // remove last character
	addListener(KEY_SHIFT, kbShift); // toggle Shift 

	// Keyboard Listeners
	addListener(KEY_LEFT, kbLeft); // left cursor
	addListener(KEY_RIGHT, kbRight); // right cursor
	addListener2(KEY_UP, kbUp); // up cursor
	addListener2(KEY_DOWN, kbDown); // down cursor
	addListener(KEY_EXE, kbEnter); // send the key

	addTouchListener(0, 0, 300, 100, testTouch); // touch listener
	addTouchListener(keyboard->x, keyboard->y, keyboard->x + keyboard->xmargin * 22 - 1, keyboard->y + keyboard->ymargin * 4 - 1, HandleTouchForKeyboard);


	// Initialize the shell
	cpshell_init();
	// Display Host
	display_host();

	LCD_Refresh();

	// frame counter for updating the blinky cursor
	uint8_t frameCounter = 0;
	bool isCursorShowing = false;

	while (shell_running) {

		// frame counter
		frameCounter++;

		// update the cursor
		if (frameCounter > 8) {
			frameCounter = 0;
			isCursorShowing = !isCursorShowing;
			if (isCursorShowing) {
				terminal->ShowCursor();
			} else {
				terminal->HideCursor();
			}
		}

		checkEvents();
		checkTouchEvents();
		// Debug_Printf(10,28,true,0,"T X: %i | Y: %i | PX: %i | PY: %i",terminal->bufferCX, terminal->bufferCY, terminal->bufferCX * terminal->xmargin + terminal->bufferOffsetX, terminal->bufferCY * terminal->ymargin + terminal->bufferOffsetY);

		LCD_Refresh();
	}

	// free memory
	// free(f_7x8);
}
