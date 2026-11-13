/* Copyright 2004,2007,2009,2013,2014,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_check.c                          **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the bipartition    **/
/**                graph consistency checking routine.     **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 08 jan 2004     **/
/**                                 to   : 07 dec 2005     **/
/**                # Version 5.1  : from : 04 oct 2009     **/
/**                                 to   : 04 oct 2009     **/
/**                # Version 6.0  : from : 06 oct 2013     **/
/**                                 to   : 25 aug 2014     **/
/**                # Version 7.0  : from : 22 feb 2023     **/
/**                                 to   : 28 mar 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "bgraph.h"

/*************************/
/*                       */
/* These routines handle */
/* bipartition graphs.   */
/*                       */
/*************************/

/* This routine checks the consistency
** of the given bipartition graph.
** It returns:
** - 0   : if graph data are consistent.
** - !0  : on error.
*/

int
bgraphCheck (
const Bgraph * restrict const grafptr)
{
  int * restrict      flagtax;                    /* Frontier flag array           */
  Gnum                cpl1sum;                    /* Sum of vertex loads in part 1 */
  Gnum                ver1nbr;                    /* Number of vertices in part 1  */
  Gnum                commloadintn;
  Gnum                commloadextn;
  Gnum                commgainextn;
  Gnum                fronnum;                    /* Number of frontier vertex     */
  Gnum                vertnum;                    /* Number of current vertex      */
  int                 o;

  const Gnum                        baseval = grafptr->s.baseval;
  const Gnum                        vertnnd = grafptr->s.vertnnd;
  const Gnum * restrict const       verttax = grafptr->s.verttax;
  const Gnum * restrict const       vendtax = grafptr->s.vendtax;
  const Gnum * restrict const       edgetax = grafptr->s.edgetax;
  const GraphPart * restrict const  parttax = grafptr->parttax;
  const Gnum * restrict const       frontab = grafptr->frontab;

  if (grafptr->compload0avg != (Gnum) (((double) (grafptr->s.velosum + grafptr->vfixload[0] + grafptr->vfixload[1]) * (double) grafptr->domnwght[0]) /
                                       (double) (grafptr->domnwght[0] + grafptr->domnwght[1])) - grafptr->vfixload[0]) {
    errorPrint ("bgraphCheck: invalid average load");
    return (1);
  }

  if (grafptr->compload0 != (grafptr->compload0avg + grafptr->compload0dlt)) {
    errorPrint ("bgraphCheck: invalid load balance");
    return (1);
  }

  for (vertnum = baseval; vertnum < vertnnd; vertnum ++) {
    if ((parttax[vertnum] | 1) != 1) {            /* If part is neither 0 nor 1 */
      errorPrint ("bgraphCheck: invalid part array");
      return (1);
    }
  }

  if ((grafptr->fronnbr < 0) ||
      (grafptr->fronnbr > grafptr->s.vertnbr)) {
    errorPrint ("bgraphCheck: invalid number of frontier vertices");
    return (1);
  }

  if ((flagtax = memAlloc (grafptr->s.vertnbr * sizeof (int))) == NULL) {
    errorPrint ("bgraphCheck: out of memory");
    return (1);
  }
  memSet (flagtax, ~0, grafptr->s.vertnbr * sizeof (int));
  flagtax -= baseval;

  o = 1;                                          /* Assume failure when checking */
  for (fronnum = 0; fronnum < grafptr->fronnbr; fronnum ++) {
    Gnum                vertnum;
    Gnum                edgenum;
    GraphPart           partval;
    GraphPart           flagval;

    vertnum = frontab[fronnum];
    if ((vertnum < baseval) || (vertnum >= vertnnd)) {
      errorPrint ("bgraphCheck: invalid vertex index in frontier array");
      goto fail;
    }
    if (flagtax[vertnum] != ~0) {
      errorPrint ("bgraphCheck: duplicate vertex in frontier array");
      goto fail;
    }
    flagtax[vertnum] = 0;
    partval = parttax[vertnum];

    for (edgenum = verttax[vertnum], flagval = 0;
         edgenum < vendtax[vertnum]; edgenum ++)
      flagval |= parttax[edgetax[edgenum]] ^ partval; /* Flag set if neighbor part differs from vertex part */

    if (flagval == 0) {
      errorPrint ("bgraphCheck: invalid vertex in frontier array");
      goto fail;
    }
  }

  for (vertnum = baseval; vertnum < vertnnd; vertnum ++) {
    GraphPart           partval;                  /* Part of current vertex    */
    GraphPart           flagval;                  /* Difference in part values */
    Gnum                edgenum;                  /* Number of current edge    */

    partval = parttax[vertnum];

    for (edgenum = verttax[vertnum], flagval = 0;
         edgenum < vendtax[vertnum]; edgenum ++)
      flagval |= parttax[edgetax[edgenum]] ^ partval; /* Flag set if neighbor part differs from vertex part */

    if ((flagval != 0) && (flagtax[vertnum] != 0)) { /* If vertex should be in frontier array */
      errorPrint ("bgraphCheck: vertex should be in frontier array");
      goto fail;
    }
  }

  bgraphCost2 (grafptr, parttax, NULL, NULL,      /* Compute values for part array */
               &cpl1sum, &ver1nbr, &commloadintn, &commloadextn, &commgainextn);

  if ((grafptr->s.vertnbr - ver1nbr) != grafptr->compsize0) {
    errorPrint ("bgraphCheck: invalid part size");
    goto fail;
  }
  if ((grafptr->s.velosum - cpl1sum) != grafptr->compload0) {
    errorPrint ("bgraphCheck: invalid part load");
    goto fail;
  }
  if ((commloadintn * grafptr->domndist + commloadextn) != grafptr->commload) {
    errorPrint ("bgraphCheck: invalid communication loads");
    goto fail;
  }
  if (commgainextn != grafptr->commgainextn) {
    errorPrint ("bgraphCheck: invalid communication gains");
    goto fail;
  }

  o = 0;                                          /* Everything turned well */

fail:
  memFree (flagtax + baseval);

  return (o);
}
