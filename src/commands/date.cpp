/**
 * @file date.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays the system date and time.
 * @version 1.1
 * @date 2022-07-01
 */

#include "../../lib/core/rtc.hpp"

extern int date_main(int argc, char **argv)
{
    // check if second argument starts with '-'
    char outBuf[BUF_SIZE];
    if (argc > 1 && argv[1][0] == '-') {
        // check if second argument is '-h'
        if (strcmp(argv[1], "-h") == 0) {
            strcpy(outBuf, "Date: Displays system date and time.\nUsage: date\n-h: Displays this help message.\n-d: Displays the date.\n-t: Displays the time.\n-s: Sets the date and time.\n    <sec> <min> <hr> <day> <mnth> <yr>\n");
            terminal->WriteChars(outBuf);
            return 0;
        } else if (strcmp(argv[1], "-d") == 0) {
            // Show date
            strcpy(outBuf, dateToString(rtc.getDay()));
            strcat(outBuf, "/");
            strcat(outBuf, dateToString(rtc.getMonth()));
            strcat(outBuf, "/");
            strcat(outBuf, yearToString(rtc.getYear()));
            strcat(outBuf, "\n");
            terminal->WriteChars(outBuf);
            return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            // Show time
            strcat(outBuf, dateToString(rtc.getHours()));
            strcat(outBuf, ":");
            strcat(outBuf, dateToString(rtc.getMinutes()));
            strcat(outBuf, ":");
            strcat(outBuf, dateToString(rtc.getSeconds()));
            strcat(outBuf, "\n");
            terminal->WriteChars(outBuf);
            return 0;
        } else if (strcmp(argv[1], "-s") == 0) {
            // Set the date and time
            // args are '-s <seconds> <minutes> <hours> <days> <months> <years>'
            // supports up to the full 6 inputs
            if (argc > 8) {
                strcpy(outBuf, "Too many arguments.\n");
                terminal->WriteChars(outBuf);
                return 0;
            } else if (argc == 2) {
                strcpy(outBuf, "Not enough arguments.\n");
                terminal->WriteChars(outBuf);
                return 0;
            } else {
                int8_t seconds = 0;
                int8_t minutes = 0;
                int8_t hours = 0;
                int8_t day = 0;
                int8_t month = 0;
                int16_t year = 0;
                if (argc > 2) {
                    seconds = stringToNum(argv[2]);
                    if (seconds < 0 || seconds > 59) {
                        strcpy(outBuf, "Invalid seconds.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    seconds = ((seconds/10)<<4) + (seconds%10);
                }
                if (argc > 3) {
                    minutes = stringToNum(argv[3]);
                    if (minutes < 0 || minutes > 59) {
                        strcpy(outBuf, "Invalid minutes.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    minutes = ((minutes/10)<<4) + (minutes%10);
                }
                if (argc > 4) {
                    hours = stringToNum(argv[4]);
                    if (hours < 0 || hours > 23) {
                        strcpy(outBuf, "Invalid hours.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    hours = ((hours/10)<<4) + (hours%10);
                }
                if (argc > 5) {
                    day = stringToNum(argv[5]);
                    if (day < 1 || day > 31) {
                        strcpy(outBuf, "Invalid day.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                }
                if (argc > 6) {
                    month = stringToNum(argv[6]);
                    if (month < 1 || month > 12) {
                        strcpy(outBuf, "Invalid month.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                }
                if (argc == 8) {
                    // Will be 20XX, so only need last 2 digits
                    year = stringToNum(argv[7]);
                    if (year < 0 || year > 99) {
                        strcpy(outBuf, "Invalid year.\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    year = ((year/10)<<4) + (year%10);
                }
                // based on the number of arguments, set the date and time
                if (argc == 8) {
                    rtc.setDate(seconds, minutes, hours, day, month, year);
                } else if (argc == 7) {
                    rtc.setDate(seconds, minutes, hours, day, month);
                } else if (argc == 6) {
                    rtc.setDate(seconds, minutes, hours, day);
                } else if (argc == 5) {
                    rtc.setDate(seconds, minutes, hours);
                } else if (argc == 4) {
                    rtc.setDate(seconds, minutes);
                } else {
                    rtc.setDate(seconds);
                }

                strcpy(outBuf, "Date and time set.\n");
                terminal->WriteChars(outBuf);
                return 0;
            }                        
        } else {
            strcpy(outBuf, "Invalid argument.\n");
            terminal->WriteChars(outBuf);
            return 0;
        }
    } else {
        // Show date and time (Default behavior)
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
    }
};
