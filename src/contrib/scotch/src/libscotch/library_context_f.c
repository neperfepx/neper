/* Copyright 2020,2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_context_f.c                     **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This file contains the Fortran API for  **/
/**                the context handling routines of the    **/
/**                libSCOTCH library.                      **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 22 aug 2020     **/
/**                                 to   : 03 jun 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "scotch.h"

/**************************************/
/*                                    */
/* These routines are the Fortran API */
/* for the context handling routines. */
/*                                    */
/**************************************/

/*
**
*/

SCOTCH_FORTRAN (                      \
CONTEXTINIT, contextinit, (           \
SCOTCH_Context * const      contptr,  \
int * const                 revaptr), \
(contptr, revaptr))
{
  *revaptr = SCOTCH_contextInit (contptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
CONTEXTEXIT, contextexit, (           \
SCOTCH_Context * const      contptr), \
(contptr))
{
  SCOTCH_contextExit (contptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
CONTEXTSIZEOF, contextsizeof, (       \
int * const                 sizeptr), \
(sizeptr))
{
  *sizeptr = SCOTCH_contextSizeof ();
}

/*
**
*/

SCOTCH_FORTRAN (                          \
CONTEXTRANDOMCLONE, contextrandomclone, ( \
SCOTCH_Context * const      contptr,      \
int * const                 revaptr),     \
(contptr, revaptr))
{
  *revaptr = SCOTCH_contextRandomClone (contptr);
}

/*
**
*/

SCOTCH_FORTRAN (                          \
CONTEXTRANDOMRESET, contextrandomreset, ( \
SCOTCH_Context * const      contptr),     \
(contptr))
{
  SCOTCH_contextRandomReset (contptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
CONTEXTRANDOMSEED, contextrandomseed, ( \
SCOTCH_Context * const      contptr,    \
SCOTCH_Num * const          seedptr),   \
(contptr, seedptr))
{
  SCOTCH_contextRandomSeed (contptr, *seedptr);
}

/*
**
*/

SCOTCH_FORTRAN (                              \
CONTEXTTHREADIMPORT1, contextthreadimport1, ( \
SCOTCH_Context * const      contptr,          \
int * const                 thrdptr,          \
int * const                 revaptr),         \
(contptr, thrdptr, revaptr))
{
  *revaptr = SCOTCH_contextThreadImport1 (contptr, *thrdptr);
}

/*
**
*/

SCOTCH_FORTRAN (                              \
CONTEXTTHREADIMPORT2, contextthreadimport2, ( \
SCOTCH_Context * const      contptr,          \
int * const                 thrdptr,          \
int * const                 revaptr),         \
(contptr, thrdptr, revaptr))
{
  *revaptr = SCOTCH_contextThreadImport2 (contptr, *thrdptr);
}

/*
**
*/

SCOTCH_FORTRAN (                          \
CONTEXTTHREADSPAWN, contextthreadspawn, ( \
SCOTCH_Context * const      contptr,      \
int * const                 thrdptr,      \
const int * const           coreptr,      \
int * const                 revaptr),     \
(contptr, thrdptr, coreptr, revaptr))
{
  *revaptr = SCOTCH_contextThreadSpawn (contptr, *thrdptr, ((void *) coreptr == (void *) contptr) ? NULL : coreptr);
}

/*
**
*/

SCOTCH_FORTRAN (                            \
CONTEXTOPTIONGETNUM, contextoptiongetnum, ( \
SCOTCH_Context * const      contptr,        \
const int * const           optinumptr,     \
SCOTCH_Num * const          optivalptr,     \
int * const                 revaptr),       \
(contptr, optinumptr, optivalptr, revaptr))
{
  *revaptr = SCOTCH_contextOptionGetNum (contptr, *optinumptr, optivalptr);
}

/*
**
*/

SCOTCH_FORTRAN (                            \
CONTEXTOPTIONSETNUM, contextoptionsetnum, ( \
SCOTCH_Context * const      contptr,        \
const int * const           optinumptr,     \
SCOTCH_Num * const          optivalptr,     \
int * const                 revaptr),       \
(contptr, optinumptr, optivalptr, revaptr))
{
  *revaptr = SCOTCH_contextOptionSetNum (contptr, *optinumptr, *optivalptr);
}
