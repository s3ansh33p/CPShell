/**
 * @file osname.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Display and edit the name of the operating system.
 * @version 1.0
 * @date 2022-07-05
 */

extern int osname_main(int argc, char **argv)
{
    char outBuf[BUF_SIZE];

    // read from memory address 808FCC10
    char *osName = (char *)0x808FCC10;
    // copy first 16 bytes into a buffer
    char buf[16];
    for (int i = 0; i < 16; i++) {
        buf[i] = osName[i];
    }
    strcpy(outBuf, "OS Name: ");
    strcat(outBuf, buf);
    strcat(outBuf, "\n");
    terminal->WriteChars(outBuf);
    return 0;
};

