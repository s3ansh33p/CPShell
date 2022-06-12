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

// conert char array to int without using library functions
int stringToNum(char *str) {
    int num = 0;
    int i = 0;
    while (str[i] != '\0') {
        num = num * 10 + str[i] - '0';
        i++;
    }
    return num;
}
