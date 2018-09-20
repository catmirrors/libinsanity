#ifndef LIN_PRINTF_H_
#define LIN_PRINTF_H_

#include <stdarg.h>

// snprintf()-like function. Should be mostly C11 compliant, except:
//  - Does not support the following conversion specifiers: n
//  - No L length modifier (for long double)
//  - No l length modifier for the following conversion specifiers: c s
//    (for wchar_t and related, which is highly unportable anyway)
//  - No glibc extensions (e.g. %m)
//  - No Microsoft extensions (this may break with <inttypes.h> format macros)
//  - Locale independent (always uses "C" locale).
//  - Supports the conversion specifier 'b' for binary number output.
//  - str is _always_ 0-terminated (unless size==0).
// The compiler's format checker won't warn against these.
__attribute__((format(printf, 3, 4)))
int lin_snprintf(char *str, size_t size, const char *format, ...);

// See lin_snprintf().
int lin_vsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
