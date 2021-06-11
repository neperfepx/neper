/* Copyright 2004,2007-2016,2018-2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : module.h                                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This is the global configuration file   **/
/**                for the whole libSCOTCH library module. **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 22 jun 1998     **/
/**                                 to   : 13 may 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 03 oct 1998     **/
/**                # Version 3.4  : from : 01 nov 2001     **/
/**                                 to   : 01 nov 2001     **/
/**                # Version 4.0  : from : 12 dec 2001     **/
/**                                 to   : 24 nov 2005     **/
/**                # Version 5.0  : from : 24 feb 2007     **/
/**                                 to   : 24 jul 2007     **/
/**                # Version 5.1  : from : 25 oct 2007     **/
/**                                 to   : 20 feb 2011     **/
/**                # Version 6.0  : from : 12 sep 2008     **/
/**                                 to   : 24 aug 2020     **/
/**                # Version 6.1  : from : 24 aug 2020     **/
/**                                 to   : 01 sep 2020     **/
/**                                                        **/
/************************************************************/

#define MODULE_H

/*
** Version and copyright strings.
*/

#define SCOTCH_COPYRIGHT_STRING     "Copyright 1992-2019 IPB, Universite de Bordeaux, INRIA & CNRS, France"
#define SCOTCH_LICENSE_STRING       "This software is libre/free software under CeCILL-C -- see the user's manual for more information"
#define SCOTCH_VERSION_STRING       STRINGIFY (SCOTCH_VERSION) "." STRINGIFY (SCOTCH_RELEASE) "." STRINGIFY (SCOTCH_PATCHLEVEL)

/*
** Handling of determinism.
*/

#ifdef SCOTCH_DETERMINISTIC
#ifndef COMMON_RANDOM_FIXED_SEED
#define COMMON_RANDOM_FIXED_SEED
#endif /* COMMON_RANDOM_FIXED_SEED */
#endif /* SCOTCH_DETERMINISTIC     */

/*
** Handling of parallelism.
*/

#ifdef SCOTCH_PTSCOTCH
#define COMMON_MPI
#endif /* SCOTCH_PTSCOTCH */

/*
** Thread management.
*/

/*+ Old versions of Scotch used COMMON_PTHREAD
   as an alias for COMMON_PTHREAD_FILE. Maintain
   that semantics.                               +*/

#ifdef COMMON_PTHREAD
#ifndef COMMON_PTHREAD_FILE
#define COMMON_PTHREAD_FILE
#endif /* COMMON_PTHREAD_FILE */
#endif /* COMMON_PTHREAD      */

/*+ Enable the new semantics of COMMON_PTHREAD_FILE. +*/

#ifdef COMMON_PTHREAD_FILE
#ifndef COMMON_PTHREAD
#define COMMON_PTHREAD
#endif /* COMMON_PTHREAD */
#ifndef COMMON_PTHREAD_MEMORY
#define COMMON_PTHREAD_MEMORY
#endif /* COMMON_PTHREAD_MEMORY */
#endif /* COMMON_PTHREAD_FILE */

/*+ Enable thread management from SCOTCH_PTHREAD. +*/

#ifdef SCOTCH_PTHREAD
#ifndef COMMON_PTHREAD
#define COMMON_PTHREAD
#endif /* COMMON_PTHREAD */
#ifndef COMMON_PTHREAD_MEMORY
#define COMMON_PTHREAD_MEMORY
#endif /* COMMON_PTHREAD_MEMORY */
#endif /* SCOTCH_PTHREAD        */

/*+ Handle number of threads. +*/

#ifdef SCOTCH_PTHREAD
#ifndef SCOTCH_PTHREAD_NUMBER
#define SCOTCH_PTHREAD_NUMBER       1
#endif /* SCOTCH_PTHREAD_NUMBER */

#else /* SCOTCH_PTHREAD */

#ifdef SCOTCH_PTHREAD_NUMBER
#undef SCOTCH_PTHREAD_NUMBER
#endif /* SCOTCH_PTHREAD_NUMBER */
#define SCOTCH_PTHREAD_NUMBER       1

#endif /* SCOTCH_PTHREAD */

/*+ Handle old semantics of thread affinity. */

#ifdef SCOTCH_PTHREAD_AFFINITY_LINUX
#ifndef COMMON_PTHREAD_AFFINITY_LINUX
#define COMMON_PTHREAD_AFFINITY_LINUX
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
#endif /* SCOTCH_PTHREAD_AFFINITY_LINUX */

/*
** Debug values.
*/

#ifdef SCOTCH_DEBUG_FULL
#ifndef SCOTCH_DEBUG_ALL
#define SCOTCH_DEBUG_ALL
#endif /* SCOTCH_DEBUG_ALL */

#define SCOTCH_DEBUG_BGRAPH3
#define SCOTCH_DEBUG_DGRAPH3
#define SCOTCH_DEBUG_FIBO3
#define SCOTCH_DEBUG_GAIN3
#define SCOTCH_DEBUG_KGRAPH3
#define SCOTCH_DEBUG_PARSER3
#define SCOTCH_DEBUG_VGRAPH3
#define SCOTCH_DEBUG_VMESH3
#define SCOTCH_DEBUG_WGRAPH3
#endif /* SCOTCH_DEBUG_FULL */

#ifdef SCOTCH_DEBUG_ALL
#ifndef SCOTCH_DEBUG
#define SCOTCH_DEBUG
#endif /* SCOTCH_DEBUG */

#define COMMON_DEBUG
#define SCOTCH_DEBUG_ARCH2
#define SCOTCH_DEBUG_FIBO2
#define SCOTCH_DEBUG_GAIN2
#define SCOTCH_DEBUG_PARSER2
#define SCOTCH_DEBUG_BDGRAPH2
#define SCOTCH_DEBUG_BGRAPH2
#define SCOTCH_DEBUG_DGRAPH2
#define SCOTCH_DEBUG_DMAP2
#define SCOTCH_DEBUG_DORDER2
#define SCOTCH_DEBUG_GEOM2
#define SCOTCH_DEBUG_GRAPH2
#define SCOTCH_DEBUG_HDGRAPH2
#define SCOTCH_DEBUG_HGRAPH2
#define SCOTCH_DEBUG_HMESH2
#define SCOTCH_DEBUG_KDGRAPH2
#define SCOTCH_DEBUG_KDMAP2
#define SCOTCH_DEBUG_KGRAPH2
#define SCOTCH_DEBUG_LIBRARY2
#define SCOTCH_DEBUG_MAP2
#define SCOTCH_DEBUG_MESH2
#define SCOTCH_DEBUG_ORDER2
#define SCOTCH_DEBUG_PARSER2
#define SCOTCH_DEBUG_VDGRAPH2
#define SCOTCH_DEBUG_VGRAPH2
#define SCOTCH_DEBUG_VMESH2
#define SCOTCH_DEBUG_WGRAPH2
#endif /* SCOTCH_DEBUG_ALL */

#ifdef SCOTCH_DEBUG
#define SCOTCH_DEBUG_ARCH1
#define SCOTCH_DEBUG_FIBO1
#define SCOTCH_DEBUG_GAIN1
#define SCOTCH_DEBUG_PARSER1
#define SCOTCH_DEBUG_BDGRAPH1
#define SCOTCH_DEBUG_BGRAPH1
#define SCOTCH_DEBUG_DGRAPH1
#define SCOTCH_DEBUG_DMAP1
#define SCOTCH_DEBUG_DORDER1
#define SCOTCH_DEBUG_GEOM1
#define SCOTCH_DEBUG_GRAPH1
#define SCOTCH_DEBUG_HDGRAPH1
#define SCOTCH_DEBUG_HGRAPH1
#define SCOTCH_DEBUG_HMESH1
#define SCOTCH_DEBUG_KDGRAPH1
#define SCOTCH_DEBUG_KDMAP1
#define SCOTCH_DEBUG_KGRAPH1
#define SCOTCH_DEBUG_LIBRARY1
#define SCOTCH_DEBUG_MAP1
#define SCOTCH_DEBUG_MESH1
#define SCOTCH_DEBUG_ORDER1
#define SCOTCH_DEBUG_PARSER1
#define SCOTCH_DEBUG_VDGRAPH1
#define SCOTCH_DEBUG_VGRAPH1
#define SCOTCH_DEBUG_VMESH1
#define SCOTCH_DEBUG_WGRAPH1
#endif /* SCOTCH_DEBUG */

/*
** Function renaming.
*/

#ifndef SCOTCH_NAME_SUFFIX
#define SCOTCH_NAME_SUFFIXC
#else /* SCOTCH_NAME_SUFFIX */
#ifndef SCOTCH_NAME_SUFFIXC
#define SCOTCH_NAME_SUFFIXC         SCOTCH_NAME_SUFFIX
#endif /* SCOTCH_NAME_SUFFIXC */
#ifndef SCOTCH_RENAME
#define SCOTCH_RENAME
#endif /* SCOTCH_RENAME */
#ifndef SCOTCH_RENAME_PUBLIC
#define SCOTCH_RENAME_PUBLIC
#endif /* SCOTCH_RENAME_PUBLIC */
#endif /* SCOTCH_NAME_SUFFIX   */
#ifndef SCOTCH_NAME_SUFFIXFL
#define SCOTCH_NAME_SUFFIXFL        SCOTCH_NAME_SUFFIXC
#define SCOTCH_NAME_SUFFIXFU        SCOTCH_NAME_SUFFIXC
#else /* SCOTCH_NAME_SUFFIXFL */
#ifndef SCOTCH_RENAME
#define SCOTCH_RENAME
#endif /* SCOTCH_RENAME */
#ifndef SCOTCH_RENAME_PUBLIC
#define SCOTCH_RENAME_PUBLIC
#endif /* SCOTCH_RENAME_PUBLIC */
#endif /* SCOTCH_NAME_SUFFIXFL */
#define SCOTCH_NAME_PREFIX_INTERN   _SCOTCH
#define SCOTCH_NAME_PREFIX_PUBLICFL scotchf
#define SCOTCH_NAME_PREFIX_PUBLICFU SCOTCHF

#define SCOTCH_NAME_GLUE2(n,s)      n##s
#define SCOTCH_NAME_GLUE3(p,n,s)    p##n##s
#define SCOTCH_NAME_MACRO2(n,s)     SCOTCH_NAME_GLUE2 (n,s)
#define SCOTCH_NAME_MACRO3(p,n,s)   SCOTCH_NAME_GLUE3 (p,n,s)
#define SCOTCH_NAME_INTERN(f)       SCOTCH_NAME_MACRO3 (SCOTCH_NAME_PREFIX_INTERN,f,SCOTCH_NAME_SUFFIXC)
#define SCOTCH_NAME_PUBLIC(f)       SCOTCH_NAME_MACRO2 (f,SCOTCH_NAME_SUFFIXC)
#define SCOTCH_NAME_PUBLICFL(f)     SCOTCH_NAME_MACRO3 (SCOTCH_NAME_PREFIX_PUBLICFL,f,SCOTCH_NAME_SUFFIXFL)
#define SCOTCH_NAME_PUBLICFU(f)     SCOTCH_NAME_MACRO3 (SCOTCH_NAME_PREFIX_PUBLICFU,f,SCOTCH_NAME_SUFFIXFU)
#define SCOTCH_FORTRAN(nu,nl,pl,pc) FORTRAN (SCOTCH_NAME_PUBLICFU(nu),SCOTCH_NAME_PUBLICFL(nl),pl,pc)

#ifdef SCOTCH_RENAME_ALL
#define SCOTCH_VERSION              SCOTCH_NAME_PUBLIC (SCOTCH_VERSION)
#define SCOTCH_RELEASE              SCOTCH_NAME_PUBLIC (SCOTCH_RELEASE)
#define SCOTCH_PATCHLEVEL           SCOTCH_NAME_PUBLIC (SCOTCH_PATCHLEVEL)
#define SCOTCH_COARSENNONE          SCOTCH_NAME_PUBLIC (SCOTCH_COARSENNONE)
#define SCOTCH_COARSENFOLD          SCOTCH_NAME_PUBLIC (SCOTCH_COARSENFOLD)
#define SCOTCH_COARSENFOLDDUP       SCOTCH_NAME_PUBLIC (SCOTCH_COARSENFOLDDUP)
#define SCOTCH_COARSENNOMERGE       SCOTCH_NAME_PUBLIC (SCOTCH_COARSENNOMERGE)
#define SCOTCH_STRATDEFAULT         SCOTCH_NAME_PUBLIC (SCOTCH_STRATDEFAULT)
#define SCOTCH_STRATQUALITY         SCOTCH_NAME_PUBLIC (SCOTCH_STRATQUALITY)
#define SCOTCH_STRATSPEED           SCOTCH_NAME_PUBLIC (SCOTCH_STRATSPEED)
#define SCOTCH_STRATBALANCE         SCOTCH_NAME_PUBLIC (SCOTCH_STRATBALANCE)
#define SCOTCH_STRATSAFETY          SCOTCH_NAME_PUBLIC (SCOTCH_STRATSAFETY)
#define SCOTCH_STRATSCALABILITY     SCOTCH_NAME_PUBLIC (SCOTCH_STRATSCALABILITY)
#define SCOTCH_STRATRECURSIVE       SCOTCH_NAME_PUBLIC (SCOTCH_STRATRECURSIVE)
#define SCOTCH_STRATREMAP           SCOTCH_NAME_PUBLIC (SCOTCH_STRATREMAP)
#define SCOTCH_STRATLEVELMAX        SCOTCH_NAME_PUBLIC (SCOTCH_STRATLEVELMAX)
#define SCOTCH_STRATLEVELMIN        SCOTCH_NAME_PUBLIC (SCOTCH_STRATLEVELMIN)
#define SCOTCH_STRATLEAFSIMPLE      SCOTCH_NAME_PUBLIC (SCOTCH_STRATLEAFSIMPLE)
#define SCOTCH_STRATSEPASIMPLE      SCOTCH_NAME_PUBLIC (SCOTCH_STRATSEPASIMPLE)
#endif /* SCOTCH_RENAME_ALL */

