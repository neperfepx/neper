/* Copyright 2015,2016,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_build2.h                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                for the type-2 decomposition-described  **/
/**                architecture building routine.          **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 02 may 2015     **/
/**                                 to   : 31 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The internal type and structure definitions.
*/

#ifdef ARCH_BUILD2

/*+ Ancillary structure to avoid passing
    too many parameters recursively in
    the archDeco2Build3() routine.       +*/

typedef struct ArchDeco2Build3_ {
  ArchSubData *             domntab;              /*+ Domain array to fill [domnnbr]          +*/
  ArchDeco2Data *           doextab;              /*+ Extended domain array to fill [domnnbr] +*/
  ArchSubTerm *             termtab;              /*+ Terminal domain array to fill [termnbr] +*/
  Gnum *                    vnumtab;              /*+ Level vertex index array                +*/
  Anum                      vnumidx;              /*+ Current first free index in index array +*/
} ArchDeco2Build3;

/*+ Hash table cell to find coarse vertex
    ends during the computation of edge
    costs in the coarsened graph.         +*/

typedef struct ArchDeco2BuildHash_ {
  Gnum                      coarvertnum;          /*+ Origin vertex (i.e. pass) number     +*/
  Gnum                      coarvertend;          /*+ Other end vertex number              +*/
  Gnum                      coaredgenum;          /*+ Index of corresponding edge in graph +*/
  Gnum                      fineedwgmin;          /*+ Minimum of all fine edge weights     +*/
} ArchDeco2BuildHash;

/*+ Communication vertex and edge weights for
    graph coarsenings. Edge array is the group
    leader because there are no vertex traversal
    costs at level 0, while there may be edge
    traversal costs.                             +*/

typedef struct ArchDeco2BuildLevl_ {
  Gnum *                    edwgtab;              /*+ Edge load array; TRICK: group leader +*/
  Gnum *                    vewgtab;              /*+ Vertex load array                    +*/
  Gnum                      edwgsum;              /*+ Sum of edge weights                  +*/
  Gnum                      vewgsum;              /*+ Sum of vertex weights                +*/
} ArchDeco2BuildLevl;

/*+ Matching structure for graph coarsenings. +*/

typedef struct ArchDeco2BuildMatch_ {
  ArchDeco2BuildHash *      hashtab;              /*+ Hash array for edge coarsening           +*/
  Gnum                      hashsiz;              /*+ Size of hash data structure              +*/
  ArchCoarsenMulti *        multtab;              /*+ Multinode array for all coarsenings      +*/
  Gnum *                    ficotab;              /*+ Fine-to-coarse array for all coarsenings +*/
  ArchDeco2BuildLevl *      lewgtab;              /*+ Level array for vertex weights           +*/
  ArchDeco2Levl *           levltab;              /*+ Level array                              +*/
  Gnum                      levlmax;              /*+ Maximum number of levels in array        +*/
  Gnum                      levlnum;              /*+ Current number of levels in array        +*/
  Gnum                      vertsum;              /*+ Sum of all vertices at all levels        +*/
} ArchDeco2BuildMatch;

#endif /* ARCH_BUILD2 */

/*
**  The function prototypes.
*/

#ifdef ARCH_BUILD2
static int                  archDeco2BuildMatchInit (ArchDeco2BuildMatch * restrict const, const Graph * restrict const);
static void                 archDeco2BuildMatchExit (ArchDeco2BuildMatch * restrict const);
static Anum                 archDeco2BuildMatchMate (ArchDeco2BuildMatch * restrict const, ArchCoarsenMulti * restrict * restrict const);
#endif /* ARCH_BUILD2 */

int                         archDeco2ArchBuild  (Arch * const, const Graph * const, const Gnum, const Gnum * restrict const);
