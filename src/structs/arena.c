#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

#include "structs/arena.h"
#include "util/log.h"

Result(Arena) ARENA_create_zeroed(size_t size)
{
	Arena ret;
	ret.ptr = malloc(size);
	memset(ret.ptr, 0, size);
	ret.capacity = size;
	ret.freed = false;
	Result(Array) res = ARR_create_empty(1, sizeof(ArenaBlock));
	if (res.err != NIL)
		return RES(ERR_ARENA, res.err, Arena);
	ret.blocks_arr = res.val;
	ArenaBlock_p first_block = ARR_get_at(ret.blocks_arr, 0).val;

	first_block->in_use = false;
	first_block->poslen.pos = 0;
	first_block->poslen.len = size;

	return RES(ret, ARENA_is_err(ret), Arena);
}

inline void ARENA_free(Arena *arena)
{
	if (arena->freed)
		return;
	ologf(LOG_DEBUG, "freeing arena(capacity=%zu)\n", arena->capacity);
	ARR_free(&arena->blocks_arr);
	free(arena->ptr);
	arena->freed = true;
}

Result(ArenaBlock_p) ARENA_get_block(Arena *arena_p, size_t size)
{
	ologf(LOG_DEBUG, "requested size=%zu\n", size);
	if (arena_p == NULL || size == 0)
		return RES(NULL, 1, ArenaBlock_p);

	if (arena_p->capacity < size)
		return RES(NULL, 2, ArenaBlock_p);
	ArenaBlock_p cur = NULL;
	// capacity is exactly right and no blocks exist yet
	if (arena_p->capacity == size && arena_p->blocks_arr.length == 0)
	{
		ArenaBlock new_block = {
			.in_use = true,
			.poslen = {
				.pos = 0,
				.len = size,
			},
		};

		ArenaBlock_p new_block_p = ARR_push_back(&arena_p->blocks_arr, &new_block).val;
		ologf(LOG_DEBUG, "new block:\n    --> %s\n", ARENA_debug_block_str(new_block_p, arena_p, true));
		ologf(LOG_DEBUG, "block_arr(length=%zu, memb_size=%zu)\n",
				arena_p->blocks_arr.length, arena_p->blocks_arr.memb_size);
		return RES(new_block_p, (new_block_p == NULL), ArenaBlock_p);
	}

	for (size_t i = 0; i < arena_p->blocks_arr.length; ++i)
	{
		cur = ARR_get_at(arena_p->blocks_arr, i).val;
		if (cur->in_use || cur->poslen.len < size)
			continue;

		const size_t remaining_len = cur->poslen.len - size;
		if (remaining_len > 0)
		{
			ArenaBlock new_block = {
				.in_use = false,
				.poslen = {
					.pos = cur->poslen.pos + size,
					.len = remaining_len,
				},
			};
#ifndef APLCORE__DISABLE_LOGGING
			ArenaBlock *temp = ARR_push_back(&arena_p->blocks_arr, &new_block).val;
			ologf(LOG_DEBUG, "new block:\n    --> %s\n", ARENA_debug_block_str(temp, arena_p, true));
			ologf(LOG_DEBUG, "block_arr(length=%zu, memb_size=%zu)\n",
					arena_p->blocks_arr.length, arena_p->blocks_arr.memb_size);
#else
			ARR_push_back(&arena_p->blocks_arr, &new_block);
#endif

			// V must be accessed again since the array may be resized by ARR_push_back V
			cur = ARR_get_at(arena_p->blocks_arr, i).val;
			cur->poslen.len = size;
			cur->in_use = true;

			return RES(cur, (cur == NULL), ArenaBlock_p);
		}
		ologf(LOG_DEBUG, "found sizeable block:\n    --> %s\n", ARENA_debug_block_str(cur, arena_p, true));

		cur->poslen.len = size;
		cur->in_use = true;

		return RES(cur, (cur == NULL), ArenaBlock_p);
	}
	cur = NULL;

	/* sufficient_contiguous_blocks_range = SCBR */
	pos_len scbr = {0};
	cur = ARR_get_at(arena_p->blocks_arr, 0).val;
	Result(void_p) res = {0};
	while (cur->in_use && scbr.pos < arena_p->blocks_arr.length)
	{
		scbr.pos++;
		res = ARR_get_at(arena_p->blocks_arr, scbr.pos);
		if (res.err == 1)
			break;
		else if (res.err != NIL)
			return RES(NULL, 5, ArenaBlock_p);
		cur = res.val;
	}

	if (scbr.pos == arena_p->blocks_arr.length)
		return RES(NULL, 6, ArenaBlock_p);

	size_t total_size = 0;
	while (scbr.pos < arena_p->blocks_arr.length
	    && scbr.pos + scbr.len < arena_p->blocks_arr.length)
	{
		if (cur->in_use)
		{
			scbr.pos += scbr.len;
			scbr.len = 0;
			total_size = 0;
		} else
		{
			scbr.len++;
			total_size += cur->poslen.len;
			if (total_size >= size)
				break;
		}

		cur = ARR_get_at(arena_p->blocks_arr, scbr.pos + scbr.len).val;
	}

	if (total_size < size)
		return RES(NULL, 3, ArenaBlock_p);

	ologf(LOG_DEBUG,
			"could not find single sizeable block, merging sizes of blocks [%03zu, %03zu] "
			"(sum size: %zu bytes)\n",
			scbr.pos, scbr.pos + scbr.len - 1, total_size);

	ArenaBlock_p first_block_p = ARR_get_at(arena_p->blocks_arr, scbr.pos).val;
	first_block_p->poslen.len = size;
	first_block_p->in_use = true;

	ologf(LOG_DEBUG, "used first block of range for data: \n    --> %s\n",
			ARENA_debug_block_str(first_block_p, arena_p, true));

	cur = ARR_get_at(arena_p->blocks_arr, scbr.pos+1).val;
	cur->poslen.pos = first_block_p->poslen.pos + size;
	cur->poslen.len = total_size - size;
	cur->in_use = false;

	for (size_t i = scbr.pos+2; i < scbr.pos + scbr.len; ++i)
	{
		cur = ARR_get_at(arena_p->blocks_arr, i).val;
		cur->poslen.pos = first_block_p->poslen.pos + total_size;
		cur->poslen.len = 0;
		cur->in_use = false;
	}
	cur = ARR_get_at(arena_p->blocks_arr, arena_p->blocks_arr.length - 1).val;
	if (cur->poslen.len == 0)
		ARR_pop_back(&arena_p->blocks_arr);

	return RES(first_block_p, (first_block_p == NULL), ArenaBlock_p);
}