#ifdef SCOTCH_RENAME
#define SCOTCH_Arch                 SCOTCH_NAME_PUBLIC (SCOTCH_Arch)
#define SCOTCH_Geom                 SCOTCH_NAME_PUBLIC (SCOTCH_Geom)
#define SCOTCH_Graph                SCOTCH_NAME_PUBLIC (SCOTCH_Graph)
#define SCOTCH_Idx                  SCOTCH_NAME_PUBLIC (SCOTCH_Idx)
#define SCOTCH_Mesh                 SCOTCH_NAME_PUBLIC (SCOTCH_Mesh)
#define SCOTCH_Mapping              SCOTCH_NAME_PUBLIC (SCOTCH_Mapping)
#define SCOTCH_Num                  SCOTCH_NAME_PUBLIC (SCOTCH_Num)
#define SCOTCH_Ordering             SCOTCH_NAME_PUBLIC (SCOTCH_Ordering)
#define SCOTCH_Strat                SCOTCH_NAME_PUBLIC (SCOTCH_Strat)
#endif /* SCOTCH_RENAME */

#if ((! defined SCOTCH_COMMON_EXTERNAL) || (defined SCOTCH_COMMON_RENAME))
#define errorPrint                  SCOTCH_NAME_MACRO2 (SCOTCH_, errorPrint) /* Same name whatever the suffix is since external library */
#define errorPrintW                 SCOTCH_NAME_MACRO2 (SCOTCH_, errorPrintW)
#define errorProg                   SCOTCH_NAME_MACRO2 (SCOTCH_, errorProg)

#define memCur                      SCOTCH_NAME_MACRO2 (SCOTCH_, memCur)
#define memMax                      SCOTCH_NAME_MACRO2 (SCOTCH_, memMax)

#define clockGet                    SCOTCH_NAME_INTERN (clockGet)

#define commonStubDummy             SCOTCH_NAME_INTERN (commonStubDummy)

#define fileBlockInit               SCOTCH_NAME_INTERN (fileBlockInit)
#define fileBlockClose              SCOTCH_NAME_INTERN (fileBlockClose)
#define fileBlockOpen               SCOTCH_NAME_INTERN (fileBlockOpen)
#define fileBlockOpenDist           SCOTCH_NAME_INTERN (fileBlockOpenDist)
#define fileCompress                SCOTCH_NAME_INTERN (fileCompress)
#define fileCompressExit            SCOTCH_NAME_INTERN (fileCompressExit)
#define fileCompressType            SCOTCH_NAME_INTERN (fileCompressType)
#define fileDecompress              SCOTCH_NAME_INTERN (fileDecompress)
#define fileDecompressType          SCOTCH_NAME_INTERN (fileDecompressType)
#define fileNameDistExpand          SCOTCH_NAME_INTERN (fileNameDistExpand)

#define intLoad                     SCOTCH_NAME_INTERN (intLoad)
#define intSave                     SCOTCH_NAME_INTERN (intSave)
#define intAscn                     SCOTCH_NAME_INTERN (intAscn)
#define intGcd                      SCOTCH_NAME_INTERN (intGcd)
#define intPerm                     SCOTCH_NAME_INTERN (intPerm)
#define intRandInit                 SCOTCH_NAME_INTERN (intRandInit)
#define intRandLoad                 SCOTCH_NAME_INTERN (intRandLoad)
#define intRandSave                 SCOTCH_NAME_INTERN (intRandSave)
#define intRandProc                 SCOTCH_NAME_INTERN (intRandProc)
#define intRandReset                SCOTCH_NAME_INTERN (intRandReset)
#define intRandSeed                 SCOTCH_NAME_INTERN (intRandSeed)
#ifndef COMMON_RANDOM_SYSTEM
#define intRandVal                  SCOTCH_NAME_INTERN (intRandVal)
#endif /* COMMON_RANDOM_SYSTEM */
#define intSort1asc1                SCOTCH_NAME_INTERN (intSort1asc1)
#define intSort2asc1                SCOTCH_NAME_INTERN (intSort2asc1)
#define intSort2asc2                SCOTCH_NAME_INTERN (intSort2asc2)
#define intSort3asc1                SCOTCH_NAME_INTERN (intSort3asc1)
#define intSort3asc2                SCOTCH_NAME_INTERN (intSort3asc2)

#define memAllocGroup               SCOTCH_NAME_INTERN (memAllocGroup)
#define memAllocRecord              SCOTCH_NAME_INTERN (memAllocRecord)
#define memCheck                    SCOTCH_NAME_INTERN (memCheck)
#define memCheckExists              SCOTCH_NAME_INTERN (memCheckExists)
#define memCheckSize                SCOTCH_NAME_INTERN (memCheckSize)
#define memCheckToggle              SCOTCH_NAME_INTERN (memCheckToggle)
#define memCheckWatch               SCOTCH_NAME_INTERN (memCheckWatch)
#define memFreeRecord               SCOTCH_NAME_INTERN (memFreeRecord)
#define memReallocGroup             SCOTCH_NAME_INTERN (memReallocGroup)
#define memReallocRecord            SCOTCH_NAME_INTERN (memReallocRecord)
#define memOffset                   SCOTCH_NAME_INTERN (memOffset)

#define stringSubst                 SCOTCH_NAME_INTERN (stringSubst)

#define usagePrint                  SCOTCH_NAME_INTERN (usagePrint)
#endif /* ((! defined SCOTCH_COMMON_EXTERNAL) || (defined SCOTCH_COMMON_RENAME)) */

