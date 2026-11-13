/* Copyright 2012,2014,2018,2019,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_color.c                   **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the graph    **/
/**                coloring routine of the libSCOTCH       **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 02 jan 2012     **/
/**                                 to   : 25 apr 2018     **/
/**                # Version 7.0  : from : 24 aug 2019     **/
/**                                 to   : 16 jan 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "arch.h"
#include "graph.h"
#include "graph_coarsen.h"
#include "scotch.h"

/*********************************/
/*                               */
/* This routine is the C API for */
/* the graph coloring routine.   */
/*                               */
/*********************************/

/* This routine creates a color array for the
** given graph.
** It returns:
** - 0  : if the graph has been coarsened.
** - 1  : if the graph could not be coarsened.
** - 2  : on error.
*/

int
SCOTCH_graphColor (
const SCOTCH_Graph * const  libgrafptr,           /* Graph to color              */
SCOTCH_Num * const          colotab,              /* Pointer to color array      */
SCOTCH_Num * const          coloptr,              /* Pointer to number of colors */
const SCOTCH_Num            flagval)              /* Flag value (not used)       */
{
  CONTEXTDECL        (libgrafptr);
  Gnum                baseval;
  Gnum                vertnum;
  Gnum                vertnbr;
  Gnum                vertnnd;
  Gnum                queunnd;
  Gnum * restrict     queutax;
  Gnum * restrict     randtax;
  Gnum                colonum;
  Gnum * restrict     colotax;
  int                 o;

  if (CONTEXTINIT (libgrafptr) != 0) {
    errorPrint (STRINGIFY (SCOTCH_graphColor) ": cannot initialize context");
    return (1);
  }

  const Graph * restrict const  grafptr = CONTEXTGETOBJECT (libgrafptr);
  const Gnum * restrict const   verttax = grafptr->verttax;
  const Gnum * restrict const   vendtax = grafptr->vendtax;
  const Gnum * restrict const   edgetax = grafptr->edgetax;

  baseval = grafptr->baseval;
  vertnbr = grafptr->vertnbr;
  vertnnd = vertnbr + baseval;

  memSet (colotab, ~0, vertnbr * sizeof (Gnum));
  colotax = ((Gnum *) colotab) - baseval;

  o = 1;                                          /* Assume an error */

  if (memAllocGroup ((void **) (void *)
                     &queutax, (size_t) (vertnbr * sizeof (Gnum)),
                     &randtax, (size_t) (vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_graphColor) ": out of memory");
    goto fail;
  }
  queutax -= baseval;
  randtax -= baseval;

  for (vertnum = baseval; vertnum < vertnnd; vertnum ++)
    randtax[vertnum] = contextIntRandVal (CONTEXTGETDATA (libgrafptr), 32768);

  queunnd = vertnnd;
  for (colonum = 0; queunnd > baseval; colonum ++) { /* Color numbers are not based */
    Gnum                queuold;
    Gnum                queunew;

    for (queunew = queuold = baseval; queuold < queunnd; queuold ++) {
      Gnum                vertnum;
      Gnum                edgenum;
      Gnum                edgennd;
      Gnum                randval;

      vertnum = (queunnd == vertnnd) ? queuold : queutax[queuold];
      randval = randtax[vertnum];
      for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum]; edgenum < edgennd; edgenum ++) {
        Gnum                vertend;
        Gnum                randend;
        Gnum                coloend;

        vertend = edgetax[edgenum];
        coloend = colotax[vertend];

        if ((coloend >= 0) &&                     /* If vertex has been colored    */
            (coloend < colonum))                  /* In a former pass              */
          continue;                               /* Do not consider it any longer */

        randend = randtax[vertend];
        if ((randend > randval) ||
            ((randend == randval) && (vertend > vertnum))) /* Tie breaking when same random value */
          break;
      }
      if (edgenum >= edgennd)
        colotax[vertnum] = colonum;
      else
        queutax[queunew ++] = vertnum;
    }
    queunnd = queunew;
  }

  *coloptr = colonum;                             /* Set number of colors found */

  memFree (queutax + baseval);

  o = 0;

fail:
  CONTEXTEXIT (libgrafptr);
  return (o);
}
