/* Copyright 2004,2007,2009,2011,2013,2014,2016,2019,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_gg.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Luca SCARANO (v3.1)                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module computes a bipartition of   **/
/**                a bipartition graph by multiple runs of **/
/**                the greedy graph growing algorithm.     **/
/**                                                        **/
/**   DATES      : # Version 3.1  : from : 07 jan 1996     **/
/**                                 to   : 07 jun 1996     **/
/**                # Version 3.2  : from : 20 sep 1996     **/
/**                                 to   : 13 sep 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 3.4  : from : 01 jun 2001     **/
/**                                 to   : 01 jun 2001     **/
/**                # Version 4.0  : from : 09 jan 2004     **/
/**                                 to   : 01 sep 2004     **/
/**                # Version 5.0  : from : 02 jan 2007     **/
/**                                 to   : 04 feb 2007     **/
/**                # Version 5.1  : from : 21 nov 2007     **/
/**                                 to   : 22 feb 2011     **/
/**                # Version 6.0  : from : 23 feb 2011     **/
/**                                 to   : 01 may 2016     **/
/**                # Version 7.0  : from : 12 sep 2019     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_BGRAPH_BIPART_GG

#define SCOTCH_TABLE_GAIN

#include "module.h"
#include "common.h"
#include "gain.h"
#include "fibo.h"
#include "graph.h"
#include "arch.h"
#include "bgraph.h"
#include "bgraph_bipart_gg.h"

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

#ifndef SCOTCH_TABLE_GAIN

/* bgraphBipartFmCmpFunc(a,b) must return a negative
** number if a is "better" than b. The smaller, the
** better.
*/

static
int
bgraphBipartGgCmpFunc (
const FiboNode * const      data0ptr,             /* TRICK: BgraphBipartFmLink is FIRST in BgraphBipartFmVertex */
const FiboNode * const      data1ptr)
{
  const BgraphBipartGgVertex * const  node0ptr = (BgraphBipartGgVertex *) data0ptr;
  const BgraphBipartGgVertex * const  node1ptr = (BgraphBipartGgVertex *) data1ptr;

  if (node0ptr->commgain < node1ptr->commgain)
    return (-1);
  if (node0ptr->commgain > node1ptr->commgain)
    return (1);
  return (0);
}

#endif /* SCOTCH_TABLE_GAIN */

/* This routine performs the bipartitioning.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

static
void
bgraphBipartGg2 (
ThreadDescriptor * restrict const   descptr,
BgraphBipartGgData * restrict const passptr)
{
  Context                 contdat;                /* Local context, only used for its random section */
  Context *               contptr;                /* Pointer to active local context                 */
  IntRandContext          randdat;                /* Local random context                            */
  BgraphBipartGgTabl      tabldat;                /* Gain table                                      */
  BgraphBipartGgVertex *  vexxtax;                /* Extended vertex array [norestrict]              */
  BgraphBipartGgVertex *  vexxptr;                /* Pointer to current vertex to swap [norestrict]  */
  Gnum                    partsiz;                /* Size of part array, for (thrdnum != 0)          */
  GraphPart * restrict    parttax;                /* Local or main part array                        */
  Gnum * restrict         permtab;                /* Permutation array for finding new roots         */
  Gnum                    permnum;                /* Current permutation index                       */
  INT                     passnum;

#ifndef BGRAPHBIPARTGGNOTHREAD
  const int                     thrdnum = threadNum (descptr);
#else /* BGRAPHBIPARTGGNOTHREAD */
  const int                     thrdnum = 0;
