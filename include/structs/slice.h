#ifndef APLCORE__SLICE_H
#define APLCORE__SLICE_H

#include <stddef.h>

#include "structs/array.h"

typedef struct Slice {
	void *ptr;
	size_t len;
	size_t memb_size;
} Slice;

typedef struct pRange {
	void *start, *end;
} pRange;

typedef struct iRange {
	size_t start, end;
} iRange;

typedef struct pos_len {
	size_t pos, len;
} pos_len;

#define SLICE_is_err(slice) ((slice).ptr == NULL)
#define ERR_SLICE ((Slice) { .ptr = NULL })
#define ERR_PRANGE ((pRange) {0})
#define ERR_IRANGE ((iRange) {0})
#define ERR_POS_LEN ((pos_len) {0})

MKSTRCT_Result(Slice);
MKSTRCT_Result(pRange);
MKSTRCT_Result(iRange);
MKSTRCT_Result(pos_len);

/* range excludes the value at end */
Result(Slice) SLICE_ptr_irange(void *ptr, iRange irange, size_t memb_size);
Result(Slice) SLICE_ptr_iposlen(void *ptr, size_t ipos, size_t len, size_t memb_size);
Result(Slice) SLICE_ptr_prange(pRange prange, size_t memb_size);
Result(Slice) SLICE_ptr_pposlen(void *ppos, size_t len, size_t memb_size);
Result(Slice) SLICE_arr_irange(Array arr, iRange irange);
Result(Slice) SLICE_arr_iposlen(Array arr, size_t ipos, size_t len);

#endif /* APLCORE__SLICE_H */
