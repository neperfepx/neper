/* Copyright 2004,2007,2008,2011,2015,2018,2019,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_map_view.c                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the mapping  **/
/**                routines of the libSCOTCH library.      **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 19 aug 1998     **/
/**                                 to   : 20 aug 1998     **/
/**                # Version 3.3  : from : 19 oct 1998     **/
/**                                 to   : 30 mar 1999     **/
/**                # Version 3.4  : from : 01 nov 2001     **/
/**                                 to   : 01 nov 2001     **/
/**                # Version 4.0  : from : 13 jan 2004     **/
/**                                 to   : 30 nov 2006     **/
/**                # Version 5.0  : from : 04 feb 2007     **/
/**                                 to   : 03 apr 2008     **/
/**                # Version 5.1  : from : 27 jul 2008     **/
/**                                 to   : 11 aug 2010     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 24 sep 2019     **/
/**                # Version 6.1  : from : 01 jul 2021     **/
/**                                 to   : 01 jul 2021     **/
/**                # Version 7.0  : from : 07 may 2019     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "kgraph.h"
#include "library_mapping.h"
#include "library_graph_map_view.h"
#include "scotch.h"

/************************************/
/*                                  */
/* These routines are the C API for */
/* the mapping routines.            */
/*                                  */
/************************************/

/* This routine computes the pseudo-diameter of
** the given part.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
Gnum
graphMapView3 (
const Graph * const         grafptr,              /*+ Graph              +*/
const Anum * const          parttax,              /*+ Part (index) array +*/
const Anum                  partval)              /*+ Part (index) value +*/
{
  GraphMapViewQueue             queudat;          /* Neighbor queue                         */
  GraphMapViewVertex * restrict vexxtax;          /* Based access to vexxtab                */
  Gnum                          rootnum;          /* Number of current root vertex          */
  Gnum                          distval;          /* Vertex distance                        */
  Gnum                          distmax;          /* Current diameter distance              */
  int                           flagval;          /* Flag set if diameter changed           */
  Gnum                          connval;          /* Base distance for connected components */
  Gnum                          passnum;          /* Pass number                            */
  const Gnum * restrict         verttax;          /* Based access to vertex array           */
  const Gnum * restrict         vendtax;          /* Based access to vertex end array       */
  const Gnum * restrict         edgetax;

  if (memAllocGroup ((void **) (void *)
                     &queudat.qtab, (size_t) (grafptr->vertnbr * sizeof (Gnum)),
                     &vexxtax,      (size_t) (grafptr->vertnbr * sizeof (GraphMapViewVertex)), NULL) == NULL) {
    errorPrint ("graphMapView3: out of memory");
    return (-1);
  }

  memSet (vexxtax, 0, grafptr->vertnbr * sizeof (GraphMapViewVertex)); /* Initialize pass numbers */
  edgetax  = grafptr->edgetax;
  verttax  = grafptr->verttax;
  vendtax  = grafptr->vendtax;
  vexxtax -= grafptr->baseval;

  distmax = 0;                                    /* Start distances from zero                  */
  for (passnum = 1, rootnum = grafptr->baseval; ; passnum ++) { /* For all connected components */
    Gnum                diamnum;                  /* Vertex which achieves diameter             */

    while ((rootnum < grafptr->vertnbr) &&
           ((vexxtax[rootnum].passnum != 0) ||    /* Find first unallocated vertex */
            (parttax[rootnum] != partval)))
      rootnum ++;
    if (rootnum >= grafptr->vertnbr)              /* Exit if all of graph processed */
      break;

    connval = ++ distmax;                         /* Start from previous distance */
    diamnum = rootnum;                            /* Start from found root        */

    for (flagval = 1; flagval -- != 0; passnum ++) { /* Loop if modifications   */
      graphMapViewQueueFlush (&queudat);          /* Flush vertex queue         */
      graphMapViewQueuePut   (&queudat, diamnum); /* Start from diameter vertex */
      vexxtax[diamnum].passnum = passnum;         /* It has been enqueued       */
      vexxtax[diamnum].distval = connval;         /* It is at base distance     */

      do {                                        /* Loop on vertices in queue */
        Gnum                vertnum;
        Gnum                edgenum;

        vertnum = graphMapViewQueueGet (&queudat); /* Get vertex from queue */
        distval = vexxtax[vertnum].distval;       /* Get vertex distance    */

        if ((distval > distmax) ||                /* If vertex increases diameter */
            ((distval == distmax) &&              /* Or is at diameter distance   */
             ((vendtax[vertnum] - verttax[vertnum]) < /* With smaller degree      */
              (vendtax[diamnum] - verttax[diamnum])))) {
          diamnum = vertnum;                      /* Set it as new diameter vertex */
          distmax = distval;
          flagval = 1;
        }

        distval ++;                               /* Set neighbor distance */
        for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
          Gnum                vertend;

          vertend = edgetax[edgenum];
          if ((vexxtax[vertend].passnum < passnum) && /* If vertex not queued yet */
              (parttax[vertend] == partval)) {    /* And of proper part           */
            graphMapViewQueuePut (&queudat, vertend); /* Enqueue neighbor vertex  */
            vexxtax[vertend].passnum = passnum;
            vexxtax[vertend].distval = distval;
          }
        }
      } while (! graphMapViewQueueEmpty (&queudat)); /* As long as queue is not empty */
    }
  }

  memFree (queudat.qtab);                         /* Free group leader */

  return (distmax);
}

