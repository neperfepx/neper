/* Copyright 2016 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_clone.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the source graph    **/
/**                cloning function.                       **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 22 feb 2016     **/
/**                                 to   : 22 feb 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define GRAPH
#define GRAPH_CLONE

#include "module.h"
#include "common.h"
#include "graph.h"

/****************************************/
/*                                      */
/* These routines handle source graphs. */
/*                                      */
/****************************************/

/* This routine builds a clone of the given
** graph.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
graphClone (
const Graph * restrict const    orggrafptr,
Graph * restrict const          clngrafptr)
{
  Gnum *                datatab;
  Gnum                  baseval;
  Gnum                  vertnbr;
  Gnum                  vertnnd;
  Gnum                  datasiz;
  Gnum                  edgesiz;

  baseval = orggrafptr->baseval;
  vertnbr = orggrafptr->vertnbr;

  datasiz  = vertnbr;                             /* Account for verttab */
  datasiz += (orggrafptr->vendtax != (orggrafptr->verttax + 1)) ? vertnbr : 1; /* Account for vendtab or compact array */
  if (orggrafptr->velotax != NULL)
    datasiz += vertnbr;                           /* Account for velotab */
  if (orggrafptr->vnumtax != NULL)
    datasiz += vertnbr;                           /* Account for vnumtab */
  if (orggrafptr->vlbltax != NULL)
    datasiz += vertnbr;                           /* Account for vlbltab */

  if ((datatab = memAlloc (datasiz * sizeof (Gnum))) == NULL) {
    errorPrint ("graphClone: out of memory (1)");
    return     (1);
  }

  clngrafptr->flagval = GRAPHFREETABS | GRAPHVERTGROUP | GRAPHEDGEGROUP; /* Cloned graph has its own arrays */
  clngrafptr->baseval = baseval;
  clngrafptr->vertnbr = vertnbr;
  clngrafptr->vertnnd = vertnnd = vertnbr + baseval;

  clngrafptr->verttax = datatab - baseval;        /* Manage verttab */
  memCpy (datatab, orggrafptr->verttax + baseval, vertnbr * sizeof (Gnum));
  datatab += vertnbr;

  if (orggrafptr->vendtax == (orggrafptr->verttax + 1)) { /* If compact array  */
    clngrafptr->vendtax = clngrafptr->verttax + 1; /* Set compact array        */
    edgesiz = orggrafptr->verttax[vertnnd];       /* Get end of compact array  */
    clngrafptr->verttax[vertnnd] = edgesiz;       /* Copy end of compact array */
    datatab ++;                                   /* Space for last index      */
  }
  else {
    const Gnum * restrict orgvendptr;
    Gnum * restrict       clnvendptr;
    Gnum                  vertnum;

    clngrafptr->vendtax = datatab - baseval;      /* Set vendtab */
    clnvendptr = datatab;
    orgvendptr = orggrafptr->vendtax + baseval;
    for (vertnum = 0, edgesiz = 0; vertnum < vertnbr; vertnum ++) {
      Gnum                vendval;

      vendval = *orgvendptr ++;                   /* Copy vertex index */
      *clnvendptr ++ = vendval;
      if (vendval > edgesiz)                      /* Record highest end index */
        edgesiz = vendval;
    }
    datatab = clnvendptr;                         /* Resume after end of cloned array */
  }
  edgesiz -= baseval;                             /* Edge size is given by highest index */

  if (orggrafptr->velotax != NULL) {              /* Account for velotab */
    memCpy (datatab, orggrafptr->velotax + baseval, vertnbr * sizeof (Gnum));
    clngrafptr->velotax = datatab - baseval;
    datatab += vertnbr;
  }
  else
    clngrafptr->velotax = NULL;

  clngrafptr->velosum = orggrafptr->velosum;

  if (orggrafptr->vnumtax != NULL) {              /* Account for vnumtab */
    memCpy (datatab, orggrafptr->vnumtax + baseval, vertnbr * sizeof (Gnum));
    clngrafptr->vnumtax = datatab - baseval;
    datatab += vertnbr;
  }
  else
    clngrafptr->vnumtax = NULL;

  if (orggrafptr->vlbltax != NULL) {              /* Account for vlbltab */
    memCpy (datatab, orggrafptr->vlbltax + baseval, vertnbr * sizeof (Gnum));
    clngrafptr->vlbltax = datatab - baseval;
  }
  else
    clngrafptr->vlbltax = NULL;

  datasiz = edgesiz;                              /* Account for edgetab */
  if (orggrafptr->edlotax != NULL)
    datasiz += edgesiz;                           /* Account for edlotab */

  if ((datatab = memAlloc (datasiz * sizeof (Gnum))) == NULL) {
    errorPrint ("graphClone: out of memory (2)");
    memFree    (clngrafptr->verttax + baseval);   /* Free group leader */
    return     (1);
  }

  clngrafptr->edgenbr = orggrafptr->edgenbr;

  clngrafptr->edgetax = datatab - baseval;        /* Manage edgetab */
  memCpy (datatab, orggrafptr->edgetax + baseval, edgesiz * sizeof (Gnum));

  if (orggrafptr->edlotax != NULL) {              /* Manage edlotab */
    datatab += edgesiz;
    clngrafptr->edlotax = datatab - baseval;
    memCpy (datatab, orggrafptr->edlotax + baseval, edgesiz * sizeof (Gnum));
  }
  else
    clngrafptr->edlotax = NULL;

  clngrafptr->edlosum = orggrafptr->edlosum;
  clngrafptr->degrmax = orggrafptr->degrmax;
  clngrafptr->procptr = orggrafptr->procptr;

#ifdef SCOTCH_DEBUG_GRAPH2
  if (graphCheck (clngrafptr) != 0) {             /* Check graph consistency */
    errorPrint ("graphClone: inconsistent graph data");
    graphExit  (clngrafptr);
    return     (1);
  }
#endif /* SCOTCH_DEBUG_GRAPH2 */

  return (0);
}
