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

#define PATH_LEN        256
#define BUF_SIZE        512
#define EXPAND_ALLOC    256
#define ARGV_SIZE       64

#define isBlank(ch)     (((ch) == ' ') || ((ch) == '\t'))
#define isDecimal(ch)   (((ch) >= '0') && ((ch) <= '9'))
#define isOctal(ch)     (((ch) >= '0') && ((ch) <= '7'))
#define isWildCard(ch)  (((ch) == '*') || ((ch) == '?') || ((ch) == '['))

struct Applet {
	const	char*	name;
	int	(*main)(int argc, char** argv);
};

extern int cpshell_main(int argc, char** argv);

#endif /* _INTERNAL_H_ */
