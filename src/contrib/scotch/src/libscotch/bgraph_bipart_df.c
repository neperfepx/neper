/* Copyright 2004,2007,2008,2011-2014,2018,2019,2021,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_df.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes a bipartition of   **/
/**                a bipartition graph by using a          **/
/**                diffusion scheme.                       **/
/**                                                        **/
/**   NOTES      : # This algorithm has been designed to   **/
/**                  work on band graphs only, for which   **/
/**                  the two anchor vertices are the two   **/
/**                  last vertices, the before-last as     **/
/**                  anchor of part 0, and the last as     **/
/**                  anchor of part 1.                     **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 09 jan 2007     **/
/**                                 to   : 10 sep 2007     **/
/**                # Version 5.1  : from : 29 oct 2007     **/
/**                                 to   : 27 mar 2011     **/
/**                # Version 6.0  : from : 07 nov 2011     **/
/**                                 to   : 08 aug 2013     **/
/**                # Version 7.0  : from : 08 jun 2018     **/
/**                                 to   : 07 oct 2026     **/
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
#include "bgraph_bipart_df.h"

/************************************/
/*                                  */
/* The threaded reduction routines. */
/*                                  */
/************************************/

#ifndef BGRAPHBIPARTDFNOTHREAD
static
void
bgraphBipartDfReduceVanc (
BgraphBipartDfThread * restrict const blocptr,    /* Pointer to local block  */
BgraphBipartDfThread * restrict const bremptr,    /* Pointer to remote block */
const void * const                    globptr)    /* Unused                  */
{
  blocptr->vanctab[0] += bremptr->vanctab[0];     /* Accumulate external gains */
  blocptr->vanctab[1] += bremptr->vanctab[1];
}

static
void
bgraphBipartDfReduceVeex (
BgraphBipartDfThread * restrict const blocptr,    /* Pointer to local block  */
BgraphBipartDfThread * restrict const bremptr,    /* Pointer to remote block */
const void * const                    globptr)    /* Unused                  */
{
  blocptr->veexsum  += bremptr->veexsum;          /* Accumulate external gains */
  blocptr->veexsum1 += bremptr->veexsum1;
}

static
void
bgraphBipartDfScan (
BgraphBipartDfThread * restrict const blocptr,    /* Pointer to local block  */
BgraphBipartDfThread * restrict const bremptr,    /* Pointer to remote block */
const int                             srcpval,    /* Source phase value      */
const int                             dstpval,    /* Destination phase value */
const void * const                    globptr)    /* Unused                  */
{
  if (bremptr != NULL) {
    blocptr->fronnnd[dstpval]      = blocptr->fronnnd[srcpval]      + bremptr->fronnnd[srcpval]; /* Compute positions of frontier sub-arrays */
    blocptr->compload1[dstpval]    = blocptr->compload1[srcpval]    + bremptr->compload1[srcpval]; /* Accumulate graph properties            */
    blocptr->compsize1[dstpval]    = blocptr->compsize1[srcpval]    + bremptr->compsize1[srcpval];
    blocptr->commloadextn[dstpval] = blocptr->commloadextn[srcpval] + bremptr->commloadextn[srcpval];
    blocptr->commloadintn[dstpval] = blocptr->commloadintn[srcpval] + bremptr->commloadintn[srcpval];
    blocptr->commgainextn[dstpval] = blocptr->commgainextn[srcpval] + bremptr->commgainextn[srcpval];
  }
  else {
    blocptr->fronnnd[dstpval]      = blocptr->fronnnd[srcpval];
    blocptr->compload1[dstpval]    = blocptr->compload1[srcpval];
    blocptr->compsize1[dstpval]    = blocptr->compsize1[srcpval];
    blocptr->commloadextn[dstpval] = blocptr->commloadextn[srcpval];
    blocptr->commloadintn[dstpval] = blocptr->commloadintn[srcpval];
    blocptr->commgainextn[dstpval] = blocptr->commgainextn[srcpval];
  }
}
#endif /* BGRAPHBIPARTDFNOTHREAD */

