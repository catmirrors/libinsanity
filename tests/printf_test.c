///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2017-2018, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Additional tests from libc-testsuite (musl)
// a9baddd7d07b9fe15e212985a808a79773ec72e4


#undef NDEBUG
#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "printf.h"

#define REQUIRE(x) assert(x)

#define REQUIRE_STR_EQ(got, expect) do {    \
    if (strcmp(got, expect)) {              \
        printf("Got: '%s'\n", got);         \
        printf("Expected: '%s'\n", expect); \
    }                                       \
    assert(!strcmp(got, expect));           \
} while (0)

// Some tests use redundant format specifier flags (against which gcc warns,
// and which OS libc will usually not support).
#define TEST_NON_STANDARD 0

// Tests redundant flags, against which gcc warns. It's possible that testing
// this would be worth anyway, because the standard does not forbid passing
// them. Normally they just cause noise with -Wformat (which you should always
// enable).
#define TEST_REDUNDANT_FLAGS 0

// Implementation defined formatting (%p in particular).
#define TEST_IMPL_DEFINED 0

#define EXP(...) __VA_ARGS__

// Require that printf(args) outputs res.
#define TEST_SNPRINTF(args, res) do {                   \
    cur_snprintf(buffer, sizeof(buffer), EXP args);     \
    REQUIRE_STR_EQ(buffer, res);                        \
} while (0)

// Require that snprintf(..., 0, args) returns res.
#define TEST_SNPRINTF_N(args, res) do {                 \
    int res_ = cur_snprintf(NULL, 0, EXP args);         \
    REQUIRE(res == res_);                               \
} while (0)

__attribute__((format(printf, 3, 4)))
typedef int (*snprintf_type)(char *str, size_t size, const char *format, ...);

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

