/* Copyright 2010-2012,2018,2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_df.c                         **/
/**                                                        **/
/**   AUTHOR     : Sebastien FOURESTIER (v6.0)             **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module computes a k-way partition  **/
/**                of the given mapping graph by applying  **/
/**                a diffusion method to what is assumed   **/
/**                to be a band graph.                     **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 05 jan 2010     **/
/**                                 to   : 04 nov 2012     **/
/**                # Version 7.0  : from : 03 aug 2018     **/
/**                                 to   : 20 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_KGRAPH_MAP_DF

#include "module.h"
#include "common.h"
#include "arch.h"
#include "graph.h"
#include "mapping.h"
#include "kgraph.h"
#include "kgraph_map_df.h"

/************************/
/*                      */
/* The sorting routine. */
/*                      */
/************************/

/* This routine sorts an array of KgraphMapDfVertex
** values in descending order by their amount of liquid.
** By nature of the sorting algorithm, data are left in
** place in case of equality. Therefore, the original
** part of the vertex, which is put first in the sort
** array during the diffusion process, is always preserved
** when all liquid amounts are equal.
** It returns:
** - void  : in all cases.
*/

#define INTSORTQUAL                 static
#define INTSORTNAME                 kgraphMapDfSort
#define INTSORTSIZE                 (sizeof (KgraphMapDfSort))
#define INTSORTSWAP(p,q)            do {                                                       \
                                      KgraphMapDfSort t;                                       \
                                      t = *((KgraphMapDfSort *) (p));                          \
                                      *((KgraphMapDfSort *) (p)) = *((KgraphMapDfSort *) (q)); \
                                      *((KgraphMapDfSort *) (q)) = t;                          \
                                    } while (0)
#define INTSORTCMP(p,q)             (((KgraphMapDfSort *) (p))->diffval > ((KgraphMapDfSort *) (q))->diffval)
#include "common_sort.c"
#undef INTSORTQUAL
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/********************************/
/*                              */
/* The sequential loop routine. */
/*                              */
/********************************/

/* This routine computes the diffusion of two
** liquids on the given part of the bipartition
** graph.
** It returns:
** - 0   : if algorithm went up to last pass.
** - !0  : on error.
*/

/* Tests flags for mapping TODO remove it after performance tests */
/* #define KGRAPHDIFFMAPPNONE */                    /* No special code for mapping                      */
/* #define KGRAPHDIFFMAPPMORE */                    /* Give more liquid on expensive architecture edges */
#define KGRAPHDIFFMAPPLESS                        /* Give less liquid on expensive architecture edges */

