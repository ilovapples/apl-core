#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "aplcore/util/error.h"

void eerror(err32_t err_code, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfprintf(stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}

#ifdef APLCORE__INCLUDE_WIDE_FUNCS
void ewerror(err32_t err_code, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwprintf(stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}
#endif

#if 0
#define IES "invalid error"
#define N4_IES IES, IES, IES, IES
#define N8_IES N4_IES, N4_IES
#define N16_INVALDERR N8_IES, N8_IES

char *get_err_string(ErrorCode code)
{
	static char *strings[] = {
		"not an error",

		"generic error",

		"generic - allocation failure",
		"malloc/calloc failure",
		"realloc failure",
		N8_IES, IES, IES, IES,

		"generic - parameter is an error",
		"Array parameter is an error",
		"String parameter is an error",
		"Arena parameter is an error",
		"pointer parameter is NULL",
		N4_IES, IES, IES, IES,

		"generic - index is out of bounds",
		"start position is out of bounds",
		"end position is out of bounds",
		N8_IES, N4_IES, IES
	};
	
	return strings[code];
}
#endif
