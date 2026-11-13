/* Copyright 2004,2007,2009,2011,2012,2015,2018-2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_match.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the source graph   **/
/**                matching functions, generated from the  **/
/**                generic pattern.                        **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 05 oct 2012     **/
/**                                 to   : 30 aug 2020     **/
/**                # Version 7.0  : from : 28 jul 2018     **/
/**                                 to   : 19 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_GRAPH_MATCH

#include "module.h"
#include "common.h"
#include "context.h"
#include "arch.h"
#include "graph.h"
#include "graph_coarsen.h"
#include "graph_match.h"

/*
**  The static variables.
*/

/* Array of matching routines */

static void              (* graphmatchfunctab[]) (GraphCoarsenData * restrict const, GraphCoarsenThread * restrict const) = {
                              GRAPHMATCHFUNCBLOCK (Seq),
#ifndef GRAPHMATCHNOTHREAD
                              GRAPHMATCHFUNCBLOCK (Thr)
#else /* GRAPHMATCHNOTHREAD */
                              NULL,               /* Raise error if functions are called */
                              NULL,
                              NULL,
                              NULL
#endif /* GRAPHMATCHNOTHREAD */
                            };

/***************************/
/*                         */
/* The sequential matching */
/* subroutines.            */
/*                         */
/***************************/

#define GRAPHMATCHSCANSEQ

#define GRAPHMATCHSCANNAME          graphMatchSeqNfNe
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME

#define GRAPHMATCHSCANEDLOTAB
#define GRAPHMATCHSCANNAME          graphMatchSeqNfEl
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME
#undef GRAPHMATCHSCANEDLOTAB

#define GRAPHMATCHSCANPFIXTAB
#define GRAPHMATCHSCANNAME          graphMatchSeqFxNe
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME

#define GRAPHMATCHSCANEDLOTAB
#define GRAPHMATCHSCANNAME          graphMatchSeqFxEl
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME
#undef GRAPHMATCHSCANEDLOTAB
#undef GRAPHMATCHSCANPFIXTAB

#undef GRAPHMATCHSCANSEQ

/*************************/
/*                       */
/* The threaded matching */
/* subroutines.          */
/*                       */
/*************************/

#ifndef GRAPHMATCHNOTHREAD

#define GRAPHMATCHSCANNAME          graphMatchThrNfNe
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME

#define GRAPHMATCHSCANEDLOTAB
#define GRAPHMATCHSCANNAME          graphMatchThrNfEl
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME
#undef GRAPHMATCHSCANEDLOTAB

#define GRAPHMATCHSCANPFIXTAB
#define GRAPHMATCHSCANNAME          graphMatchThrFxNe
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME

#define GRAPHMATCHSCANEDLOTAB
#define GRAPHMATCHSCANNAME          graphMatchThrFxEl
#include "graph_match_scan.c"
#undef GRAPHMATCHSCANNAME
#undef GRAPHMATCHSCANEDLOTAB
#undef GRAPHMATCHSCANPFIXTAB

#endif /* GRAPHMATCHNOTHREAD */

/**********************/
/*                    */
/* The matching queue */
/* handling routines. */
/*                    */
/**********************/

static
void
graphMatchQueueSort (
GraphCoarsenData * restrict const   coarptr,
GraphCoarsenThread * restrict const thrdptr,
Gnum                                finevertbas,
Gnum                                finevertnnd)
{
  Gnum * restrict     finequeuptr;
  Gnum                finequeunbr;
  Gnum                finevertnum;

  const Gnum * restrict const fineverttax = coarptr->finegrafptr->verttax;
  const Gnum * restrict const finevendtax = coarptr->finegrafptr->vendtax;

  for (finevertnum = finevertbas, finequeuptr = thrdptr->finequeutab;
       finevertnum < finevertnnd; finevertnum ++) {
    *(finequeuptr ++) = finevendtax[finevertnum] - fineverttax[finevertnum];
    *(finequeuptr ++) = finevertnum;
  }
  thrdptr->finequeunbr =
  finequeunbr          = finevertnnd - finevertbas;

  intPsort2asc1 (thrdptr->finequeutab, finequeunbr, 3);
}

/***********************************/
/*                                 */
/* The matching handling routines. */
/*                                 */
/***********************************/

