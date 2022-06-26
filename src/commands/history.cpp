/**
 * @file history.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays the history of commands.
 * @version 1.0
 * @date 2022-06-15
 */

#include "../internal.hpp"
#include <sdk/os/file.hpp>

// write to history file with int argc, char **argv 
extern int history_main(int argc, char **argv) {
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
            findClose(findHandle);
            close(fd);
            return -1;
        }
        // close the file
        close(fd);

        // as history file will be empty, return
        return 0;

    // history file exists, check if it is a directory
    } else if (findInfoBuf.type == findInfoBuf.EntryTypeDirectory) {
        // history file is a directory
        strcpy(outBuf, "History file is a directory.\n");
        terminal->WriteChars(outBuf);
        findClose(findHandle);
        return -1;
    }

    findClose(findHandle);

    // just cat the file for now
    char *argv2[3];
    argv2[0] = "cat";
    argv2[1] = g_history;
    argv2[2] = 0;
    cat_main(2, argv2);
    
    return 0; // return 0 on success
}
