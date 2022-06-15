/**
 * @file utilities.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Utility functions for the CPShell.
 * @version 1.0
 * @date 2022-06-15
 */

#include "internal.hpp"
#include <sdk/os/file.hpp>

// write to history file with int argc, char **argv 
int add_history(int argc, char **argv) {
    terminal->ClearBuffer();
    char outBuf[BUF_SIZE];

    // check if history file exists
    int findHandle;
    wchar_t fileName[100];
    struct findInfo findInfoBuf;
    int ret = findFirst(g_whistory, &findHandle, fileName, &findInfoBuf);
    if (ret < 0) {
        // history file does not exist
        // create the file
        int fd = open(g_history, OPEN_WRITE | OPEN_CREATE);
        if (fd < 0) {
            // failed to create file
            strcpy(outBuf, "Failed to create history file.\n");
            terminal->WriteChars(outBuf);
            close(fd);
            findClose(findHandle);
            return -1;
        }
        // close the file
        close(fd);

    // history file exists, check if it is a directory
    } else if (findInfoBuf.type == findInfoBuf.EntryTypeDirectory) {
        // history file is a directory
        strcpy(outBuf, "History file is a directory.\n");
        terminal->WriteChars(outBuf);
        findClose(findHandle);
        return -1;
    }

    findClose(findHandle);

    // history file exists, add to it
    int fd = open(g_history, OPEN_WRITE);
    if (fd < 0) {
        // An error occurred calling open
        strcpy(outBuf, "An error occurred calling open.\n");
        terminal->WriteChars(outBuf);
        close(fd);
        return -1;
    }

    // write to the end of the file
    lseek(fd, 0, SEEK_END);

    // write to file
    // for now just do first argv
    ret = write(fd, argv[0], strlen(argv[0]));
     if (ret < 0) {
        // An error occurred calling write
        strcpy(outBuf, "An error occurred calling write.\n");
        terminal->WriteChars(outBuf);
        close(fd);
        return -1;
    }
    // write newline
    ret = write(fd, "\n", 1);
    if (ret < 0) {
        // An error occurred calling write
        strcpy(outBuf, "An error occurred calling write.\n");
        terminal->WriteChars(outBuf);
        close(fd);
        return -1;
    }

    // close the file
    ret = close(fd);
    if (ret < 0) {
        // An error occurred calling close
        strcpy(outBuf, "An error occurred calling close.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }

    return 0; // return 0 on success
}
