#include <stdlib.h>

#include "util/misc.h"

inline void __scope_free(void *p)
{
	free(*(void **)p);
}
