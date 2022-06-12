/**
 * @file cd.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Changes the current working directory.
 * @version 1.1
 * @date 2022-06-12
 */

#include "../internal.hpp"
#include <sdk/os/file.hpp>

extern int cd_main(int argc, char **argv)
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

    // check if the path exists
    int findHandle;
    wchar_t fileName[100];
    struct findInfo findInfoBuf;
    int ret = findFirst(g_wpath, &findHandle, fileName, &findInfoBuf);
    if (ret < 0) {
        // path does not exist
        strcpy(outBuf, "Path does not exist.\n");
        terminal->WriteChars(outBuf);
        
        // close the file
        findClose(findHandle);
        return 0;
    }

    // path exists, check if it is a directory
    if (findInfoBuf.type != findInfoBuf.EntryTypeDirectory) {
        // path is not a directory
        strcpy(outBuf, "Path is not a directory.\n");
        terminal->WriteChars(outBuf);
        
        // close the file
        findClose(findHandle);
        return 0;
    }

    // close the file
    findClose(findHandle);

    // check for .. - will need to change this in the future to support ../../.. etc
    if (strcmp(argv[1], "..") == 0) {
        // go up a directory
        char parentDir[PATH_LEN];
        // search for the last '\\'
        int lastSlash = -1;
        for (int i = 0; i < strlen(g_path) - 1; i++) { // len -1 as will always be a slash
            if (g_path[i] == '\\') {
                lastSlash = i;
            }
        }
        // copy the parent directory
        for (int i = 0; i < lastSlash; i++) {
            parentDir[i] = g_path[i];
        }
        // add the slash back
        parentDir[lastSlash] = '\\';
        // set the new path
        strcpy(g_path, parentDir);

        // check if path starts with a backslash
    } else if (argv[1][0] == '\\') {
        // absolute path
        strcpy(g_path, argv[1]);
    } else {
        // relative path
        strcat(g_path, argv[1]);
    }

    // check if g_path ends with a backslash
    if (g_path[strlen(g_path) - 1] != '\\') {
        // add a backslash
        strcat(g_path, "\\");
    }

    //convert from char to wchar
    for(int i=0; g_path[i]!=0; i++){
        wchar_t ch = g_path[i];
        g_wpath[i] = ch;
    }

    // fill the rest of the wchar array with 0's
    for(int i=strlen(g_path); i<PATH_LEN; i++){
        g_wpath[i] = 0;
    }
    
    //add the * to the file path 
    {
        int i=0;
        while(g_wpath[i]!=0)i++; //seek to the end of the string
        g_wpath[i++]='*'; //add an *
        g_wpath[i  ]= 0 ; //add the 0
    }

    return 0; // return 0 on success
};
