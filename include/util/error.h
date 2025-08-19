#ifndef APLCORE__UTIL__ERROR_H
#define APLCORE__UTIL__ERROR_H

#include <stddef.h>
#include <sys/types.h>

#include "aplcore/types.h"

/* RESULT TYPE */
#define Resultv1(type) Result_##type
#define Result(type) Resultv1(type)
#define RES(val, err, val_type) ((Result(val_type)) { (val), (err) })

#define MKSTRCT_Result(type) \
typedef struct {             \
	type val;              \
	err32_t err;           \
} Result(type)

MKSTRCT_Result(s32);
MKSTRCT_Result(s64);
MKSTRCT_Result(u32);
MKSTRCT_Result(u64);
MKSTRCT_Result(size_t);
MKSTRCT_Result(ssize_t);

/* ERROR ENUM(S) */
#define NUM_ERR_CODES 256
typedef enum {
	NIL = 0x00,

	GENERIC_ERROR					= 0x01,

	GENERIC_ALLOC_FAILURE				= 0x02,
	MALLOC_FAILURE					= 0x03, /* covers calloc as well */
	REALLOC_FAILURE					= 0x04,

	GENERIC_PARAM_IS_ERR				= 0x10,
	ARR_PARAM_IS_ERR					= 0x11,
	STR_PARAM_IS_ERR					= 0x12,
	ARENA_PARAM_IS_ERR				= 0x13,
	PTR_PARAM_IS_NULL					= 0x18,

	GENERIC_INDEX_OUT_OF_BOUNDS			= 0x20,
	START_POS_OUT_OF_BOUNDS				= 0x21,
	END_POS_OUT_OF_BOUNDS				= 0x22,

	GENERIC_IS_OWNED_ERR				= 0x30,
	ARR_IS_OWNED_ERR					= 0x31,
	STR_IS_OWNED_ERR					= 0x32,

	GENERIC_ALLOC_SIZE_PARAM_IS_ZERO		= 0x40,
	ARR_ALLOC_NMEMB_PARAM_IS_ZERO			= 0x41,
	ARR_ALLOC_MEMB_SIZE_PARAM_IS_ZERO		= 0x42,
	ARR_ALLOC_CAPACITY_PARAM_IS_ZERO		= 0x43,

	GENERIC_BAD_DATA_SIZE				= 0x50,
	DATA_TOO_SMALL_ERR				= 0x51,
	DATA_TOO_BIG_ERR					= 0x52,

	GENERIC_NONEXISTENT_ITEM			= 0x60,
	TARGET_NOT_IN_ARR					= 0x61,

	GENERIC_DOUBLE_FREE_ERR				= 0x70,
	ARR_ALREADY_FREED					= 0x71,
} ErrorCode;


//char *get_err_string(ErrorCode code);


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
