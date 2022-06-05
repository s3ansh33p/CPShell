/**
 * @file clear.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Clears the terminal screen.
 * @version 1.0
 * @date 2022-06-06
 */

extern int clear_main(int argc, char **argv)
{
    // clear the screen to black
    fillScreen(0);
    // rerender the keyboard
    keyboard->Render();
    // keyboard highlight
    keyboard->Highlight(keyboard->xcursor, keyboard->ycursor);
    // clear the terminal buffer
    terminal->ClearBuffer();
    // reset terminal y
    terminal->bufferCY = 0;
    return 0;
};