#endif /* BGRAPHBIPARTGGNOTHREAD */
  Bgraph * restrict const       grafptr = passptr->grafptr;
  const Gnum                    vertnnd = grafptr->s.vertnnd;
  const Gnum * restrict const   verttax = grafptr->s.verttax;
  const Gnum * restrict const   vendtax = grafptr->s.vendtax;
  const Gnum * restrict const   velotax = grafptr->s.velotax;
  const Gnum * restrict const   edgetax = grafptr->s.edgetax;
  const Gnum * restrict const   edlotax = grafptr->s.edlotax;
  BgraphBipartGgThread * const  thrdptr = &passptr->thrdtab[thrdnum];
  const Anum                    dod2val = grafptr->domndist * 2;
  const Gnum * restrict const   cmg0tax = passptr->cmg0tax;

  thrdptr->parttab = NULL;                        /* In case of error                 */
  partsiz = (thrdnum == 0) ? 0 : grafptr->s.vertnbr; /* Thread 0 uses graph part data */
  if ((bgraphBipartGgTablInit (&tabldat) != 0) ||
      (memAllocGroup ((void **) (void *)          /* Allocate here for memory affinity as it is a private array */
                      &thrdptr->parttab, (size_t) (partsiz            * sizeof (GraphPart)),
                      &vexxtax,          (size_t) (grafptr->s.vertnbr * sizeof (BgraphBipartGgVertex)), NULL) == NULL)) {
    errorPrint ("bgraphBipartGg2: out of memory (1)");
fail:
    if (thrdptr->parttab != NULL) {               /* In case we fail later */
      memFree (thrdptr->parttab);
      thrdptr->parttab = NULL;                    /* Indicate an error */
    }
    bgraphBipartGgTablExit (&tabldat);
    return;
  }
  vexxtax -= grafptr->s.baseval;
  permtab  = NULL;                                /* Do not allocate permutation array yet */

  if (thrdnum == 0) {                             /* If root process      */
    parttax = grafptr->parttax;                   /* Use graph part array */
    contptr = grafptr->contptr;                   /* Use provided context */
  }
  else {
    parttax = thrdptr->parttab - grafptr->s.baseval; /* Use local array                       */
    intRandSpawn (grafptr->contptr->randptr, thrdnum, &randdat); /* Create fake local context */
    contdat.randptr = &randdat;
    contptr = &contdat;
  }

  for (passnum = 0; passnum < passptr->passnbr; passnum ++) { /* For all local passes */
    Gnum                vertnum;
    Gnum                cmloval;
    Gnum                cpl0dlt;

    for (vertnum = grafptr->s.baseval; vertnum < vertnnd; vertnum ++) { /* Reset extended vertex array */
      bgraphBipartGgSetFree (&vexxtax[vertnum]);  /* TRICK: gain link is first field of data structure */
      vexxtax[vertnum].cmgnval = cmg0tax[vertnum];
    }
    bgraphBipartGgTablFree (&tabldat);            /* Reset gain table                     */
    permnum = 0;                                  /* No permutation built yet             */
    cpl0dlt = grafptr->s.velosum - grafptr->compload0avg; /* Reset bipartition parameters */
    cmloval = grafptr->commloadextn0;

    vexxptr = vexxtax + (grafptr->s.baseval + contextIntRandVal (contptr, grafptr->s.vertnbr)); /* Randomly select first root vertex */

    do {                                          /* For all root vertices, till balance */
#ifdef SCOTCH_TABLE_GAIN
      vexxptr->linkdat.next =                     /* TRICK: allow deletion of root vertex */
      vexxptr->linkdat.prev = (GainLink *) vexxptr;
#ifdef SCOTCH_DEBUG_BGRAPH2
      vexxptr->linkdat.tabl = NULL;
#endif /* SCOTCH_DEBUG_BGRAPH2 */
#endif /* SCOTCH_TABLE_GAIN    */

      do {                                        /* As long as vertices can be retrieved */
        Gnum                        vertnum;      /* Number of current vertex             */
        Gnum                        veloval;      /* Load of selected vertex              */
        Gnum                        edgenum;      /* Number of current edge               */
        Gnum                        edgennd;      /* End index of edge sub-array          */

#ifndef SCOTCH_TABLE_GAIN
        if (bgraphBipartGgIsTabl (vexxptr))
#endif /* SCOTCH_TABLE_GAIN */
        bgraphBipartGgTablDel (&tabldat, vexxptr); /* Remove vertex from table */

        vertnum = (Gnum) (vexxptr - vexxtax);     /* Get number of selected vertex */
        veloval = (velotax == NULL) ? 1 : velotax[vertnum];

        if ((ABS (cpl0dlt - veloval) >= ABS (cpl0dlt)) && /* If swapping would cause imbalance */
            (veloval > 0)) {                      /* And not a zero weight vertex                        */
#ifndef SCOTCH_TABLE_GAIN
          bgraphBipartGgNext (vexxptr) = BGRAPHBIPARTGGSTATELINK; /* Vertex belongs to frontier of part 0 */
#endif /* SCOTCH_TABLE_GAIN */
          permnum = grafptr->s.vertnbr;           /* Terminate swapping process */
          vexxptr = NULL;
          break;
        }

        bgraphBipartGgSetUsed (vexxptr);          /* Mark it as swapped          */
        cpl0dlt -= veloval;                       /* Update partition parameters */
        cmloval += vexxptr->cmgnval;
        for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum]; /* (Re-)link neighbors */
             edgenum < edgennd; edgenum ++) {
          BgraphBipartGgVertex *  vexxend;        /* Pointer to end vertex of current edge */

          vexxend = &vexxtax[edgetax[edgenum]];   /* Point to end vertex            */
          if (! bgraphBipartGgIsUsed (vexxend)) { /* If vertex needs to be updated  */
            Gnum                edloval;

            edloval = (edlotax == NULL) ? 1 : edlotax[edgenum];
            vexxend->cmgnval -= edloval * dod2val; /* Adjust gain value             */
            if (bgraphBipartGgIsTabl (vexxend))   /* If vertex is linked            */
              bgraphBipartGgTablDel (&tabldat, vexxend); /* Remove it from table    */
            bgraphBipartGgTablAdd (&tabldat, vexxend); /* (Re-)link vertex in table */
          }
        }
      } while ((vexxptr = (BgraphBipartGgVertex *) bgraphBipartGgTablFrst (&tabldat)) != NULL);

      if (permnum == 0) {                         /* If permutation has not been built yet  */
        if (permtab == NULL) {                    /* If permutation array not allocated yet */
          if ((permtab = (Gnum *) memAlloc (grafptr->s.vertnbr * sizeof (Gnum))) == NULL) {
            errorPrint ("bgraphBipartGg2: out of memory (2)");
            goto fail;
          }
          intAscn (permtab, grafptr->s.vertnbr, grafptr->s.baseval); /* Initialize based permutation array */
        }
        intPerm (permtab, grafptr->s.vertnbr, contptr); /* Build random permutation */
      }
      for ( ; permnum < grafptr->s.vertnbr; permnum ++) { /* Find next root vertex */
        if (bgraphBipartGgIsFree (&vexxtax[permtab[permnum]])) {
          vexxptr = vexxtax + permtab[permnum ++];
          break;
        }
      }
    } while (vexxptr != NULL);

    if ((passnum == 0) ||                         /* If first try                        */
        ( (thrdptr->cmloval >  cmloval) ||        /* Or if better local solution reached */
         ((thrdptr->cmloval == cmloval) &&
          (ABS (thrdptr->cpl0dlt) > ABS (cpl0dlt))))) {
      Gnum                vertnum;

      thrdptr->cmloval = cmloval;                 /* Record current solution */
      thrdptr->cpl0dlt = cpl0dlt;

      for (vertnum = grafptr->s.baseval; vertnum < vertnnd; vertnum ++) /* Copy bipartition state with flag 2 for tabled vertices */
        parttax[vertnum] = (bgraphBipartGgIsTabl (&vexxtax[vertnum])) ? 2 : (GraphPart) ((intptr_t) bgraphBipartGgNext (&vexxtax[vertnum]));
    }
  }

  if (permtab != NULL)                            /* Free work arrays but keep parttax for later processing */
    memFree (permtab);
  bgraphBipartGgTablExit (&tabldat);
}

