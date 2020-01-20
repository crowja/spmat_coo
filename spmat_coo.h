/**
 *  @file spmat_coo.h
 *  @version 0.3.0-dev0
 *  @date Sun Jan 19 20:11:15 CST 2020
 *  @copyright 2020 John A. Crow
 *  @license Unlicense <http://unlicense.org/>
 */

#ifndef _SPMAT_COO_H_
#define _SPMAT_COO_H_

struct spmat_coo;
struct spmat_coo_iter;

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
const char *spmat_coo_version(void);
int         spmat_coo_init(struct spmat_coo *p, void *x);

void        spmat_coo_colsums(struct spmat_coo *p, unsigned n, double *c);
void        spmat_coo_compact(struct spmat_coo *p, double tol);
int         spmat_coo_copy(struct spmat_coo *p, struct spmat_coo *q);
void        spmat_coo_dump(struct spmat_coo *p);
int         spmat_coo_insert(struct spmat_coo *p, unsigned i, unsigned j, double v);
void        spmat_coo_mksym(struct spmat_coo *p);
void        spmat_coo_reset(struct spmat_coo *p);
void        spmat_coo_rowsums(struct spmat_coo *p, unsigned m, double *r);
void        spmat_coo_shape(struct spmat_coo *p, unsigned *minrow, unsigned *maxrow,
                            unsigned *mincol, unsigned *maxcol);

struct spmat_coo_iter *spmat_coo_iter_new(const struct spmat_coo *p);
void        spmat_coo_iter_free(struct spmat_coo_iter **pp);
int         spmat_coo_iter_next(struct spmat_coo_iter *p, unsigned *i, unsigned *j,
                                double *v);
void        spmat_coo_iter_reset(struct spmat_coo_iter *p);

#endif
