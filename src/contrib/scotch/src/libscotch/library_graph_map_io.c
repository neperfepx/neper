/* Copyright 2011,2012,2015,2018,2019 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_map_io.c                  **/
/**                                                        **/
/**   AUTHOR     : Sebastien FOURESTIER (v6.0)             **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the API mappings    **/
/**                of the libSCOTCH library.               **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 16 apr 2011     **/
/**                                 to   : 26 oct 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "library_mapping.h"
#include "library_graph_map_io.h"
#include "scotch.h"

/*************************************/
/*                                   */
/* These routines are the C API for  */
/* the API mapping handling          */
/* routines.                         */
/*                                   */
/*************************************/

/*+ This routine loads the contents of the
*** given mapping array from the given stream.
*** It returns:
*** - 0   : on success.
*** - !0  : on error.
+*/

int
SCOTCH_graphTabLoad (
const SCOTCH_Graph * const    actgrafptr,         /*+ Graph to map  +*/
SCOTCH_Num * const            parttab,            /*+ Array to load +*/
FILE * const                  stream)             /*+ Input stream  +*/
{
  Gnum                  vertsortnbr;              /* Number of vertex sort slots to allocate  */
  VertSort *            vertsorttab;              /* Pointer to graph sorting array           */
  VertSort *            mappsorttab;              /* Pointer to mapping data sorting array    */
  int                   mappsortflag;             /* Flag set if mapping data sorted by label */
  Gnum                  mappsortlabl;             /* Old label value                          */
  Gnum                  mappsortval;
  Gnum                  mappfileval;
  Gnum                  mappfilenbr;              /* Number of mapping pairs in file          */
  Gnum                  mappfilenum;              /* Counter of mapping pairs in file         */
  Graph *               grafptr;
  Gnum                  vertnbr;

  grafptr = (Graph *) actgrafptr;
  vertnbr = grafptr->vertnbr;
  memSet (parttab, ~0, vertnbr * sizeof (Anum));  /* Pre-initialize the partition array */

  if ((fscanf (stream, GNUMSTRING, &mappfileval) != 1) || /* Read number of mapping pairs */
      (mappfileval < 1)) {
    errorPrint (STRINGIFY (SCOTCH_graphTabLoad) ": bad input (1)");
    return     (1);
  }
  mappfilenbr = (Gnum) mappfileval;

  if (mappfilenbr <= 0)                           /* If nothing to do */
    return (0);

  vertsortnbr = (grafptr->vlbltax != NULL) ? vertnbr : 0;
  if (memAllocGroup ((void **) (void *)           /* Allocate temporary data */
                     &mappsorttab, (size_t) (mappfilenbr * sizeof (VertSort)),
                     &vertsorttab, (size_t) (vertsortnbr * sizeof (VertSort)), NULL) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_graphTabLoad) ": out of memory (1)");
    return     (1);
  }

  mappsortflag = 1;                               /* Assume mapping data sorted */
  mappsortlabl = GNUMMIN;
  for (mappfilenum = 0; mappfilenum < mappfilenbr; mappfilenum ++) {
    if (fscanf (stream, GNUMSTRING GNUMSTRING,
                &mappsortval,
                &mappfileval) != 2) {
      errorPrint (STRINGIFY (SCOTCH_graphTabLoad) ": bad input (2)");
      memFree    (mappsorttab);                   /* Free group leader */
      return     (1);
    }
    mappsorttab[mappfilenum].labl = mappsortval;
    mappsorttab[mappfilenum].num  = mappfileval;

    if (mappsortval < mappsortlabl)               /* Check if mapping data sorted */
      mappsortflag = 0;                           /* Mapping data not sorted      */
    mappsortlabl = mappsortval;
  }
  if (mappsortflag != 1)                          /* If mapping data not sorted    */
    intSort2asc1 (mappsorttab, mappfilenbr);      /* Sort area by ascending labels */

  for (mappfilenum = 1; mappfilenum < mappfilenbr; mappfilenum ++) { /* Check mapping data integrity */
    if (mappsorttab[mappfilenum].labl == mappsorttab[mappfilenum - 1].labl) {
      errorPrint (STRINGIFY (SCOTCH_graphTabLoad) ": duplicate vertex label");
      memFree    (mappsorttab);                   /* Free group leader */
      return     (1);
    }
  }

  if (grafptr->vlbltax != NULL) {                 /* If graph has vertex labels             */
    int                   vertsortflag;           /* Flag set if graph data sorted by label */
    Gnum                  vertsortlabl;           /* Old label value                        */
    Gnum                  vertsortnum;

    vertsortflag = 1;                             /* Assume graph data sorted */
    vertsortlabl = GNUMMIN;
    for (vertsortnum = 0; vertsortnum < vertnbr; vertsortnum ++) {
      vertsorttab[vertsortnum].labl = grafptr->vlbltax[vertsortnum];
      vertsorttab[vertsortnum].num  = vertsortnum;

      if (vertsorttab[vertsortnum].labl < vertsortlabl) /* Check if graph data sorted */
        vertsortflag = 0;                         /* Graph data not sorted            */
      vertsortlabl = vertsorttab[vertsortnum].labl;
    }
    if (vertsortflag != 1)                        /* If graph label data not sorted     */
      intSort2asc1 (vertsorttab, vertnbr);        /* Sort sort area by ascending labels */

    for (vertsortnum = 0, mappfilenum = 0; vertsortnum < vertnbr; vertsortnum ++) { /* For all vertices in graph */
      while (mappsorttab[mappfilenum].labl < vertsorttab[vertsortnum].labl) {
        mappfilenum ++;                           /* Search mapping vertex with same label */
        if (mappfilenum >= mappfilenbr)           /* If all mapping data processed         */
          goto end;                               /* Exit main loop                        */
      }
      if (mappsorttab[mappfilenum].labl == vertsorttab[vertsortnum].labl) /* If label match found */
        ((Anum *) parttab)[vertsorttab[vertsortnum].num] = mappsorttab[mappfilenum ++].num;
    }
end: ;
  }
  else {                                          /* Graph does not have vertex labels */
    const Gnum          baseval = grafptr->baseval;

    for (mappfilenum = 0; mappfilenum < mappfilenbr; mappfilenum ++) { /* For all mapping pairs     */
      Gnum                vertnum;                /* Vertex index to which mapping label is applied */

      vertnum = mappsorttab[mappfilenum].labl - baseval;
      if ((vertnum >= 0) && (vertnum < vertnbr))  /* If valid vertex label */
        ((Anum *) parttab)[vertnum] = (Anum) mappsorttab[mappfilenum].num;
    }
  }

  memFree (mappsorttab);                          /* Free group leader */

  return (0);
}

