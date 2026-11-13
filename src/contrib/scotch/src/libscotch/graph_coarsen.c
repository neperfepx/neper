/* Copyright 2004,2007,2009,2011-2016,2018,2020,2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_coarsen.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the source graph   **/
/**                coarsening functions.                   **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 01 dec 1992     **/
/**                                 to   : 18 may 1993     **/
/**                # Version 1.3  : from : 30 apr 1994     **/
/**                                 to   : 18 may 1994     **/
/**                # Version 2.0  : from : 06 jun 1994     **/
/**                                 to   : 31 oct 1994     **/
/**                # Version 3.0  : from : 07 jul 1995     **/
/**                                 to   : 28 sep 1995     **/
/**                # Version 3.1  : from : 28 nov 1995     **/
/**                                 to   : 08 jun 1996     **/
/**                # Version 3.2  : from : 07 sep 1996     **/
/**                                 to   : 17 sep 1998     **/
/**                # Version 4.0  : from : 13 dec 2001     **/
/**                                 to   : 31 aug 2005     **/
/**                # Version 5.0  : from : 13 dec 2006     **/
/**                                 to   : 24 mar 2008     **/
/**                # Version 5.1  : from : 30 oct 2009     **/
/**                                 to   : 30 oct 2009     **/
/**                # Version 6.0  : from : 09 mar 2011     **/
/**                                 to   : 29 apr 2019     **/
/**                # Version 7.0  : from : 28 jul 2018     **/
/**                                 to   : 19 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_GRAPH_COARSEN

#include "module.h"
#include "common.h"
#include "arch.h"
#include "graph.h"
#include "graph_coarsen.h"
#include "graph_match.h"

/****************************************/
/*                                      */
/* The edge array building subroutines. */
/*                                      */
/****************************************/

#define GRAPHCOARSENEDGENAME        graphCoarsenEdgeLl
#define GRAPHCOARSENEDLOTAB
#include "graph_coarsen_edge.c"
#undef GRAPHCOARSENEDGENAME
#undef GRAPHCOARSENEDLOTAB

#define GRAPHCOARSENEDGENAME        graphCoarsenEdgeLu
#include "graph_coarsen_edge.c"
#undef GRAPHCOARSENEDGENAME

#ifndef GRAPHCOARSENNOTHREAD                      /* Only relevant when threads are enabled */
#define GRAPHCOARSENEDGENAME        graphCoarsenEdgeCt
#define GRAPHCOARSENEDGECOUNT                     /* Local coarse edge count routine */
#include "graph_coarsen_edge.c"
#undef GRAPHCOARSENEDGENAME
#undef GRAPHCOARSENEDGECOUNT
#endif /* GRAPHCOARSENNOTHREAD */

/***************************/
/*                         */
/* The coarsening routine. */
/*                         */
/***************************/

#ifndef GRAPHCOARSENNOTHREAD

/* This routine aggregates a sum and max
** reduction of partial coarse graph
** parameters computed by multiple
** threads.
*/

static
void
graphCoarsenReduce (
GraphCoarsenThread * restrict const tlocptr,      /* Pointer to local thread block  */
GraphCoarsenThread * restrict const tremptr,      /* Pointer to remote thread block */
const void * const                  globptr)      /* Unused                         */
{
  tlocptr->coaredgebas += tremptr->coaredgebas;   /* Sum number of local edges     */
  tlocptr->coaredloadj += tremptr->coaredloadj;   /* Sum edge load sum adjustments */
  if (tremptr->coardegrmax > tlocptr->coardegrmax) /* Take maximum of degrees      */
    tlocptr->coardegrmax = tremptr->coardegrmax;
}

/* This routine performs a prefix scan
** sum operation on a single Gnum value,
** backed by a temporary area.
*/

static
void
graphCoarsenScan (
Gnum * restrict const       tlocptr,              /* Pointer to local area   */
Gnum * restrict const       tremptr,              /* Pointer to lremote area */
const int                   srcpval,              /* Source phase value      */
const int                   dstpval,              /* Destination phase value */
const void * const          globptr)              /* Unused                  */
{
  tlocptr[dstpval] = tlocptr[srcpval] + ((tremptr == NULL) ? 0 : tremptr[srcpval]);
}
#endif /* GRAPHCOARSENNOTHREAD */

