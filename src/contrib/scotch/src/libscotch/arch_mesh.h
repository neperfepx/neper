/* Copyright 2004,2007,2008,2011,2014,2015,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_mesh.h                             **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the mesh graph target architecture  **/
/**                functions.                              **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 01 dec 1992     **/
/**                                 to   : 24 mar 1993     **/
/**                # Version 1.2  : from : 04 feb 1994     **/
/**                                 to   : 11 feb 1994     **/
/**                # Version 1.3  : from : 20 apr 1994     **/
/**                                 to   : 20 apr 1994     **/
/**                # Version 2.0  : from : 06 jun 1994     **/
/**                                 to   : 12 nov 1994     **/
/**                # Version 2.1  : from : 07 apr 1995     **/
/**                                 to   : 30 jun 1995     **/
/**                # Version 3.0  : from : 01 jul 1995     **/
/**                                 to   : 17 aug 1995     **/
/**                # Version 3.1  : from : 22 jul 1996     **/
/**                                 to   : 23 jul 1996     **/
/**                # Version 3.2  : from : 16 oct 1996     **/
/**                                 to   : 14 may 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 4.0  : from : 09 jan 2004     **/
/**                                 to   : 09 jan 2004     **/
/**                # Version 5.1  : from : 21 jan 2008     **/
/**                                 to   : 21 jan 2008     **/
/**                # Version 6.0  : from : 14 feb 2011     **/
/**                                 to   : 28 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The type and structure definitions.
*/

#ifndef ARCH_MESH_H_STRUCT
#define ARCH_MESH_H_STRUCT

/*+ Maximum dimension. +*/

#define ARCHMESHDIMNMAX             5             /* Maximum dimension (at least 3) */

/*+ The xD-mesh definitions. +*/

typedef struct ArchMeshX_ {
  Anum                      dimnnbr;              /*+ Number of mesh dimensions +*/
  Anum                      c[ARCHMESHDIMNMAX];   /*+ Mesh dimensions           +*/
} ArchMeshX;

typedef struct ArchMeshXDom_ {
  Anum                      c[ARCHMESHDIMNMAX][2]; /*+ Inclusive X and Y coordinates +*/
} ArchMeshXDom;

typedef struct ArchMeshXMatch_ {
  ArchCoarsenMulti *        multtab;              /*+ Multinode array for all coarsenings       +*/
  Anum                      dimnnbr;              /*+ Number of mesh dimensions                 +*/
  Anum                      dimnnum;              /*+ Current dimension to coarsen              +*/
  Anum                      dimntab[ARCHMESHDIMNMAX]; /*+ Mesh dimensions                       +*/
  int                       passtab[ARCHMESHDIMNMAX]; /*+ Flags for coarsening along dimensions +*/
} ArchMeshXMatch;

/*+ The 2D-mesh definitions. +*/

#define ArchMesh2                   ArchMeshX
#define ArchMesh2Dom                ArchMeshXDom
#define ArchMesh2Match              ArchMeshXMatch

/*+ The 3D-mesh definitions. +*/

#define ArchMesh3                   ArchMeshX
#define ArchMesh3Dom                ArchMeshXDom
#define ArchMesh3Match              ArchMeshXMatch

#endif /* ARCH_MESH_H_STRUCT */

/*
**  The function prototypes.
*/

#ifndef ARCH_NOPROTO
#ifndef ARCH_MESH_H_PROTO
#define ARCH_MESH_H_PROTO

int                         archMesh2ArchLoad   (ArchMesh2 * restrict const, FILE * restrict const);
int                         archMesh2ArchSave   (const ArchMesh2 * const, FILE * restrict const);
#define archMesh2ArchFree           NULL

#define archMesh2MatchInit          archMeshXMatchInit
#define archMesh2MatchExit          archMeshXMatchExit
#define archMesh2MatchMate          archMeshXMatchMate