#ifdef SCOTCH_RENAME
#define archInit                    SCOTCH_NAME_INTERN (archInit)
#define archExit                    SCOTCH_NAME_INTERN (archExit)
#define archFree                    SCOTCH_NAME_INTERN (archFree)
#define archLoad                    SCOTCH_NAME_INTERN (archLoad)
#define archSave                    SCOTCH_NAME_INTERN (archSave)
/* #define archName                 SCOTCH_NAME_INTERN (archName) Already a macro */
#define archClass                   SCOTCH_NAME_INTERN (archClass)
#define archClass2                  SCOTCH_NAME_INTERN (archClass2)
#define archClassTab                SCOTCH_NAME_INTERN (archClassTab)
#define archDomLoad                 SCOTCH_NAME_INTERN (archDomLoad)
#define archDomSave                 SCOTCH_NAME_INTERN (archDomSave)
#ifdef SCOTCH_DEBUG_ARCH2                         /* If already redefined */
#define archDomNum                  SCOTCH_NAME_INTERN (archDomNum)
#define archDomDist                 SCOTCH_NAME_INTERN (archDomDist)
#define archDomFrst                 SCOTCH_NAME_INTERN (archDomFrst)
#define archDomIncl                 SCOTCH_NAME_INTERN (archDomIncl)
#define archDomSize                 SCOTCH_NAME_INTERN (archDomSize)
#define archDomTerm                 SCOTCH_NAME_INTERN (archDomTerm)
#define archDomWght                 SCOTCH_NAME_INTERN (archDomWght)
#define archDomBipart               SCOTCH_NAME_INTERN (archDomBipart)
#endif /* SCOTCH_DEBUG_ARCH2 */
#define archDomMpiType              SCOTCH_NAME_INTERN (archDomMpiType)
#define archBuild                   SCOTCH_NAME_INTERN (archBuild)
#define archCmpltArchLoad           SCOTCH_NAME_INTERN (archCmpltArchLoad)
#define archCmpltArchSave           SCOTCH_NAME_INTERN (archCmpltArchSave)
#define archCmpltMatchInit          SCOTCH_NAME_INTERN (archCmpltMatchInit)
#define archCmpltMatchExit          SCOTCH_NAME_INTERN (archCmpltMatchExit)
#define archCmpltMatchMate          SCOTCH_NAME_INTERN (archCmpltMatchMate)
#define archCmpltDomNum             SCOTCH_NAME_INTERN (archCmpltDomNum)
#define archCmpltDomTerm            SCOTCH_NAME_INTERN (archCmpltDomTerm)
#define archCmpltDomSize            SCOTCH_NAME_INTERN (archCmpltDomSize)
/* #define archCmpltDomWght         SCOTCH_NAME_INTERN (archCmpltDomWght) Already a macro */
#define archCmpltDomDist            SCOTCH_NAME_INTERN (archCmpltDomDist)
#define archCmpltDomFrst            SCOTCH_NAME_INTERN (archCmpltDomFrst)
#define archCmpltDomIncl            SCOTCH_NAME_INTERN (archCmpltDomIncl)
#define archCmpltDomLoad            SCOTCH_NAME_INTERN (archCmpltDomLoad)
#define archCmpltDomSave            SCOTCH_NAME_INTERN (archCmpltDomSave)
#define archCmpltDomBipart          SCOTCH_NAME_INTERN (archCmpltDomBipart)
#define archCmpltDomMpiType         SCOTCH_NAME_INTERN (archCmpltDomMpiType)
#define archCmpltwArchBuild         SCOTCH_NAME_INTERN (archCmpltwArchBuild)
#define archCmpltwArchFree          SCOTCH_NAME_INTERN (archCmpltwArchFree)
#define archCmpltwArchLoad          SCOTCH_NAME_INTERN (archCmpltwArchLoad)
#define archCmpltwArchSave          SCOTCH_NAME_INTERN (archCmpltwArchSave)
#define archCmpltwDomNum            SCOTCH_NAME_INTERN (archCmpltwDomNum)
#define archCmpltwDomTerm           SCOTCH_NAME_INTERN (archCmpltwDomTerm)
#define archCmpltwDomSize           SCOTCH_NAME_INTERN (archCmpltwDomSize)
#define archCmpltwDomWght           SCOTCH_NAME_INTERN (archCmpltwDomWght)
#define archCmpltwDomDist           SCOTCH_NAME_INTERN (archCmpltwDomDist)
#define archCmpltwDomFrst           SCOTCH_NAME_INTERN (archCmpltwDomFrst)
#define archCmpltwDomIncl           SCOTCH_NAME_INTERN (archCmpltwDomIncl)
#define archCmpltwDomLoad           SCOTCH_NAME_INTERN (archCmpltwDomLoad)
#define archCmpltwDomSave           SCOTCH_NAME_INTERN (archCmpltwDomSave)
#define archCmpltwDomBipart         SCOTCH_NAME_INTERN (archCmpltwDomBipart)
#define archCmpltwDomMpiType        SCOTCH_NAME_INTERN (archCmpltwDomMpiType)
#define archDecoArchBuild           SCOTCH_NAME_INTERN (archDecoArchBuild)
#define archDecoArchBuild2          SCOTCH_NAME_INTERN (archDecoArchBuild2)
#define archDecoArchFree            SCOTCH_NAME_INTERN (archDecoArchFree)
#define archDecoArchLoad            SCOTCH_NAME_INTERN (archDecoArchLoad)
#define archDecoArchSave            SCOTCH_NAME_INTERN (archDecoArchSave)
#define archDecoDomNum              SCOTCH_NAME_INTERN (archDecoDomNum)
#define archDecoDomTerm             SCOTCH_NAME_INTERN (archDecoDomTerm)
#define archDecoDomSize             SCOTCH_NAME_INTERN (archDecoDomSize)
#define archDecoDomWght             SCOTCH_NAME_INTERN (archDecoDomWght)
#define archDecoDomDist             SCOTCH_NAME_INTERN (archDecoDomDist)
#define archDecoDomFrst             SCOTCH_NAME_INTERN (archDecoDomFrst)
#define archDecoDomIncl             SCOTCH_NAME_INTERN (archDecoDomIncl)
#define archDecoDomLoad             SCOTCH_NAME_INTERN (archDecoDomLoad)
#define archDecoDomSave             SCOTCH_NAME_INTERN (archDecoDomSave)
#define archDecoDomBipart           SCOTCH_NAME_INTERN (archDecoDomBipart)
#define archDecoDomMpiType          SCOTCH_NAME_INTERN (archDecoDomMpiType)
#define archDeco2ArchBuild          SCOTCH_NAME_INTERN (archDeco2ArchBuild)
#define archDeco2ArchFree           SCOTCH_NAME_INTERN (archDeco2ArchFree)
#define archDeco2ArchLoad2          SCOTCH_NAME_INTERN (archDeco2ArchLoad2)
#define archDeco2ArchSave           SCOTCH_NAME_INTERN (archDeco2ArchSave)
#define archDeco2DomNum             SCOTCH_NAME_INTERN (archDeco2DomNum)
#define archDeco2DomTerm            SCOTCH_NAME_INTERN (archDeco2DomTerm)
#define archDeco2DomSize            SCOTCH_NAME_INTERN (archDeco2DomSize)
#define archDeco2DomWght            SCOTCH_NAME_INTERN (archDeco2DomWght)
#define archDeco2DomDist            SCOTCH_NAME_INTERN (archDeco2DomDist)
#define archDeco2DomFrst            SCOTCH_NAME_INTERN (archDeco2DomFrst)
#define archDeco2DomIncl            SCOTCH_NAME_INTERN (archDeco2DomIncl)
#define archDeco2DomLoad            SCOTCH_NAME_INTERN (archDeco2DomLoad)
#define archDeco2DomSave            SCOTCH_NAME_INTERN (archDeco2DomSave)
#define archDeco2DomBipart          SCOTCH_NAME_INTERN (archDeco2DomBipart)
#define archDeco2DomMpiType         SCOTCH_NAME_INTERN (archDeco2DomMpiType)
#define archDeco2MatchInit          SCOTCH_NAME_INTERN (archDeco2MatchInit)
#define archDistArchLoad            SCOTCH_NAME_INTERN (archDistArchLoad)
#define archDistArchSave            SCOTCH_NAME_INTERN (archDistArchSave)
#define archDistArchBuild           SCOTCH_NAME_INTERN (archDistArchBuild)
#define archDistDomNum              SCOTCH_NAME_INTERN (archDistDomNum)
#define archDistDomTerm             SCOTCH_NAME_INTERN (archDistDomTerm)
#define archDistDomSize             SCOTCH_NAME_INTERN (archDistDomSize)
#define archDistDomWght             SCOTCH_NAME_INTERN (archDistDomWght)
#define archDistDomDist             SCOTCH_NAME_INTERN (archDistDomDist)
#define archDistDomFrst             SCOTCH_NAME_INTERN (archDistDomFrst)
#define archDistDomIncl             SCOTCH_NAME_INTERN (archDistDomIncl)
#define archDistDomLoad             SCOTCH_NAME_INTERN (archDistDomLoad)
#define archDistDomSave             SCOTCH_NAME_INTERN (archDistDomSave)
#define archDistDomBipart           SCOTCH_NAME_INTERN (archDistDomBipart)
#define archDistDomMpiType          SCOTCH_NAME_INTERN (archDistDomMpiType)
#define archHcubArchLoad            SCOTCH_NAME_INTERN (archHcubArchLoad)
#define archHcubArchSave            SCOTCH_NAME_INTERN (archHcubArchSave)
#define archHcubMatchInit           SCOTCH_NAME_INTERN (archHcubMatchInit)
#define archHcubMatchExit           SCOTCH_NAME_INTERN (archHcubMatchExit)
#define archHcubMatchMate           SCOTCH_NAME_INTERN (archHcubMatchMate)
#define archHcubDomNum              SCOTCH_NAME_INTERN (archHcubDomNum)
#define archHcubDomTerm             SCOTCH_NAME_INTERN (archHcubDomTerm)
#define archHcubDomSize             SCOTCH_NAME_INTERN (archHcubDomSize)
/* #define archHcubDomWght          SCOTCH_NAME_INTERN (archHcubDomWght) Already a macro */
#define archHcubDomDist             SCOTCH_NAME_INTERN (archHcubDomDist)
#define archHcubDomFrst             SCOTCH_NAME_INTERN (archHcubDomFrst)
#define archHcubDomIncl             SCOTCH_NAME_INTERN (archHcubDomIncl)
#define archHcubDomLoad             SCOTCH_NAME_INTERN (archHcubDomLoad)
#define archHcubDomSave             SCOTCH_NAME_INTERN (archHcubDomSave)
#define archHcubDomBipart           SCOTCH_NAME_INTERN (archHcubDomBipart)
#define archHcubDomMpiType          SCOTCH_NAME_INTERN (archHcubDomMpiType)
#define archLtleafArchLoad          SCOTCH_NAME_INTERN (archLtleafArchLoad)
#define archLtleafArchSave          SCOTCH_NAME_INTERN (archLtleafArchSave)
#define archLtleafDomNum            SCOTCH_NAME_INTERN (archLtleafDomNum)
#define archLtleafDomTerm           SCOTCH_NAME_INTERN (archLtleafDomTerm)
#define archMesh2ArchLoad           SCOTCH_NAME_INTERN (archMesh2ArchLoad)
#define archMesh2ArchSave           SCOTCH_NAME_INTERN (archMesh2ArchSave)
#define archMesh2DomNum             SCOTCH_NAME_INTERN (archMesh2DomNum)
#define archMesh2DomTerm            SCOTCH_NAME_INTERN (archMesh2DomTerm)
#define archMesh2DomSize            SCOTCH_NAME_INTERN (archMesh2DomSize)
/* #define archMesh2DomWght         SCOTCH_NAME_INTERN (archMesh2DomWght) Already a macro */
#define archMesh2DomDist            SCOTCH_NAME_INTERN (archMesh2DomDist)
/* #define archMesh2DomFrst         SCOTCH_NAME_INTERN (archMesh2DomFrst) Already a macro */
#define archMesh2DomIncl            SCOTCH_NAME_INTERN (archMesh2DomIncl)
/* #define archMesh2DomLoad         SCOTCH_NAME_INTERN (archMesh2DomLoad) Already a macro */
/* #define archMesh2DomSave         SCOTCH_NAME_INTERN (archMesh2DomSave) Already a macro */
#define archMesh2DomBipart          SCOTCH_NAME_INTERN (archMesh2DomBipart)
#define archMesh2DomBipartO         SCOTCH_NAME_INTERN (archMesh2DomBipartO)
#define archMesh2DomBipartU         SCOTCH_NAME_INTERN (archMesh2DomBipartU)
#define archMesh2DomMpiType         SCOTCH_NAME_INTERN (archMesh2DomMpiType)
#define archMesh3ArchLoad           SCOTCH_NAME_INTERN (archMesh3ArchLoad)
#define archMesh3ArchSave           SCOTCH_NAME_INTERN (archMesh3ArchSave)
#define archMesh3DomNum             SCOTCH_NAME_INTERN (archMesh3DomNum)
#define archMesh3DomTerm            SCOTCH_NAME_INTERN (archMesh3DomTerm)
#define archMesh3DomSize            SCOTCH_NAME_INTERN (archMesh3DomSize)
/* #define archMesh3DomWght         SCOTCH_NAME_INTERN (archMesh3DomWght) Already a macro */
#define archMesh3DomDist            SCOTCH_NAME_INTERN (archMesh3DomDist)
/* #define archMesh3DomFrst         SCOTCH_NAME_INTERN (archMesh3DomFrst) Already a macro */
#define archMesh3DomIncl            SCOTCH_NAME_INTERN (archMesh3DomIncl)
/* #define archMesh3DomLoad         SCOTCH_NAME_INTERN (archMesh3DomLoad) Already a macro */
/* #define archMesh3DomSave         SCOTCH_NAME_INTERN (archMesh3DomSave) Already a macro */
#define archMesh3DomBipart          SCOTCH_NAME_INTERN (archMesh3DomBipart)
#define archMesh3DomMpiType         SCOTCH_NAME_INTERN (archMesh3DomMpiType)
#define archMeshXArchLoad           SCOTCH_NAME_INTERN (archMeshXArchLoad)
#define archMeshXArchSave           SCOTCH_NAME_INTERN (archMeshXArchSave)
#define archMeshXMatchInit          SCOTCH_NAME_INTERN (archMeshXMatchInit)
#define archMeshXMatchExit          SCOTCH_NAME_INTERN (archMeshXMatchExit)
#define archMeshXMatchMate          SCOTCH_NAME_INTERN (archMeshXMatchMate)
#define archMeshXDomNum             SCOTCH_NAME_INTERN (archMeshXDomNum)
#define archMeshXDomTerm            SCOTCH_NAME_INTERN (archMeshXDomTerm)
#define archMeshXDomSize            SCOTCH_NAME_INTERN (archMeshXDomSize)
/* #define archMeshXDomWght         SCOTCH_NAME_INTERN (archMeshXDomWght) Already a macro */
#define archMeshXDomDist            SCOTCH_NAME_INTERN (archMeshXDomDist)
#define archMeshXDomFrst            SCOTCH_NAME_INTERN (archMeshXDomFrst)
#define archMeshXDomIncl            SCOTCH_NAME_INTERN (archMeshXDomIncl)
#define archMeshXDomLoad            SCOTCH_NAME_INTERN (archMeshXDomLoad)
#define archMeshXDomSave            SCOTCH_NAME_INTERN (archMeshXDomSave)
#define archMeshXDomBipart          SCOTCH_NAME_INTERN (archMeshXDomBipart)
#define archMeshXDomMpiType         SCOTCH_NAME_INTERN (archMeshXDomMpiType)
#define archSubArchLoad             SCOTCH_NAME_INTERN (archSubArchLoad)
#define archSubArchSave             SCOTCH_NAME_INTERN (archSubArchSave)
#define archSubArchFree             SCOTCH_NAME_INTERN (archSubArchFree)
#define archSubArchBuild            SCOTCH_NAME_INTERN (archSubArchBuild)
#define archSubArchBuild2           SCOTCH_NAME_INTERN (archSubArchBuild2)
#define archSubMatchInit            SCOTCH_NAME_INTERN (archSubMatchInit)
#define archSubMatchExit            SCOTCH_NAME_INTERN (archSubMatchExit)
#define archSubMatchMate            SCOTCH_NAME_INTERN (archSubMatchMate)
#define archSubDomNum               SCOTCH_NAME_INTERN (archSubDomNum)
#define archSubDomTerm              SCOTCH_NAME_INTERN (archSubDomTerm)
#define archSubDomSize              SCOTCH_NAME_INTERN (archSubDomSize)
#define archSubDomWght              SCOTCH_NAME_INTERN (archSubDomWght)
#define archSubDomDist              SCOTCH_NAME_INTERN (archSubDomDist)
#define archSubDomFrst              SCOTCH_NAME_INTERN (archSubDomFrst)
#define archSubDomIncl              SCOTCH_NAME_INTERN (archSubDomIncl)
#define archSubDomLoad              SCOTCH_NAME_INTERN (archSubDomLoad)
#define archSubDomSave              SCOTCH_NAME_INTERN (archSubDomSave)
#define archSubDomBipart            SCOTCH_NAME_INTERN (archSubDomBipart)
#define archSubDomMpiType           SCOTCH_NAME_INTERN (archSubDomMpiType)
#define archTermArchLoad            SCOTCH_NAME_INTERN (archTermArchLoad)
#define archTermArchSave            SCOTCH_NAME_INTERN (archTermArchSave)
#define archTermDomNum              SCOTCH_NAME_INTERN (archTermDomNum)
#define archTermDomTerm             SCOTCH_NAME_INTERN (archTermDomTerm)
#define archTermDomSize             SCOTCH_NAME_INTERN (archTermDomSize)
/* #define archTermDomWght          SCOTCH_NAME_INTERN (archTermDomWght) Already a macro */
#define archTermDomDist             SCOTCH_NAME_INTERN (archTermDomDist)
#define archTermDomFrst             SCOTCH_NAME_INTERN (archTermDomFrst)
#define archTermDomIncl             SCOTCH_NAME_INTERN (archTermDomIncl)
#define archTermDomLoad             SCOTCH_NAME_INTERN (archTermDomLoad)
#define archTermDomSave             SCOTCH_NAME_INTERN (archTermDomSave)
#define archTermDomBipart           SCOTCH_NAME_INTERN (archTermDomBipart)
#define archTermDomMpiType          SCOTCH_NAME_INTERN (archTermDomMpiType)
#define archTleafArchLoad           SCOTCH_NAME_INTERN (archTleafArchLoad)
#define archTleafArchFree           SCOTCH_NAME_INTERN (archTleafArchFree)
#define archTleafArchSave           SCOTCH_NAME_INTERN (archTleafArchSave)
#define archTleafMatchInit          SCOTCH_NAME_INTERN (archTleafMatchInit)
#define archTleafMatchExit          SCOTCH_NAME_INTERN (archTleafMatchExit)
#define archTleafMatchMate          SCOTCH_NAME_INTERN (archTleafMatchMate)
#define archTleafDomNum             SCOTCH_NAME_INTERN (archTleafDomNum)
#define archTleafDomTerm            SCOTCH_NAME_INTERN (archTleafDomTerm)
#define archTleafDomSize            SCOTCH_NAME_INTERN (archTleafDomSize)
/* #define archTleafDomWght         SCOTCH_NAME_INTERN (archTleafDomWght) Already a macro */
#define archTleafDomDist            SCOTCH_NAME_INTERN (archTleafDomDist)
#define archTleafDomFrst            SCOTCH_NAME_INTERN (archTleafDomFrst)
#define archTleafDomIncl            SCOTCH_NAME_INTERN (archTleafDomIncl)
#define archTleafDomLoad            SCOTCH_NAME_INTERN (archTleafDomLoad)
#define archTleafDomSave            SCOTCH_NAME_INTERN (archTleafDomSave)
#define archTleafDomBipart          SCOTCH_NAME_INTERN (archTleafDomBipart)
#define archTleafDomMpiType         SCOTCH_NAME_INTERN (archTleafDomMpiType)
/* #define archTorus2ArchLoad       SCOTCH_NAME_INTERN (archTorus2ArchLoad) Already a macro */
/* #define archTorus2ArchSave       SCOTCH_NAME_INTERN (archTorus2ArchSave) Already a macro */
/* #define archTorus2ArchFree       SCOTCH_NAME_INTERN (archTorus2ArchFree) Already a macro */
/* #define archTorus2MatchInit      SCOTCH_NAME_INTERN (archTorus2MatchInit) Already a macro */
/* #define archTorus2MatchExit      SCOTCH_NAME_INTERN (archTorus2MatchExit) Already a macro */
/* #define archTorus2MatchMate      SCOTCH_NAME_INTERN (archTorus2MatchMate) Already a macro */
/* #define archTorus2DomNum         SCOTCH_NAME_INTERN (archTorus2DomNum) Already a macro */
/* #define archTorus2DomTerm        SCOTCH_NAME_INTERN (archTorus2DomTerm) Already a macro */
/* #define archTorus2DomSize        SCOTCH_NAME_INTERN (archTorus2DomSize) Already a macro */
/* #define archTorus2DomWght        SCOTCH_NAME_INTERN (archTorus2DomWght) Already a macro */
#define archTorus2DomDist           SCOTCH_NAME_INTERN (archTorus2DomDist)
/* #define archTorus2DomFrst        SCOTCH_NAME_INTERN (archTorus2DomFrst) Already a macro */
/* #define archTorus2DomIncl        SCOTCH_NAME_INTERN (archTorus2DomIncl) Already a macro */
/* #define archTorus2DomLoad        SCOTCH_NAME_INTERN (archTorus2DomLoad) Already a macro */
/* #define archTorus2DomSave        SCOTCH_NAME_INTERN (archTorus2DomSave) Already a macro */
/* #define archTorus2DomBipart      SCOTCH_NAME_INTERN (archTorus2DomBipart) Already a macro */
/* #define archTorus2DomMpiType     SCOTCH_NAME_INTERN (archTorus2DomMpiType) Already a macro */
/* #define archTorus3ArchLoad       SCOTCH_NAME_INTERN (archTorus3ArchLoad) Already a macro */
/* #define archTorus3ArchSave       SCOTCH_NAME_INTERN (archTorus3ArchSave) Already a macro */
/* #define archTorus3DomNum         SCOTCH_NAME_INTERN (archTorus3DomNum) Already a macro */
/* #define archTorus3DomTerm        SCOTCH_NAME_INTERN (archTorus3DomTerm) Already a macro */
/* #define archTorus3DomSize        SCOTCH_NAME_INTERN (archTorus3DomSize) Already a macro */
/* #define archTorus3DomWght        SCOTCH_NAME_INTERN (archTorus3DomWght) Already a macro */
#define archTorus3DomDist           SCOTCH_NAME_INTERN (archTorus3DomDist)
/* #define archTorus3DomFrst        SCOTCH_NAME_INTERN (archTorus3DomFrst) Already a macro */
/* #define archTorus3DomIncl        SCOTCH_NAME_INTERN (archTorus3DomIncl) Already a macro */
/* #define archTorus3DomLoad        SCOTCH_NAME_INTERN (archTorus3DomLoad) Already a macro */
/* #define archTorus3DomSave        SCOTCH_NAME_INTERN (archTorus3DomSave) Already a macro */
/* #define archTorus3DomBipart      SCOTCH_NAME_INTERN (archTorus3DomBipart) Already a macro */
/* #define archTorus3DomMpiType     SCOTCH_NAME_INTERN (archTorus3DomMpiType) Already a macro */
/* #define archTorusXArchLoad       SCOTCH_NAME_INTERN (archTorusXArchLoad) Already a macro */
/* #define archTorusXArchSave       SCOTCH_NAME_INTERN (archTorusXArchSave) Already a macro */
/* #define archTorusXDomNum         SCOTCH_NAME_INTERN (archTorusXDomNum) Already a macro */
/* #define archTorusXDomTerm        SCOTCH_NAME_INTERN (archTorusXDomTerm) Already a macro */
/* #define archTorusXDomSize        SCOTCH_NAME_INTERN (archTorusXDomSize) Already a macro */
/* #define archTorusXDomWght        SCOTCH_NAME_INTERN (archTorusXDomWght) Already a macro */
#define archTorusXDomDist           SCOTCH_NAME_INTERN (archTorusXDomDist)
/* #define archTorusXDomFrst        SCOTCH_NAME_INTERN (archTorusXDomFrst) Already a macro */
/* #define archTorusXDomIncl        SCOTCH_NAME_INTERN (archTorusXDomIncl) Already a macro */
/* #define archTorusXDomLoad        SCOTCH_NAME_INTERN (archTorusXDomLoad) Already a macro */
/* #define archTorusXDomSave        SCOTCH_NAME_INTERN (archTorusXDomSave) Already a macro */
/* #define archTorusXDomBipart      SCOTCH_NAME_INTERN (archTorusXDomBipart) Already a macro */
/* #define archTorusXDomMpiType     SCOTCH_NAME_INTERN (archTorusXDomMpiType) Already a macro */
/* #define archVcmpltArchLoad       SCOTCH_NAME_INTERN (archVcmpltArchLoad) Already a macro */
/* #define archVcmpltArchSave       SCOTCH_NAME_INTERN (archVcmpltArchSave) Already a macro */
#define archVcmpltDomNum            SCOTCH_NAME_INTERN (archVcmpltDomNum)
#define archVcmpltDomTerm           SCOTCH_NAME_INTERN (archVcmpltDomTerm)
#define archVcmpltDomSize           SCOTCH_NAME_INTERN (archVcmpltDomSize)
/* #define archVcmpltDomWght        SCOTCH_NAME_INTERN (archVcmpltDomWght) Already a macro */
#define archVcmpltDomDist           SCOTCH_NAME_INTERN (archVcmpltDomDist)
#define archVcmpltDomFrst           SCOTCH_NAME_INTERN (archVcmpltDomFrst)
#define archVcmpltDomIncl           SCOTCH_NAME_INTERN (archVcmpltDomIncl)
#define archVcmpltDomBipart         SCOTCH_NAME_INTERN (archVcmpltDomBipart)
#define archVcmpltDomLoad           SCOTCH_NAME_INTERN (archVcmpltDomLoad)
#define archVcmpltDomSave           SCOTCH_NAME_INTERN (archVcmpltDomSave)
#define archVcmpltDomBipart         SCOTCH_NAME_INTERN (archVcmpltDomBipart)
#define archVcmpltDomMpiType        SCOTCH_NAME_INTERN (archVcmpltDomMpiType)
/* #define archVhcubArchLoad        SCOTCH_NAME_INTERN (archVhcubArchLoad) Already a macro */
/* #define archVhcubArchSave        SCOTCH_NAME_INTERN (archVhcubArchSave) Already a macro */
#define archVhcubDomNum             SCOTCH_NAME_INTERN (archVhcubDomNum)
#define archVhcubDomTerm            SCOTCH_NAME_INTERN (archVhcubDomTerm)
#define archVhcubDomSize            SCOTCH_NAME_INTERN (archVhcubDomSize)
/* #define archVhcubDomWght         SCOTCH_NAME_INTERN (archVhcubDomWght) Already a macro */
#define archVhcubDomDist            SCOTCH_NAME_INTERN (archVhcubDomDist)
#define archVhcubDomFrst            SCOTCH_NAME_INTERN (archVhcubDomFrst)
#define archVhcubDomIncl            SCOTCH_NAME_INTERN (archVhcubDomIncl)
#define archVhcubDomLoad            SCOTCH_NAME_INTERN (archVhcubDomLoad)
#define archVhcubDomSave            SCOTCH_NAME_INTERN (archVhcubDomSave)
#define archVhcubDomBipart          SCOTCH_NAME_INTERN (archVhcubDomBipart)
#define archVhcubDomMpiType         SCOTCH_NAME_INTERN (archVhcubDomMpiType)

