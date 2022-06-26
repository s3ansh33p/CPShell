#pragma once

// convert int to char without using library functions
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

// convert char array to int without using library functions
int stringToNum(char *str) {
    int num = 0;
    int i = 0;
    while (str[i] != '\0') {
        num = num * 10 + str[i] - '0';
        i++;
    }
    return num;
}

// date convert - faster
char *dateToString(int date) {
    char *str = (char *)malloc(sizeof(char) * 3);
    str[0] = date / 10 + '0';
    str[1] = date % 10 + '0';
    str[2] = '\0';
    return str;
}

// will always be 20XX
char *yearToString(unsigned char src) { 
    static const char outputs[] = "0123456789ABCDEF";
    char *str = (char *)malloc(sizeof(char) * 5);
    str[0] = '2';
    str[1] = '0';
    str[2] = outputs[(src>>4)&0xf];
    str[3] = outputs[src&0xf];
    str[4] = '\0';
    return str;
}
