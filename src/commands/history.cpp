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

    // history file exists, add to it
    int fd = open(g_history, OPEN_WRITE);
    if (fd < 0) {
        // An error occurred calling open
        strcpy(outBuf, "An error occurred calling open.\n");
        terminal->WriteChars(outBuf);
        close(fd);
        return -1;
    }

	// uint8_t* addr;
	// getAddr(fd,0,(const void**)&addr);

    // close the file
    ret = close(fd);
    if (ret < 0) {
        // An error occurred calling close
        strcpy(outBuf, "An error occurred calling close.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }

    unsigned char ch;

    // loop through last 10 commands
    // check if there is a number
    uint32_t numLines = 10;
    if (argc == 3) {
        numLines = stringToNum(argv[2]);
    }

    // counter for newlines
    uint32_t newlineCount = 0;

    // lseek to end of file
    lseek(fd, -1, SEEK_END);
    bool end = false;
    // work backwards until we find the final newline

    // storage for the command
    char command[BUF_SIZE];
    uint32_t commandIndex = 0;

    while (!end) {
        // check if we are at the beginning of the file
        if (lseek(fd, -2, SEEK_CUR) < 0) {
            end = true;      
        } else {
            // update character with lseek
            ch = lseek(fd, -1, SEEK_CUR);
            // check if we are at a newline
            if (ch == '\n') {
                // print the command
                command[commandIndex] = '\0';
                // write each character backwards as seeking backwards
                for (int i = commandIndex; i >= 0; i--) {
                    terminal->WriteBuffer(command[i], false);
                }
                terminal->WriteBuffer('\n', false);
                // refresh screen
                LCD_Refresh();

                // reset the command
                commandIndex = 0;
                newlineCount++;
                if (newlineCount == numLines) {
                    // we found the final newline
                    end = true;
                }
            } else {
                // we are not at a newline, add to the command
                command[commandIndex] = ch;
                commandIndex++;
            }
        }
    }
    // close the file
    close(fd);
    // write a newline
    terminal->WriteBuffer('\n', false);

    return 0; // return 0 on success
}