/* This routine writes standard or raw
** mapping or remapping statistics to
** the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
int
graphMapView2 (
const SCOTCH_Graph * const    libgrafptr,         /*+ Ordered graph                                    +*/
const SCOTCH_Mapping * const  libmappptr,         /*+ Computed mapping                                 +*/
const SCOTCH_Mapping * const  libmapoptr,         /*+ Old mapping (equal to NULL if no repartitioning) +*/
const double                  emraval,            /*+ Edge migration ratio                             +*/
SCOTCH_Num *                  vmlotab,            /*+ Vertex migration cost array                      +*/
Gnum                          flagval,            /*+ 0: standard output, !0: raw output for curves    +*/
FILE * const                  stream)             /*+ Output stream                                    +*/
{
  const Arch * restrict     archptr;
  LibMapping * restrict     lmaoptr;
  Anum * restrict           parttax;              /* Part array                                   */
  Anum * restrict           parotax;              /* Old part array                               */
  Anum * restrict           pidxtax;              /* Part index array                             */
  MappingSort * restrict    domntab;              /* Pointer to domain sort array                 */
  ArchDom                   domnfrst;             /* Largest domain in architecture               */
  ArchDom                   domnorg;              /* Vertex domain                                */
  ArchDom                   domnend;              /* End domain                                   */
  ArchDom                   domnold;              /* Vertex old domain                            */
  Anum                      tgtnbr;               /* Number of processors in target topology      */
  Anum                      mapnbr;               /* Number of processors effectively used        */
  double                    mapavg;               /* Average mapping weight                       */
  Gnum                      mapmin;
  Gnum                      mapmax;
  Gnum                      mapsum;               /* (Partial) sum of vertex loads                */
  double                    mapdlt;
  double                    mapmmy;               /* Maximum / average ratio                      */
  Anum * restrict           nghbtab;              /* Table storing neighbors of current subdomain */
  Anum                      nghbnbr;
  Anum                      nghbmin;
  Anum                      nghbmax;
  Anum                      nghbsum;
  Gnum                      vertnum;
  Gnum                      vertidx;
  Gnum                      veloval;
  Gnum                      edloval;
  Gnum                      commdist[256];        /* Array of load distribution */
  Gnum                      commload;             /* Total edge load (edge sum) */
  Gnum                      commdilat;            /* Total edge dilation        */
  Gnum                      commexpan;            /* Total edge expansion       */
  Anum                      distmax;
  Anum                      distval;
  Gnum                      diammin;
  Gnum                      diammax;
  Gnum                      diamsum;
  double                    diamavg;
  Gnum                      migrnbr;
  double                    migrloadavg;
  double                    migrdistavg;
  double                    migrcostsum;
  Gnum * restrict           vmlotax;

  const Graph * restrict const  grafptr = (Graph *) CONTEXTOBJECT (libgrafptr);
  LibMapping * restrict const   lmapptr = (LibMapping *) libmappptr;
  const Gnum * restrict const   verttax = grafptr->verttax;
  const Gnum * restrict const   vendtax = grafptr->vendtax;
  const Gnum * restrict const   velotax = grafptr->velotax;
  const Gnum * restrict const   edgetax = grafptr->edgetax;
  const Gnum * restrict const   edlotax = grafptr->edlotax;

#ifdef SCOTCH_DEBUG_LIBRARY1
  if (sizeof (SCOTCH_Mapping) < sizeof (LibMapping)) {
    errorPrint (STRINGIFY (SCOTCH_graphMapView) ": internal error");
    return (1);
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */

#ifdef SCOTCH_DEBUG_LIBRARY1
  if (lmapptr->grafptr != grafptr) {
    errorPrint (STRINGIFY (SCOTCH_graphMapView) ": input graph must be the same as mapping graph");
    return (1);
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */

  if ((grafptr->vertnbr == 0) ||                  /* Return if nothing to do */
      (grafptr->edgenbr == 0))
    return (0);

  if (libmapoptr != NULL) {
    lmaoptr = (LibMapping *) libmapoptr;
    parotax = lmaoptr->parttab;
  }
  else {
    lmaoptr = NULL;
    parotax = NULL;
  }

  if (vmlotab != NULL)
    vmlotax = (Gnum *) vmlotab - grafptr->baseval;
  else
    vmlotax = NULL;

#ifdef SCOTCH_DEBUG_LIBRARY1
  if (lmapptr->parttab == NULL) {
    errorPrint (STRINGIFY (SCOTCH_graphMapView) ": the mapping given in input must contain a valid partition array");
    return (1);
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */
  archptr = lmapptr->archptr;
  parttax = lmapptr->parttab;                     /* Use un-based array */

  if (memAllocGroup ((void **) (void *)
                     &domntab, (size_t) ((grafptr->vertnbr + 1) * sizeof (MappingSort)),
                     &nghbtab, (size_t) ((grafptr->vertnbr + 2) * sizeof (Anum)),
                     &pidxtax, (size_t) ( grafptr->vertnbr      * sizeof (Anum)), NULL) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_graphMapView) ": out of memory");
    return (1);
  }
  pidxtax -= grafptr->baseval;

  for (vertnum = 0; vertnum < grafptr->vertnbr; vertnum ++) {
    domntab[vertnum].labl = parttax[vertnum];     /* Un-based array at this stage    */
    domntab[vertnum].peri = vertnum + grafptr->baseval; /* Build inverse permutation */
  }
  domntab[grafptr->vertnbr].labl = ARCHDOMNOTTERM; /* TRICK: avoid testing (i+1)   */
  domntab[grafptr->vertnbr].peri = ~0;            /* Prevent Valgrind from yelling */

  intSort2asc2 (domntab, grafptr->vertnbr);       /* Sort domain label array by increasing target labels */

  archDomFrst (archptr, &domnfrst);               /* Get architecture domain */
  tgtnbr = archDomSize (archptr, &domnfrst);      /* Get architecture size   */

  mapnbr  = 0;
  mapsum  = 0;
  veloval = 1;                                    /* Assume unweighted vertices */
  for (vertidx = 0; domntab[vertidx].labl != ARCHDOMNOTTERM; vertidx ++) {
    pidxtax[domntab[vertidx].peri] = mapnbr;      /* Build map of partition parts starting from 0  */
    if (domntab[vertidx].labl != domntab[vertidx + 1].labl) /* TRICK: if new (or end) domain label */
      mapnbr ++;                                  /* Create new terminal part index                */
    if (velotax != NULL)
      veloval = velotax[domntab[vertidx].peri];
    mapsum += veloval;
  }
  mapavg = (mapnbr == 0) ? 0.0L : (double) mapsum / (double) mapnbr;

  mapsum = 0;
  mapmax = 0;
  mapdlt = 0.0L;
  if (mapnbr > 0) {
    mapmin = GNUMMAX;

    for (vertidx = 0; domntab[vertidx].labl != ARCHDOMNOTTERM; vertidx ++) {
      if (velotax != NULL)
        veloval = velotax[domntab[vertidx].peri];
      mapsum += veloval;

      if (domntab[vertidx].labl != domntab[vertidx + 1].labl) { /* TRICK: if new (or end) domain label */
        if (mapsum < mapmin)
          mapmin = mapsum;
        if (mapsum > mapmax)
          mapmax = mapsum;
        mapdlt += fabs ((double) mapsum - mapavg);
        mapsum = 0;                               /* Reset domain load sum */
      }
    }

    mapdlt = mapdlt / ((double) mapnbr * mapavg);
    mapmmy = (double) mapmax / (double) mapavg;
  }
  else {
    mapmin = 0;
    mapmmy = 0;
  }

  if (mapnbr > tgtnbr) {                          /* If more subdomains than architecture size */
#ifdef SCOTCH_DEBUG_MAP2
    if (! archVar (archptr)) {                    /* If not a variable-sized architecture */
      errorPrint (STRINGIFY (SCOTCH_graphMapView) ": invalid mapping");
      memFree    (domntab);                       /* Free group leader */
      return (1);
    }
#endif /* SCOTCH_DEBUG_MAP2 */
    tgtnbr = mapnbr;                              /* Assume it is a variable-sized architecture */
  }

  if (flagval == 0) {
    fprintf (stream, "M\tProcessors " GNUMSTRING "/" GNUMSTRING " (%g)\n",
             (Gnum) mapnbr,
             (Gnum) tgtnbr,
             (double) mapnbr / (double) tgtnbr);
    fprintf (stream, "M\tTarget min=" GNUMSTRING "\tmax=" GNUMSTRING "\tavg=%g\tdlt=%g\tmaxavg=%g\n",
             (Gnum) mapmin,
             (Gnum) mapmax,
             mapavg,
             mapdlt,
             mapmmy);
  }

  nghbnbr = 0;
  nghbmax = 0;
  nghbsum = 0;
  nghbtab[0] = -2;
  if (mapnbr > 0) {
    nghbmin = ANUMMAX;

    for (vertidx = 0; domntab[vertidx].labl != ARCHDOMNOTTERM; vertidx ++) {
      Gnum                edgenum;
      Gnum                edgennd;
      Anum                pidxnum;

      pidxnum = pidxtax[domntab[vertidx].peri];
      for (edgenum = verttax[domntab[vertidx].peri],
           edgennd = vendtax[domntab[vertidx].peri];
           edgenum < edgennd; edgenum ++) {
        Anum                pidxend;

        pidxend = pidxtax[edgetax[edgenum]];
        if ((pidxend != pidxnum) &&               /* If edge is not internal                                      */
            (pidxend != nghbtab[nghbnbr])) {      /* And neighbor is not sole neighbor or has not just been found */
          Anum                pidxmin;
          Anum                pidxmax;

          pidxmin = 0;
          pidxmax = nghbnbr;
          while ((pidxmax - pidxmin) > 1) {
            Anum                pidxmed;

            pidxmed = (pidxmax + pidxmin) >> 1;
            if (nghbtab[pidxmed] > pidxend)
              pidxmax = pidxmed;
            else
              pidxmin = pidxmed;
          }
          if (nghbtab[pidxmin] == pidxend)        /* If neighboring part found, skip to next neighbor */
            continue;

#ifdef SCOTCH_DEBUG_MAP2
          if (nghbnbr >= (grafptr->vertnbr + 1)) {
            errorPrint (STRINGIFY (SCOTCH_graphMapView) ": internal error");
            return (1);
          }
#endif /* SCOTCH_DEBUG_MAP2 */

          nghbnbr ++;
          for (pidxmax = nghbnbr; pidxmax > (pidxmin + 1); pidxmax --)
            nghbtab[pidxmax] = nghbtab[pidxmax - 1];
          nghbtab[pidxmin + 1] = pidxend;         /* Add new neighbor part in the right place */
        }
      }
      if (domntab[vertidx].labl != domntab[vertidx + 1].labl) { /* TRICK: if new (or end) domain label */
        if (nghbnbr < nghbmin)
          nghbmin = nghbnbr;
        if (nghbnbr > nghbmax)
          nghbmax = nghbnbr;
        nghbsum += nghbnbr;

        nghbnbr = 0;
      }
    }
  }
  else
    nghbmin = 0;

  if (flagval == 0) {
    fprintf (stream, "M\tNeighbors min=" GNUMSTRING "\tmax=" GNUMSTRING "\tsum=" GNUMSTRING "\n",
             (Gnum) nghbmin,
             (Gnum) nghbmax,
             (Gnum) nghbsum);
  }

  memSet (commdist, 0, 256 * sizeof (Gnum));      /* Initialize the data */
  commload  =
  commdilat =
  commexpan = 0;

  parttax -= grafptr->baseval;
  edloval  = 1;
  for (vertnum = grafptr->baseval; vertnum < grafptr->vertnnd; vertnum ++) {
    Gnum                edgenum;

    if (parttax[vertnum] == ~0)                   /* Skip unmapped vertices */
      continue;
    for (edgenum = verttax[vertnum];
         edgenum < vendtax[vertnum]; edgenum ++) {
      if (parttax[edgetax[edgenum]] == ~0)
        continue;
      archDomTerm (archptr, &domnorg, parttax[vertnum]); /* Get terminal domains */
      archDomTerm (archptr, &domnend, parttax[edgetax[edgenum]]);
      distval = archDomDist (archptr, &domnorg, &domnend);
      if (edlotax != NULL)                        /* Get edge weight if any */
        edloval = edlotax[edgenum];
      commdist[(distval > 255) ? 255 : distval] += edloval;
      commload  += edloval;
      commdilat += distval;
      commexpan += distval * edloval;
    }
  }

  migrnbr = 0;                                    /* Initialize variables unconditionally to avoid compiler warnings */
  migrdistavg = 0;
  migrloadavg = 0;
  migrcostsum = 0;
  if (lmaoptr != NULL) {
    for (vertnum = grafptr->baseval; vertnum < grafptr->vertnnd; vertnum ++) {
      if ((parttax[vertnum] == -1) || (parotax[vertnum] == -1))
        continue;
      if (parotax[vertnum] != parttax[vertnum]) {
        migrnbr ++;
        archDomTerm (archptr, &domnorg, parotax[vertnum]); /* Get terminal domains */
        archDomTerm (archptr, &domnold, parotax[vertnum]);
        migrdistavg += archDomDist (archptr, &domnorg, &domnold);
        migrloadavg += (grafptr->velotax == NULL) ? 1 : grafptr->velotax[vertnum];
        migrcostsum += emraval * ((vmlotax != NULL) ? vmlotax[vertnum] : 1);
      }
    }
    if (migrnbr > 0) {
      migrdistavg /= migrnbr;
      migrloadavg /= migrnbr;
    }
  }

  if (flagval == 0) {
    fprintf (stream, "M\tCommDilat=%f\t(" GNUMSTRING ")\n", /* Print expansion parameters */
             (double) commdilat / grafptr->edgenbr,
             (Gnum) (commdilat / 2));
    fprintf (stream, "M\tCommExpan=%f\t(" GNUMSTRING ")\n",
             ((commload == 0) ? (double) 0.0L
                              : (double) commexpan / (double) commload),
             (Gnum) (commexpan / 2));
    fprintf (stream, "M\tCommCutSz=%f\t(" GNUMSTRING ")\n",
             ((commload == 0) ? (double) 0.0L
                              : (double) (commload - commdist[0]) / (double) commload),
             (Gnum) ((commload - commdist[0]) / 2));
    fprintf (stream, "M\tCommDelta=%f\n",
             (((double) commload  * (double) commdilat) == 0.0L)
             ? (double) 0.0L
             : ((double) commexpan * (double) grafptr->edgenbr) /
               ((double) commload  * (double) commdilat));
  }

  for (distmax = 255; distmax != -1; distmax --)  /* Find longest distance */
    if (commdist[distmax] != 0)
      break;
  if (flagval == 0) {
    for (distval = 0; distval <= distmax; distval ++) /* Print distance histogram */
      fprintf (stream, "M\tCommLoad[" ANUMSTRING "]=%f\n",
               (Anum) distval, (double) commdist[distval] / (double) commload);
  }

  diammax = 0;
  diamsum = 0;
  if (mapnbr != 0) {
    Anum                mapnum;

    diammin = GNUMMAX;
    mapnum  = 0;
    do {
      Gnum                diamval;

      diamval  = graphMapView3 (grafptr, pidxtax, mapnum);
      diamsum += diamval;
      if (diamval < diammin)
        diammin = diamval;
      if (diamval > diammax)
        diammax = diamval;
    } while (++ mapnum < mapnbr);

    diamavg = (double) diamsum / (double) mapnbr;
  }
  else {
    diammin = 0;
    diamavg = 0.0;
  }

  if (flagval == 0) {
    fprintf (stream, "M\tPartDiam\tmin=" GNUMSTRING "\tmax=" GNUMSTRING "\tavg=%lf\n",
             (Gnum) diammin,
             (Gnum) diammax,
             diamavg);
  }

  if ((flagval == 0) && (lmaoptr != NULL)) {
    fprintf (stream, "M\tMigrNbr=" GNUMSTRING "(%lf %%)\n",
             migrnbr, (((double) migrnbr) / ((double) grafptr->vertnbr)) * 100);
    fprintf (stream, "M\tAvgMigrDist=%lf\n",
             (double) migrdistavg);
    fprintf (stream, "M\tAvgMigrLoad=%lf\n",
             (double) migrloadavg);
    fprintf (stream, "M\tMigrCost=%lf\n",
             (double) migrcostsum);
  }

  if (flagval != 0) {                             /* If raw output */
    fprintf (stream, "" GNUMSTRING "\t" GNUMSTRING "\t" GNUMSTRING "\t" GNUMSTRING "\t%g\t%g\t%g\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf", /* Print standard data */
             (Gnum) mapnbr,
             (Gnum) tgtnbr,
             (Gnum) mapmin,
             (Gnum) mapmax,
             mapavg,
             mapdlt,
             mapmmy,
             (double) commload,
             (double) commexpan,
             (double) commdilat / grafptr->edgenbr,
             ((commload == 0) ? (double) 0.0L
                              : (double) commexpan / (double) commload),
             ((commload == 0) ? (double) 0.0L
                              : (double) (commload - commdist[0]) / (double) commload),
             (((double) commload  * (double) commdilat) == 0.0L)
             ? (double) 0.0L
             : ((double) commexpan * (double) grafptr->edgenbr) /
               ((double) commload  * (double) commdilat));
    if (lmaoptr != NULL)                          /* If we are doing repartitioning */
      fprintf (stream, "\t%lf\t%lf\t%lf\t%lf\t%lf", /* Print repartitioning data    */
               (double) emraval,
               (double) migrnbr / (double) grafptr->vertnbr,
               (double) migrdistavg,
               (double) migrloadavg,
               (double) migrcostsum);
    fprintf (stream, "\n");
  }

  memFree (domntab);                              /* Free group leader */

  return (0);
}

/* This routine writes mapping statistics
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphMapView (
const SCOTCH_Graph * const    libgrafptr,         /*+ Ordered graph    +*/
const SCOTCH_Mapping * const  libmappptr,         /*+ Computed mapping +*/
FILE * const                  stream)             /*+ Output stream    +*/
{
  return (graphMapView2 (libgrafptr, libmappptr, NULL, 0, NULL, 0, stream));
}

