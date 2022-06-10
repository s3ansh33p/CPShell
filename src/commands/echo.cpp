/**
 * @file echo.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Echo command, prints the input to the screen.
 * @version 1.0
 * @date 2022-06-10
 */

extern int echo_main(int argc, char **argv)
{
    terminal->ClearBuffer();
    // print all arguments
    for (int i = 0; i < argc; i++) {
        terminal->WriteChars(argv[i]);
        // add a space between arguments as passed in with spaces
        terminal->WriteBuffer(' ', false);
    }
    terminal->WriteBuffer('\n', false);
    return 0; // return 0 on success
};