static void run_test(snprintf_type cur_snprintf)
{
    char buffer[200];
    int ret;

    TEST_SNPRINTF(("%d", -1000), "-1000");
    cur_snprintf(buffer, 3U, "%d", -1000);
    REQUIRE_STR_EQ(buffer, "-1");

    TEST_SNPRINTF(("%d", -1), "-1");

    cur_snprintf(buffer, sizeof(buffer), "%d %d %s", 3, -1000, "test");
    REQUIRE_STR_EQ(buffer, "3 -1000 test");

    TEST_SNPRINTF(("% d", 42), " 42");
    TEST_SNPRINTF(("% d", -42), "-42");
    TEST_SNPRINTF(("% 5d", 42), "   42");
    TEST_SNPRINTF(("% 5d", -42), "  -42");
    TEST_SNPRINTF(("% 15d", 42), "             42");
    TEST_SNPRINTF(("% 15d", -42), "            -42");
    TEST_SNPRINTF(("% 15d", -42), "            -42");
    TEST_SNPRINTF(("% 15.3f", -42.987), "        -42.987");
    TEST_SNPRINTF(("% 15.3f", 42.987), "         42.987");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("% s", "Hello testing"), "Hello testing");
    #endif

    TEST_SNPRINTF(("% d", 1024), " 1024");
    TEST_SNPRINTF(("% d", -1024), "-1024");
    TEST_SNPRINTF(("% i", 1024), " 1024");
    TEST_SNPRINTF(("% i", -1024), "-1024");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("% u", 1024), "1024");
    TEST_SNPRINTF(("% u", 4294966272U), "4294966272");
    TEST_SNPRINTF(("% o", 511), "777");
    TEST_SNPRINTF(("% o", 4294966785U), "37777777001");
    TEST_SNPRINTF(("% x", 305441741), "1234abcd");
    TEST_SNPRINTF(("% x", 3989525555U), "edcb5433");
    TEST_SNPRINTF(("% X", 305441741), "1234ABCD");
    TEST_SNPRINTF(("% X", 3989525555U), "EDCB5433");
    TEST_SNPRINTF(("% c", 'x'), "x");
    #endif

    TEST_SNPRINTF(("%+d", 42), "+42");
    TEST_SNPRINTF(("%+d", -42), "-42");
    TEST_SNPRINTF(("%+5d", 42), "  +42");
    TEST_SNPRINTF(("%+5d", -42), "  -42");
    TEST_SNPRINTF(("%+15d", 42), "            +42");
    TEST_SNPRINTF(("%+15d", -42), "            -42");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%+s", "Hello testing"), "Hello testing");
    #endif

    TEST_SNPRINTF(("%+d", 1024), "+1024");
    TEST_SNPRINTF(("%+d", -1024), "-1024");
    TEST_SNPRINTF(("%+i", 1024), "+1024");
    TEST_SNPRINTF(("%+i", -1024), "-1024");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%+u", 1024), "1024");
    TEST_SNPRINTF(("%+u", 4294966272U), "4294966272");
    TEST_SNPRINTF(("%+o", 511), "777");
    TEST_SNPRINTF(("%+o", 4294966785U), "37777777001");
    TEST_SNPRINTF(("%+x", 305441741), "1234abcd");
    TEST_SNPRINTF(("%+x", 3989525555U), "edcb5433");
    TEST_SNPRINTF(("%+X", 305441741), "1234ABCD");
    TEST_SNPRINTF(("%+X", 3989525555U), "EDCB5433");
    TEST_SNPRINTF(("%+c", 'x'), "x");
    #endif

    TEST_SNPRINTF(("%0d", 42), "42");
    TEST_SNPRINTF(("%0ld", 42L), "42");
    TEST_SNPRINTF(("%0d", -42), "-42");
    TEST_SNPRINTF(("%05d", 42), "00042");
    TEST_SNPRINTF(("%05d", -42), "-0042");
    TEST_SNPRINTF(("%015d", 42), "000000000000042");
    TEST_SNPRINTF(("%015d", -42), "-00000000000042");
    TEST_SNPRINTF(("%015.2f", 42.1234), "000000000042.12");
    TEST_SNPRINTF(("%015.3f", 42.9876), "00000000042.988");
    TEST_SNPRINTF(("%015.5f", -42.9876), "-00000042.98760");
    TEST_SNPRINTF(("%-d", 42), "42");
    TEST_SNPRINTF(("%-d", -42), "-42");
    TEST_SNPRINTF(("%-5d", 42), "42   ");
    TEST_SNPRINTF(("%-5d", -42), "-42  ");
    TEST_SNPRINTF(("%-15d", 42), "42             ");
    TEST_SNPRINTF(("%-15d", -42), "-42            ");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%-0d", 42), "42");
    TEST_SNPRINTF(("%-0d", -42), "-42");
    TEST_SNPRINTF(("%-05d", 42), "42   ");
    TEST_SNPRINTF(("%-05d", -42), "-42  ");
    TEST_SNPRINTF(("%-015d", 42), "42             ");
    TEST_SNPRINTF(("%-015d", -42), "-42            ");
    TEST_SNPRINTF(("%0-d", 42), "42");
    TEST_SNPRINTF(("%0-d", -42), "-42");
    TEST_SNPRINTF(("%0-5d", 42), "42   ");
    TEST_SNPRINTF(("%0-5d", -42), "-42  ");
    TEST_SNPRINTF(("%0-15d", 42), "42             ");
    TEST_SNPRINTF(("%0-15d", -42), "-42            ");
    #endif

    cur_snprintf(buffer, sizeof(buffer), "Hello testing");
    REQUIRE_STR_EQ(buffer, "Hello testing");

    TEST_SNPRINTF(("%s", "Hello testing"), "Hello testing");
    TEST_SNPRINTF(("%d", 1024), "1024");
    TEST_SNPRINTF(("%d", -1024), "-1024");
    TEST_SNPRINTF(("%i", 1024), "1024");
    TEST_SNPRINTF(("%i", -1024), "-1024");
    TEST_SNPRINTF(("%u", 1024), "1024");
    TEST_SNPRINTF(("%u", 4294966272U), "4294966272");
    TEST_SNPRINTF(("%o", 511), "777");
    TEST_SNPRINTF(("%o", 4294966785U), "37777777001");
    TEST_SNPRINTF(("%x", 305441741), "1234abcd");
    TEST_SNPRINTF(("%x", 3989525555U), "edcb5433");
    TEST_SNPRINTF(("%X", 305441741), "1234ABCD");
    TEST_SNPRINTF(("%X", 3989525555U), "EDCB5433");
    TEST_SNPRINTF(("%%"), "%");

    TEST_SNPRINTF(("%1s", "Hello testing"), "Hello testing");
    TEST_SNPRINTF(("%1d", 1024), "1024");
    TEST_SNPRINTF(("%1d", -1024), "-1024");
    TEST_SNPRINTF(("%1i", 1024), "1024");
    TEST_SNPRINTF(("%1i", -1024), "-1024");
    TEST_SNPRINTF(("%1u", 1024), "1024");
    TEST_SNPRINTF(("%1u", 4294966272U), "4294966272");
    TEST_SNPRINTF(("%1o", 511), "777");
    TEST_SNPRINTF(("%1o", 4294966785U), "37777777001");
    TEST_SNPRINTF(("%1x", 305441741), "1234abcd");
    TEST_SNPRINTF(("%1x", 3989525555U), "edcb5433");
    TEST_SNPRINTF(("%1X", 305441741), "1234ABCD");
    TEST_SNPRINTF(("%1X", 3989525555U), "EDCB5433");
    TEST_SNPRINTF(("%1c", 'x'), "x");
    TEST_SNPRINTF(("%20s", "Hello"), "               Hello");
    TEST_SNPRINTF(("%20d", 1024), "                1024");
    TEST_SNPRINTF(("%20d", -1024), "               -1024");
    TEST_SNPRINTF(("%20i", 1024), "                1024");
    TEST_SNPRINTF(("%20i", -1024), "               -1024");
    TEST_SNPRINTF(("%20u", 1024), "                1024");
    TEST_SNPRINTF(("%20u", 4294966272U), "          4294966272");
    TEST_SNPRINTF(("%20o", 511), "                 777");
    TEST_SNPRINTF(("%20o", 4294966785U), "         37777777001");
    TEST_SNPRINTF(("%20x", 305441741), "            1234abcd");
    TEST_SNPRINTF(("%20x", 3989525555U), "            edcb5433");
    TEST_SNPRINTF(("%20X", 305441741), "            1234ABCD");
    TEST_SNPRINTF(("%20X", 3989525555U), "            EDCB5433");
    TEST_SNPRINTF(("%20c", 'x'), "                   x");
    TEST_SNPRINTF(("%*s", 20, "Hello"), "               Hello");
    TEST_SNPRINTF(("%*d", 20, 1024), "                1024");
    TEST_SNPRINTF(("%*d", 20, -1024), "               -1024");
    TEST_SNPRINTF(("%*i", 20, 1024), "                1024");
    TEST_SNPRINTF(("%*i", 20, -1024), "               -1024");
    TEST_SNPRINTF(("%*u", 20, 1024), "                1024");
    TEST_SNPRINTF(("%*u", 20, 4294966272U), "          4294966272");
    TEST_SNPRINTF(("%*o", 20, 511), "                 777");
    TEST_SNPRINTF(("%*o", 20, 4294966785U), "         37777777001");
    TEST_SNPRINTF(("%*x", 20, 305441741), "            1234abcd");
    TEST_SNPRINTF(("%*x", 20, 3989525555U), "            edcb5433");
    TEST_SNPRINTF(("%*X", 20, 305441741), "            1234ABCD");
    TEST_SNPRINTF(("%*X", 20, 3989525555U), "            EDCB5433");
    TEST_SNPRINTF(("%*c", 20,'x'), "                   x");
    TEST_SNPRINTF(("%-20s", "Hello"), "Hello               ");
    TEST_SNPRINTF(("%-20d", 1024), "1024                ");
    TEST_SNPRINTF(("%-20d", -1024), "-1024               ");
    TEST_SNPRINTF(("%-20i", 1024), "1024                ");
    TEST_SNPRINTF(("%-20i", -1024), "-1024               ");
    TEST_SNPRINTF(("%-20u", 1024), "1024                ");
    TEST_SNPRINTF(("%-20.4f", 1024.1234), "1024.1234           ");
    TEST_SNPRINTF(("%-20u", 4294966272U), "4294966272          ");
    TEST_SNPRINTF(("%-20o", 511), "777                 ");
    TEST_SNPRINTF(("%-20o", 4294966785U), "37777777001         ");
    TEST_SNPRINTF(("%-20x", 305441741), "1234abcd            ");
    TEST_SNPRINTF(("%-20x", 3989525555U), "edcb5433            ");
    TEST_SNPRINTF(("%-20X", 305441741), "1234ABCD            ");
    TEST_SNPRINTF(("%-20X", 3989525555U), "EDCB5433            ");
    TEST_SNPRINTF(("%-20c", 'x'), "x                   ");
    TEST_SNPRINTF(("|%5d| |%-2d| |%5d|", 9, 9, 9), "|    9| |9 | |    9|");
    TEST_SNPRINTF(("|%5d| |%-2d| |%5d|", 10, 10, 10), "|   10| |10| |   10|");
    TEST_SNPRINTF(("|%5d| |%-12d| |%5d|", 9, 9, 9), "|    9| |9           | |    9|");
    TEST_SNPRINTF(("|%5d| |%-12d| |%5d|", 10, 10, 10), "|   10| |10          | |   10|");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%0-20s", "Hello"), "Hello               ");
    TEST_SNPRINTF(("%0-20d", 1024), "1024                ");
    TEST_SNPRINTF(("%0-20d", -1024), "-1024               ");
    TEST_SNPRINTF(("%0-20i", 1024), "1024                ");
    TEST_SNPRINTF(("%0-20i", -1024), "-1024               ");
    TEST_SNPRINTF(("%0-20u", 1024), "1024                ");
    TEST_SNPRINTF(("%0-20u", 4294966272U), "4294966272          ");
    TEST_SNPRINTF(("%0-20o", 511), "777                 ");
    TEST_SNPRINTF(("%0-20o", 4294966785U), "37777777001         ");
    TEST_SNPRINTF(("%0-20x", 305441741), "1234abcd            ");
    TEST_SNPRINTF(("%0-20x", 3989525555U), "edcb5433            ");
    TEST_SNPRINTF(("%0-20X", 305441741), "1234ABCD            ");
    TEST_SNPRINTF(("%0-20X", 3989525555U), "EDCB5433            ");
    TEST_SNPRINTF(("%0-20c", 'x'), "x                   ");
    #endif

    TEST_SNPRINTF(("%020d", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%020d", -1024), "-0000000000000001024");
    TEST_SNPRINTF(("%020i", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%020i", -1024), "-0000000000000001024");
    TEST_SNPRINTF(("%020u", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%020u", 4294966272U), "00000000004294966272");
    TEST_SNPRINTF(("%020o", 511), "00000000000000000777");
    TEST_SNPRINTF(("%020o", 4294966785U), "00000000037777777001");
    TEST_SNPRINTF(("%020x", 305441741), "0000000000001234abcd");
    TEST_SNPRINTF(("%020x", 3989525555U), "000000000000edcb5433");
    TEST_SNPRINTF(("%020X", 305441741), "0000000000001234ABCD");
    TEST_SNPRINTF(("%020X", 3989525555U), "000000000000EDCB5433");
    TEST_SNPRINTF(("%.20d", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%.20d", -1024), "-00000000000000001024");
    TEST_SNPRINTF(("%.20i", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%.20i", -1024), "-00000000000000001024");
    TEST_SNPRINTF(("%.20u", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%.20u", 4294966272U), "00000000004294966272");
    TEST_SNPRINTF(("%.20o", 511), "00000000000000000777");
    TEST_SNPRINTF(("%.20o", 4294966785U), "00000000037777777001");
    TEST_SNPRINTF(("%.20x", 305441741), "0000000000001234abcd");
    TEST_SNPRINTF(("%.20x", 3989525555U), "000000000000edcb5433");
    TEST_SNPRINTF(("%.20X", 305441741), "0000000000001234ABCD");
    TEST_SNPRINTF(("%.20X", 3989525555U), "000000000000EDCB5433");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%#020d", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%#020d", -1024), "-0000000000000001024");
    TEST_SNPRINTF(("%#020i", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%#020i", -1024), "-0000000000000001024");
    TEST_SNPRINTF(("%#020u", 1024), "00000000000000001024");
    TEST_SNPRINTF(("%#020u", 4294966272U), "00000000004294966272");
    #endif

    TEST_SNPRINTF(("%#020o", 511), "00000000000000000777");
    TEST_SNPRINTF(("%#020o", 4294966785U), "00000000037777777001");
    TEST_SNPRINTF(("%#020x", 305441741), "0x00000000001234abcd");
    TEST_SNPRINTF(("%#020x", 3989525555U), "0x0000000000edcb5433");
    TEST_SNPRINTF(("%#020X", 305441741), "0X00000000001234ABCD");
    TEST_SNPRINTF(("%#020X", 3989525555U), "0X0000000000EDCB5433");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%#20d", 1024), "                1024");
    TEST_SNPRINTF(("%#20d", -1024), "               -1024");
    TEST_SNPRINTF(("%#20i", 1024), "                1024");
    TEST_SNPRINTF(("%#20i", -1024), "               -1024");
    TEST_SNPRINTF(("%#20u", 1024), "                1024");
    TEST_SNPRINTF(("%#20u", 4294966272U), "          4294966272");
    #endif

    TEST_SNPRINTF(("%#20o", 511), "                0777");
    TEST_SNPRINTF(("%#20o", 4294966785U), "        037777777001");
    TEST_SNPRINTF(("%#20x", 305441741), "          0x1234abcd");
    TEST_SNPRINTF(("%#20x", 3989525555U), "          0xedcb5433");
    TEST_SNPRINTF(("%#20X", 305441741), "          0X1234ABCD");
    TEST_SNPRINTF(("%#20X", 3989525555U), "          0XEDCB5433");
    TEST_SNPRINTF(("%20.5d", 1024), "               01024");
    TEST_SNPRINTF(("%20.5d", -1024), "              -01024");
    TEST_SNPRINTF(("%20.5i", 1024), "               01024");
    TEST_SNPRINTF(("%20.5i", -1024), "              -01024");
    TEST_SNPRINTF(("%20.5u", 1024), "               01024");
    TEST_SNPRINTF(("%20.5u", 4294966272U), "          4294966272");
    TEST_SNPRINTF(("%20.5o", 511), "               00777");
    TEST_SNPRINTF(("%20.5o", 4294966785U), "         37777777001");
    TEST_SNPRINTF(("%20.5x", 305441741), "            1234abcd");
    TEST_SNPRINTF(("%20.10x", 3989525555U), "          00edcb5433");
    TEST_SNPRINTF(("%20.5X", 305441741), "            1234ABCD");
    TEST_SNPRINTF(("%20.10X", 3989525555U), "          00EDCB5433");
    TEST_SNPRINTF(("%.0s", "Hello testing"), "");
    TEST_SNPRINTF(("%20.0s", "Hello testing"), "                    ");
    TEST_SNPRINTF(("%.s", "Hello testing"), "");
    TEST_SNPRINTF(("%20.s", "Hello testing"), "                    ");
    TEST_SNPRINTF(("%20.0d", 1024), "                1024");
    TEST_SNPRINTF(("%20.0d", -1024), "               -1024");
    TEST_SNPRINTF(("%20.d", 0), "                    ");
    TEST_SNPRINTF(("%20.0i", 1024), "                1024");
    TEST_SNPRINTF(("%20.i", -1024), "               -1024");
    TEST_SNPRINTF(("%20.i", 0), "                    ");
    TEST_SNPRINTF(("%20.u", 1024), "                1024");
    TEST_SNPRINTF(("%20.0u", 4294966272U), "          4294966272");
    TEST_SNPRINTF(("%20.u", 0U), "                    ");
    TEST_SNPRINTF(("%20.o", 511), "                 777");
    TEST_SNPRINTF(("%20.0o", 4294966785U), "         37777777001");
    TEST_SNPRINTF(("%20.o", 0U), "                    ");
    TEST_SNPRINTF(("%20.x", 305441741), "            1234abcd");
    TEST_SNPRINTF(("%50.x", 305441741), "                                          1234abcd");
    TEST_SNPRINTF(("%50.x%10.u", 305441741, 12345), "                                          1234abcd     12345");
    TEST_SNPRINTF(("%20.0x", 3989525555U), "            edcb5433");
    TEST_SNPRINTF(("%20.x", 0U), "                    ");
    TEST_SNPRINTF(("%20.X", 305441741), "            1234ABCD");
    TEST_SNPRINTF(("%20.0X", 3989525555U), "            EDCB5433");
    TEST_SNPRINTF(("%20.X", 0U), "                    ");
    TEST_SNPRINTF(("%.4f", 3.1415354), "3.1415");
    TEST_SNPRINTF(("%.3f", 30343.1415354), "30343.142");
    TEST_SNPRINTF(("%.0f", 34.1415354), "34");
    TEST_SNPRINTF(("%.2f", 42.8952), "42.90");
    TEST_SNPRINTF(("%.9f", 42.8952), "42.895200000");
    TEST_SNPRINTF(("%.10f", 42.895223), "42.8952230000");
    TEST_SNPRINTF(("%.12f", 42.89522312345678), "42.895223123457");
    TEST_SNPRINTF(("%.12f", 42.89522387654321), "42.895223876543");
    TEST_SNPRINTF(("%6.2f", 42.8952), " 42.90");
    TEST_SNPRINTF(("%+6.2f", 42.8952), "+42.90");
    TEST_SNPRINTF(("%+5.1f", 42.9252), "+42.9");
    TEST_SNPRINTF(("%f", 42.5), "42.500000");
    TEST_SNPRINTF(("%.1f", 42.5), "42.5");
    TEST_SNPRINTF(("%f", 42167.0), "42167.000000");
    TEST_SNPRINTF(("%.9f", -12345.987654321), "-12345.987654321");
    TEST_SNPRINTF(("%.1f", 3.999), "4.0");
    TEST_SNPRINTF(("%.0f", 3.5), "4");
    TEST_SNPRINTF(("%.0f", 3.49), "3");
    TEST_SNPRINTF(("%.1f", 3.49), "3.5");
    TEST_SNPRINTF(("%.1f", 1E20), "100000000000000000000.0");
    TEST_SNPRINTF(("a%-5.1f", 0.5), "a0.5  ");
    TEST_SNPRINTF(("%i", 0), "0");
    TEST_SNPRINTF(("%i", 1234), "1234");
    TEST_SNPRINTF(("%i", 32767), "32767");
    TEST_SNPRINTF(("%i", -32767), "-32767");
    TEST_SNPRINTF(("%li", 30L), "30");
    TEST_SNPRINTF(("%li", -2147483647L), "-2147483647");
    TEST_SNPRINTF(("%li", 2147483647L), "2147483647");
    TEST_SNPRINTF(("%lli", 30LL), "30");
    TEST_SNPRINTF(("%lli", -9223372036854775807LL), "-9223372036854775807");
    TEST_SNPRINTF(("%lli", 9223372036854775807LL), "9223372036854775807");
    TEST_SNPRINTF(("%lu", 100000L), "100000");
    TEST_SNPRINTF(("%lu", 0xFFFFFFFFL), "4294967295");
    TEST_SNPRINTF(("%llu", 281474976710656LLU), "281474976710656");
    TEST_SNPRINTF(("%llu", 18446744073709551615LLU), "18446744073709551615");
    TEST_SNPRINTF(("%zu", 2147483647UL), "2147483647");
    TEST_SNPRINTF(("%zd", 2147483647UL), "2147483647");
    TEST_SNPRINTF(("%zi", (ptrdiff_t)-2147483647LL), "-2147483647");
    #if TEST_NON_STANDARD
    // Unportable extension in original printf implementation.
    TEST_SNPRINTF(("%b", 60000), "1110101001100000");
    // Unportable extension in original printf implementation.
    TEST_SNPRINTF(("%lb", 12345678L), "101111000110000101001110");
    #endif

    TEST_SNPRINTF(("%o", 60000), "165140");
    TEST_SNPRINTF(("%lo", 12345678L), "57060516");
    TEST_SNPRINTF(("%lx", 0x12345678L), "12345678");
    TEST_SNPRINTF(("%llx", 0x1234567891234567LLU), "1234567891234567");
    TEST_SNPRINTF(("%lx", 0xabcdefabL), "abcdefab");
    TEST_SNPRINTF(("%lX", 0xabcdefabL), "ABCDEFAB");
    TEST_SNPRINTF(("%c", 'v'), "v");
    TEST_SNPRINTF(("%cv", 'w'), "wv");
    TEST_SNPRINTF(("%s", "A Test"), "A Test");
    TEST_SNPRINTF(("%hhu", (signed char)0xFFFFU), "255");
    TEST_SNPRINTF(("%hu", (unsigned short)0x123456U), "13398");
    TEST_SNPRINTF(("%s%hhi %hu", "Test", 10000, 0xFFFFFFFF), "Test16 65535");
    TEST_SNPRINTF(("%tx", &buffer[10] - &buffer[0]), "a");
    // TBD
    TEST_SNPRINTF(("%ji", (intmax_t)-2147483647L), "-2147483647");

    TEST_SNPRINTF(("%.*d", -1, 1), "1");

    #if TEST_IMPL_DEFINED
    cur_snprintf(buffer, sizeof(buffer), "%p", (void*)0x1234U);
    if (sizeof(void*) == 4U) {
        REQUIRE_STR_EQ(buffer, "00001234");
    } else {
        REQUIRE_STR_EQ(buffer, "0000000000001234");
    }

    cur_snprintf(buffer, sizeof(buffer), "%p", (void*)0x12345678U);
    if (sizeof(void*) == 4U) {
        REQUIRE_STR_EQ(buffer, "12345678");
    } else {
        REQUIRE_STR_EQ(buffer, "0000000012345678");
    }

    cur_snprintf(buffer, sizeof(buffer), "%p-%p", (void*)0x12345678U, (void*)0x7EDCBA98U);
    if (sizeof(void*) == 4U) {
        REQUIRE_STR_EQ(buffer, "12345678-7EDCBA98");
    } else {
        REQUIRE_STR_EQ(buffer, "0000000012345678-000000007EDCBA98");
    }

    if (sizeof(uintptr_t) == sizeof(uint64_t)) {
        cur_snprintf(buffer, sizeof(buffer), "%p", (void*)(uintptr_t)0xFFFFFFFFU);
        REQUIRE_STR_EQ(buffer, "00000000FFFFFFFF");
    } else {
        cur_snprintf(buffer, sizeof(buffer), "%p", (void*)(uintptr_t)0xFFFFFFFFU);
        REQUIRE_STR_EQ(buffer, "FFFFFFFF");
    }
    #endif

    buffer[0] = (char)0xA5;
    ret = cur_snprintf(buffer, 0, "%s", "Test");
    REQUIRE(buffer[0] == (char)0xA5);
    REQUIRE(ret == 4);

    buffer[0] = (char)0xCC;
    cur_snprintf(buffer, 1, "%s", "Test");
    REQUIRE(buffer[0] == '\0');

    cur_snprintf(buffer, 2, "%s", "Hello");
    REQUIRE_STR_EQ(buffer, "H");

    ret = cur_snprintf(buffer, 6, "0%s", "1234");
    REQUIRE_STR_EQ(buffer, "01234");
    REQUIRE(ret == 5);

    ret = cur_snprintf(buffer, 6, "0%s", "12345");
    REQUIRE_STR_EQ(buffer, "01234");
    REQUIRE(ret == 6);  // '5' is truncated

    ret = cur_snprintf(buffer, 6, "0%s", "1234567");
    REQUIRE_STR_EQ(buffer, "01234");
    REQUIRE(ret == 8);  // '567' are truncated

    ret = cur_snprintf(buffer, 10, "hello, world");
    REQUIRE(ret == 12);

    ret = cur_snprintf(buffer, 3, "%d", 10000);
    REQUIRE(ret == 5);
    REQUIRE(strlen(buffer) == 2U);
    REQUIRE(buffer[0] == '1');
    REQUIRE(buffer[1] == '0');
    REQUIRE(buffer[2] == '\0');

    TEST_SNPRINTF(("%u%u%ctest%d %s", 5, 3000, 'a', -20, "bit"), "53000atest-20 bit");
    TEST_SNPRINTF(("%.*f", 2, 0.33333333), "0.33");
    TEST_SNPRINTF(("%.3s", "foobar"), "foo");
    TEST_SNPRINTF(("%10.5d", 4), "     00004");
    TEST_SNPRINTF(("%*sx", -3, "hi"), "hi x");

    // libc-testsuite tests

    /* width, precision, alignment */
    TEST_SNPRINTF(("%04d", 12), "0012");
    TEST_SNPRINTF(("%.3d", 12), "012");
    TEST_SNPRINTF(("%3d", 12), " 12");
    TEST_SNPRINTF(("%-3d", 12), "12 ");
    TEST_SNPRINTF(("%+3d", 12), "+12");
    TEST_SNPRINTF(("%+-5d", 12), "+12  ");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%+- 5d", 12), "+12  ");
    #endif
    TEST_SNPRINTF(("%- 5d", 12), " 12  ");
    TEST_SNPRINTF(("% d", 12), " 12");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%0-5d", 12), "12   ");
    TEST_SNPRINTF(("%-05d", 12), "12   ");
    #endif

    /* ...explicit precision of 0 shall be no characters. */
    TEST_SNPRINTF(("%.0d", 0), "");
    TEST_SNPRINTF(("%.0o", 0), "");
    #if TEST_REDUNDANT_FLAGS
    TEST_SNPRINTF(("%#.0d", 0), "");
    #endif
    // Note: the original libc-testsuite specifies "" as expected.
    TEST_SNPRINTF(("%#.0o", 0), "0");
    TEST_SNPRINTF(("%#.0x", 0), "");

    /* ...but it still has to honor width and flags. */
    TEST_SNPRINTF(("%2.0u", 0), "  ");
    #if TEST_REDUNDANT_FLAGS
    // BUG: mpaland disagrees
    //TEST_SNPRINTF(("%02.0u", 0), "  ");
    //TEST_SNPRINTF(("%02.0d", 0), "  ");
    #endif
    TEST_SNPRINTF(("%2.0d", 0), "  ");
    TEST_SNPRINTF(("% .0d", 0), " ");
    TEST_SNPRINTF(("%+.0d", 0), "+");

    /* hex: test alt form and case */
    TEST_SNPRINTF(("%x", 63), "3f");
    TEST_SNPRINTF(("%#x", 63), "0x3f");
    TEST_SNPRINTF(("%X", 63), "3F");

    /* octal: test alt form */
    TEST_SNPRINTF(("%o", 15), "17");
    TEST_SNPRINTF(("%#o", 15), "017");

    /* basic form, handling of exponent/precision for 0 */
    TEST_SNPRINTF(("%e", 0.0), "0.000000e+00");
    TEST_SNPRINTF(("%f", 0.0), "0.000000");
    TEST_SNPRINTF(("%g", 0.0), "0");
    TEST_SNPRINTF(("%#g", 0.0), "0.00000");

    /* rounding */
    TEST_SNPRINTF(("%f", 1.1), "1.100000");
    TEST_SNPRINTF(("%f", 1.2), "1.200000");
    TEST_SNPRINTF(("%f", 1.3), "1.300000");
    TEST_SNPRINTF(("%f", 1.4), "1.400000");
    TEST_SNPRINTF(("%f", 1.5), "1.500000");
    // Note: the original libc-testsuite test specifies "1.0612" as expected.
    TEST_SNPRINTF(("%.4f", 1.06125), "1.0613");
    TEST_SNPRINTF(("%.2f", 1.375), "1.38");
    TEST_SNPRINTF(("%.1f", 1.375), "1.4");
    TEST_SNPRINTF(("%.15f", 1.1), "1.100000000000000");
    TEST_SNPRINTF(("%.16f", 1.1), "1.1000000000000001");
    TEST_SNPRINTF(("%.17f", 1.1), "1.10000000000000009");
    TEST_SNPRINTF(("%.2e", 1500001.0), "1.50e+06");
    TEST_SNPRINTF(("%.2e", 1505000.0), "1.50e+06");
    TEST_SNPRINTF(("%.2e", 1505000.00000095367431640625), "1.51e+06");
    TEST_SNPRINTF(("%.2e", 1505001.0), "1.51e+06");
    TEST_SNPRINTF(("%.2e", 1506000.0), "1.51e+06");

    /* correctness in DBL_DIG places */
    TEST_SNPRINTF(("%.15g", 1.23456789012345), "1.23456789012345");

    /* correct choice of notation for %g */
    TEST_SNPRINTF(("%g", 0.0001), "0.0001");
    TEST_SNPRINTF(("%g", 0.00001), "1e-05");
    TEST_SNPRINTF(("%g", 123456.0), "123456");
    TEST_SNPRINTF(("%g", 1234567.0), "1.23457e+06");
    TEST_SNPRINTF(("%.7g", 1234567.0), "1234567");
    TEST_SNPRINTF(("%.7g", 12345678.0), "1.234568e+07");
    TEST_SNPRINTF(("%.8g", 0.1), "0.1");
    TEST_SNPRINTF(("%.9g", 0.1), "0.1");
    TEST_SNPRINTF(("%.10g", 0.1), "0.1");
    TEST_SNPRINTF(("%.11g", 0.1), "0.1");

    /* pi in double precision, printed to a few extra places */
    TEST_SNPRINTF(("%.15f", M_PI), "3.141592653589793");
    TEST_SNPRINTF(("%.18f", M_PI), "3.141592653589793116");

    /* exact conversion of large integers */
    TEST_SNPRINTF(("%.0f", 340282366920938463463374607431768211456.0),
                    "340282366920938463463374607431768211456");

    TEST_SNPRINTF_N(("%d", 123456), 6);
    TEST_SNPRINTF_N(("%.4s", "hello"), 4);
    TEST_SNPRINTF_N(("%.0s", "goodbye"), 0);

    {
        char b[] = "xxxxxxxx";
        char *s = "%d";
        int res = snprintf(b, 4, s, 123456);
        REQUIRE(res == 6);
        REQUIRE_STR_EQ(b, "123");
        REQUIRE(b[5] == 'x'); // buffer overrun
    }

    {
        char b[2000];
        /* Perform ascii arithmetic to test printing tiny doubles */
        int res = snprintf(b, sizeof(b), "%.1022f", 0x1p-1021);
        REQUIRE(res == 1024);
        b[1] = '0';
        int i, k, j;
        for (i=0; i<1021; i++) {
            for (k=0, j=1023; j>0; j--) {
                if (b[j]<'5') b[j]+=b[j]-'0'+k, k=0;
                else b[j]+=b[j]-'0'-10+k, k=1;
            }
        }
        REQUIRE(b[1] == '1');
        for (j=2; b[j]=='0'; j++);
            REQUIRE(j == 1024);
    }

    // TODO
    //TEST_SNPRINTF_N(("%.*u", 2147483647, 0), 2147483647);
    //TEST_SNPRINTF_N(("%.*u ", 2147483647, 0), -1);

    TEST_SNPRINTF_N(("%.4a", 1.0), 11);

    printf("All tests succeeded.\n");
}

int main()
{
    #if !TEST_NON_STANDARD && !TEST_IMPL_DEFINED
    printf("Testing system snprintf...\n");
    run_test(snprintf);
    #else
    printf("_Not_ testing system snprintf!\n");
    #endif

    printf("Testing libinsanity snprintf...\n");
    run_test(lin_snprintf);

    return 0;
}