/******************************/
/*                            */
/* The threaded loop routine. */
/*                            */
/******************************/

/* This routine computes the diffusion of two
** liquids on the given part of the bipartition
** graph.
** It returns:
** - 0   : if algorithm went up to last pass.
** - !0  : on error.
*/

static
void
bgraphBipartDfLoop (
ThreadDescriptor * restrict const   descptr,
BgraphBipartDfData * restrict const loopptr)
{
  float * restrict      ielstax;                  /* Inverse of edge load sum array           */
  float * restrict      difotax;                  /* Old diffusion value array                */
  float * restrict      difntax;                  /* New diffusion value array                */
  Gnum                  vancnnd;                  /* Index of last non-anchor vertex in range */
  Gnum                  vertbas;                  /* Start index of vertex range              */
  Gnum                  vertnnd;                  /* End index of vertex range                */
  Gnum                  vertnum;
  Gnum                  vertsiz;                  /* Size of local vertex array               */
  Gnum                  fronsiz;                  /* Size of local frontier array             */
  Gnum * restrict       frontab;
  Gnum                  fronnbr;                  /* Local number of frontier vertices        */
  Gnum                  compload1;
  Gnum                  compsize1;
  Gnum                  commloadintn;
  Gnum                  commloadextn;
  Gnum                  commgainextn;
  Gnum                  veexval;
  Gnum                  veexval1;                 /* Negative external gain to part 1         */
  Gnum                  veexsum;
  Gnum                  veexsum1;                 /* Sum of negative external gains           */
  Gnum                  veloval;
  float                 velfval;
  INT                   passnum;
  Anum                  distval;

#ifndef BGRAPHBIPARTDFNOTHREAD
  const int                           thrdnbr = threadNbr (descptr);
  const int                           thrdnum = threadNum (descptr);
#else /* BGRAPHBIPARTDFNOTHREAD */
  const int                           thrdnbr = 1;
  const int                           thrdnum = 0;
#endif /* BGRAPHBIPARTDFNOTHREAD */
  Bgraph * restrict const             grafptr = loopptr->grafptr;
  const Gnum * restrict const         verttax = grafptr->s.verttax;
  const Gnum * restrict const         vendtax = grafptr->s.vendtax;
  const Gnum * restrict const         velotax = grafptr->s.velotax;
  const Gnum * restrict const         edgetax = grafptr->s.edgetax;
  const Gnum * restrict const         edlotax = grafptr->s.edlotax;
  const Gnum * restrict const         veextax = grafptr->veextax;
  GraphPart * restrict const          parttax = grafptr->parttax;
  const int                           thrdlst = thrdnbr - 1;
  const Gnum                          vancnbr = grafptr->s.vertnbr - 2; /* Number of non-anchor vertices in graph   */

  vertbas = grafptr->s.baseval + DATASCAN (vancnbr, thrdnbr, thrdnum); /* Compute bounds of each thread */
  if (thrdnum == thrdnbr - 1) {
    vertnnd = grafptr->s.vertnnd;
    vancnnd = vertnnd - 2;
  }
  else {
    vertnnd = grafptr->s.baseval + DATASCAN (vancnbr, thrdnbr, (thrdnum + 1));
    vancnnd = vertnnd;
  }
  vertsiz = vertnnd - vertbas;                    /* Compute size of vertex sub-array     */
  fronsiz = (thrdnum != 0) ? vertsiz : 0;         /* No extra frontier array for thread 0 */

  difotax = loopptr->difotax;
  difntax = loopptr->difntax;
  distval = grafptr->domndist;

  if (memAllocGroup ((void **) (void *)           /* Allocate here for memory affinity as it is a private array */
                     &ielstax, (size_t) (vertsiz * sizeof (Gnum)), /* Ielstab is group leader as never optional */
                     &frontab, (size_t) (fronsiz * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("bgraphBipartDfLoop: out of memory");
    loopptr->abrtval = 1;
    goto skip;
  }
  if (thrdnum == 0)                               /* Thread 0 will write directly in frontab */
    frontab = grafptr->frontab;

  ielstax -= vertbas;                             /* Base access to local part of edge load sum array */

  veexval  =                                      /* Assume no external gains */
  veexval1 = 0;
  veexsum  =
  veexsum1 = 0;
  for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) { /* Process all vertices, including anchors */
    Gnum                edlosum;

    if (edlotax == NULL)                          /* If graph doesn't have edge weights */
      edlosum = vendtax[vertnum] - verttax[vertnum];
    else {
      Gnum                edgenum;
      Gnum                edgennd;

      for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum], edlosum = 0;
           edgenum < edgennd; edgenum ++)
        edlosum += edlotax[edgenum];
    }
    edlosum *= distval;

    if (veextax != NULL) {
      veexval  = veextax[vertnum];
      veexval1 = veexval & BGRAPHBIPARTDFGNUMSGNMSK (veexval); /* Get negative external gain only, by superscalar update */
#ifdef SCOTCH_DEBUG_BGRAPH2
      if (((veexval >= 0) && (veexval1 != 0)) ||
          ((veexval <  0) && (veexval1 != veexval))) {
        errorPrint ("bgraphBipartDfLoop: internal error");
        loopptr->abrtval = 1;
      }
#endif /* SCOTCH_DEBUG_BGRAPH2 */

      veexsum  += veexval;                        /* Sum all external gains, positive and negative */
      veexsum1 += veexval1;                       /* Sum all negative gains                        */
    }

    difotax[vertnum] = 0.0;
    ielstax[vertnum] = 1.0F / (float) (edlosum + veexval - 2 * veexval1); /* Add absolute value of veexval */
  }
  if (veextax != NULL) {
    loopptr->thrdtab[thrdnum].veexsum  = veexsum;
    loopptr->thrdtab[thrdnum].veexsum1 = veexsum1;
#ifndef BGRAPHBIPARTDFNOTHREAD
    threadReduce (descptr, &loopptr->thrdtab[thrdnum], sizeof (BgraphBipartDfThread), (ThreadReduceFunc) bgraphBipartDfReduceVeex, thrdlst, NULL);
#endif /* BGRAPHBIPARTDFNOTHREAD */
    veexsum  = loopptr->thrdtab[thrdnum].veexsum; /* Will be useful for thread (thrdlst) only */
    veexsum1 = loopptr->thrdtab[thrdnum].veexsum1;

    if (thrdnum == thrdlst) {                     /* Last thread will handle anchors as root of reduction */
      ielstax[vertnnd - 2] = 1.0F / (1.0F / ielstax[vertnnd - 2] + (float) (veexsum - veexsum1));
      ielstax[vertnnd - 1] = 1.0F / (1.0F / ielstax[vertnnd - 1] - (float) veexsum1);
    }
  }
  if (thrdnum == thrdlst) {                       /* Last thread will handle anchors as root of reduction     */
    difotax[vertnnd - 2] = loopptr->vanctab[0] * ielstax[vertnnd - 2]; /* Load anchor vertices for first pass */
    difotax[vertnnd - 1] = loopptr->vanctab[1] * ielstax[vertnnd - 1];
  }

