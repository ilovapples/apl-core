#ifndef APLCORE__ERROR_H
#define APLCORE__ERROR_H

#include <stddef.h>
#include <sys/types.h>

#include "aplcore/types.h"

#define Resultv1(type) Result_##type
#define Result(type) Resultv1(type)
#define RES(res, err, res_type) ((Result(res_type)) { res, err })

#define MKSTRCT_Result(type) \
typedef struct {             \
	type res;              \
	err32_t err;           \
} Result(type)

MKSTRCT_Result(s32);
MKSTRCT_Result(s64);
MKSTRCT_Result(u32);
MKSTRCT_Result(u64);
MKSTRCT_Result(size_t);
MKSTRCT_Result(ssize_t);

#ifndef APLCORE__UTIL__LOG_H
/* RAISE ERROR FUNCTIONS */

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
#ifdef APLCORE__INCLUDE_WIDE_FUNCS
#include <wchar.h>
/* The wide-character printing version of `errlogf`. See `errlogf` for more details. */
void werrlogf(err32_t err_code, const wchar_t *fmt, ...);
/* The wide-character printing version of `eerror`. See `eerror` for more details. */
void ewerror(err32_t err_code, const wchar_t *fmt, ...);
#endif
#endif

#endif /* APLCORE__ERROR_H */
