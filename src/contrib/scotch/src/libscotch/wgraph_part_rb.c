/* Copyright 2010,2014,2018,2019,2021,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_part_rb.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Jun-Ho HER (v6.0)                       **/
/**                                                        **/
/**   FUNCTION   : This module performs the vertex overla- **/
/**                pped graph partitioning based on recur- **/
/**                sive bipartitioning approach.           **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 16 mar 2010     **/
/**                                 to   : 26 feb 2018     **/
/**                # Version 6.1  : from : 01 nov 2021     **/
/**                                 to   : 25 nov 2021     **/
/**                # Version 7.0  : from : 23 aug 2019     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/**   NOTES      : # This code originally derived from     **/
/**                  the code of kgraph_map_rb_part.c,     **/
/**                  which was then adapted for vertex     **/
/**                  overlapped graph partitioning.        **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "vgraph.h"
#include "vgraph_separate_st.h"
#include "vgraph_separate_zr.h"
#include "wgraph.h"
#include "wgraph_part_rb.h"

/***********************************/
/*                                 */
/* Recursion management routines.  */
/*                                 */
/***********************************/

/* This routine propagates the local frontier
** array to the global frontier array.
** It returns:
** - void  : in all cases.
*/

static
void
wgraphPartRb3Fron (
WgraphPartRbData * restrict const dataptr,        /* Top-level graph and partition data */
const Graph * restrict const      orggrafptr,     /* Graph to induce and bipartition    */
const Gnum * restrict const       orgfrontab,     /* Frontier array of original graph   */
const Gnum                        orgfronnbr)     /* Part of graph to consider          */
{
  Gnum                fronnbr;
  Gnum                fronnum;

  const Gnum * restrict const       orgvnumtax = orggrafptr->vnumtax;
  Gnum * restrict const             frontab    = dataptr->frontab;

#ifdef SCOTCH_PTHREAD
  pthread_mutex_lock (&dataptr->mutedat);         /* Lock frontier mutex */
#endif /* SCOTCH_PTHREAD */
  fronnbr = dataptr->fronnbr;                     /* Get position where to insert frontier */
  dataptr->fronnbr = fronnbr + orgfronnbr;        /* Update current frontier end position  */
#ifdef SCOTCH_PTHREAD
  pthread_mutex_unlock (&dataptr->mutedat);       /* Unlock frontier mutex */
#endif /* SCOTCH_PTHREAD */

  if (orgvnumtax == NULL)                         /* If original graph is not itself a subgraph */
    memCpy (frontab + fronnbr, orgfrontab, orgfronnbr * sizeof (Gnum)); /* Directly copy array  */
  else {                                          /* Original graph is a subgraph               */
    for (fronnum = 0; fronnum < orgfronnbr; fronnum ++, fronnbr ++)
      frontab[fronnbr] = orgvnumtax[orgfrontab[fronnum]];
  }
}

/* This routine propagates the local frontier
** array to the global frontier array, and sets
** the separator part array.
** It returns:
** - void  : in all cases.
*/

