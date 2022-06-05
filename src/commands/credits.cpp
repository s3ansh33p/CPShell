/**
 * @file credits.cpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Displays credits.
 * @version 1.0
 * @date 2022-06-06
 */

extern int credits_main(int argc, char **argv)
{
    terminal->ClearBuffer();
    char msg[] = "Written by Sean McGinty (s3ansh33p)\nCredits to:\n- diddyholz for command initialization\n- Interchan for drawing functions\n- Pho3 for general assistance\n";
    terminal->WriteChars(msg);
    return 0; // return 0 on success
};