/* This routine is the threaded core of the building
** of the coarse graph from the fine graph.
** It returns:
** - void: in all cases.
*/

static
void
graphCoarsen3 (
ThreadDescriptor * restrict const descptr,
GraphCoarsenData * restrict const coarptr)
{
  GraphCoarsenMulti * restrict  coarmulttax;      /* Work pointer to multinode array    */
  Gnum                          coarvertnbr;
  Gnum                          coarhashnbr;      /* Size of neighbor vertex hash table */
  Gnum                          coaredgebas;      /* Start of local edge sub-array      */

#ifdef SCOTCH_PTHREAD
  const int                           thrdnbr     = threadNbr (descptr);
  const int                           thrdnum     = threadNum (descptr);
  GraphCoarsenThread * const          thrdptr     = &coarptr->thrdtab[thrdnum];
#else /* SCOTCH_PTHREAD */
  GraphCoarsenThread * restrict const thrdptr     = &coarptr->thrdtab[0];
#endif /* SCOTCH_PTHREAD */
  const Graph * restrict const        finegrafptr = coarptr->finegrafptr;
  Gnum * const                        finecoartax = coarptr->finematetax; /* [norestrict] */
  Graph * const                       coargrafptr = coarptr->coargrafptr; /* [norestrict] */
  const Gnum                          baseval     = finegrafptr->baseval;

#ifdef SCOTCH_PTHREAD
  thrdptr->finevertbas = baseval + DATASCAN (finegrafptr->vertnbr, thrdnbr, thrdnum);
  thrdptr->finevertnnd = baseval + DATASCAN (finegrafptr->vertnbr, thrdnbr, thrdnum + 1);
#else /* SCOTCH_PTHREAD */
  thrdptr->finevertbas = baseval;
  thrdptr->finevertnnd = finegrafptr->vertnnd;
#endif /* SCOTCH_PTHREAD */

  if ((coarptr->flagval & GRAPHCOARSENUSEMATE) == 0) { /* If matching data not provided */
    graphMatch (descptr, coarptr);                /* Perform threaded matching          */

    if ((coarptr->retuval != 0) ||                /* If matching failed                    */
        (coargrafptr == NULL))                    /* Or if only matching wanted, stop here */
      return;

    coarvertnbr = coarptr->coarvertnbr;           /* Get number of vertices actually created    */
    if (coarvertnbr >= coarptr->coarvertmax) {    /* If coarsened graph is too large, stop here */
#ifdef SCOTCH_PTHREAD
      if (thrdnum == 0)                           /* Thread 0 sets the return value */
#endif /* SCOTCH_PTHREAD */
        coarptr->retuval = 1;
      return;
    }
  }
  else
    coarvertnbr = coarptr->coarvertnbr;           /* Get provided number of vertices */

#ifdef GRAPHCOARSENNOTHREAD
#ifdef SCOTCH_PTHREAD
  if (thrdnum != 0) {                             /* Only thread 0 will perform the work    */
    threadBarrier (descptr);                      /* End-of-routine synchronization barrier */
    return;
  }
#endif /* SCOTCH_PTHREAD */
  thrdptr->finevertbas = baseval;                 /* Now thread 0 takes care of all vertices */
  thrdptr->finevertnnd = finegrafptr->vertnnd;
#else /* GRAPHCOARSENNOTHREAD */
  if (thrdnum == 0)                               /* Thread 0 populates the graph data structure */
#endif /* GRAPHCOARSENNOTHREAD */
  {
    GraphCoarsenMulti * coarmulttab;              /* [norestrict]                     */
    Gnum                coarmultsiz;              /* Size of embedded multinode array */
    Gnum                coarvendsiz;
#ifdef SCOTCH_DEBUG_GRAPH2
    Gnum                finevertnum;
    Gnum                finevertnnd;

    for (finevertnum = thrdptr->finevertbas, finevertnnd = thrdptr->finevertnnd;
         finevertnum < finevertnnd; finevertnum ++) {
      Gnum                finematenum;            /* Number of current mate vertex */

      finematenum = finecoartax[finevertnum];
      if ((finematenum < baseval) || (finematenum >= finegrafptr->vertnnd)) {
        errorPrint ("graphCoarsen3: invalid matching (1)");
        return;
      }
      if (finecoartax[finematenum] != finevertnum) {
        errorPrint ("graphCoarsen3: invalid matching (2)");
        return;
      }
    }
#endif /* SCOTCH_DEBUG_GRAPH2 */

    coarvendsiz = ((coarptr->flagval & GRAPHCOARSENNOCOMPACT) != 0) ? coarvertnbr : 1; /* TRICK: If not a compact graph, allocate a whole array for vendtab */
    coarmultsiz = ((coarptr->flagval & GRAPHCOARSENHASMULT)   == 0) ? coarvertnbr : 0; /* If coarmulttab is not user-provided, allocate it among graph data */

    memSet (coargrafptr, 0, sizeof (Graph));      /* Initialize coarse graph on thread 0 */
    coargrafptr->flagval = GRAPHFREEVERT | GRAPHVERTGROUP | GRAPHFREEEDGE;
    coargrafptr->baseval = baseval;
    coargrafptr->vertnbr = coarvertnbr;
    coargrafptr->vertnnd = coarvertnbr + baseval;
    coargrafptr->velosum = finegrafptr->velosum;  /* Keep load of finer graph         */
    if ((memAllocGroup ((void **) (void *)        /* Allocate coarser graph structure */
                        &coargrafptr->verttax, (size_t) (coarvertnbr * sizeof (Gnum)),
                        &coargrafptr->vendtax, (size_t) (coarvendsiz * sizeof (Gnum)),
                        &coargrafptr->velotax, (size_t) (coarvertnbr * sizeof (Gnum)),
                        &coarmulttab,          (size_t) (coarmultsiz * sizeof (GraphCoarsenMulti)), NULL) == NULL) ||
        ((coargrafptr->edgetax = memAlloc (finegrafptr->edgenbr * 2 * sizeof (Gnum))) == NULL)) { /* "* 2" for edlotab */
      errorPrint ("graphCoarsen3: out of memory (1)");
      if (coargrafptr->verttax != NULL)
        memFree (coargrafptr->verttax);
      coarptr->retuval = 2;
#ifdef GRAPHCOARSENNOTHREAD
#ifdef SCOTCH_PTHREAD
      threadBarrier (descptr);                    /* End-of-routine synchronization barrier for thread 0 */
#endif /* SCOTCH_PTHREAD */
      return;                                     /* Thread 0 returns */
#endif /* GRAPHCOARSENNOTHREAD */
    }
    else {
      coargrafptr->verttax -= baseval;            /* Base coarse graph arrays */
      coargrafptr->vendtax  = ((coarptr->flagval & GRAPHCOARSENNOCOMPACT) != 0) ? (coargrafptr->vendtax - baseval) : (coargrafptr->verttax + 1);
      coargrafptr->velotax -= baseval;
      coargrafptr->edgetax -= baseval;
      coargrafptr->edlotax  = coargrafptr->edgetax + finegrafptr->edgenbr;
      if (coarmultsiz > 0)                        /* If array created internally, record its location */
        coarptr->coarmulttab = coarmulttab;       /* Record un-based array location                   */
    }
  }

#ifndef GRAPHCOARSENNOTHREAD
  if (thrdnbr > 1) {                              /* If more than one thread */
    Gnum                finevertnum;
    Gnum                finevertnnd;
    Gnum                coarvertnnd;
    Gnum                coarvertnum;

    for (finevertnum = thrdptr->finevertbas, finevertnnd = thrdptr->finevertnnd, coarvertnum = 0;
         finevertnum < finevertnnd; finevertnum ++) {
      Gnum                finematenum;            /* Number of current mate vertex */

      finematenum = finecoartax[finevertnum];     /* Get mate number                  */
      if (finematenum >= finevertnum)             /* If mate has larger number        */
        coarvertnum ++;                           /* One more local multinode created */
    }

    thrdptr->scantab[0] = coarvertnum;
    threadScan (descptr, (void * const) &thrdptr->scantab[0], sizeof (GraphCoarsenThread), (ThreadScanFunc) graphCoarsenScan, NULL); /* Compute start indices for multinodes; barrier for coarptr->coarmulttab */

    if (coarptr->retuval != 0)                    /* After scan barrier, in case memory allocation failed */
      return;

#ifdef SCOTCH_DEBUG_GRAPH2
    if ((thrdnum == (thrdnbr - 1)) &&
        (thrdptr->scantab[0] != coarvertnbr)) {
      errorPrint ("graphCoarsen3: internal error (1)");
      return;
    }
#endif /* SCOTCH_DEBUG_GRAPH2 */
    coarmulttax = coarptr->coarmulttab - baseval; /* All threads know coarptr->coarmulttab after scan barrier       */
    coarvertnum = thrdptr->scantab[0] - coarvertnum + baseval; /* Local start of coarse vertex numbering after scan */
    for (finevertnum = thrdptr->finevertbas;
         finevertnum < finevertnnd; finevertnum ++) {
      Gnum                finematenum;            /* Number of current mate vertex */

      finematenum = finecoartax[finevertnum];     /* Get mate number                      */
      if (finematenum >= finevertnum) {           /* If mate has larger number            */
        coarmulttax[coarvertnum].vertnum[0] = finevertnum; /* Build new multinode         */
        coarmulttax[coarvertnum].vertnum[1] = finematenum; /* Second index always biggest */
        coarvertnum ++;                           /* One more local multinode created     */
      }
    }

    thrdptr->coarvertbas = baseval + DATASCAN (coarvertnbr, thrdnbr, thrdnum); /* Set bounds for coarse vertex processing */
    thrdptr->coarvertnnd = baseval + DATASCAN (coarvertnbr, thrdnbr, thrdnum + 1);

    threadBarrier (descptr);                      /* Ensure all of coarmulttax has been written */

    for (coarvertnum = thrdptr->coarvertbas, coarvertnnd = thrdptr->coarvertnnd;
         coarvertnum < coarvertnnd; coarvertnum ++) {
      finecoartax[coarmulttax[coarvertnum].vertnum[0]] = /* Build fine-to-coarse array */
      finecoartax[coarmulttax[coarvertnum].vertnum[1]] = coarvertnum;
    }
  }
  else
#endif /* GRAPHCOARSENNOTHREAD */
  {
    Gnum                finevertnnd;
    Gnum                finevertnum;
    Gnum                coarvertnum;

#ifndef GRAPHCOARSENNOTHREAD
    if (coarptr->retuval != 0)                    /* In case memory allocation failed */
      return;
#endif /* GRAPHCOARSENNOTHREAD */

    coarmulttax = coarptr->coarmulttab - baseval; /* Only thread 0 knows coarptr->coarmulttab */
    for (finevertnum = thrdptr->finevertbas, finevertnnd = thrdptr->finevertnnd, coarvertnum = baseval; /* Finalize finecoartab array */
         finevertnum < finevertnnd; finevertnum ++) {
      Gnum                finematenum;            /* Number of current mate vertex */

      finematenum = finecoartax[finevertnum];     /* Get mate number                               */
      if (finematenum >= finevertnum) {           /* If mate has larger number                     */
        coarmulttax[coarvertnum].vertnum[0] = finevertnum; /* Build new multinode                  */
        coarmulttax[coarvertnum].vertnum[1] = finematenum; /* Second index always biggest          */
        finecoartax[finematenum] =                /* Point to coarse vertex                        */
        finecoartax[finevertnum] = coarvertnum;   /* Always valid since coarvertnum <= finevertnum */
        coarvertnum ++;                           /* One more multinode created                    */
      }
    }
#ifdef SCOTCH_DEBUG_GRAPH2
    if (coarvertnum != (coarvertnbr + baseval)) {
      errorPrint ("graphCoarsen3: internal error (2)");
      return;
    }
#endif /* SCOTCH_DEBUG_GRAPH2 */

    thrdptr->coarvertbas = baseval;               /* Set bounds for coarse vertex processing */
    thrdptr->coarvertnnd = coarvertnbr + baseval;
  }

  coarhashnbr = coarptr->coarhashmsk + 1;
  if ((thrdptr->coarhashtab = memAlloc (coarhashnbr * sizeof (GraphCoarsenHash))) == NULL) { /* Allocate local thread memory */
    errorPrint ("graphCoarsen3: out of memory (2)");
    coarptr->retuval = 2;                         /* No problem if concurrent writes */
#ifdef GRAPHCOARSENNOTHREAD
#ifdef SCOTCH_PTHREAD
    threadBarrier (descptr);                      /* End-of-routine synchronization barrier */
#endif /* SCOTCH_PTHREAD */
    return;                                       /* Thread 0 returns */
#endif /* GRAPHCOARSENNOTHREAD */
  }
  else
    memSet (thrdptr->coarhashtab, ~0, coarhashnbr * sizeof (GraphCoarsenHash)); /* Initialize (local) hash table */

#ifndef GRAPHCOARSENNOTHREAD
  if (thrdnbr > 1) {                              /* If more than one thread                                      */
    threadBarrier (descptr);                      /* Ensure all of finecoartax has been written (or memory error) */

    if (coarptr->retuval != 0)                    /* After barrier, in case memory allocation failed */
      return;

    if ((coarptr->flagval & GRAPHCOARSENNOCOMPACT) != 0) { /* If willing to have faster a non-compact graph */
      Gnum                coarvertnnd;
      Gnum                coarvertnum;
      Gnum                coaredgenbr;

      const Gnum * restrict const fineverttax = finegrafptr->verttax;
      const Gnum * restrict const finevendtax = finegrafptr->vendtax;

      for (coarvertnum = thrdptr->coarvertbas,    /* For all local coarse vertices */
           coarvertnnd = thrdptr->coarvertnnd, coaredgenbr = 0;
           coarvertnum < coarvertnnd; coarvertnum ++) {
        Gnum                finevertnum;
        int                 i;

        i = 0;
        do {                                      /* For all fine edges of multinode vertices */
          finevertnum  = coarmulttax[coarvertnum].vertnum[i];
          coaredgenbr += finevendtax[finevertnum] - fineverttax[finevertnum];
        } while (i ++, finevertnum != coarmulttax[coarvertnum].vertnum[1]); /* Skip to next matched vertex if both vertices not equal */
      }
      thrdptr->coaredgebas = coaredgenbr;         /* Save upper bound on local number of coarse edges for scan */
    }
    else {
      thrdptr->coaredgebas = 0;                   /* No coarse edges accounted for yet                                */
      graphCoarsenEdgeCt (coarptr, thrdptr);      /* Count number of coarse local edges in thrdptr->coaredgebas       */
      memSet (thrdptr->coarhashtab, ~0, coarhashnbr * sizeof (GraphCoarsenHash)); /* Re-initialize (local) hash table */
    }
    thrdptr->scantab[0] = thrdptr->coaredgebas;
    threadScan (descptr, &thrdptr->scantab[0], sizeof (GraphCoarsenThread), (ThreadScanFunc) graphCoarsenScan, NULL); /* Compute scan on coarse edge indices */
#ifdef SCOTCH_DEBUG_GRAPH2
    if (thrdptr->scantab[0] > finegrafptr->edgenbr) {
      errorPrint ("graphCoarsen3: internal error (3)");
      return;
    }
    if (((coarptr->flagval & GRAPHCOARSENNOCOMPACT) != 0) &&
	(thrdnum == (thrdnbr - 1)) &&
        (thrdptr->scantab[0] != finegrafptr->edgenbr)) {
      errorPrint ("graphCoarsen3: internal error (4)");
      return;
    }
#endif /* SCOTCH_DEBUG_GRAPH2 */
    thrdptr->coaredgebas = thrdptr->scantab[0] - thrdptr->coaredgebas + baseval; /* Adjust value to have real edge start index */
  }
  else
#endif /* GRAPHCOARSENNOTHREAD */
  {
#ifndef GRAPHCOARSENNOTHREAD
    if (coarptr->retuval != 0)                    /* In case memory allocation failed */
      return;
#endif /* GRAPHCOARSENNOTHREAD */

    thrdptr->coaredgebas = baseval;               /* Start from the beginning */
  }
  coaredgebas = thrdptr->coaredgebas;             /* Record edge start index */

  ((finegrafptr->edlotax != NULL) ? graphCoarsenEdgeLl : graphCoarsenEdgeLu) (coarptr, thrdptr); /* Build coarse graph edge array */

  memFree (thrdptr->coarhashtab);                 /* Free local hash table */

  thrdptr->coaredgebas -= coaredgebas;            /* Compute accurate number of local edges */

#ifndef GRAPHCOARSENNOTHREAD
  if (thrdnbr > 1)
    threadReduce (descptr, thrdptr, sizeof (GraphCoarsenThread), (ThreadReduceFunc) graphCoarsenReduce, 0, NULL); /* Sum edloadj and get maximum of degrmax */

  if (thrdnum == 0)
#endif /* GRAPHCOARSENNOTHREAD */
  {
    coargrafptr->edgenbr = thrdptr->coaredgebas;
    coargrafptr->edlosum = thrdptr->coaredloadj + finegrafptr->edlosum;
    coargrafptr->degrmax = thrdptr->coardegrmax;
#ifndef GRAPHCOARSENNOTHREAD
    if ((coarptr->flagval & GRAPHCOARSENNOCOMPACT) == 0) /* If graph is compact */
#endif /* GRAPHCOARSENNOTHREAD */
    {
      size_t              coaredlooft;
      byte *              coaredgetab;

      coargrafptr->verttax[coargrafptr->vertnnd] = coargrafptr->edgenbr + finegrafptr->baseval; /* Mark end of edge array */

      coaredlooft = (byte *) coargrafptr->edlotax - (byte *) coargrafptr->edgetax;
      coaredgetab = memRealloc (coargrafptr->edgetax + baseval, coaredlooft + (coargrafptr->edgenbr * sizeof (Gnum)));
      coargrafptr->edgetax = (Gnum *) coaredgetab - baseval;
      coargrafptr->edlotax = (Gnum *) (coaredgetab + coaredlooft) - baseval;
    }
  }

#ifdef GRAPHCOARSENNOTHREAD
#ifdef SCOTCH_PTHREAD
  threadBarrier (descptr);                        /* End-of-routine synchronization barrier for thread 0 */
#endif /* SCOTCH_PTHREAD */
#endif /* GRAPHCOARSENNOTHREAD */
}