skip:
#ifndef BGRAPHBIPARTDFNOTHREAD
  threadBarrier (descptr);                        /* Wait until all array values have been computed */
#endif /* BGRAPHBIPARTDFNOTHREAD */

  if (loopptr->abrtval == 1) {                    /* If any process decided to quit */
    if (ielstax != NULL)                          /* Free local array if necessary  */
      memFree (ielstax + vertbas);
    return;
  }

  velfval = 1.0F;                                 /* Assume no vertex loads     */
  for (passnum = loopptr->passnbr; passnum > 0; passnum --) { /* For all passes */
    Gnum                vertnum;
    Gnum                vancnnt;
    float               vancval;                  /* Value to load vertex with if anchor   */
    float *             difttax;                  /* Temporary swap value                  */
    float               vancold0 = difotax[grafptr->s.vertnnd - 2]; /* Get for all threads */
    float               vancold1 = difotax[grafptr->s.vertnnd - 1];
    float               vancval0;                 /* External gain contributions from regular vertices to anchors */
    float               vancval1;

    vancval0 =
    vancval1 = 0.0F;
    vancval  = 0.0F;                              /* At first vertices are not anchors           */
    vertnum  = vertbas;                           /* Start processing regular vertices, then see */
    vancnnt  = vancnnd;                           /* Loop until end of (regular) vertex block    */
    while (1) {
      for ( ; vertnum < vancnnt; vertnum ++) {
        Gnum                edgenum;
        Gnum                edgennd;
        float               diffval;

        edgenum = verttax[vertnum];
        edgennd = vendtax[vertnum];
        diffval = 0.0F;
        if (edlotax != NULL)
          for ( ; edgenum < edgennd; edgenum ++)
            diffval += difotax[edgetax[edgenum]] * (float) edlotax[edgenum];
        else
          for ( ; edgenum < edgennd; edgenum ++)
            diffval += difotax[edgetax[edgenum]];

        diffval *= (float) distval;

        if (veextax != NULL) {
          Gnum                veexval;

          veexval = veextax[vertnum];
          if (veexval != 0) {
            float               veextmp;
            float               vanctmp;

            veextmp = (float) veexval;
            vanctmp = veextmp * difotax[vertnum];

            if (veexval > 0) {                    /* If external gain links to part 0  */
              diffval  += veextmp * vancold0;     /* Spread contribution from anchor 0 */
              vancval0 += vanctmp;
            }
            else {                                /* If external gain links to part 1 */
              diffval  -= veextmp * vancold1;     /* Take opposite of negative value  */
              vancval1 -= vanctmp;
            }
          }
        }

        diffval += vancval;                       /* Add anchor contribution if anchor vertex */

        if (velotax != NULL)
          velfval = (float) velotax[vertnum];
        if (diffval >= 0.0F) {
          diffval -= velfval;
          if (diffval <= 0.0F)
            diffval = +BGRAPHBIPARTDFEPSILON;
        }
        else {
          diffval += velfval;
          if (diffval >= 0.0F)
            diffval = -BGRAPHBIPARTDFEPSILON;
        }
        if (isnan (diffval)) {                    /* If overflow occured (because of avalanche process) */
#ifdef SCOTCH_DEBUG_BGRAPH2
          errorPrintW ("bgraphBipartDfLoop: overflow");
#endif /* SCOTCH_DEBUG_BGRAPH2 */
          loopptr->abrtval = 1;                   /* Threads need to halt                      */
          vertnum = vancnnt;                      /* Skip regular computations but synchronize */
        }

        difntax[vertnum] = diffval * ielstax[vertnum];
      }
      if (vertnum == vancnnd) {                   /* If first time we reach the end of regular vertices */
        loopptr->thrdtab[thrdnum].vanctab[0] = vancval0;
        loopptr->thrdtab[thrdnum].vanctab[1] = vancval1;
        if (veextax != NULL) {
#ifndef BGRAPHBIPARTDFNOTHREAD
          threadReduce (descptr, &loopptr->thrdtab[thrdnum], sizeof (BgraphBipartDfThread), (ThreadReduceFunc) bgraphBipartDfReduceVanc, thrdlst, NULL);
#endif /* BGRAPHBIPARTDFNOTHREAD */
        }
      }

      if (vertnum >= vertnnd)                     /* If all vertices processed in range array, exit intermediate infinite loop */
        break;

      vancnnt ++;                                 /* Prepare to go only for one more run, to be done twice */
      vancval = loopptr->vanctab[vertnum - vancnnd] + loopptr->thrdtab[thrdnum].vanctab[vertnum - vancnnd]; /* Load variable with anchor value */
    }

    difttax = (float *) difntax;                  /* Swap old and new diffusion arrays          */
    difntax = (float *) difotax;                  /* Casts to prevent IBM compiler from yelling */
    difotax = (float *) difttax;

#ifndef BGRAPHBIPARTDFNOTHREAD
    threadBarrier (descptr);
#endif /* BGRAPHBIPARTDFNOTHREAD */

    if (loopptr->abrtval == 1) {                  /* If all threads need to abort       */
      difotax = difntax;                          /* Roll-back to keep last valid array */
      break;
    }
  }

  for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) /* Update part according to diffusion state */
    parttax[vertnum] = (difotax[vertnum] <= 0.0F) ? 0 : 1;

