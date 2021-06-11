/* Copyright 2016,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_ielo.c                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the source graph    **/
/**                functions.                              **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 28 dec 2016     **/
/**                                 to   : 31 may 2018     **/
/**                                                        **/
/**   NOTES      : # This code derives from that of        **/
/**                  arch_build.c.                         **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define GRAPH

#include "module.h"
#include "common.h"
#include "graph.h"

/****************************************/
/*                                      */
/* These routines handle source graphs. */
/*                                      */
/****************************************/

/* This routine computes the inverse of the edge load
** array of the given graph structure. The original
** edge load array and the inverse edge load array
** might be the same.
** It returns:
** - Gnum  : new edlosum of the inverse weight load structure.
** - -1    : on error.
*/

Gnum
graphIelo (
const Graph * const         grafptr,
Gnum * const                edlotax,              /* Pointer to edge load array [norestrict]                 */
Gnum * const                ielotax)              /* Pointer to inverse edge load array to fill [norestrict] */
{
  Gnum                vertnum;
  Gnum                edlomin;
  Gnum                edlomax;
  Gnum                edlosum;
  float               prodval;

  const Gnum                  vertnnd = grafptr->vertnnd;
  const Gnum * restrict const verttax = grafptr->verttax;
  const Gnum * restrict const vendtax = grafptr->vendtax;

  edlomin = GNUMMAX;
  edlomax = 0;
  for (vertnum = grafptr->baseval; vertnum < vertnnd; vertnum ++) { /* Handle non-compact graphs as well as compact graphs */
    Gnum                  edgenum;
    Gnum                  edgennd;

    for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum];
         edgenum < edgennd; edgenum ++) {
      Gnum                edloval;

      edloval = edlotax[edgenum];
      if (edloval < edlomin)
        edlomin = edloval;
      if (edloval > edlomax)
        edlomax = edloval;
    }
  }

  if (edlomin < 1)                                /* Zero-weight edges cannot be inverted */
    edlomin = 1;
  prodval = (float) edlomin * (float) edlomax;

  edlosum = 0;
  for (vertnum = grafptr->baseval; vertnum < vertnnd; vertnum ++) {
    Gnum                  edgenum;
    Gnum                  edgennd;

    for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum];
         edgenum < edgennd; edgenum ++) {
      Gnum                edloval;

      edloval = edlotax[edgenum];
      if (edloval <= edlomin)                     /* "<=" for zero-weight edges */
        edloval = edlomax;
      else if (edloval == edlomax)
        edloval = edlomin;
      else
        edloval = (Gnum) (prodval / (float) edloval + 0.49F);
#ifdef SCOTCH_DEBUG_ARCH2
      if ((edloval < edlomin) || (edloval > edlomax)) {
        errorPrint ("graphIelo: internal error");
        return (-1);
      }
#endif /* SCOTCH_DEBUG_ARCH2 */
      edlosum +=                                  /* Accumulate edge load sum          */
      ielotax[edgenum] = edloval;                 /* Write inversed cost in work array */
    }
  }

  return (edlosum);
}
