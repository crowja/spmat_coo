#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "spmat_coo.h"
#include "t/tinytest.h"
#include "t/tinyhelp.h"

static void
test_constr(void)
{
   struct spmat_coo *z;

   fprintf_test_info(stdout, "test_constr", "spmat_coo_new, spmat_coo_free");
   z = spmat_coo_new();
   ASSERT("Constructor test", z);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_compact(void)
{
   /* TODO */
   struct spmat_coo *z = spmat_coo_new();

   fprintf_test_info(stdout, "test_compact", "spmat_coo_compact");
   ASSERT("Constructor test, pt 1", z);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_insert(void)
{
   /* TODO */
   struct spmat_coo *z;

   fprintf_test_info(stdout, "test_insert", "spmat_coo_insert");
   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

static void
test_shape(void)
{
   struct spmat_coo *z;
   unsigned    a, b, c, d;

   fprintf_test_info(stdout, "test_shape", "spmat_coo_insert, spmat_coo_shape");
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

   fprintf_test_info(stdout, "test_iter", "spmat_coo_iter");
   z = spmat_coo_new();
   for (k = 0; k < 5; k++)
      spmat_coo_insert(z, ilist[k], jlist[k], vlist[k]);

   iter = spmat_coo_iter_new(z);

   k = 0;
   while (spmat_coo_iter_next(iter, &i, &j, &v)) {
      ASSERT_EQUALS(i, ilist[k]);
      ASSERT_EQUALS(j, jlist[k]);
      ASSERT_DOUBLE_EQUALS(v, vlist[k]);
      k++;
   }
   ASSERT_EQUALS(5, k);

   spmat_coo_iter_reset(iter);

   k = 0;
   while (spmat_coo_iter_next(iter, &i, &j, &v)) {
      ASSERT_EQUALS(i, ilist[k]);
      ASSERT_EQUALS(j, jlist[k]);
      ASSERT_DOUBLE_EQUALS(v, vlist[k]);
      k++;
   }
   ASSERT_EQUALS(5, k);

   spmat_coo_iter_free(&iter);
   ASSERT_EQUALS(NULL, iter);
   spmat_coo_free(&z);
   ASSERT_EQUALS(NULL, z);
}

#if 0                                            /* 11 yy */
static void
test_stub(void)
{
   struct spmat_coo *z;

   fprintf_test_info(stdout, "test_stub", NULL);
   z = spmat_coo_new();
   ASSERT("Constructor test, pt 1", z);
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
