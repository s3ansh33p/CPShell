#include <string.h>
#include <stdlib.h>
/**
 * @file ls.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Lists the contents of a directory.
 * @version 1.0
 * @date 2022-06-09
 */

#include "../internal.hpp"
#include <sdk/os/file.h>
#include <sdk/os/mem.h>

extern int ls_main(int, char **)
{
    // clear buffer
    terminal->ClearBuffer();
    
    // init file variables
    int dirFiles = 0;

    int findHandle;
    char_const16_t fileName[100] __attribute__((aligned(4)));
    char outBuf[110];
    struct File_FindInfo findInfoBuf __attribute__((aligned(4)));
    int ret = File_FindFirst((const char_const16_t*)g_wpath, &findHandle, fileName, &findInfoBuf);
    while (ret>=0){
        //create dirEntry structure
        struct dirEntry thisfile;
        Mem_Memset(&thisfile, 0, sizeof(thisfile));
        //copy file name
        for (int i=0; fileName[i]!=0; i++){
            char_const16_t ch = fileName[i];
            thisfile.fileName[i] = ch;
        }
        //copy file type
        thisfile.type=findInfoBuf.type==File_FindInfo::EntryTypeDirectory?'D':'F';
        //display this
        strcpy(outBuf, thisfile.fileName);
        // check if directory
        uint32_t newColor = thisfile.type=='D'?color(255,0,0):0xFFFF; // red for directories, white for files
        terminal->SetColor(newColor);
        terminal->WriteChars(outBuf, true);
        terminal->WriteBuffer('\n', false);
        //save this dirEntry to directory
        directory[dirFiles++] = thisfile;
        
        //serch the next
        ret = File_FindNext(findHandle, fileName, &findInfoBuf);
    }
    File_FindClose(findHandle);

    terminal->WriteBuffer('\n', false);
    return 0; // return 0 on success
};
