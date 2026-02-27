#include <string.h>
#include <appdef.h>

// main
#include "calc.hpp"
#include "lib/draw_functions.hpp"
#include "lib/core/exceptions.hpp"
// #include "lib/core/event_handler.hpp" // Removed legacy event handler
#include "lib/core/touch_event_handler.hpp" // New unified handler
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

void ProcessCommand() {
	// store buffer
	if (terminal->bufferInPos != 0) {
		terminal->HideCursor();

		// Copy command
		char callingArgs[BUF_SIZE];
		for (int i = 0; i < terminal->bufferInPos; i++) {
			callingArgs[i] = terminal->bufferIn[i];
		}
		callingArgs[terminal->bufferInPos] = '\0';

		// Write newline to visual terminal
		terminal->WriteBuffer('\n', false);

		// Parse args
		int argc = 0;
		// count number spaces in callingArgs
		for (int i = 0; i < (int)strlen(callingArgs); i++) {
			if (callingArgs[i] == ' ') {
				argc++;
			}
		}
		// check if final space is in callingArgs
		if (strlen(callingArgs) > 0 && callingArgs[strlen(callingArgs) - 1] != ' ') {
			argc++;
		}

		if (argc > 0) {
			// create instance of argv
			char** argv = new char*[argc];
			// split callingArgs into argv
			int argvIndex = 0;

			char currentArg[ARGV_SIZE];
			int currentArgIndex = 0;
			for (int i = 0; i < (int)strlen(callingArgs); i++) {
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
			if (argvIndex < argc) {
				argv[argvIndex] = new char[currentArgIndex + 1];
				for (int j = 0; j < currentArgIndex; j++) {
					argv[argvIndex][j] = currentArg[j];
				}
				argv[argvIndex][currentArgIndex] = '\0';
			}

			// call cpshell_main
			psuedo_main(argc, argv);

			// Clean up argv
			for(int i=0; i<argc; i++) delete[] argv[i];
			delete[] argv;
		}

		// display host after psuedo_main
		display_host();
	} else {
		// Empty line
		terminal->WriteBuffer('\n', false);
		display_host();
	}
}

// Kayboard Pseudo functions
void kbToggle() {
	keyboard->Toggle();
	terminal->keyboardVisible = keyboard->visible;
	terminal->UpdateLayout();

	// Clear screen and redraw
	fillScreen(0);
	keyboard->Render();
	terminal->Render();
}

void kbUp() {
	if (!keyboard->visible) {
		terminal->Scroll(-1); // Scroll up
	}
}

void kbDown() {
	if (!keyboard->visible) {
		terminal->Scroll(1); // Scroll down
	}
}

void kbBackspace() {
	terminal->RemoveLast();
}

void kbEnter() {
	ProcessCommand();
}

void HandleTouchForKeyboard() {
	if (!keyboard->visible) return;

	uint16_t xIn = event.data.touch_single.p1_x;
	uint16_t yIn = event.data.touch_single.p1_y;
	uint32_t type = event.data.touch_single.direction; // TOUCH_DOWN etc.

	const char* key = keyboard->Update(xIn, yIn, type);
	if (key) {
		if (strcmp(key, "BACKSPACE") == 0) {
			kbBackspace();
		} else if (strcmp(key, "ENTER") == 0) {
			kbEnter();
		} else if (strcmp(key, "SPACE") == 0) { // Check space key string from VirtualKeyboard
			terminal->WriteBuffer(' ', false);
		} else {
			terminal->WriteChars(key, true);
		}
	}
}

void testTouch() {
	// Debug_Printf(10,32,true,0,"Working");
}

//The acutal main
void main2() {

	// load the textures and fonts
	LOAD_FONT_PTR("7x8", f_7x8);
	
	if (!f_7x8) {
		Debug_Printf(0, 0, false, 0, "Error loading font!");
		LCD_Refresh();
		while(1);
	}

	fillScreen(0); // clear the screen to black (0,0,0)

	RandomGenerator rngp;
	rng = &rngp;
	rng->SetSeed(1337);

	VirtualKeyboard keyboardp;
	keyboard = &keyboardp;
	keyboard->SetFont(f_7x8);

	Terminal terminalp;
	terminal = &terminalp;
	terminal->SetFont(f_7x8);

	// Sync layout
	terminal->keyboardVisible = keyboard->visible;
	terminal->UpdateLayout();

	// Initial Render
	terminal->Render();
	keyboard->Render();

	// Add event listeners (Migrated to SDK keycodes and addKeyListener)
	addKeyListener(KEYCODE_POWER_CLEAR, endShell);
	addKeyListener(KEYCODE_BACKSPACE, kbBackspace);

	addKeyListener(KEYCODE_KEYBOARD, kbToggle);

	// Keyboard Listeners
	addKeyListener(KEYCODE_UP, kbUp);
	addKeyListener(KEYCODE_DOWN, kbDown);
	addKeyListener(KEYCODE_EXE, kbEnter);

	addTouchListener(0, height - KBD_H, width, height, HandleTouchForKeyboard, TOUCH_DOWN);
	addTouchListener(0, height - KBD_H, width, height, HandleTouchForKeyboard, TOUCH_UP);

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

		// checkEvents(); // Removed legacy event loop
		checkTouchEvents(); // Unified event loop

		LCD_Refresh();
	}

	// free memory
	// free(f_7x8);
}