static
void
kgraphMapDfLoop (
ThreadDescriptor * restrict const descptr,
KgraphMapDfData * restrict const  loopptr)
{
  KgraphMapDfVertex * restrict  difotax;          /* Old diffusion value array               */
  KgraphMapDfVertex * restrict  difntax;          /* New diffusion value array               */
  KgraphMapDfSort * restrict    sorttab;          /* Liquid sort array                       */
  Gnum                          vertbas;          /* Range of non-anchor vertices to process */
  Gnum                          vertnnd;
  Gnum                          vertnum;
  Anum                          domnbas;          /* Range of anchor vertices to process     */
  Anum                          domnnnd;
  Anum                          domnnum;
  Gnum                          passnum;
  int                           velsmsk;
  int                           mappflag = 0;     /* Flag set if we are computing a mapping  */

#ifndef KGRAPHMAPDFNOTHREAD
  const int                           thrdnbr = threadNbr (descptr);
  const int                           thrdnum = threadNum (descptr);
#else /* KGRAPHMAPDFNOTHREAD */
  const int                           thrdnbr = 1;
  const int                           thrdnum = 0;
#endif /* KGRAPHMAPDFNOTHREAD */
  const Kgraph * restrict const       grafptr = loopptr->grafptr;
  const Gnum                          baseval = grafptr->s.baseval;
  float * restrict const              vanctab = loopptr->vanctab;
  float * restrict const              valotab = loopptr->valotab; /* Fraction of load to leak */
  Gnum * restrict const               velstax = loopptr->velstax;
  const Arch * restrict const         archptr = grafptr->m.archptr;
  const Anum                          domnnbr = grafptr->m.domnnbr;
  Anum * restrict const               parttax = grafptr->m.parttax;
  const float                         crloval = (float) grafptr->r.crloval;
  const Anum * restrict const         parotax = grafptr->r.m.parttax;
  const Gnum * restrict const         verttax = grafptr->s.verttax;
  const Gnum * restrict const         vendtax = grafptr->s.vendtax;
  const Gnum * restrict const         velotax = grafptr->s.velotax;
  const Gnum * const                  edgetax = grafptr->s.edgetax;
  const Gnum * const                  edlotax = grafptr->s.edlotax;
  const Gnum                          vancnbr = grafptr->s.vertnbr - domnnbr;
  const Gnum                          vancnnd = grafptr->s.vertnnd - domnnbr;

  domnbas = DATASCAN (domnnbr, thrdnbr, thrdnum);
  domnnnd = DATASCAN (domnnbr, thrdnbr, thrdnum + 1);
  vertbas = baseval + DATASCAN (vancnbr, thrdnbr, thrdnum);
  vertnnd = baseval + DATASCAN (vancnbr, thrdnbr, thrdnum + 1);

  sorttab = NULL;                                 /* In case of abort */

  velsmsk = 1;                                    /* Assume no anchors are isolated */
  if (edlotax != NULL) {
    for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) { /* For all local non-anchor vertices */
      Gnum                velssum;
      Gnum                edgenum;
      Gnum                edgennd;

#ifdef SCOTCH_DEBUG_KGRAPH2
      if ((vendtax[vertnum] - verttax[vertnum]) == 0) { /* Non-anchor vertices should not be isolated */
        errorPrint ("kgraphMapDfLoop: internal error (1)");
        loopptr->abrtval = 1;
        goto fail;
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

      for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum], velssum = 0;
           edgenum < edgennd; edgenum ++)
        velssum += edlotax[edgenum];
      velstax[vertnum] = velssum;
    }

    for (domnnum = domnbas; domnnum < domnnnd; domnnum ++) { /* For all local anchor vertices */
      Gnum                edgenum;
      Gnum                edgennd;
      Gnum                velssum;

      for (edgenum = verttax[vancnnd + domnnum], edgennd = vendtax[vancnnd + domnnum], velssum = 0;
           edgenum < edgennd; edgenum ++)
        velssum += edlotax[edgenum];
      velstax[vancnnd + domnnum] = velssum;
      velsmsk &= (velssum != 0);
    }
  }
  else {
    for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) { /* For all local non-anchor vertices */
#ifdef SCOTCH_DEBUG_KGRAPH2
      if ((vendtax[vertnum] - verttax[vertnum]) == 0) { /* Non-anchor vertices should not be isolated */
        errorPrint ("kgraphMapDfLoop: internal error (2)");
        loopptr->abrtval = 1;
        goto fail;
      }
#endif /* SCOTCH_DEBUG_KGRAPH2 */

      velstax[vertnum] = vendtax[vertnum] - verttax[vertnum];
    }
    for (domnnum = domnbas; domnnum < domnnnd; domnnum ++) { /* For all local anchor vertices */
      Gnum                velssum;

      velssum = vendtax[vancnnd + domnnum] - verttax[vancnnd + domnnum]; /* Local degree of anchor vertices */
      velstax[vancnnd + domnnum] = velssum;
      velsmsk &= (velssum != 0);
    }
  }
  if (velsmsk == 0) {                             /* If graph is too small to have any usable anchors */
    loopptr->abrtval = 1;                         /* We will leave during the first iteration         */
    goto fail;
  }

  if ((sorttab = memAlloc (domnnbr * sizeof (KgraphMapDfSort))) == NULL) { /* Allocate here for memory affinity as it is a private array */
    errorPrint ("kgraphMapDfLoop: out of memory");
    loopptr->abrtval = 1;
    goto fail;
  }

  if (velotax == NULL) {
    for (domnnum = domnbas; domnnum < domnnnd; domnnum ++)
      valotab[domnnum] = 1.0F;
  }
  else {
    for (domnnum = domnbas; domnnum < domnnnd; domnnum ++)
      valotab[domnnum] = (float) velotax[vancnnd + domnnum];
  }

  difntax = loopptr->difntax;
  difotax = loopptr->difotax;

  for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) {
    difotax[vertnum].partval = parttax[vertnum]; /* Set initial part by default */
    difotax[vertnum].diffval =
    difotax[vertnum].fdifval =
    difotax[vertnum].mdisval =
    difotax[vertnum].mdidval =
    difntax[vertnum].fdifval =
    difntax[vertnum].mdisval =
    difntax[vertnum].mdidval = 0.0F;
  }