#define bdgraphInit                 SCOTCH_NAME_INTERN (bdgraphInit)
#define bdgraphInit2                SCOTCH_NAME_INTERN (bdgraphInit2)
#define bdgraphExit                 SCOTCH_NAME_INTERN (bdgraphExit)
#define bdgraphZero                 SCOTCH_NAME_INTERN (bdgraphZero)
#define bdgraphbipartststratab      SCOTCH_NAME_INTERN (bdgraphbipartststratab)
#define bdgraphCheck                SCOTCH_NAME_INTERN (bdgraphCheck)
#define bdgraphGatherAll            SCOTCH_NAME_INTERN (bdgraphGatherAll)
#define bdgraphBipartBd             SCOTCH_NAME_INTERN (bdgraphBipartBd)
#define bdgraphBipartDf             SCOTCH_NAME_INTERN (bdgraphBipartDf)
#define bdgraphBipartEx             SCOTCH_NAME_INTERN (bdgraphBipartEx)
#define bdgraphBipartMl             SCOTCH_NAME_INTERN (bdgraphBipartMl)
#define bdgraphBipartSq             SCOTCH_NAME_INTERN (bdgraphBipartSq)
#define bdgraphBipartSt             SCOTCH_NAME_INTERN (bdgraphBipartSt)
#define bdgraphBipartZr             SCOTCH_NAME_INTERN (bdgraphBipartZr)
#define bdgraphStoreInit            SCOTCH_NAME_INTERN (bdgraphStoreInit)
#define bdgraphStoreExit            SCOTCH_NAME_INTERN (bdgraphStoreExit)
#define bdgraphStoreSave            SCOTCH_NAME_INTERN (bdgraphStoreSave)
#define bdgraphStoreUpdt            SCOTCH_NAME_INTERN (bdgraphStoreUpdt)

#define bgraphbipartststratab       SCOTCH_NAME_INTERN (bgraphbipartststratab)
#define bgraphInit                  SCOTCH_NAME_INTERN (bgraphInit)
#define bgraphInit2                 SCOTCH_NAME_INTERN (bgraphInit2)
#define bgraphInit3                 SCOTCH_NAME_INTERN (bgraphInit3)
#define bgraphInit4                 SCOTCH_NAME_INTERN (bgraphInit4)
#define bgraphInit5                 SCOTCH_NAME_INTERN (bgraphInit5)
#define bgraphExit                  SCOTCH_NAME_INTERN (bgraphExit)
#define bgraphCheck                 SCOTCH_NAME_INTERN (bgraphCheck)
#define bgraphSwal                  SCOTCH_NAME_INTERN (bgraphSwal)
#define bgraphZero                  SCOTCH_NAME_INTERN (bgraphZero)
#define bgraphBipartBd              SCOTCH_NAME_INTERN (bgraphBipartBd)
#define bgraphBipartDf              SCOTCH_NAME_INTERN (bgraphBipartDf)
#define bgraphBipartDf2             SCOTCH_NAME_INTERN (bgraphBipartDf2)
#define bgraphBipartDfJoin          SCOTCH_NAME_INTERN (bgraphBipartDfJoin)
#define bgraphBipartEx              SCOTCH_NAME_INTERN (bgraphBipartEx)
#define bgraphBipartFm              SCOTCH_NAME_INTERN (bgraphBipartFm)
#define bgraphBipartGg              SCOTCH_NAME_INTERN (bgraphBipartGg)
#define bgraphBipartGp              SCOTCH_NAME_INTERN (bgraphBipartGp)
#define bgraphBipartMl              SCOTCH_NAME_INTERN (bgraphBipartMl)
#define bgraphBipartSt              SCOTCH_NAME_INTERN (bgraphBipartSt)
#define bgraphBipartZr              SCOTCH_NAME_INTERN (bgraphBipartZr)
#define bgraphStoreInit             SCOTCH_NAME_INTERN (bgraphStoreInit)
#define bgraphStoreExit             SCOTCH_NAME_INTERN (bgraphStoreExit)
#define bgraphStoreSave             SCOTCH_NAME_INTERN (bgraphStoreSave)
#define bgraphStoreUpdt             SCOTCH_NAME_INTERN (bgraphStoreUpdt)

#if ((defined INTSIZE64) || (defined COMM))
#define commAllgatherv              SCOTCH_NAME_INTERN (commAllgatherv)
#define commGatherv                 SCOTCH_NAME_INTERN (commGatherv)
#define commScatterv                SCOTCH_NAME_INTERN (commScatterv)
#endif /* ((defined INTSIZE64) || (defined COMM)) */

