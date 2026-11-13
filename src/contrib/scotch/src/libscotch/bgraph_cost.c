/* Copyright 2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_cost.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the bipartition    **/
/**                cost function computation routine.      **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 22 feb 2023     **/
/**                                 to   : 09 aug 2024     **/
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

/* This routine computes the cost of the
** provided partition for the given graph.
** It returns:
** - void  : in all cases.
*/

void
bgraphCost2 (
const Bgraph * restrict const     grafptr,        /*+ Graph the topology and external gains of which to use +*/
const GraphPart * restrict const  parttax,        /*+ Part array to use; may not be graph's parttax         +*/
Gnum * restrict const             frontab,        /*+ Frontier array to compute; NULL if no need to         +*/
Gnum * restrict const             fnbrptr,        /*+ Pointer to frontier size to compute if needed         +*/
Gnum * restrict const             cpl1ptr,        /*+ Pointer to load sum in part 1 to compute              +*/
Gnum * restrict const             ver1ptr,        /*+ Pointer to size of part 1 to compute                  +*/
Gnum * restrict const             cmliptr,        /*+ Pointer to internal communication load to compute     +*/
Gnum * restrict const             cmleptr,        /*+ Pointer to external communication load to compute     +*/
Gnum * restrict const             cmgeptr)        /*+ Pointer to external communication gain to compute     +*/
{
  Gnum *              fronptr;                    /* Pointer to current end of frontier array     */
  Gnum                cpl1sum;                    /* Sum of vertex loads in part 1                */
  Gnum                ver1nbr;                    /* Number of vertices in part 1                 */
  Gnum                cmlisum;                    /* Twice the sum of internal communication load */
  Gnum                cmlesum;                    /* External communication load                  */
  Gnum                cmgesum;                    /* External communication gain                  */
  Gnum                vertnum;                    /* Number of current vertex                     */

  const Gnum * restrict const       verttax = grafptr->s.verttax;
  const Gnum * restrict const       vendtax = grafptr->s.vendtax;
  const Gnum * restrict const       velotax = grafptr->s.velotax;
  const Gnum * restrict const       veextax = grafptr->veextax;
  const Gnum * restrict const       edgetax = grafptr->s.edgetax;
  const Gnum * restrict const       edlotax = grafptr->s.edlotax;

  fronptr = frontab;
  cpl1sum = 0;
  ver1nbr = 0;
  cmlisum = 0;
  cmlesum = grafptr->commloadextn0;
  cmgesum = 0;
  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) {
    Gnum                fronval;                  /* Frontier flag                        */
    Gnum                partval;                  /* Part of current vertex               */
    Gnum                partmsk;                  /* Mask for adding contributions or not */
    Gnum                veloval;                  /* Vertex load                          */
    Gnum                edloval;                  /* Edge load                            */
    Gnum                edgenum;                  /* Number of current edge               */

    partval = (Gnum) parttax[vertnum];
    partmsk = - partval;                          /* TRICK: 0 -> 0; 1 -> 0xFF..FF */

    veloval  = (velotax == NULL) ? 1 : velotax[vertnum];
    ver1nbr += partval;
    cpl1sum += veloval & partmsk;                 /* TRICK: add contribution only for part 1 */

    if (veextax != NULL) {
      Gnum                veexval;

      veexval  = veextax[vertnum];
      cmlesum += veexval & partmsk;               /* TRICK: add contribution only for part 1 */
      cmgesum += veexval * (1 - 2 * partval);
    }

    edloval = 1;                                  /* Assume edges are not weighted    */
    fronval = 0;                                  /* Assume vertex is not in frontier */
    for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
      Gnum                partend;
      Gnum                partdlt;

      if (edlotax != NULL)
        edloval = edlotax[edgenum];
      partend  = parttax[edgetax[edgenum]];
      partdlt  = partval ^ partend;
      cmlisum += edloval & (- partdlt);           /* TRICK: add load only if difference in parts; loads are counted twice */
      fronval |= partdlt;                         /* Record whether some neighbor is not in same part as vertex           */
    }

    if ((fronptr != NULL) && (fronval != 0))      /* If frontier array wanted and vertex belongs to it */
      *(fronptr ++) = vertnum;                    /* Add vertex to frontier array                      */
  }

  if (fronptr != NULL)                            /* If frontier array wanted      */
    *fnbrptr = (Gnum) (fronptr - frontab);        /* Record size of frontier array */
  *cpl1ptr = cpl1sum;
  *ver1ptr = ver1nbr;
  *cmliptr = cmlisum / 2;                         /* We counted cut load twice */
  *cmleptr = cmlesum;
  *cmgeptr = cmgesum;
}

/* This routine computes the cost of the
** current partition of the given graph.
** It returns:
** - void  : in all cases.
*/

void
bgraphCost (
Bgraph * restrict const     grafptr)
{
  Gnum                cpl1sum;                    /* Sum of vertex loads in part 1      */
  Gnum                ver1nbr;                    /* Number of vertices in part 1       */
  Gnum                cmlisum;                    /* Sum of internal communication load */
  Gnum                cmlesum;                    /* External communication load        */
  Gnum                cmgesum;                    /* External communication gain        */

  bgraphCost2 (grafptr, grafptr->parttax, grafptr->frontab, &grafptr->fronnbr,
               &cpl1sum, &ver1nbr, &cmlisum, &cmlesum, &cmgesum);

  grafptr->compload0    = grafptr->s.velosum - cpl1sum;
  grafptr->compload0dlt = grafptr->compload0 - grafptr->compload0avg;
  grafptr->compsize0    = grafptr->s.vertnbr - ver1nbr;
  grafptr->commload     = cmlisum * grafptr->domndist + cmlesum; /* Overall communication load */
  grafptr->commgainextn = cmgesum;
}
