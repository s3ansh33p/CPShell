/**
 * @file exit.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Exits the shell.
 * @version 1.0
 * @date 2022-06-06
 */

extern int exit_main(int argc, char **argv)
{
	shell_running = false;
    return 0;
};
