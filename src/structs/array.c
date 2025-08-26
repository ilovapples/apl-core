#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "structs/array.h"
#include "util/error.h"
#include "util/misc.h"

Result(Array) ARR_create_empty(size_t nmemb, size_t memb_size)
{
	if (nmemb == 0)
		return RES(ERR_ARR, ALLOC_NMEMB_PARAM_IS_ZERO, Array);
	if (memb_size == 0)
		return RES(ERR_ARR, ALLOC_MEMB_SIZE_PARAM_IS_ZERO, Array);

	Array ret;
	ret.ptr = malloc(nmemb * memb_size);
	ret.capacity_b = (ret.length = nmemb) * memb_size;
	ret.memb_size = memb_size;
	ret.owner = NULL;
	ret.freed = false;
	return RES(ret, ARR_is_err(ret), Array);
}

Result(Array) ARR_create_zeroed(size_t nmemb, size_t memb_size)
{
	if (nmemb == 0)
		return RES(ERR_ARR, ALLOC_NMEMB_PARAM_IS_ZERO, Array);
	if (memb_size == 0)
		return RES(ERR_ARR, ALLOC_MEMB_SIZE_PARAM_IS_ZERO, Array);

	Array ret;
	ret.ptr = calloc(nmemb, memb_size);
	ret.capacity_b = (ret.length = nmemb) * memb_size;
	ret.memb_size = memb_size;
	ret.owner = NULL;
	ret.freed = false;
	return RES(ret, ARR_is_err(ret), Array);
}

Result(Array) ARR_clone(Array from)
{
	Array dst = from;
	dst.ptr = (ARR_is_err(from)) ? NULL : malloc(from.length * from.memb_size);
	if (dst.ptr != NULL)
		memcpy(dst.ptr, from.ptr, from.length * from.memb_size);
	dst.owner = NULL;
	return RES(dst, MALLOC_FAILURE * ARR_is_err(dst), Array);
}

err32_t ARR_resize(Array *arrp, size_t memb_capacity)
{
	if (arrp->owner != NULL)
		return GENERIC_IS_OWNED_ERR;
	if (memb_capacity == 0)
		return ALLOC_SIZE_PARAM_IS_ZERO;

	void *temp_realloc_ptr = realloc(arrp->ptr, memb_capacity * arrp->memb_size);
	if (temp_realloc_ptr == NULL)
		return REALLOC_FAILURE;

	arrp->ptr = temp_realloc_ptr;
	arrp->capacity_b = memb_capacity * arrp->memb_size;
	arrp->length = MIN(arrp->length, memb_capacity);

	return NIL;
}

err32_t ARR_resize_B(Array *arrp, size_t capacity_b)
{
	if (arrp->owner != NULL)
		return GENERIC_IS_OWNED_ERR;
	if (capacity_b == 0)
		return ALLOC_SIZE_PARAM_IS_ZERO;

	void *temp_realloc_ptr = realloc(arrp->ptr, capacity_b); 
	if (temp_realloc_ptr == NULL)
		return REALLOC_FAILURE;

	arrp->ptr = temp_realloc_ptr;
	arrp->capacity_b = capacity_b;
	arrp->length = MIN(arrp->length, capacity_b / arrp->memb_size);

	return 0;
}

inline err32_t ARR_shrink(Array *arrp)
{
	return ARR_resize(arrp, arrp->length * arrp->memb_size);
}

err32_t ARR_copy_at_front(Array *arrp, const void *p, size_t n)
{
	if (n > (arrp->capacity_b / arrp->memb_size))
		return END_POS_OUT_OF_BOUNDS;
	if (p == NULL)
		return PTR_PARAM_IS_NULL;
	memcpy(arrp->ptr, p, n * arrp->memb_size);
	arrp->length = MAX(arrp->length, n);
	
	return 0;
}

err32_t ARR_copy_at_front_B(Array *arrp, const void *p, size_t n_b)
{
	if (n_b > arrp->capacity_b)
		return END_POS_OUT_OF_BOUNDS;
	if (p == NULL)
		return PTR_PARAM_IS_NULL;
	memcpy(arrp->ptr, p, n_b);
	const size_t MEMB_SIZE = arrp->memb_size;
	arrp->length = MAX(arrp->length, RND_UP_TO_B(n_b / MEMB_SIZE, MEMB_SIZE));
	
	return 0;
}

err32_t ARR_copy_range_to(Array *arrp, size_t start, const void *in_p, size_t n)
{
	if (ARR_is_err(*arrp))
		return ARR_PARAM_IS_ERR;
	if (in_p == NULL)
		return PTR_PARAM_IS_NULL;
	if (start > arrp->length)
		return START_POS_OUT_OF_BOUNDS;
	if (start + n > (arrp->capacity_b / arrp->memb_size))
		return END_POS_OUT_OF_BOUNDS;
	memcpy((u8 *)arrp->ptr + (start * arrp->memb_size), in_p, n * arrp->memb_size);
	arrp->length = MAX(arrp->length, start + n);

	return 0;
}