err32_t ARENA_return_block(Arena *arena_p, size_t pos)
{
	ArenaBlock_p cur = NULL;
	for (size_t i = 0; i < arena_p->blocks_arr.length; ++i)
	{
		cur = ARR_get_at(arena_p->blocks_arr, i).val;
		if (cur->poslen.pos == pos)
			break;
	}

	if (cur->poslen.pos != pos)
		return -1;

	ologf(LOG_DEBUG, "returning a block of memory its owner:\n    --> %s\n",
			ARENA_debug_block_str(cur, arena_p, true));

	cur->in_use = false;
	
	return 0;
}

s32 ArenaBlock_compare(const ArenaBlock *left, const ArenaBlock *right)
{
	return left->poslen.pos - right->poslen.pos;
}

void ARENA_sort_blocks_arr(Arena arena)
{
	qsort(arena.blocks_arr.ptr, arena.blocks_arr.length, arena.blocks_arr.memb_size, (int (*) (const void *, const void *))ArenaBlock_compare);
}

Result(Array) ARR_create_empty_in_arena(Arena *arena_p, size_t nmemb, size_t memb_size)
{
	if (nmemb == 0)
		return RES(ERR_ARR, ALLOC_NMEMB_PARAM_IS_ZERO, Array);
	if (memb_size == 0)
		return RES(ERR_ARR, ALLOC_MEMB_SIZE_PARAM_IS_ZERO, Array);

	Array ret = {0};

	if (arena_p == NULL)
		ret.ptr = malloc(nmemb * memb_size);
	else
	{
		Result(ArenaBlock_p) res = ARENA_get_block(arena_p, nmemb * memb_size);
		if (res.err != NIL)
			return RES(ERR_ARR, res.err, Array);
		ret.ptr = (u8 *)arena_p->ptr + res.val->poslen.pos;
	}
	
	ret.capacity_b = (ret.length = nmemb) * memb_size;
	ret.memb_size = memb_size;
	ret.owner = arena_p;

	return RES(ret, ARR_is_err(ret), Array);
}