#ifndef BGRAPHBIPARTDFNOTHREAD
  threadBarrier (descptr);
#endif /* BGRAPHBIPARTDFNOTHREAD */

  veloval = 1;
  for (vertnum = vertbas, fronnbr = 0, commloadextn = commgainextn = commloadintn = compload1 = compsize1 = 0;
       vertnum < vertnnd; vertnum ++) {
    Gnum                edgenum;
    Gnum                partval;
    Gnum                commload;                 /* Vertex internal communication load */

    partval = (Gnum) parttax[vertnum];
    if (velotax != NULL)
      veloval = velotax[vertnum];
    if (veextax != NULL) {
      Gnum                  veexval;

      veexval = veextax[vertnum];
      commloadextn += veexval * partval;
      commgainextn += veexval * (1 - 2 * partval);
    }
    compsize1 += partval;
    compload1 += partval * veloval;

    commload = 0;
    if (edlotax != NULL) {
      for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
        Gnum                partend;

        partend   = (Gnum) parttax[edgetax[edgenum]];
        commload += (partval ^ partend) * edlotax[edgenum];
      }
    }
    else {
      for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++)
        commload += partval ^ (Gnum) parttax[edgetax[edgenum]];
    }
    commloadintn += commload;                     /* Internal loads will be added twice */
    if (commload != 0)                            /* If end vertex is in the other part */
      frontab[fronnbr ++] = vertnum;              /* Then it belongs to the frontier    */
  }
  loopptr->thrdtab[thrdnum].fronnnd[0]      = fronnbr; /* Save state for scan-reduce */
  loopptr->thrdtab[thrdnum].compload1[0]    = compload1;
  loopptr->thrdtab[thrdnum].compsize1[0]    = compsize1;
  loopptr->thrdtab[thrdnum].commloadextn[0] = commloadextn;
  loopptr->thrdtab[thrdnum].commloadintn[0] = commloadintn;
  loopptr->thrdtab[thrdnum].commgainextn[0] = commgainextn;