/* This routine is the sequential core of the
** matching and coarse graph building process.
** It returns:
** - 0  : if the graph has been coarsened.
** - 1  : if the graph could not be coarsened.
** - 2  : on error.
*/

static
int
graphCoarsen2 (
GraphCoarsenData * restrict const     coarptr)
{
  Gnum *              finematetab;                /* Pointer to locally allocated mate array  */
  Gnum                coarhashmsk;                /* Mask for access to hash table            */

  Graph * restrict const        coargrafptr = coarptr->coargrafptr;
  const Graph * restrict const  finegrafptr = coarptr->finegrafptr;
  const Gnum                    finevertnbr = finegrafptr->vertnbr;
  const Gnum                    baseval     = finegrafptr->baseval;
#ifdef SCOTCH_PTHREAD
  const int                     thrdnbr     = contextThreadNbr (coarptr->contptr);
#else /* SCOTCH_PTHREAD */
  const int                     thrdnbr     = 1;
#endif /* SCOTCH_PTHREAD */

  for (coarhashmsk = 31; coarhashmsk < finegrafptr->degrmax; coarhashmsk = coarhashmsk * 2 + 1) ; /* Compute size of hash table */
  coarptr->coarhashmsk = coarhashmsk * 4 + 3;     /* Record it for (local) hash table allocation */

  finematetab = NULL;                             /* Assume mating array provided     */
  if (coarptr->finematetax == NULL) {             /* If no user-provided mating array */
    if ((finematetab = (Gnum *) memAlloc (finevertnbr * sizeof (Gnum))) == NULL) {
      errorPrint ("graphCoarsen2: out of memory (1)"); /* Allocate coarse graph mating and indexing array */
      return     (2);
    }
    coarptr->finematetax = finematetab - baseval;
  }

#ifndef GRAPHCOARSENNOTHREAD
  if (thrdnbr <= 1)                               /* If no multithreading, coarse graph will always be compact */
#endif /* GRAPHCOARSENNOTHREAD */
    coarptr->flagval &= ~GRAPHCOARSENNOCOMPACT;   /* Non-compact graphs always imply more than one thread */

  if ((coarptr->flagval & GRAPHCOARSENUSEMATE) == 0) { /* If mating array not provided          */
    if (graphMatchInit (coarptr, thrdnbr) != 0)   /* Initialize global data needed for matching */
      return (2);
  }

  if (coarptr->coarmulttab != NULL)               /* Record that multinode array was provided */
    coarptr->flagval |= GRAPHCOARSENHASMULT;

  if ((coarptr->thrdtab = memAlloc (thrdnbr * sizeof (GraphCoarsenThread))) == NULL) {
    errorPrint ("graphCoarsen2: out of memory (2)");
    if (finematetab != NULL)
      memFree (finematetab);
    return (2);
  }
  coarptr->retuval = 0;                           /* Assume no error */

  contextThreadLaunch (coarptr->contptr, (ThreadFunc) graphCoarsen3, (void *) coarptr);

  memFree (coarptr->thrdtab);

  if ((coarptr->flagval & GRAPHCOARSENDSTMATE) == 0) /* If mating array destination not provided */
    memFree (finematetab);                        /* Do not keep mating data array               */

  if (coarptr->coarvertnbr >= coarptr->coarvertmax) /* If coarsened graph is said to be too small, return here */
    return (coarptr->retuval);

  if (coargrafptr == NULL)                        /* If coarse graph not wanted */
    return (0);

  if (coargrafptr->verttax == NULL)               /* If graph could not be created */
    return (coarptr->retuval);

#ifdef SCOTCH_DEBUG_GRAPH2
  if (graphCheck (coargrafptr) != 0) {            /* Check graph consistency */
    errorPrint ("graphCoarsen2: inconsistent graph data");
    graphFree  (coargrafptr);
    return     (2);
  }
#endif /* SCOTCH_DEBUG_GRAPH2 */

  return (0);
}