Result(Array) ARR_create_zeroed_in_arena(Arena *arena_p, size_t nmemb, size_t memb_size)
{
	if (nmemb == 0)
		return RES(ERR_ARR, ALLOC_NMEMB_PARAM_IS_ZERO, Array);
	if (memb_size == 0)
		return RES(ERR_ARR, ALLOC_MEMB_SIZE_PARAM_IS_ZERO, Array);

	Array ret = {0};

	if (arena_p == NULL)
		ret.ptr = calloc(nmemb, memb_size);
	else
	{
		Result(ArenaBlock_p) res = ARENA_get_block(arena_p, nmemb * memb_size);
		if (res.err != NIL)
			return RES(ERR_ARR, res.err, Array);
		ret.ptr = (u8 *)arena_p->ptr + res.val->poslen.pos;
		memset(ret.ptr, 0, nmemb * memb_size);
	}

	ret.capacity_b = (ret.length = nmemb) * memb_size;
	ret.memb_size = memb_size;
	ret.owner = arena_p;

	return RES(ret, ARR_is_err(ret), Array);
}

inline err32_t ARR_free(Array *arr)
{
	if (arr->freed)
		return GENERIC_DOUBLE_FREE_ERR;

	if (arr->owner == NULL)
	{
		free(arr->ptr);
		arr->freed = true;
		return NIL;
	}

	ARENA_return_block(arr->owner, (u8 *)arr->ptr - (u8 *)arr->owner->ptr);
	arr->owner = NULL;
	arr->freed = true;
	return NIL;
}

inline err32_t STR_free(String *str_p)
{
	if (str_p->freed)
		return GENERIC_DOUBLE_FREE_ERR;

	if (str_p->owner == NULL)
	{
		free(str_p->ptr);
		str_p->freed = true;
		return NIL;
	}

	ARENA_return_block(str_p->owner, (u8 *)str_p->ptr - (u8 *)str_p->owner->ptr);
	str_p->owner = NULL;
	str_p->freed = true;

	return NIL;
}

char *ARENA_debug_block_str(ArenaBlock *block_p, Arena *owner, bool print_spec)
{
	#define BUF_LEN 500
	static char ret[BUF_LEN + 1] = {0};
	char *pos = ret;
	pos += snprintf(ret, 18, "block(n=%03zu, ", block_p - (ArenaBlock *)owner->blocks_arr.ptr);
	if (print_spec)
		pos += sprintf(pos, "spec_addr=%p, ", (void *)block_p);
	snprintf(pos, BUF_LEN - (pos - ret), "data_addr=%p, start=%zu, end=%zu, len=%zu, in_use=%s)",
			(void *) ((u8 *)owner->ptr + block_p->poslen.pos),
			block_p->poslen.pos, block_p->poslen.pos + block_p->poslen.len, block_p->poslen.len,
			(block_p->in_use) ? "true" : "false");
	return ret;
}

wchar_t *ARENA_debug_block_wcs(ArenaBlock *block_p, Arena *owner, bool print_spec)
{
	#define BUF_LEN 500
	static wchar_t ret[BUF_LEN + 1] = {0};
	wchar_t *pos = ret;
	pos += swprintf(ret, 20, L"block(n=%03zu, ", block_p - (ArenaBlock *)owner->blocks_arr.ptr);
	if (print_spec)
		pos += swprintf(pos, 24, L"spec_addr=%p, ", (void *)block_p);
	swprintf(pos, BUF_LEN - (pos - ret), L"data_addr=%p, start=%zu, end=%zu, len=%zu, in_use=%ls)",
			(void *) ((u8 *)owner->ptr + block_p->poslen.pos),
			block_p->poslen.pos, block_p->poslen.pos + block_p->poslen.len, block_p->poslen.len,
			(block_p->in_use) ? L"true" : L"false");
	return ret;
}
