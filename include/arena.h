#ifndef APLCORE__ARENA_H
#define APLCORE__ARENA_H

#include <stdbool.h>

#include "aplcore/array.h"
#include "aplcore/slice.h"
#include "aplcore/util/error.h"

typedef struct ArenaBlock {
	pos_len poslen; // pos is byte offset from Arena::ptr, len is also in bytes
	bool in_use;
} ArenaBlock, *ArenaBlock_p;
MKSTRCT_Result(ArenaBlock_p);

typedef Array ArrayT(ArenaBlock);

typedef struct Arena {
	void *ptr;
	size_t capacity;
	ArrayT(ArenaBlock) blocks_arr;
} Arena;
MKSTRCT_Result(Arena);

#define ARENA_is_err(arena) ((arena).ptr == NULL)
#define ERR_ARENA ((Arena) {0})

#define INITIAL_BLOCK_SIZE 8

/* Allocate and return an `Arena` of SIZE bytes */
Result(Arena) ARENA_create_zeroed(size_t size);

/* Free an `Arena` ARENA (including the Arena::blocks_arr Array and the 
 * memory block, making any pointer obtained from the `Arena` invalid).
 * It's unnecessary to call `ARENA_return_block` for every block prior to
 * freeing the Arena which owns the block, but no error will be raised anyway.
 * Actually, the point of an `Arena` (from my perspective) is that you don't
 * have to allocate and free several separate blocks of memory; you only have
 * to allocate/free a couple, depending on how many different arenas you need.
 */
void ARENA_free(Arena *arena);

/* Requests and obtains pointer to a structure of information regarding the
 * requested block of memory of size SIZE and the location at which it was
 * found in the heap space of the `Arena` at ARENA_P.
 */
Result(ArenaBlock_p) ARENA_get_block(Arena *arena_p, size_t size);

/* Finds the block allocated at position POS and returns it to the `Arena` at
 * ARENA_P, without clearing the memory.
 *
 * -- Returns:
 *  -  0: nothing wrong
 *  - -1: no designated block starting at POS in ARENA_P
 */
err32_t ARENA_return_block(Arena *arena_p, size_t pos);

void ARENA_sort_blocks_arr(Arena arena);

/* Requests a block of memory of size NMEMB * MEMB_SIZE bytes from the `Arena`
 * pointed to by ARENA_P. If it ARENA_P is NULL, it falls back to the behavior
 * of ARR_create_empty and uses `malloc` to request memory from the operation
 * system.
 */
Result(Array) ARR_create_empty_in_arena(Arena *arena_p, size_t nmemb, size_t memb_size);

/* Requests a block of memory of size NMEMB * MEMB_SIZE bytes from the `Arena`
 * pointed to by ARENA_P and zeroes it out. If it ARENA_P is NULL, it falls
 * back to the behavior of ARR_create_empty and uses `calloc` to request memory
 * from the operation system.
 */
Result(Array) ARR_create_zeroed_in_arena(Arena *arena_p, size_t nmemb, size_t memb_size);

char *ARENA_debug_block_str(ArenaBlock *block_p, Arena *owner, bool print_spec);
wchar_t *ARENA_debug_block_wcs(ArenaBlock *block_p, Arena *owner, bool print_spec);

#endif /* APLCORE__ARENA_H */
