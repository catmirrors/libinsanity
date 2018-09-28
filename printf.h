#ifndef LIN_PRINTF_H_
#define LIN_PRINTF_H_

#include <stdarg.h>

// snprintf()-like function. Should be mostly C11 compliant, except:
//  - Does not support the following conversion specifiers: n
//  - No L length modifier (for long double)
//  - No l length modifier for the following conversion specifiers: c s
//    (for wchar_t and related, which is highly unportable anyway)
//  - No glibc extensions (e.g. %m)
//  - On some systems, <inttypes.h> format macros might break, if they use
//    unsupported extensions (consider systems where int64_t maps to no C
//    standard type, such as Microsoft Windows).
// The compiler's format checker won't warn against these.
//
// It supports the following extensions or guarantees:
//  - Locale independent (always uses "C" locale).
//  - Supports the conversion specifier 'b' for binary number output.
//  - str is _always_ 0-terminated (unless size==0).
//  - Length modifier for explicitly sized integer types:
//      I64u => uint64_t
//      I64d =>  int64_t
//      I32u => uint32_t
//      I32d =>  int32_t
//      I16u => uint16_t
//      I16d =>  int16_t
//      I8u  => uint8_t
//      I8d  =>  int8_t
//      Iu   => size_t
//      Id   => ptrdiff_t
//    (I32, I64 and I are compatible with Microsoft extensions)
__attribute__((format(printf, 3, 4)))
int lin_snprintf(char *str, size_t size, const char *format, ...);

// See lin_snprintf().
int lin_vsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
