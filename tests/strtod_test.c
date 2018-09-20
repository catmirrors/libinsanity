// From libc-testsuite (musl), a9baddd7d07b9fe15e212985a808a79773ec72e4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "printf.h"
#include "strtod.h"

/* r = place to store result
 * f = function call to test (or any expression)
 * x = expected result
 * m = message to print on failure (with formats for r & x)
 **/

#define TEST3(r, f, x, m) ( \
    ((r) = (f)) == (x) || \
    (printf(__FILE__ ":%d: %s failed (" m ")\n", __LINE__, #f, r, x, r - x), \
        err++, 0))

#define TEST2(r, f, x, m) ( \
    ((r) = (f)) == (x) || \
    (printf(__FILE__ ":%d: %s failed (" m ")\n", __LINE__, #f, r, x), err++, \
        0))

__attribute__((format(printf, 3, 4)))
typedef int (*snprintf_type)(char *str, size_t size, const char *format, ...);
typedef double (*strtod_type)(const char *nptr, char **endptr);

static int run_test(snprintf_type cur_snprintf, strtod_type cur_strtod)
{
    int i;
    double d, d2;
    char buf[1000];
    int err = 0;

    for (i = 0; i < 100; i++) {
        d = sin(i);
        cur_snprintf(buf, sizeof buf, "%.300f", d);
        TEST3(d2, cur_strtod(buf, 0), d, "round trip fail %a != %a (%a)");
    }

    TEST2(d, cur_strtod("0x1p4", 0), 16.0, "hex float %a != %a");
    TEST2(d, cur_strtod("0x1.1p4", 0), 17.0, "hex float %a != %a");

    if (!err)
        printf("Tests succeeded.\n");

    return err;
}

int main()
{
    int err = 0;

    printf("Testing system snprintf() + system strtod()...\n");
    err |= run_test(snprintf, strtod);

    printf("Testing system snprintf() + lin_strtod()...\n");
    err |= run_test(snprintf, lin_strtod);

    printf("Testing lin_snprintf() + system strtod()...\n");
    err |= run_test(lin_snprintf, strtod);

    if (!err)
        printf("All tests succeeded.\n");
    return err ? 1 : 0;
}