#ifndef KGRAPHMAPDFNOTHREAD
  threadBarrier (descptr);                        /* Make sure all of velstax is written */
#endif /* KGRAPHMAPDFNOTHREAD */

  for (domnnum = domnbas, vertnum = vancnnd + domnbas; /* For all the subset of anchor vertices */
       domnnum < domnnnd; domnnum ++, vertnum ++) {
    float               vancval;
    Gnum                comploadbal;              /* Compload to reach to get wished balance */

    if (velstax[vancnnd + domnnum] <= 0) {
      vancval          =
      vanctab[domnnum] = 0.0F;
      velstax[vertnum] = -1;
    }
    else {
      comploadbal = grafptr->comploadavg[domnnum];
      vancval = ((float) comploadbal - valotab[domnnum]) / (float) velstax[vancnnd + domnnum]; /* Amount of liquid to be added at each step */
      vanctab[domnnum] = (float) comploadbal;
    }
    difotax[vertnum].diffval = vancval;           /* Load anchor vertices for first pass */
    difotax[vertnum].partval =
    difntax[vertnum].partval = domnnum;
    difntax[vertnum].diffval =                    /* In case of isolated anchors, do not risk overflow because of NaN */
    difotax[vertnum].fdifval =
    difotax[vertnum].mdisval =
    difotax[vertnum].mdidval =
    difntax[vertnum].fdifval =
    difntax[vertnum].mdisval =
    difntax[vertnum].mdidval = 0.0F;              /* Do not consider migration costs for anchors */
  }

#ifndef KGRAPHMAPDFNOTHREAD
  threadBarrier (descptr);
#endif /* KGRAPHMAPDFNOTHREAD */

  if (loopptr->abrtval == 1) {                    /* If process alone or some decided to quit */
    memFree (sorttab);                            /* Free local array                         */
    return;
  }

#ifndef KGRAPHDIFFMAPPNONE
  if (! archPart (archptr))
    mappflag = 1;