ArchDomNum                  archMesh2DomNum     (const ArchMesh2 * const, const ArchMesh2Dom * const);
int                         archMesh2DomTerm    (const ArchMesh2 * const, ArchMesh2Dom * restrict const, const ArchDomNum);
Anum                        archMesh2DomSize    (const ArchMesh2 * const, const ArchMesh2Dom * const);
#define archMesh2DomWght            archMesh2DomSize
Anum                        archMesh2DomDist    (const ArchMesh2 * const, const ArchMesh2Dom * const, const ArchMesh2Dom * const);
#define archMesh2DomFrst            archMeshXDomFrst
#define archMesh2DomLoad            archMeshXDomLoad
#define archMesh2DomSave            archMeshXDomSave
int                         archMesh2DomBipart  (const ArchMesh2 * const, const ArchMesh2Dom * const, ArchMesh2Dom * restrict const, ArchMesh2Dom * restrict const);
int                         archMesh2DomBipartO (const ArchMesh2 * const, const ArchMesh2Dom * const, ArchMesh2Dom * restrict const, ArchMesh2Dom * restrict const);
int                         archMesh2DomBipartU (const ArchMesh2 * const, const ArchMesh2Dom * const, ArchMesh2Dom * restrict const, ArchMesh2Dom * restrict const);
int                         archMesh2DomIncl    (const ArchMesh2 * const, const ArchMesh2Dom * const, const ArchMesh2Dom * const);
#ifdef SCOTCH_PTSCOTCH
int                         archMesh2DomMpiType (const ArchMesh2 * const, MPI_Datatype * const);
#endif /* SCOTCH_PTSCOTCH */

int                         archMesh3ArchLoad   (ArchMesh3 * restrict const, FILE * restrict const);
int                         archMesh3ArchSave   (const ArchMesh3 * const, FILE * restrict const);
#define archMesh3ArchFree           NULL

#define archMesh3MatchInit          archMeshXMatchInit
#define archMesh3MatchExit          archMeshXMatchExit
#define archMesh3MatchMate          archMeshXMatchMate

ArchDomNum                  archMesh3DomNum     (const ArchMesh3 * const, const ArchMesh3Dom * const);
int                         archMesh3DomTerm    (const ArchMesh3 * const, ArchMesh3Dom * restrict const, const ArchDomNum);
Anum                        archMesh3DomSize    (const ArchMesh3 * const, const ArchMesh3Dom * const);
#define archMesh3DomWght            archMesh3DomSize
Anum                        archMesh3DomDist    (const ArchMesh3 * const, const ArchMesh3Dom * const, const ArchMesh3Dom * const);
#define archMesh3DomFrst            archMeshXDomFrst
#define archMesh3DomLoad            archMeshXDomLoad
#define archMesh3DomSave            archMeshXDomSave
int                         archMesh3DomBipart  (const ArchMesh3 * const, const ArchMesh3Dom * const, ArchMesh3Dom * restrict const, ArchMesh3Dom * restrict const);
int                         archMesh3DomIncl    (const ArchMesh3 * const, const ArchMesh3Dom * const, const ArchMesh3Dom * const);
#ifdef SCOTCH_PTSCOTCH
int                         archMesh3DomMpiType (const ArchMesh3 * const, MPI_Datatype * const);
#endif /* SCOTCH_PTSCOTCH */

int                         archMeshXArchLoad   (ArchMeshX * restrict const, FILE * restrict const);
int                         archMeshXArchSave   (const ArchMeshX * const, FILE * restrict const);
#define archMeshXArchFree           NULL

int                         archMeshXMatchInit  (ArchMeshXMatch * restrict const, const ArchMeshX * restrict const);
int                         archMeshXMatchInit2 (ArchMeshXMatch * restrict const, const Anum, const Anum * restrict const);
void                        archMeshXMatchExit  (ArchMeshXMatch * restrict const);
Anum                        archMeshXMatchMate  (ArchMeshXMatch * restrict const, ArchCoarsenMulti ** restrict const);

ArchDomNum                  archMeshXDomNum     (const ArchMeshX * const, const ArchMeshXDom * const);
int                         archMeshXDomTerm    (const ArchMeshX * const, ArchMeshXDom * restrict const, const ArchDomNum);
Anum                        archMeshXDomSize    (const ArchMeshX * const, const ArchMeshXDom * const);
#define archMeshXDomWght            archMeshXDomSize
Anum                        archMeshXDomDist    (const ArchMeshX * const, const ArchMeshXDom * const, const ArchMeshXDom * const);
int                         archMeshXDomFrst    (const ArchMeshX * const, ArchMeshXDom * const);
int                         archMeshXDomLoad    (const ArchMeshX * const, ArchMeshXDom * const, FILE * restrict const);
int                         archMeshXDomSave    (const ArchMeshX * const, const ArchMeshXDom * const, FILE * restrict const);
int                         archMeshXDomBipart  (const ArchMeshX * const, const ArchMeshXDom * const, ArchMeshXDom * restrict const, ArchMeshXDom * restrict const);
int                         archMeshXDomIncl    (const ArchMeshX * const, const ArchMeshXDom * const, const ArchMeshXDom * const);
#ifdef SCOTCH_PTSCOTCH
int                         archMeshXDomMpiType (const ArchMeshX * const, MPI_Datatype * const);
#endif /* SCOTCH_PTSCOTCH */

#endif /* ARCH_MESH_H_PROTO */
#endif /* ARCH_NOPROTO      */
