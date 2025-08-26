#ifndef APLCORE__STRING_H
#define APLCORE__STRING_H

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "types.h"
#include "util/error.h"

typedef struct Arena Arena;

/* capacity and any n arguments don't include the null terminator. */
/* unless specified otherwise, the null terminator is not factored into
 * any of the arguments, so `STR_create_zeroed(10)` allocates 11 bytes 
 * for the string buffer, not 10. */
typedef struct String {
	char *ptr;
	size_t capacity;
	Arena *owner;
	bool freed;
} String;

#define STR_is_err(str) ((str).ptr == NULL)
#define ERR_STR ((String) { .ptr = NULL })

MKSTRCT_Result(String);

/* allocates an empty String (doesn't initialize any data) with
 * size LENGTH+1 (an extra is needed for null terminator)
 */
Result(String) STR_create_empty(size_t length);
Result(String) STR_create_zeroed(size_t length);

err32_t STR_free(String *str_p);
size_t STR_len(const String str);

Result(String) STR_clone(const String from);
/* like STR_clone, but copies N characters from FROM. Uses strncpy, so if
 * strlen(FROM) < N then any leftover space is zeroed.
 */
Result(String) STR_clone_N(const String from, size_t n);

/* resize a string's buffer to CAPACITY+1 bytes (capacity doesn't include 
 * null terminator), and set a null terminator at position CAPACITY if 
 * the string has been shrunk */
err32_t STR_resize(String *str_p, size_t capacity);
err32_t STR_shrink(String *str_p);

/* copy the bytes at IN_P into STR_P. Doesn't copy IN_P's null terminator.
 */
err32_t STR_copy_cstr_at_front(String *str_p, const char *in_p);
/* like `STR_copy_cstr_at_front` but copies N characters using strncpy, so if
 * IN_P is shorter than N characters, it'll zero out any extras in STR_P.
 * If N is greater than strlen(IN_P), the null terminator of IN_P will be
 * copied as well.
 */
err32_t STR_copy_N_at_front(String *str_p, const char *in_p, size_t n);

/* copy the bytes at IN_P into STR_P, offset START bytes. Doesn't copy IN_P's
 * null terminator. */
err32_t STR_copy_cstr_to(String *str_p, size_t start, const char *in_p);
/* like `STR_copy_str_to` but copies N characters using strncpy, so if 
 * IN_P is shorter than N characters, it'll zero out any extras in STR_P. If 
 * N is greater than strlen(IN_P), the null terminator of IN_P will be copied
 * as well. */
err32_t STR_copy_N_to(String *str_p, size_t start, const char *in_p, size_t n);

err32_t STR_set_at(String *str_p, size_t index, char c);
Result(char_p) STR_get_at(const String str, size_t index);

/* equivalent to a call to `STR_copy_N_from_pos` with 0 passed to the START
 * argument */
err32_t STR_copy_N_from_front(const String str, char *out_p, size_t n);
/* copy N bytes in STR into OUT_P, starting at an offset of START bytes */
err32_t STR_copy_N_from_pos(const String str, size_t start, char *out_p, size_t n);

/* concat a cstr (aka char *) to a String */
err32_t STR_concat_cstr(String *lstr_p, const char *rcstr);
/* concat a str (aka String) to a String */
err32_t STR_concat_str(String *lstr_p, const String rstr);

#endif /* APLCORE__STRING_H */
