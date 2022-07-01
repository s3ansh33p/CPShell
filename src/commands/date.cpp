/**
 * @file date.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays the system date and time.
 * @version 1.2
 * @date 2022-07-02
 */

#include "../../lib/core/rtc.hpp"

extern int date_main(int argc, char **argv)
{
    // check if second argument starts with '-'
    char outBuf[BUF_SIZE];
    if (argc > 1 && argv[1][0] == '-') {
        // check if second argument is '-h'
        if (strcmp(argv[1], "-h") == 0) {
            strcpy(outBuf, "Date: Displays system date and time.\nUsage: date\n-h: Displays this help message.\n-d: Displays the date.\n-t: Displays the time.\n-s: Sets the date and time.\n\n-s  <sec> <min> <hr> <day> <mnth> <yr>\n-s  [sec|seconds] <seconds>\n-s  [min|minutes] <minutes>\n-s  [hr|hours] <hours>\n-s  [day|days] <days>\n-s  [mnth|months] <months>\n-s  [yr|years] <years>\n\nExamples\ndate               Shows date & time\ndate -s min 50     Sets minutes to 50\ndate -s year 22    Sets year to 2022\n");
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
            // first check if 3rd arg is time descriptor
            if (argc > 3) {
                if (strcmp(argv[2], "seconds") == 0 || strcmp(argv[2], "sec") == 0) {
                    int8_t seconds = stringToNum(argv[3]);
                    if (seconds < 0 || seconds > 59) {
                        strcpy(outBuf, "Invalid seconds (must be between 0 & 59).\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    seconds = ((seconds/10)<<4) + (seconds%10);
                    rtc.setSeconds(seconds);
                    strcpy(outBuf, "Seconds set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                } else if (strcmp(argv[2], "minutes") == 0 || strcmp(argv[2], "min") == 0) {
                    int8_t minutes = stringToNum(argv[3]);
                    if (minutes < 0 || minutes > 59) {
                        strcpy(outBuf, "Invalid minutes (must be between 0 & 59).\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    minutes = ((minutes/10)<<4) + (minutes%10);
                    rtc.setMinutes(minutes);
                    strcpy(outBuf, "Minutes set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                } else if (strcmp(argv[2], "hours") == 0 || strcmp(argv[2], "hr") == 0) {
                    int8_t hours = stringToNum(argv[3]);
                    if (hours < 0 || hours > 23) {
                        strcpy(outBuf, "Invalid hours (must be between 0 & 23).\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    hours = ((hours/10)<<4) + (hours%10);
                    rtc.setHours(hours);
                    strcpy(outBuf, "Hours set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                } else if (strcmp(argv[2], "days") == 0) {
                    // get the current month
                    int8_t month = rtc.getMonth();
                    // depending on the month, get the number of days in that month
                    int8_t days = 0;
                    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
                        days = 31;
                    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                        days = 30;
                    } else if (month == 2) {
                        // check if year is leap year
                        if (rtc.getYear() % 4 == 0) {
                            days = 29;
                        } else {
                            days = 28;
                        }
                    }
                    int8_t day = stringToNum(argv[3]); 
                    // check if day is valid
                    if (day < 0 || day > days) {
                        strcpy(outBuf, "Invalid day (Must be between 0 & ");
                        strcat(outBuf, dateToString(days));
                        strcat(outBuf, ").\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }                    

                    rtc.setDay(day);
                    strcpy(outBuf, "Day set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                } else if (strcmp(argv[2], "months") == 0 || strcmp(argv[2], "mnth") == 0) {
                    int8_t month = stringToNum(argv[3]);
                    if (month < 1 || month > 12) {
                        strcpy(outBuf, "Invalid month (Must be between 1 & 12).\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    rtc.setMonth(month);
                    strcpy(outBuf, "Month set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                } else if (strcmp(argv[2], "years") == 0 || strcmp(argv[2], "yr") == 0) {
                    int16_t year = stringToNum(argv[3]);
                    if (year < 0 || year > 99) {
                        strcpy(outBuf, "Invalid year (Must be between 0 & 99).\n");
                        terminal->WriteChars(outBuf);
                        return 0;
                    }
                    // Convert to bcd
                    year = ((year/10)<<4) + (year%10);
                    rtc.setYear(year);
                    strcpy(outBuf, "Year set.\n");
                    terminal->WriteChars(outBuf);
                    return 0;
                }
            }
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
