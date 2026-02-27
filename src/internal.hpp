/**
 * @file internal.hpp
 * @author Sean McGinty (newfolderlocation@gmail.com)
 * @brief Ported from BusyBox main internal header file on 0_32 branch
 * @version 1.1
 * @date 2022-06-09
 */

#ifndef	_INTERNAL_H_
#define	_INTERNAL_H_

/* Some useful definitions */
#define STDIN	0
#define STDOUT	1
#define FALSE   ((int) 1)
#define TRUE    ((int) 0)

#define PATH_LEN        400
#define BUF_SIZE        512
#define EXPAND_ALLOC    256
#define ARGV_SIZE       64
#define APPLET_SIZE     64

#define CPS_VERSION     "0.1.0"

// UI Constants
#define KBD_H 260
#define TAB_H 30
#define SCREEN_W width
#define SCREEN_H height

#define isBlank(ch)     (((ch) == ' ') || ((ch) == '\t'))
#define isDecimal(ch)   (((ch) >= '0') && ((ch) <= '9'))
#define isOctal(ch)     (((ch) >= '0') && ((ch) <= '7'))
#define isWildCard(ch)  (((ch) == '*') || ((ch) == '?') || ((ch) == '['))

// Files

#include <sdk/os/file.h>

struct dirEntry{
	char fileName[100];
	char type;
};
struct dirEntry directory[64];

char g_path[PATH_LEN] __attribute__((aligned(4)));
char16_t g_wpath[PATH_LEN] __attribute__((aligned(4)));
char g_home[PATH_LEN] = "\\fls0\\"; // default home for now

char16_t g_whistory[32] __attribute__((aligned(4))) = u"\\fls0\\usr\\.history";
char g_history[32] = "\\fls0\\usr\\.history";

char16_t g_wuserprofile[32] __attribute__((aligned(4))) = u"\\fls0\\usr\\.profile";
char g_userprofile[32] = "\\fls0\\usr\\.profile";

struct Applet {
	char name[APPLET_SIZE];
	int (*main)(int argc, char** argv);
};

static struct Applet applets[14]; // length is num funcs + 1 for empty

extern int cpshell_main(int argc, char** argv);

// Tracks the main Shell loop
bool shell_running = true;

#endif /* _INTERNAL_H_ */