#define dgraphAllreduceMaxSum2      SCOTCH_NAME_INTERN (dgraphAllreduceMaxSum2)
#define dgraphBuild                 SCOTCH_NAME_INTERN (dgraphBuild)
#define dgraphBuild2                SCOTCH_NAME_INTERN (dgraphBuild2)
#define dgraphBuild3                SCOTCH_NAME_INTERN (dgraphBuild3)
#define dgraphBuild4                SCOTCH_NAME_INTERN (dgraphBuild4)
#define dgraphBuildGrid3D           SCOTCH_NAME_INTERN (dgraphBuildGrid3D)
#define dgraphBuildHcub             SCOTCH_NAME_INTERN (dgraphBuildHcub)
#define dgraphCheck                 SCOTCH_NAME_INTERN (dgraphCheck)
#define dgraphBand                  SCOTCH_NAME_INTERN (dgraphBand)
#define dgraphBand2Coll             SCOTCH_NAME_INTERN (dgraphBand2Coll)
#define dgraphBand2Ptop             SCOTCH_NAME_INTERN (dgraphBand2Ptop)
#define dgraphCoarsen               SCOTCH_NAME_INTERN (dgraphCoarsen)
#define dgraphExit                  SCOTCH_NAME_INTERN (dgraphExit)
#define dgraphFold                  SCOTCH_NAME_INTERN (dgraphFold)
#define dgraphFold2                 SCOTCH_NAME_INTERN (dgraphFold2)
#define dgraphFoldComm              SCOTCH_NAME_INTERN (dgraphFoldComm)
#define dgraphFoldDup               SCOTCH_NAME_INTERN (dgraphFoldDup)
#define dgraphFree                  SCOTCH_NAME_INTERN (dgraphFree)
#define dgraphGather                SCOTCH_NAME_INTERN (dgraphGather)
#define dgraphGatherAll             SCOTCH_NAME_INTERN (dgraphGatherAll)
#define dgraphGatherAll2            SCOTCH_NAME_INTERN (dgraphGatherAll2)
/* #define dgraphGhst               SCOTCH_NAME_INTERN (dgraphGhst) Already a macro        */
/* #define dgraphGhstReplace        SCOTCH_NAME_INTERN (dgraphGhstReplace) Already a macro */
#define dgraphGhst2                 SCOTCH_NAME_INTERN (dgraphGhst2)
#define dgraphGrow                  SCOTCH_NAME_INTERN (dgraphGrow) /* Used before macro replacement */
#define dgraphGrow2Coll             SCOTCH_NAME_INTERN (dgraphGrow2Coll)
#define dgraphGrow2Ptop             SCOTCH_NAME_INTERN (dgraphGrow2Ptop)
#define dgraphHaloSync              SCOTCH_NAME_INTERN (dgraphHaloSync)
#define dgraphHaloAsync             SCOTCH_NAME_INTERN (dgraphHaloAsync)
#define dgraphHaloWait              SCOTCH_NAME_INTERN (dgraphHaloWait)
#define dgraphHaloCheck             SCOTCH_NAME_INTERN (dgraphHaloCheck)
#define dgraphInduceList            SCOTCH_NAME_INTERN (dgraphInduceList)
#define dgraphInducePart            SCOTCH_NAME_INTERN (dgraphInducePart)
#define dgraphInducePart2           SCOTCH_NAME_INTERN (dgraphInducePart2)
#define dgraphInduce2               SCOTCH_NAME_INTERN (dgraphInduce2)
#define dgraphInit                  SCOTCH_NAME_INTERN (dgraphInit)
#define dgraphLoad                  SCOTCH_NAME_INTERN (dgraphLoad)
#define dgraphMatchInit             SCOTCH_NAME_INTERN (dgraphMatchInit)
#define dgraphMatchExit             SCOTCH_NAME_INTERN (dgraphMatchExit)
#define dgraphMatchSync             SCOTCH_NAME_INTERN (dgraphMatchSync)
#define dgraphMatchSyncColl         SCOTCH_NAME_INTERN (dgraphMatchSyncColl)
#define dgraphMatchSyncPtop         SCOTCH_NAME_INTERN (dgraphMatchSyncPtop)
#define dgraphMatchCheck            SCOTCH_NAME_INTERN (dgraphMatchCheck)
#define dgraphMatchHl               SCOTCH_NAME_INTERN (dgraphMatchHl)
#define dgraphMatchHy               SCOTCH_NAME_INTERN (dgraphMatchHy)
#define dgraphMatchLc               SCOTCH_NAME_INTERN (dgraphMatchLc)
#define dgraphMatchLy               SCOTCH_NAME_INTERN (dgraphMatchLy)
#define dgraphMatchSc               SCOTCH_NAME_INTERN (dgraphMatchSc)
#define dgraphRedist                SCOTCH_NAME_INTERN (dgraphRedist)
#define dgraphSave                  SCOTCH_NAME_INTERN (dgraphSave)
#define dgraphScatter               SCOTCH_NAME_INTERN (dgraphScatter)
#define dgraphView                  SCOTCH_NAME_INTERN (dgraphView)

#define dmapInit                    SCOTCH_NAME_INTERN (dmapInit)
#define dmapExit                    SCOTCH_NAME_INTERN (dmapExit)
#define dmapAdd                     SCOTCH_NAME_INTERN (dmapAdd)
#define dmapTerm                    SCOTCH_NAME_INTERN (dmapTerm)
#define dmapSave                    SCOTCH_NAME_INTERN (dmapSave)

#define dorderDispose               SCOTCH_NAME_INTERN (dorderDispose)
#define dorderExit                  SCOTCH_NAME_INTERN (dorderExit)
#define dorderFree                  SCOTCH_NAME_INTERN (dorderFree)
#define dorderFrst                  SCOTCH_NAME_INTERN (dorderFrst)
#define dorderGather                SCOTCH_NAME_INTERN (dorderGather)
#define dorderGatherTree            SCOTCH_NAME_INTERN (dorderGatherTree)
#define dorderInit                  SCOTCH_NAME_INTERN (dorderInit)
#define dorderNew                   SCOTCH_NAME_INTERN (dorderNew)
#define dorderNewSequ               SCOTCH_NAME_INTERN (dorderNewSequ)
#define dorderNewSequIndex          SCOTCH_NAME_INTERN (dorderNewSequIndex)
#define dorderPerm                  SCOTCH_NAME_INTERN (dorderPerm)
#define dorderSave                  SCOTCH_NAME_INTERN (dorderSave)
#define dorderSaveBlock             SCOTCH_NAME_INTERN (dorderSaveBlock)
#define dorderSaveMap               SCOTCH_NAME_INTERN (dorderSaveMap)
#define dorderSaveTree              SCOTCH_NAME_INTERN (dorderSaveTree)
#define dorderSaveTree2             SCOTCH_NAME_INTERN (dorderSaveTree2)
#define dorderCblkDist              SCOTCH_NAME_INTERN (dorderCblkDist)
#define dorderTreeDist              SCOTCH_NAME_INTERN (dorderTreeDist)

#define fiboHeapCheck               SCOTCH_NAME_INTERN (fiboHeapCheck)
#define fiboHeapConsolidate         SCOTCH_NAME_INTERN (fiboHeapConsolidate)
/* #define fiboHeapAdd              SCOTCH_NAME_INTERN (fiboHeapAdd) Already a macro */
#define fiboHeapDecrease            SCOTCH_NAME_INTERN (fiboHeapDecrease)
#define fiboHeapDel                 SCOTCH_NAME_INTERN (fiboHeapDel)
#define fiboHeapExit                SCOTCH_NAME_INTERN (fiboHeapExit)
#define fiboHeapFree                SCOTCH_NAME_INTERN (fiboHeapFree)
#define fiboHeapInit                SCOTCH_NAME_INTERN (fiboHeapInit)
/* #define fiboHeapMin              SCOTCH_NAME_INTERN (fiboHeapMin) Already a macro */

#define gainTablAddLin              SCOTCH_NAME_INTERN (gainTablAddLin)
#define gainTablAddLog              SCOTCH_NAME_INTERN (gainTablAddLog)
#define gainTablCheck               SCOTCH_NAME_INTERN (gainTablCheck)
#ifdef SCOTCH_DEBUG_GAIN1                         /* If not already redefined as accelerated macro */
#define gainTablDel                 SCOTCH_NAME_INTERN (gainTablDel)
#endif /* SCOTCH_DEBUG_GAIN1 */
#define gainTablExit                SCOTCH_NAME_INTERN (gainTablExit)
#define gainTablFree                SCOTCH_NAME_INTERN (gainTablFree)
#define gainTablFrst                SCOTCH_NAME_INTERN (gainTablFrst)
#define gainTablInit                SCOTCH_NAME_INTERN (gainTablInit)
#define gainTablNext                SCOTCH_NAME_INTERN (gainTablNext)
#define gainTablMove                SCOTCH_NAME_INTERN (gainTablMove)

#define geomExit                    SCOTCH_NAME_INTERN (geomExit)
#define geomInit                    SCOTCH_NAME_INTERN (geomInit)

#define graphInit                   SCOTCH_NAME_INTERN (graphInit)
#define graphExit                   SCOTCH_NAME_INTERN (graphExit)
#define graphFree                   SCOTCH_NAME_INTERN (graphFree)
#define graphLoad                   SCOTCH_NAME_INTERN (graphLoad)
#define graphLoad2                  SCOTCH_NAME_INTERN (graphLoad2)
#define graphSave                   SCOTCH_NAME_INTERN (graphSave)
#define graphBand                   SCOTCH_NAME_INTERN (graphBand)
#define graphBase                   SCOTCH_NAME_INTERN (graphBase)
#define graphCheck                  SCOTCH_NAME_INTERN (graphCheck)
#define graphClone                  SCOTCH_NAME_INTERN (graphClone)
#define graphCoarsen                SCOTCH_NAME_INTERN (graphCoarsen)
#define graphCoarsenBuild           SCOTCH_NAME_INTERN (graphCoarsenBuild)
#define graphCoarsenMatch           SCOTCH_NAME_INTERN (graphCoarsenMatch)
#define graphDiamPV                 SCOTCH_NAME_INTERN (graphDiamPV)
#define graphDump                   SCOTCH_NAME_INTERN (graphDump)
#define graphDump2                  SCOTCH_NAME_INTERN (graphDump2)
#define graphDumpArray              SCOTCH_NAME_INTERN (graphDumpArray)
#define graphDumpArrays             SCOTCH_NAME_INTERN (graphDumpArrays)
#define graphIelo                   SCOTCH_NAME_INTERN (graphIelo)
#define graphInduceList             SCOTCH_NAME_INTERN (graphInduceList)
#define graphInducePart             SCOTCH_NAME_INTERN (graphInducePart)
#define graphMatch                  SCOTCH_NAME_INTERN (graphMatch)
#define graphMatchInit              SCOTCH_NAME_INTERN (graphMatchInit)
#define graphMatchNone              SCOTCH_NAME_INTERN (graphMatchNone)
#define graphGeomLoadChac           SCOTCH_NAME_INTERN (graphGeomLoadChac)
#define graphGeomLoadHabo           SCOTCH_NAME_INTERN (graphGeomLoadHabo)
#define graphGeomLoadMmkt           SCOTCH_NAME_INTERN (graphGeomLoadMmkt)
#define graphGeomLoadScot           SCOTCH_NAME_INTERN (graphGeomLoadScot)
#define graphGeomSaveChac           SCOTCH_NAME_INTERN (graphGeomSaveChac)
#define graphGeomSaveScot           SCOTCH_NAME_INTERN (graphGeomSaveScot)
#define graphGeomSaveMmkt           SCOTCH_NAME_INTERN (graphGeomSaveMmkt)
#define graphPtscotch               SCOTCH_NAME_INTERN (graphPtscotch)

#define hallOrderHdR2Halmd          SCOTCH_NAME_INTERN (hallOrderHdR2Halmd)
#define hallOrderHfR3Hamdf4         SCOTCH_NAME_INTERN (hallOrderHfR3Hamdf4)
#define hallOrderHxBuild            SCOTCH_NAME_INTERN (hallOrderHxBuild)
#define hallOrderHxTree             SCOTCH_NAME_INTERN (hallOrderHxTree)

#define hdgraphorderststratab       SCOTCH_NAME_INTERN (hdgraphorderststratab)
#define hdgraphInit                 SCOTCH_NAME_INTERN (hdgraphInit)
#define hdgraphExit                 SCOTCH_NAME_INTERN (hdgraphExit)
#define hdgraphCheck                SCOTCH_NAME_INTERN (hdgraphCheck)
#define hdgraphFold                 SCOTCH_NAME_INTERN (hdgraphFold)
#define hdgraphFold2                SCOTCH_NAME_INTERN (hdgraphFold2)
#define hdgraphGather               SCOTCH_NAME_INTERN (hdgraphGather)
#define hdgraphInduceList           SCOTCH_NAME_INTERN (hdgraphInduceList)
#define hdgraphOrderNd              SCOTCH_NAME_INTERN (hdgraphOrderNd)
#define hdgraphOrderNd2             SCOTCH_NAME_INTERN (hdgraphOrderNd2)
#define hdgraphOrderSi              SCOTCH_NAME_INTERN (hdgraphOrderSi)
#define hdgraphOrderSq              SCOTCH_NAME_INTERN (hdgraphOrderSq)
#define hdgraphOrderSq2             SCOTCH_NAME_INTERN (hdgraphOrderSq2)
#define hdgraphOrderSt              SCOTCH_NAME_INTERN (hdgraphOrderSt)

