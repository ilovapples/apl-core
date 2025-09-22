#ifndef APLCORE__WIDE_COMPAT_H
#define APLCORE__WIDE_COMPAT_H

#ifdef APLCORE__INCLUDE_WIDE_FUNCS
#include <wchar.h>
#include <locale.h>

#define __L(s) L##s
#define L(s) __L(s)
#else
#define L(s) s
#endif

#ifndef APLCORE__INCLUDE_WIDE_FUNCS
	typedef char wc_char, *wc_char_p;
	#define wc_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
	#define wc_fprintf(stream, fmt, ...) fprintf(stream, fmt, ##__VA_ARGS__)
	#define wc_puts(s) puts(s)
	#define wc_fputs(s, stream) fputs(s, stream)
	#define wc_memset(buf, c, n) memset(buf, c, n)

	#define wc_ologf(log_type, fmt, ...) ologf(log_type, fmt, ##__VA_ARGS__)
	#define wc_elogf(log_type, fmt, ...) elogf(log_type, fmt, ##__VA_ARGS__)
	#define wc_flogf(log_type, stream, fmt, ...) flogf(log_type, stream, fmt, ##__VA_ARGS__)

	#define wc_errlogf(err_code, fmt, ...) errlogf(err_code, fmt, ##__VA_ARGS__)
	#define wc_eerror(err_code, fmt, ...) eerror(err_code, fmt, ##__VA_ARGS__)

	#define wc_ARENA_debug_block_str(block_p, arena_p, is_print_spec_addr) ARENA_debug_block_str(block_p, arena_p, is_print_spec_addr)
#else
	typedef wchar_t wc_char, *wc_char_p;
	#define wc_printf(fmt, ...) wprintf(fmt, ##__VA_ARGS__)
	#define wc_fprintf(stream, fmt, ...) wprintf(stream, fmt, ##__VA_ARGS__)
	#define wc_puts(s) fputws(s, stdout)
	#define wc_fputs(s, stream) fputws(s, stream)
	#define wc_memset(buf, c, n) wmemset(buf, c, n)

	#define wc_elogf(log_type, fmt, ...) ewlogf(log_type, fmt, ##__VA_ARGS__)
	#define wc_ologf(log_type, fmt, ...) owlogf(log_type, fmt, ##__VA_ARGS__)
	#define wc_flogf(log_type, stream, fmt, ...) fwlogf(log_type, stream, fmt, ##__VA_ARGS__)

	#define wc_errlogf(err_code, fmt, ...) werrlogf(err_code, fmt, ##__VA_ARGS__)
	#define wc_eerror(err_code, fmt, ...) ewerror(err_code, fmt, ##__VA_ARGS__)

	#define wc_ARENA_debug_block_str(block_p, arena_p, is_print_spec_addr) ARENA_debug_block_wcs(block_p, arena_p, is_print_spec_addr)
#endif

#ifdef APLCORE__ARENA_H
#ifndef APLCORE__INCLUDE_WIDE_FUNCS
#else
	#define wc_ARENA_debug_block_str(block_p, arena_p, is_print_spec_addr) ARENA_debug_block_wcs(block_p, arena_p, is_print_spec_addr)
#endif
#endif

#endif /* APLCORE__WIDE_COMPAT_H */