int
bgraphBipartGg (
Bgraph * restrict const           grafptr,        /*+ Active graph      +*/
const BgraphBipartGgParam * const paraptr)        /*+ Method parameters +*/
{
  BgraphBipartGgData    passdat;                  /* Common data for parallel passes */
  GraphPart * restrict  parttax;
  Gnum                  ver1nbr;
  Gnum                  vertnum;
  byte * restrict       flagtax;
  Gnum * restrict       frontab;
  Gnum                  fronnum;
  int                   thrdbst;
  int                   thrdnum;
  Gnum                  cmloval;
  Gnum                  cmgxval;
  Gnum                  cpl0dlt;
  int                   o;

#ifndef BGRAPHBIPARTGGNOTHREAD
  const int                   thrdnbr = contextThreadNbr (grafptr->contptr);
#else /* BGRAPHBIPARTGGNOTHREAD */
  const int                   thrdnbr = 1;
#endif /* BGRAPHBIPARTGGNOTHREAD */
  const Gnum                  vertnnd = grafptr->s.vertnnd; /* Fast accesses */
  const Gnum * restrict const verttax = grafptr->s.verttax;
  const Gnum * restrict const vendtax = grafptr->s.vendtax;
  const Gnum * restrict const edgetax = grafptr->s.edgetax;
  const Gnum * restrict const edlotax = grafptr->s.edlotax;
  const Gnum * restrict const veextax = grafptr->veextax;
  const Gnum                  dodival = grafptr->domndist;

  if (memAllocGroup ((void **) (void *)           /* Allocate shared data */
                     &passdat.thrdtab, (size_t) (thrdnbr            * sizeof (BgraphBipartGgThread)),
                     &passdat.cmg0tax, (size_t) (grafptr->s.vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("bgraphBipartGg: out of memory");
    return (1);
  }
  passdat.cmg0tax -= grafptr->s.baseval;
  passdat.grafptr  = grafptr;
  passdat.passnbr  = (paraptr->passnbr + (thrdnbr - 1)) / thrdnbr; /* Share passes across threads */

  if (edlotax == NULL) {                          /* If graph has no edge weights */
    Gnum                vertnum;
    Gnum * restrict     cmg0tax = passdat.cmg0tax;

    for (vertnum = grafptr->s.baseval; vertnum < vertnnd; vertnum ++) {
      Gnum                cmloval;

      cmloval = (vendtax[vertnum] - verttax[vertnum]) * dodival;
      cmg0tax[vertnum] = (veextax == NULL) ? cmloval : (cmloval + veextax[vertnum]);
    }
  }
  else {                                          /* Graph has edge weights */
    Gnum                vertnum;
    Gnum * restrict     cmg0tax = passdat.cmg0tax;

    for (vertnum = grafptr->s.baseval; vertnum < vertnnd; vertnum ++) {
      Gnum                cmloval;
      Gnum                edgenum;

      for (edgenum = verttax[vertnum], cmloval = 0;
           edgenum < vendtax[vertnum]; edgenum ++)
        cmloval += edlotax[edgenum];
      cmloval *= dodival;

      cmg0tax[vertnum] = (veextax == NULL) ? cmloval : (cmloval + veextax[vertnum]);
    }
  }

#ifndef BGRAPHBIPARTGGNOTHREAD
  contextThreadLaunch (grafptr->contptr, (ThreadFunc) bgraphBipartGg2, (void *) &passdat);
#else /* BGRAPHBIPARTGGNOTHREAD */
  bgraphBipartGg2 (NULL, &passdat);
#endif /* BGRAPHBIPARTGGNOTHREAD */

  for (thrdnum = 0, thrdbst = -1; thrdnum < thrdnbr; thrdnum ++) { /* Find best thread */
    if (passdat.thrdtab[thrdnum].parttab == NULL) /* If partition is invalid, skip it  */
      continue;

    if (thrdbst == -1) {                          /* If first valid partition */
found:
      thrdbst = thrdnum;                          /* Record location of current best partition and preserve array */
      cmloval = passdat.thrdtab[thrdnum].cmloval;
      cpl0dlt = passdat.thrdtab[thrdnum].cpl0dlt;
      continue;
    }
    if ( (passdat.thrdtab[thrdnum].cmloval <  cmloval) || /* If better partition */
        ((passdat.thrdtab[thrdnum].cmloval == cmloval) &&
         (passdat.thrdtab[thrdnum].cpl0dlt <  cpl0dlt))) {
      memFree (passdat.thrdtab[thrdbst].parttab); /* Free preserved former best partition */
      goto found;                                 /* Record new best partition            */
    }
    else                                          /* Partition is no better      */
      memFree (passdat.thrdtab[thrdnum].parttab); /* Free its local group leader */
  }

  if (thrdbst < 0) {                              /* If no valid partition found */
    o = 1;
    goto fail;
  }

  grafptr->compload0dlt = cpl0dlt;               /* Set graph parameters */
  grafptr->commload     = cmloval;
  if (thrdbst != 0)                              /* Put partition data in place if not first thread */
    memCpy (grafptr->parttax + grafptr->s.baseval, passdat.thrdtab[thrdbst].parttab, grafptr->s.vertnbr * sizeof (GraphPart));
  memFree (passdat.thrdtab[thrdbst].parttab);    /* Free group leader of best thread */

  parttax = grafptr->parttax;
  frontab = grafptr->frontab;
  flagtax = (byte *) (passdat.cmg0tax + grafptr->s.baseval) - grafptr->s.baseval; /* Re-use extended vertex array for flag array */
  memSet (flagtax + grafptr->s.baseval, ~0, grafptr->s.vertnbr * sizeof (byte));
  for (vertnum = grafptr->s.baseval, fronnum = 0, ver1nbr = 0, cmgxval = grafptr->commgainextn0;
       vertnum < vertnnd; vertnum ++) {
    Gnum                partval;

    partval = (Gnum) parttax[vertnum];
    if (partval > 1) {                            /* If vertex belongs to frontier of part 0 */
      Gnum                edgenum;
      Gnum                frontmp;                /* Temporary count value for frontier */

      frontab[fronnum ++] = vertnum;              /* Then it belongs to the frontier */
      parttax[vertnum]    = 0;                    /* And it belongs to part 0        */
      for (edgenum = verttax[vertnum], frontmp = 1;
           edgenum < vendtax[vertnum]; edgenum ++) {
        Gnum                vertend;

        vertend = edgetax[edgenum];
        if (parttax[vertend] == 1) {              /* If vertex belongs to other part       */
          frontmp = 0;                            /* Then first frontier vertex was useful */
          if (flagtax[vertend] != 0) {            /* If vertex has not yet been flagged    */
            frontab[fronnum ++] = vertend;        /* Then add it to the frontier           */
            flagtax[vertend] = 0;                 /* Flag it                               */
          }
        }
      }
      fronnum -= frontmp;                         /* Remove vertex from frontier if it was useless */
    }
    partval &= 1;
    ver1nbr += partval;
    if (veextax != NULL)
      cmgxval -= partval * 2 * veextax[vertnum];
  }
  grafptr->fronnbr      = fronnum;
  grafptr->compload0    = grafptr->compload0avg + grafptr->compload0dlt;
  grafptr->compsize0    = grafptr->s.vertnbr - ver1nbr;
  grafptr->commgainextn = cmgxval;
  grafptr->bbalval      = (double) ((grafptr->compload0dlt < 0) ? (- grafptr->compload0dlt) : grafptr->compload0dlt) / (double) grafptr->compload0avg;

  o = 0;                                          /* Everyhting went well */
#ifdef SCOTCH_DEBUG_BGRAPH2
  if (bgraphCheck (grafptr) != 0) {
    errorPrint ("bgraphBipartGg: inconsistent graph data");
    o = 1;
  }
#endif /* SCOTCH_DEBUG_BGRAPH2 */

fail:
  memFree (passdat.thrdtab);                      /* Free group leader */

  return (o);
}