/* This routine coarsens the given "finegraph" into
** "coargraph", as long as the coarsening ratio remains
** below some threshold value and the coarsened graph
** is not too small.
** It returns:
** - 0  : if the graph has been coarsened.
** - 1  : if the graph could not be coarsened.
** - 2  : on error.
*/

int
graphCoarsen (
const Graph * restrict const                  finegrafptr, /*+ Graph to coarsen                                  +*/
Graph * restrict const                        coargrafptr, /*+ Coarse graph to build                             +*/
Gnum * restrict * restrict const              finecoarptr, /*+ Pointer to un-based fine-to-coarse array to build +*/
GraphCoarsenMulti * restrict * restrict const coarmultptr, /*+ Pointer to un-based multinode table to build      +*/
const Gnum                                    coarvertnbr, /*+ Minimum number of coarse vertices                 +*/
const double                                  coarval, /*+ Maximum contraction ratio                             +*/
const Gnum                                    flagval,
const Anum * restrict const                   fineparotax,
const Anum * restrict const                   finepfixtax,
const Gnum                                    finevfixnbr,
Context * restrict const                      contptr) /*+ Execution context                                     +*/
{
  GraphCoarsenData    coardat;                    /* Graph coarsening global data */
  int                 o;

#ifdef SCOTCH_DEBUG_GRAPH1
  if (coarval < 0.5L)                             /* If impossible coarsening ratio wanted */
    return (1);                                   /* We will never succeed                 */
#endif /* SCOTCH_DEBUG_GRAPH1 */

  coardat.coarvertmax = (Gnum) ((double) (finegrafptr->vertnbr - finevfixnbr) * coarval) + finevfixnbr; /* Maximum number of coarse vertices */
  if (coardat.coarvertmax < coarvertnbr)          /* If there will be too few vertices in graph */
    return (1);                                   /* It is useless to go any further            */

  if (finecoarptr != NULL) {                      /* If fine-to-coarse destination provided    */
    coardat.flagval     = flagval | GRAPHCOARSENDSTMATE; /* Array will be provided and/or kept */
    coardat.finematetax = (*finecoarptr == NULL) ? NULL : (*finecoarptr - finegrafptr->baseval);
  }
  else {
    coardat.flagval     = flagval;                /* No mating array nor array data provided */
    coardat.finematetax = NULL;                   /* No user-provided mating array           */
  }
  coardat.finegrafptr = finegrafptr;              /* Fill caller part of matching data structure */
  coardat.fineparotax = fineparotax;
  coardat.finepfixtax = finepfixtax;
  coardat.finevfixnbr = finevfixnbr;
  coardat.coargrafptr = coargrafptr;
  coardat.coarmulttab = *coarmultptr;
  coardat.contptr     = contptr;

  o = graphCoarsen2 (&coardat);
  if (o != 0)
    return (o);

  *coarmultptr = coardat.coarmulttab;             /* Give back location of multinode array                   */
  if (finecoarptr != NULL)                        /* If fine-to-coarse destination provided                  */
    *finecoarptr = coardat.finematetax + finegrafptr->baseval; /* Give back location of fine-to-coarse array */

  return (0);
}

