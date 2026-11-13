/* Copyright 2019,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_context_graph.c                 **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the context  **/
/**                management routines of the libSCOTCH    **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 25 aug 2019     **/
/**                                 to   : 24 oct 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "graph.h"
#include "scotch.h"

/****************************************/
/*                                      */
/* These routines are the C API for     */
/* execution context structure handling */
/* routines.                            */
/*                                      */
/****************************************/


/* This routine binds a context to a graph.
** It returns:
** - 0   : if the binding succeeded.
** - !0  : on error.
*/

int
SCOTCH_contextBindGraph (
SCOTCH_Context * restrict const     libcontptr,
const SCOTCH_Graph * restrict const orggrafptr,
SCOTCH_Graph * restrict const       cntgrafptr)
{
  ContextContainer * restrict const cocoptr = (ContextContainer *) cntgrafptr;
  Context * restrict const          contptr = (Context *) libcontptr;
  Graph * restrict const            grafptr = (Graph *) orggrafptr;

  if (sizeof (SCOTCH_Graph) < sizeof (ContextContainer)) {
    errorPrint (STRINGIFY (SCOTCH_contextBindGraph) ": internal error");
    return (1);
  }

#ifdef SCOTCH_DEBUG_LIBRARY1
  if (grafptr == NULL) {
    errorPrint (STRINGIFY (SCOTCH_contextBindGraph) ": invalid graph");
    return (1);
  }
  if ((grafptr->flagval & CONTEXTCONTAINERTYPE) != 0) {
    errorPrint (STRINGIFY (SCOTCH_contextBindGraph) ": cannot bind to a container");
    return (1);
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */

  if (contextCommit (contptr) != 0) {
    errorPrint (STRINGIFY (SCOTCH_contextBindGraph) ": cannot commit context");
    return (1);
  }

  memSet (cocoptr, 0, sizeof (SCOTCH_Graph));     /* Bind context to graph */
  cocoptr->flagval = CONTEXTCONTAINERTYPE;
  cocoptr->contptr = contptr;
  cocoptr->dataptr = grafptr;

  return (0);
}
