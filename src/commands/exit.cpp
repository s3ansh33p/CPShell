#include <string.h>
#include <stdlib.h>
/**
 * @file exit.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Exits the shell.
 * @version 1.0
 * @date 2022-06-06
 */

extern int exit_main(int, char **)
{
	shell_running = false;
    return 0;
};
