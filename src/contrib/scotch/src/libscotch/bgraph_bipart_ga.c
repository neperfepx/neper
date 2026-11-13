/* Copyright 2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_ga.c                      **/
/**                                                        **/
/**   AUTHOR     : Connor MAYON                            **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes a bipartition of   **/
/**                a bipartition graph by using a          **/
/**                genetic algorithm.                      **/
/**                                                        **/
/**   NOTES      : # This algorithm has been designed to   **/
/**                  work on band graphs only, for which   **/
/**                  the two anchor vertices are the two   **/
/**                  last vertices, the before-last as     **/
/**                  anchor of part 0, and the last as     **/
/**                  anchor of part 1.                     **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 15 feb 2023     **/
/**                                 to   : 08 oct 2026     **/
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
#include "bgraph_bipart_ga.h"
//#define BGRAPHBIPARTGANOTHREAD

#define DEMENBR(p,t)                ((((p) + (2 * (t)) - 1) / (2 * (t))) * 2) /* Even number and multiple of number of threads */

/************************/
/*                      */
/* The sorting routine. */
/*                      */
/************************/

/* This routine sorts an array of bgraphBipartGaSort
** values in ascending order by their cost.
** By nature of the sorting algorithm, data are left in
** place in case of equality.
** It returns:
** - void  : in all cases.
*/

#define INTSORTQUAL                 static
#define INTSORTNAME                 bgraphBipartGaSort
#define INTSORTSIZE                 (sizeof (BgraphBipartGaSort))
#define INTSORTSWAP(p,q)            do {                                                             \
                                      BgraphBipartGaSort t;                                          \
                                      t = *((BgraphBipartGaSort *) (p));                             \
                                      *((BgraphBipartGaSort *) (p)) = *((BgraphBipartGaSort *) (q)); \
                                      *((BgraphBipartGaSort *) (q)) = t;                             \
                                    } while (0)
#define INTSORTCMP(p,q)             ((((BgraphBipartGaSort *) (p))->cmloval < ((BgraphBipartGaSort *) (q))->cmloval) ||   \
                                     ((((BgraphBipartGaSort *) (p))->cmloval == ((BgraphBipartGaSort *) (q))->cmloval) && \
                                      (((BgraphBipartGaSort *) (p))->cplodlt < ((BgraphBipartGaSort *) (q))->cplodlt)))
#include "common_sort.c"
#undef INTSORTQUAL
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/******************************/
/*                            */
/* The threaded loop routine. */
/*                            */
/******************************/

/* This routine sorts and mates partions
** on the given part of the bipartition
** graph.
** It returns:
** - 0   : if algorithm went up to last pass.
** - !0  : on error.
*/

