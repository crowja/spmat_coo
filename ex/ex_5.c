#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

int
main(int argc, char *argv[])
{
   unsigned    i, j;
   unsigned    a, b, c, d;
   struct spmat_coo *x = spmat_coo_new();

   spmat_coo_insert(x, 0,  4, 10);
   spmat_coo_insert(x, 0,  1, 2);
   spmat_coo_insert(x, 1,  0, 3);

   printf("=====\n");
   spmat_coo_dump(x);

   printf("=====\n");
   spmat_coo_mksym(x);
   spmat_coo_dump(x);

   spmat_coo_free(&x);

   return 0;
}