err32_t ARR_copy_range_to_B(Array *arr, size_t start_b, const void *in_p, size_t n_b)
{
	if (ARR_is_err(*arr))
		return ARR_PARAM_IS_ERR;
	if (in_p == NULL)
		return PTR_PARAM_IS_NULL;
	if (start_b > arr->length * arr->memb_size)
		return START_POS_OUT_OF_BOUNDS;
	if (start_b + n_b > arr->capacity_b)
		return END_POS_OUT_OF_BOUNDS;
	memcpy((u8 *)arr->ptr + start_b, in_p, n_b);
	arr->length = MAX(arr->length, (start_b + n_b) / arr->memb_size);

	return NIL;
}

err32_t ARR_copy_range_from(const Array arr, size_t start, void *out_p, size_t n)
{
	if (ARR_is_err(arr))
		return ARR_PARAM_IS_ERR;
	if (out_p == NULL)
		return PTR_PARAM_IS_NULL;
	if (start >= arr.length)
		return START_POS_OUT_OF_BOUNDS;
	if (start + n > arr.length)
		return END_POS_OUT_OF_BOUNDS;
	memcpy(out_p, (u8 *)arr.ptr + (start * arr.memb_size), n * arr.memb_size);

	return NIL;
}

err32_t ARR_copy_range_from_B(const Array arr, size_t start_b, void *out_p, size_t n_b)
{
	if (ARR_is_err(arr))
		return ARR_PARAM_IS_ERR;
	const size_t arr_size_b = arr.length * arr.memb_size;
	if (start_b >= arr_size_b)
		return START_POS_OUT_OF_BOUNDS;
	if (start_b + n_b > arr_size_b)
		return END_POS_OUT_OF_BOUNDS;
	memcpy(out_p, (u8 *)arr.ptr + start_b, n_b);

	return NIL;
}

Result(void_p) ARR_get_at(const Array arr, size_t index)
{
	err32_t err = NIL;
	if (ARR_is_err(arr))
		err = ARR_PARAM_IS_ERR;
	if (index >= arr.length)
		err = GENERIC_INDEX_OUT_OF_BOUNDS;
	return RES((!err) ? (u8 *)arr.ptr + index*arr.memb_size : NULL, err, void_p);
}

err32_t ARR_copy_one_from(const Array arr, size_t index, void *out)
{
	if (ARR_is_err(arr))
		return ARR_PARAM_IS_ERR;
	if (out == NULL)
		return PTR_PARAM_IS_NULL;
	if (index >= arr.length)
		return GENERIC_INDEX_OUT_OF_BOUNDS;
	memcpy(out, (u8 *)arr.ptr + index*arr.memb_size, arr.memb_size);

	return NIL;
}

err32_t ARR_copy_one_to(Array *arrp, size_t index, void *in)
{
	if (ARR_is_err(*arrp))
		return ARR_PARAM_IS_ERR;
	if (in == NULL)
		return PTR_PARAM_IS_NULL;
	if (index >= arrp->length)
		return GENERIC_INDEX_OUT_OF_BOUNDS;
	memcpy((u8 *)arrp->ptr + index*arrp->memb_size, in, arrp->memb_size);
	
	return NIL;
}

Result(void_p) ARR_push_back(Array *arrp, void *in)
{
	if (ARR_is_err(*arrp))
		return RES(NULL, ARR_PARAM_IS_ERR, void_p);
	if ((arrp->capacity_b - (arrp->length*arrp->memb_size)) < arrp->memb_size)
		ARR_resize(arrp, arrp->length+1);
	
	void *copied_to_pos = (u8 *)arrp->ptr + arrp->length*arrp->memb_size;
	memcpy(copied_to_pos, in, arrp->memb_size);
	arrp->length++;

	return RES(copied_to_pos, NIL, void_p);
}

Result(void_p) ARR_pop_back(Array *arrp)
{
	if (ARR_is_err(*arrp))
		return RES(NULL, ARR_PARAM_IS_ERR, void_p);
	if (arrp->length == 0)
		return RES(NULL, DATA_TOO_SMALL_ERR, void_p);
	
	return ARR_get_at(*arrp, --arrp->length);
}

Result(size_t) ARR_search(Array arr, void *target, comp_func cmp)
{
	for (size_t i = 0; i < arr.length; ++i)
	{
		if (cmp(ARR_get_at(arr, i).val, target) == 0)
			return RES(i, 0, size_t);
	}

	return RES(-1, GENERIC_NONEXISTENT_ITEM, size_t);
}

Result(size_t) ARR_search_lesser(Array arr, void *target, comp_func cmp)
{
	for (size_t i = 0; i < arr.length; ++i)
	{
		if (cmp(ARR_get_at(arr, i).val, target) < 0)
			return RES(i, 0, size_t);
	}

	return RES(-1, GENERIC_NONEXISTENT_ITEM, size_t);
}

Result(size_t) ARR_search_greater(Array arr, void *target, comp_func cmp)
{
	for (size_t i = 0; i < arr.length; ++i)
	{
		if (cmp(ARR_get_at(arr, i).val, target) > 0)
			return RES(i, 0, size_t);
	}

	return RES(-1, GENERIC_NONEXISTENT_ITEM, size_t);
}

void ARR_accumulate(Array arr, accum_func accum, void *out)
{
	for (size_t i = 0; i < arr.length; ++i)
		accum(out, ARR_get_at(arr, i).val);
}
