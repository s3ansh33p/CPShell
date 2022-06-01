#include "internal.hpp"

static int been_there_done_that = 0;

static const struct Applet applets[] = {

#ifdef CPS_MAIN		//bin
    {"cpshell", cpshell_main},
#endif
    {0}
};

int cpshell_main(int argc, char **argv)
{
    return (-1);
}
