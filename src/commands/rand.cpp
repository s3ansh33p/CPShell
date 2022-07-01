/**
 * @file rand.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Sends a random number to the terminal.
 * @version 1.0
 * @date 2022-07-02
 */

extern int rand_main(int argc, char **argv)
{
    // check if second argument starts with '-'
    char outBuf[BUF_SIZE];
    if (argc > 1 && argv[1][0] == '-') {
        // check if second argument is '-h'
        if (strcmp(argv[1], "-h") == 0) {
            strcpy(outBuf, "Rand: Returns a psuedo-random number.\nUsage: rand\n-h:  Displays this help message.\n<n>: Returns a number between 0 & n\n\nExamples\nrand               Number up to 100\nrand 98234         Number up to 98234\n");
            terminal->WriteChars(outBuf);
            return 0;
        } else {
            strcpy(outBuf, "Invalid argument.\n");
            terminal->WriteChars(outBuf);
            return 0;
        }
    }

    int num = 100;
    if (argc == 2) num = stringToNum(argv[1]);
    uint32_t outNum = rng->Generate(num);
    // for some reason, the string returned is reversed.
    char toReverse[10];
    strcpy(toReverse, numToString(outNum));
    for (int i = 0, j = strlen(toReverse) - 1; i < j; i++, j--) {
        char temp = toReverse[i];
        toReverse[i] = toReverse[j];
        toReverse[j] = temp;
    }
    strcpy(outBuf, toReverse);
    strcat(outBuf, "\n");
    terminal->WriteChars(outBuf);
    return 0;
};
