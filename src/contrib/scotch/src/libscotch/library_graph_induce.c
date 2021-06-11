/* Copyright 2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_induce.c                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the source   **/
/**                graph handling routines of the          **/
/**                libSCOTCH library.                      **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 14 jan 2018     **/
/**                                 to   : 21 apr 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "graph.h"
#include "scotch.h"

/************************************/
/*                                  */
/* These routines are the C API for */
/* the graph handling routines.     */
/*                                  */
/************************************/

/* This routine builds the graph induced
** by the original graph and the list of
** selected vertices.
** The induced vnumtab array is the list
** array if the original graph does not have
** a vnumtab, or the proper subset of the
** original vnumtab else.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphInduceList (
const SCOTCH_Graph * restrict const orggrafptr,
const SCOTCH_Num                    vnumnbr,
const SCOTCH_Num * restrict const   vnumtab,
SCOTCH_Graph * restrict const       indgrafptr)
{
  return (graphInduceList ((const Graph * restrict const) orggrafptr,
                           (const Gnum) vnumnbr,
                           (const Gnum * restrict const) vnumtab,
                           (Graph * const) indgrafptr));
}

/* This routine builds the graph induced
** by the original graph and the vector
** of selected vertices.
** The induced vnumtab array is the list of
** selected vertices if the original graph
** does not have a vnumtab, or the proper
** subset of the original vnumtab else.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphInducePart (
const SCOTCH_Graph * restrict const       orggrafptr,
const SCOTCH_Num                          vnumnbr,
const SCOTCH_GraphPart2 * restrict const  parttab,
const SCOTCH_GraphPart2                   partval,
SCOTCH_Graph * restrict const             indgrafptr)
{
  return (graphInducePart ((const Graph * restrict const) orggrafptr,
                           ((GraphPart * restrict const) parttab) - ((const Graph * const) orggrafptr)->baseval,
                           (const Gnum) vnumnbr,
                           (const GraphPart) partval,
                           (Graph * restrict const) indgrafptr));
}
