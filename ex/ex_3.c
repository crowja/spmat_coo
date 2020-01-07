#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

int
main(int argc, char *argv[])
{
   unsigned    i, j;
   unsigned    a, b, c, d;
   struct spmat_coo *x = spmat_coo_new();
   double      u[4] = { 1, 1, 1, 1 };
   double      v[4] = { 1, 1, 1, 1 };
   double      wrk[4];

   spmat_coo_insert(x, 0, 0, 1);
   spmat_coo_insert(x, 1, 1, 1);
   spmat_coo_insert(x, 2, 2, 1);
   spmat_coo_insert(x, 3, 3, 1);
   spmat_coo_insert(x, 0, 3, 2);
   spmat_coo_insert(x, 1, 3, 2);
   spmat_coo_insert(x, 2, 3, 2);

   printf("=====\n");
   spmat_coo_shape(x, &a, &b, &c, &d);
   printf("min row %d max row %d min col %d max col %d\n", a, b, c, d);
   spmat_coo_dump(x);
   printf("=====\n");
   spmat_coo_spmv(0, wrk, 4, x, 1.0, 1, u, 1.0, 1, v);
#if 1
   for (i = 0; i < 4; i++) {
      printf("%d--%f\n", i, v[i]);
   }
#endif


   spmat_coo_free(&x);

   return 0;
}
