/* Copyright 2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_part_es.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes an overlap k-way   **/
/**                partition from a k-way edge partition.  **/
/**                                                        **/
/**   DATES      : # Version 6.1  : from : 25 aug 2020     **/
/**                                 to   : 26 aug 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define WGRAPH_PART_ES

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "kgraph.h"
#include "kgraph_map_st.h"
#include "wgraph.h"
#include "wgraph_part_es.h"

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

int
wgraphPartEs (
Wgraph * restrict const         grafptr,          /*+ Active graph      +*/
const WgraphPartEsParam * const paraptr)          /*+ Method parameters +*/
{
  Kgraph              actgrafdat;                 /* K-way graph structure              */
  Arch                tgtarchdat;                 /* Complete graph target architecture */
  ArchDom             tgtdomndat;                 /* Initial domain                     */
  Gnum * restrict     comploadtab;
  Gnum * restrict     compsizetab;
  Gnum * restrict     npfltab;                    /* Neighbor part flag array           */
  Anum                partnbr;                    /* Number of parts                    */
  Anum                partnum;
  Gnum                fronnum;
  Gnum                frlosum;
  Gnum                vertnum;

  const Gnum * restrict const verttax = grafptr->s.verttax;
  const Gnum * restrict const vendtax = grafptr->s.vendtax;
  const Gnum * restrict const velotax = grafptr->s.velotax;
  const Gnum * restrict const edgetax = grafptr->s.edgetax;

  partnbr = grafptr->partnbr;

   if ((npfltab = memAlloc ((partnbr + 1) * sizeof (Gnum))) == NULL) {
    errorPrint ("wgraphPartEs: out of memory");
    return (1);
  }
  npfltab ++;                                     /* TRICK: set slot for frontier */

  tgtarchdat.class   = archClass ("cmplt");       /* Create complete graph target architecture */
  tgtarchdat.flagval = tgtarchdat.class->flagval; /* Copy architecture flag                    */
  ((ArchCmplt *) &tgtarchdat.data)->termnbr = partnbr;
  archDomFrst (&tgtarchdat, &tgtdomndat);

  if (kgraphInit (&actgrafdat, &grafptr->s, &tgtarchdat, &tgtdomndat, 0, NULL, NULL, 1, 0, 0) != 0) {
    errorPrint ("wgraphPartEs: cannot create mapping graph");
    memFree    (npfltab - 1);
    return     (1);
  }

  if (kgraphMapSt (&actgrafdat, paraptr->strat) != 0) { /* Map active subgraph */
    errorPrint ("wgraphPartEs: cannot partition active graph");
    return (1);
  }

  comploadtab = grafptr->compload;
  compsizetab = grafptr->compsize;

  for (partnum = 0; partnum < partnbr; partnum ++)
    comploadtab[archDomNum (&tgtarchdat, &actgrafdat.m.domntab[partnum])] = actgrafdat.comploadavg[partnum] + actgrafdat.comploaddlt[partnum];

  memSet (compsizetab, 0, partnbr * sizeof (Gnum));
  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) {
    Anum                partnum;

    partnum = archDomNum (&tgtarchdat, &actgrafdat.m.domntab[actgrafdat.m.parttax[vertnum]]); /* Set part as value of terminal domain */
    grafptr->parttax[vertnum] = partnum;
    compsizetab[partnum] ++;
  }

  memSet (npfltab - 1, ~0, (partnbr + 1) * sizeof (Gnum));

  frlosum = 0;
  for (fronnum = 0; fronnum < actgrafdat.fronnbr; fronnum ++) { /* Update part array to set frontier vertices as such */
    Gnum                vertnum;
    Gnum                veloval;
    Anum                partnum;

    vertnum = actgrafdat.frontab[fronnum];
    veloval = (velotax != NULL) ? velotax[vertnum] : 1;
    partnum = grafptr->parttax[vertnum];          /* Get old vertex part                     */
    comploadtab[partnum] -= veloval;              /* Assume vertex is no longer in said part */
    compsizetab[partnum] --;
    grafptr->frontab[fronnum] = vertnum;
    grafptr->parttax[vertnum] = -1;               /* Set frontier vertex     */
    frlosum += veloval;                           /* Account for vertex load */
  }

  for (fronnum = 0; fronnum < actgrafdat.fronnbr; fronnum ++) { /* Update part array to set frontier vertices as such */
    Gnum                vertnum;
    Gnum                veloval;
    Gnum                edgenum;

    vertnum = grafptr->frontab[fronnum];
    veloval = (velotax != NULL) ? velotax[vertnum] : 1;
    for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
      Gnum                vertend;
      Anum                partend;

      vertend = edgetax[edgenum];
      partend = grafptr->parttax[vertend];
      if (partend == -1)
        continue;

      if (npfltab[partend] != vertnum) {          /* If part not already considered */
        comploadtab[partend] += veloval;          /* Add vertex to part             */
        compsizetab[partend] ++;
        npfltab[partend] = vertnum;
      }
    }
  }
  grafptr->fronnbr  = actgrafdat.fronnbr;
  grafptr->fronload = frlosum;

  kgraphExit (&actgrafdat);
  memFree    (npfltab - 1);                       /* TRICK: free original array address */

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (wgraphCheck (grafptr) != 0) {
    errorPrint ("wgraphPartEs: inconsistent graph data");
    return (1);
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  return (0);
}
