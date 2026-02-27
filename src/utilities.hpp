/**
 * @file utilities.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Utility functions for the CPShell.
 * @version 1.1
 * @date 2022-06-27
 */

#pragma once

#include "internal.hpp"
#include <sdk/os/file.h>

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
    for (int i = start; i < start + (int)strlen(str2); i++) {
        if (str1[i] != str2[i - start]) {
            return 0;
        }
    }
    return 1;
}

// "Safe" file operations
int safe_internal(int ret, const char *msg) {
    if (ret < 0) {
        terminal->WriteChars(msg);
        File_Close(ret);
        return -1;
    }
    return ret;
}

int safe_read(int fd, char *buf, int len) {
    int ret = File_Read(fd, buf, len);
    return safe_internal(ret, "An error occurred calling read.\n");
}

int safe_write(int fd, const char *buf, int len) {
    int ret = File_Write(fd, buf, len);
    return safe_internal(ret, "An error occurred calling write.\n");
}

int safe_open(const char *path, int flags) {
    int ret = File_Open(path, flags);
    return safe_internal(ret, "An error occurred calling open.\n");
}

int safe_close(int fd) {
    int ret = File_Close(fd);
    return safe_internal(ret, "An error occurred calling close.\n");
}

int safe_lseek(int fd, int offset, int whence) {
    int ret = File_Lseek(fd, offset, (File_Whence)whence);
    return safe_internal(ret, "An error occurred calling lseek.\n");
}

// write to history file with int argc, char **argv 
int add_history(int argc, char **argv) {
    terminal->ClearBuffer();
    char outBuf[BUF_SIZE];

    // check if history file exists
    int findHandle;
    char_const16_t fileName[100];
    struct File_FindInfo findInfoBuf;
    int ret = File_FindFirst((const char_const16_t*)(const char_const16_t*)g_whistory, &findHandle, fileName, &findInfoBuf);
    if (ret < 0) {
        // history file does not exist
        // create the file
        int fd = File_Open(g_history, FILE_OPEN_WRITE | FILE_OPEN_CREATE);
        if (fd < 0) {
            // failed to create file
            strcpy(outBuf, "Failed to create history file.\n");
            terminal->WriteChars(outBuf);
            File_Close(fd);
            File_FindClose(findHandle);
            return -1;
        }
        // close the file
        safe_close(fd);

    // history file exists, check if it is a directory
    } else if (findInfoBuf.type == File_FindInfo::EntryTypeDirectory) {
        // history file is a directory
        strcpy(outBuf, "History file is a directory.\n");
        terminal->WriteChars(outBuf);
        File_FindClose(findHandle);
        return -1;
    }

    File_FindClose(findHandle);

    // history file exists, add to it
    int fd = safe_open(g_history, FILE_OPEN_WRITE);

    // write to the end of the file
    safe_lseek(fd, 0, FILE_SEEK_END);

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
