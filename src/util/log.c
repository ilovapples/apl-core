#include <stdlib.h>
#include <wchar.h>

#include "aplcore/util/log.h"

void vflogf(LOG_TYPE type, FILE *stream, const char *fmt, va_list arg_list)
{
	switch (type) {
	case LOG_ERR:
		fprintf(stream, ERR_LOG);
		break;
	case LOG_WARN:
		fprintf(stream, WARN_LOG);
		break;
	case LOG_INFO:
		fprintf(stream, INFO_LOG);
		break;
	case LOG_DEBUG:
		/* arguments would be nice to decide whether to use LOG_DEBUG messages or not */
		fprintf(stream, DEBUG_LOG);
		break;
	}
	vfprintf(stream, fmt, arg_list);
	fprintf(stream, LOG_END);
}

void flogf(LOG_TYPE type, FILE *stream, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(type, stream, fmt, arg_list);
	va_end(arg_list);
}

void ologf(LOG_TYPE type, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(type, stdout, fmt, arg_list);
	va_end(arg_list);
}

void errlogf(err32_t err_code, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(LOG_ERR, stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}

void print_info(void)
{
	printf("Compiled on " __DATE__ "at " __TIME__ " with " 
#ifdef __GNUC__
	"GCC"
#elif defined(__clang__)
	"Clang"
#elif defined(__INTEL_COMPILER__)
	"an Intel compiler of"
#endif
			" version " __VERSION__ " C standard version %ld.\n",
			__STDC_VERSION__);
}


/* wide-char funcs */
void vfwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, va_list arg_list)
{
	switch (type) {
	case LOG_ERR:
		fwprintf(stream, TOWIDE(ERR_LOG));
		break;
	case LOG_WARN:
		fwprintf(stream, TOWIDE(WARN_LOG));
		break;
	case LOG_INFO:
		fwprintf(stream, TOWIDE(INFO_LOG));
		break;
	case LOG_DEBUG:
		/* arguments would be nice to decide whether to use LOG_DEBUG messages or not */
		fwprintf(stream, TOWIDE(DEBUG_LOG));
		break;
	}
	vfwprintf(stream, fmt, arg_list);
	fwprintf(stream, TOWIDE(LOG_END));
}

void fwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwlogf(type, stream, fmt, arg_list);
	va_end(arg_list);
}

void owlogf(LOG_TYPE type, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwlogf(type, stdout, fmt, arg_list);
	va_end(arg_list);
}

void werrlogf(err32_t err_code, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwlogf(LOG_ERR, stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}

void wprint_info(void)
{
	wprintf(L"Compiled on " TOWIDE(__DATE__) L"at " TOWIDE(__TIME__) L" with " 
#ifdef __GNUC__
	L"GCC"
#elif defined(__clang__)
	L"Clang"
#elif defined(__INTEL_COMPILER__)
	L"an Intel compiler of"
#endif
			L" version " TOWIDE(__VERSION__) L" C standard version %ld.\n",
			__STDC_VERSION__);
}