/* This routine performs the sequential
** initialization of the global mating
** data structures, before the threads
** are launched.
** It returns:
** - 0  : if initialization could be performed.
** - 1  : on error.
*/

int
graphMatchInit (
GraphCoarsenData * restrict coarptr,
const int                   thrdnbr)
{
  int                 fumaval;                    /* Matching routine index in function array */
  Gnum                deteval;                    /* Flag set if deterministic behavior       */

  const Graph * restrict const  finegrafptr = coarptr->finegrafptr;

  contextValuesGetInt (coarptr->contptr, CONTEXTOPTIONNUMDETERMINISTIC, &deteval);

  fumaval = (finegrafptr->edlotax != NULL) ? 1 : 0;
  if ((coarptr->finevfixnbr > 0) || (coarptr->fineparotax != NULL))
    fumaval |= 2;

#ifndef GRAPHMATCHNOTHREAD
  if ((deteval == 0) && (thrdnbr > 1)) {          /* If non-deterministic behavior accepted and several threads available */
    if ((coarptr->finelocktax = memAlloc (finegrafptr->vertnbr * sizeof (int))) == NULL) {
      errorPrint ("graphMatchInit: out of memory");
      return (1);
    }
    coarptr->finelocktax -= finegrafptr->baseval;

    fumaval |= 4;                                 /* Run threaded routines */
  }
  else
#endif /* GRAPHMATCHNOTHREAD */
  {
    coarptr->finelocktax = NULL;                  /* A NULL finelocktax means sequential (deterministic) process wanted */
  }

  coarptr->fumaval = fumaval;

  return (0);
}

/* This routine merges the results of two mating
** threads and re-launches a mating operation
** if necessary.
*/

#if 0 /* Recursive reduction routine that merges queues; a bit of an overkill to date */
static
void
graphMatchReduce (
GraphCoarsenThread * restrict const tlocptr,      /* Pointer to local block  */
GraphCoarsenThread * restrict const tremptr)      /* Pointer to remote block */
{
  Gnum                qremnbr;

  qremnbr = tremptr->finequeunnd - tremptr->finequeubas; /* Number of enqueued fine vertices in second thread */

  memMov (tlocptr->finequeutab + tlocptr->finequeunbr, /* Merge queues */
          tremptr->finequeutab, qremnbr * sizeof (Gnum));
  tlocptr->finequeunbr += qremnbr;

  if ((tlocptr->thrdnum == 0) && (((tremptr - tlocptr) << 1) >= tlocptr->thrdnbr)) /* If last join         */
    graphmatchfunctab[coarptr->funcval & ~4] (tlocptr->coarptr, tlocptr); /* Call sequential match routine */
  else
    graphmatchfunctab[coarptr->funcval] (tlocptr->coarptr, tlocptr); /* Call threaded, intermediate match routine */
}
#endif /* 0 */

/* This routine matches the vertices of the given
** graph, according to various constraints. The
** matching can be either single-threaded or
** multi-threaded.
** It returns:
** - void  : in all cases.
*/

