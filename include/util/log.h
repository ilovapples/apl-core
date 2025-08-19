#ifndef APLCORE__UTIL__LOG_H
#define APLCORE__UTIL__LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include "aplcore/types.h"

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
void vflogf(LOG_TYPE type, FILE *stream, const char *fmt, va_list arg_list);
/* Logs to STREAM with FMT and subsequent arguments via the
 * `printf` format, prefixed and colored depending on TYPE. */
void flogf(LOG_TYPE type, FILE *stream, const char *fmt, ...);
/* Logs to stdout with FMT and subsequent arguments via the
 * `printf` format, prefixed and colored depending on TYPE. */
void ologf(LOG_TYPE type, const char *fmt, ...);

#ifndef APLCORE__ERROR_H
/* Logs to stderr with the LOG_ERR log type, and terminates the program
 * with ERR_CODE if ERR_CODE > -5280.
 * Equivalent to running:
 * `flogf(LOG_ERR, stderr, fmt, ...);`
 * `exit(err_code);`
 * (unless, of course, ERR_CODE <= -5280)
 */
void errlogf(err32_t err_code, const char *fmt, ...);
/* Without any special colors or prefixes, writes to stderr with FMT and following arguments,
 * via `printf` format, then terminates with ERR_CODE if ERR_CODE > -5280. */
void eerror(err32_t err_code, const char *fmt, ...);
#endif


/* Prints compile time info such as date and time of compilation and compiler 
 * version */
void print_info(void);

#ifdef APLCORE__INCLUDE_WIDE_FUNCS
/* The variable-argument version  of `fwlogf`. See `fwlogf` for more details. */
void vfwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, va_list arg_list);
/* The wide-character printing version of `flogf`. See `flogf` for more details. */
void fwlogf(LOG_TYPE type, FILE *stream, const wchar_t *fmt, ...);
/* The wide-character printing version of `ologf`. See `ologf` for more details. */
void owlogf(LOG_TYPE, const wchar_t *fmt, ...);

#ifndef APLCORE__ERROR_H
/* The wide-character printing version of `errlogf`. See `errlogf` for more details. */
void werrlogf(err32_t err_code, const wchar_t *fmt, ...);
/* The wide-character printing version of `eerror`. See `eerror` for more details. */
void ewerror(err32_t err_code, const wchar_t *fmt, ...);
#endif

/* Prints compile time info such as date and time of compilation and compiler 
 * version */
void wprint_info(void);
#endif

#endif /* APLCORE__UTIL__LOG_H */

