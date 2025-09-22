#include "aplcore/structs/slice.h"

// check that poslen is on multiples of memb_size or something
Result(Slice) SLICE_ptr_irange(void *ptr, iRange irange, size_t memb_size)
{
	if (irange.start >= irange.end)
		return RES(ERR_SLICE, 1, Slice);
	Slice ret;
	ret.ptr = (u8 *)ptr + irange.start*memb_size;
	ret.len = irange.end - irange.start;
	ret.memb_size = memb_size;

	return RES(ret, 0, Slice);
}

Result(Slice) SLICE_ptr_iposlen(void *ptr, size_t ipos, size_t len, size_t memb_size)
{
	Slice ret;
	ret.ptr = (u8 *)ptr + ipos*memb_size;
	ret.len = len;
	ret.memb_size = memb_size;

	return RES(ret, 0, Slice);
}

Result(Slice) SLICE_ptr_prange(pRange prange, size_t memb_size)
{
	Slice ret;
	ret.ptr = prange.start;
	ret.len = (u8 *)prange.end - (u8 *)prange.start;
	ret.memb_size = memb_size;

	return RES(ret, 0, Slice);
}

Result(Slice) SLICE_ptr_pposlen(void *ppos, size_t len, size_t memb_size)
{
	Slice ret;
	ret.ptr = ppos;
	ret.len = len;
	ret.memb_size = memb_size;

	return RES(ret, 0, Slice);
}

Result(Slice) SLICE_arr_irange(Array arr, iRange irange)
{
	if (irange.start >= irange.end
	 || irange.start > arr.length
	 || irange.end > arr.length)
		return RES(ERR_SLICE, 1, Slice);
		
	Slice ret;
	ret.ptr = (u8 *)arr.ptr + irange.start*arr.memb_size;
	ret.len = irange.end - irange.start;
	ret.memb_size = arr.memb_size;

	return RES(ret, 0, Slice);
}

Result(Slice) SLICE_arr_iposlen(Array arr, size_t ipos, size_t len)
{
	if (ipos > arr.length
	 || ipos + len > arr.length)
		return RES(ERR_SLICE, 1, Slice);

	Slice ret;
	ret.ptr = (u8 *)arr.ptr + ipos*arr.memb_size;
	ret.len = len;
	ret.memb_size = arr.memb_size;

	return RES(ret, 0, Slice);
}

Result(pRange) SLICE_get_prange(Slice slice)
{
	pRange ret;
	ret.start = slice.ptr;
	ret.end = (u8 *)slice.ptr + slice.len*slice.memb_size;

	return RES(ret, SLICE_is_err(slice), pRange);
}