#define hgraphorderststratab        SCOTCH_NAME_INTERN (hgraphorderststratab)
#define hgraphInit                  SCOTCH_NAME_INTERN (hgraphInit)
#define hgraphExit                  SCOTCH_NAME_INTERN (hgraphExit)
#define hgraphFree                  SCOTCH_NAME_INTERN (hgraphFree)
#define hgraphInduceList            SCOTCH_NAME_INTERN (hgraphInduceList)
#define hgraphCheck                 SCOTCH_NAME_INTERN (hgraphCheck)
#define hgraphDump                  SCOTCH_NAME_INTERN (hgraphDump)
#define hgraphOrderBl               SCOTCH_NAME_INTERN (hgraphOrderBl)
#define hgraphOrderCc               SCOTCH_NAME_INTERN (hgraphOrderCc)
#define hgraphOrderCp               SCOTCH_NAME_INTERN (hgraphOrderCp)
#define hgraphOrderGp               SCOTCH_NAME_INTERN (hgraphOrderGp)
#define hgraphOrderHd               SCOTCH_NAME_INTERN (hgraphOrderHd)
#define hgraphOrderHf               SCOTCH_NAME_INTERN (hgraphOrderHf)
#define hgraphOrderHxFill           SCOTCH_NAME_INTERN (hgraphOrderHxFill)
#define hgraphOrderKp               SCOTCH_NAME_INTERN (hgraphOrderKp)
#define hgraphOrderNd               SCOTCH_NAME_INTERN (hgraphOrderNd)
#define hgraphOrderSi               SCOTCH_NAME_INTERN (hgraphOrderSi)
#define hgraphOrderSt               SCOTCH_NAME_INTERN (hgraphOrderSt)
#define hgraphUnhalo                SCOTCH_NAME_INTERN (hgraphUnhalo)

#define hmeshorderststratab         SCOTCH_NAME_INTERN (hmeshorderststratab)
#define hmeshExit                   SCOTCH_NAME_INTERN (hmeshExit)
#define hmeshBase                   SCOTCH_NAME_INTERN (hmeshBase)
#define hmeshCheck                  SCOTCH_NAME_INTERN (hmeshCheck)
#define hmeshInducePart             SCOTCH_NAME_INTERN (hmeshInducePart)
#define hmeshHgraph                 SCOTCH_NAME_INTERN (hmeshHgraph)
#define hmeshMesh                   SCOTCH_NAME_INTERN (hmeshMesh)
#define hmeshOrderBl                SCOTCH_NAME_INTERN (hmeshOrderBl)
#define hmeshOrderCp                SCOTCH_NAME_INTERN (hmeshOrderCp)
#define hmeshOrderGp                SCOTCH_NAME_INTERN (hmeshOrderGp)
#define hmeshOrderGr                SCOTCH_NAME_INTERN (hmeshOrderGr)
#define hmeshOrderHd                SCOTCH_NAME_INTERN (hmeshOrderHd)
#define hmeshOrderHf                SCOTCH_NAME_INTERN (hmeshOrderHf)
#define hmeshOrderHxFill            SCOTCH_NAME_INTERN (hmeshOrderHxFill)
#define hmeshOrderNd                SCOTCH_NAME_INTERN (hmeshOrderNd)
#define hmeshOrderSi                SCOTCH_NAME_INTERN (hmeshOrderSi)
#define hmeshOrderSt                SCOTCH_NAME_INTERN (hmeshOrderSt)

#define kdgraphmapststratab         SCOTCH_NAME_INTERN (kdgraphmapststratab)
#define kdgraphInit                 SCOTCH_NAME_INTERN (kdgraphInit)
#define kdgraphExit                 SCOTCH_NAME_INTERN (kdgraphExit)
#define kdgraphGather               SCOTCH_NAME_INTERN (kdgraphGather)
#define kdgraphMapRb                SCOTCH_NAME_INTERN (kdgraphMapRb)
#define kdgraphMapRbAdd2            SCOTCH_NAME_INTERN (kdgraphMapRbAdd2)
#define kdgraphMapRbAddBoth         SCOTCH_NAME_INTERN (kdgraphMapRbAddBoth)
#define kdgraphMapRbAddOne          SCOTCH_NAME_INTERN (kdgraphMapRbAddOne)
#define kdgraphMapRbAddPart         SCOTCH_NAME_INTERN (kdgraphMapRbAddPart)
#define kdgraphMapRbMap             SCOTCH_NAME_INTERN (kdgraphMapRbMap)
#define kdgraphMapRbPart            SCOTCH_NAME_INTERN (kdgraphMapRbPart)
#define kdgraphMapSt                SCOTCH_NAME_INTERN (kdgraphMapSt)

#define kgraphmapststratab          SCOTCH_NAME_INTERN (kgraphmapststratab)
#define kgraphInit                  SCOTCH_NAME_INTERN (kgraphInit)
#define kgraphExit                  SCOTCH_NAME_INTERN (kgraphExit)
#define kgraphCheck                 SCOTCH_NAME_INTERN (kgraphCheck)
#define kgraphBand                  SCOTCH_NAME_INTERN (kgraphBand)
#define kgraphCost                  SCOTCH_NAME_INTERN (kgraphCost)
#define kgraphFron                  SCOTCH_NAME_INTERN (kgraphFron)
#define kgraphFrst                  SCOTCH_NAME_INTERN (kgraphFrst)
#define kgraphMapBd                 SCOTCH_NAME_INTERN (kgraphMapBd)
#define kgraphMapCp                 SCOTCH_NAME_INTERN (kgraphMapCp)
#define kgraphMapDf                 SCOTCH_NAME_INTERN (kgraphMapDf)
#define kgraphMapEx                 SCOTCH_NAME_INTERN (kgraphMapEx)
#define kgraphMapFm                 SCOTCH_NAME_INTERN (kgraphMapFm)
#define kgraphMapMl                 SCOTCH_NAME_INTERN (kgraphMapMl)
#define kgraphMapRb                 SCOTCH_NAME_INTERN (kgraphMapRb)
#define kgraphMapRbMap              SCOTCH_NAME_INTERN (kgraphMapRbMap)
#define kgraphMapRbBgraph           SCOTCH_NAME_INTERN (kgraphMapRbBgraph)
#define kgraphMapRbPart             SCOTCH_NAME_INTERN (kgraphMapRbPart)
#define kgraphMapRbVfloBuild        SCOTCH_NAME_INTERN (kgraphMapRbVfloBuild)
#define kgraphMapRbVfloMerge        SCOTCH_NAME_INTERN (kgraphMapRbVfloMerge)
#define kgraphMapRbVfloSplit        SCOTCH_NAME_INTERN (kgraphMapRbVfloSplit)
#define kgraphMapSt                 SCOTCH_NAME_INTERN (kgraphMapSt)
#define kgraphStoreInit             SCOTCH_NAME_INTERN (kgraphStoreInit)
#define kgraphStoreExit             SCOTCH_NAME_INTERN (kgraphStoreExit)
#define kgraphStoreSave             SCOTCH_NAME_INTERN (kgraphStoreSave)
#define kgraphStoreUpdt             SCOTCH_NAME_INTERN (kgraphStoreUpdt)

#define listInit                    SCOTCH_NAME_INTERN (listInit)
#define listExit                    SCOTCH_NAME_INTERN (listExit)
#define listAlloc                   SCOTCH_NAME_INTERN (listAlloc)
#define listFree                    SCOTCH_NAME_INTERN (listFree)
#define listLoad                    SCOTCH_NAME_INTERN (listLoad)
#define listSave                    SCOTCH_NAME_INTERN (listSave)
#define listSort                    SCOTCH_NAME_INTERN (listSort)
#define listCopy                    SCOTCH_NAME_INTERN (listCopy)

#define mapInit                     SCOTCH_NAME_INTERN (mapInit)
#define mapInit2                    SCOTCH_NAME_INTERN (mapInit2)
#define mapExit                     SCOTCH_NAME_INTERN (mapExit)
#define mapAlloc                    SCOTCH_NAME_INTERN (mapAlloc)
#define mapBuild                    SCOTCH_NAME_INTERN (mapBuild)
#define mapCopy                     SCOTCH_NAME_INTERN (mapCopy)
#define mapFree                     SCOTCH_NAME_INTERN (mapFree)
#define mapFrst                     SCOTCH_NAME_INTERN (mapFrst)
#define mapLoad                     SCOTCH_NAME_INTERN (mapLoad)
#define mapMerge                    SCOTCH_NAME_INTERN (mapMerge)
#define mapResize                   SCOTCH_NAME_INTERN (mapResize)
#define mapResize2                  SCOTCH_NAME_INTERN (mapResize2)
#define mapSave                     SCOTCH_NAME_INTERN (mapSave)
#define mapTerm                     SCOTCH_NAME_INTERN (mapTerm)

#define meshInit                    SCOTCH_NAME_INTERN (meshInit)
#define meshExit                    SCOTCH_NAME_INTERN (meshExit)
#define meshFree                    SCOTCH_NAME_INTERN (meshFree)
#define meshLoad                    SCOTCH_NAME_INTERN (meshLoad)
#define meshSave                    SCOTCH_NAME_INTERN (meshSave)
#define meshBase                    SCOTCH_NAME_INTERN (meshBase)
#define meshGraph                   SCOTCH_NAME_INTERN (meshGraph)
#define meshCoarsen                 SCOTCH_NAME_INTERN (meshCoarsen)
#define meshInduceList              SCOTCH_NAME_INTERN (meshInduceList)
#define meshInducePart              SCOTCH_NAME_INTERN (meshInducePart)
#define meshInduceSepa              SCOTCH_NAME_INTERN (meshInduceSepa)
#define meshCheck                   SCOTCH_NAME_INTERN (meshCheck)
#define meshGeomLoadHabo            SCOTCH_NAME_INTERN (meshGeomLoadHabo)
#define meshGeomLoadScot            SCOTCH_NAME_INTERN (meshGeomLoadScot)
#define meshGeomSaveScot            SCOTCH_NAME_INTERN (meshGeomSaveScot)

#define orderInit                   SCOTCH_NAME_INTERN (orderInit)
#define orderExit                   SCOTCH_NAME_INTERN (orderExit)
#define orderLoad                   SCOTCH_NAME_INTERN (orderLoad)
#define orderSave                   SCOTCH_NAME_INTERN (orderSave)
#define orderSaveMap                SCOTCH_NAME_INTERN (orderSaveMap)
#define orderSaveTree               SCOTCH_NAME_INTERN (orderSaveTree)
#define orderCheck                  SCOTCH_NAME_INTERN (orderCheck)
#define orderPeri                   SCOTCH_NAME_INTERN (orderPeri)
#define orderRang                   SCOTCH_NAME_INTERN (orderRang)
#define orderTree                   SCOTCH_NAME_INTERN (orderTree)

#define parsermethtokentab          SCOTCH_NAME_INTERN (parsermethtokentab)
#define parserparamcurr             SCOTCH_NAME_INTERN (parserparamcurr)
#define parserstratcurr             SCOTCH_NAME_INTERN (parserstratcurr)
#define parserstrattab              SCOTCH_NAME_INTERN (parserstrattab)

#define stratdummy                  SCOTCH_NAME_INTERN (stratdummy)
#define stratInit                   SCOTCH_NAME_INTERN (stratInit)
#define stratExit                   SCOTCH_NAME_INTERN (stratExit)
#define stratSave                   SCOTCH_NAME_INTERN (stratSave)
#define stratCondEval               SCOTCH_NAME_INTERN (stratCondEval)
#define stratCondExit               SCOTCH_NAME_INTERN (stratCondExit)
#define stratCondSave               SCOTCH_NAME_INTERN (stratCondSave)
#define stratParserInit             SCOTCH_NAME_INTERN (stratParserInit)
#define stratParserInput            SCOTCH_NAME_INTERN (stratParserInput)
#define stratParserLex              SCOTCH_NAME_INTERN (stratParserLex)
#define stratParserRemain           SCOTCH_NAME_INTERN (stratParserRemain)
#define stratParserSelect           SCOTCH_NAME_INTERN (stratParserSelect)
#define stratParserParse            SCOTCH_NAME_INTERN (stratParserParse)
#define stratParserParse2           SCOTCH_NAME_INTERN (stratParserParse2)
#define stratTestEval               SCOTCH_NAME_INTERN (stratTestEval)
#define stratTestExit               SCOTCH_NAME_INTERN (stratTestExit)
#define stratTestSave               SCOTCH_NAME_INTERN (stratTestSave)

#define threadLaunch                SCOTCH_NAME_INTERN (threadLaunch)
#define threadReduce                SCOTCH_NAME_INTERN (threadReduce)
#define threadScan                  SCOTCH_NAME_INTERN (threadScan)

