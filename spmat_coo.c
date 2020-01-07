/**
 *  @file spmat_coo.c
 *  @version 0.1.1-dev0
 *  @date Mon Jan  6 20:51:27 CST 2020
 *  @copyright 2020 John A. Crow
 *  @license Unlicense <http://unlicense.org/>
 */

#include <stdlib.h>
#include <stdio.h>                               /* FIXME */
#include <math.h>
#include <limits.h>
#include "spmat_coo.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

struct spmat_coo_triple {
   unsigned    i;
   unsigned    j;
   double      v;
};

struct spmat_coo {
   unsigned    len;                         /* number of triples */
   unsigned    size;                        /* size of triples buffer */
   unsigned    extend;                      /* buffer extension */
   struct spmat_coo_triple *x;              /* list of triples */
   double     *wrk;                         /* used only if needed */
};

struct spmat_coo *
spmat_coo_new(void)
{
   struct spmat_coo *tp;

   tp = (struct spmat_coo *) malloc(sizeof(struct spmat_coo));
   if (_IS_NULL(tp))
      return NULL;

   tp->len = 0;
   tp->size = 0;
   tp->extend = 1024;
   tp->x = NULL;
   tp->wrk = NULL;

   return tp;
}

void
spmat_coo_free(struct spmat_coo **pp)
{
   _FREE((*pp)->x);
   _FREE((*pp)->wrk);
   _FREE(*pp);
   *pp = NULL;
}

const char *
spmat_coo_version(void)
{
   return "0.1.1-dev0";
}

static int
_cmp(const void *a, const void *b)
{
   if (((struct spmat_coo_triple *) a)->i < ((struct spmat_coo_triple *) b)->i)
      return -1;
   if (((struct spmat_coo_triple *) a)->i > ((struct spmat_coo_triple *) b)->i)
      return 1;
   if (((struct spmat_coo_triple *) a)->j < ((struct spmat_coo_triple *) b)->j)
      return -1;
   if (((struct spmat_coo_triple *) a)->j > ((struct spmat_coo_triple *) b)->j)
      return 1;
   if (((struct spmat_coo_triple *) a)->v < ((struct spmat_coo_triple *) b)->v)
      return -1;
   if (((struct spmat_coo_triple *) a)->v > ((struct spmat_coo_triple *) b)->v)
      return 1;

   return 0;
}

int
spmat_coo_compact(struct spmat_coo *p, double tol)
{
   unsigned    k, k0;

   tol = fabs(tol);

   qsort(p->x, p->len, sizeof(struct spmat_coo_triple), _cmp);

   /* Combine (add) consecutive locations with repeated i, j */
   for (k = 1, k0 = 0; k < p->len; k++) {
      if (p->x[k].i == p->x[k0].i && p->x[k].j == p->x[k0].j) {
         printf("Found a twin with %d and %d\n", p->x[k].i, p->x[k].j);
         p->x[k0].v += p->x[k].v;
      }
      else {
         k0 += 1;
         p->x[k0].i = p->x[k].i;
         p->x[k0].j = p->x[k].j;
         p->x[k0].v = p->x[k].v;
      }
   }

   p->len = k0 + 1;

   /* Remove all zero elements */
   for (k = 0, k0 = 0; k < p->len; k++) {
      if (fabs(p->x[k].v) < tol) {               /* found a zero */
         printf("Found a zero with %d and %d\n", p->x[k].i, p->x[k].j);
      }
      else {
         p->x[k0].i = p->x[k].i;
         p->x[k0].j = p->x[k].j;
         p->x[k0].v = p->x[k].v;
         k0 += 1;
      }
   }

   p->len = k0;

   return 0;
}

#if 0
/*** spmat_coo_copy() ***/

int
spmat_coo_copy(struct spmat_coo *p, struct spmat_coo *q)
{

   /* Do some magic here ... */

   return 0;
}
#endif

int
spmat_coo_dump(struct spmat_coo *p)
{
   unsigned    k;

   for (k = 0; k < p->len; k++)
      printf("%d\t%d\t%e\n", p->x[k].i, p->x[k].j, p->x[k].v);

   return 0;
}

int
spmat_coo_insert(struct spmat_coo *p, unsigned i, unsigned j, double v)
{
   struct spmat_coo_triple *y;

   if (p->len == p->size) {
      y = realloc(p->x, (p->size + p->extend) * sizeof(struct spmat_coo_triple));

      if (_IS_NULL(y))
         return 1;

      p->x = y;
      p->size += p->extend;
   }

   p->x[p->len].i = i;
   p->x[p->len].j = j;
   p->x[p->len].v = v;

   p->len += 1;

   return 0;
}

void
spmat_coo_shape(struct spmat_coo *p, unsigned *minrow, unsigned *maxrow, unsigned *mincol,
                unsigned *maxcol)
{
   unsigned    i, j, k;

   *minrow = UINT_MAX;
   *maxrow = 0;
   *mincol = UINT_MAX;
   *maxcol = 0;

   for (k = 0; k < p->len; k++) {
      i = (p->x)[k].i;
      j = (p->x)[k].j;
      *minrow = i < *minrow ? i : *minrow;
      *maxrow = i > *maxrow ? i : *maxrow;
      *mincol = j < *mincol ? j : *mincol;
      *maxcol = j > *maxcol ? j : *maxcol;
   }
}

int
spmat_coo_spmv(int trans, double *wrk, unsigned m, struct spmat_coo *a, double cx,
               unsigned incx, double *x, double cy, unsigned incy, double *y)
{
   unsigned    i, j, k;

   for (i = 0; i < m; i++)
      wrk[i] = 0;

   /* wrk = a x or transpose(a) x */
   for (k = 0; k < a->len; k++) {
      if (trans) {                               /* transpose(a) */
         i = (a->x)[k].j;
         j = (a->x)[k].i;
      }
      else {
         i = (a->x)[k].i;
         j = (a->x)[k].j;
      }
      wrk[i] += (a->x)[k].v * x[j * incx];
#if 0
      printf("DEBUG WRK[%d] = %f\n", i, wrk[i]);
#endif
   }

   for (i = 0; i < m; i++) 
      y[i * incy] = cx * wrk[i] + cy * y[i * incy];

   return 0;
}

#undef  _IS_NULL
#undef  _FREE
