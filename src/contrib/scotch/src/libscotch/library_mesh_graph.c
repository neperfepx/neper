/* Copyright 2004,2007,2021,2023,2025,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_mesh_graph.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Marc FUENTES (v6.1)                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the Scotch   **/
/**                mesh-to-graph converter.                **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 21 jan 2004     **/
/**                                 to   : 21 jan 2004     **/
/**                # Version 6.1  : from : 28 feb 2021     **/
/**                                 to   : 28 feb 2021     **/
/**                # Version 7.0  : from : 21 jan 2023     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "graph.h"
#include "mesh.h"
#include "scotch.h"

/*************************************/
/*                                   */
/* These routines are the C API for  */
/* the Scotch mesh and graph         */
/* handling routines.                */
/*                                   */
/*************************************/

/* This routine builds an opaque nodal graph structure
** from an opaque mesh structure. The nodal graph
** adjacency is defined such that two nodes are adjacent
** if both share at least one element.
** - 0   : if building has succeeded.
** - !0  : on error.
*/

int
SCOTCH_meshGraph (
const SCOTCH_Mesh * restrict const  meshptr,
SCOTCH_Graph * restrict const       grafptr)
{
  return (meshGraphNodal ((Mesh *) CONTEXTOBJECT (meshptr), (Graph *) grafptr));
}

/* This routine builds an opaque dual graph structure
** from an opaque mesh structure. The dual graph adjacency
** is defined such that two elements are adjacent, if
** both shared at least noconbr points.
** - 0   : if building has succeeded.
** - !0  : on error.
*/

int
SCOTCH_meshGraphDual (
const SCOTCH_Mesh * restrict const  meshptr,
SCOTCH_Graph * restrict const       grafptr,
const SCOTCH_Num                    noconbr)
{
  return (meshGraphDual ((Mesh *) CONTEXTOBJECT (meshptr), (Graph *) grafptr, noconbr));
}
