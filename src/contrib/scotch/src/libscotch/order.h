/* Copyright 2004,2007,2010,2018,2023-2025 IPB, Universite de Bordeaux, INRIA & CNRS
**
** This file is part of the Scotch software package for static mapping,
** graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-C license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
**
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : order.h                                 **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Clement BARTHELEMY                      **/
/**                                                        **/
/**   FUNCTION   : This module contains the data           **/
/**                declarations for the generic ordering   **/
/**                structure.                              **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 19 oct 1996     **/
/**                                 to   : 21 aug 1998     **/
/**                # Version 4.0  : from : 19 dec 2001     **/
/**                                 to   : 28 dec 2004     **/
/**                # Version 5.0  : from : 25 jul 2007     **/
/**                                 to   : 25 jul 2007     **/
/**                # Version 5.1  : from : 04 nov 2010     **/
/**                                 to   : 04 nov 2010     **/
/**                # Version 6.0  : from : 08 may 2018     **/
/**                                 to   : 06 jun 2018     **/
/**                # Version 7.0  : from : 26 apr 2021     **/
/**                                 to   : 17 jan 2025     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

#define SCOTCH_ORDER_H

/*+ Ordering option flags. +*/

#define ORDERNONE                   0x0000        /* No options set                 */
#define ORDERFREEPERI               0x0001        /* Free inverse permutation array */

/*+ Column block tree cell flags.
    They must be the same as the
    distributed column block tree flags.
    These flags must be separate bits, so
    that values can be or-ed (notably with
    ORDERCBLKLEAF in hdgraphOrderNd().
    ORDERCBLKNEDI corresponds to a nested
    dissection node. If the node has a
    separator (3 sub-blocks), the father of
    the leftmost two blocks is the last one;
    if is has no separator (2 sub-blocks), the
    father of the two sub-blocks is the father
    of the node (like ORDERCBLKDICO).
    ORDERCBLKDICO corresponds to a set of
    disconnected components; the father of
    the two sub-blocks is the father of the
    node.
    ORDERCBLKSEQU corresponds to a sequential
    node: each sub-block is the father of its
    left neighbor.
    ORDERCBLKLEAF corresponds to a leaf node
    in the elimination tree, i.e., the node
    should not have sub-nodes (save when
    or-ed with other flags).                   +*/

#define ORDERCBLKNONE               0x0000        /*+ Not yet assigned                 +*/
#define ORDERCBLKNEDI               0x0001        /*+ Nested dissection separator node +*/
#define ORDERCBLKDICO               0x0002        /*+ Disconnected components node     +*/
#define ORDERCBLKSEQU               0x0004        /*+ Sequentially dependent node      +*/
#define ORDERCBLKLEAF               0x0008        /*+ Leaf node                        +*/

/*
**  The type and structure definitions.
*/

/*+ Column-block tree node. Each node
    defines a column block, which is either
    a separator or a leaf built by nested
    dissection, or a super-variable built
    by minimum-degree algorithms. Column
    blocks are given in ascending order
    within sub-arrays, for proper infix
    traversal.                              +*/

typedef struct OrderCblk_ {
  int                       typeval;              /*+ Type of tree node                  +*/
  Gnum                      vnodnbr;              /*+ Number of node vertices in subtree +*/
  Gnum                      cblknbr;              /*+ Number of descendent column blocks +*/
  struct OrderCblk_ *       cblktab;              /*+ Sub-array of column-blocks         +*/
} OrderCblk;

/*+ Ordering structure. A block ordering is
    defined by its inverse permutation peritab
    and by the tree of permuted ordering blocks,
    which, once flattened, defines the blocks
    of the ordering. For the sake of consistency
    between orderings that have been produced
    either from graphs or meshes, all ordering
    values are based from baseval.               +*/

typedef struct Order_ {
  int                       flagval;              /*+ Flag value                               +*/
  Gnum                      baseval;              /*+ Base value for structures                +*/
  Gnum                      vnodnbr;              /*+ Number of node vertices                  +*/
  Gnum                      treenbr;              /*+ Number of column block tree nodes        +*/
  Gnum                      cblknbr;              /*+ Number of column blocks                  +*/
  OrderCblk                 rootdat;              /*+ Root of column block tree                +*/
  Gnum *                    peritab;              /*+ Inverse permutation array [vnodnbr]      +*/
#ifdef SCOTCH_PTHREAD
  pthread_mutex_t           mutedat;              /*+ Local mutex for counter and link updates +*/
#endif /* SCOTCH_PTHREAD */
} Order;

/*
**  The function prototypes.
*/

#ifdef SCOTCH_ORDER
static void                 orderExit2          (OrderCblk * const, const Gnum);
static void                 orderRang2          (Gnum ** const, Gnum * const, const OrderCblk * const);
static void                 orderTree2          (Gnum * restrict const, Gnum * restrict const, const OrderCblk * restrict const, Gnum);
#endif /* SCOTCH_ORDER */

int                         orderInit           (Order * const, const Gnum, const Gnum, Gnum * const);
void                        orderExit           (Order * const);
int                         orderLoad           (Order * restrict const, const Gnum * restrict const, FILE * restrict const);
int                         orderSave           (const Order * restrict const, const Gnum * restrict const, FILE * restrict const);
int                         orderSaveMap        (const Order * restrict const, const Gnum * restrict const, FILE * restrict const);
int                         orderSaveTree       (const Order * restrict const, const Gnum * restrict const, FILE * restrict const);
void                        orderPeri           (const Gnum * const, const Gnum, const Gnum, Gnum * const, const Gnum);
void                        orderRang           (const Order * const, Gnum * const);
void                        orderTree           (const Order * restrict const, Gnum * restrict const);
int                         orderCheck          (const Order * const);
