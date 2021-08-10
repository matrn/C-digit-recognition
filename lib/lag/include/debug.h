#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <time.h>

//#define DEBUG
//#define TM_DEBUG


#ifdef DEBUG
	#define dbg(fmt, ...) printf(fmt, __VA_ARGS__)
	#define dbgln(str) puts(str)

	#define dbgerr(str) fprintf(stderr, "\x1b[31mERROR:\x1b[0m%d:%s: %s", __LINE__, __func__, str)
	#define dbgerrln(str) fprintf(stderr, "\x1b[31mERROR:\x1b[0m%d:%s: %s\n", __LINE__, __func__, str)

	#define dbgexec(func) func;
#else
	#define dbg(fmt, ...) //do {} while (false)
	#define dbgln(str) //do {} while (false)

	#define dbgerr(str)
	#define dbgerrln(str)

	#define dbgexec(func)
#endif



typedef clock_t dbg_tm;

dbg_tm get_dbg_tm();
void c_dbg_stop(dbg_tm begin, const int line_start, const int line_stop, const char * var_name, const char * fname);


#ifdef TM_DEBUG
	#define cstart(name) \
		const int dln_##name = __LINE__;\
		dbg_tm dtm_##name = get_dbg_tm() // cstart()      // ## means concatenation
	#define cstop(name) c_dbg_stop(dtm_##name, dln_##name, __LINE__ , #name, __func__)
#else
	#define cstart(name)
	#define cstop(name)
#endif

#endif