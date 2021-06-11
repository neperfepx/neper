/* Copyright 2017,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_diam.c                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the graph          **/
/**                (weighted) pseudo-diameter computation  **/
/**                routines.                               **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 17 jan 2017     **/
/**                                 to   : 22 feb 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define GRAPH

#include "module.h"
#include "common.h"
#include "graph.h"
#include "fibo.h"
#include "graph_diam.h"

/***************************************/
/*                                     */
/* This routine handles source graphs. */
/*                                     */
/***************************************/

/* Comparison routine for the Fibonacci
** heap structure.
*/

static
int
graphDiamCmpFunc (
const FiboNode *            nod0ptr,
const FiboNode *            nod1ptr)
{
  Gnum                dis0val;
  Gnum                dis1val;

  dis0val = ((GraphDiamVertex *) nod0ptr)->distval;
  dis1val = ((GraphDiamVertex *) nod1ptr)->distval;
  if (dis0val < dis1val)
    return (-1);
  return (1);
}

/* This routine computes the vertex-traversal,
** edge-weighted pseudo-diameter of the given
** graph. It is equal to GNUMMAX if the graph
** is not connected.
** It returns:
** - >= 0  : weighted pseudo-diameter.
** - -1    : on error.
*/

Gnum
graphDiamPV (
const Graph * const         grafptr)
{
  FiboHeap                    fibodat;
  GraphDiamVertex * restrict  vexxtax;            /* Extended vertex array */
  Gnum                        vertnum;
  Gnum                        rootnum;
  Gnum                        diammax;
  Gnum                        diambst;

  const Gnum * restrict const verttax = grafptr->verttax;
  const Gnum * restrict const vendtax = grafptr->vendtax;
  const Gnum * restrict const velotax = grafptr->velotax;
  const Gnum * restrict const edgetax = grafptr->edgetax;
  const Gnum * restrict const edlotax = grafptr->edlotax;

  if ((vexxtax = (GraphDiamVertex *) memAlloc (grafptr->vertnbr * sizeof (GraphDiamVertex))) == NULL) {
    errorPrint ("graphWdiam: out of memory");
    return     (-1);
  }
  if (fiboHeapInit (&fibodat, graphDiamCmpFunc) != 0) {
    errorPrint ("graphWdiam: cannot initialize Fibonacci heap");
    memFree    (vexxtax);
    return     (-1);
  }
  vexxtax -= grafptr->baseval;

  rootnum = intRandVal (grafptr->vertnbr) + grafptr->baseval;
  diammax = 0;                                    /* Ensure at least one pass */

  do {
    GraphDiamVertex *   vexxptr;
    Gnum                vertnbr;

    diambst = diammax;                            /* Record as starting values for this pass */

    fiboHeapFree (&fibodat);                      /* Reset tree */
    for (vertnum = grafptr->baseval; vertnum < grafptr->vertnnd; vertnum ++) /* Reset work array */
      vexxtax[vertnum].distval = GNUMMAX;

    vexxptr = vexxtax + rootnum;
    vexxptr->distval = 0;
    fiboHeapAdd (&fibodat, (FiboNode *) vexxptr);

    vertnbr = 0;
    while ((vexxptr = (GraphDiamVertex *) fiboHeapMin (&fibodat)) != NULL) { /* As long as there's a vertex to extract */
      Gnum                vertnum;
      Gnum                distval;
      Gnum                edgenum;

      vertnbr ++;                                 /* One more vertex processed        */
      fiboHeapDel (&fibodat, (FiboNode *) vexxptr); /* Remove it from heap            */
      distval = vexxptr->distval;                 /* Keep current distance value      */
      vexxptr->distval = -1;                      /* Flag vertex as already processed */

      vertnum = vexxptr - vexxtax;
      if (diammax < distval) {
        diammax = distval;
        rootnum = vertnum;
      }
      if (velotax != NULL)                        /* Add node weight as it is being traversed */
        distval += velotax[vertnum];

      for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
        GraphDiamVertex *   vexxpnd;
        Gnum                vertend;
        Gnum                disttmp;
        Gnum                distold;

        vertend = edgetax[edgenum];
        vexxpnd = vexxtax + vertend;
        disttmp = distval + ((edlotax != NULL) ? edlotax[edgenum] : 1);
        distold = vexxpnd->distval;

        if (disttmp < distold) {                  /* Never the case for already processed vertices */
          vexxpnd->distval = disttmp;
          if (distold == GNUMMAX)                 /* If newly touched node      */
            fiboHeapAdd (&fibodat, (FiboNode *) vexxpnd); /* Add it to the heap */
          else
            fiboHeapDecrease (&fibodat, (FiboNode *) vexxpnd); /* Else decrease its value in the heap */
        }
      }
    }

    if (vertnbr != grafptr->vertnbr) {            /* If graph is not connected */
      diambst = GNUMMAX;
      goto abort;
    }
  } while (diambst < diammax);

  diambst = diammax;

abort:
  fiboHeapExit (&fibodat);
  memFree (vexxtax + grafptr->baseval);

  return (diambst);
}
