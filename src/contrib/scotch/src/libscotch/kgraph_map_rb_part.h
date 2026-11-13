/* Copyright 2008,2011,2014,2018,2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_rb_part.h                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                Clement BARTHELEMY                      **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the Dual Recursive Bipartitioning   **/
/**                mapping algorithm.                      **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 22 sep 2008     **/
/**                                 to   : 14 apr 2011     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 07 jun 2018     **/
/**                # Version 7.0  : from : 06 may 2021     **/
/**                                 to   : 16 jul 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the splitting parameters. +*/

typedef struct KgraphMapRbPartSplit2_ {
  Gnum                        vertnbr;            /*+ Number of vertices in part or in graph         +*/
  Anum                        vflonbr;            /*+ Number of fixed vertex load slots              +*/
  KgraphMapRbVflo * restrict  vflotab;            /*+ Array of fixed vertex load slots               +*/
  ArchDom const *             domnptr;            /*+ Original domain to bipartition (to avoid lock) +*/
} KgraphMapRbPartSplit2;

typedef struct KgraphMapRbPartSplit_ {
  KgraphMapRbPartSplit2             splttab[2];   /*+ Array of induced subgraph data           +*/
  KgraphMapRbData const * restrict  dataptr;      /*+ Global mapping data                      +*/
  Graph const * restrict            grafptr;      /*+ Graph to induce and bipartition          +*/
  GraphPart const * restrict        parttax;      /*+ Part array of original graph to consider +*/
  Gnum                              levlnum;      /*+ Recursion level number                   +*/
  int *                             revaptr;      /*+ Pointer to return value                  +*/
} KgraphMapRbPartSplit;

/*
**  The function prototypes.
*/

#ifdef SCOTCH_KGRAPH_MAP_RB_PART
static void                 kgraphMapRbPart2    (Context * restrict const, const int, KgraphMapRbPartSplit * const);
#endif /* SCOTCH_KGRAPH_MAP_RB_PART */

int                         kgraphMapRbPart     (const KgraphMapRbData * restrict const, const Graph * restrict const, const Anum, KgraphMapRbVflo * restrict const, Context * restrict const);
