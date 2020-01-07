#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

int
main(int argc, char *argv[])
{
   unsigned    i, j;
   struct spmat_coo *x = spmat_coo_new();

#if 0
   spmat_coo_insert(x, 1, 20, 5);
   spmat_coo_insert(x, 0, 1, 10);
   spmat_coo_insert(x, 1, 0, 20);
   spmat_coo_insert(x, 1, 0, 30);
   spmat_coo_insert(x, 0, 0, 20);
   spmat_coo_insert(x, 1, 0, 5);
   spmat_coo_insert(x, 1, 0, 5);
   spmat_coo_insert(x, 1, 1, 5);
   spmat_coo_insert(x, 1, 20, 5);
#else
   spmat_coo_insert(x, 0, 10, 10);
   spmat_coo_insert(x, 20, 5, 20);
   spmat_coo_insert(x, 30, 0, 30);
   spmat_coo_insert(x, 40, 0, 40);
   spmat_coo_insert(x, 50, 0, 50);
   spmat_coo_insert(x, 60, 0, 60);
   spmat_coo_insert(x, 80, 1, 80);
   spmat_coo_insert(x, 90, 20, 90);
#endif

   printf("=====\n");
   spmat_coo_dump(x);

   printf("=====\n");
   spmat_coo_compact(x, 0.0);
   spmat_coo_dump(x);

   spmat_coo_free(&x);

   return 0;
}
