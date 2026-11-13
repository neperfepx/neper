/* Copyright 2008,2011,2014,2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_rb_part.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                Clement BARTHELEMY                      **/
/**                                                        **/
/**   FUNCTION   : This module performs the Dual Recursive **/
/**                Bipartitioning mapping algorithm for    **/
/**                (eventually weighted) complete graph    **/
/**                target architectures.                   **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 16 sep 2008     **/
/**                                 to   : 31 aug 2011     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 16 sep 2014     **/
/**                # Version 6.1  : from : 28 jun 2021     **/
/**                                 to   : 28 jun 2021     **/
/**                # Version 7.0  : from : 03 may 2021     **/
/**                                 to   : 16 jul 2023     **/
/**                                                        **/
/**   NOTES      : # This is a rewrite of kgraphMapRb()    **/
/**                  for complete-graph target topologies. **/
/**                  Its advantage over kgraphMapRbMap()   **/
/**                  is that no job arrays are allocated,  **/
/**                  which can save space for instance for **/
/**                  using the variable-sized complete     **/
/**                  graph architecture.                   **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_KGRAPH_MAP_RB_PART

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "bgraph.h"
#include "bgraph_bipart_st.h"
#include "kgraph.h"
#include "kgraph_map_rb.h"
#include "kgraph_map_rb_part.h"

/********************************************/
/*                                          */
/* This is the entry point for the Dual     */
/* Recursive Bipartitioning mapping method. */
/*                                          */
/********************************************/

/* This routine updates partial mappings
** according to the result of the recursive
** bipartitioning process.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
int
kgraphMapRbPart3 (
const Graph * restrict const      srcgrafptr,     /* Graph to induce and bipartition */
const GraphPart * restrict const  srcparttax,     /* Part array of original graph    */
const GraphPart                   indpartval,     /* Part of graph to consider       */
const ArchDom * restrict const    domnptr,        /* Domain to map                   */
Mapping * restrict const          mappptr)        /* Final mapping                   */
{
  Anum               domnnum;
  Gnum               vertnum;

  const Gnum * restrict const srcvnumtax = srcgrafptr->vnumtax;
  Anum * const                mapparttax = mappptr->parttax; /* [norestrict] as array can be writter by different threads */

#ifdef SCOTCH_PTHREAD
  pthread_mutex_lock (&mappptr->mutedat);
#endif /* SCOTCH_PTHREAD */
  domnnum = mappptr->domnnbr ++;                  /* One more subdomain to account for */
  if (domnnum >= mappptr->domnmax) {
    int                 o;

    if ((o = mapResize (mappptr, domnnum + (domnnum >> 2) + 8)) != 0) { /* Increase size by 25% */
      errorPrint ("kgraphMapRbPart3: cannot resize structures");
#ifdef SCOTCH_PTHREAD
      pthread_mutex_unlock (&mappptr->mutedat);
#endif /* SCOTCH_PTHREAD */
      return (o);
    }
  }

  mappptr->domntab[domnnum] = *domnptr;           /* Write domain in (possibly reallocated) array */
#ifdef SCOTCH_PTHREAD
  pthread_mutex_unlock (&mappptr->mutedat);
#endif /* SCOTCH_PTHREAD */

  if (srcparttax == NULL) {                       /* If graph is full graph */
#ifdef SCOTCH_DEBUG_KGRAPH2
    if (domnnum != 0) {
      errorPrint ("kgraphMapRbPart3: internal error (1)");
      return (1);
    }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
    if (srcvnumtax == NULL)                       /* If full graph doesn't have fixed vertices */
      memSet (mapparttax + srcgrafptr->baseval, 0, srcgrafptr->vertnbr * sizeof (Anum));
    else {
      Gnum                vertnnd;

      for (vertnum = srcgrafptr->baseval, vertnnd = srcgrafptr->vertnnd;
           vertnum < vertnnd; vertnum ++) {
#ifdef SCOTCH_DEBUG_KGRAPH2
        if (mapparttax[srcvnumtax[vertnum]] == ~0) {
          errorPrint ("kgraphMapRbPart3: internal error (2)");
          return (1);
        }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
        mapparttax[srcvnumtax[vertnum]] = domnnum;
      }
    }
  }
  else {                                          /* Graph to consider is a subgraph of the original graph */
    if (srcvnumtax == NULL) {                     /* If original graph is not itself a subgraph            */
      Gnum                vertnnd;

      for (vertnum = srcgrafptr->baseval, vertnnd = srcgrafptr->vertnnd;
           vertnum < vertnnd; vertnum ++) {
        if (srcparttax[vertnum] == indpartval) {  /* If vertex belongs to the right part */
#ifdef SCOTCH_DEBUG_KGRAPH2
          if (mapparttax[vertnum] == ~0) {
            errorPrint ("kgraphMapRbPart3: internal error (3)");
            return (1);
          }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
          mapparttax[vertnum] = domnnum;
        }
      }
    }
    else {
      Gnum                vertnnd;

      for (vertnum = srcgrafptr->baseval, vertnnd = srcgrafptr->vertnnd;
           vertnum < vertnnd; vertnum ++) {
        if (srcparttax[vertnum] == indpartval) {  /* If vertex belongs to the right part */
#ifdef SCOTCH_DEBUG_KGRAPH2
          if (mapparttax[srcvnumtax[vertnum]] == ~0) {
            errorPrint ("kgraphMapRbPart3: internal error (4)");
            return (1);
          }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
          mapparttax[srcvnumtax[vertnum]] = domnnum;
        }
      }
    }
  }

  return (0);
}

