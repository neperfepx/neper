/* Copyright 2004,2007,2018-2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hgraph_order_hf.c                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module orders a subgraph using     **/
/**                the block-oriented Halo Approximate     **/
/**                (Multiple) Minimum Fill algorithm,      **/
/**                with super-variable accounting          **/
/**                R2HAMDf4 v2.0).                         **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 15 may 2001     **/
/**                                 to   : 23 nov 2001     **/
/**                # Version 4.0  : from : 10 jan 2003     **/
/**                                 to   : 24 jan 2004     **/
/**                # Version 5.0  : from : 10 sep 2007     **/
/**                                 to   : 10 sep 2007     **/
/**                # Version 6.0  : from : 30 apr 2018     **/
/**                                 to   : 30 apr 2018     **/
/**                # Version 6.1  : from : 29 oct 2019     **/
/**                                 to   : 18 jan 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HGRAPH_ORDER_HF

#include "module.h"
#include "common.h"
#include "graph.h"
#include "order.h"
#include "hgraph.h"
#include "hall_order_hf.h"
#include "hall_order_hx.h"
#include "hgraph_order_hf.h"
#include "hgraph_order_hx.h"
#include "hgraph_order_si.h"

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
hgraphOrderHf (
const Hgraph * restrict const             grafptr,
Order * restrict const                    ordeptr,
const Gnum                                ordenum, /*+ Zero-based ordering number +*/
OrderCblk * restrict const                cblkptr, /*+ Multiple column-block      +*/
const HgraphOrderHfParam * restrict const paraptr)
{
  Gnum                n;                          /* Number of nodes to order (with halo or not) */
  Gnum                norig;                      /* Number of nodes in uncompressed graph       */
  Gnum                nbbuck;
  Gnum * restrict     petab;
  Gnum                pfree;
  Gnum * restrict     lentab;
  Gnum                iwlen;
  Gnum * restrict     iwtab;
  Gnum * restrict     nvtab;
  Gnum * restrict     elentab;
  Gnum * restrict     lasttab;
  Gnum * restrict     leaftab;
  Gnum * restrict     secntab;                    /* Array of index to first secondary variable  */
  Gnum * restrict     nexttab;                    /* Array of index of next principal variable   */
  Gnum * restrict     frsttab;
  Gnum * restrict     headtab;                    /* Head array : nbbuck = 2 * n                 */
  Gnum * restrict     cwgttax;                    /* Column weight array                         */
  Gnum                cwgtsiz;
  Gnum                ncmpa;
  int                 o;

  if (grafptr->vnlosum < paraptr->colmin)         /* If graph is too small, order simply */
    return (hgraphOrderSi (grafptr, ordeptr, ordenum, cblkptr));

  n      = grafptr->s.vertnbr;
  norig  = grafptr->s.velosum;
  nbbuck = norig * 2;
  iwlen  = (Gnum) ((double) grafptr->s.edgenbr * HGRAPHORDERHFCOMPRAT) + 32;
  if (iwlen < n)                                  /* TRICK: make sure to be able to re-use array */
    iwlen = n;
  cwgtsiz = (grafptr->s.velotax != NULL) ? n : 0;

  if (memAllocGroup ((void **) (void *)
                     &petab,   (size_t) (n * sizeof (Gnum)),
                     &lentab,  (size_t) (n * sizeof (Gnum)),
                     &nvtab,   (size_t) (n * sizeof (Gnum)),
                     &elentab, (size_t) (n * sizeof (Gnum)),
                     &lasttab, (size_t) (n * sizeof (Gnum)),
                     &leaftab, (size_t) (n * sizeof (Gnum)),
                     &frsttab, (size_t) (n * sizeof (Gnum)),
                     &secntab, (size_t) (n * sizeof (Gnum)),
                     &nexttab, (size_t) (n * sizeof (Gnum)),
                     &cwgttax, (size_t) (cwgtsiz * sizeof (Gnum)), /* Not based yet */
                     &headtab, (size_t) ((nbbuck + 2) * sizeof (Gnum)),
                     &iwtab,   (size_t) (iwlen * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("hgraphOrderHf: out of memory");
    return     (1);
  }

  hgraphOrderHxFill (grafptr, petab, lentab, iwtab, nvtab, elentab, &pfree);

  hallOrderHfR3Hamdf4 (norig, n, 0, nbbuck, iwlen, petab, pfree,
                       lentab, iwtab, nvtab, elentab, lasttab, &ncmpa,
                       leaftab, secntab, nexttab, frsttab, headtab);
  if (ncmpa < 0) {
    errorPrint ("hgraphOrderHf: internal error");
    memFree    (petab);                           /* Free group leader */
    return     (1);
  }

  if (grafptr->s.velotax != NULL) {
    memCpy (cwgttax, grafptr->s.velotax + grafptr->s.baseval, n * sizeof (Gnum));
    cwgttax -= grafptr->s.baseval;
  }
  else
    cwgttax = NULL;

  o = hallOrderHxBuild (grafptr->s.baseval, n, grafptr->vnohnbr,
                        grafptr->s.vnumtax, ordeptr, cblkptr,
                        nvtab   - grafptr->s.baseval,
                        lentab  - grafptr->s.baseval,
                        cwgttax,
                        petab   - grafptr->s.baseval,
                        frsttab - grafptr->s.baseval,
                        nexttab - grafptr->s.baseval,
                        secntab - grafptr->s.baseval,
                        iwtab   - grafptr->s.baseval,
                        elentab - grafptr->s.baseval,
                        ordeptr->peritab + ordenum, /* Use given inverse permutation as inverse permutation space, never based */
                        leaftab,
                        paraptr->colmin, paraptr->colmax, (float) paraptr->fillrat);

  memFree (petab);                                /* Free group leader */

  return (o);
}
