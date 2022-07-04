/*
 * Ported from BusyBox main internal header file on 0_32 branch
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

#define isBlank(ch)     (((ch) == ' ') || ((ch) == '\t'))
#define isDecimal(ch)   (((ch) >= '0') && ((ch) <= '9'))
#define isOctal(ch)     (((ch) >= '0') && ((ch) <= '7'))
#define isWildCard(ch)  (((ch) == '*') || ((ch) == '?') || ((ch) == '['))

// Files

struct dirEntry{
	char fileName[100];
	char type;
};
struct dirEntry directory[64];

char g_path[PATH_LEN];
wchar_t g_wpath[PATH_LEN];
char g_home[PATH_LEN] = "\\fls0\\"; // default home for now

wchar_t g_whistory[32] = L"\\fls0\\usr\\.history";
char g_history[32] = "\\fls0\\usr\\.history";

wchar_t g_wuserprofile[32] = L"\\fls0\\usr\\.profile";
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
