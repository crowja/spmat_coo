#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "spmat_coo.h"
#include "t/tinytest.h"

#ifdef  COLOR_CODE
#undef  COLOR_CODE
#endif
#define COLOR_CODE       0x1B

#ifdef  COLOR_RED
#undef  COLOR_RED
#endif
#define COLOR_RED        "[1;31m"

#ifdef  COLOR_GREEN
#undef  COLOR_GREEN
#endif
#define COLOR_GREEN      "[1;32m"

#ifdef  COLOR_YELLOW
#undef  COLOR_YELLOW
#endif
#define COLOR_YELLOW     "[1;33m"

#ifdef  COLOR_RESET
#undef  COLOR_RESET
#endif
#define COLOR_RESET      "[0m"

static void
printf_test_name(char *name, char *info)
{
   printf("%c%s%s%c%s", COLOR_CODE, COLOR_YELLOW, name, COLOR_CODE, COLOR_RESET);

   if (NULL != info)
      printf(" [%s]\n", info);
   else
      printf("\n");
}

static int
two_doubles_equal(double x, double y)
{
   double      t = fabs(x) + fabs(y);
   return fabs(x - y) < 4 * DBL_EPSILON * t ? 1 : 0;
}

static void
test_constr(void)
{
   struct spmat_coo *z;

   printf_test_name("test_constr", "spmat_coo_new, spmat_coo_free");

   z = spmat_coo_new();
   ASSERT("Constructor test", z);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);

}

static void
test_compact(void)
{
   struct spmat_coo *z = spmat_coo_new();

   printf_test_name("test_compact", "spmat_coo_compact");

   ASSERT("Constructor test, pt 1", z);

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_insert(void)
{
   struct spmat_coo *z;
   double      x = 1.23;                    /* TODO */

   printf_test_name("test_insert", "spmat_coo_insert");

   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
   ASSERT("Here's a test ...", two_doubles_equal(x, 1.23));

   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_shape(void)
{
   struct spmat_coo *z;
   unsigned    a, b, c, d;

   printf_test_name("test_shape", "spmat_coo_insert, spmat_coo_shape");

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

static void
test_iter(void)
{
   struct spmat_coo *z;
   struct spmat_coo_iter *iter;
   unsigned    ilist[5] = { 50, 1, 40, 2, 30 };
   unsigned    jlist[5] = { 4, 3, 2, 1, 0 };
   double      vlist[5] = { 10.0, 11.0, 12.0, 13.0, 14.0 };
   unsigned    i, j, k;
   double      v;

   printf_test_name("test_iter", "spmat_coo_iter");

   z = spmat_coo_new();
   for (k = 0; k < 5; k++)
      spmat_coo_insert(z, ilist[k], jlist[k], vlist[k]);

   iter = spmat_coo_iter_new(z);

   k = 0;
   while (spmat_coo_iter_next(iter, &i, &j, &v)) {
      ASSERT_EQUALS(i, ilist[k]);
      ASSERT_EQUALS(j, jlist[k]);
      ASSERT("Testing doubles", two_doubles_equal(v, vlist[k]));
      k++;
   }
   ASSERT_EQUALS(5, k);

   spmat_coo_iter_reset(iter);

   k = 0;
   while (spmat_coo_iter_next(iter, &i, &j, &v)) {
      ASSERT_EQUALS(i, ilist[k]);
      ASSERT_EQUALS(j, jlist[k]);
      ASSERT("Testing doubles", two_doubles_equal(v, vlist[k]));
      k++;
   }
   ASSERT_EQUALS(5, k);

   spmat_coo_iter_free(&iter);
   ASSERT_EQUALS(NULL, iter);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

#if 0                                            /* 15 yy */
static void
test_stub(void)
{
   struct spmat_coo *z;
   double      x = 1.23;                    /* TODO */

   printf_test_name("test_stub", NULL);

   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
   ASSERT("Here's a test ...", two_doubles_equal(x, 1.23));

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
   RUN(test_iter);

   return TEST_REPORT();
}
