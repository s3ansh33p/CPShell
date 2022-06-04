#pragma once

#include "../lib/functions/convert.hpp"

static int been_there_done_that = 0;

static const struct Applet applets[] = {

#ifdef CPS_MAIN		//bin
    {"cpshell", cpshell_main},
#endif
#ifdef CPS_TEST		//bin
    {"test", testfunc_main},
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
int testfunc_main(int argc, char **argv)
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
        strcat(message, "\n\n");
        terminal->WriteChars(message);
    }
    // output "test main" to the terminal
    char message[] = "This is the actual result of the function\n";
    terminal->WriteChars(message);
    return 0; // return 0 on success
};

int psuedo_main(int argc, char **argv)
{
    char *s = argv[0];
    char *name = argv[0];
    const struct Applet *a = applets;

    while (*s != '\0') {
	if (*s++ == '/')
	    name = s;
    }

    while (a->name != 0) {
        if (strcmp(name, a->name) == 0) {
            // print name of program
            char message[] = "Name of program: ";
            strcat(message, name);
            strcat(message, "\n");
            terminal->WriteChars(message);
            int status;

            status = ((*(a->main)) (argc, argv));
            if (status < 0) {
                char err[ARGV_SIZE];
                strcat(err, a->name);
                strcat(err, ": An error occurred\n");
                terminal->WriteChars(err);
            }
            return(status);
        }
        a++;
    }
    return (cpshell_main(argc, argv));
}


int cpshell_main(int argc, char **argv)
{
    argc--;
    argv++;

    if (been_there_done_that == 1 || argc < 1) {
        const struct Applet *a = applets;
        terminal->ClearBuffer();
        char msg[] = "Currently defined functions:\n";
        terminal->WriteChars(msg);

        char prog_name[ARGV_SIZE];
        while (a->name != 0) {
            strcpy(prog_name, (a++)->name);
            strcat(prog_name, "\n");
            terminal->WriteChars(prog_name);
        }
        terminal->WriteBuffer('\n', false);

        return -1;
    } else {
        /* If we've already been here once, exit now */
        been_there_done_that = 1;
        return (psuedo_main(argc, argv));
    }
}
