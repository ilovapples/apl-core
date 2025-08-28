#ifndef APLCORE__UTIL__ERROR_H
#define APLCORE__UTIL__ERROR_H

#include <stddef.h>
#include <sys/types.h>

#include "types.h"
#include "util/misc.h"

/* RESULT TYPE */
#define Resultv1(type) Result_##type
#define Result(type) Resultv1(type)
#define RES(val, err, val_type) ((Result(val_type)) { (val), (err) })

#define MKSTRCT_Result(type) \
typedef struct {             \
	type val;              \
	err32_t err;           \
} Result(type)

MKSTRCT_Result(s8);
MKSTRCT_Result(s16);
MKSTRCT_Result(s32);
MKSTRCT_Result(s64);
MKSTRCT_Result(u8);
MKSTRCT_Result(u16);
MKSTRCT_Result(u32);
MKSTRCT_Result(u64);
MKSTRCT_Result(size_t);
MKSTRCT_Result(ssize_t);
MKSTRCT_Result(char_p);
MKSTRCT_Result(wchar_p);
MKSTRCT_Result(void_p);

/* ERROR ENUM(S) */
#define NUM_ERR_CODES 256
typedef enum {
	/* first few are non-error values */
	NIL							= 0x00, 
	TRUNCATED_OUTPUT					= 0x01,

	GENERIC_ERROR					= 0x10,

	GENERIC_ALLOC_FAILURE				= 0x32,
	MALLOC_FAILURE					= 0x33, /* covers calloc as well */
	REALLOC_FAILURE					= 0x34,

	GENERIC_PARAM_IS_ERR				= 0x50,
	ARR_PARAM_IS_ERR					= 0x51,
	ARR_P_PARAM_IS_NULL                       = 0x52,
	STR_PARAM_IS_ERR					= 0x53,
      STR_P_PARAM_IS_NULL                       = 0x54,
	ARENA_PARAM_IS_ERR				= 0x55,
      ARENA_P_PARAM_IS_NULL                     = 0x56,
	PTR_PARAM_IS_NULL					= 0x57,

	GENERIC_INDEX_OUT_OF_BOUNDS			= 0x60,
	START_POS_OUT_OF_BOUNDS				= 0x61,
	END_POS_OUT_OF_BOUNDS				= 0x62,

	GENERIC_IS_OWNED_ERR				= 0x70,

	GENERIC_ALLOC_SIZE_PARAM_IS_ERR		= 0x80,
	ALLOC_NMEMB_PARAM_IS_ZERO			= 0x81,
	ALLOC_MEMB_SIZE_PARAM_IS_ZERO			= 0x82,
	ALLOC_SIZE_PARAM_IS_ZERO			= 0x83,
	ALLOC_SIZE_PARAM_IS_TOO_BIG			= 0x84,

	GENERIC_BAD_DATA_SIZE				= 0x90,
	DATA_TOO_SMALL_ERR				= 0x91,
	DATA_TOO_BIG_ERR					= 0x92,

	GENERIC_NONEXISTENT_ITEM			= 0xA0,

	GENERIC_DOUBLE_FREE_ERR				= 0xB0,
} ErrorCode;

#define iserr(e) ((e) >= GENERIC_ERROR)

extern err64_t cur_err;

// the below has nothing to do with this library
//static const volatile unsigned long long *const volatile *const volatile a;
 /* `a` is a static, constant, volatile pointer to a constant, volatile pointer to a constant, volatile unsigned 64-bit integer
 */


//char *get_err_string(ErrorCode code);


/* RAISE ERROR FUNCTIONS */

/* Logs to stderr with the LOG_ERR log type, and terminates the program
 * with ERR_CODE if ERR_CODE > -5280.
 * Equivalent to running:
 * `flogf(LOG_ERR, stderr, fmt, ...);`
 * `exit(err_code);`
 * (unless, of course, ERR_CODE <= -5280)
 */
void errlogf(err32_t err_code, const char *fmt, ...) __printf_fmt_attr(2,3);
/* Without any special colors or prefixes, writes to stderr with FMT and following arguments,
 * via `printf` format, then terminates with ERR_CODE if ERR_CODE > -5280. */
void eerror(err32_t err_code, const char *fmt, ...) __printf_fmt_attr(2, 3);

#ifdef APLCORE__INCLUDE_WIDE_FUNCS
	#include <wchar.h>
	/* The wide-character printing version of `eerror`. See `eerror` for more details. */
	void ewerror(err32_t err_code, const wchar_t *fmt, ...);
	/* The wide-character printing version of `errlogf`. See `errlogf` for more details. */
	void werrlogf(err32_t err_code, const wchar_t *fmt, ...);
#endif

#endif /* APLCORE__ERROR_H */
