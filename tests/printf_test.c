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
// \brief printf unit tests
//
///////////////////////////////////////////////////////////////////////////////

#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "printf.h"

#define REQUIRE(x) assert(x)

// Some tests use redundant format specifier flags (against which gcc warns,
// and which OS libc will usually not support).
#define TEST_NON_STANDARD 0

// Tests redundant flags, against which gcc warns. It's possible that testing
// this would be worth anyway, because the standard does not forbid passing
// them. Normally they just cause noise with -Wformat (which you should always
// enable).
#define TEST_REDUNDANT_FLAGS 0

static void vsnprintf_builder_1(char* buffer, int size, ...)
{
  va_list args;
  va_start(args, size);
  lin_vsnprintf(buffer, size, "%d", args);
  va_end(args);
}

static void vsnprintf_builder_3(char* buffer, int size, ...)
{
  va_list args;
  va_start(args, size);
  lin_vsnprintf(buffer, size, "%d %d %s", args);
  va_end(args);
}

int main()
{
  char buffer[100];
  int ret;

  lin_snprintf(buffer, sizeof(buffer), "%d", -1000);
  REQUIRE(!strcmp(buffer, "-1000"));

  lin_snprintf(buffer, 3U, "%d", -1000);
  REQUIRE(!strcmp(buffer, "-1"));

  vsnprintf_builder_1(buffer, sizeof(buffer), -1);
  REQUIRE(!strcmp(buffer, "-1"));

  vsnprintf_builder_3(buffer, sizeof(buffer), 3, -1000, "test");
  REQUIRE(!strcmp(buffer, "3 -1000 test"));

  lin_snprintf(buffer, sizeof(buffer), "% d", 42);
  REQUIRE(!strcmp(buffer, " 42"));

  lin_snprintf(buffer, sizeof(buffer), "% d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "% 5d", 42);
  REQUIRE(!strcmp(buffer, "   42"));

  lin_snprintf(buffer, sizeof(buffer), "% 5d", -42);
  REQUIRE(!strcmp(buffer, "  -42"));

  lin_snprintf(buffer, sizeof(buffer), "% 15d", 42);
  REQUIRE(!strcmp(buffer, "             42"));

  lin_snprintf(buffer, sizeof(buffer), "% 15d", -42);
  REQUIRE(!strcmp(buffer, "            -42"));

  lin_snprintf(buffer, sizeof(buffer), "% 15d", -42);
  REQUIRE(!strcmp(buffer, "            -42"));

  lin_snprintf(buffer, sizeof(buffer), "% 15.3f", -42.987);
  REQUIRE(!strcmp(buffer, "        -42.987"));

  lin_snprintf(buffer, sizeof(buffer), "% 15.3f", 42.987);
  REQUIRE(!strcmp(buffer, "         42.987"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "% s", "Hello testing");
  REQUIRE(!strcmp(buffer, "Hello testing"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "% d", 1024);
  REQUIRE(!strcmp(buffer, " 1024"));

  lin_snprintf(buffer, sizeof(buffer), "% d", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "% i", 1024);
  REQUIRE(!strcmp(buffer, " 1024"));

  lin_snprintf(buffer, sizeof(buffer), "% i", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "% u", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "% u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "% o", 511);
  REQUIRE(!strcmp(buffer, "777"));

  lin_snprintf(buffer, sizeof(buffer), "% o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "% x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "% x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "% X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "% X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "% c", 'x');
  REQUIRE(!strcmp(buffer, "x"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%+d", 42);
  REQUIRE(!strcmp(buffer, "+42"));

  lin_snprintf(buffer, sizeof(buffer), "%+d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "%+5d", 42);
  REQUIRE(!strcmp(buffer, "  +42"));

  lin_snprintf(buffer, sizeof(buffer), "%+5d", -42);
  REQUIRE(!strcmp(buffer, "  -42"));

  lin_snprintf(buffer, sizeof(buffer), "%+15d", 42);
  REQUIRE(!strcmp(buffer, "            +42"));

  lin_snprintf(buffer, sizeof(buffer), "%+15d", -42);
  REQUIRE(!strcmp(buffer, "            -42"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%+s", "Hello testing");
  REQUIRE(!strcmp(buffer, "Hello testing"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%+d", 1024);
  REQUIRE(!strcmp(buffer, "+1024"));

  lin_snprintf(buffer, sizeof(buffer), "%+d", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "%+i", 1024);
  REQUIRE(!strcmp(buffer, "+1024"));

  lin_snprintf(buffer, sizeof(buffer), "%+i", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%+u", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%+u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%+o", 511);
  REQUIRE(!strcmp(buffer, "777"));

  lin_snprintf(buffer, sizeof(buffer), "%+o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%+x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%+x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%+X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%+X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%+c", 'x');
  REQUIRE(!strcmp(buffer, "x"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%0d", 42);
  REQUIRE(!strcmp(buffer, "42"));

  lin_snprintf(buffer, sizeof(buffer), "%0ld", 42L);
  REQUIRE(!strcmp(buffer, "42"));

  lin_snprintf(buffer, sizeof(buffer), "%0d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "%05d", 42);
  REQUIRE(!strcmp(buffer, "00042"));

  lin_snprintf(buffer, sizeof(buffer), "%05d", -42);
  REQUIRE(!strcmp(buffer, "-0042"));

  lin_snprintf(buffer, sizeof(buffer), "%015d", 42);
  REQUIRE(!strcmp(buffer, "000000000000042"));

  lin_snprintf(buffer, sizeof(buffer), "%015d", -42);
  REQUIRE(!strcmp(buffer, "-00000000000042"));

  lin_snprintf(buffer, sizeof(buffer), "%015.2f", 42.1234);
  REQUIRE(!strcmp(buffer, "000000000042.12"));

  lin_snprintf(buffer, sizeof(buffer), "%015.3f", 42.9876);
  REQUIRE(!strcmp(buffer, "00000000042.988"));

  lin_snprintf(buffer, sizeof(buffer), "%015.5f", -42.9876);
  REQUIRE(!strcmp(buffer, "-00000042.98760"));

  lin_snprintf(buffer, sizeof(buffer), "%-d", 42);
  REQUIRE(!strcmp(buffer, "42"));

  lin_snprintf(buffer, sizeof(buffer), "%-d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "%-5d", 42);
  REQUIRE(!strcmp(buffer, "42   "));

  lin_snprintf(buffer, sizeof(buffer), "%-5d", -42);
  REQUIRE(!strcmp(buffer, "-42  "));

  lin_snprintf(buffer, sizeof(buffer), "%-15d", 42);
  REQUIRE(!strcmp(buffer, "42             "));

  lin_snprintf(buffer, sizeof(buffer), "%-15d", -42);
  REQUIRE(!strcmp(buffer, "-42            "));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%-0d", 42);
  REQUIRE(!strcmp(buffer, "42"));

  lin_snprintf(buffer, sizeof(buffer), "%-0d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "%-05d", 42);
  REQUIRE(!strcmp(buffer, "42   "));

  lin_snprintf(buffer, sizeof(buffer), "%-05d", -42);
  REQUIRE(!strcmp(buffer, "-42  "));

  lin_snprintf(buffer, sizeof(buffer), "%-015d", 42);
  REQUIRE(!strcmp(buffer, "42             "));

  lin_snprintf(buffer, sizeof(buffer), "%-015d", -42);
  REQUIRE(!strcmp(buffer, "-42            "));

  lin_snprintf(buffer, sizeof(buffer), "%0-d", 42);
  REQUIRE(!strcmp(buffer, "42"));

  lin_snprintf(buffer, sizeof(buffer), "%0-d", -42);
  REQUIRE(!strcmp(buffer, "-42"));

  lin_snprintf(buffer, sizeof(buffer), "%0-5d", 42);
  REQUIRE(!strcmp(buffer, "42   "));

  lin_snprintf(buffer, sizeof(buffer), "%0-5d", -42);
  REQUIRE(!strcmp(buffer, "-42  "));

  lin_snprintf(buffer, sizeof(buffer), "%0-15d", 42);
  REQUIRE(!strcmp(buffer, "42             "));

  lin_snprintf(buffer, sizeof(buffer), "%0-15d", -42);
  REQUIRE(!strcmp(buffer, "-42            "));
#endif

  lin_snprintf(buffer, sizeof(buffer), "Hello testing");
  REQUIRE(!strcmp(buffer, "Hello testing"));

  lin_snprintf(buffer, sizeof(buffer), "%s", "Hello testing");
  REQUIRE(!strcmp(buffer, "Hello testing"));

  lin_snprintf(buffer, sizeof(buffer), "%d", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%d", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "%i", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%i", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "%u", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%o", 511);
  REQUIRE(!strcmp(buffer, "777"));

  lin_snprintf(buffer, sizeof(buffer), "%o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%%");
  REQUIRE(!strcmp(buffer, "%"));

  lin_snprintf(buffer, sizeof(buffer), "%1s", "Hello testing");
  REQUIRE(!strcmp(buffer, "Hello testing"));

  lin_snprintf(buffer, sizeof(buffer), "%1d", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%1d", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "%1i", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%1i", -1024);
  REQUIRE(!strcmp(buffer, "-1024"));

  lin_snprintf(buffer, sizeof(buffer), "%1u", 1024);
  REQUIRE(!strcmp(buffer, "1024"));

  lin_snprintf(buffer, sizeof(buffer), "%1u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%1o", 511);
  REQUIRE(!strcmp(buffer, "777"));

  lin_snprintf(buffer, sizeof(buffer), "%1o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%1x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%1x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%1X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%1X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%1c", 'x');
  REQUIRE(!strcmp(buffer, "x"));

  lin_snprintf(buffer, sizeof(buffer), "%20s", "Hello");
  REQUIRE(!strcmp(buffer, "               Hello"));

  lin_snprintf(buffer, sizeof(buffer), "%20d", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20d", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20i", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20i", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20u", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20u", 4294966272U);
  REQUIRE(!strcmp(buffer, "          4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%20o", 511);
  REQUIRE(!strcmp(buffer, "                 777"));

  lin_snprintf(buffer, sizeof(buffer), "%20o", 4294966785U);
  REQUIRE(!strcmp(buffer, "         37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%20x", 305441741);
  REQUIRE(!strcmp(buffer, "            1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%20x", 3989525555U);
  REQUIRE(!strcmp(buffer, "            edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20X", 305441741);
  REQUIRE(!strcmp(buffer, "            1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%20X", 3989525555U);
  REQUIRE(!strcmp(buffer, "            EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20c", 'x');
  REQUIRE(!strcmp(buffer, "                   x"));

  lin_snprintf(buffer, sizeof(buffer), "%*s", 20, "Hello");
  REQUIRE(!strcmp(buffer, "               Hello"));

  lin_snprintf(buffer, sizeof(buffer), "%*d", 20, 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%*d", 20, -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%*i", 20, 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%*i", 20, -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%*u", 20, 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%*u", 20, 4294966272U);
  REQUIRE(!strcmp(buffer, "          4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%*o", 20, 511);
  REQUIRE(!strcmp(buffer, "                 777"));

  lin_snprintf(buffer, sizeof(buffer), "%*o", 20, 4294966785U);
  REQUIRE(!strcmp(buffer, "         37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%*x", 20, 305441741);
  REQUIRE(!strcmp(buffer, "            1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%*x", 20, 3989525555U);
  REQUIRE(!strcmp(buffer, "            edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%*X", 20, 305441741);
  REQUIRE(!strcmp(buffer, "            1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%*X", 20, 3989525555U);
  REQUIRE(!strcmp(buffer, "            EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%*c", 20,'x');
  REQUIRE(!strcmp(buffer, "                   x"));

  lin_snprintf(buffer, sizeof(buffer), "%-20s", "Hello");
  REQUIRE(!strcmp(buffer, "Hello               "));

  lin_snprintf(buffer, sizeof(buffer), "%-20d", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%-20d", -1024);
  REQUIRE(!strcmp(buffer, "-1024               "));

  lin_snprintf(buffer, sizeof(buffer), "%-20i", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%-20i", -1024);
  REQUIRE(!strcmp(buffer, "-1024               "));

  lin_snprintf(buffer, sizeof(buffer), "%-20u", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%-20.4f", 1024.1234);
  REQUIRE(!strcmp(buffer, "1024.1234           "));

  lin_snprintf(buffer, sizeof(buffer), "%-20u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272          "));

  lin_snprintf(buffer, sizeof(buffer), "%-20o", 511);
  REQUIRE(!strcmp(buffer, "777                 "));

  lin_snprintf(buffer, sizeof(buffer), "%-20o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001         "));

  lin_snprintf(buffer, sizeof(buffer), "%-20x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd            "));

  lin_snprintf(buffer, sizeof(buffer), "%-20x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433            "));

  lin_snprintf(buffer, sizeof(buffer), "%-20X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD            "));

  lin_snprintf(buffer, sizeof(buffer), "%-20X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433            "));

  lin_snprintf(buffer, sizeof(buffer), "%-20c", 'x');
  REQUIRE(!strcmp(buffer, "x                   "));

  lin_snprintf(buffer, sizeof(buffer), "|%5d| |%-2d| |%5d|", 9, 9, 9);
  REQUIRE(!strcmp(buffer, "|    9| |9 | |    9|"));

  lin_snprintf(buffer, sizeof(buffer), "|%5d| |%-2d| |%5d|", 10, 10, 10);
  REQUIRE(!strcmp(buffer, "|   10| |10| |   10|"));

  lin_snprintf(buffer, sizeof(buffer), "|%5d| |%-12d| |%5d|", 9, 9, 9);
  REQUIRE(!strcmp(buffer, "|    9| |9           | |    9|"));

  lin_snprintf(buffer, sizeof(buffer), "|%5d| |%-12d| |%5d|", 10, 10, 10);
  REQUIRE(!strcmp(buffer, "|   10| |10          | |   10|"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%0-20s", "Hello");
  REQUIRE(!strcmp(buffer, "Hello               "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20d", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20d", -1024);
  REQUIRE(!strcmp(buffer, "-1024               "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20i", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20i", -1024);
  REQUIRE(!strcmp(buffer, "-1024               "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20u", 1024);
  REQUIRE(!strcmp(buffer, "1024                "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20u", 4294966272U);
  REQUIRE(!strcmp(buffer, "4294966272          "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20o", 511);
  REQUIRE(!strcmp(buffer, "777                 "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20o", 4294966785U);
  REQUIRE(!strcmp(buffer, "37777777001         "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20x", 305441741);
  REQUIRE(!strcmp(buffer, "1234abcd            "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20x", 3989525555U);
  REQUIRE(!strcmp(buffer, "edcb5433            "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20X", 305441741);
  REQUIRE(!strcmp(buffer, "1234ABCD            "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20X", 3989525555U);
  REQUIRE(!strcmp(buffer, "EDCB5433            "));

  lin_snprintf(buffer, sizeof(buffer), "%0-20c", 'x');
  REQUIRE(!strcmp(buffer, "x                   "));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%020d", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%020d", -1024);
  REQUIRE(!strcmp(buffer, "-0000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%020i", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%020i", -1024);
  REQUIRE(!strcmp(buffer, "-0000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%020u", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%020u", 4294966272U);
  REQUIRE(!strcmp(buffer, "00000000004294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%020o", 511);
  REQUIRE(!strcmp(buffer, "00000000000000000777"));

  lin_snprintf(buffer, sizeof(buffer), "%020o", 4294966785U);
  REQUIRE(!strcmp(buffer, "00000000037777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%020x", 305441741);
  REQUIRE(!strcmp(buffer, "0000000000001234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%020x", 3989525555U);
  REQUIRE(!strcmp(buffer, "000000000000edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%020X", 305441741);
  REQUIRE(!strcmp(buffer, "0000000000001234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%020X", 3989525555U);
  REQUIRE(!strcmp(buffer, "000000000000EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%.20d", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%.20d", -1024);
  REQUIRE(!strcmp(buffer, "-00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%.20i", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%.20i", -1024);
  REQUIRE(!strcmp(buffer, "-00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%.20u", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%.20u", 4294966272U);
  REQUIRE(!strcmp(buffer, "00000000004294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%.20o", 511);
  REQUIRE(!strcmp(buffer, "00000000000000000777"));

  lin_snprintf(buffer, sizeof(buffer), "%.20o", 4294966785U);
  REQUIRE(!strcmp(buffer, "00000000037777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%.20x", 305441741);
  REQUIRE(!strcmp(buffer, "0000000000001234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%.20x", 3989525555U);
  REQUIRE(!strcmp(buffer, "000000000000edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%.20X", 305441741);
  REQUIRE(!strcmp(buffer, "0000000000001234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%.20X", 3989525555U);
  REQUIRE(!strcmp(buffer, "000000000000EDCB5433"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%#020d", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%#020d", -1024);
  REQUIRE(!strcmp(buffer, "-0000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%#020i", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%#020i", -1024);
  REQUIRE(!strcmp(buffer, "-0000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%#020u", 1024);
  REQUIRE(!strcmp(buffer, "00000000000000001024"));

  lin_snprintf(buffer, sizeof(buffer), "%#020u", 4294966272U);
  REQUIRE(!strcmp(buffer, "00000000004294966272"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%#020o", 511);
  REQUIRE(!strcmp(buffer, "00000000000000000777"));

  lin_snprintf(buffer, sizeof(buffer), "%#020o", 4294966785U);
  REQUIRE(!strcmp(buffer, "00000000037777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%#020x", 305441741);
  REQUIRE(!strcmp(buffer, "0x00000000001234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%#020x", 3989525555U);
  REQUIRE(!strcmp(buffer, "0x0000000000edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%#020X", 305441741);
  REQUIRE(!strcmp(buffer, "0X00000000001234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%#020X", 3989525555U);
  REQUIRE(!strcmp(buffer, "0X0000000000EDCB5433"));

#if TEST_REDUNDANT_FLAGS
  lin_snprintf(buffer, sizeof(buffer), "%#20d", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%#20d", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%#20i", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%#20i", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%#20u", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%#20u", 4294966272U);
  REQUIRE(!strcmp(buffer, "          4294966272"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%#20o", 511);
  REQUIRE(!strcmp(buffer, "                0777"));

  lin_snprintf(buffer, sizeof(buffer), "%#20o", 4294966785U);
  REQUIRE(!strcmp(buffer, "        037777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%#20x", 305441741);
  REQUIRE(!strcmp(buffer, "          0x1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%#20x", 3989525555U);
  REQUIRE(!strcmp(buffer, "          0xedcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%#20X", 305441741);
  REQUIRE(!strcmp(buffer, "          0X1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%#20X", 3989525555U);
  REQUIRE(!strcmp(buffer, "          0XEDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5d", 1024);
  REQUIRE(!strcmp(buffer, "               01024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5d", -1024);
  REQUIRE(!strcmp(buffer, "              -01024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5i", 1024);
  REQUIRE(!strcmp(buffer, "               01024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5i", -1024);
  REQUIRE(!strcmp(buffer, "              -01024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5u", 1024);
  REQUIRE(!strcmp(buffer, "               01024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5u", 4294966272U);
  REQUIRE(!strcmp(buffer, "          4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5o", 511);
  REQUIRE(!strcmp(buffer, "               00777"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5o", 4294966785U);
  REQUIRE(!strcmp(buffer, "         37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5x", 305441741);
  REQUIRE(!strcmp(buffer, "            1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%20.10x", 3989525555U);
  REQUIRE(!strcmp(buffer, "          00edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20.5X", 305441741);
  REQUIRE(!strcmp(buffer, "            1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%20.10X", 3989525555U);
  REQUIRE(!strcmp(buffer, "          00EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%.0s", "Hello testing");
  REQUIRE(!strcmp(buffer, ""));

  lin_snprintf(buffer, sizeof(buffer), "%20.0s", "Hello testing");
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%.s", "Hello testing");
  REQUIRE(!strcmp(buffer, ""));

  lin_snprintf(buffer, sizeof(buffer), "%20.s", "Hello testing");
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.0d", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.0d", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.d", 0);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.0i", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.i", -1024);
  REQUIRE(!strcmp(buffer, "               -1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.i", 0);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.u", 1024);
  REQUIRE(!strcmp(buffer, "                1024"));

  lin_snprintf(buffer, sizeof(buffer), "%20.0u", 4294966272U);
  REQUIRE(!strcmp(buffer, "          4294966272"));

  lin_snprintf(buffer, sizeof(buffer), "%20.u", 0U);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.o", 511);
  REQUIRE(!strcmp(buffer, "                 777"));

  lin_snprintf(buffer, sizeof(buffer), "%20.0o", 4294966785U);
  REQUIRE(!strcmp(buffer, "         37777777001"));

  lin_snprintf(buffer, sizeof(buffer), "%20.o", 0U);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.x", 305441741);
  REQUIRE(!strcmp(buffer, "            1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%50.x", 305441741);
  REQUIRE(!strcmp(buffer, "                                          1234abcd"));

  lin_snprintf(buffer, sizeof(buffer), "%50.x%10.u", 305441741, 12345);
  REQUIRE(!strcmp(buffer, "                                          1234abcd     12345"));

  lin_snprintf(buffer, sizeof(buffer), "%20.0x", 3989525555U);
  REQUIRE(!strcmp(buffer, "            edcb5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20.x", 0U);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%20.X", 305441741);
  REQUIRE(!strcmp(buffer, "            1234ABCD"));

  lin_snprintf(buffer, sizeof(buffer), "%20.0X", 3989525555U);
  REQUIRE(!strcmp(buffer, "            EDCB5433"));

  lin_snprintf(buffer, sizeof(buffer), "%20.X", 0U);
  REQUIRE(!strcmp(buffer, "                    "));

  lin_snprintf(buffer, sizeof(buffer), "%.4f", 3.1415354);
  REQUIRE(!strcmp(buffer, "3.1415"));

  lin_snprintf(buffer, sizeof(buffer), "%.3f", 30343.1415354);
  REQUIRE(!strcmp(buffer, "30343.142"));

  lin_snprintf(buffer, sizeof(buffer), "%.0f", 34.1415354);
  REQUIRE(!strcmp(buffer, "34"));

  lin_snprintf(buffer, sizeof(buffer), "%.2f", 42.8952);
  REQUIRE(!strcmp(buffer, "42.90"));

  lin_snprintf(buffer, sizeof(buffer), "%.9f", 42.8952);
  REQUIRE(!strcmp(buffer, "42.895200000"));

  lin_snprintf(buffer, sizeof(buffer), "%.10f", 42.895223);
  REQUIRE(!strcmp(buffer, "42.8952230000"));

  // this testcase checks, that the precision is truncated to 9 digits.
  // a perfect working float should return the whole number
  lin_snprintf(buffer, sizeof(buffer), "%.12f", 42.89522312345678);
  REQUIRE(!strcmp(buffer, "42.895223123000"));

  // this testcase checks, that the precision is truncated AND rounded to 9 digits.
  // a perfect working float should return the whole number
  lin_snprintf(buffer, sizeof(buffer), "%.12f", 42.89522387654321);
  REQUIRE(!strcmp(buffer, "42.895223877000"));

  lin_snprintf(buffer, sizeof(buffer), "%6.2f", 42.8952);
  REQUIRE(!strcmp(buffer, " 42.90"));

  lin_snprintf(buffer, sizeof(buffer), "%+6.2f", 42.8952);
  REQUIRE(!strcmp(buffer, "+42.90"));

  lin_snprintf(buffer, sizeof(buffer), "%+5.1f", 42.9252);
  REQUIRE(!strcmp(buffer, "+42.9"));

  lin_snprintf(buffer, sizeof(buffer), "%f", 42.5);
  REQUIRE(!strcmp(buffer, "42.500000"));

  lin_snprintf(buffer, sizeof(buffer), "%.1f", 42.5);
  REQUIRE(!strcmp(buffer, "42.5"));

  lin_snprintf(buffer, sizeof(buffer), "%f", 42167.0);
  REQUIRE(!strcmp(buffer, "42167.000000"));

  lin_snprintf(buffer, sizeof(buffer), "%.9f", -12345.987654321);
  REQUIRE(!strcmp(buffer, "-12345.987654321"));

  lin_snprintf(buffer, sizeof(buffer), "%.1f", 3.999);
  REQUIRE(!strcmp(buffer, "4.0"));

  lin_snprintf(buffer, sizeof(buffer), "%.0f", 3.5);
  REQUIRE(!strcmp(buffer, "4"));

  lin_snprintf(buffer, sizeof(buffer), "%.0f", 3.49);
  REQUIRE(!strcmp(buffer, "3"));

  lin_snprintf(buffer, sizeof(buffer), "%.1f", 3.49);
  REQUIRE(!strcmp(buffer, "3.5"));

  // out of range in the moment, need to be fixed by someone
  lin_snprintf(buffer, sizeof(buffer), "%.1f", 1E20);
  REQUIRE(!strcmp(buffer, ""));

  lin_snprintf(buffer, sizeof(buffer), "%i", 0);
  REQUIRE(!strcmp(buffer, "0"));

  lin_snprintf(buffer, sizeof(buffer), "%i", 1234);
  REQUIRE(!strcmp(buffer, "1234"));

  lin_snprintf(buffer, sizeof(buffer), "%i", 32767);
  REQUIRE(!strcmp(buffer, "32767"));

  lin_snprintf(buffer, sizeof(buffer), "%i", -32767);
  REQUIRE(!strcmp(buffer, "-32767"));

  lin_snprintf(buffer, sizeof(buffer), "%li", 30L);
  REQUIRE(!strcmp(buffer, "30"));

  lin_snprintf(buffer, sizeof(buffer), "%li", -2147483647L);
  REQUIRE(!strcmp(buffer, "-2147483647"));

  lin_snprintf(buffer, sizeof(buffer), "%li", 2147483647L);
  REQUIRE(!strcmp(buffer, "2147483647"));

  lin_snprintf(buffer, sizeof(buffer), "%lli", 30LL);
  REQUIRE(!strcmp(buffer, "30"));

  lin_snprintf(buffer, sizeof(buffer), "%lli", -9223372036854775807LL);
  REQUIRE(!strcmp(buffer, "-9223372036854775807"));

  lin_snprintf(buffer, sizeof(buffer), "%lli", 9223372036854775807LL);
  REQUIRE(!strcmp(buffer, "9223372036854775807"));

  lin_snprintf(buffer, sizeof(buffer), "%lu", 100000L);
  REQUIRE(!strcmp(buffer, "100000"));

  lin_snprintf(buffer, sizeof(buffer), "%lu", 0xFFFFFFFFL);
  REQUIRE(!strcmp(buffer, "4294967295"));

  lin_snprintf(buffer, sizeof(buffer), "%llu", 281474976710656LLU);
  REQUIRE(!strcmp(buffer, "281474976710656"));

  lin_snprintf(buffer, sizeof(buffer), "%llu", 18446744073709551615LLU);
  REQUIRE(!strcmp(buffer, "18446744073709551615"));

  lin_snprintf(buffer, sizeof(buffer), "%zu", 2147483647UL);
  REQUIRE(!strcmp(buffer, "2147483647"));

  lin_snprintf(buffer, sizeof(buffer), "%zd", 2147483647UL);
  REQUIRE(!strcmp(buffer, "2147483647"));

  lin_snprintf(buffer, sizeof(buffer), "%zi", (ptrdiff_t)-2147483647LL);
  REQUIRE(!strcmp(buffer, "-2147483647"));

#if TEST_NON_STANDARD
  // Unportable extension in original printf implementation.
  lin_snprintf(buffer, sizeof(buffer), "%b", 60000);
  REQUIRE(!strcmp(buffer, "1110101001100000"));

  // Unportable extension in original printf implementation.
  lin_snprintf(buffer, sizeof(buffer), "%lb", 12345678L);
  REQUIRE(!strcmp(buffer, "101111000110000101001110"));
#endif

  lin_snprintf(buffer, sizeof(buffer), "%o", 60000);
  REQUIRE(!strcmp(buffer, "165140"));

  lin_snprintf(buffer, sizeof(buffer), "%lo", 12345678L);
  REQUIRE(!strcmp(buffer, "57060516"));

  lin_snprintf(buffer, sizeof(buffer), "%lx", 0x12345678L);
  REQUIRE(!strcmp(buffer, "12345678"));

  lin_snprintf(buffer, sizeof(buffer), "%llx", 0x1234567891234567LLU);
  REQUIRE(!strcmp(buffer, "1234567891234567"));

  lin_snprintf(buffer, sizeof(buffer), "%lx", 0xabcdefabL);
  REQUIRE(!strcmp(buffer, "abcdefab"));

  lin_snprintf(buffer, sizeof(buffer), "%lX", 0xabcdefabL);
  REQUIRE(!strcmp(buffer, "ABCDEFAB"));

  lin_snprintf(buffer, sizeof(buffer), "%c", 'v');
  REQUIRE(!strcmp(buffer, "v"));

  lin_snprintf(buffer, sizeof(buffer), "%cv", 'w');
  REQUIRE(!strcmp(buffer, "wv"));

  lin_snprintf(buffer, sizeof(buffer), "%s", "A Test");
  REQUIRE(!strcmp(buffer, "A Test"));

  lin_snprintf(buffer, sizeof(buffer), "%hhu", (signed char)0xFFFFU);
  REQUIRE(!strcmp(buffer, "255"));

  lin_snprintf(buffer, sizeof(buffer), "%hu", (unsigned short)0x123456U);
  REQUIRE(!strcmp(buffer, "13398"));

  lin_snprintf(buffer, sizeof(buffer), "%s%hhi %hu", "Test", 10000, 0xFFFFFFFF);
  REQUIRE(!strcmp(buffer, "Test16 65535"));

  lin_snprintf(buffer, sizeof(buffer), "%tx", &buffer[10] - &buffer[0]);
  REQUIRE(!strcmp(buffer, "a"));

  // TBD
  lin_snprintf(buffer, sizeof(buffer), "%ji", (intmax_t)-2147483647L);
  REQUIRE(!strcmp(buffer, "-2147483647"));

  lin_snprintf(buffer, sizeof(buffer), "%p", (void*)0x1234U);
  if (sizeof(void*) == 4U) {
    REQUIRE(!strcmp(buffer, "00001234"));
  }
  else {
    REQUIRE(!strcmp(buffer, "0000000000001234"));
  }

  lin_snprintf(buffer, sizeof(buffer), "%p", (void*)0x12345678U);
  if (sizeof(void*) == 4U) {
    REQUIRE(!strcmp(buffer, "12345678"));
  }
  else {
    REQUIRE(!strcmp(buffer, "0000000012345678"));
  }

  lin_snprintf(buffer, sizeof(buffer), "%p-%p", (void*)0x12345678U, (void*)0x7EDCBA98U);
  if (sizeof(void*) == 4U) {
    REQUIRE(!strcmp(buffer, "12345678-7EDCBA98"));
  }
  else {
    REQUIRE(!strcmp(buffer, "0000000012345678-000000007EDCBA98"));
  }

  if (sizeof(uintptr_t) == sizeof(uint64_t)) {
    lin_snprintf(buffer, sizeof(buffer), "%p", (void*)(uintptr_t)0xFFFFFFFFU);
    REQUIRE(!strcmp(buffer, "00000000FFFFFFFF"));
  }
  else {
    lin_snprintf(buffer, sizeof(buffer), "%p", (void*)(uintptr_t)0xFFFFFFFFU);
    REQUIRE(!strcmp(buffer, "FFFFFFFF"));
  }

  ret = lin_snprintf(NULL, 10, "%s", "Test");
  REQUIRE(ret == 4);
  ret = lin_snprintf(NULL, 0, "%s", "Test");
  REQUIRE(ret == 4);

  buffer[0] = (char)0xA5;
  ret = lin_snprintf(buffer, 0, "%s", "Test");
  REQUIRE(buffer[0] == (char)0xA5);
  REQUIRE(ret == 4);

  buffer[0] = (char)0xCC;
  lin_snprintf(buffer, 1, "%s", "Test");
  REQUIRE(buffer[0] == '\0');

  lin_snprintf(buffer, 2, "%s", "Hello");
  REQUIRE(!strcmp(buffer, "H"));

  ret = lin_snprintf(buffer, 6, "0%s", "1234");
  REQUIRE(!strcmp(buffer, "01234"));
  REQUIRE(ret == 5);

  ret = lin_snprintf(buffer, 6, "0%s", "12345");
  REQUIRE(!strcmp(buffer, "01234"));
  REQUIRE(ret == 6);  // '5' is truncated

  ret = lin_snprintf(buffer, 6, "0%s", "1234567");
  REQUIRE(!strcmp(buffer, "01234"));
  REQUIRE(ret == 8);  // '567' are truncated

  ret = lin_snprintf(buffer, 10, "hello, world");
  REQUIRE(ret == 12);

  ret = lin_snprintf(buffer, 3, "%d", 10000);
  REQUIRE(ret == 5);
  REQUIRE(strlen(buffer) == 2U);
  REQUIRE(buffer[0] == '1');
  REQUIRE(buffer[1] == '0');
  REQUIRE(buffer[2] == '\0');

  lin_snprintf(buffer, sizeof(buffer), "%u%u%ctest%d %s", 5, 3000, 'a', -20, "bit");
  REQUIRE(!strcmp(buffer, "53000atest-20 bit"));

  lin_snprintf(buffer, sizeof(buffer), "%.*f", 2, 0.33333333);
  REQUIRE(!strcmp(buffer, "0.33"));

  lin_snprintf(buffer, sizeof(buffer), "%.3s", "foobar");
  REQUIRE(!strcmp(buffer, "foo"));

  lin_snprintf(buffer, sizeof(buffer), "%10.5d", 4);
  REQUIRE(!strcmp(buffer, "     00004"));

  lin_snprintf(buffer, sizeof(buffer), "%*sx", -3, "hi");
  REQUIRE(!strcmp(buffer, "hi x"));

  printf("All tests succeeded.\n");
  return 0;
}
