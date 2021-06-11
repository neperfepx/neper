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
/**   NAME       : arch_deco2.h                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the type-2 decomposition-defined    **/
/**                target architecture functions.          **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 01 may 2015     **/
/**                                 to   : 28 may 2018     **/
/**                                                        **/
/**   NOTES      : # The internals of the "deco2"          **/
/**                  architecture derive from those of     **/
/**                  the "sub" architecture regarding      **/
/**                  domain handling. Hence, most domain-  **/
/**                  related routines are identical, and   **/
/**                  amount to looking-up into dedicated   **/
/**                  arrays. Hence, it was more efficient  **/
/**                  to copy its code act as a sub-class   **/
/**                  of the "sub" class. The main          **/
/**                  difference with the latter is in the  **/
/**                  distance computation routine, which   **/
/**                  uses the graph hierarchy.             **/
/**                  Matching in the "deco2" architecture  **/
/**                  is identical to matching in the       **/
/**                  "sub" architecture.                   **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

#ifndef ARCH_DECO2_H_STRUCT
#define ARCH_DECO2_H_STRUCT

/*+ Depth of local search. +*/

#define ARCHDECO2PASSNBR            3

/*+ Decomposition architecture flags. +*/

#define ARCHDECONONE                0x0000        /*+ No options set +*/

#define ARCHDECOFREE                0x0001        /*+ Free arrays    +*/

/*
**  The internal type and structure definitions.
*/

#ifdef ARCH_DECO2

typedef struct ArchDeco2Levl_ {
  Graph                     grafdat;              /*+ Graph at this level     +*/
  Gnum                      wdiaval;              /*+ Graph weighted diameter +*/
} ArchDeco2Levl;

/*+ Data related to each architecture domain.
    This structure extends ArchSubData in order
    to record vertex numbers associated with
    every domain and ascendent collapsed domains. +*/

typedef struct ArchDeco2Data_ {
  Anum                      levlnum;              /*+ Deepest level where this domain exists +*/
  Anum                      vnumidx;              /*+ Index to vertex number index array     +*/
} ArchDeco2Data;

#endif /* ARCH_DECO2 */

/*
**  The type and structure definitions.
*/

/*+ The type-2 decomposition-described target
    architecture. This architecture is based on
    the "sub" architecture for handling domains.
    Distance computations are specific.          +*/

typedef struct ArchDeco2_ {
  Anum                      baseval;              /*+ Base value for terminal numbers         +*/
  Anum                      termnbr;              /*+ Number of terminals in architecture     +*/
  struct ArchSubTerm_ *     termtab;              /*+ Terminal domain array                   +*/
  Anum                      domnnbr;              /*+ Number of domains in domain array       +*/
  struct ArchSubData_ *     domntab;              /*+ Domain array [domnnbr]                  +*/
  struct ArchDeco2Data_ *   doextab;              /*+ Extended domain array [domnnbr]         +*/
  Anum                      vnumnbr;              /*+ Number of indices un vertex index array +*/
  Anum *                    vnumtab;              /*+ Level vertex index array [vnumnbr]      +*/
  Anum                      levlmax;              /*+ Maximum level number in decomposition   +*/
  struct ArchDeco2Levl_ *   levltab;              /*+ Level array [levlnbr]                   +*/
} ArchDeco2;

typedef struct ArchDeco2Dom_ {
  Anum                      domnidx;              /*+ Domain index in domain array +*/
} ArchDeco2Dom;

#endif /* ARCH_DECO2_H_STRUCT */

/*
**  The function prototypes.
*/

#ifndef ARCH_NOPROTO
#ifndef ARCH_DECO2_H_PROTO
#define ARCH_DECO2_H_PROTO

#define archDeco2ArchLoad           NULL
int                         archDeco2ArchLoad2  (ArchDeco2 * const, FILE * restrict const);
int                         archDeco2ArchSave   (const ArchDeco2 * const, FILE * restrict const);
int                         archDeco2ArchFree   (ArchDeco2 * const);

int                         archDeco2MatchInit  (struct ArchSubMatch_ * restrict const, const ArchDeco2 * restrict const); /* Use "sub" matching routines */
#define archDeco2MatchExit          archSubMatchExit
#define archDeco2MatchMate          archSubMatchMate

ArchDomNum                  archDeco2DomNum     (const ArchDeco2 * const, const ArchDeco2Dom * const);
int                         archDeco2DomTerm    (const ArchDeco2 * const, ArchDeco2Dom * restrict const, const ArchDomNum);
Anum                        archDeco2DomSize    (const ArchDeco2 * const, const ArchDeco2Dom * const);
Anum                        archDeco2DomWght    (const ArchDeco2 * const, const ArchDeco2Dom * const);
Anum                        archDeco2DomDist    (const ArchDeco2 * const, const ArchDeco2Dom * const, const ArchDeco2Dom * const);
int                         archDeco2DomFrst    (const ArchDeco2 * const, ArchDeco2Dom * restrict const);
int                         archDeco2DomLoad    (const ArchDeco2 * const, ArchDeco2Dom * restrict const, FILE * restrict const);
int                         archDeco2DomSave    (const ArchDeco2 * const, const ArchDeco2Dom * const, FILE * restrict const);
int                         archDeco2DomBipart  (const ArchDeco2 * const, const ArchDeco2Dom * const, ArchDeco2Dom * restrict const, ArchDeco2Dom * restrict const);
int                         archDeco2DomIncl    (const ArchDeco2 * const, const ArchDeco2Dom * const, const ArchDeco2Dom * const);
#ifdef SCOTCH_PTSCOTCH
int                         archDecoDomMpiType  (const ArchDeco2 * const, MPI_Datatype * const);
#endif /* SCOTCH_PTSCOTCH */

#endif /* ARCH_DECO2_H_PROTO */
#endif /* ARCH_NOPROTO       */
