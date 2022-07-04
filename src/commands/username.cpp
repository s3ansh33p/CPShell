/**
 * @file username.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Display and edit the name of the operating system.
 * @version 1.0
 * @date 2022-07-05
 */

extern int username_main(int argc, char **argv)
{
    // check if second argument starts with '-'
    char outBuf[BUF_SIZE];
    if (argc > 1 && argv[1][0] == '-') {
        // check if second argument is '-h'
        if (strcmp(argv[1], "-h") == 0) {
            strcpy(outBuf, "username: Display and edit the username.\nUsage: username\n-h:  Displays this help message.\n<name>: Sets username to name given\n\nExamples\nusername           Display username\nusername Dev       Sets userame to Dev\n");
            terminal->WriteChars(outBuf);
            return 0;
        } else {
            strcpy(outBuf, "Invalid argument.\n");
            terminal->WriteChars(outBuf);
            return 0;
        }
    }

    if (argc == 1) {
        // call char getUsername
        char *username = getUsername();
        strcpy(outBuf, "Username: ");
        strcat(outBuf, username);
        strcat(outBuf, "\n");
        terminal->WriteChars(outBuf);
        // free username
        free(username);
    } else {
        char *userName = (char *)0x8C1BE984;
        char buf[BUF_SIZE];
        // join the rest of the arguments into a string
        strcpy(buf, argv[1]);
        for (int i = 2; i < argc; i++) {
            strcat(buf, " ");
            strcat(buf, argv[i]);
        }
        // check length of string
        if (strlen(buf) > 88) {
            strcpy(outBuf, "Username too long (max 88 characters).\n");
            terminal->WriteChars(outBuf);
            return 0;
        }        
        // copy into memory
        for (int i = 0; i < 88; i++) {
            userName[i] = buf[i];
        }
        // output
        strcpy(outBuf, "Username set to: ");
        strcat(outBuf, buf);
        strcat(outBuf, "\n");
        terminal->WriteChars(outBuf);
    }
    return 0;
};

