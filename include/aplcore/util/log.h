#ifndef APLCORE__UTIL__LOG_H
#define APLCORE__UTIL__LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include "aplcore/types.h"
#include "aplcore/util/misc.h"

/* lots of macros here */
#define __tostr(x) #x
#define STRINGIZE(x) __tostr(x)
#define __towide(str) L##str
#define TOWIDE(str) __towide(str)
#define SET_FG_ESC "\x1b[38;5;"
#define ERR_COLOR 9
#define WARN_COLOR 214
#define INFO_COLOR 81
#define DEBUG_COLOR 255
#define ERR_ENT "[ERROR] "
#define WARN_ENT "[WARN] "
#define INFO_ENT "[INFO] "
#define DEBUG_ENT "[DEBUG] "
#define ERR_LOG SET_FG_ESC STRINGIZE(ERR_COLOR) ";1m" ERR_ENT "\x1b[0m" SET_FG_ESC STRINGIZE(ERR_COLOR) "m"
#define WARN_LOG SET_FG_ESC STRINGIZE(WARN_COLOR) ";1m" WARN_ENT "\x1b[0m" SET_FG_ESC STRINGIZE(WARN_COLOR) "m"
#define INFO_LOG SET_FG_ESC STRINGIZE(INFO_COLOR) ";1m" INFO_ENT "\x1b[0m" SET_FG_ESC STRINGIZE(INFO_COLOR) "m"
#define DEBUG_LOG SET_FG_ESC STRINGIZE(DEBUG_COLOR) ";1m" DEBUG_ENT "\x1b[0m" SET_FG_ESC STRINGIZE(DEBUG_COLOR) "m"
#define LOG_END "\x1b[0m"

typedef enum {
	LOG_ERR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
} LOG_TYPE;

/* The variable-argument version of `flogf`. See `flogf` for more details. */
void vflogf(LOG_TYPE type, FILE *stream, const char *fmt, va_list arg_list) __printf_fmt_attr(3, 0);

#ifndef APLCORE__UTIL__ERROR_H
	/* Logs to stderr with the LOG_ERR log type, and terminates the program
	 * with ERR_CODE if ERR_CODE > -5280.
	 * Equivalent to running:
	 * `flogf(LOG_ERR, stderr, fmt, ...);`
	 * `exit(err_code);`
	 * (unless, of course, ERR_CODE <= -5280)
	 */
	void errlogf(err32_t err_code, const char *fmt, ...) __printf_fmt_attr(2, 3);
	/* Without any special colors or prefixes, writes to stderr with FMT and following arguments,
	 * via `printf` format, then terminates with ERR_CODE if ERR_CODE > -5280. */
	void eerror(err32_t err_code, const char *fmt, ...) __printf_fmt_attr(2, 3);
#endif


#ifdef APLCORE__INCLUDE_WIDE_FUNCS
/* The variable-argument version  of `fwlogf`. See `fwlogf` for more details. */
void vfwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, va_list arg_list);

#ifndef APLCORE__UTIL__ERROR_H
	/* The wide-character printing version of `errlogf`. See `errlogf` for more details. */
	void werrlogf(err32_t err_code, const wchar_t *fmt, ...);
	/* The wide-character printing version of `eerror`. See `eerror` for more details. */
	void ewerror(err32_t err_code, const wchar_t *fmt, ...);
#endif

#endif


#ifndef APLCORE__DISABLE_LOGGING
	/* Logs to STREAM with FMT and subsequent arguments via the
	 * `printf` format, prefixed and colored depending on TYPE. */
	void flogf(LOG_TYPE type, FILE *stream, const char *fmt, ...) __printf_fmt_attr(3, 4);
	/* Logs to stdout with FMT and subsequent arguments via the
	 * `printf` format, prefixed and colored depending on TYPE. */
	void ologf(LOG_TYPE type, const char *fmt, ...) __printf_fmt_attr(2, 3);
	/* Logs to stderr with FMT and subsequent arguments via the
	 * `printf` format, prefixed and colored depending on TYPE. */
	void elogf(LOG_TYPE type, const char *fmt, ...) __printf_fmt_attr(2, 3);

	#ifdef APLCORE__INCLUDE_WIDE_FUNCS
		/* The wide-character printing version of `flogf`. See `flogf` for more details. */
		void fwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, ...);
		/* The wide-character printing version of `ologf`. See `ologf` for more details. */
		void owlogf(LOG_TYPE, const wchar_t *fmt, ...);
		/* The wide-character printing version of `elogf`. See `elogf` for more details. */
		void ewlogf(LOG_TYPE, const wchar_t *fmt, ...);
	#endif

#else
	#define flogf(a,...)
	#define ologf(a,...)
	#define elogf(a,...)

	#ifdef APLCORE__INCLUDE_WIDE_FUNCS
		#define fwlogf(a,...)
		#define owlogf(a,...)
		#define ewlogf(a,...)
	#endif
#endif

#ifdef __GNUC__
	#define APLCORE__UTIL__COMPILER_NAME "GCC"
#elif defined(__clang__)
	#define APLCORE__UTIL__COMPILER_NAME "Clang"
#elif defined(__INTEL_COMPILER__)
	#define APLCORE__UTIL__COMPILER_NAME "an Intel compiler of"
#endif

#define MKFN_print_info(append) \
void print_info##append(void) \
{ \
	printf("Compiled on " __DATE__ "at " __TIME__ " with " \
			APLCORE__UTIL__COMPILER_NAME \
			" version " __VERSION__ " C standard version %ld.\n", \
			__STDC_VERSION__); \
}
#define MKPRTP_print_info(append) \
void print_info##append(void)

#define MKFN_wprint_info(append) \
void wprint_info##append(void) \
{ \
	wprintf(L"Compiled on " __DATE__ "at " __TIME__ " with " \
			APLCORE__UTIL__COMPILER_NAME \
			" version " __VERSION__ " C standard version %ld.\n", \
			__STDC_VERSION__); \
}
#define MKPRTP_wprint_info(append) \
void wprint_info##append(void)


#endif /* APLCORE__UTIL__LOG_H */

