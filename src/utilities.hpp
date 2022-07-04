/**
 * @file utilities.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Utility functions for the CPShell.
 * @version 1.1
 * @date 2022-06-27
 */

#pragma once

#include "internal.hpp"
#include <sdk/os/file.hpp>

// Get the username from memory address 0x8C1BE984
char *getUsername()
{
    char *userName = (char *)0x8C1BE984;
    // get username until null byte or length is 88
    int i = 0;
    while (userName[i] != '\0' && i < 88) {
        i++;
    }
    // allocate memory for username
    char *user = (char *)malloc(i + 1);
    // copy username into memory
    for (int j = 0; j < i; j++) {
        user[j] = userName[j];
    }
    // null terminate string
    user[i] = '\0';
    return user;
}

// String comparison
int comparePartial(const char *str1, const char *str2, int start) {
    for (int i = start; i < start + strlen(str2); i++) {
        if (str1[i] != str2[i - start]) {
            return 0;
        }
    }
    return 1;
}

// "Safe" file operations
int safe_internal(int ret, char *msg) {
    if (ret < 0) {
        terminal->WriteChars(msg);
        close(ret);
        return -1;
    }
    return ret;
}

int safe_read(int fd, char *buf, int len) {
    int ret = read(fd, buf, len);
    return safe_internal(ret, "An error occurred calling read.\n");
}

int safe_write(int fd, char *buf, int len) {
    int ret = write(fd, buf, len);
    return safe_internal(ret, "An error occurred calling write.\n");
}

int safe_open(char *path, int flags) {
    int ret = open(path, flags);
    return safe_internal(ret, "An error occurred calling open.\n");
}

int safe_close(int fd) {
    int ret = close(fd);
    return safe_internal(ret, "An error occurred calling close.\n");
}

int safe_lseek(int fd, int offset, int whence) {
    int ret = lseek(fd, offset, whence);
    return safe_internal(ret, "An error occurred calling lseek.\n");
}

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
        safe_close(fd);

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
    int fd = safe_open(g_history, OPEN_WRITE);

    // write to the end of the file
    safe_lseek(fd, 0, SEEK_END);

    // write to file
    for (int i = 0; i < argc; i++) {
        strcpy(outBuf, argv[i]);
        safe_write(fd, outBuf, strlen(outBuf));
        // add a space between arguments
        if (i < argc - 1) {
            strcpy(outBuf, " ");
            safe_write(fd, outBuf, strlen(outBuf));
        }
    }

    // write newline
    safe_write(fd, "\n", 1);

    // close the file
    safe_close(fd);

    return 0; // return 0 on success
}
