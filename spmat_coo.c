/**
 *  @file spmat_coo.c
 *  @version 0.3.0-dev0
 *  @date Sat Jan 18 19:16:38 CST 2020
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
   unsigned    nnz;                         /* number of triples */
   unsigned    size;                        /* alloced size of list */
   unsigned    extend;                      /* buffer extension */
   struct spmat_coo_triple *list;           /* list of triples */
};

struct spmat_coo *
spmat_coo_new(void)
{
   struct spmat_coo *tp;

   tp = (struct spmat_coo *) malloc(sizeof(struct spmat_coo));
   if (_IS_NULL(tp))
      return NULL;

   tp->nnz = 0;
   tp->size = 0;
   tp->extend = 1024;
   tp->list = NULL;

   return tp;
}

void
spmat_coo_free(struct spmat_coo **pp)
{
   _FREE((*pp)->list);
   _FREE(*pp);
   *pp = NULL;
}

const char *
spmat_coo_version(void)
{
   return "0.3.0-dev0";
}

void
spmat_coo_colsums(struct spmat_coo *p, unsigned n, double *c)
{
   unsigned    j, k;

   for (j = 0; j < n; j++)
      c[j] = 0;                                  /* initialize the colsums */

   for (k = 0; k < p->nnz; k++)
      if (p->list[k].j < n)
         c[p->list[k].j] += p->list[k].v;        /* update */

   printf("COLSUMS ARE ...\n");
   for (j = 0; j < n; j++)
      printf("%f\n", c[j]);
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

void
spmat_coo_compact(struct spmat_coo *p, double tol)
{
   unsigned    k, k0;

   tol = fabs(tol);

   qsort(p->list, p->nnz, sizeof(struct spmat_coo_triple), _cmp);

   /* Combine (add) consecutive locations with repeated i, j */
   for (k = 1, k0 = 0; k < p->nnz; k++) {
      if (p->list[k].i == p->list[k0].i && p->list[k].j == p->list[k0].j) {
#if 0
         /* TODO remove next line */
         printf("Found a twin with %d and %d\n", p->list[k].i, p->list[k].j);
#endif
         p->list[k0].v += p->list[k].v;
      }
      else {
         k0 += 1;
         p->list[k0].i = p->list[k].i;
         p->list[k0].j = p->list[k].j;
         p->list[k0].v = p->list[k].v;
      }
   }

   p->nnz = k0 + 1;

   /* Remove all zero elements */
   for (k = 0, k0 = 0; k < p->nnz; k++) {
      if (fabs(p->list[k].v) > tol) {
         p->list[k0].i = p->list[k].i;
         p->list[k0].j = p->list[k].j;
         p->list[k0].v = p->list[k].v;
         k0 += 1;
      }
   }

   p->nnz = k0;
}

int
spmat_coo_copy(struct spmat_coo *p, struct spmat_coo *q)
{
   unsigned    k;
   int         rc = 0;

   spmat_coo_reset(q);

   for (k = 0; k < p->nnz; k++) {
      rc = spmat_coo_insert(q, p->list[k].i, p->list[k].j, p->list[k].v);
      if (rc)
         return rc;
   }

   return 0;
}

void
spmat_coo_dump(struct spmat_coo *p)
{
   unsigned    k;

   for (k = 0; k < p->nnz; k++)
      printf("%u\t%u\t%e\n", p->list[k].i, p->list[k].j, p->list[k].v);
}

int
spmat_coo_insert(struct spmat_coo *p, unsigned i, unsigned j, double v)
{
   struct spmat_coo_triple *y;

   if (p->nnz == p->size) {
      y = realloc(p->list, (p->size + p->extend) * sizeof(struct spmat_coo_triple));

      if (_IS_NULL(y))
         return -1;

      p->list = y;
      p->size += p->extend;
   }

   p->list[p->nnz].i = i;
   p->list[p->nnz].j = j;
   p->list[p->nnz].v = v;

   p->nnz += 1;

   return 0;
}

void
spmat_coo_mksym(struct spmat_coo *p)
{
   unsigned    nnz0 = p->nnz;
   unsigned    k;

   /**
    *  Using nnz0 in the first loop rather than p->nnz since p->nnz
    *  will be changing as we go.
    *
    *  Basically we're adding a new element for each existing one, in
    *  effect adding the transpose to the original. We'll then compress
    *  and divide by two.
    */

   for (k = 0; k < nnz0; k++) {
      unsigned    i = p->list[k].i;
      unsigned    j = p->list[k].j;
      double      v = p->list[k].v;
      spmat_coo_insert(p, j, i, v);
   }

   spmat_coo_compact(p, 0.0);

   for (k = 0; k < p->nnz; k++)
      p->list[k].v /= 2;
}

void
spmat_coo_reset(struct spmat_coo *p)
{
   if (!_IS_NULL(p))
      p->nnz = 0;
}

void
spmat_coo_rowsums(struct spmat_coo *p, unsigned m, double *r)
{
   unsigned    i, k;

   for (i = 0; i < m; i++)
      r[i] = 0;                                  /* initialize the rowsums */

   for (k = 0; k < p->nnz; k++)
      if (p->list[k].i < m)
         r[p->list[k].i] += p->list[k].v;        /* update */

   printf("ROWSUMS ARE ...\n");
   for (i = 0; i < m; i++)
      printf("%f\n", r[i]);

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

   for (k = 0; k < p->nnz; k++) {
      i = (p->list)[k].i;
      j = (p->list)[k].j;
      *minrow = i < *minrow ? i : *minrow;
      *maxrow = i > *maxrow ? i : *maxrow;
      *mincol = j < *mincol ? j : *mincol;
      *maxcol = j > *maxcol ? j : *maxcol;
   }
}


#undef  _IS_NULL
#undef  _FREE
