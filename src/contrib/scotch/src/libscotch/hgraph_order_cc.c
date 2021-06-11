/* Copyright 2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hgraph_order_cc.c                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module orders a subgraph by        **/
/**                ordering separately its connected       **/
/**                components.                             **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 04 apr 2018     **/
/**                                 to   : 06 jun 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HGRAPH_ORDER_CC

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "order.h"
#include "hgraph.h"
#include "hgraph_order_cc.h"
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
hgraphOrderCc (
const Hgraph * restrict const             grafptr,
Order * restrict const                    ordeptr,
const Gnum                                ordenum, /*+ Zero-based ordering number +*/
OrderCblk * restrict const                cblkptr, /*+ Single column-block        +*/
const HgraphOrderCcParam * restrict const paraptr)
{
  Hgraph                indgrafdat;
  Gnum                  indordenum;
  Gnum * restrict       flagtax;                  /* Flag array                       */
  Gnum * restrict       roottab;                  /* Array of root indices in queutab */
  Gnum                  rootnbr;                  /* Number of connected components   */
  Gnum                  rootnum;
  Gnum                  vrotnum;                  /* Number of found root vertex      */
  Gnum * restrict       queutab;                  /* Vertex queue and sub-lists       */
  Gnum                  qhedidx;
  Gnum                  qtalidx;

  const Gnum * restrict const verttax = grafptr->s.verttax;
  const Gnum * restrict const vnhdtax = grafptr->vnhdtax;
  const Gnum * restrict const edgetax = grafptr->s.edgetax;

  if (memAllocGroup ((void **) (void *)
                     &queutab, (size_t) ( grafptr->vnohnbr      * sizeof (Gnum)),
                     &roottab, (size_t) ((grafptr->vnohnbr + 1) * sizeof (Gnum)), /* TRICK: +1 to store end index */
                     &flagtax, (size_t) ( grafptr->vnohnbr      * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("hgraphOrderCc: out of memory");
    return     (1);
  }
  memSet (flagtax, ~0, grafptr->vnohnbr * sizeof (Gnum)); /* Initialize flag array */
  flagtax -= grafptr->s.baseval;

  rootnbr = 0;
  qhedidx =
  qtalidx = 0;
  vrotnum = grafptr->s.baseval;
  while (qtalidx < grafptr->vnohnbr) {
    while (flagtax[vrotnum] >= 0)                 /* Search for an unflagged vertex */
      vrotnum ++;
    roottab[rootnbr] = qtalidx;                   /* Record start vertex of component */
    flagtax[vrotnum] = rootnbr;                   /* Flag vertex as enqueued          */
    queutab[qtalidx ++] = vrotnum;                /* Enqueue found root vertex        */

    while (qhedidx < qtalidx) {
      Gnum                vertnum;
      Gnum                edgenum;

      vertnum = queutab[qhedidx ++];              /* Get vertex from queue */
#ifdef SCOTCH_DEBUG_ORDER2
      if ((vertnum < grafptr->s.baseval) || (vertnum >= grafptr->vnohnnd)) {
        errorPrint ("hgraphOrderCc: internal error (1)");
        return     (1);
      }
#endif /* SCOTCH_DEBUG_ORDER2 */

      for (edgenum = verttax[vertnum]; edgenum < vnhdtax[vertnum]; edgenum ++) {
        Gnum                  vertend;

        vertend = edgetax[edgenum];
        if (flagtax[vertend] < 0) {               /* If end vertex has never been enqueued */
          flagtax[vertend] = rootnbr;             /* Flag end vertex                       */
          queutab[qtalidx ++] = vertend;          /* Enqueue it                            */
        }
#ifdef SCOTCH_DEBUG_ORDER2
        else {
          if (flagtax[vertend] != rootnbr) {
            errorPrint ("hgraphOrderCc: internal error (2)");
            return     (1);
          }
        }
#endif /* SCOTCH_DEBUG_ORDER2 */
      }
    }

    rootnbr ++;                                   /* One more connected component */
  }
  roottab[rootnbr] = qtalidx;                     /* Record end index */

  if (rootnbr == 1) {                             /* If only one connected component                                  */
    memFree (queutab);                            /* Free group leader                                                */
    return (hgraphOrderSt (grafptr, ordeptr, ordenum, cblkptr, paraptr->straptr)); /* Perform strategy on whole graph */
  }

  if ((cblkptr->cblktab = (OrderCblk *) memAlloc (rootnbr * sizeof (OrderCblk))) == NULL) {
    errorPrint ("hgraphOrderCc: out of memory");
    memFree    (queutab);
    return     (1);
  }
  ordeptr->treenbr += rootnbr;                    /* These more number of tree nodes    */
  ordeptr->cblknbr += rootnbr - 1;                /* These more number of column blocks */
  cblkptr->cblknbr  = rootnbr;
  cblkptr->typeval  = ORDERCBLKDICO;              /* Disconnected components node  */
  for (rootnum = 0; rootnum < rootnbr; rootnum ++) { /* Initialize tree node array */
    cblkptr->cblktab[rootnum].typeval = ORDERCBLKOTHR;
    cblkptr->cblktab[rootnum].vnodnbr = roottab[rootnum + 1] - roottab[rootnum];
    cblkptr->cblktab[rootnum].cblknbr = 0;
    cblkptr->cblktab[rootnum].cblktab = NULL;
  }

  for (rootnum = 0, indordenum = 0; rootnum < rootnbr; rootnum ++) {
    Gnum                indvnohnbr;
    int                 o;

    indvnohnbr = roottab[rootnum + 1] - roottab[rootnum];

    if (hgraphInduceList (grafptr, indvnohnbr, &queutab[roottab[rootnum]], grafptr->s.vertnbr - grafptr->vnohnbr, &indgrafdat) != 0) {
      errorPrint ("hgraphOrderCc: cannot create induced graph");
      memFree    (queutab);
      return     (1);
    }

    o = hgraphOrderSt (&indgrafdat, ordeptr, indordenum, &cblkptr->cblktab[rootnum], paraptr->straptr); /* Perform strategy on induced subgraph */

    hgraphExit (&indgrafdat);

    if (o != 0) {
      errorPrint ("hgraphOrderCc: cannot compute ordering on induced graph");
      memFree    (queutab);
      return     (1);
    }

    indordenum += indvnohnbr;
  }

  memFree (queutab);

  return (0);
}
