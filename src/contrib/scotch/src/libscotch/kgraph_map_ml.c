/* Copyright 2010,2011,2012,2014,2015,2018,2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_ml.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module maps an active graph        **/
/**                to a specific architecture graph        **/
/**                using a multi-level scheme.             **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 13 jul 2010     **/
/**                                 to   : 14 jul 2010     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 25 feb 2018     **/
/**                # Version 7.0  : from : 03 aug 2018     **/
/**                                 to   : 19 jul 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_KGRAPH_MAP_ML

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "graph_coarsen.h"
#include "kgraph.h"
#include "kgraph_map_ml.h"
#include "kgraph_map_st.h"

/*********************************************/
/*                                           */
/* The coarsening and uncoarsening routines. */
/*                                           */
/*********************************************/

/* This routine builds a coarser graph from the
** graph that is given on input. The coarser
** graphs differ at this stage from classical
** active graphs as their internal gains are not
** yet computed.
** It returns:
** - 0  : if the coarse graph has been built.
** - 1  : if threshold reached or on error.
*/

static
int
kgraphMapMlCoarsen (
Kgraph * restrict const               finegrafptr, /*+ Finer graph                                  +*/
Kgraph * restrict const               coargrafptr, /*+ Coarser graph to build                       +*/
GraphCoarsenMulti * restrict * const  coarmultptr, /*+ Pointer to un-based multinode table to build +*/
const KgraphMapMlParam * const        paraptr)    /*+ Method parameters                             +*/
{
  const Anum * restrict const finepfixtax = finegrafptr->pfixtax;

#ifdef SCOTCH_DEBUG_KGRAPH2
  if ((finegrafptr->comploadavg == NULL) ||
      (finegrafptr->comploaddlt == NULL)) {
    errorPrint ("kgraphMapMlCoarsen: internal error (1)");
    return (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

  *coarmultptr = NULL;                            /* Allocate coarmulttab along with coarse graph */
  if (graphCoarsen (&finegrafptr->s, &coargrafptr->s, NULL, coarmultptr, paraptr->coarnbr, paraptr->coarval, GRAPHCOARSENNOCOMPACT,
                    finegrafptr->r.m.parttax, finepfixtax, finegrafptr->vfixnbr, finegrafptr->contptr) != 0)
    return (1);

  coargrafptr->domnorg = finegrafptr->domnorg;    /* Keep initial domain */
  mapInit2 (&coargrafptr->m,   &coargrafptr->s, finegrafptr->m.archptr,   finegrafptr->m.domnmax,   finegrafptr->m.domnnbr);
  mapInit2 (&coargrafptr->r.m, &coargrafptr->s, finegrafptr->r.m.archptr, finegrafptr->r.m.domnmax, finegrafptr->r.m.domnnbr);

  coargrafptr->comploadavg = finegrafptr->comploadavg; /* By default, use fine target load arrays as coarse load arrays */
  coargrafptr->comploaddlt = finegrafptr->comploaddlt;
  coargrafptr->frontab     = finegrafptr->frontab; /* Share frontier array of finer graph as coarse frontier array (no freeing) */
  coargrafptr->contptr     = finegrafptr->contptr;

  coargrafptr->r.cmloval = finegrafptr->r.cmloval;
  coargrafptr->r.crloval = finegrafptr->r.crloval;
  if (finegrafptr->r.m.parttax != NULL) {
    Gnum * restrict     coarparotab;
    Gnum * restrict     coarvmlotab;
    Gnum                coarvertnum;

    const Gnum * restrict const         fineparotax = finegrafptr->r.m.parttax;
    const Gnum * restrict const         finevmlotax = finegrafptr->r.vmlotax;
    const Gnum                          coarvertnbr = coargrafptr->s.vertnbr;
    const GraphCoarsenMulti * restrict  coarmulttab = *coarmultptr;

    coargrafptr->r.m.domntab = finegrafptr->r.m.domntab; /* Re-use old mapping domain array in band graph (no freeing) */

    if (memAllocGroup ((void **) (void *)
                       &coarparotab, (size_t) (coarvertnbr * sizeof (Anum)),
                       &coarvmlotab, (size_t) (coarvertnbr * sizeof (Gnum)), NULL) == NULL) {
      errorPrint ("kgraphMapMlCoarsen: out of memory (1)");
      kgraphExit (coargrafptr);
      return (1);
    }
    coargrafptr->r.m.flagval = MAPPINGINCOMPLETE | MAPPINGFREEPART; /* Free group leader  */
    coargrafptr->r.m.parttax = coarparotab - coargrafptr->s.baseval; /* Set coarse arrays */
    coargrafptr->r.vmlotax   = coarvmlotab - coargrafptr->s.baseval;

    for (coarvertnum = 0; coarvertnum < coarvertnbr; coarvertnum ++) { /* Un-based traversal */
      Gnum                finevertnum0;
      Gnum                finevertnum1;

      finevertnum0 = coarmulttab[coarvertnum].vertnum[0];
      finevertnum1 = coarmulttab[coarvertnum].vertnum[1];
      coarparotab[coarvertnum] = fineparotax[finevertnum0];
      coarvmlotab[coarvertnum] = (finevmlotax != NULL)
                                 ? ((finevertnum0 == finevertnum1) ? 0 : finevmlotax[finevertnum1]) + finevmlotax[finevertnum0]
                                 : ((finevertnum0 == finevertnum1) ? 1 : 2);
#ifdef SCOTCH_DEBUG_KGRAPH2
      if ((fineparotax[finevertnum1] != fineparotax[finevertnum0]) && /* If vertices were not in the same part */
          ((finegrafptr->pfixtax == NULL) ||
           ((finepfixtax[finevertnum1] == -1) &&  /* And both are not fixed */
            (finepfixtax[finevertnum0] == -1)))) {
        errorPrint ("kgraphMapMlCoarsen: internal error (2)");
        return (1);
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
    }
  }
  else
    coargrafptr->r.vmlotax = NULL;

  if (finepfixtax != NULL) {                      /* If we have fixed vertices */
    Anum * restrict     coarpfixtab;
    Gnum                coarvfixnbr;
    Gnum                coarvertnbr;
    Gnum                coarvertnum;

    const GraphCoarsenMulti * restrict  coarmulttab = *coarmultptr;

    coarvertnbr = coargrafptr->s.vertnbr;
    if ((coarpfixtab = (Anum *) memAlloc (coarvertnbr * sizeof (Anum))) == NULL) {
      errorPrint ("kgraphMapMlCoarsen: out of memory (2)");
      kgraphExit (coargrafptr);
      return (1);
    }
    coargrafptr->s.flagval |= KGRAPHFREEPFIX;
    coargrafptr->pfixtax    = coarpfixtab - coargrafptr->s.baseval;

    coarvfixnbr = coarvertnbr;                    /* Assume all vertices are fixed */
    for (coarvertnum = 0; coarvertnum < coarvertnbr; coarvertnum ++) {
      Anum                coarpfixval;

      coarpfixval  = finepfixtax[coarmulttab[coarvertnum].vertnum[0]];
      coarvfixnbr += coarpfixval >> (sizeof (Anum) * 8 - 1); /* Accumulate -1's, that is, non-fixed vertices */

      coarpfixtab[coarvertnum] = coarpfixval;
#ifdef SCOTCH_DEBUG_KGRAPH2
      if (finepfixtax[coarmulttab[coarvertnum].vertnum[1]] != coarpfixval) {
        errorPrint ("kgraphMapMlCoarsen: internal error (3)");
        return (1);
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
    }
    coargrafptr->vfixnbr = coarvfixnbr;
  }
  else {
    coargrafptr->vfixnbr = 0;
    coargrafptr->pfixtax = NULL;
  }

  coargrafptr->comploadrat = finegrafptr->comploadrat;
  coargrafptr->kbalval     = finegrafptr->kbalval;
  coargrafptr->levlnum     = finegrafptr->levlnum + 1;

  return (0);
}

/* This routine propagates the partitioning of the
** coarser graph back to the finer graph, according
** to the multinode table of collapsed vertices.
** After the partitioning is propagated, it finishes
** to compute the parameters of the finer graph that
** were not computed at the coarsening stage.
** It returns:
** - 0   : if coarse graph data has been propagated to fine graph.
** - !0  : on error.
*/

static
int
kgraphMapMlUncoarsen (
Kgraph * restrict const         finegrafptr,      /*+ Finer graph                +*/
Kgraph * restrict const         coargrafptr,      /*+ Coarser graph              +*/
const GraphCoarsenMulti * const coarmulttab)      /*+ Pointer to multinode array +*/
{
  const Anum * restrict coarparttax;              /* Only known when coagrafptr is not NULL      */
  Gnum                  coarvertnnd;
  Gnum                  coarvertnum;
  Gnum * restrict       coarfrontab;              /* Coarse and fine frontiers arrays are merged */
  Gnum                  coarfronnbr;
  Gnum                  coarfronnum;
  Gnum                  finefronnum;
  Anum * restrict       fineparttax;              /* May not have been allocated yet             */

  const GraphCoarsenMulti * restrict const coarmulttax = coarmulttab - finegrafptr->s.baseval;
  const Gnum * restrict const              fineverttax = finegrafptr->s.verttax;
  const Gnum * restrict const              finevendtax = finegrafptr->s.vendtax;
  const Gnum * restrict const              fineedgetax = finegrafptr->s.edgetax;

  if (coargrafptr == NULL) {                      /* If no coarse graph provided             */
    if (mapAlloc (&finegrafptr->m) != 0) {        /* Allocate mapping arrays at lowest level */
      errorPrint ("kgraphMapMlUncoarsen: cannot allocate mapping arrays (1)");
      return (1);
    }
    kgraphFrst (finegrafptr);                     /* Assign all vertices to first subdomain */
    return (0);
  }

  if (mapAlloc (&finegrafptr->m) != 0) {          /* Allocate partition array if needed */
    errorPrint ("kgraphMapMlUncoarsen: cannot allocate mapping arrays (2)");
    return (1);
  }

  finegrafptr->s.flagval  |= KGRAPHFREECOMP;
  finegrafptr->comploadavg = coargrafptr->comploadavg; /* Propagate part load data in case it was changed at the coarser levels */
  finegrafptr->comploaddlt = coargrafptr->comploaddlt;
  coargrafptr->comploadavg = NULL;                /* No need to free coarse graph load array as it has been transferred */

  fineparttax = finegrafptr->m.parttax;           /* Fine part array is now allocated */
  coarparttax = coargrafptr->m.parttax;
  coarfrontab = coargrafptr->frontab;
  for (coarvertnum = coargrafptr->s.baseval, coarvertnnd = coargrafptr->s.vertnnd;
       coarvertnum < coarvertnnd; coarvertnum ++) {
    Gnum                finevertnum0;             /* First multinode vertex  */
    Gnum                finevertnum1;             /* Second multinode vertex */
    Anum                partval;

    finevertnum0 = coarmulttax[coarvertnum].vertnum[0];
    finevertnum1 = coarmulttax[coarvertnum].vertnum[1];
    partval      = coarparttax[coarvertnum];

    fineparttax[finevertnum0] = partval;
    if (finevertnum0 != finevertnum1)
      fineparttax[finevertnum1] = partval;
  }

  finegrafptr->commload = coargrafptr->commload;

  for (coarfronnum = 0, finefronnum = coarfronnbr = coargrafptr->fronnbr; /* Re-cycle frontier array from coarse to fine graph */
       coarfronnum < coarfronnbr; coarfronnum ++) {
    Gnum                coarvertnum;
    Gnum                finevertnum0;             /* First multinode vertex  */
    Gnum                finevertnum1;             /* Second multinode vertex */

    coarvertnum  = coarfrontab[coarfronnum];
    finevertnum0 = coarmulttax[coarvertnum].vertnum[0];
    finevertnum1 = coarmulttax[coarvertnum].vertnum[1];

    if (finevertnum0 != finevertnum1) {           /* If multinode si made of two distinct vertices */
      Gnum                fineedgenum;
      Gnum                partval;

      partval = coarparttax[coarvertnum];

#ifdef SCOTCH_DEBUG_KGRAPH2
      coarfrontab[coarfronnum] = ~0;
#endif /* SCOTCH_DEBUG_KGRAPH2 */

      for (fineedgenum = fineverttax[finevertnum0];
           fineedgenum < finevendtax[finevertnum0]; fineedgenum ++) {
        if (fineparttax[fineedgetax[fineedgenum]] != partval) { /* If first vertex belongs to frontier */
          coarfrontab[coarfronnum] = finevertnum0; /* Record it in lieu of the coarse frontier vertex  */
          break;
        }
      }
      if (fineedgenum >= finegrafptr->s.vendtax[finevertnum0]) { /* If first vertex not in frontier */
        coarfrontab[coarfronnum] = finevertnum1;  /* Then second vertex must be in frontier         */
        continue;                                 /* Skip to next multinode                         */
      }

      for (fineedgenum = fineverttax[finevertnum1]; /* Check if second vertex also belongs to frontier */
           fineedgenum < finevendtax[finevertnum1]; fineedgenum ++) {
        if (fineparttax[fineedgetax[fineedgenum]] != partval) { /* If second vertex belongs to frontier      */
          coarfrontab[finefronnum ++] = finevertnum1; /* Record it at the end of the recycled frontier array */
          break;
        }
      }

#ifdef SCOTCH_DEBUG_KGRAPH2
      if (coarfrontab[coarfronnum] == ~0) {
        errorPrint ("kgraphMapMlUncoarsen: internal error");
        return (1);
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
    }
    else                                          /* If coarse vertex is single node */
      coarfrontab[coarfronnum] = finevertnum0;    /* Then it belongs to the frontier */
  }
  finegrafptr->fronnbr = finefronnum;

#ifdef SCOTCH_DEBUG_KGRAPH2
  if (kgraphCheck (finegrafptr) != 0) {
    errorPrint ("kgraphMapMlUncoarsen: inconsistent graph data");
    return (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

  return (0);
}

/* This routine performs the
** partitioning recursion.
** It returns:
** - 0 : if partitioning could be computed.
** - 1 : on error.
*/

static
int
kgraphMapMl2 (
Kgraph * restrict const           grafptr,        /*+ Active graph      +*/
const KgraphMapMlParam * const    paraptr)        /*+ Method parameters +*/
{
  Kgraph              coargrafdat;
  GraphCoarsenMulti * coarmulttab;                /* Pointer to un-based multinode array */
  int                 o;

  if (kgraphMapMlCoarsen (grafptr, &coargrafdat, &coarmulttab, paraptr) == 0) {
    coargrafdat.m.flagval |= MAPPINGFREEDOMN;     /* Transfer ownership of mapping domain array to coarse graph */
    coargrafdat.m.domntab  = grafptr->m.domntab;
    grafptr->m.domntab     = NULL;

    o = kgraphMapMl2 (&coargrafdat, paraptr);     /* Compute mapping on coarsened graph */

    grafptr->m.flagval    = coargrafdat.m.flagval; /* Transfer (back) mapping domain array to fine graph */
    grafptr->m.domntab    = coargrafdat.m.domntab;
    grafptr->m.domnnbr    = coargrafdat.m.domnnbr;
    grafptr->m.domnmax    = coargrafdat.m.domnmax;
    coargrafdat.m.domntab = NULL;

    if ((o == 0) &&                               /* If coarsened mapping succeeded */
        ((o = kgraphMapMlUncoarsen (grafptr, &coargrafdat, coarmulttab)) == 0) &&
        ((o = kgraphMapSt          (grafptr, paraptr->stratasc))         != 0)) /* Apply ascending strategy */
      errorPrint ("kgraphMapMl2: cannot apply ascending strategy");
    kgraphExit (&coargrafdat);
  }
  else {                                          /* Cannot coarsen due to lack of memory or error */
    if (((o = kgraphMapMlUncoarsen (grafptr, NULL, NULL))        == 0) && /* Finalize graph        */
        ((o = kgraphMapSt          (grafptr, paraptr->stratlow)) != 0)) /* Apply low strategy      */
      errorPrint ("kgraphMapMl2: cannot apply low strategy");
  }

  return (o);
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine performs the multi-level mapping.
** It returns:
** - 0 : if mapping could be computed.
** - 1 : on error.
*/

int
kgraphMapMl (
Kgraph * const                  grafptr,          /*+ Active graph      +*/
const KgraphMapMlParam * const  paraptr)          /*+ Method parameters +*/
{
  Gnum                levlnum;                    /* Save value for graph level */
  int                 o;

  levlnum = grafptr->levlnum;                     /* Save graph level            */
  grafptr->levlnum = 0;                           /* Initialize coarsening level */
  o = kgraphMapMl2 (grafptr, paraptr);            /* Perform multi-level mapping */
  grafptr->levlnum = levlnum;                     /* Restore graph level         */

  return (o);
}