#endif /* KGRAPHDIFFMAPPNONE */

  passnum = loopptr->passnbr;
  for ( ; passnum > 0; passnum --) {              /* For all passes       */
    KgraphMapDfVertex * difttax;                  /* Temporary swap value */
    Gnum                vertnum;
    float               veloval;

    veloval = 1.0F;                               /* Assume no vertex loads */

    for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) { /* For all local regular vertices */
      Gnum                edgenum;
      Gnum                edgennd;
      Gnum                soplval;                /* Load sum of edges going to vertex old part                 */
      Gnum                sfplval;                /* Load sum of edges going to vertex of other parts           */
      Gnum                dfplval;                /* Load sum of edges going to vertex of other parts * distval */
      float               migrval;
      Anum                partnbr;                /* Number of active parts */
      Anum                partnum;
      float               diffval;
      Anum                partcur;

      partnbr            = 1;                     /* Keep vertex in first place to preserve its part */
      partcur            =
      sorttab[0].partval = difotax[vertnum].partval; /* Always keep old part value                */
      sorttab[0].diffval = 0.0F;                  /* Assume at first it is null                   */
      sorttab[0].edlosum = 0;                     /* Assume at first there are no loads           */
      sorttab[0].distval = 1;                     /* Do not take distval of our part into account */

      for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum];
           edgenum < edgennd; edgenum ++) {
        Gnum                vertend;
        float               diffval;
        float               fdifval;
        float               mdisval;
        float               mdidval;
        Anum                partval;
        Anum                partnum;
        Gnum                edloval;

        vertend = edgetax[edgenum];
        edloval = (edlotax != NULL) ? edlotax[edgenum] : 1;

        partval = difotax[vertend].partval;
        diffval = difotax[vertend].diffval;       /* Value is not yet scaled with respect to diffusion coefficient */
        fdifval = difotax[vertend].fdifval;
        mdisval = difotax[vertend].mdisval;
        mdidval = difotax[vertend].mdidval;

        if ((mappflag == 1) && (partval != partcur))
          diffval = fdifval;

        diffval *= (float) edloval * crloval;
        if (parotax != NULL) {
          if (difotax[vertnum].partval == parotax[vertend])
            diffval += mdisval;
          else
            diffval += mdidval;
        }

        for (partnum = 0; partnum < partnbr; partnum ++) {
          if (sorttab[partnum].partval == partval) {
            sorttab[partnum].diffval += diffval;  /* Accumulate contribution in slot */
            sorttab[partnum].edlosum += edloval;
            goto endloop1;                        /* Do not consider creating a new slot */
          }
        }
        sorttab[partnbr].partval = partval;       /* Create new slot */
        sorttab[partnbr].distval = ((mappflag == 1) && (partcur != partval)) ? archDomDist (archptr, &grafptr->m.domntab[partcur], &grafptr->m.domntab[partval]) : 1;
        sorttab[partnbr].diffval = diffval;
        sorttab[partnbr].edlosum = edloval;
        partnbr ++;
endloop1 : ;
      }

      if (mappflag == 1)
        for (partnum = 0; partnum < partnbr; partnum ++)
#ifdef KGRAPHDIFFMAPPMORE
          sorttab[partnum].diffval *= sorttab[partnum].distval;
#else /* KGRAPHDIFFMAPPLESS */
          sorttab[partnum].diffval /= sorttab[partnum].distval;
