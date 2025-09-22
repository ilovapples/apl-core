#ifndef APLCORE__UTIL__MISC_H
#define APLCORE__UTIL__MISC_H


#define CARR_LEN(carr) (sizeof(carr) / sizeof(*carr))

#define lval_p(p, type) (*(type *)(p))


#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define RND_UP_TO_B_P2B(a, b) (((a)+(b)-1) & ~((b)-1))
#define RND_DOWN_TO_B(a, b) ((a) - ((a) % (b)))
#define RND_UP_TO_B(a, b) RND_DOWN_TO_B((a)+(b)-1, (b))


#if defined(__clang__) || defined(__GNUC__)
#define __defer_func(func) __attribute__((cleanup (func)))
#define __printf_fmt_attr(fmt, varg) __attribute__((format (printf, fmt, varg)))

void __scope_free(void *p);
#define __defer_free __defer_func(__scope_free)
#endif


#endif /* APLCORE__UTIL__MISC_H */
