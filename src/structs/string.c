#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "structs/string.h"
#include "util/error.h"

Result(String) STR_create_empty(size_t length)
{
	String ret;
	ret.ptr = malloc(length+1);
	ret.ptr[length] = '\0';
	ret.capacity = length;
	ret.owner = NULL;
	ret.freed = false;

	return RES(ret,
			cur_err = ((STR_is_err(ret)) ? MALLOC_FAILURE : NIL),
			String);
}

Result(String) STR_create_zeroed(size_t length)
{
	String ret;
	ret.ptr = calloc(length+1, 1);
	ret.capacity = length;
	ret.owner = NULL;
	ret.freed = false;

	return RES(ret,
			cur_err = ((STR_is_err(ret)) ? MALLOC_FAILURE : NIL),
			String);
}

inline Result(String) STR_from_cstr(const char *cstr)
{
	if (cstr == NULL)
		return RES(ERR_STR, PTR_PARAM_IS_NULL, String);
	const size_t cstr_len = strlen(cstr);
	Result(String) res = STR_create_empty(cstr_len);
	if (iserr(res.err))
		return RES(ERR_STR, cur_err, String);
	if (iserr(STR_copy_cstr_at_front(&res.val, cstr)))
		return RES(ERR_STR, cur_err, String);

	return RES(res.val, cur_err = NIL, String);
}

inline size_t STR_len(const String str)
{
	return strlen(str.ptr);
}

Result(String) STR_clone(const String from)
{
	if (STR_is_err(from))
		return RES(ERR_STR, cur_err = STR_PARAM_IS_ERR, String);

	String dst;
	const size_t from_len = strlen(from.ptr);
	dst.ptr = malloc(from_len+1);
	if (dst.ptr == NULL)
		return RES(ERR_STR, cur_err = MALLOC_FAILURE, String);
	strcpy(dst.ptr, from.ptr);
	dst.capacity = from_len;
	dst.owner = NULL;
	dst.freed = false;

	return RES(dst, cur_err = NIL, String);
}

Result(String) STR_clone_N(const String from, size_t n)
{
	if (STR_is_err(from))
		return RES(ERR_STR, cur_err = STR_PARAM_IS_ERR, String);

	String dst;
	dst.ptr = malloc(n+1);
	if (dst.ptr == NULL)
		return RES(ERR_STR, cur_err = MALLOC_FAILURE, String);
	strncpy(dst.ptr, from.ptr, n+1);
	dst.capacity = n;
	dst.owner = NULL;
	dst.freed = false;

	return RES(dst, cur_err = NIL, String);
}

err32_t STR_resize(String *str_p, size_t capacity)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (str_p->owner != NULL)
		return cur_err = GENERIC_IS_OWNED_ERR;

	char *temp_realloc_ptr = realloc(str_p->ptr, capacity+1);
	if (temp_realloc_ptr == NULL)
		return cur_err = REALLOC_FAILURE;
	str_p->ptr = temp_realloc_ptr;
	if (capacity < str_p->capacity)
		str_p->ptr[capacity] = '\0';
	str_p->capacity = capacity;
	
	return cur_err = NIL;
}

inline err32_t STR_shrink(String *str_p)
{
	return STR_resize(str_p, STR_len(*str_p));
}

err32_t STR_copy_cstr_at_front(String *str_p, const char *in_p)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (in_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;

	const size_t in_p_len = strnlen(in_p, str_p->capacity+1);
	if (in_p_len == str_p->capacity+1)
		return cur_err = DATA_TOO_BIG_ERR;
	strncpy(str_p->ptr, in_p, in_p_len+1);

	return cur_err = NIL;
}

err32_t STR_copy_N_at_front(String *str_p, const char *in_p, size_t n)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (in_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;
	if (n > str_p->capacity)
		return cur_err = END_POS_OUT_OF_BOUNDS;

	strncpy(str_p->ptr, in_p, n);

	return cur_err = NIL;
}