#endif /* KGRAPHDIFFMAPPMORE */

      if (partnbr > 1)                            /* If not interior vertex           */
        kgraphMapDfSort (sorttab, partnbr);       /* Sort array by descending amounts */

      soplval = 0;
      if (parotax != NULL) {
        for (partnum = 0; partnum < partnbr; partnum ++) {
          if (sorttab[partnum].partval == parotax[vertnum]) {
            soplval = sorttab[partnum].edlosum;
            break;
          }
        }
      }

      sfplval = 0;
      dfplval = 0;
      if (mappflag == 1) {                        /* We are doing a mapping */
        for (partnum = 1; partnum < partnbr; partnum ++) {
          sfplval += sorttab[partnum].edlosum;
#ifdef KGRAPHDIFFMAPPMORE
          dfplval += sorttab[partnum].edlosum * sorttab[partnum].distval;
#else /* KGRAPHDIFFMAPPLESS */
          dfplval += sorttab[partnum].edlosum / sorttab[partnum].distval;
#endif /* KGRAPHDIFFMAPPMORE */
        }
      }

      difntax[vertnum].partval = sorttab[0].partval; /* New part is part of most abundant liquid */

      diffval = sorttab[0].diffval;               /* Get amount of most abundant liquid    */

      if (velotax != NULL)                        /* Account for capacity of barrel        */
        veloval = (float) velotax[vertnum];
      diffval -= veloval;                         /* Leak liquid from barrel               */
      if (diffval <= 0.0F)                        /* Amount of liquid cannot be negative   */
        diffval = 0.0F;
      migrval = ((soplval == 0) || (soplval == velstax[vertnum])) ? 0.0F : (float) grafptr->r.cmloval * ((grafptr->r.vmlotax != NULL) ? (float) grafptr->r.vmlotax[vertnum] : 1.0F);
      if (migrval > diffval) {
        migrval = diffval;
        diffval = 0;
      }
      else
        diffval -= migrval;
      diffval = diffval / ((float) velstax[vertnum] * crloval);
      if (isnan (diffval)) {                      /* If overflow occured */
#ifdef SCOTCH_DEBUG_KGRAPH2
        errorPrintW ("kgraphMapDfLoop: overflow (1)");
#endif /* SCOTCH_DEBUG_KGRAPH2 */
        loopptr->abrtval = 1;                     /* Threads need to halt              */
        goto fail;                                /* Skip computations but synchronize */
      }

      if (parotax != NULL) {
        if (migrval == 0.0F) {
          difntax[vertnum].mdisval =
          difntax[vertnum].mdidval = 0.0F;
        }
        else {
          if (parotax[vertnum] == sorttab[0].partval) {
            difntax[vertnum].mdisval = migrval / (float) soplval;
            difntax[vertnum].mdidval = 0.0F;
          }
          else {
            difntax[vertnum].mdisval = 0.0F;
            difntax[vertnum].mdidval = migrval / (float) (velstax[vertnum] - soplval);
          }
        }
      }

      difntax[vertnum].diffval = diffval;
      if (dfplval != 0)
        difntax[vertnum].fdifval = diffval * sfplval / dfplval;
      else
        difntax[vertnum].fdifval = 0;
    }

    for (domnnum = domnbas, vertnum = vancnnd + domnbas; /* For all the subset of anchor vertices */
         domnnum < domnnnd; domnnum ++, vertnum ++) {
      Gnum                edgenum;
      Gnum                edgennd;
      Anum                partnbr;                /* Number of active parts */
      float               diffval;

      partnbr = 1;                                /* Keep vertex in first place to preserve its part */
      sorttab[0].partval = domnnum;               /* Always keep initial part value                  */
      sorttab[0].diffval = 0.0F;                  /* Assume at first it is null                      */

      edgenum = verttax[vertnum];
      edgennd = vendtax[vertnum];
      if (edgenum == edgennd)                     /* If isolated anchor */
        continue;                                 /* Barrel is empty    */

      for ( ; edgenum < edgennd; edgenum ++) {    /* For all edges except anchors */
        Gnum                vertend;
        float               diffval;
        Anum                partval;
        Anum                partnum;

        vertend = edgetax[edgenum];

        partval = difotax[vertend].partval;
        diffval = difotax[vertend].diffval;       /* Value is not yet scaled with respect to diffusion coefficient */

        diffval *= (float) ((edlotax != NULL) ? edlotax[edgenum] : 1);
        diffval *= crloval;

        for (partnum = 0; partnum < partnbr; partnum ++) {
          if (sorttab[partnum].partval == partval) {
            sorttab[partnum].diffval += diffval;  /* Accumulate contribution in slot     */
            goto endloop2;                        /* Do not consider creating a new slot */
          }
        }
        sorttab[partnbr].partval = partval;       /* Create new slot */
        sorttab[partnbr].diffval = diffval;
        partnbr ++;
endloop2 : ;
      }

      if (partnbr > 1)                            /* If not interior vertex           */
        kgraphMapDfSort (sorttab, partnbr);       /* Sort array by descending amounts */

      diffval = sorttab[0].diffval;               /* Get amount of most abundant liquid */

      if (sorttab[0].partval != domnnum)          /* Add liquid from tap to barrel */
        diffval = vanctab[domnnum] - diffval;
      else
        diffval += vanctab[domnnum];

      diffval = (diffval - valotab[domnnum]) / ((float) velstax[vertnum] * crloval); /* Add input and leak liquid from barrel */

      if (diffval <= 0.0F)                        /* Amount of liquid cannot be negative   */
        diffval = 0.0F;
      if (isnan (diffval)) {                      /* If overflow occured */
#ifdef SCOTCH_DEBUG_KGRAPH2
        errorPrintW ("kgraphMapDfLoop: overflow (2)");
#endif /* SCOTCH_DEBUG_KGRAPH2 */
        loopptr->abrtval = 1;                     /* Threads need to halt              */
        goto fail;                                /* Skip computations but synchronize */
      }

      difntax[vertnum].partval = domnnum;         /* Anchor part is always domain part */
      difntax[vertnum].diffval = diffval;
    }

    difttax = (KgraphMapDfVertex *) difntax;      /* Swap old and new diffusion arrays          */
    difntax = (KgraphMapDfVertex *) difotax;      /* Casts to prevent IBM compiler from yelling */
    difotax = (KgraphMapDfVertex *) difttax;
