#pragma once

#include "../lib/functions/convert.hpp"

static int been_there_done_that = 0;

static const struct Applet applets[] = {

#ifdef CPS_MAIN		//bin
    {"cpshell", cpshell_main},
#endif
    {0}
};

// argc is the number of arguments
// argv is the array of arguments
// argv[0] is the calling program name
// argv[1] is the path to the current program
// argv[2] is the first argument
// argv[3] is the second argument
// argv[n-2] is the nth argument
int cpshell_main(int argc, char **argv)
{
    terminal->ClearBuffer();
    if (argc < 2)
    {
        // char message[] = "Usage: %s <command> [<args>]\n", argv[0];
        char message[] = "Usage: ";
        strcat(message, argv[0]);
        strcat(message, " <command> [<args>]\n");
        
        terminal->WriteChars(message);
        return 1; // error
    } else {
        // print all arguments
        for (int i = 0; i < argc; i++) {
            terminal->WriteChars(argv[i]);
            terminal->WriteBuffer('\n', false);
        }
        // print number of arguments
        char message[] = "Number of arguments: ";
        strcat(message, numToString(argc));
        strcat(message, "\n");
        terminal->WriteChars(message);
    }
    // output "test main" to the terminal
    char message[] = "test main\n";
    terminal->WriteChars(message);
    return 0; // return 0 on success
};
