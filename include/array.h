#ifndef APLCORE__ARRAY_H
#define APLCORE__ARRAY_H

#include <stdbool.h>
#include <stddef.h>

#include "aplcore/types.h"
#include "aplcore/util/error.h"
#include "aplcore/polymorph.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

typedef struct Arena Arena;

typedef struct Array {
	void *ptr;
	size_t length;
	size_t memb_size;
	size_t capacity_b; // measured in bytes
	Arena *owner;
	bool freed;
} Array;

#define __array_of_T(type) Array_##type

#define ArrayT(type) __array_of_T(type)
typedef char *char_p;
typedef Array ArrayT(s8), ArrayT(s16), ArrayT(s32), ArrayT(s64),
              ArrayT(u8), ArrayT(u16), ArrayT(u32), ArrayT(u64),
		  ArrayT(char), ArrayT(char_p),
		  ArrayT(size_t), ArrayT(Slice);

#define ARR_is_err(arr) ((arr).ptr == NULL)
#define ERR_ARR ((Array) { .ptr = NULL })

MKSTRCT_Result(Array);

typedef void *voidp;
#define ptr_lval(p, type) (*(type *)(p))
MKSTRCT_Result(voidp);

#define ARR_for_each(arr, cur, type) \
	for (size_t i = 0; i < (arr).length \
		 || (0 && (cur = ptr_lval(ARR_get_at((arr), i).val, type))); ++i)
#define ARR_for_each_p(arr, cur) \
	for (size_t i = 0; i < (arr).length \
		 && (cur = ARR_get_at((arr), i).val); ++i)

/* - All functions measure any arguments of type `size_t` in units of MEMB_SIZE.
 *   The `Array`'s MEMB_SIZE field was set when it was created using any
 *   `ARR_create_*` function. 
 *
 * - If a function has an alternative whose name ends in
 *   `*_B`, all `size_t` arguments are measured in bytes rather than MEMB_SIZE
 *   units. Even if the argument isn't of type `size_t`, any argument with an
 *   identifier ending in `*_b` is also measured bytes.
 */


/* Allocate an empty, uninitialized `Array` of NMEMB elements, each of size    
 * MEMB_SIZE bytes. 
 * -- Returns: the allocated `Array` structure */
Result(Array) ARR_create_empty(size_t nmemb, size_t memb_size);

/* Allocate an empty, zeroed `Array` of NMEMB elements, each of size MEMB_SIZE
 * bytes. 
 * -- Returns: the allocated `Array` structure */
Result(Array) ARR_create_zeroed(size_t nmemb, size_t memb_size);


/* Free the existing `Array` structure ARR 
 * (really, there doesn't need to be function for this, but I feel obligated)
 * -- Returns: nothing */
err32_t ARR_free(Array *arr);

/* Allocate a new `Array` and copy in the data from the original `Array` FROM.
 * (the capacity field of the returned `Array` structure is set to the in-use
 * size of FROM (from.nmemb)
 * -- Returns: the allocated `Array` structure with a copy of the data stored
 * in FROM */
Result(Array) ARR_clone(Array from);

/* Resize the existing `Array` structure pointed to by ARRP to NMEMB elements
 * without initializing new memory.
 * The `Array` pointed to by ARRP must have been created by an `ARR_create_*`
 * function.
 * (uses `reallocarray`, so any and all hitches with that function probably
 * apply to this one)
 * If any error occurs (return value is non-zero), the `Array` pointed to by
 * ARRP is not modified.
 * -- Returns: an error code as specified in util/error.h
 */
err32_t ARR_resize(Array *arrp, size_t capacity);

err32_t ARR_shrink(Array *arrp);

/* Perform the equivalent of `ARR_copy_range_to(arrp, 0, p, n)`.
 * If any error occurs (return value is non-zero) ARRP is left unchanged.
 * -- Returns: an error code as specified in util/error.h
 */
err32_t ARR_copy_at_front(Array *arrp, const void *p, size_t n);

err32_t ARR_copy_at_front_B(Array *arrp, const void *p, size_t n_b);

/* Copy N elements (n * arrp->memb_size bytes) starting from P into the
 * `Array` pointed to by ARRP, starting START elements from the front (at
 * (arrp->ptr+start)), assuming P contains elements of the same size as the 
 * `Array` pointed to by ARRP.
 * If any error occurs (return value is non-zero) ARRP is left unchanged.
 * -- Returns: an error code as specified here:
 *  -  0: nothing wrong
 *  - -1: START + N is greater than the current capacity of the array
 *  - -2: P is NULL
 *  - -3: START is out of bounds (required that (start <= arrp->length) evaluates to true)
 *  - -4: ARRP points to an error `Array`
 */
err32_t ARR_copy_range_to(Array *arrp, size_t start, const void *p, size_t n);

err32_t ARR_copy_range_to_B(Array *arrp, size_t start_b, const void *p, size_t n_b);

Result(voidp) ARR_get_at(Array arr, size_t index);
err32_t ARR_copy_one_from(Array arr, size_t index, void *out);
err32_t ARR_copy_one_to(Array *arrp, size_t index, void *in);

/* Copies the value pointed to by IN to one past the last element of 
 * the array, resizing if necessary. */
Result(voidp) ARR_push_back(Array *arrp, void *in);
Result(voidp) ARR_pop_back(Array *arrp);
Result(size_t) ARR_search(Array arr, void *target, comp_func cmp);
Result(size_t) ARR_search_lesser(Array arr, void *target, comp_func cmp);
Result(size_t) ARR_search_greater(Array arr, void *target, comp_func cmp);

void ARR_accumulate(Array arr, accum_func accum, void *out);

#endif /* APLCORE__ARRAY_H */
