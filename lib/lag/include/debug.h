#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
	#define dbg(str) printf(str)
	#define dbgln(str) puts(str)

	#define dbgerr(str) fprintf(stderr, "\x1b[31mERROR:\x1b[0m%d:%s: %s", __LINE__, __func__, str)
	#define dbgerrln(str) fprintf(stderr, "\x1b[31mERROR:\x1b[0m%d:%s: %s\n", __LINE__, __func__, str)
#else
	#define dbg(str) //do {} while (false)
	#define dbgln(str) //do {} while (false)

	#define dbgerr(str)
	#define dbgerrln(str)
#endif

#endif