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
/**   NAME       : arch_sub.h                              **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the sub-architecture meta-          **/
/**                architecture functions.                 **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 13 mar 2015     **/
/**                                 to   : 31 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

#ifndef ARCH_SUB_H_STRUCT
#define ARCH_SUB_H_STRUCT

/*+ The sub-architecture definitions. +*/

typedef struct ArchSubTerm_ {
  Anum                      domnidx;              /*+ Domain index corresponding to local terminal number             +*/
  Anum                      termnum;              /*+ Corresponding un-based terminal number in original architecture +*/
} ArchSubTerm;

typedef struct ArchSubData_ {
  Anum                      domnnum;              /*+ Domain number, for dichotomy part. Starts at 1              +*/
  Anum                      domnsiz;              /*+ Size of domain, in terms of internal vertices               +*/
  Anum                      domnwgt;              /*+ Weight of internal vertices within domain                   +*/
  Anum                      termnum;              /*+ Smallest original un-based terminal domain number in domain +*/
  Anum                      dfatidx;              /*+ Index of father subdomain; -1 if root                       +*/
  Anum                      dsubidx[2];           /*+ Indices of son subdomains; -1 if terminal                   +*/
} ArchSubData;

typedef struct ArchSub_ {
  struct Arch_ *            archptr;              /*+ Pointer to original architecture        +*/
  Anum                      termnbr;              /*+ Number of terminals in sub-architecture +*/
  ArchSubTerm *             termtab;              /*+ Terminal domain array                   +*/
  Anum                      domnnbr;              /*+ Number of domains in domain array       +*/
  ArchSubData *             domntab;              /*+ Domain array                            +*/
} ArchSub;

typedef struct ArchSubTree_ {
  Anum                      domnsiz;              /*+ Size of domain                            +*/
  Anum                      domnwgt;              /*+ Weight of internal vertices within domain +*/
  Anum                      termnum;              /*+ Number of first subdomain; -1 if terminal +*/
  Anum                      vertnum;              /*+ Number of vertex at this level            +*/
  struct ArchSubTree_ *     sonstab[2];           /*+ Pointer to son cells                      +*/
} ArchSubTree;

typedef struct ArchSubDom_ {
  Anum                      domnidx;              /*+ Domain index in domain array +*/
} ArchSubDom;

typedef struct ArchSubMatch_ {
  const ArchSubData *       domntab;              /*+ Pointer to sub-architecture domain array     +*/
  ArchCoarsenMulti *        multtab;              /*+ Multinode array for all coarsenings          +*/
  Anum                      multnum;              /*+ Current number of multinode at current level +*/
  Anum                      vertnum;              /*+ Current fine vertex number at current level  +*/
  Anum                      levlnum;              /*+ Current level                                +*/
  Anum                      levlmax;              /*+ Maximum level to be considered               +*/
} ArchSubMatch;

#endif /* ARCH_SUB_H_STRUCT */

/*
**  The function prototypes.
*/

#ifndef ARCH_NOPROTO

int                         archSubArchLoad     (ArchSub * restrict const, FILE * restrict const);
int                         archSubArchSave     (const ArchSub * const, FILE * restrict const);
int                         archSubArchFree     (ArchSub * const);

int                         archSubMatchInit    (ArchSubMatch * restrict const, const ArchSub * restrict const);
void                        archSubMatchExit    (ArchSubMatch * restrict const);
Anum                        archSubMatchMate    (ArchSubMatch * restrict const, ArchCoarsenMulti ** restrict const);

int                         archSubArchBuild    (ArchSub * const, Arch * const, const Anum, const Anum * const);
ArchSubTree *               archSubArchBuild2   (void *, Anum (*) (void *, ArchCoarsenMulti * restrict *), ArchSubTree * restrict const, const Anum);
ArchDomNum                  archSubDomNum       (const ArchSub * const, const ArchSubDom * const);
int                         archSubDomTerm      (const ArchSub * const, ArchSubDom * restrict const, const ArchDomNum);
Anum                        archSubDomSize      (const ArchSub * const, const ArchSubDom * const);
Anum                        archSubDomWght      (const ArchSub * const, const ArchSubDom * const);
Anum                        archSubDomDist      (const ArchSub * const, const ArchSubDom * const, const ArchSubDom * const);
int                         archSubDomFrst      (const ArchSub * const, ArchSubDom * const);
int                         archSubDomLoad      (const ArchSub * const, ArchSubDom * const, FILE * const);
int                         archSubDomSave      (const ArchSub * const, const ArchSubDom * const, FILE * const);
int                         archSubDomBipart    (const ArchSub * const, const ArchSubDom * const, ArchSubDom * restrict const, ArchSubDom * restrict const);
int                         archSubDomIncl      (const ArchSub * const, const ArchSubDom * const, const ArchSubDom * const);
#ifdef SCOTCH_PTSCOTCH
int                         archSubDomMpiType   (const ArchSub * const, MPI_Datatype * const);
#endif /* SCOTCH_PTSCOTCH */

#endif /* ARCH_NOPROTO     */
