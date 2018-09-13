#ifndef LIN_CTYPE_H_
#define LIN_CTYPE_H_

// Roughly follows standard C semantics, but doesn't account for EOF, uses char
// as parameter, and is locale independent (always uses "C" locale).
// The parameter type change means that passing EOF (from stdio) is meaningless,
// but on the other hand, e.g. char *s = ...; lin_isspace(s[0]) is not a bug
// (isspace(s[0]) is a bug, and can crash if char is signed).

static inline int lin_isprint(char c) { return (unsigned char)c >= 32; }
static inline int lin_isspace(char c) { return c == ' ' || c == '\f' || c == '\n' ||
                                              c == '\r' || c == '\t' || c =='\v'; }
static inline int lin_isupper(char c) { return c >= 'A' && c <= 'Z'; }
static inline int lin_islower(char c) { return c >= 'a' && c <= 'z'; }
static inline int lin_isdigit(char c) { return c >= '0' && c <= '9'; }
static inline int lin_isalpha(char c) { return lin_isupper(c) || lin_islower(c); }
static inline int lin_isalnum(char c) { return lin_isalpha(c) || lin_isdigit(c); }

static inline char lin_tolower(char c) { return lin_isupper(c) ? c - 'A' + 'a' : c; }
static inline char lin_toupper(char c) { return lin_islower(c) ? c - 'a' + 'A' : c; }

#endif