static
void
bgraphBipartGaLoop (
ThreadDescriptor * restrict const   descptr,
BgraphBipartGaData * restrict const loopptr)
{
  GraphPart *         demetab;                    /* Parent and offspring local population arrays        */
  GraphPart *         demeptr[2];                 /* Pointers to current parent and offspring sub-arrays */
  INT                 passnum;
  Gnum                partnum;

#ifndef BGRAPHBIPARTGANOTHREAD
  const int                           thrdnbr = threadNbr (descptr);
  const int                           thrdnum = threadNum (descptr);
#else /* BGRAPHBIPARTGANOTHREAD */
  const int                           thrdnbr = 1;
  const int                           thrdnum = 0;
#endif /* BGRAPHBIPARTGANOTHREAD */
  Bgraph * restrict const             grafptr = loopptr->grafptr;
  BgraphBipartGaSort * const          sorttab = loopptr->sorttab; /* [norestrict: async] */
  Gnum * const                        matetab = loopptr->matetab; /* [norestrict: async] */
  const Gnum                          baseval = grafptr->s.baseval;
  const Gnum                          vertnbr = grafptr->s.vertnbr;
  const Gnum                          popunbr = loopptr->popunbr;
  const Gnum                          compload0min = grafptr->compload0min;
  const Gnum                          compload0max = grafptr->compload0max;

  const Gnum                          demenbr = DEMENBR (popunbr, thrdnbr); /* Number of individuals in local deme */
  const Gnum                          demesiz = demenbr * vertnbr; /* Size of each deme array (current and new)    */
  const Gnum                          sortnbr = demenbr * thrdnbr; /* Size of global sort array for thread 0       */
  const Gnum                          sortbas = demenbr * thrdnum; /* Start index in sort array for the thread     */
  const Gnum                          sortnnd = sortbas + demenbr; /* End index in sort array for the thread       */
  const Gnum                          cpl0avg = grafptr->compload0avg; /* Average vertex load in part 0            */
  const Gnum                          cmloadj = grafptr->s.edlosum * grafptr->domndist; /* Penalty for imbalance   */

  if ((demetab = (GraphPart *) memAlloc ((size_t) (demesiz * 2 * sizeof (GraphPart)))) == NULL) {
    errorPrint ("bgraphBipartGa: out of memory (1)");
    loopptr->abrtval = 1;
  }

#ifndef BGRAPHBIPARTGANOTHREAD
  threadBarrier (descptr);                        /* Check that local memory allocation went well */
#endif /* BGRAPHBIPARTGANOTHREAD */
  if (loopptr->abrtval == 1) {                    /* If any process decided to quit */
    if (demetab != NULL)                          /* Free local array if necessary  */
      memFree (demetab);
    return;
  }

  demeptr[0] = demetab;                           /* Point to initial and future demes */
  demeptr[1] = demetab + demesiz;

  partnum = demesiz;
  if (thrdnum == 0) {                             /* Insert initial solution in last slot of deme of thread 0 */
    memCpy (demetab + demesiz - vertnbr, grafptr->parttax + baseval, vertnbr * sizeof (GraphPart));
    partnum -= vertnbr;
  }
  while (partnum > 0) {                           /* Fill (remaining) deme area with random values */
    UINT                randval;                  /* Integer random value to slice into bits       */
    Gnum                randnbr;                  /* Number of slices to use in a random value     */

    randval = contextIntRandVal2 (grafptr->contptr); /* Get UINT random value */
    randnbr = MIN (sizeof (UINT), partnum);       /* Compute useable slices   */
    while (randnbr -- > 0) {
      demetab[-- partnum] = (GraphPart) (randval & 1); /* Use lowest bit value as part value */
      randval >>= 1;                              /* Shift to next bit                       */
    }
  }

  for (passnum = loopptr->passnbr; ; passnum --) {
    GraphPart *     demetmp;
    GraphPart *     parttab;
    Gnum            sortnum;

    for (sortnum = sortbas, parttab = demeptr[0]; /* Compute cost of individuals in current deme */
         sortnum < sortnnd; sortnum ++, parttab += vertnbr) {
      Gnum          ver1nbr;                      /* Number of vertices in part 1      */
      Gnum          cpl1val;                      /* Load of part 1                    */
      Gnum          cpl0val;                      /* Load of part 0                    */
      Gnum          cmlival;                      /* Internal communication load       */
      Gnum          cmleval;                      /* External communication load       */
      Gnum          cxgnval;                      /* Communication gain if all swapped */
      Gnum          cmloval;                      /* Communication load                */

      bgraphCost2 (grafptr, parttab - baseval, NULL, NULL, &cpl1val, &ver1nbr, &cmlival, &cmleval, &cxgnval); /* Compute cost of individual */
      cpl0val = grafptr->s.velosum - cpl1val;
      cmloval = cmlival * grafptr->domndist + cmleval;
      sorttab[sortnum].cmloval = ((cpl0val >= compload0min) && (cpl0val <= compload0max)) ? cmloval : (cmloval + cmloadj);
      sorttab[sortnum].cplodlt = ABS (cpl0val - cpl0avg);
      sorttab[sortnum].parttab = parttab;
    }

#ifndef BGRAPHBIPARTGANOTHREAD
    threadBarrier (descptr);
#endif /* BGRAPHBIPARTGANOTHREAD */

    if (thrdnum == 0)                             /* Main thread sorts the individuals by ascending cost */
      bgraphBipartGaSort (loopptr->sorttab, sortnbr);

    if (passnum < 0)                              /* Exit loop for last generation */
      break;

    if (thrdnum == 0) {
      Gnum                sortold;
      Gnum                sortnew;

      for (sortold = 0, sortnew = (3 * sortnbr) / 4; /* Replace 1/4 of the least fit individuals by 1/4 of best fit */
           sortnew < sortnbr; )
        sorttab[sortnew ++] = sorttab[sortold ++];

      intAscn (loopptr->matetab, sortnbr, 0);     /* Generate permutation for mating array */
      intPerm (loopptr->matetab, sortnbr, grafptr->contptr);
      intSort2asc1 (loopptr->matetab, sortnbr / 2);
    }

#ifndef BGRAPHBIPARTGANOTHREAD
    threadBarrier (descptr);
#endif /* BGRAPHBIPARTGANOTHREAD */

    for (sortnum = sortbas, parttab = demeptr[1]; sortnum < sortnnd; sortnum += 2) { /* Create offsprings */
      Gnum                vecridx;                /* Crossover point for individuals to be mated          */
      Gnum                vemuidx;                /* Mutation point for individuals                       */

      vecridx = 1 + contextIntRandVal (grafptr->contptr, vertnbr - 2); /* Crossing-over preserves at least one vertex in each segment */

      memCpy (parttab, sorttab[matetab[sortnum]].parttab, vecridx);
      memCpy (parttab + vecridx, sorttab[matetab[sortnum + 1]].parttab + vecridx, vertnbr - vecridx);
      parttab += vertnbr;                         /* Move to next offspring */
      memCpy (parttab, sorttab[matetab[sortnum + 1]].parttab, vecridx);
      memCpy (parttab + vecridx, sorttab[matetab[sortnum]].parttab + vecridx, vertnbr - vecridx);
      vemuidx = contextIntRandVal (grafptr->contptr, vertnbr); /* Perform mutation on every other offspring */
      parttab[vemuidx] ^= 1;
      parttab += vertnbr;                         /* Move to next offspring */
    }

    demetmp    = demeptr[0];                      /* Swap pointers for current and new demes */
    demeptr[0] = demeptr[1];
    demeptr[1] = demetmp;
  }

  if (thrdnum == 0) {
    if ((loopptr->sorttab[0].cmloval < grafptr->commload) || /* If most fit offspring has lower cost than current solution */
        ((loopptr->sorttab[0].cmloval == grafptr->commload) &&
         (loopptr->sorttab[0].cplodlt < ABS (grafptr->compload0dlt)))) {
      memCpy     (grafptr->parttax + baseval, sorttab[0].parttab, vertnbr * sizeof (GraphPart));
      bgraphCost (grafptr);
    }
  }

#ifndef BGRAPHBIPARTGANOTHREAD
  threadBarrier (descptr);                        /* Make sure champion is copied before local memory is freed */
#endif /* BGRAPHBIPARTGANOTHREAD */

  memFree (demetab);
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

int
bgraphBipartGa (
Bgraph * restrict const           grafptr,        /* Active graph      */
const BgraphBipartGaParam * const paraptr)        /* Method parameters */
{
  BgraphBipartGaData  loopdat;

#ifndef BGRAPHBIPARTGANOTHREAD
  const int                 thrdnbr = contextThreadNbr (grafptr->contptr);
#else /* BGRAPHBIPARTGANOTHREAD */
  const int                 thrdnbr = 1;
#endif /* BGRAPHBIPARTGANOTHREAD */
  const Gnum                demenbr = DEMENBR (paraptr->popunbr, thrdnbr); /* Size of thread local deme */
  const Gnum                sortnbr = demenbr * thrdnbr; /* Overall number of individuals to sort       */

  if (grafptr->s.vertnbr < 2)                     /* If less than two vertices, no crossing-over possible and nothing relevant to do */
    return (0);

  if (memAllocGroup ((void **) (void *)
                     &loopdat.thrdtab, (size_t) (thrdnbr * sizeof (BgraphBipartGaThread)),
                     &loopdat.sorttab, (size_t) (sortnbr * sizeof (BgraphBipartGaSort)),
                     &loopdat.matetab, (size_t) (sortnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("bgraphBipartGa: out of memory (1)");
    return (1);
  }

  loopdat.grafptr = grafptr;
  loopdat.passnbr = paraptr->passnbr;
  loopdat.popunbr = paraptr->popunbr;
  loopdat.abrtval = 0;                            /* No reason to abort (yet) */

#ifndef BGRAPHBIPARTGANOTHREAD
  contextThreadLaunch (grafptr->contptr, (ThreadFunc) bgraphBipartGaLoop, (void *) &loopdat);
#else /* BGRAPHBIPARTGANOTHREAD */
  bgraphBipartGaLoop (NULL, &loopdat);
#endif /* BGRAPHBIPARTGANOTHREAD */

  memFree (loopdat.thrdtab);                      /* Free group leader */

#ifdef SCOTCH_DEBUG_BGRAPH2
  if (bgraphCheck (grafptr) != 0) {
    errorPrint ("bgraphBipartGa: inconsistent graph data");
    return (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH2 */

  return (0);                                     /* Genetic Algorithm partition successful */
}
