/**
 *  @file spmat_coo.h
 *  @version 0.0.0
 *  @date Mon Jan  6 19:53:07 CST 2020
 *  @copyright 2020 John A. Crow
 *  @license Unlicense <http://unlicense.org/>
 */

#ifndef _SPMAT_COO_H_
#define _SPMAT_COO_H_

#ifdef  _PACKAGE_NAME
#undef  _PACKAGE_NAME
#endif
#define _PACKAGE_NAME "spmat_coo"

struct spmat_coo;

/**
 *  @brief Constructor.
 *  @details Create and return a new spmat_coo object.
 *  @returns New spmat_coo object.
 */
struct spmat_coo *spmat_coo_new(void);

/**
 *  @brief Destructor.
 *  @details Clean up and free a spmat_coo structure.
 *  @param[in, out] p.
 */
void        spmat_coo_free(struct spmat_coo **pp);

/**
 *  @brief Initialize a spmat_coo object.
 *  @details FIXME longer description here ...
 *  @param[in,out] p Pointer to a spmat_coo object
 *  @param[in] x FIXME
    @returns FIXME
 */
int         spmat_coo_init(struct spmat_coo *p, void *x);

/**
 *  @brief Return the version of this package.
 *  @returns Version string.
 */
const char *spmat_coo_version(void);

int         spmat_coo_compact(struct spmat_coo *p, double tol);
int         spmat_coo_copy(struct spmat_coo *p, struct spmat_coo *q);
int         spmat_coo_dump(struct spmat_coo *p);
int         spmat_coo_insert(struct spmat_coo *p, unsigned i, unsigned j, double v);
void        spmat_coo_shape(struct spmat_coo *p, unsigned *minrow, unsigned *maxrow,
                            unsigned *mincol, unsigned *maxcol);
int         spmat_coo_spmv(int trans, double *wrk, unsigned m, struct spmat_coo *a,
                           double cx, unsigned incx, double *x, double cy, unsigned incy,
                           double *y);


#endif
