#include "include/debug.h"

dbg_tm get_dbg_tm() {
	return clock();
}

void c_dbg_stop(dbg_tm begin, const int line_start, const int line_stop, const char* var_name, const char* fname) {
	dbg_tm end = get_dbg_tm();
	printf("%d-%d: %s():%s: Time difference = %f [µs]\n", line_start + 1, line_stop - 1, fname, var_name, (double)(end - begin));
}