#define vdgraphseparateststratab    SCOTCH_NAME_INTERN (vdgraphseparateststratab)
#define vdgraphCheck                SCOTCH_NAME_INTERN (vdgraphCheck)
#define vdgraphExit                 SCOTCH_NAME_INTERN (vdgraphExit)
#define vdgraphGatherAll            SCOTCH_NAME_INTERN (vdgraphGatherAll)
#define vdgraphInit                 SCOTCH_NAME_INTERN (vdgraphInit)
#define vdgraphSeparateBd           SCOTCH_NAME_INTERN (vdgraphSeparateBd)
#define vdgraphSeparateDf           SCOTCH_NAME_INTERN (vdgraphSeparateDf)
#define vdgraphSeparateMl           SCOTCH_NAME_INTERN (vdgraphSeparateMl)
#define vdgraphSeparateSq           SCOTCH_NAME_INTERN (vdgraphSeparateSq)
#define vdgraphSeparateSt           SCOTCH_NAME_INTERN (vdgraphSeparateSt)
#define vdgraphSeparateZr           SCOTCH_NAME_INTERN (vdgraphSeparateZr)
#define vdgraphStoreExit            SCOTCH_NAME_INTERN (vdgraphStoreExit)
#define vdgraphStoreInit            SCOTCH_NAME_INTERN (vdgraphStoreInit)
#define vdgraphStoreSave            SCOTCH_NAME_INTERN (vdgraphStoreSave)
#define vdgraphStoreUpdt            SCOTCH_NAME_INTERN (vdgraphStoreUpdt)
#define vdgraphZero                 SCOTCH_NAME_INTERN (vdgraphZero)

#define vgraphseparateststratab     SCOTCH_NAME_INTERN (vgraphseparateststratab)
#define vgraphInit                  SCOTCH_NAME_INTERN (vgraphInit)
#define vgraphExit                  SCOTCH_NAME_INTERN (vgraphExit)
#define vgraphCheck                 SCOTCH_NAME_INTERN (vgraphCheck)
#define vgraphZero                  SCOTCH_NAME_INTERN (vgraphZero)
#define vgraphSeparateBd            SCOTCH_NAME_INTERN (vgraphSeparateBd)
#define vgraphSeparateDf            SCOTCH_NAME_INTERN (vgraphSeparateDf)
#define vgraphSeparateEs            SCOTCH_NAME_INTERN (vgraphSeparateEs)
#define vgraphSeparateFm            SCOTCH_NAME_INTERN (vgraphSeparateFm)
#define vgraphSeparateGg            SCOTCH_NAME_INTERN (vgraphSeparateGg)
#define vgraphSeparateGp            SCOTCH_NAME_INTERN (vgraphSeparateGp)
#define vgraphSeparateMl            SCOTCH_NAME_INTERN (vgraphSeparateMl)
#define vgraphSeparateMt            SCOTCH_NAME_INTERN (vgraphSeparateMt)
#define vgraphSeparateSt            SCOTCH_NAME_INTERN (vgraphSeparateSt)
#define vgraphSeparateTh            SCOTCH_NAME_INTERN (vgraphSeparateTh)
#define vgraphSeparateVw            SCOTCH_NAME_INTERN (vgraphSeparateVw)
#define vgraphSeparateZr            SCOTCH_NAME_INTERN (vgraphSeparateZr)
#define vgraphStoreInit             SCOTCH_NAME_INTERN (vgraphStoreInit)
#define vgraphStoreExit             SCOTCH_NAME_INTERN (vgraphStoreExit)
#define vgraphStoreSave             SCOTCH_NAME_INTERN (vgraphStoreSave)
#define vgraphStoreUpdt             SCOTCH_NAME_INTERN (vgraphStoreUpdt)

#define vmeshseparateststratab      SCOTCH_NAME_INTERN (vmeshseparateststratab)
#define vmeshExit                   SCOTCH_NAME_INTERN (vmeshExit)
#define vmeshCheck                  SCOTCH_NAME_INTERN (vmeshCheck)
#define vmeshZero                   SCOTCH_NAME_INTERN (vmeshZero)
#define vmeshSeparateFm             SCOTCH_NAME_INTERN (vmeshSeparateFm)
#define vmeshSeparateGg             SCOTCH_NAME_INTERN (vmeshSeparateGg)
#define vmeshSeparateGr             SCOTCH_NAME_INTERN (vmeshSeparateGr)
#define vmeshSeparateMl             SCOTCH_NAME_INTERN (vmeshSeparateMl)
#define vmeshSeparateSt             SCOTCH_NAME_INTERN (vmeshSeparateSt)
#define vmeshSeparateZr             SCOTCH_NAME_INTERN (vmeshSeparateZr)
#define vmeshStoreInit              SCOTCH_NAME_INTERN (vmeshStoreInit)
#define vmeshStoreExit              SCOTCH_NAME_INTERN (vmeshStoreExit)
#define vmeshStoreSave              SCOTCH_NAME_INTERN (vmeshStoreSave)
#define vmeshStoreUpdt              SCOTCH_NAME_INTERN (vmeshStoreUpdt)

#define wgraphpartststratab         SCOTCH_NAME_INTERN (wgraphpartststratab)
#define wgraphAlloc                 SCOTCH_NAME_INTERN (wgraphAlloc)
#define wgraphInit                  SCOTCH_NAME_INTERN (wgraphInit)
#define wgraphExit                  SCOTCH_NAME_INTERN (wgraphExit)
#define wgraphCheck                 SCOTCH_NAME_INTERN (wgraphCheck)
#define wgraphZero                  SCOTCH_NAME_INTERN (wgraphZero)
#define wgraphPartEs                SCOTCH_NAME_INTERN (wgraphPartEs)
#define wgraphPartFm                SCOTCH_NAME_INTERN (wgraphPartFm)
#define wgraphPartGg                SCOTCH_NAME_INTERN (wgraphPartGg)
#define wgraphPartMl                SCOTCH_NAME_INTERN (wgraphPartMl)
#define wgraphPartRb                SCOTCH_NAME_INTERN (wgraphPartRb)
#define wgraphPartSt                SCOTCH_NAME_INTERN (wgraphPartSt)
#define wgraphPartZr                SCOTCH_NAME_INTERN (wgraphPartZr)
#define wgraphStoreInit             SCOTCH_NAME_INTERN (wgraphStoreInit)
#define wgraphStoreExit             SCOTCH_NAME_INTERN (wgraphStoreExit)
#define wgraphStoreSave             SCOTCH_NAME_INTERN (wgraphStoreSave)
#define wgraphStoreUpdt             SCOTCH_NAME_INTERN (wgraphStoreUpdt)
#endif /* SCOTCH_RENAME */