/*+ This routine saves the contents of the
*** given mapping array to the given stream.
*** It returns:
*** - 0   : on success.
*** - !0  : on error.
+*/

int
SCOTCH_graphTabSave (
const SCOTCH_Graph * const  libgrafptr,           /*+ Graph to map  +*/
const SCOTCH_Num * const    parttab,              /*+ Array to save +*/
FILE * const                stream)               /*+ Input stream  +*/
{
  Gnum                vertnum;

  const Graph * restrict const  grafptr = (Graph *) libgrafptr;
  const Gnum * restrict const   vlbltax = grafptr->vlbltax;
  const Gnum * restrict const   parttax = parttab - grafptr->baseval;

  if (fprintf (stream, GNUMSTRING "\n", (Gnum) grafptr->vertnbr) == EOF) {
    errorPrint (STRINGIFY (SCOTCH_graphTabSave) ": bad output (1)");
    return (1);
  }

  for (vertnum = grafptr->baseval; vertnum < grafptr->vertnnd; vertnum ++) {
    if (fprintf (stream, GNUMSTRING "\t" GNUMSTRING "\n",
                 (Gnum) ((vlbltax != NULL) ? vlbltax[vertnum] : vertnum),
                 (Gnum) parttax[vertnum]) == EOF) {
      errorPrint (STRINGIFY (SCOTCH_graphTabSave) ": bad output (2)");
      return (1);
    }
  }

  return (0);
}

/*+ This routine loads the contents of
*** the given user mapping from the
*** given stream.
*** It returns:
*** - 0   : on success.
*** - !0  : on error.
+*/

int
SCOTCH_graphMapLoad (
const SCOTCH_Graph * const  actgrafptr,           /*+ Graph to map    +*/
SCOTCH_Mapping * const      mappptr,              /*+ Mapping to save +*/
FILE * const                stream)               /*+ Output stream   +*/
{
  const Graph * restrict const  grafptr = (Graph *) actgrafptr;
  LibMapping * restrict         lmapptr = (LibMapping *) mappptr;
#ifdef SCOTCH_DEBUG_GRAPH2
  if (grafptr != lmapptr->grafptr) {
    errorPrint (STRINGIFY (SCOTCH_graphMapLoad) ": mapping structure must derive from graph");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_GRAPH2 */

  if (lmapptr->parttab == NULL) {                 /* Allocate array if necessary */
    if ((lmapptr->parttab = (Gnum *) memAlloc (grafptr->vertnbr * sizeof (Gnum))) == NULL) {
      errorPrint (STRINGIFY (SCOTCH_graphMapLoad) ": out of memory");
      return     (1);
    }
    lmapptr->flagval |= LIBMAPPINGFREEPART;       /* As the array was not allocated by the user, it will be freed */
  }

  return (SCOTCH_graphTabLoad (actgrafptr, (SCOTCH_Num *) lmapptr->parttab, stream));
}

/*+ This routine saves the contents of
*** the given user mapping to the given
*** stream.
*** It returns:
*** - 0   : on success.
*** - !0  : on error.
+*/

int
SCOTCH_graphMapSave (
const SCOTCH_Graph * const    actgrafptr,         /*+ Graph to map    +*/
const SCOTCH_Mapping * const  mappptr,            /*+ Mapping to save +*/
FILE * const                  stream)             /*+ Output stream   +*/
{
#ifdef SCOTCH_DEBUG_GRAPH2
  const Graph * restrict const  grafptr = (Graph *) actgrafptr;
  if (grafptr != (((LibMapping *) mappptr)->grafptr)) {
    errorPrint (STRINGIFY (SCOTCH_graphMapSave) ": mapping structure must derive from graph");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_GRAPH2 */

  return (SCOTCH_graphTabSave (actgrafptr, (SCOTCH_Num *) ((LibMapping *) mappptr)->parttab, stream));
}