/* This routine is the core of the degenerated,
** graph partitioning version of the Dual Recursive
** Bipartitioning algorithm.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
void
kgraphMapRbPart2 (
Context * restrict const      contptr,            /*+ (Sub-)context                          +*/
const int                     spltnum,            /*+ Rank of sub-context in initial context +*/
KgraphMapRbPartSplit * const  spltptr)
{
  Graph                 indgrafdat;
  const Graph *         indgrafptr;
  Bgraph                actgrafdat;
  ArchDom               domnsubtab[2];            /* Target subdomains                           */
  Anum                  vflonbrtab[2];            /* Number of fixed vertex slots in subdomains  */
  Gnum                  vflowgttab[2];            /* Weights of fixed vertex slots in subdomains */
  KgraphMapRbPartSplit  spltdat;                  /* Parameters for context splitting            */
  int                   avarval;                  /* Flag set if variable-sized                  */
  GraphPart             partval;
  int                   o;

  Mapping * restrict const          mappptr    = spltptr->dataptr->mappptr;
  const Graph * restrict const      srcgrafptr = spltptr->grafptr;
  const GraphPart * restrict const  srcparttax = spltptr->parttax;
  const GraphPart                   indpartval = (GraphPart) spltnum;
  const Gnum                        indvertnbr = spltptr->splttab[spltnum].vertnbr;

  avarval = archVar (mappptr->archptr);
  o = ((avarval    != 0) &&                       /* If architecture is variable-sized   */
       (indvertnbr <= 1))                         /* And source subgraph of minimal size */
      ? 1                                         /* Then do not bipartition target more */
      : archDomBipart (mappptr->archptr, spltptr->splttab[spltnum].domnptr, &domnsubtab[0], &domnsubtab[1]);

  switch (o) {
    case 1 :                                      /* If target domain is terminal */
      o = kgraphMapRbPart3 (srcgrafptr, srcparttax, indpartval, spltptr->splttab[spltnum].domnptr, mappptr); /* Update mapping and return */
      goto end3;                                  /* Propagate errors only */
    case 2 :                                      /* On error              */
      errorPrint ("kgraphMapRbPart2: cannot bipartition domain");
      o = 1;
      goto end3;
  }

  indgrafptr = srcgrafptr;                        /* Assume we will work on the original graph */
  if ((srcparttax != NULL) &&                     /* If not the case, build induced subgraph   */
      (indvertnbr < srcgrafptr->vertnbr)) {
    indgrafptr = &indgrafdat;
    if ((o = graphInducePart (srcgrafptr, srcparttax, indvertnbr, indpartval, &indgrafdat)) != 0) {
      errorPrint ("kgraphMapRbPart2: cannot induce graph");
      goto end3;
    }
  }

  kgraphMapRbVfloSplit (mappptr->archptr, domnsubtab, spltptr->splttab[spltnum].vflonbr, spltptr->splttab[spltnum].vflotab, vflonbrtab, vflowgttab);

  if ((o = kgraphMapRbBgraph (spltptr->dataptr, &actgrafdat, indgrafptr, mappptr, domnsubtab, vflowgttab, contptr)) != 0) { /* Create active graph */
    errorPrint ("kgraphMapRbPart2: cannot create bipartition graph");
    goto end2;
  }
  actgrafdat.levlnum = spltptr->levlnum;

  if (avarval == 0) {                             /* If not variable-sized, impose constraints on bipartition */
    double              comploadavg;

    comploadavg = (double) (actgrafdat.s.velosum + vflowgttab[0] + vflowgttab[1]) /
                  (double) archDomWght (mappptr->archptr, spltptr->splttab[spltnum].domnptr);
    actgrafdat.compload0min = actgrafdat.compload0avg -
                              (Gnum) MIN ((spltptr->dataptr->comploadmax - comploadavg) * (double) actgrafdat.domnwght[0],
                                          (comploadavg - spltptr->dataptr->comploadmin) * (double) actgrafdat.domnwght[1]);
    actgrafdat.compload0max = actgrafdat.compload0avg +
                              (Gnum) MIN ((comploadavg - spltptr->dataptr->comploadmin) * (double) actgrafdat.domnwght[0],
                                          (spltptr->dataptr->comploadmax - comploadavg) * (double) actgrafdat.domnwght[1]);
  }

  if ((o = bgraphBipartSt (&actgrafdat, spltptr->dataptr->paraptr->strat)) != 0) { /* Perform bipartitioning */
    errorPrint ("kgraphMapRbPart2: cannot bipartition graph");
    goto end1;
  }
  memFree (actgrafdat.frontab);                   /* Frontier array of bipartitioning graph is no longer necessary */
  actgrafdat.s.flagval &= ~BGRAPHFREEFRON;

  spltdat.splttab[0].vertnbr = actgrafdat.compsize0; /* Prepare the two subjobs */
  spltdat.splttab[0].vflonbr = vflonbrtab[0];
  spltdat.splttab[0].vflotab = spltptr->splttab[spltnum].vflotab;
  spltdat.splttab[0].domnptr = &domnsubtab[0];
  spltdat.splttab[1].vertnbr = actgrafdat.s.vertnbr - actgrafdat.compsize0;
  spltdat.splttab[1].vflonbr = vflonbrtab[1];
  spltdat.splttab[1].vflotab = spltptr->splttab[spltnum].vflotab + vflonbrtab[0];
  spltdat.splttab[1].domnptr = &domnsubtab[1];

  if ((partval = 1, (actgrafdat.compsize0 == 0)) || /* If bipartition failed */
      (partval = 0, (actgrafdat.compsize0 == actgrafdat.s.vertnbr))) {
    if (avarval == 0) {                           /* If architecture is not variable-sized     */
      bgraphExit (&actgrafdat);                   /* Free bipartition graph (that is, parttax) */
      if (indgrafptr == &indgrafdat)              /* If an induced subgraph had been created   */
        graphExit (&indgrafdat);                  /* Free it                                   */

      spltptr->splttab[spltnum].vflonbr = spltdat.splttab[partval].vflonbr; /* Restrict to non-empty subdomain (vertnbr unchanged) */
      spltptr->splttab[spltnum].vflotab = spltdat.splttab[partval].vflotab;
      spltptr->splttab[spltnum].domnptr = spltdat.splttab[partval].domnptr;
      kgraphMapRbPart2 (contptr, spltnum, spltptr); /* Run bipartitioning on same subgraph     */
      return;                                     /* Error management performed at lower level */
    }
    else {                                        /* If architecture is variable-sized */
      o = kgraphMapRbPart3 (srcgrafptr, srcparttax, indpartval, spltptr->splttab[spltnum].domnptr, mappptr); /* Update mapping with original domain */
      goto end1;
    }
  }

  spltdat.dataptr = spltptr->dataptr;             /* Complete data of the two subjobs for possible concurrent execution */
  spltdat.grafptr = indgrafptr;
  spltdat.parttax = actgrafdat.parttax;
  spltdat.levlnum = spltptr->levlnum + 1;
  spltdat.revaptr = &o;

#ifndef KGRAPHMAPRBPARTNOTHREAD
  if (contextThreadLaunchSplit (contptr, (ContextSplitFunc) kgraphMapRbPart2, &spltdat) != 0) /* If counld not split context to run concurrently */
#endif /* KGRAPHMAPRBPARTNOTHREAD */
  {
    kgraphMapRbPart2 (contptr, 0, &spltdat);      /* Run tasks in sequence */
    if (o == 0)
      kgraphMapRbPart2 (contptr, 1, &spltdat);
  }

end1:
  bgraphExit (&actgrafdat);                       /* Free bipartition graph (that is, parttax) */
end2:
  if (indgrafptr == &indgrafdat)                  /* If an induced subgraph had been created */
    graphExit (&indgrafdat);                      /* Free it                                 */
end3:
  if (o != 0) {                                   /* Lock only to propagate errors */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_lock (&mappptr->mutedat);
#endif /* SCOTCH_PTHREAD */
    *spltptr->revaptr = o;
#ifdef SCOTCH_PTHREAD
    pthread_mutex_unlock (&mappptr->mutedat);
#endif /* SCOTCH_PTHREAD */
  }
}

