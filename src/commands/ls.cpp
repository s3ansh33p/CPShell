/**
 * @file ls.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Lists the contents of a directory.
 * @version 1.0
 * @date 2022-06-09
 */

#include "../internal.hpp"
#include <sdk/os/file.hpp>

extern int ls_main(int argc, char **argv)
{
    // clear buffer
    terminal->ClearBuffer();
    
    // init file variables
    int dirFiles = 0;

    int findHandle;
    wchar_t fileName[100];
    char outBuf[110];
    struct findInfo findInfoBuf;
    int ret = findFirst(g_wpath, &findHandle, fileName, &findInfoBuf);
    while (ret>=0){
        //create dirEntry structure
        struct dirEntry thisfile;
        memset(&thisfile, 0, sizeof(thisfile));
        //copy file name
        for (int i=0; fileName[i]!=0; i++){
            wchar_t ch = fileName[i];
            thisfile.fileName[i] = ch;
        }
        //copy file type
        thisfile.type=findInfoBuf.type==findInfoBuf.EntryTypeDirectory?'D':'F';
        //display this
        strcpy(outBuf, thisfile.fileName);
        // check if it will fit on the screen or we are in second half
        if ((terminal->bufferCX + strlen(outBuf)) >= terminal->xmax || (terminal->bufferCX + 1) >= (terminal->xmax/2)) {
            terminal->WriteBuffer('\n', false);
            terminal->ClearBuffer();
        } else if (terminal->bufferCX > 0) {
            // change to second half
            terminal->bufferCX = (terminal->xmax/2);
        }
        // check if directory
        uint32_t newColor = thisfile.type=='D'?color(255,0,0):0xFFFF; // red for directories, white for files
        terminal->SetColor(newColor);
        terminal->WriteChars(outBuf, true);
        //save this dirEntry to directory
        directory[dirFiles++] = thisfile;
        
        //serch the next
        ret = findNext(findHandle, fileName, &findInfoBuf);
    }
    findClose(findHandle);

    terminal->WriteBuffer('\n', false);
    return 0; // return 0 on success
};
