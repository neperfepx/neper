/* Copyright 2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_cost.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes the cost of k-way  **/
/**                partitions.                             **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 16 jun 2023     **/
/**                                 to   : 16 jun 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "kgraph.h"

/***********************************/
/*                                 */
/* Active graph handling routines. */
/*                                 */
/***********************************/

/* This routine computes the cost of the
** current partition.
** It returns:
** - void  : in all cases.
*/

void
kgraphCost (
Kgraph * restrict const     grafptr)
{
  Gnum                      vertnum;
  Gnum * restrict           compload;
  Gnum                      commload;
  double                    fdomwgt;
  Gnum                      fvelsum;
  Gnum                      velosum;
  Anum                      domnnum;
  ArchDom                   domndat;
  double                    domnrat;

  const Gnum * restrict const     verttax = grafptr->s.verttax;
  const Gnum * restrict const     velotax = grafptr->s.velotax;
  const Gnum * restrict const     vendtax = grafptr->s.vendtax;
  const Gnum * restrict const     edlotax = grafptr->s.edlotax;
  const Gnum * restrict const     edgetax = grafptr->s.edgetax;
  const Arch * restrict const     archptr = grafptr->m.archptr;
  const ArchDom * restrict const  domntab = grafptr->m.domntab;
  Anum * restrict const           parttax = grafptr->m.parttax;
  const Anum                      domnnbr = grafptr->m.domnnbr;

  commload = 0;
  compload = grafptr->comploaddlt;                   /* Use delta array as temporary storage */
  memSet (compload, 0, domnnbr * sizeof (Gnum));
  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) {
    Gnum                edgenum;
    Gnum                edgennd;
    Anum                partval;                  /* Part of current vertex                                */
    Anum                partlst;                  /* Part of last vertex for which a distance was computed */
    Anum                distlst;                  /* Last distance computed                                */
    Gnum                veloval;

    partval = parttax[vertnum];
    partlst = -1;                                 /* Invalid part to recompute distance */
    distlst = -1;                                 /* To prevent compiler from yelling   */

#ifdef SCOTCH_DEBUG_KGRAPH2
    if ((partval < 0) || (partval >= domnnbr)) {
      errorPrint ("kgraphCost: invalid part number (1)");
      return;
    }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
    veloval = (velotax != NULL) ? velotax[vertnum] : 1;
    compload[partval] += veloval;

    for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum];
         edgenum < edgennd; edgenum ++) {
      Gnum                vertend;
      Anum                partend;

      vertend = edgetax[edgenum];
      if (vertend > vertnum)                      /* Compute loads only once */
        continue;

      partend = parttax[vertend];
#ifdef SCOTCH_DEBUG_KGRAPH2
      if ((partend < 0) || (partend >= domnnbr)) {
        errorPrint ("kgraphCost: invalid part number (2)");
        return;
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
      if (partval != partend) {
        if (partend != partlst) {
          distlst = archDomDist (archptr, &domntab[partval], &domntab[partend]);
          partlst = partend;
        }
        commload += (Gnum) distlst * ((edlotax != NULL) ? edlotax[edgenum] : 1);
      }
    }
  }
  grafptr->commload = commload;

  fdomwgt = 0;
  fvelsum = 0;
  if ((grafptr->s.flagval & KGRAPHHASANCHORS) != 0) {
    const Gnum                  vertancnnd = grafptr->s.vertnnd - domnnbr;
    Gnum                        veloval;

    for (domnnum = 0; domnnum < domnnbr; domnnum ++)
      if ((grafptr->s.verttax[vertancnnd + domnnum + 1] - grafptr->s.verttax[vertancnnd + domnnum]) != 0)
        continue;

    if (domnnum != domnnbr) {
      for (domnnum = 0; domnnum < domnnbr; domnnum ++) {
        if ((grafptr->s.verttax[vertancnnd + domnnum + 1] - grafptr->s.verttax[vertancnnd + domnnum]) == 0) {
          veloval = grafptr->s.velotax[vertancnnd + domnnum];

          fdomwgt += (double) archDomWght (archptr, &domntab[domnnum]);
          fvelsum += veloval;
          compload[domnnum] -=
          grafptr->comploadavg[domnnum] = veloval;
#ifdef SCOTCH_DEBUG_KGRAPH2
          if (compload[domnnum] != 0) {
            errorPrint ("kgraphCost: invalid load difference");
            return;
          }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
        }
      }
    }
  }
  archDomFrst (archptr, &domndat);
  domnrat = (double) archDomWght (archptr, &domndat);
  domnrat -= fdomwgt;
  velosum = grafptr->s.velosum - fvelsum;
  for (domnnum = 0; domnnum < domnnbr; domnnum ++) {
    compload[domnnum]            -=
    grafptr->comploadavg[domnnum] = (Gnum) ((double) velosum * ((double) archDomWght (archptr, &domntab[domnnum]) / domnrat));
  }
}
