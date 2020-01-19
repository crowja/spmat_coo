#include <stdlib.h>
#include <stdio.h>
#include "spmat_coo.h"

int main(void) {
   unsigned i, j;
   double v;
   struct spmat_coo *counts = spmat_coo_new();

   printf("Reading the data ...\n");
   while (EOF != fscanf(stdin, "%u %u %lf", &i, &j, &v)) {
      spmat_coo_insert(counts, i, j, v);
   }
   printf("... done!\n");

   printf("Doing a compact ...\n");
   spmat_coo_compact(counts, 0.00001);
   printf("... done!\n");

   printf("Doing a mksym ...\n");
   spmat_coo_mksym(counts);
   printf("... done!\n");

   /* spmat_coo_dump(counts); */

   spmat_coo_free(&counts);

   return 0;
}
