/* Copyright 2004,2007,2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hgraph_order_bl.c                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module resizes block data using    **/
/**                the block splitting post-processing     **/
/**                algorithm.                              **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 24 jun 2002     **/
/**                                 to   : 24 jun 2002     **/
/**                # Version 4.0  : from : 26 jun 2002     **/
/**                                 to   : 17 mar 2005     **/
/**                # Version 5.0  : from : 25 jul 2007     **/
/**                                 to   : 25 jul 2007     **/
/**                # Version 6.1  : from : 18 jan 2020     **/
/**                                 to   : 18 jan 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HGRAPH_ORDER_BL

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "order.h"
#include "hgraph.h"
#include "hgraph_order_bl.h"
#include "hgraph_order_st.h"

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine performs the ordering.
** It returns:
** - 0   : if the ordering could be computed.
** - !0  : on error.
*/

int
hgraphOrderBl (
const Hgraph * restrict const             grafptr,
Order * restrict const                    ordeptr,
const Gnum                                ordenum, /*+ Zero-based ordering number +*/
OrderCblk * restrict const                cblkptr, /*+ Single column-block        +*/
const HgraphOrderBlParam * restrict const paraptr)
{
  Gnum                cblknbr;                    /* Number of column blocks for splitting */
  Gnum                cblksiz;                    /* Number of allocated column blocks     */

  if (paraptr->cblkmin <= 0) {
    errorPrint ("hgraphOrderBl: invalid minimum block size");
    return     (1);
  }

  if (hgraphOrderSt (grafptr, ordeptr, ordenum, cblkptr, paraptr->strat) != 0) /* Perform ordering strategy */
    return (1);

  if (cblkptr->cblktab == NULL) {                 /* If single column block */
#ifdef SCOTCH_DEBUG_HGRAPH2
    if (cblkptr->vnodnbr != grafptr->vnohnbr) {
      errorPrint ("hgraphOrderBl: invalid column block");
      return (1);
    }
#endif /* SCOTCH_DEBUG_HGRAPH2 */

    if ((cblkptr->vnodnbr <= 1) ||                /* Il block has only one column    */
        (grafptr->vnlosum < (2 * paraptr->cblkmin))) /* Or does not need to be split */
      return (0);

    cblknbr = grafptr->vnlosum / paraptr->cblkmin; /* Get new (maximum) number of blocks */
    cblksiz = MIN (cblknbr, grafptr->vnohnbr);    /* No more column blocks than columns  */

    if ((cblkptr->cblktab = (OrderCblk *) memAlloc (cblksiz * sizeof (OrderCblk))) == NULL) {
      errorPrint ("hgraphOrderBl: out of memory");
      return     (1);
    }

    if (grafptr->s.velotax == NULL) {             /* For unweighted graphs, split blocks evenly */
      Gnum                cblknum;

      for (cblknum = 0; cblknum < cblksiz; cblknum ++) { /* No more column blocks than those allocated */
        cblkptr->cblktab[cblknum].typeval = ORDERCBLKOTHR;
        cblkptr->cblktab[cblknum].vnodnbr = DATASIZE (grafptr->vnlosum, cblknbr, cblknum);
        cblkptr->cblktab[cblknum].cblknbr = 0;
        cblkptr->cblktab[cblknum].cblktab = NULL;
      }
    }
    else {
      Gnum                vnlomax;                /* Maxumum weight to reach for this subblock      */
      Gnum                vnlosum;                /* Current accumulated weight                     */
      Gnum                vnodnum;                /* Current permuted node index                    */
      Gnum                vnodbas;                /* Starting permuted node index for current block */
      Gnum                cblknum;                /* Current number of column subblocks             */
      Gnum                cblkidx;                /* Ghost column block index for even splitting    */

      vnlomax =
      vnlosum = 0;
      vnodbas =
      vnodnum = 0;
      cblknum =
      cblkidx = 0;
      do {                                        /* Go on filling column blocks    */
#ifdef SCOTCH_DEBUG_HGRAPH2
        if (cblknum >= cblksiz) {                 /* No more column blocks than those allocated */
          errorPrint ("hgraphOrderBl: internal error (1)");
          return (1);
        }
#endif /* SCOTCH_DEBUG_HGRAPH2 */
        do {                                      /* Compute bound of next subblock */
          cblkidx ++;
#ifdef SCOTCH_DEBUG_HGRAPH2
          if (cblkidx > cblknbr) {                /* No more ghost indices than ghost column blocks */
            errorPrint ("hgraphOrderBl: internal error (2)");
            return (1);
          }
#endif /* SCOTCH_DEBUG_HGRAPH2 */
          vnlomax = DATASCAN (grafptr->vnlosum, cblknbr, cblkidx); /* Bound of next subblock      */
        } while (vnlomax <= vnlosum);             /* Next bound must be higher than current state */

        do {                                      /* Accumulate weights until bound is reached */
          vnlosum += grafptr->s.velotax[ordeptr->peritab[vnodnum]];
          vnodnum ++;
        } while (vnlosum < vnlomax);

        cblkptr->cblktab[cblknum].typeval = ORDERCBLKOTHR; /* Build relevant column block */
        cblkptr->cblktab[cblknum].vnodnbr = vnodnum - vnodbas;
        cblkptr->cblktab[cblknum].cblknbr = 0;
        cblkptr->cblktab[cblknum].cblktab = NULL;
        cblknum ++;                               /* One more effective column block */
        vnodbas = vnodnum;                        /* Reset base for next block       */
      } while (vnlosum < grafptr->vnlosum);       /* As long as columns remain       */

      cblknbr = cblknum;                          /* Use effective number of blocks */
    }

    ordeptr->treenbr += cblknbr;                  /* That many more tree nodes    */
    ordeptr->cblknbr += cblknbr - 1;              /* That many more column blocks */
    cblkptr->cblknbr  = cblknbr;
  }
  else {                                          /* Block already partitioned */
    Gnum                cblknum;

    for (cblknum = 0; cblknum < cblkptr->cblknbr; cblknum ++) {
      if (hgraphOrderBl (grafptr, ordeptr, ordenum, cblkptr->cblktab + cblknum, paraptr) != 0)
        return (1);
    }
  }

  return (0);
}
