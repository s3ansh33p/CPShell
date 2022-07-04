#pragma once

// convert int to char without using library functions
char *numToString(uint32_t num) {
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
uint32_t stringToNum(char *str) {
    int num = 0;
    int i = 0;
    bool isNegative = false;
    if(str[0] == '-'){
        isNegative = true;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9')){
        num = num * 10 + (str[i] - '0');
        i++;
    }
    if(isNegative) num = -1 * num;
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

// for reading user profile, so "0xFF11" is 65297
uint32_t hexStringToInt(char *str) {
    uint32_t num = 0;
    int i = 0;
    if(str[0] == '0' && str[1] == 'x') i = 2;
    while (str[i] && (str[i] >= '0' && str[i] <= '9' || str[i] >= 'a' && str[i] <= 'f' || str[i] >= 'A' && str[i] <= 'F')){
        if(str[i] >= '0' && str[i] <= '9'){
            num = num * 16 + (str[i] - '0');
        } else if(str[i] >= 'a' && str[i] <= 'f'){
            num = num * 16 + (str[i] - 'a' + 10);
        } else if(str[i] >= 'A' && str[i] <= 'F'){
            num = num * 16 + (str[i] - 'A' + 10);
        }
        i++;
    }
    return num;
}

// ascii to hex
char *asciiToHex(char *str) {
    char *hex = (char *)malloc(sizeof(char) * (strlen(str) * 2 + 1));
    int i = 0;
    while (str[i]) {
        hex[i * 2] = str[i] / 16 + '0';
        hex[i * 2 + 1] = str[i] % 16 + '0';
        i++;
    }
    hex[i * 2] = '\0';
    return hex;
}

// hex to ascii
char *hexToAscii(char *str) {
    char *ascii = (char *)malloc(sizeof(char) * (strlen(str) / 2 + 1));
    int i = 0;
    while (str[i * 2] && str[i * 2 + 1]) {
        ascii[i] = (str[i * 2] - '0') * 16 + (str[i * 2 + 1] - '0');
        i++;
    }
    ascii[i] = '\0';
    return ascii;
}