/* This routine writes remapping statistics
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphRemapView (
const SCOTCH_Graph * const    libgrafptr,         /*+ Ordered graph                                    +*/
const SCOTCH_Mapping * const  libmappptr,         /*+ Computed mapping                                 +*/
const SCOTCH_Mapping * const  libmapoptr,         /*+ Old mapping (equal to NULL if no repartitioning) +*/
const double                  emraval,            /*+ Edge migration ratio                             +*/
SCOTCH_Num *                  vmlotab,            /*+ Vertex migration cost array                      +*/
FILE * const                  stream)             /*+ Output stream                                    +*/
{
  return (graphMapView2 (libgrafptr, libmappptr, libmapoptr, emraval, vmlotab, 0, stream));
}

/* This routine writes raw mapping statistics
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphMapViewRaw (
const SCOTCH_Graph * const    libgrafptr,         /*+ Ordered graph    +*/
const SCOTCH_Mapping * const  libmappptr,         /*+ Computed mapping +*/
FILE * const                  stream)             /*+ Output stream    +*/
{
  return (graphMapView2 (libgrafptr, libmappptr, NULL, 0, NULL, 1, stream));
}

/* This routine writes raw remapping statistics
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphRemapViewRaw (
const SCOTCH_Graph * const    libgrafptr,         /*+ Ordered graph                                    +*/
const SCOTCH_Mapping * const  libmappptr,         /*+ Computed mapping                                 +*/
const SCOTCH_Mapping * const  libmapoptr,         /*+ Old mapping (equal to NULL if no repartitioning) +*/
const double                  emraval,            /*+ Edge migration ratio                             +*/
SCOTCH_Num *                  vmlotab,            /*+ Vertex migration cost array                      +*/
FILE * const                  stream)             /*+ Output stream                                    +*/
{
  return (graphMapView2 (libgrafptr, libmappptr, libmapoptr, emraval, vmlotab, 1, stream));
}