fail: ;                                           /* If overflow occured, resume here */
#ifndef KGRAPHMAPDFNOTHREAD
    threadBarrier (descptr);
#endif /* KGRAPHMAPDFNOTHREAD */

    if (loopptr->abrtval == 1)                    /* If all threads need to abort */
      break;
  }

  if (loopptr->abrtval == 0) {
    for (vertnum = vertbas; vertnum < vertnnd; vertnum ++) /* Set new part distribution of local vertices */
      parttax[vertnum] = difntax[vertnum].partval;
  }

  if (sorttab != NULL)
    memFree (sorttab);                            /* Free local array */
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine computes a k-way partition
** by diffusion across what is assumed
** to be a k-way band graph.
** It returns:
** - 0   : if the k-partition could be computed.
** - !0  : on error.
*/

int
kgraphMapDf (
Kgraph * restrict const        grafptr,           /*+ Active graph      +*/
const KgraphMapDfParam * const paraptr)           /*+ Method parameters +*/
{
  KgraphMapDfData     loopdat;

  const Gnum                domnnbr = grafptr->m.domnnbr;
  const Gnum                vertnbr = grafptr->s.vertnbr;

#ifdef SCOTCH_DEBUG_KGRAPH1
  if ((grafptr->s.flagval & KGRAPHHASANCHORS) == 0) { /* Method valid only if graph has anchors */
    errorPrint ("kgraphMapDf: graph does not have anchors");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH1 */

  if (memAllocGroup ((void **) (void *)
                     &loopdat.vanctab, (size_t) (domnnbr * sizeof (float)),
                     &loopdat.valotab, (size_t) (domnnbr * sizeof (Gnum)),
                     &loopdat.velstax, (size_t) (vertnbr * sizeof (Gnum)),
                     &loopdat.difntax, (size_t) (vertnbr * sizeof (KgraphMapDfVertex)),
                     &loopdat.difotax, (size_t) (vertnbr * sizeof (KgraphMapDfVertex)), NULL) == NULL) {
    errorPrint ("kgraphMapDf: out of memory");
    return     (1);
  }
  loopdat.grafptr  = grafptr;
  loopdat.velstax -= grafptr->s.baseval;
  loopdat.difntax -= grafptr->s.baseval;
  loopdat.difotax -= grafptr->s.baseval;
  loopdat.passnbr  = paraptr->passnbr;

  loopdat.abrtval = 0;                            /* No one wants to abort yet */

#ifndef KGRAPHMAPDFNOTHREAD
  contextThreadLaunch (grafptr->contptr, (ThreadFunc) kgraphMapDfLoop, (void *) &loopdat);
#else /* KGRAPHMAPDFNOTHREAD */
  kgraphMapDfLoop (NULL, &loopdat);
#endif /* KGRAPHMAPDFNOTHREAD */

  memFree (loopdat.vanctab);                      /* Free group leader */

  kgraphFron (grafptr);
  kgraphCost (grafptr);

#ifdef SCOTCH_DEBUG_KGRAPH2
  if (kgraphCheck (grafptr) != 0) {
    errorPrint ("kgraphMapDf: internal error");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
  return (0);
}
