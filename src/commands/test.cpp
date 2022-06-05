/**
 * @file test.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Testing function for the CPShell. Useful for debugging.
 * @version 1.0
 * @date 2022-06-06
 */

extern int test_main(int argc, char **argv)
{
    // print all arguments
    for (int i = 0; i < argc; i++) {
        terminal->WriteChars(argv[i]);
        terminal->WriteBuffer('\n', false);
    }
    // print number of arguments
    char message[] = "Number of arguments: ";
    strcat(message, numToString(argc));
    strcat(message, "\n\n");
    terminal->WriteChars(message);
    char message2[] = "Test function appears to be working.\n";
    terminal->WriteChars(message2);
    return 0; // return 0 on success
};