void
graphMatch (
ThreadDescriptor * restrict const descptr,
GraphCoarsenData * const          coarptr)        /* [norestrict] because of retuval in threaded contexts */
{
  Gnum                finevertbas;
  Gnum                finevertnnd;
  Gnum                finevertsiz;                /* Fine vertex local (or global) range */
#ifdef SCOTCH_DEBUG_GRAPH2
  Gnum                finevertnum;
#endif /* SCOTCH_DEBUG_GRAPH2 */

#ifdef SCOTCH_PTHREAD
  const int                   thrdnbr = threadNbr (descptr);
  const int                   thrdnum = threadNum (descptr);
  GraphCoarsenThread * const  thrdptr = &coarptr->thrdtab[thrdnum];
#else /* SCOTCH_PTHREAD */
  GraphCoarsenThread * const  thrdptr = &coarptr->thrdtab[0];
#endif /* SCOTCH_PTHREAD */

  if (coarptr->finelocktax == NULL) {             /* If sequential, deterministic processing wanted */
#ifdef SCOTCH_PTHREAD
    if (thrdnum != 0) {                           /* Only thread 0 will perform the work    */
      threadBarrier (descptr);                    /* End-of-routine synchronization barrier */
      return;
    }
#endif /* SCOTCH_PTHREAD */

    finevertbas = coarptr->finegrafptr->baseval;  /* Work on all graph fine vertices */
    finevertnnd = coarptr->finegrafptr->vertnnd;
  }
  else {
    finevertbas = thrdptr->finevertbas;           /* Work on slice of fine graph */
    finevertnnd = thrdptr->finevertnnd;
  }
  finevertsiz = finevertnnd - finevertbas;

  thrdptr->finequeudlt = 2;                       /* For sort queue */
  if ((thrdptr->finequeutab = memAlloc (finevertsiz * thrdptr->finequeudlt * sizeof (Gnum))) == NULL) { /* Allocate (local or global) processing queue */
    errorPrint ("graphMatch: out of memory");
    coarptr->retuval = 2;
    if (coarptr->finelocktax == NULL) {           /* If only thread 0 is working */
#ifdef SCOTCH_PTHREAD
      threadBarrier (descptr);                    /* End-of-routine synchronization barrier */
#endif /* SCOTCH_PTHREAD */
      return;                                     /* Thread 0 returns */
    }
  }

  memSet (coarptr->finematetax + finevertbas, ~0, finevertsiz * sizeof (Gnum)); /* Initialize (local part of) mate array */
  if (coarptr->finelocktax != NULL) {
    memSet (coarptr->finelocktax + finevertbas, 0, finevertsiz * sizeof (int)); /* Initialize local part of lock array for concurrent acces */

    threadBarrier (descptr);                      /* Synchronization for mating arrays and retuval */

    if (coarptr->retuval != 0) {
      if (thrdptr->finequeutab != NULL)           /* If someone else's allocation failed */
        memFree (thrdptr->finequeutab);           /* Free our own queue                  */
      return;
    }
  }

  graphMatchQueueSort (coarptr, thrdptr, finevertbas, finevertnnd);

  thrdptr->coarvertnbr = 0;                       /* No coarse vertices created yet */

#ifdef SCOTCH_PTHREAD
  if (coarptr->finelocktax != NULL) {
    graphmatchfunctab[coarptr->fumaval] (coarptr, thrdptr); /* Call parallel matching routine */

    threadBarrier (descptr);                      /* Barrier before pseudo-reduction */

    if (thrdnum == 0) {
      Gnum                coarvertnbr;
      int                 thrdtmp;

      for (thrdtmp = 0, coarvertnbr = 0;          /* Sequential pseudo-reduction on remaining vertices */
           thrdtmp < thrdnbr; thrdtmp ++) {
        graphmatchfunctab[coarptr->fumaval & ~4] (coarptr, &coarptr->thrdtab[thrdtmp]); /* Call sequential matching routine */
        coarvertnbr += coarptr->thrdtab[thrdtmp].coarvertnbr;
      }
      coarptr->coarvertnbr = coarvertnbr;         /* Global number of coarse vertices is reduced number */

      memFree (coarptr->finelocktax + coarptr->finegrafptr->baseval); /* Free now useless lock array */
    }

    threadBarrier (descptr);                      /* coarptr->coarvertnbr must be known to all */
  }
  else
#endif /* SCOTCH_PTHREAD */
  {
    graphmatchfunctab[coarptr->fumaval & ~4] (coarptr, thrdptr); /* Call sequential matching routine                            */
    coarptr->coarvertnbr = thrdptr->coarvertnbr;  /* Global number of coarse vertices is that computed by (sequential) thread 0 */
  }

  memFree (thrdptr->finequeutab);

#ifdef SCOTCH_DEBUG_GRAPH2
  for (finevertnum = finevertbas; finevertnum < finevertnnd; finevertnum ++) {
    if (coarptr->finematetax[finevertnum] == ~0) { /* If matching not aborted, this should not happen */
      errorPrint ("graphMatch: internal error");
      coarptr->coarvertnbr = coarptr->coarvertmax;
    }
  }
#endif /* SCOTCH_DEBUG_GRAPH2 */

#ifdef SCOTCH_PTHREAD
  if (coarptr->finelocktax == NULL)               /* If only thread 0 is working                         */
    threadBarrier (descptr);                      /* End-of-routine synchronization barrier for thread 0 */
#endif /* SCOTCH_PTHREAD */
}
