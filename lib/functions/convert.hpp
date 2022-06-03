#pragma once

// convert int to char withouth using library functions
char *numToString(int num) {
    char *str = (char *)malloc(sizeof(char) * 10);
    int i = 0;
    while (num > 0) {
        str[i] = num % 10 + '0';
        num /= 10;
        i++;
    }
    str[i] = '\0';
    return str;
}
