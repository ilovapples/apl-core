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
