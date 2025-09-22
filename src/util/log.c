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

void errlogf(err32_t err_code, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(LOG_ERR, stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}

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

void werrlogf(err32_t err_code, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwlogf(LOG_ERR, stderr, fmt, arg_list);
	va_end(arg_list);
	if (err_code > -5280)
		exit(err_code);
}

#ifndef APLCORE__DISABLE_LOGGING
void flogf(LOG_TYPE type, FILE *stream, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(type, stream, fmt, arg_list);
	va_end(arg_list);
}

inline void ologf(LOG_TYPE type, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(type, stdout, fmt, arg_list);
	va_end(arg_list);
}

inline void elogf(LOG_TYPE type, const char *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vflogf(type, stderr, fmt, arg_list);
	va_end(arg_list);
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

void ewlogf(LOG_TYPE type, const wchar_t *fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
	vfwlogf(type, stderr, fmt, arg_list);
	va_end(arg_list);
}
#endif
