/**
 * @file help.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays help information.
 * @version 1.1
 * @date 2022-06-13
 */

#include "../internal.hpp"

extern int help_main(int argc, char **argv)
{
    struct Applet *a = applets;
    terminal->ClearBuffer();
    char msg[] = "Running CPShell Version ";
    strcat(msg, CPS_VERSION);
    strcat(msg, "\nWritten by Sean McGinty (s3ansh33p)\n");
    // Additional information in creditsCurrently defined functions:\n
    terminal->WriteChars(msg);

    char cmds[APPLET_SIZE];
    while (a->name[0] != 0) {
        strcpy(cmds, (a++)->name);
        // check if terminal->bufferCX is at the end of the line + 2 for ', '
        if ((terminal->bufferCX + strlen(a->name) + 2) >= terminal->xmax) {
            terminal->WriteBuffer('\n', false);
            terminal->ClearBuffer();
        }
        strcat(cmds, ", ");
        terminal->WriteChars(cmds, true);
    }
    // remove last comma
    terminal->RemoveLast();
    terminal->RemoveLast();
    terminal->WriteBuffer('\n', false);
    return 0; // return 0 on success
};
