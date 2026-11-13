/* Copyright 2012,2014,2015,2018-2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_match_scan.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module contains the stub of the    **/
/**                threaded and un-threaded centralized    **/
/**                graph matching functions.               **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 01 oct 2012     **/
/**                                 to   : 30 aug 2020     **/
/**                # Version 7.0  : from : 28 jul 2018     **/
/**                                 to   : 14 jan 2020     **/
/**                                                        **/
/**   NOTES      : # This code partly derives from the     **/
/**                  code of graph_match.c partly updated  **/
/**                  in the early stages of release 6.0    **/
/**                  so as to account for fixed vertices   **/
/**                  and an old partition.                 **/
/**                                                        **/
/************************************************************/

/***************************/
/*                         */
/* The matching subroutine */
/* pattern.                */
/*                         */
/***************************/

/* This routine matches the vertices of the given
** centralized graph according to various constraints.
** It returns:
** - void  : in all cases
*/

static
void
GRAPHMATCHSCANNAME (
GraphCoarsenData * restrict const   coarptr,
GraphCoarsenThread * restrict const thrdptr)
{
  Gnum                finequeunew;                /* Write index in queue; always <= queunum */
  Gnum                finequeunum;                /* read index in queue                     */
  Gnum                finequeunnd;                /* Index of end of queue                   */

  const Graph * restrict const    finegrafptr = coarptr->finegrafptr;
  Gnum * restrict const           finequeutab = thrdptr->finequeutab;
  const Gnum                      finequeudlt = thrdptr->finequeudlt;
  Gnum                            coarvertnbr = thrdptr->coarvertnbr; /* Current number of multinode vertices */
  const int                       flagval = coarptr->flagval;
#ifndef GRAPHMATCHSCANSEQ
  volatile int * const            locktax = coarptr->finelocktax;
#endif /* GRAPHMATCHSCANSEQ */
  const Gnum * restrict const     fineverttax = finegrafptr->verttax;
  const Gnum * restrict const     finevendtax = finegrafptr->vendtax;
  const Gnum * restrict const     fineedgetax = finegrafptr->edgetax;
#ifdef GRAPHMATCHSCANEDLOTAB
  const Gnum * restrict const     fineedlotax = finegrafptr->edlotax;
#endif /* GRAPHMATCHSCANEDLOTAB */
  volatile Gnum * restrict const  finematetax = coarptr->finematetax;
#ifdef GRAPHMATCHSCANPFIXTAB
  const Gnum * restrict const     fineparotax = coarptr->fineparotax;
  const Gnum * restrict const     finepfixtax = coarptr->finepfixtax;
#endif /* GRAPHMATCHSCANPFIXTAB */

  finequeunew = 0;
  finequeunum = finequeudlt >> 1;                 /* First pass: dlt = 2, start index = 1; next passes: dlt = 1, start index = 0 */
  finequeunnd = finequeunum + thrdptr->finequeunbr * finequeudlt;
  for ( ; finequeunum < finequeunnd; finequeunum += finequeudlt) { /* For all queued vertex indices */
    Gnum                finevertnum;
    Gnum                finevertbst;
    Gnum                fineedgenum;
    Gnum                fineedgennd;
#ifdef GRAPHMATCHSCANEDLOTAB
    Gnum                fineedlobst = -1;         /* Edge load of current best neighbor */
#endif /* GRAPHMATCHSCANEDLOTAB */

    finevertnum = finequeutab[finequeunum];
    if (finematetax[finevertnum] >= 0)            /* If vertex already mated, skip it without remembering it */
      continue;

    finevertbst = finevertnum;                    /* Assume we match with ourselves */
    fineedgenum = fineverttax[finevertnum];
    fineedgennd = finevendtax[finevertnum];

    if (fineedgenum == fineedgennd) {             /* If isolated vertex                               */
      if ((flagval & GRAPHCOARSENNOMERGE) == 0) { /* If can be merged                                 */
        Gnum                finequisnnd;          /* Index for mating isolated vertex at end of queue */

	while ((finequeunnd > finequeunum) &&     /* Discard already matched vertices at end of queue */
               (finematetax[finequeutab[finequeunnd - finequeudlt]] >= 0))
          finequeunnd -= finequeudlt;

        for (finequisnnd = finequeunnd; finequisnnd > finequeunum; ) { /* As long we have candidates */
          Gnum                fineverttmp;

          fineverttmp  = finequeutab[finequisnnd - finequeudlt]; /* Get mate from end of queue */
          finequisnnd -= finequeudlt;             /* Assume vertex is consumed                 */
          if (finematetax[fineverttmp] < 0) {     /* If vertex can be matched                  */
#ifdef GRAPHMATCHSCANPFIXTAB
            if (((finepfixtax == NULL) || (finepfixtax[fineverttmp] == finepfixtax[finevertnum])) && /* We can only mate if potential mate has same value */
                ((fineparotax == NULL) || (fineparotax[fineverttmp] == fineparotax[finevertnum]))) /* And is in the same old part                         */
#endif /* GRAPHMATCHSCANPFIXTAB */
            {
              finevertbst = fineverttmp;
#ifndef GRAPHMATCHSCANPFIXTAB
#ifdef GRAPHMATCHSCANSEQ
              finequeunnd = finequisnnd;          /* In sequential mode and without fixed vertices, vertex is always consumed */
#endif /* GRAPHMATCHSCANSEQ */
#endif /* GRAPHMATCHSCANPFIXTAB */
              break;
            }
          }
        }
      }
    }
    else {                                        /* Vertex has at least one neighbor     */
      do {                                        /* Perform search for mate on neighbors */
        Gnum                finevertend;

        finevertend = fineedgetax[fineedgenum];

        if ((finematetax[finevertend] < 0)        /* If unmatched vertex */
#ifdef GRAPHMATCHSCANPFIXTAB
            && ((finepfixtax == NULL) || (finepfixtax[finevertend] == finepfixtax[finevertnum])) /* We can only mate if potential mate has same value */
            && ((fineparotax == NULL) || (fineparotax[finevertend] == fineparotax[finevertnum])) /* And is in the same old part                       */
#endif /* GRAPHMATCHSCANPFIXTAB */
#ifdef GRAPHMATCHSCANEDLOTAB
            && (fineedlotax[fineedgenum] > fineedlobst) /* And is better candidate */
#endif /* GRAPHMATCHSCANEDLOTAB */
        ) {
          finevertbst = finevertend;
#ifdef GRAPHMATCHSCANEDLOTAB
          fineedlobst = fineedlotax[fineedgenum];
#else /* GRAPHMATCHSCANEDLOTAB */
          break;                                  /* Matching vertex found */
#endif /* GRAPHMATCHSCANEDLOTAB */
        }
      } while (++ fineedgenum < fineedgennd);
    }

#ifndef GRAPHMATCHSCANSEQ
    if (__sync_lock_test_and_set (&locktax[finevertnum], 1)) /* If could not acquire local vertex (always succeeds for isolated) */
      continue;                                   /* Do not remember it as some other vertex has already acquired both           */

    if (finevertbst != finevertnum) {             /* If we mated with another vertex                 */
      if (__sync_lock_test_and_set (&locktax[finevertbst], 1)) { /* If could not acquire mate vertex */
        __sync_lock_release (&locktax[finevertnum]); /* Release lock on local vertex                 */
        finequeutab[finequeunew ++] = finevertnum; /* Postpone processing to next pass               */
        continue;
      }
      finematetax[finevertbst] = finevertnum;     /* Match other vertex with us */
    }
#else /* GRAPHMATCHSCANSEQ */
    finematetax[finevertbst] = finevertnum;       /* Match other vertex with us */
#endif /* GRAPHMATCHSCANSEQ */
    finematetax[finevertnum] = finevertbst;       /* Match ourselves with other vertex */
    coarvertnbr ++;                               /* One more coarse vertex created    */
  }

  thrdptr->finequeunbr = finequeunew;             /* Record queue index for next pass            */
  thrdptr->finequeudlt = 1;                       /* Next passes always on index queues only     */
  thrdptr->coarvertnbr = coarvertnbr;             /* Record updated number of multinode vertices */
}
