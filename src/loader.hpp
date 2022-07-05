/**
 * @file loader.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Loads a user settings at launch.
 * @version 1.0
 * @date 2022-07-03
 */

#pragma once

#include "internal.hpp"
#include "utilities.hpp"
#include "../lib/functions/convert.hpp"
#include <sdk/os/file.hpp>

// Terminal - Defaults here
uint32_t TERM_COLOR = 0xFFFF;
uint32_t TERM_CURSOR = 0xFFFF;
bool     HISTORY_ENABLED = true;

void load_settings() {
    terminal->SetColor(TERM_COLOR);
    terminal->SetCursorColor(TERM_CURSOR);
}

int load_userprofile() {
    char outBuf[BUF_SIZE];
    terminal->ClearBuffer();

    int findHandle;
    wchar_t fileName[100];
    struct findInfo findInfoBuf;
    int ret = findFirst(g_wuserprofile, &findHandle, fileName, &findInfoBuf);
    if (ret < 0) {
        // Does not exist so skip
        strcpy(outBuf, "LOAD: Skipping user profile load.\n");
        terminal->WriteChars(outBuf);
        findClose(findHandle);
        load_settings();
        return 0;
    } else if (findInfoBuf.type == findInfoBuf.EntryTypeDirectory) {
        strcpy(outBuf, "LOAD: User profile is a directory.\n");
        terminal->WriteChars(outBuf);
        findClose(findHandle);
        return -2;
    }

    findClose(findHandle);

    int fd = open(g_userprofile, OPEN_READ);
    if (fd < 0) {
        close(fd);
        strcpy(outBuf, "LOAD: File error.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }    

	uint8_t* addr;
	getAddr(fd,0,(const void**)&addr);

    ret = close(fd);
    if (ret < 0) {
        strcpy(outBuf, "LOAD: File error.\n");
        terminal->WriteChars(outBuf);
        return -1;
    }

    // read the user profile from memory
    uint32_t byteIndex = 0;
    uint32_t lineNum = 1;
    uint32_t fileLength = findInfoBuf.size;

    // assume a line is less than 512 characters long
    char *line = (char*)malloc(sizeof(char) * BUF_SIZE);
    while (byteIndex < fileLength) {
        // read a line
        uint32_t lineLength = 0;
        while (addr[byteIndex] != '\n' && byteIndex < fileLength) {
            // check if line is too long
            if (lineLength >= BUF_SIZE) {
                lineLength = 0;
                // write error message
                strcpy(outBuf, "LOAD: Line ");
                strcat(outBuf, numToString(lineNum));
                strcat(outBuf, " is too long.\n");
                terminal->WriteChars(outBuf);
                break;
            }
            line[lineLength] = addr[byteIndex];
            lineLength++;
            byteIndex++;
        }
        line[lineLength] = '\0';
        byteIndex++;
        // parse the line
        if (lineLength > 0) {
            // parse the line
            // check if the line starts with '#'
            if (line[0] == '#') {
                // ignore this line
            } else {
                // parse the line
                // check if starts with 'TERM_'
                // min length here is TERM_COLOR=FFFF so 15
                if (lineLength > 14) {
                    if(comparePartial(line, "TERM_", 0)) {
                        // check if keyword is 'TERM_COLOR'
                        if (comparePartial(line, "COLOR", 5)) {
                            // ensure that length after '=' is 4 or 6
                            if (lineLength == 15 || lineLength == 17) {
                                TERM_COLOR = hexStringToInt(line + 11);
                                strcpy(outBuf, "LOAD: TERM_COLOR set to ");
                                strcat(outBuf, line + 11);
                                strcat(outBuf, "\n");
                                terminal->WriteChars(outBuf);
                            } else {
                                // invalid length
                                strcpy(outBuf, "LOAD: Invalid TERM_COLOR length\n");
                                terminal->WriteChars(outBuf);
                            }
                        } else if (comparePartial(line, "CURSOR", 5)) {
                            // ensure that length after '=' is 4 or 6
                            if (lineLength == 16 || lineLength == 18) {
                                TERM_CURSOR = hexStringToInt(line + 12);
                                strcpy(outBuf, "LOAD: TERM_CURSOR set to ");
                                strcat(outBuf, line + 12);
                                strcat(outBuf, "\n");
                                terminal->WriteChars(outBuf);
                            } else {
                                // invalid length
                                strcpy(outBuf, "LOAD: Invalid TERM_CURSOR length\n");
                                terminal->WriteChars(outBuf);
                            }
                        } else {
                            // invalid keyword
                            strcpy(outBuf, "LOAD: Invalid TERM_ keyword on line ");
                            strcat(outBuf, numToString(lineNum));
                            strcat(outBuf, "\n");
                            terminal->WriteChars(outBuf);
                        }
                    } else if (comparePartial(line, "HISTORY_", 0)) {
                        // check if keyword is 'HISTORY_ENABLED'
                        if (comparePartial(line, "ENABLED", 8)) {
                            // ensure that after '=' is ON or OFF
                            if (lineLength == 18 || lineLength == 19) {
                                if (comparePartial(line, "ON", 16)) {
                                    // enable history
                                    strcpy(outBuf, "LOAD: HISTORY_ENABLED set to ON\n");
                                    terminal->WriteChars(outBuf);
                                    HISTORY_ENABLED = true;
                                } else if (comparePartial(line, "OFF", 16)) {
                                    // disable history
                                    strcpy(outBuf, "LOAD: HISTORY_ENABLED set to OFF\n");
                                    terminal->WriteChars(outBuf);
                                    HISTORY_ENABLED = false;
                                } else {
                                    // invalid value
                                    strcpy(outBuf, "LOAD: Invalid HISTORY_ENABLED value\n");
                                    terminal->WriteChars(outBuf);
                                }
                            } else {
                                // invalid length
                                strcpy(outBuf, "LOAD: Invalid HISTORY_ENABLED length\n");
                                terminal->WriteChars(outBuf);
                            }
                        } else {
                            // invalid keyword
                            strcpy(outBuf, "LOAD: Invalid HISTORY_ keyword on line ");
                            strcat(outBuf, numToString(lineNum));
                            strcat(outBuf, "\n");
                            terminal->WriteChars(outBuf);
                        }
                    } else {
                        // invalid keyword
                        strcpy(outBuf, "LOAD: Invalid keyword on line ");
                        strcat(outBuf, numToString(lineNum));
                        strcat(outBuf, "\n");
                        terminal->WriteChars(outBuf);
                    }
                }
            }
        }
        lineNum++;
    }

    // free the line buffer
    free(line);

    strcpy(outBuf, "LOAD: User profile loaded.\n");
    terminal->WriteChars(outBuf);

    load_settings();

    return 0; // return 0 on success
}
