#ifndef APLCORE__POLYMORPH_H
#define APLCORE__POLYMORPH_H

#include <sys/types.h>

typedef ssize_t (*comp_func) (void *left, void *right);
/* Modifies the value of accum, doesn't return the new value.
 * ~~When called with NULL for either argument, returns the size of its return
 * value.~~ Doesn't have to do this*/
typedef void (*accum_func) (void *accum, void *val);

#endif /* APLCORE__POLYMORPH_H */
