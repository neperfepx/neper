/* Copyright 2004,2007,2008,2011,2013,2014,2015,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_torus.h                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declaration    **/
/**                for the tori graph target architecture  **/
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
/**                # Version 4.0  : from : 05 nov 2003     **/
/**                                 to   : 05 nov 2003     **/
/**                # Version 5.1  : from : 21 jan 2008     **/
/**                                 to   : 21 jan 2008     **/
/**                # Version 6.0  : from : 14 feb 2011     **/
/**                                 to   : 28 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

#ifndef ARCH_TORUS_H_STRUCT
#define ARCH_TORUS_H_STRUCT

/*
**  The type and structure definitions.
*/

/*+ The torus definitions. +*/

#define ArchTorus2                  ArchMesh2
#define ArchTorus2Dom               ArchMesh2Dom

#define ArchTorus3                  ArchMesh3
#define ArchTorus3Dom               ArchMesh3Dom

#define ArchTorusX                  ArchMeshX
#define ArchTorusXDom               ArchMeshXDom

#endif /* ARCH_TORUS_H_STRUCT */

/*
**  The function prototypes.
*/

#ifndef ARCH_NOPROTO
#ifndef ARCH_TORUS_H_PROTO
#define ARCH_TORUS_H_PROTO

#define archTorus2ArchLoad          archMesh2ArchLoad
#define archTorus2ArchSave          archMesh2ArchSave
#define archTorus2ArchFree          archMesh2ArchFree

#define archTorus2MatchInit         archMesh2MatchInit
#define archTorus2MatchExit         archMesh2MatchExit
#define archTorus2MatchMate         archMesh2MatchMate

#define archTorus2DomNum            archMesh2DomNum
#define archTorus2DomTerm           archMesh2DomTerm
#define archTorus2DomSize           archMesh2DomSize
#define archTorus2DomWght           archMesh2DomSize
Anum                        archTorus2DomDist   (const ArchTorus2 * const, const ArchTorus2Dom * const, const ArchTorus2Dom * const);
#define archTorus2DomFrst           archMesh2DomFrst
#define archTorus2DomLoad           archMesh2DomLoad
#define archTorus2DomSave           archMesh2DomSave
#define archTorus2DomBipart         archMesh2DomBipart
#define archTorus2DomBipartO        archMesh2DomBipartO
#define archTorus2DomBipartU        archMesh2DomBipartU
#define archTorus2DomIncl           archMesh2DomIncl

#define archTorus3ArchLoad          archMesh3ArchLoad
#define archTorus3ArchSave          archMesh3ArchSave
#define archTorus3ArchFree          archMesh3ArchFree

#define archTorus3MatchInit         archMesh3MatchInit
#define archTorus3MatchExit         archMesh3MatchExit
#define archTorus3MatchMate         archMesh3MatchMate

#define archTorus3DomNum            archMesh3DomNum
#define archTorus3DomTerm           archMesh3DomTerm
#define archTorus3DomSize           archMesh3DomSize
#define archTorus3DomWght           archMesh3DomSize
Anum                        archTorus3DomDist   (const ArchTorus3 * const, const ArchTorus3Dom * const, const ArchTorus3Dom * const);
#define archTorus3DomFrst           archMesh3DomFrst
#define archTorus3DomLoad           archMesh3DomLoad
#define archTorus3DomSave           archMesh3DomSave
#define archTorus3DomBipart         archMesh3DomBipart
#define archTorus3DomIncl           archMesh3DomIncl

#define archTorusXArchLoad          archMeshXArchLoad
#define archTorusXArchSave          archMeshXArchSave
#define archTorusXArchFree          archMeshXArchFree

#define archTorusXMatchInit         archMeshXMatchInit
#define archTorusXMatchExit         archMeshXMatchExit
#define archTorusXMatchMate         archMeshXMatchMate

#define archTorusXDomNum            archMeshXDomNum
#define archTorusXDomTerm           archMeshXDomTerm
#define archTorusXDomSize           archMeshXDomSize
#define archTorusXDomWght           archMeshXDomSize
Anum                        archTorusXDomDist   (const ArchTorusX * const, const ArchTorusXDom * const, const ArchTorusXDom * const);
#define archTorusXDomFrst           archMeshXDomFrst
#define archTorusXDomLoad           archMeshXDomLoad
#define archTorusXDomSave           archMeshXDomSave
#define archTorusXDomBipart         archMeshXDomBipart
#define archTorusXDomIncl           archMeshXDomIncl

#endif /* ARCH_TORUS_H_PROTO */
#endif /* ARCH_NOPROTO       */
