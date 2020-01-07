#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

int
main(int argc, char *argv[])
{
   unsigned    i, j;
   unsigned    a, b, c, d;
   struct spmat_coo *x = spmat_coo_new();

   spmat_coo_insert(x, 0, 10, 0);
   spmat_coo_insert(x, 20, 5, 0);
   spmat_coo_insert(x, 30, 0, 1);
   spmat_coo_insert(x, 30, 0, -1);
   spmat_coo_insert(x, 40, 0, 0);
   spmat_coo_insert(x, 50, 0, 0);
   spmat_coo_insert(x, 60, 0, 0);
   spmat_coo_insert(x, 80, 1, 0);
   spmat_coo_insert(x, 90, 20, 10);

   printf("=====\n");
   spmat_coo_shape(x, &a, &b, &c, &d);
   printf("min row %d max row %d min col %d max col %d\n", a, b, c, d);
   spmat_coo_dump(x);

   printf("=====\n");
   spmat_coo_compact(x, 0.001);
   spmat_coo_shape(x, &a, &b, &c, &d);
   printf("min row %d max row %d min col %d max col %d\n", a, b, c, d);
   spmat_coo_dump(x);

   spmat_coo_free(&x);

   return 0;
}