/* This routine coarsens the given "finegraph" into
** "coargraph", as long as the coarsening ratio remains
** below some threshold value and the coarsened graph
** is not too small.
** It returns:
** - 0  : if the graph has been coarsened.
** - 1  : if the graph could not be coarsened.
** - 2  : on error.
*/

int
graphCoarsenMatch (
const Graph * restrict const      finegrafptr,    /*+ Graph to coarsen                          +*/
Gnum * restrict * restrict const  finemateptr,    /*+ Pointer to un-based mating array to build +*/
Gnum * restrict const             coarvertptr,    /*+ Minimum number of coarse vertices         +*/
const double                      coarval,        /*+ Maximum contraction ratio                 +*/
const Gnum                        flagval,        /*+ Flag value                                +*/
const Anum * restrict const       fineparotax,
const Anum * restrict const       finepfixtax,
const Gnum                        finevfixnbr,
Context * restrict const          contptr)        /*+ Execution context                         +*/
{
  GraphCoarsenData    coardat;                    /* Graph coarsening global data */
  int                 o;

#ifdef SCOTCH_DEBUG_GRAPH1
  if (coarval < 0.5L)                             /* If impossible coarsening ratio wanted      */
    return (1);                                   /* We will never succeed                      */
#endif /* SCOTCH_DEBUG_GRAPH1 */

  coardat.coarvertmax = (Gnum) ((double) (finegrafptr->vertnbr - finevfixnbr) * coarval) + finevfixnbr; /* Maximum number of coarse vertices */
  if (coardat.coarvertmax < *coarvertptr)         /* If there will be too few vertices in graph */
    return (1);                                   /* It is useless to go any further            */

  coardat.flagval     = GRAPHCOARSENDSTMATE | (flagval & GRAPHCOARSENNOMERGE); /* Array will be provided and/or kept */
  coardat.finematetax = (*finemateptr == NULL) ? NULL : (*finemateptr - finegrafptr->baseval);
  coardat.finegrafptr = finegrafptr;              /* Fill caller part of matching data structure */
  coardat.fineparotax = fineparotax;
  coardat.finepfixtax = finepfixtax;
  coardat.finevfixnbr = finevfixnbr;
  coardat.coargrafptr = NULL;
  coardat.coarmulttab = NULL;
  coardat.contptr     = contptr;

  o = graphCoarsen2 (&coardat);
  if (o != 0)
    return (o);

  *coarvertptr = coardat.coarvertnbr;
  *finemateptr = coardat.finematetax + finegrafptr->baseval; /* Give back location of fine-to-coarse array */

  return (0);
}