#ifdef SCOTCH_RENAME_PUBLIC
#define SCOTCH_archAlloc            SCOTCH_NAME_PUBLIC (SCOTCH_archAlloc)
#define SCOTCH_archBuild            SCOTCH_NAME_PUBLIC (SCOTCH_archBuild)
#define SCOTCH_archBuild0           SCOTCH_NAME_PUBLIC (SCOTCH_archBuild0)
#define SCOTCH_archBuild2           SCOTCH_NAME_PUBLIC (SCOTCH_archBuild2)
#define SCOTCH_archCmplt            SCOTCH_NAME_PUBLIC (SCOTCH_archCmplt)
#define SCOTCH_archCmpltw           SCOTCH_NAME_PUBLIC (SCOTCH_archCmpltw)
#define SCOTCH_archExit             SCOTCH_NAME_PUBLIC (SCOTCH_archExit)
#define SCOTCH_archHcub             SCOTCH_NAME_PUBLIC (SCOTCH_archHcub)
#define SCOTCH_archInit             SCOTCH_NAME_PUBLIC (SCOTCH_archInit)
#define SCOTCH_archLoad             SCOTCH_NAME_PUBLIC (SCOTCH_archLoad)
#define SCOTCH_archLtleaf           SCOTCH_NAME_PUBLIC (SCOTCH_archLtleaf)
#define SCOTCH_archMesh2            SCOTCH_NAME_PUBLIC (SCOTCH_archMesh2)
#define SCOTCH_archMesh3            SCOTCH_NAME_PUBLIC (SCOTCH_archMesh3)
#define SCOTCH_archMeshX            SCOTCH_NAME_PUBLIC (SCOTCH_archMeshX)
#define SCOTCH_archName             SCOTCH_NAME_PUBLIC (SCOTCH_archName)
#define SCOTCH_archSave             SCOTCH_NAME_PUBLIC (SCOTCH_archSave)
#define SCOTCH_archSize             SCOTCH_NAME_PUBLIC (SCOTCH_archSize)
#define SCOTCH_archSub              SCOTCH_NAME_PUBLIC (SCOTCH_archSub)
#define SCOTCH_archTleaf            SCOTCH_NAME_PUBLIC (SCOTCH_archTleaf)
#define SCOTCH_archTorus2           SCOTCH_NAME_PUBLIC (SCOTCH_archTorus2)
#define SCOTCH_archTorus3           SCOTCH_NAME_PUBLIC (SCOTCH_archTorus3)
#define SCOTCH_archTorusX           SCOTCH_NAME_PUBLIC (SCOTCH_archTorusX)
#define SCOTCH_archVar              SCOTCH_NAME_PUBLIC (SCOTCH_archVar)
#define SCOTCH_archVcmplt           SCOTCH_NAME_PUBLIC (SCOTCH_archVcmplt)
#define SCOTCH_archVhcub            SCOTCH_NAME_PUBLIC (SCOTCH_archVhcub)
#define SCOTCH_dgraphAlloc          SCOTCH_NAME_PUBLIC (SCOTCH_dgraphAlloc)
#define SCOTCH_dgraphBand           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphBand)
#define SCOTCH_dgraphBuild          SCOTCH_NAME_PUBLIC (SCOTCH_dgraphBuild)
#define SCOTCH_dgraphBuildGrid3D    SCOTCH_NAME_PUBLIC (SCOTCH_dgraphBuildGrid3D)
#define SCOTCH_dgraphCheck          SCOTCH_NAME_PUBLIC (SCOTCH_dgraphCheck)
#define SCOTCH_dgraphCoarsen        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphCoarsen)
#define SCOTCH_dgraphCorderExit     SCOTCH_NAME_PUBLIC (SCOTCH_dgraphCorderExit)
#define SCOTCH_dgraphCorderInit     SCOTCH_NAME_PUBLIC (SCOTCH_dgraphCorderInit)
#define SCOTCH_dgraphData           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphData)
#define SCOTCH_dgraphExit           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphExit)
#define SCOTCH_dgraphFree           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphFree)
#define SCOTCH_dgraphGather         SCOTCH_NAME_PUBLIC (SCOTCH_dgraphGather)
#define SCOTCH_dgraphGhst           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphGhst)
#define SCOTCH_dgraphGhstReplace    SCOTCH_NAME_PUBLIC (SCOTCH_dgraphGhstReplace)
#define SCOTCH_dgraphGrow           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphGrow)
#define SCOTCH_dgraphHalo           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphHalo)
#define SCOTCH_dgraphHaloAsync      SCOTCH_NAME_PUBLIC (SCOTCH_dgraphHaloAsync)
#define SCOTCH_dgraphHaloReqAlloc   SCOTCH_NAME_PUBLIC (SCOTCH_dgraphHaloReqAlloc)
#define SCOTCH_dgraphHaloWait       SCOTCH_NAME_PUBLIC (SCOTCH_dgraphHaloWait)
#define SCOTCH_dgraphInducePart     SCOTCH_NAME_PUBLIC (SCOTCH_dgraphInducePart)
#define SCOTCH_dgraphInit           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphInit)
#define SCOTCH_dgraphLoad           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphLoad)
#define SCOTCH_dgraphMap            SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMap)
#define SCOTCH_dgraphMapCompute     SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMapCompute)
#define SCOTCH_dgraphMapExit        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMapExit)
#define SCOTCH_dgraphMapInit        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMapInit)
#define SCOTCH_dgraphMapSave        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMapSave)
#define SCOTCH_dgraphMapView        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphMapView)
#define SCOTCH_dgraphOrderCblkDist  SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderCblkDist)
#define SCOTCH_dgraphOrderCompute   SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderCompute)
#define SCOTCH_dgraphOrderComputeList SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderComputeList)
#define SCOTCH_dgraphOrderExit      SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderExit)
#define SCOTCH_dgraphOrderGather    SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderGather)
#define SCOTCH_dgraphOrderInit      SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderInit)
#define SCOTCH_dgraphOrderPerm      SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderPerm)
#define SCOTCH_dgraphOrderSave      SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderSave)
#define SCOTCH_dgraphOrderSaveBlock SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderSaveBlock)
#define SCOTCH_dgraphOrderSaveMap   SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderSaveMap)
#define SCOTCH_dgraphOrderSaveTree  SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderSaveTree)
#define SCOTCH_dgraphOrderTreeDist  SCOTCH_NAME_PUBLIC (SCOTCH_dgraphOrderTreeDist)
#define SCOTCH_dgraphPart           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphPart)
#define SCOTCH_dgraphRedist         SCOTCH_NAME_PUBLIC (SCOTCH_dgraphRedist)
#define SCOTCH_dgraphSave           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphSave)
#define SCOTCH_dgraphScatter        SCOTCH_NAME_PUBLIC (SCOTCH_dgraphScatter)
#define SCOTCH_dgraphSize           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphSize)
#define SCOTCH_dgraphStat           SCOTCH_NAME_PUBLIC (SCOTCH_dgraphStat)
#define SCOTCH_dmapAlloc            SCOTCH_NAME_PUBLIC (SCOTCH_dmapAlloc)
#define SCOTCH_dorderAlloc          SCOTCH_NAME_PUBLIC (SCOTCH_dorderAlloc)
#define SCOTCH_geomAlloc            SCOTCH_NAME_PUBLIC (SCOTCH_geomAlloc)
#define SCOTCH_geomData             SCOTCH_NAME_PUBLIC (SCOTCH_geomData)
#define SCOTCH_geomExit             SCOTCH_NAME_PUBLIC (SCOTCH_geomExit)
#define SCOTCH_geomInit             SCOTCH_NAME_PUBLIC (SCOTCH_geomInit)
#define SCOTCH_graphAlloc           SCOTCH_NAME_PUBLIC (SCOTCH_graphAlloc)
#define SCOTCH_graphBase            SCOTCH_NAME_PUBLIC (SCOTCH_graphBase)
#define SCOTCH_graphBuild           SCOTCH_NAME_PUBLIC (SCOTCH_graphBuild)
#define SCOTCH_graphCheck           SCOTCH_NAME_PUBLIC (SCOTCH_graphCheck)
#define SCOTCH_graphCoarsen         SCOTCH_NAME_PUBLIC (SCOTCH_graphCoarsen)
#define SCOTCH_graphCoarsenBuild    SCOTCH_NAME_PUBLIC (SCOTCH_graphCoarsenBuild)
#define SCOTCH_graphCoarsenMatch    SCOTCH_NAME_PUBLIC (SCOTCH_graphCoarsenMatch)
#define SCOTCH_graphColor           SCOTCH_NAME_PUBLIC (SCOTCH_graphColor)
#define SCOTCH_graphData            SCOTCH_NAME_PUBLIC (SCOTCH_graphData)
#define SCOTCH_graphDiamPV          SCOTCH_NAME_PUBLIC (SCOTCH_graphDiamPV)
#define SCOTCH_graphExit            SCOTCH_NAME_PUBLIC (SCOTCH_graphExit)
#define SCOTCH_graphFree            SCOTCH_NAME_PUBLIC (SCOTCH_graphFree)
#define SCOTCH_graphGeomLoadChac    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomLoadChac)
#define SCOTCH_graphGeomLoadHabo    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomLoadHabo)
#define SCOTCH_graphGeomLoadMmkt    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomLoadMmkt)
#define SCOTCH_graphGeomLoadScot    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomLoadScot)
#define SCOTCH_graphGeomSaveChac    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomSaveChac)
#define SCOTCH_graphGeomSaveMmkt    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomSaveMmkt)
#define SCOTCH_graphGeomSaveScot    SCOTCH_NAME_PUBLIC (SCOTCH_graphGeomSaveScot)
#define SCOTCH_graphInduceList      SCOTCH_NAME_PUBLIC (SCOTCH_graphInduceList)
#define SCOTCH_graphInducePart      SCOTCH_NAME_PUBLIC (SCOTCH_graphInducePart)
#define SCOTCH_graphInit            SCOTCH_NAME_PUBLIC (SCOTCH_graphInit)
#define SCOTCH_graphLoad            SCOTCH_NAME_PUBLIC (SCOTCH_graphLoad)
#define SCOTCH_graphMap             SCOTCH_NAME_PUBLIC (SCOTCH_graphMap)
#define SCOTCH_graphMapCompute      SCOTCH_NAME_PUBLIC (SCOTCH_graphMapCompute)
#define SCOTCH_graphMapExit         SCOTCH_NAME_PUBLIC (SCOTCH_graphMapExit)
#define SCOTCH_graphMapFixed        SCOTCH_NAME_PUBLIC (SCOTCH_graphMapFixed)
#define SCOTCH_graphMapFixedCompute SCOTCH_NAME_PUBLIC (SCOTCH_graphMapFixedCompute)
#define SCOTCH_graphMapInit         SCOTCH_NAME_PUBLIC (SCOTCH_graphMapInit)
#define SCOTCH_graphMapLoad         SCOTCH_NAME_PUBLIC (SCOTCH_graphMapLoad)
#define SCOTCH_graphMapSave         SCOTCH_NAME_PUBLIC (SCOTCH_graphMapSave)
#define SCOTCH_graphMapView         SCOTCH_NAME_PUBLIC (SCOTCH_graphMapView)
#define SCOTCH_graphMapViewRaw      SCOTCH_NAME_PUBLIC (SCOTCH_graphMapViewRaw)
#define SCOTCH_graphOrder           SCOTCH_NAME_PUBLIC (SCOTCH_graphOrder)
#define SCOTCH_graphOrderCheck      SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderCheck)
#define SCOTCH_graphOrderCompute    SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderCompute)
#define SCOTCH_graphOrderComputeList SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderComputeList)
#define SCOTCH_graphOrderExit       SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderExit)
#define SCOTCH_graphOrderInit       SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderInit)
#define SCOTCH_graphOrderList       SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderList)
#define SCOTCH_graphOrderLoad       SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderLoad)
#define SCOTCH_graphOrderSave       SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderSave)
#define SCOTCH_graphOrderSaveMap    SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderSaveMap)
#define SCOTCH_graphOrderSaveTree   SCOTCH_NAME_PUBLIC (SCOTCH_graphOrderSaveTree)
#define SCOTCH_graphPart            SCOTCH_NAME_PUBLIC (SCOTCH_graphPart)
#define SCOTCH_graphPartFixed       SCOTCH_NAME_PUBLIC (SCOTCH_graphPartFixed)
#define SCOTCH_graphPartOvl         SCOTCH_NAME_PUBLIC (SCOTCH_graphPartOvl)
#define SCOTCH_graphRemap           SCOTCH_NAME_PUBLIC (SCOTCH_graphRemap)
#define SCOTCH_graphRemapCompute    SCOTCH_NAME_PUBLIC (SCOTCH_graphRemapCompute)
#define SCOTCH_graphRemapFixed      SCOTCH_NAME_PUBLIC (SCOTCH_graphRemapFixed)
#define SCOTCH_graphRemapFixedCompute SCOTCH_NAME_PUBLIC (SCOTCH_graphRemapFixedCompute)
#define SCOTCH_graphRemapView       SCOTCH_NAME_PUBLIC (SCOTCH_graphRemapView)
#define SCOTCH_graphRemapViewRaw    SCOTCH_NAME_PUBLIC (SCOTCH_graphRemapViewRaw)
#define SCOTCH_graphRepart          SCOTCH_NAME_PUBLIC (SCOTCH_graphRepart)
#define SCOTCH_graphRepartFixed     SCOTCH_NAME_PUBLIC (SCOTCH_graphRepartFixed)
#define SCOTCH_graphSave            SCOTCH_NAME_PUBLIC (SCOTCH_graphSave)
#define SCOTCH_graphSize            SCOTCH_NAME_PUBLIC (SCOTCH_graphSize)
#define SCOTCH_graphStat            SCOTCH_NAME_PUBLIC (SCOTCH_graphStat)
#define SCOTCH_graphTabLoad         SCOTCH_NAME_PUBLIC (SCOTCH_graphTabLoad)
#define SCOTCH_mapAlloc             SCOTCH_NAME_PUBLIC (SCOTCH_mapAlloc)
#define SCOTCH_meshAlloc            SCOTCH_NAME_PUBLIC (SCOTCH_meshAlloc)
#define SCOTCH_meshBuild            SCOTCH_NAME_PUBLIC (SCOTCH_meshBuild)
#define SCOTCH_meshCheck            SCOTCH_NAME_PUBLIC (SCOTCH_meshCheck)
#define SCOTCH_meshData             SCOTCH_NAME_PUBLIC (SCOTCH_meshData)
#define SCOTCH_meshExit             SCOTCH_NAME_PUBLIC (SCOTCH_meshExit)
#define SCOTCH_meshGeomLoadHabo     SCOTCH_NAME_PUBLIC (SCOTCH_meshGeomLoadHabo)
#define SCOTCH_meshGeomLoadScot     SCOTCH_NAME_PUBLIC (SCOTCH_meshGeomLoadScot)
#define SCOTCH_meshGeomSaveScot     SCOTCH_NAME_PUBLIC (SCOTCH_meshGeomSaveScot)
#define SCOTCH_meshGraph            SCOTCH_NAME_PUBLIC (SCOTCH_meshGraph)
#define SCOTCH_meshInit             SCOTCH_NAME_PUBLIC (SCOTCH_meshInit)
#define SCOTCH_meshLoad             SCOTCH_NAME_PUBLIC (SCOTCH_meshLoad)
#define SCOTCH_meshOrder            SCOTCH_NAME_PUBLIC (SCOTCH_meshOrder)
#define SCOTCH_meshOrderCheck       SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderCheck)
#define SCOTCH_meshOrderCompute     SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderCompute)
#define SCOTCH_meshOrderComputeList SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderComputeList)
#define SCOTCH_meshOrderExit        SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderExit)
#define SCOTCH_meshOrderInit        SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderInit)
#define SCOTCH_meshOrderList        SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderList)
#define SCOTCH_meshOrderSave        SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderSave)
#define SCOTCH_meshOrderSaveMap     SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderSaveMap)
#define SCOTCH_meshOrderSaveTree    SCOTCH_NAME_PUBLIC (SCOTCH_meshOrderSaveTree)
#define SCOTCH_meshSave             SCOTCH_NAME_PUBLIC (SCOTCH_meshSave)
#define SCOTCH_meshSize             SCOTCH_NAME_PUBLIC (SCOTCH_meshSize)
#define SCOTCH_meshStat             SCOTCH_NAME_PUBLIC (SCOTCH_meshStat)
#define SCOTCH_numSizeof            SCOTCH_NAME_PUBLIC (SCOTCH_numSizeof)
#define SCOTCH_orderAlloc           SCOTCH_NAME_PUBLIC (SCOTCH_orderAlloc)
#define SCOTCH_randomLoad           SCOTCH_NAME_PUBLIC (SCOTCH_randomLoad)
#define SCOTCH_randomProc           SCOTCH_NAME_PUBLIC (SCOTCH_randomProc)
#define SCOTCH_randomReset          SCOTCH_NAME_PUBLIC (SCOTCH_randomReset)
#define SCOTCH_randomSave           SCOTCH_NAME_PUBLIC (SCOTCH_randomSave)
#define SCOTCH_randomSeed           SCOTCH_NAME_PUBLIC (SCOTCH_randomSeed)
#define SCOTCH_stratAlloc           SCOTCH_NAME_PUBLIC (SCOTCH_stratAlloc)
#define SCOTCH_stratArchBuild       SCOTCH_NAME_PUBLIC (SCOTCH_stratArchBuild)
#define SCOTCH_stratDgraphClusterBuild SCOTCH_NAME_PUBLIC (SCOTCH_stratDgraphClusterBuild)
#define SCOTCH_stratDgraphMap       SCOTCH_NAME_PUBLIC (SCOTCH_stratDgraphMap)
#define SCOTCH_stratDgraphMapBuild  SCOTCH_NAME_PUBLIC (SCOTCH_stratDgraphMapBuild)
#define SCOTCH_stratDgraphOrder     SCOTCH_NAME_PUBLIC (SCOTCH_stratDgraphOrder)
#define SCOTCH_stratDgraphOrderBuild SCOTCH_NAME_PUBLIC (SCOTCH_stratDgraphOrderBuild)
#define SCOTCH_stratExit            SCOTCH_NAME_PUBLIC (SCOTCH_stratExit)
#define SCOTCH_stratFree            SCOTCH_NAME_PUBLIC (SCOTCH_stratFree)
#define SCOTCH_stratGraphBipart     SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphBipart)
#define SCOTCH_stratGraphClusterBuild SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphClusterBuild)
#define SCOTCH_stratGraphMap        SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphMap)
#define SCOTCH_stratGraphMapBuild   SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphMapBuild)
#define SCOTCH_stratGraphOrder      SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphOrder)
#define SCOTCH_stratGraphOrderBuild SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphOrderBuild)
#define SCOTCH_stratGraphPartOvl    SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphPartOvl)
#define SCOTCH_stratGraphPartOvlBuild SCOTCH_NAME_PUBLIC (SCOTCH_stratGraphPartOvlBuild)
#define SCOTCH_stratInit            SCOTCH_NAME_PUBLIC (SCOTCH_stratInit)
#define SCOTCH_stratMeshOrder       SCOTCH_NAME_PUBLIC (SCOTCH_stratMeshOrder)
#define SCOTCH_stratMeshOrderBuild  SCOTCH_NAME_PUBLIC (SCOTCH_stratMeshOrderBuild)
#define SCOTCH_stratSave            SCOTCH_NAME_PUBLIC (SCOTCH_stratSave)
#define SCOTCH_version              SCOTCH_NAME_PUBLIC (SCOTCH_version)
#endif /* SCOTCH_RENAME_PUBLIC */