static
void
wgraphPartRb3SepFron (
WgraphPartRbData * restrict const dataptr,        /* Top-level graph and partition data */
const Graph * restrict const      orggrafptr,     /* Graph to induce and bipartition    */
const Gnum * restrict const       orgfrontab,     /* Frontier array of original graph   */
const Gnum                        orgfronnbr)     /* Part of graph to consider          */
{
  Gnum                fronnbr;
  Gnum                fronnum;

  const Gnum * restrict const       orgvnumtax = orggrafptr->vnumtax;
  Anum * restrict const             parttax    = dataptr->parttax;
  Gnum * restrict const             frontab    = dataptr->frontab;

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (orgfrontab == NULL) {                       /* Part array must exist */
    errorPrint ("wgraphPartRb3SepFron: invalid parameters");
    return;
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

#ifdef SCOTCH_PTHREAD
  pthread_mutex_lock (&dataptr->mutedat);         /* Lock frontier mutex */
#endif /* SCOTCH_PTHREAD */
  fronnbr = dataptr->fronnbr;                     /* Get position where to insert frontier */
  dataptr->fronnbr = fronnbr + orgfronnbr;        /* Update current frontier end position  */
#ifdef SCOTCH_PTHREAD
  pthread_mutex_unlock (&dataptr->mutedat);       /* Unlock frontier mutex */
#endif /* SCOTCH_PTHREAD */

  if (orgvnumtax == NULL) {                       /* If original graph is not itself a subgraph */
    for (fronnum = 0; fronnum < orgfronnbr; fronnum ++, fronnbr ++) {
      Gnum                vertnum;

      vertnum = orgfrontab[fronnum];
      frontab[fronnbr] = vertnum;
      parttax[vertnum] = -1;
    }
  }
  else {                                          /* Original graph is a subgraph */
    for (fronnum = 0; fronnum < orgfronnbr; fronnum ++, fronnbr ++) {
      Gnum                vertnum;

      vertnum = orgvnumtax[orgfrontab[fronnum]];
      frontab[fronnbr] = vertnum;
      parttax[vertnum] = -1;
    }
  }
}

/* This routine fills the global part array
** with part data from the given part and
** its separator.
** It returns:
** - void  : in all cases.
*/

static
void
wgraphPartRb3One (
WgraphPartRbData * restrict const dataptr,        /* Top-level graph and partition data */
const Graph * restrict const      orggrafptr,     /* Graph to induce and bipartition    */
const GraphPart * restrict const  orgparttax,     /* Part array of original graph       */
const int                         indpartval,     /* Part value to consider             */
const Anum                        inddomnnum)     /* Domain onto which to map the part  */
{
  Anum                indparttmp;                 /* Part value to exclude */
  Gnum                vertnum;

  const Gnum * restrict const       orgvnumtax = orggrafptr->vnumtax;
  Anum * restrict const             parttax    = dataptr->parttax;

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (orgparttax == NULL) {                       /* Graph can never be a full graph */
    errorPrint ("wgraphPartRb3One: invalid parameters");
    return;
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  indparttmp = 1 - indpartval;                    /* Part to exclude from update                */
  if (orgvnumtax == NULL) {                       /* If original graph is not itself a subgraph */
    for (vertnum = orggrafptr->baseval; vertnum < orggrafptr->vertnnd; vertnum ++) {
      GraphPart           orgpartval;

      orgpartval = orgparttax[vertnum];
      if (orgpartval != indparttmp)               /* If vertex belongs to the right part or the separator */
        parttax[vertnum] = (orgpartval == indpartval) ? inddomnnum : -1;
    }
  }
  else {
    for (vertnum = orggrafptr->baseval; vertnum < orggrafptr->vertnnd; vertnum ++) {
      GraphPart           orgpartval;

      orgpartval = orgparttax[vertnum];
      if (orgpartval != indparttmp)               /* If vertex belongs to the right part or the separator */
        parttax[orgvnumtax[vertnum]] = (orgpartval == indpartval) ? inddomnnum : -1;
    }
  }
}

/* This routine fills the global part array
** with part data from both parts and
** their separator.
** It returns:
** - void  : in all cases.
*/

static
void
wgraphPartRb3Both (
WgraphPartRbData * restrict const dataptr,        /* Top-level graph and partition data */
const Graph * restrict const      orggrafptr,     /* Graph to induce and bipartition    */
const GraphPart * restrict const  orgparttax,     /* Part array of original graph       */
const Anum                        inddomnnum)     /* Part of graph to consider          */
{
  Gnum                vertnum;

  const Gnum * restrict const       orgvnumtax = orggrafptr->vnumtax;
  Anum * restrict const             parttax    = dataptr->parttax;

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (orgparttax == NULL) {                       /* Part array must exist */
    errorPrint ("wgraphPartRb3Both: invalid parameters");
    return;
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  if (orgvnumtax == NULL) {                       /* If original graph is not itself a subgraph */
    for (vertnum = orggrafptr->baseval; vertnum < orggrafptr->vertnnd; vertnum ++) {
      GraphPart           orgpartval;

      orgpartval = orgparttax[vertnum];
      parttax[vertnum] = (orgpartval < 2) ? (inddomnnum + (Anum) orgpartval) : -1;
    }
  }
  else {
    for (vertnum = orggrafptr->baseval; vertnum < orggrafptr->vertnnd; vertnum ++) {
      GraphPart           orgpartval;

      orgpartval = orgparttax[vertnum];
      parttax[orgvnumtax[vertnum]] = (orgpartval < 2) ? (inddomnnum + (Anum) orgpartval) : -1;
    }
  }
}

/* This routine is the recursive vertex
** bipartitioning core routine.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
void
wgraphPartRb2 (
Context * restrict const        contptr,          /*+ (Sub-)context                          +*/
const int                       spltnum,          /*+ Rank of sub-context in initial context +*/
const WgraphPartRbSplit * const spltptr)
{
  Vgraph              actgrafdat;
  WgraphPartRbSplit   spltdat;
  int                 partval;
  int                 o;

  WgraphPartRbData * restrict const dataptr = spltptr->dataptr;
  const Graph * restrict const      orggrafptr = spltptr->grafptr; /* Graph to induce and bipartition                      */
  const Gnum * restrict const       orgfrontab = spltptr->frontab; /* Graph frontier array                                 */
  const Gnum                        orgfronnbr = spltptr->fronnbr; /* Number of frontier vertices                          */
  const GraphPart * restrict const  orgparttax = spltptr->parttax; /* Part array of original graph to consider             */
  const GraphPart                   indpartval = (GraphPart) spltnum; /* Part of graph to consider                         */
  const Gnum                        indvertnbr = spltptr->splttab[spltnum].vertnbr; /* Number of vertices in part or graph */
  const Anum                        inddomnnum = spltptr->splttab[spltnum].domnnum; /* Initial domain number to map        */
  const Anum                        inddomnsiz = spltptr->splttab[spltnum].domnsiz; /* Number of domains to map            */

  if (indpartval == 0) {                          /* If in small branch of the recursion; TRICK: never at first call      */
    if (inddomnsiz <= 1) {                        /* If target domain is terminal                                         */
      wgraphPartRb3Fron (dataptr, orggrafptr, orgfrontab, orgfronnbr); /* Copy previous frontier to global frontier array */
      wgraphPartRb3One  (dataptr, orggrafptr, orgparttax, indpartval, inddomnnum); /* Update mapping and return           */
      return;
    }
    wgraphPartRb3SepFron (dataptr, orggrafptr, orgfrontab, orgfronnbr); /* Copy previous frontier to global frontier array and update separator */
  }

  if (orgparttax == NULL) {                       /* If working graph is original graph */
    actgrafdat.s = *orggrafptr;                   /* Clone original graph data          */
    actgrafdat.s.flagval &= ~GRAPHFREETABS;       /* Nothing to be freed (yet)          */
    actgrafdat.s.vlbltax  = NULL;                 /* Vertex labels are no use           */
  }
  else {                                          /* If not the case, build induced subgraph */
    if (graphInducePart (orggrafptr, orgparttax, indvertnbr, indpartval, &actgrafdat.s) != 0) {
      errorPrint ("wgraphPartRb2: cannot induce graph");
      goto fail;
    }
  }

  if (memAllocGroup ((void **) (void *)
                     &actgrafdat.parttax, (size_t) (actgrafdat.s.vertnbr * sizeof (GraphPart)),
                     &actgrafdat.frontab, (size_t) (actgrafdat.s.vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("wgraphPartRb2: out of memory");
    graphExit  (&actgrafdat.s);
    goto fail;
  }
  actgrafdat.parttax   -= actgrafdat.s.baseval;
  actgrafdat.s.flagval |= VGRAPHFREEPART;         /* Free group leader   */
  actgrafdat.levlnum    = 0;                      /* Initial level       */
  actgrafdat.contptr    = contptr;                /* Use current context */

  actgrafdat.dwgttab[0] = inddomnsiz / 2;         /* Compute relative weights of subdomains to compute */
  actgrafdat.dwgttab[1] = inddomnsiz - actgrafdat.dwgttab[0];
  vgraphZero (&actgrafdat);
  if (vgraphSeparateSt (&actgrafdat, dataptr->straptr) != 0) { /* Perform bipartitioning */
    errorPrint ("wgraphPartRb2: cannot bipartition graph");
    vgraphExit (&actgrafdat);
    goto fail;
  }

  if (inddomnsiz <= 2) {                          /* If end of recursion, set both parts and separator */
    wgraphPartRb3Fron (dataptr, &actgrafdat.s, actgrafdat.frontab, actgrafdat.fronnbr);
    wgraphPartRb3Both (dataptr, &actgrafdat.s, actgrafdat.parttax, inddomnnum);
    vgraphExit        (&actgrafdat);
    return;
  }

  o = 0;                                          /* Assume that everything will go well */
  spltdat.splttab[0].domnnum = inddomnnum;
  spltdat.splttab[0].domnsiz = inddomnsiz / 2;    /* Compute median values */
  spltdat.splttab[1].domnnum = inddomnnum + spltdat.splttab[0].domnsiz;
  spltdat.splttab[1].domnsiz = inddomnsiz - spltdat.splttab[0].domnsiz;
  spltdat.dataptr = dataptr;                      /* Refer to global data */
  spltdat.grafptr = &actgrafdat.s;
  spltdat.revaptr = &o;

  if ((partval = 1, actgrafdat.compsize[0] <= 0) || /* If a subpart is empty, run on other part (without considering separator vertices) */
      (partval = 0, actgrafdat.compsize[1] <= 0)) {
    spltdat.splttab[1].vertnbr = actgrafdat.s.vertnbr; /* TRICK: use fake part 1 for calling */
    spltdat.splttab[1].domnnum = spltdat.splttab[partval].domnnum;
    spltdat.splttab[1].domnsiz = spltdat.splttab[partval].domnsiz;
    spltdat.frontab = NULL;                       /* No separator vertices, even if they were some */
    spltdat.fronnbr = 0;
    spltdat.parttax = NULL;
    wgraphPartRb2 (contptr, partval, &spltdat);
    vgraphExit    (&actgrafdat);
    if (o != 0)
      goto fail;
    return;
  }

  spltdat.splttab[0].vertnbr = actgrafdat.compsize[0];
  spltdat.splttab[1].vertnbr = actgrafdat.compsize[1];
  spltdat.frontab = actgrafdat.frontab;
  spltdat.fronnbr = actgrafdat.fronnbr;
  spltdat.parttax = actgrafdat.parttax;

#ifndef WGRAPHPARTRBNOTHREAD
  if (contextThreadLaunchSplit (contptr, (ContextSplitFunc) wgraphPartRb2, &spltdat) != 0) /* If counld not split context to run concurrently */
#endif /* WGRAPHPARTRBNOTHREAD */
  {
    wgraphPartRb2 (contptr, 0, &spltdat);         /* Run tasks in sequence */
    if (o == 0)
      wgraphPartRb2 (contptr, 1, &spltdat);
  }

  vgraphExit (&actgrafdat);

  if (o == 0)                                     /* If no error detected, return directly */
    return;

fail:
#ifdef SCOTCH_PTHREAD
  pthread_mutex_lock (&dataptr->mutedat);
#endif /* SCOTCH_PTHREAD */
  *spltptr->revaptr = 1;
#ifdef SCOTCH_PTHREAD
  pthread_mutex_unlock (&dataptr->mutedat);
#endif /* SCOTCH_PTHREAD */
}

/*********************************************/
/*                                           */
/* This is the entry point for vertex        */
/* overlapped graph partitioning based on    */
/* on the recursive bipartitioning approach. */
/*                                           */
/*********************************************/

int
wgraphPartRb (
Wgraph * restrict const                   grafptr,
const WgraphPartRbParam * restrict const  paraptr)
{
  WgraphPartRbData    datadat;
  WgraphPartRbSplit   spltdat;
  int                 o;

  if (grafptr->partnbr <= 1) {                    /* If only one part needed    */
    wgraphZero (grafptr);                         /* All vertices set to part 0 */
    return (0);
  }

  datadat.grafptr = &grafptr->s;                  /* Start with full graph    */
  datadat.parttax = grafptr->parttax;             /* Take part array          */
  datadat.frontab = grafptr->frontab;             /* Take frontier array      */
  datadat.fronnbr = 0;                            /* No frontier vertices yet */
  datadat.straptr = paraptr->straptr;
  spltdat.splttab[1].vertnbr = grafptr->s.vertnbr; /* TRICK: initial fake part is 1 */
  spltdat.splttab[1].domnnum = 0;
  spltdat.splttab[1].domnsiz = grafptr->partnbr;
  spltdat.dataptr = &datadat;                     /* Refer to global data */
  spltdat.grafptr = &grafptr->s;
  spltdat.frontab = NULL;
  spltdat.fronnbr = 0;
  spltdat.parttax = NULL;
  spltdat.revaptr = &o;

  o = 0;                                          /* Assume everything will go well */
#ifdef SCOTCH_PTHREAD
  pthread_mutex_init (&datadat.mutedat, NULL);    /* Create mutex for global frontier and return values */
#endif /* SCOTCH_PTHREAD */
  wgraphPartRb2 (grafptr->contptr, 1, &spltdat);
#ifdef SCOTCH_PTHREAD
  pthread_mutex_destroy (&datadat.mutedat);
#endif /* SCOTCH_PTHREAD */
  if (o != 0) {
    errorPrint ("wgraphPartRb: cound not perform recursion");
    return (1);
  }
  grafptr->fronnbr = datadat.fronnbr;             /* Set overall number of frontier vertices */

  if (wgraphCost (grafptr) != 0) {
    errorPrint ("wgraphPartRb: could not compute partition cost");
    return (1);
  }

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (wgraphCheck (grafptr) != 0) {
    errorPrint ("wgraphPartRb: inconsistent graph data");
    return (1);
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  return (0);
}
