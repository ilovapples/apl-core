#include <stdlib.h>

#include "aplcore/util/misc.h"

inline void __scope_free(void *p)
{
	free(*(void **)p);
}