err32_t STR_copy_cstr_to(String *str_p, size_t start, const char *in_p)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (start >= str_p->capacity)
		return cur_err = START_POS_OUT_OF_BOUNDS;
	const size_t in_p_len = strnlen(in_p, str_p->capacity+1 - start);
	if (in_p_len == (str_p->capacity+1 - start))
		return cur_err = DATA_TOO_BIG_ERR;
	if (in_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;

	strncpy(str_p->ptr + start, in_p, in_p_len+1);

	return cur_err = NIL;
}

err32_t STR_copy_N_to(String *str_p, size_t start, const char *in_p, size_t n)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (start >= str_p->capacity)
		return cur_err = START_POS_OUT_OF_BOUNDS;
	if (start + n > str_p->capacity)
		return cur_err = END_POS_OUT_OF_BOUNDS;
	if (in_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;

	strncpy(str_p->ptr + start, in_p, n);
	
	return cur_err = NIL;
}

err32_t STR_set_at(String *str_p, size_t index, char c)
{
	if (str_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*str_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (index >= str_p->capacity)
		return cur_err = GENERIC_INDEX_OUT_OF_BOUNDS;

	str_p->ptr[index] = c;
	
	return cur_err = NIL;
}

Result(char_p) STR_get_at(const String str, size_t index)
{
	if (STR_is_err(str))
		return RES(NULL, cur_err = STR_PARAM_IS_ERR, char_p);
	if (index >= str.capacity)
		return RES(NULL, cur_err = GENERIC_INDEX_OUT_OF_BOUNDS, char_p);
	
	return RES(str.ptr + index, cur_err = NIL, char_p);
}

err32_t STR_copy_N_from_front(const String str, char *out_p, size_t n)
{
	if (STR_is_err(str))
		return cur_err = STR_PARAM_IS_ERR;
	if (out_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;
	if (n > str.capacity)
		return cur_err = END_POS_OUT_OF_BOUNDS;

	if (strlcpy(out_p, str.ptr, n) >= n)
		return cur_err = TRUNCATED_OUTPUT;

	return cur_err = NIL;
}

err32_t STR_copy_N_from_pos(const String str, size_t start, char *out_p, size_t n)
{
	if (STR_is_err(str))
		return cur_err = STR_PARAM_IS_ERR;
	if (out_p == NULL)
		return cur_err = PTR_PARAM_IS_NULL;
	if (start >= str.capacity)
		return cur_err = START_POS_OUT_OF_BOUNDS;
	if (start + n > str.capacity)
		return cur_err = END_POS_OUT_OF_BOUNDS;

	if (strlcpy(out_p, str.ptr + start, n) >= n)
		return cur_err = TRUNCATED_OUTPUT;

	return cur_err = NIL;
}

err32_t STR_concat_cstr(String *lstr_p, const char *rcstr)
{
	if (lstr_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*lstr_p))
		return cur_err = STR_PARAM_IS_ERR;
	if (rcstr == NULL) 
		return cur_err = PTR_PARAM_IS_NULL;
	const size_t rcstr_len = strlen(rcstr);
	const size_t lstr_p_len = STR_len(*lstr_p);
	if (lstr_p_len + rcstr_len > lstr_p->capacity)
		STR_resize(lstr_p, lstr_p_len + rcstr_len);

	if (strlcat(lstr_p->ptr, rcstr, lstr_p->capacity+1)
			>= lstr_p->capacity+1)
		return cur_err = TRUNCATED_OUTPUT;

	return cur_err = NIL;
}

err32_t STR_concat_str(String *lstr_p, const String rstr)
{
	if (lstr_p == NULL)
		return cur_err = STR_P_PARAM_IS_NULL;
	if (STR_is_err(*lstr_p)
	 || STR_is_err(rstr))
		return cur_err = STR_PARAM_IS_ERR;

	const size_t rstr_len = STR_len(rstr);
	const size_t lstr_p_len = STR_len(*lstr_p);
	if (lstr_p_len + rstr_len > lstr_p->capacity)
		STR_resize(lstr_p, lstr_p_len + rstr_len);

	if (strlcat(lstr_p->ptr, rstr.ptr, lstr_p->capacity+1)
			>= lstr_p->capacity+1)
		return cur_err = TRUNCATED_OUTPUT;

	return cur_err = NIL;
}
