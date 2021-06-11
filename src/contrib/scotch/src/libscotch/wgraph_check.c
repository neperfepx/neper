/* Copyright 2007-2010,2018,2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_check.c                          **/
/**                                                        **/
/**   AUTHOR     : Jun-Ho HER (v6.0)                       **/
/**                Charles-Edmond BICHOT (v5.1b)           **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module checks the graph consist-   **/
/**                ency for the vertex overlapped graph    **/
/**                partitioning module.                    **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 01 dec 2007     **/
/**                                 to   : 01 jul 2008     **/
/**                # Version 6.0  : from : 05 nov 2009     **/
/**                                 to   : 31 may 2018     **/
/**                # Version 6.1  : from : 09 aug 2020     **/
/**                                 to   : 09 aug 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define WGRAPH_CHECK

#include "module.h"
#include "common.h"
#include "graph.h"
#include "wgraph.h"

/*************************/
/*                       */
/* These routines handle */
/* separator graphs.     */
/*                       */
/*************************/

/* This routine checks the consistency
** of the given separator graph.
** It returns:
** - 0   : if graph data are consistent.
** - !0  : on error.
*/

int
wgraphCheck (
const Wgraph * const        grafptr)
{
  Gnum                      vertnum;
  Gnum                      partnum;
  Gnum                      fronnbr;
  Gnum                      fronnum;
  Gnum                      frlosum;
  Gnum * restrict           comploadtab;
  Gnum * restrict           compsizetab;
  Gnum * restrict           flagtab;
  int                       o;

  if (memAllocGroup ((void **) (void *)
                     &flagtab,     (size_t) (grafptr->partnbr * sizeof (Gnum)),
                     &comploadtab, (size_t) (grafptr->partnbr * sizeof (Gnum)),
                     &compsizetab, (size_t) (grafptr->partnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("wgraphCheck: out of memory");
    return (1);
  }

  o = 1;                                          /* Assume an error */
  fronnbr =
  frlosum = 0;
  memSet (comploadtab, 0, grafptr->partnbr * sizeof (Gnum)); /* Reset loads */
  memSet (compsizetab, 0, grafptr->partnbr * sizeof (Gnum));
  memSet (flagtab,    ~0, grafptr->partnbr * sizeof (Gnum)); /* Reset flag array */

  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) { /* Check part array */
    Anum                partnum;

    partnum = grafptr->parttax[vertnum];

    if ((partnum >= grafptr->partnbr) ||
        (partnum <  -1)) {
      errorPrint ("wgraphCheck: invalid part array");
      goto abort;
    }
  }

  if (grafptr->fronnbr < 0) {
    errorPrint ("wgraphCheck: invalid frontier size");
    goto abort;
  }
  for (fronnum = 0; fronnum < grafptr->fronnbr; fronnum ++) {
    Gnum                vertnum;

    vertnum = grafptr->frontab[fronnum];
    if (grafptr->parttax[vertnum] != -1) {
      errorPrint ("wgraphCheck: invalid frontier array");
      goto abort;
    }
  }

  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) {
    Anum                partnum;
    Gnum                veloval;

    veloval = (grafptr->s.velotax != NULL) ? grafptr->s.velotax[vertnum] : 1;
    partnum = grafptr->parttax[vertnum];

    if (partnum == -1) {
      Gnum                edgenum;

      frlosum += veloval;
      fronnbr ++;

      for (edgenum = grafptr->s.verttax[vertnum];
           edgenum < grafptr->s.vendtax[vertnum]; edgenum ++) {
        Gnum                vertend;
        Anum                partend;

        vertend = grafptr->s.edgetax[edgenum];
        partend = grafptr->parttax[vertend];
        if ((partend != -1) &&
            (flagtab[partend] != vertnum)) {
          comploadtab[partend] += veloval;
          compsizetab[partend] ++;
          flagtab[partend] = vertnum;
        }
      }
    }
    else {
      comploadtab[partnum] += veloval;
      compsizetab[partnum] ++;
    }
  }

  for (partnum = 0; partnum < grafptr->partnbr; partnum ++) {
    if (grafptr->compsize[partnum] != compsizetab[partnum]) {
      errorPrint ("wgraphCheck: invalid part size array");
      goto abort;
    }
    if (grafptr->compload[partnum] != comploadtab[partnum]) {
      errorPrint ("wgraphCheck: invalid part load array");
      goto abort;
    }
  }

  if (grafptr->fronload != frlosum) {
    errorPrint ("wgraphCheck: invalid frontier load");
    goto abort;
  }
  if (grafptr->fronnbr != fronnbr) {
    errorPrint ("wgraphCheck: invalid frontier size");
    goto abort;
  }

  o = 0;                                          /* Everything went all right */
abort :
  memFree (flagtab);                              /* Free group leader */

  return (o);
}
