/* Copyright 2010,2011,2014,2018,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_bd.c                         **/
/**                                                        **/
/**   AUTHOR     : Sebastien FOURESTIER (v6.0)             **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes a partition of     **/
/**                the given k-way mapping graph by        **/
/**                creating a band graph of given          **/
/**                width around the current frontier,      **/
/**                computing an improved partition of the  **/
/**                band graph, and projecting back the     **/
/**                obtained frontier to the original       **/
/**                graph.                                  **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 05 jan 2010     **/
/**                                 to   : 21 may 2018     **/
/**                # Version 6.1  : from : 30 jun 2021     **/
/**                                 to   : 16 jul 2021     **/
/**                # Version 7.0  : from : 20 jan 2023     **/
/**                                 to   : 07 oct 2026     **/
/**                                                        **/
/**   NOTES      : # Since only edges from local vertices  **/
/**                  to local anchors are created in       **/
/**                  kdgraphBand(), the communication cost **/
/**                  might be wrong if a local vertex of   **/
/**                  the last layer is linked to a remote  **/
/**                  vertex of different part which was    **/
/**                  not in the band graph. Hence, commun- **/
/**                  ication costs have to be recomputed   **/
/**                  from scratch.                         **/
/**                                                        **/
/**                # This code derives from the code of    **/
/**                  bdgraph_bipart_bd.c in version 5.1    **/
/**                  for direct k-way partitioning.        **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "parser.h"
#include "arch.h"
#include "graph.h"
#include "mapping.h"
#include "kgraph.h"
#include "kgraph_map_bd.h"
#include "kgraph_map_st.h"

/*
**  The static variables.
*/

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine computes a band graph of given
** width around the current frontier and applies
** partitioning routines to it.
** The graph is not guaranteed to be balanced
** at all.
** It returns:
** - 0   : if the band graph could be computed.
** - !0  : on error.
*/