#ifndef BGRAPHBIPARTDFNOTHREAD
  threadScan (descptr, (void *) &loopptr->thrdtab[thrdnum], sizeof (BgraphBipartDfThread), (ThreadScanFunc) bgraphBipartDfScan, NULL);
#endif /* BGRAPHBIPARTDFNOTHREAD */

  if (thrdnum != 0)                               /* If thread is not first one, gather frontier sub-array */
    memCpy (grafptr->frontab + loopptr->thrdtab[thrdnum].fronnnd[0] - fronnbr, frontab, fronnbr * sizeof (Gnum));

  memFree (ielstax + vertbas);                    /* Free group leader of local arrays */
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine performs the bipartitioning.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

int
bgraphBipartDf (
Bgraph * restrict const           grafptr,        /*+ Active graph      +*/
const BgraphBipartDfParam * const paraptr)        /*+ Method parameters +*/
{
  BgraphBipartDfData  loopdat;
  Gnum                compload0;

  const int                 thrdnbr = contextThreadNbr (grafptr->contptr);
#ifndef BGRAPHBIPARTDFNOTHREAD
  const int                 thrdlst = thrdnbr - 1;
#else /* BGRAPHBIPARTDFNOTHREAD */
  const int                 thrdlst = 0;
#endif /* BGRAPHBIPARTDFNOTHREAD */

#ifdef SCOTCH_DEBUG_BGRAPH1
  if ((grafptr->s.flagval & BGRAPHHASANCHORS) == 0) { /* Method valid only if graph has anchors */
    errorPrint ("bgraphBipartDf: graph does not have anchors");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH1 */

  if (memAllocGroup ((void **) (void *)
                     &loopdat.thrdtab, (size_t) (thrdnbr            * sizeof (BgraphBipartDfThread)),
                     &loopdat.difotax, (size_t) (grafptr->s.vertnbr * sizeof (float)),
                     &loopdat.difntax, (size_t) (grafptr->s.vertnbr * sizeof (float)), NULL) == NULL) {
    errorPrint ("bgraphBipartDf: out of memory (1)");
    return     (1);
  }

  loopdat.grafptr  = grafptr;
  loopdat.difotax -= grafptr->s.baseval;
  loopdat.difntax -= grafptr->s.baseval;
  loopdat.passnbr  = paraptr->passnbr;

  compload0 = (paraptr->typeval == BGRAPHBIPARTDFTYPEBAL) /* If balanced parts wanted */
              ? grafptr->compload0avg             /* Target is average                */
              : ( (grafptr->compload0 < grafptr->compload0min) ? grafptr->compload0min : /* Else keep load if not off balance */
                 ((grafptr->compload0 > grafptr->compload0max) ? grafptr->compload0max : grafptr->compload0));
  loopdat.vanctab[0] = (float) - compload0;       /* Values to be injected to anchor vertices at every iteration                 */
  loopdat.vanctab[1] = (float) (grafptr->s.velosum - compload0) - BGRAPHBIPARTDFEPSILON; /* Slightly tilt value to add to part 1 */
  loopdat.abrtval = 0;                            /* Nobody wants to abort yet */

#ifndef BGRAPHBIPARTDFNOTHREAD
  contextThreadLaunch (grafptr->contptr, (ThreadFunc) bgraphBipartDfLoop, (void *) &loopdat);
#else /* BGRAPHBIPARTDFNOTHREAD */
  bgraphBipartDfLoop (NULL, &loopdat);
#endif /* BGRAPHBIPARTDFNOTHREAD */

  grafptr->fronnbr      = loopdat.thrdtab[thrdlst].fronnnd[0]; /* Get data after scan-reduction */
  grafptr->compload0    = grafptr->s.velosum - loopdat.thrdtab[thrdlst].compload1[0];
  grafptr->compload0dlt = grafptr->compload0 - grafptr->compload0avg;
  grafptr->compsize0    = grafptr->s.vertnbr - loopdat.thrdtab[thrdlst].compsize1[0];
  grafptr->commload     = loopdat.thrdtab[thrdlst].commloadextn[0] + grafptr->commloadextn0 + (loopdat.thrdtab[thrdlst].commloadintn[0] / 2) * grafptr->domndist;
  grafptr->commgainextn = loopdat.thrdtab[thrdlst].commgainextn[0];
  grafptr->bbalval      = (double) ((grafptr->compload0dlt < 0) ? (- grafptr->compload0dlt) : grafptr->compload0dlt) / (double) grafptr->compload0avg;

  memFree (loopdat.thrdtab);                      /* Free group leader */

#ifdef SCOTCH_DEBUG_BGRAPH2
  if (bgraphCheck (grafptr) != 0) {
    errorPrint ("bgraphBipartDf: inconsistent graph data");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH2 */

  return (0);
}