/* This routine builds a coarse graph from the fine
** graph topology and a user-provided mating array.
** It returns:
** - 0  : if the graph has been coarsened.
** - 1  : if the graph could not be coarsened.
** - 2  : on error.
*/

int
graphCoarsenBuild (
const Graph * restrict const                  finegrafptr, /*+ Graph to coarsen                               +*/
Graph * restrict const                        coargrafptr, /*+ Coarse graph to build                          +*/
Gnum * restrict const                         finematetab, /*+ Pointer to un-based user-provided mating array +*/
GraphCoarsenMulti * restrict * restrict const coarmultptr, /*+ Pointer to un-based multinode table to build   +*/
const Gnum                                    coarvertnbr, /*+ User-provided number of coarse vertices        +*/
Context * restrict const                      contptr) /*+ Execution context                                  +*/
{
  GraphCoarsenData    coardat;                    /* Graph coarsening global data */
  int                 o;

  coardat.flagval     = GRAPHCOARSENDSTMATE | GRAPHCOARSENUSEMATE; /* Mating array data provided */
  coardat.finegrafptr = finegrafptr;              /* Fill caller part of matching data structure */
  coardat.fineparotax = NULL;                     /* TODO: arrays not handled yet                */
  coardat.finepfixtax = NULL;
  coardat.finevfixnbr = 0;
  coardat.finematetax = finematetab - finegrafptr->baseval; /* User-provided mating array */
  coardat.coargrafptr = coargrafptr;
  coardat.coarvertmax = finegrafptr->vertnbr + 1; /* Value that always succeeds    */
  coardat.coarvertnbr = coarvertnbr;              /* Set number of coarse vertices */
  coardat.coarmulttab = *coarmultptr;
  coardat.contptr     = contptr;

  o = graphCoarsen2 (&coardat);
  if (o != 0)
    return (o);

  *coarmultptr = coardat.coarmulttab;             /* Give back location of multinode array */

  return (0);
}
