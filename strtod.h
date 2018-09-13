#ifndef LIN_STRTOD_H_
#define LIN_STRTOD_H_

// According to C11. Locale independent (always uses "C" locale).
double lin_strtod(const char *nptr, char **endptr);
float lin_strtof(const char *nptr, char **endptr);
long double lin_strtold(const char *nptr, char **endptr);

#endif