int
kgraphMapBd (
Kgraph * const                      orggrafptr,   /*+ Graph             +*/
const KgraphMapBdParam * const      paraptr)      /*+ Method parameters +*/
{
  Kgraph                    bndgrafdat;           /* Partitioning band graph structure                                   */
  const Gnum * restrict     bndverttax;
  const Gnum * restrict     bndvendtax;
  const Gnum * restrict     bndedgetax;
  const Gnum * restrict     bndedlotax;
  const Anum * restrict     bndparttax;
  const ArchDom * restrict  bnddomntab;
  Gnum                      bndvertancnnd;        /* End of local vertex array, without anchors                          */
  Gnum                      bndvertnum;
  Gnum                      bndvertlvlnum;        /* Based number of first band vertex in last layer                     */
  Gnum                      orgfronnum;
  int * restrict            orgflagtab;
  Gnum                      commload;
  Gnum                      commload2;            /* Twice twice (4 times) the internal communication load of last layer */
  Anum                      domnnum;
  Gnum * restrict           bndvnumtax;           /* Orignal numbers of vertices in band graph                           */
  Gnum *                    vertnbrtab;
  Gnum                      vertnum;
  int                       o;

  const Gnum * restrict const orgverttax = orggrafptr->s.verttax;
  const Gnum * restrict const orgvendtax = orggrafptr->s.vendtax;
  const Gnum * restrict const orgedgetax = orggrafptr->s.edgetax;
  const Gnum * restrict const orgedlotax = orggrafptr->s.edlotax;
  const Arch * restrict const archptr    = orggrafptr->m.archptr;
  const Anum                  domnnbr    = orggrafptr->m.domnnbr;
  Gnum * restrict const       orgparttax = orggrafptr->m.parttax;
  Gnum * restrict const       orgfrontab = orggrafptr->frontab;

  if (orggrafptr->fronnbr == 0)                   /* If no separator vertices, apply strategy to full (original) graph */
    return (kgraphMapSt (orggrafptr, paraptr->stratorg));

  if (kgraphBand (orggrafptr, paraptr->distmax, &bndgrafdat, &bndvertlvlnum, &bndvnumtax) != 0) {
    errorPrint ("kgraphMapBd: cannot create band graph");
    return (1);
  }

  if ((vertnbrtab = memAlloc (domnnbr * sizeof (Gnum))) == NULL) {
    errorPrint ("kgraphMapBd: out of memory (1)");
    memFree    (bndvnumtax + bndgrafdat.s.baseval);
    kgraphExit (&bndgrafdat);
    return (1);
  }
  memSet (vertnbrtab, 0, domnnbr * sizeof (Gnum));
  for (vertnum = orggrafptr->s.baseval; vertnum < orggrafptr->s.vertnnd; vertnum ++)
    vertnbrtab[orgparttax[vertnum]] ++;           /* TODO check optimize? */

  bndvertancnnd = bndgrafdat.s.vertnnd - domnnbr;
  for (domnnum = 0; domnnum < domnnbr; domnnum ++) { /* For all anchor domains */
    Gnum                vertnum;

    vertnum = bndvertancnnd + domnnum;
    if (((bndgrafdat.s.verttax[vertnum + 1] - bndgrafdat.s.verttax[vertnum]) == 0) &&
        (vertnbrtab[domnnum] != 0))
     break;
  }

  memFree (vertnbrtab);

  if (domnnum != domnnbr) {                       /* If graph is too small to have any usable anchors, apply org strategy */
    memFree    (bndvnumtax + bndgrafdat.s.baseval);
    kgraphExit (&bndgrafdat);
    return     (kgraphMapSt (orggrafptr, paraptr->stratorg));
  }

  bndgrafdat.m.domntab  = orggrafptr->m.domntab;  /* Transfer mapping domain array to band graph */
  orggrafptr->m.domntab = NULL;

  o = kgraphMapSt (&bndgrafdat, paraptr->stratbnd); /* Partition band graph */

  orggrafptr->m.domntab = bndgrafdat.m.domntab;   /* Transfer (potentially updated) mapping domain array back to original graph */
  orggrafptr->m.domnnbr = bndgrafdat.m.domnnbr;
  orggrafptr->m.domnmax = bndgrafdat.m.domnmax;
  bndgrafdat.m.domntab  = NULL;

  if (o != 0) {
    errorPrint  ("kgraphMapBd: cannot partition band graph");
    memFree     (bndvnumtax + bndgrafdat.s.baseval);
    kgraphExit  (&bndgrafdat);
    return (1);
  }
  if (bndgrafdat.m.domnnbr != orggrafptr->m.domnnbr) {
    errorPrint  ("kgraphMapBd: change in band graph number of parts not supported");
    memFree     (bndvnumtax + bndgrafdat.s.baseval);
    kgraphExit  (&bndgrafdat);
    return (1);
  }

  memCpy (orggrafptr->comploaddlt, bndgrafdat.comploaddlt, domnnbr * sizeof (Gnum)); /* Propagate back imbalance information */

  bndparttax = bndgrafdat.m.parttax;
  for (bndvertnum = bndgrafdat.s.baseval; bndvertnum < bndvertancnnd; bndvertnum ++) /* Update part array of all vertices except anchors */
    orgparttax[bndvnumtax[bndvertnum]] = bndparttax[bndvertnum];

  bndverttax = bndgrafdat.s.verttax;
  bndvendtax = bndgrafdat.s.vendtax;
  bndedgetax = bndgrafdat.s.edgetax;
  bndedlotax = bndgrafdat.s.edlotax;
  bnddomntab = orggrafptr->m.domntab;             /* Mapping domain array is shared between original and band graphs */

  commload = 0;
  for (bndvertnum = bndgrafdat.s.baseval; bndvertnum < bndvertlvlnum; bndvertnum ++) { /* For all vertices of band graph save for last layer */
    Gnum                bndedgenum;
    Gnum                bndedgennd;
    Anum                bndpartval;
    Anum                bndpartlst;               /* Part of last vertex for which a distance was computed */
    Anum                bnddistlst;               /* Last distance computed                                */

    bndpartval = bndparttax[bndvertnum];
    bndpartlst = -1;                              /* Invalid part to recompute distance */
    bnddistlst = -1;                              /* To prevent compiler from yelling   */

    for (bndedgenum = bndverttax[bndvertnum], bndedgennd = bndvendtax[bndvertnum];
	 bndedgenum < bndedgennd; bndedgenum ++) {
      Gnum                bndpartend;

      bndpartend = bndparttax[bndedgetax[bndedgenum]];

      if (bndpartval != bndpartend) {             /* TODO maybe can be optimized */
        Anum                bnddistval;

        bnddistval = (bndpartend != bndpartlst) ? archDomDist (archptr, &bnddomntab[bndpartval], &bnddomntab[bndpartend]) : bnddistlst;
        bndpartlst = bndpartend;
        bnddistlst = bnddistval;

        commload += (Gnum) bnddistval * ((bndedlotax != NULL) ? bndedlotax[bndedgenum] : 1);
      }
    }
  }
  for ( ; bndvertnum < bndvertancnnd; bndvertnum ++) { /* For all vertices of last layer, remove communication loads to band vertices once */
    Gnum                bndedgenum;
    Gnum                bndedgennd;
    Anum                bndpartval;
    Anum                bndpartlst;               /* Part of last vertex for which a distance was computed */
    Anum                bnddistlst;               /* Last distance computed                                */

    bndpartval = bndparttax[bndvertnum];
    bndpartlst = -1;                              /* Invalid part to recompute distance */
    bnddistlst = -1;                              /* To prevent compiler from yielding  */

    for (bndedgenum = bndverttax[bndvertnum], bndedgennd = bndvendtax[bndvertnum] - 1; /* "-1" to avoid anchor edges */
	 bndedgenum < bndedgennd; bndedgenum ++) {
      Gnum                bndpartend;

      bndpartend = bndparttax[bndedgetax[bndedgenum]];

      if (bndpartval != bndpartend) {
        Anum                bnddistval;

        bnddistval = (bndpartend != bndpartlst) ? archDomDist (archptr, &bnddomntab[bndpartval], &bnddomntab[bndpartend]) : bnddistlst;
        bndpartlst = bndpartend;
        bnddistlst = bnddistval;

        commload -= (Gnum) bnddistval * ((bndedlotax != NULL) ? bndedlotax[bndedgenum] : 1); /* Remove communication loads to band graph vertices once because afterwards they will be accounted for twice */
      }
    }
  }

  if ((orgflagtab = memAlloc (kgraphMapBdFlagSize (orggrafptr->s.vertnnd) * sizeof (unsigned int))) == NULL) {
    errorPrint ("kgraphMapBd: out of memory (2)");
    return (1);
  }

  memSet (orgflagtab, 0, kgraphMapBdFlagSize (orggrafptr->s.vertnnd) * sizeof (unsigned int)); /* Set vertices as not already considered */

  orgfronnum = 0;

  commload2 = 0;
  for (bndvertnum = bndgrafdat.s.baseval; bndvertnum < bndvertancnnd; bndvertnum ++) { /* For all vertices */
    Gnum                orgedgenum;
    Gnum                orgedgennd;
    Gnum                orgvertnum;
    Anum                orgpartval;
    int                 orgflagval;

    orgvertnum = bndvnumtax[bndvertnum];
    orgpartval = bndparttax[bndvertnum];

    orgflagval = 0;                               /* Assume vertex does not belong to the frontier */
    for (orgedgenum = orgverttax[orgvertnum], orgedgennd = orgvendtax[orgvertnum];
         orgedgenum < orgedgennd; orgedgenum ++) {
      Gnum                orgvertend;
      Gnum                orgpartend;
      Anum                orgdistval;

      orgvertend = orgedgetax[orgedgenum];
      orgpartend = orgparttax[orgvertend];
      orgdistval = archDomDist (archptr, &bnddomntab[orgpartval], &bnddomntab[orgpartend]);

      if (orgpartval != orgpartend) {
        orgflagval = 1;
        commload2 += ((orgedlotax != NULL) ? orgedlotax[orgedgenum] : 1) * orgdistval; /* Internal load to band and original graph vertices are accounted for twice */
        if ((orgvertend < orggrafptr->s.vertnnd) && (kgraphMapBdFlagVal (orgflagtab, orgvertend) == 0)) {
          orgfrontab[orgfronnum ++] = orgvertend;
          kgraphMapBdFlagSet (orgflagtab, orgvertend);
        }
      }
    }
    if ((orgflagval != 0) && (kgraphMapBdFlagVal (orgflagtab, orgvertnum) == 0))
      orgfrontab[orgfronnum ++] = orgvertnum;

    kgraphMapBdFlagSet (orgflagtab, orgvertnum);  /* Set vertex as processed anyway */
  }
  commload += 2 * commload2;                      /* Add twice the communication load of original graph edges and once the one of band edges (one removed before) */

  if (orggrafptr->pfixtax != NULL) {              /* Add fixed vertices with fixed neighbours only in the frontier array */
    Gnum                  vertnum;

    for (vertnum = orggrafptr->s.baseval; vertnum < orggrafptr->s.vertnnd; vertnum ++) {
      Gnum                partval;
      Gnum                edgenum;

      if ((orggrafptr->pfixtax[vertnum] == -1) || /* If it is not a fixed vertex            */
          (kgraphMapBdFlagVal (orgflagtab, vertnum) != 0)) /* Or has already been processed */
        continue;                                 /* Skip it                                */

      partval = orgparttax[vertnum];

      for (edgenum = orgverttax[vertnum]; edgenum < orgvendtax[vertnum]; edgenum ++) {
        if (orgparttax[orgedgetax[edgenum]] != partval) { /* If first vertex belongs to frontier */
          orgfrontab[orgfronnum] = vertnum;
          orgfronnum ++;
          break;
        }
      }
    }
  }
  orggrafptr->fronnbr = orgfronnum;
  orggrafptr->commload = commload / 2;

  memFree (orgflagtab);
  memFree (bndvnumtax + bndgrafdat.s.baseval);

  kgraphExit (&bndgrafdat);

  kgraphCost (orggrafptr);
#ifdef SCOTCH_DEBUG_KGRAPH2
  if (kgraphCheck (orggrafptr) != 0) {
    errorPrint ("kgraphMapBd: inconsistent graph data");
    return (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

  return (0);
}

