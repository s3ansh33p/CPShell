/**
 * @file cat.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays the contents of a file.
 * @version 1.0
 * @date 2022-06-12
 */

#include "../internal.hpp"
#include <sdk/os/file.hpp>

extern int cat_main(int argc, char **argv)
{
    terminal->ClearBuffer();

    char outBuf[BUF_SIZE];

    // check if there is a path
    if (argc == 1) {
        // no path
        strcpy(outBuf, "No path specified.\n");
        terminal->WriteChars(outBuf);
        return 0;
    }

    char path[PATH_LEN];
    // check if path given starts with '\\'
    if (argv[1][0] != '\\') strcpy(path, g_path);
    strcat(path, argv[1]);
    // convert to wchar_t
    wchar_t wpath[PATH_LEN];
    for (int i = 0; i < PATH_LEN; i++) {
        wpath[i] = path[i];
    }

    // check if the path exists
    int findHandle;
    wchar_t fileName[100];
    struct findInfo findInfoBuf;
    int ret = findFirst(wpath, &findHandle, fileName, &findInfoBuf);
    if (ret < 0) {
        // path does not exist
        strcpy(outBuf, "Path does not exist.\n");
        terminal->WriteChars(outBuf);
        
        // close the file
        findClose(findHandle);
        return 0;
    }

    // path exists, check if it is a directory
    if (findInfoBuf.type == findInfoBuf.EntryTypeDirectory) {
        // path is a directory
        strcpy(outBuf, "Path is a directory.\n");
        terminal->WriteChars(outBuf);
        
        // close the file
        findClose(findHandle);
        return 0;
    }

    // close the file
    findClose(findHandle);

    // now that we know the path is a file, open it
    int fd = open(path, OPEN_READ);
    if (fd < 0) {
        // An error occurred calling open
        strcpy(outBuf, "An error occurred calling open.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }

    // copy memory address
	uint8_t* addr;
	getAddr(fd,0,(const void**)&addr);

    // close the file
    ret = close(fd);
    if (ret < 0) {
        // An error occurred calling close
        strcpy(outBuf, "An error occurred calling close.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }

    // using the address, loop through the file and print it
	uint32_t byteIndex = 0;
    unsigned char ch = addr[byteIndex];
    // loop through first 300 chars or number specified in argv[2]
    // check if there is a number
    uint32_t numChars = 300;
    if (argc == 3) {
        numChars = stringToNum(argv[2]);
    }

    for (uint32_t i = 0; i < numChars; i++) {
        // check if we are at the end of the file
        if (ch == 0) {
            // we are at the end of the file
            break;
        }
        // print the character
        terminal->WriteBuffer(ch, false);
        // increment the byte index
        byteIndex++;
        // get the next character
        ch = addr[byteIndex];
    }

    // write a newline
    terminal->WriteBuffer('\n', false);

    return 0; // return 0 on success
};
