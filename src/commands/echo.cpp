/**
 * @file echo.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Echo command, prints the input to the screen.
 * @version 1.1
 * @date 2022-06-13
 */

extern int echo_main(int argc, char **argv)
{
    terminal->ClearBuffer();
    // check if there is an argument
    if (argc == 1) return 0;
    // print all arguments
    for (int i = 1; i < argc; i++) {
        terminal->WriteChars(argv[i], true);
        // add a space between arguments as passed in with spaces
        terminal->WriteBuffer(' ', false);
    }
    terminal->WriteBuffer('\n', false);
    return 0; // return 0 on success
};
