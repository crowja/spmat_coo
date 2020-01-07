#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "spmat_coo.h"
#include "t/tinytest.h"

#ifdef  _COLOR_CODE
#undef  _COLOR_CODE
#endif
#define _COLOR_CODE       0x1B

#ifdef  _COLOR_RED
#undef  _COLOR_RED
#endif
#define _COLOR_RED        "[1;31m"

#ifdef  _COLOR_GREEN
#undef  _COLOR_GREEN
#endif
#define _COLOR_GREEN      "[1;32m"

#ifdef  _COLOR_YELLOW
#undef  _COLOR_YELLOW
#endif
#define _COLOR_YELLOW     "[1;33m"

#ifdef  _COLOR_RESET
#undef  _COLOR_RESET
#endif
#define _COLOR_RESET      "[0m"

static void
_printf_test_name(char *name, char *info)
{
   printf("%c%s%s%c%s", _COLOR_CODE, _COLOR_YELLOW, name, _COLOR_CODE, _COLOR_RESET);

   if (NULL != info)
      printf(" [%s]\n", info);
   else
      printf("\n");
}

static int
_two_doubles_equal(double x, double y)
{
   double      t = fabs(x) + fabs(y);
   return fabs(x - y) < 4 * DBL_EPSILON * t ? 1 : 0;
}

static void
test_constr(void)
{
   struct spmat_coo *z;

   _printf_test_name("test_constr()", "spmat_coo_new, spmat_coo_free");

   z = spmat_coo_new();
   ASSERT("Constructor test", z);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);

}

static void
test_compact(void)
{
   struct spmat_coo *z = spmat_coo_new();

   _printf_test_name("test_compact()", "spmat_coo_compact");

   ASSERT("Constructor test, pt 1", z);

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_insert(void)
{
   struct spmat_coo *z;
   double      x = 1.23;                    /* TODO */

   _printf_test_name("test_insert()", "spmat_coo_insert");

   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
   ASSERT("Here's a test ...", _two_doubles_equal(x, 1.23));

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_shape(void)
{
   struct spmat_coo *z;
   unsigned    a, b, c, d;

   _printf_test_name("test_shape()", "spmat_coo_insert, spmat_coo_shape");

   z = spmat_coo_new();
   spmat_coo_insert(z, 40, 4000, 200.0);
   spmat_coo_insert(z, 10, 1000, 50.0);
   spmat_coo_insert(z, 20, 2000, 100.0);
   spmat_coo_insert(z, 20, 2000, 100.0);
   spmat_coo_shape(z, &a, &b, &c, &d);
   ASSERT_EQUALS(10, a);
   ASSERT_EQUALS(40, b);
   ASSERT_EQUALS(1000, c);
   ASSERT_EQUALS(4000, d);

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

#if 0                                            /* 14 yy */
static void
test_stub(void)
{
   struct spmat_coo *z;
   double      x = 1.23;                    /* TODO */

   _printf_test_name("test_stub()", NULL);

   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
   ASSERT("Here's a test ...", _two_doubles_equal(x, 1.23));

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}
#endif


int
main(void)
{
   printf("%s\n", spmat_coo_version());

   RUN(test_constr);
   RUN(test_insert);
   RUN(test_compact);
   RUN(test_shape);

   return TEST_REPORT();
}