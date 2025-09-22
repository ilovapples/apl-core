#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>
#include <stddef.h>


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


typedef u64 usize;
typedef s64 ssize;


typedef s16 sc16;
typedef u16 uc16;
typedef s32 sc32;
typedef u32 uc32;

typedef s32 fd_t;

typedef s32 err32_t;
typedef s64 err64_t;

#ifndef DISCLUDE_VOLATILE_TYPES
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;


typedef volatile u64 vusize;
typedef volatile s64 vssize;


typedef volatile sc16 vsc16;
typedef volatile uc16 vuc16;
typedef volatile sc32 vsc32;
typedef volatile uc32 vuc32;

typedef volatile fd_t vfd_t;

typedef volatile err32_t verr32_t;
typedef volatile err64_t verr64_t;

#endif

typedef void *void_p;
typedef char *char_p;
typedef wchar_t *wchar_p;

#endif /* TYPES_H */
