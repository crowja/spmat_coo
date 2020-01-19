#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

#define  N    100

int
main(void)
{
   unsigned    i;
   unsigned    maxcol, mincol, maxrow, minrow;
   struct spmat_coo *x = spmat_coo_new();
   double      wrk[N], y[N];

   spmat_coo_insert(x, 0, 0, -2);
   spmat_coo_insert(x, 0, 1, 2);
   y[0] = 1;
   for (i = 1; i < N - 1; i++) {
      spmat_coo_insert(x, i, i, -2);
      spmat_coo_insert(x, i, i + 1, 1);
      spmat_coo_insert(x, i, i - 1, 1);
      y[i] = 1;
   }
   spmat_coo_insert(x, N - 1, N - 2, 2);
   spmat_coo_insert(x, N - 1, N - 1, -2);
   y[N - 1] = 1;

   spmat_coo_shape(x, &minrow, &maxrow, &mincol, &maxcol);
   printf("ROWS FROM %d TO %d\n", minrow, maxrow);
   printf("ROWS FROM %d TO %d\n", mincol, maxcol);

   /*
      spmat_coo_spmv(int trans, double *wrk, unsigned m, struct spmat_coo *a, double cx,
      unsigned incx, double *x, double cy, unsigned incy, double *y)
    */

#if 1
   spmat_coo_spmv(0, wrk, N, x, 1.0, 1, y, 0.0, 1, y);
   for (i = 0; i < N; i++)
      printf("%d --> %10.2f\n", i, y[i]);
#endif

   spmat_coo_free(&x);

   return 0;
}
