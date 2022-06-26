/**
 * @file date.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays the system date and time.
 * @version 1.0
 * @date 2022-06-27
 */

#include "../../lib/core/rtc.hpp"

extern int date_main(int argc, char **argv)
{
    char outBuf[32];
    strcpy(outBuf, dateToString(rtc.getDay()));
    strcat(outBuf, "/");
    strcat(outBuf, dateToString(rtc.getMonth()));
    strcat(outBuf, "/");
    strcat(outBuf, yearToString(rtc.getYear()));
    strcat(outBuf, " ");
    strcat(outBuf, dateToString(rtc.getHours()));
    strcat(outBuf, ":");
    strcat(outBuf, dateToString(rtc.getMinutes()));
    strcat(outBuf, ":");
    strcat(outBuf, dateToString(rtc.getSeconds()));
    strcat(outBuf, "\n");
    terminal->WriteChars(outBuf);
    return 0;
};