/* This routine is the entry point for
** the degenerated, graph partitioning
** version, of the Dual Recursive
** Bipartitioning algorithm.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
kgraphMapRbPart (
const KgraphMapRbData * restrict const  dataptr,  /*+ Global mapping data                  +*/
const Graph * restrict const            grafptr,  /*+ Graph to map, without fixed vertices +*/
const Anum                              vflonbr,  /*+ Number of fixed vertex load slots    +*/
KgraphMapRbVflo * restrict const        vflotab,  /*+ Array of fixed vertex load slots     +*/
Context * restrict const                contptr)  /*+ Execution context                    +*/
{
  KgraphMapRbPartSplit  spltdat;                  /*+ Data for initial call +*/
  int                   o;

  Mapping * const           mappptr = dataptr->mappptr;

#ifdef SCOTCH_DEBUG_KGRAPH2
  if (dataptr->pfixtax != NULL) {                 /* In debug mode, fixed vertex parts are set to ~0 */
    Gnum                vertnum;

    for (vertnum = dataptr->grafptr->baseval; vertnum < dataptr->grafptr->vertnnd; vertnum ++)
      mappptr->parttax[vertnum] = (dataptr->pfixtax[vertnum] >= 0) ? ~0 : 0;
  }
  else
    memSet (mappptr->parttax + dataptr->grafptr->baseval, 0, dataptr->grafptr->vertnbr * sizeof (Anum));
#endif /* SCOTCH_DEBUG_KGRAPH2 */

#ifdef SCOTCH_PTHREAD
  pthread_mutex_init (&mappptr->mutedat, NULL);   /* Initialize mapping mutex */
#endif /* SCOTCH_PTHREAD */

  mappptr->domnnbr = 0;                           /* Initialize mapping without setting initial domain */

  spltdat.splttab[0].vertnbr = grafptr->vertnbr;  /* Start from initial domain */
  spltdat.splttab[0].vflonbr = vflonbr;
  spltdat.splttab[0].vflotab = vflotab;
  spltdat.splttab[0].domnptr = &dataptr->domnorg; /* Point to initial domain to avoid centralized locking on domain array */
  spltdat.dataptr = dataptr;
  spltdat.grafptr = grafptr;
  spltdat.parttax = NULL;
  spltdat.levlnum = 0;
  spltdat.revaptr = &o;

  o = 0;                                          /* Assume everything will run without error */
  kgraphMapRbPart2 (contptr, 0, &spltdat);        /* Run recursive partitioning on main graph */

#ifdef SCOTCH_PTHREAD
  pthread_mutex_destroy (&mappptr->mutedat);      /* Destroy local mutex */
#endif /* SCOTCH_PTHREAD */

  return (o);
}