/* This routine writes the characteristics
** of the given overlap partition to the
** given stream.
** It returns :
** - void  : in case of success
** - exit  : on error (because of errorPrint)
*/

int
SCOTCH_graphPartOvlView (
const SCOTCH_Graph * restrict const libgrafptr,
const SCOTCH_Num                    partnbr,
const SCOTCH_Num * restrict const   parttab,
FILE * const                        stream)
{
  const Gnum * restrict       parttax;
  Gnum                        partnum;
  GraphMapViewList * restrict listtab;
  Gnum                        vertnum;
  Gnum                        fronload;
  Gnum * restrict             compload;
  Gnum * restrict             compsize;
  Gnum                        comploadsum;
  Gnum                        comploadmax;
  Gnum                        comploadmin;
  double                      comploadavg;

  const Graph * restrict const  grafptr = (Graph *) CONTEXTOBJECT (libgrafptr);
  const Gnum * restrict const   verttax = grafptr->verttax;
  const Gnum * restrict const   velotax = grafptr->velotax;
  const Gnum * restrict const   vendtax = grafptr->vendtax;
  const Gnum * restrict const   edgetax = grafptr->edgetax;

  if (memAllocGroup ((void **) (void *)
                     &compload, (size_t) (partnbr * sizeof (Gnum)),
                     &compsize, (size_t) (partnbr * sizeof (Gnum)),
                     &listtab,  (size_t) ((partnbr + 1) * sizeof (GraphMapViewList)), NULL) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_graphPartOvlView) ": out of memory");
  }
  listtab ++;                                     /* TRICK: Trim array so that listtab[-1] is valid */
  memSet (listtab, ~0, partnbr * sizeof (GraphMapViewList)); /* Set vertex indices to ~0            */
  memSet (compload, 0, partnbr * sizeof (Gnum));
  memSet (compsize, 0, partnbr * sizeof (Gnum));

  parttax = ((Gnum *) parttab) - grafptr->baseval;

  fronload = 0;
  for (vertnum = grafptr->baseval; vertnum < grafptr->vertnnd; vertnum ++) {
    Gnum          partval;

    partval = parttax[vertnum];
    if (partval >= 0) {
      compload[partval] += (velotax != NULL) ? velotax[vertnum] : 1;
      compsize[partval] ++;
    }
    else {                                  /* Vertex is in separator       */
      Gnum          listidx;                /* Index of first neighbor part */
      Gnum          edgenum;
      Gnum          veloval;

      veloval = (velotax != NULL) ? velotax[vertnum] : 1;

      fronload += veloval;                        /* Add vertex to frontier */

      listidx = -1;                               /* No neighboring parts recorded yet          */
      listtab[-1].vertnum = vertnum;              /* Separator neighbors will not be considered */
      for (edgenum = verttax[vertnum];
           edgenum < vendtax[vertnum]; edgenum ++) { /* Compute gain */
        Gnum          vertend;
        Gnum          partend;

        vertend = edgetax[edgenum];
        partend = parttax[vertend];
        if (listtab[partend].vertnum != vertnum) { /* If part not yet considered  */
          listtab[partend].vertnum = vertnum;     /* Link it in list of neighbors */
          listtab[partend].nextidx = listidx;
          listidx = partend;
        }
      }

      while (listidx != -1) {                     /* For all neighboring parts found      */
        compload[listidx] += veloval;             /* Add load of separator vertex to part */
        compsize[listidx] ++;
        listidx = listtab[listidx].nextidx;
      }
    }
  }

  comploadsum = 0;
  for (partnum = 0; partnum < partnbr; partnum ++)
    comploadsum += compload[partnum];

  comploadmax = 0;
  comploadmin = comploadsum;
  for (partnum = 0; partnum < partnbr; partnum ++) {
    if (compload[partnum] > comploadmax)
      comploadmax = compload[partnum];
    if (compload[partnum] < comploadmin)
      comploadmin = compload[partnum];
  }
  comploadavg = (double) comploadsum / (double) partnbr;
  fprintf (stream, "P\tsep=" GNUMSTRING "\n",
           (Gnum) fronload);
  fprintf (stream, "P\tmin=" GNUMSTRING "\tmax=" GNUMSTRING "\tavg=%g\n",
           (Gnum) comploadmin,
           (Gnum) comploadmax,
           (double) comploadavg);
#if 0 /* TODO REMOVE */
  for (partnum = 0; partnum < partnbr; partnum ++)
    fprintf (stream, "P\tload[" GNUMSTRING "]=" GNUMSTRING "\n",
             (Gnum) partnum,
             (Gnum) compload[partnum]);
#endif
  fprintf (stream, "P\tmaxavg=%g\tminavg=%g\n",
           ((double) comploadmax / comploadavg),
           ((double) comploadmin / comploadavg));

  memFree (compload);

  return (0);
}
