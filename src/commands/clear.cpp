#include <string.h>
#include <stdlib.h>
/**
 * @file clear.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Clears the terminal screen.
 * @version 1.0
 * @date 2022-06-06
 */

extern int clear_main(int, char **)
{
    // clear the screen to black
    fillScreen(0);
    // rerender the keyboard
    keyboard->Render();
    // clear the terminal buffer
    terminal->ClearBuffer();
    terminal->Render();
    return 0;
};
