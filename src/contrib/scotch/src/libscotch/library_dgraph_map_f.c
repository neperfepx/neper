/* Copyright 2008,2010-2012,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_dgraph_map_f.c                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the Fortran API for the  **/
/**                parallel mapping routines of the        **/
/**                libSCOTCH library.                      **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 28 jun 2008     **/
/**                                 to   : 31 aug 2011     **/
/**                # Version 6.0  : from : 29 nov 2012     **/
/**                                 to   : 25 apr 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "ptscotch.h"

/**************************************/
/*                                    */
/* These routines are the Fortran API */
/* for the parallel mapping routines. */
/*                                    */
/**************************************/

/*
**
*/

SCOTCH_FORTRAN (                        \
DGRAPHMAPINIT, dgraphmapinit, (         \
const SCOTCH_Dgraph * const grafptr,    \
SCOTCH_Dmapping * const     mappptr,    \
const SCOTCH_Arch * const   archptr,    \
SCOTCH_Num * const          termloctab, \
int * const                 revaptr),   \
(grafptr, mappptr, archptr, termloctab, revaptr))
{
  *revaptr = SCOTCH_dgraphMapInit (grafptr, mappptr, archptr, termloctab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
DGRAPHMAPEXIT, dgraphmapexit, (       \
const SCOTCH_Dgraph * const grafptr,  \
SCOTCH_Dmapping * const     mappptr), \
(grafptr, mappptr))
{
  SCOTCH_dgraphMapExit (grafptr, mappptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
DGRAPHMAPSAVE, dgraphmapsave, (       \
const SCOTCH_Dgraph * const grafptr,  \
SCOTCH_Dmapping * const     mappptr,  \
int * const                 fileptr,  \
int * const                 revaptr), \
(grafptr, mappptr, fileptr, revaptr))
{
  FILE *              stream;                     /* Stream to build from handle */
  int                 filenum;                    /* Duplicated handle           */
  int                 o;

  if ((filenum = dup (*fileptr)) < 0) {           /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (DGRAPHMAPSAVE)) ": cannot duplicate handle");

    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((stream = fdopen (filenum, "w")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (DGRAPHMAPSAVE)) ": cannot open output stream");
    close      (filenum);
    *revaptr = 1;
    return;
  }

  o = SCOTCH_dgraphMapSave (grafptr, mappptr, stream);

  fclose (stream);                                /* This closes filenum too */

  *revaptr = o;
}

/*
**
*/

SCOTCH_FORTRAN (                      \
DGRAPHMAPCOMPUTE, dgraphmapcompute, ( \
SCOTCH_Dgraph * const       grafptr,  \
SCOTCH_Dmapping * const     mappptr,  \
SCOTCH_Strat * const        straptr,  \
int * const                 revaptr), \
(grafptr, mappptr, straptr, revaptr))
{
  *revaptr = SCOTCH_dgraphMapCompute (grafptr, mappptr, straptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
DGRAPHMAP, dgraphmap, (                 \
SCOTCH_Dgraph * const       grafptr,    \
const SCOTCH_Arch * const   archptr,    \
SCOTCH_Strat * const        straptr,    \
SCOTCH_Num * const          partloctab, \
int * const                 revaptr),   \
(grafptr, archptr, straptr, partloctab, revaptr))
{
  *revaptr = SCOTCH_dgraphMap (grafptr, archptr, straptr, partloctab);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
DGRAPHPART, dgraphpart, (               \
SCOTCH_Dgraph * const       grafptr,    \
const SCOTCH_Num * const    partnbr,    \
SCOTCH_Strat * const        straptr,    \
SCOTCH_Num * const          partloctab, \
int * const                 revaptr),   \
(grafptr, partnbr, straptr, partloctab, revaptr))
{
  *revaptr = SCOTCH_dgraphPart (grafptr, *partnbr, straptr, partloctab);
}

/* String lengths are passed at the very
** end of the argument list.
*/

SCOTCH_FORTRAN (                     \
STRATDGRAPHMAP, stratdgraphmap, (    \
SCOTCH_Strat * const        straptr, \
const char * const          string,  \
int * const                 revaptr, \
const int                   strnbr), \
(straptr, string, revaptr, strnbr))
{
  char * restrict     strtab;                     /* Pointer to null-terminated string */

  if ((strtab = (char *) memAlloc (strnbr + 1)) == NULL) { /* Allocate temporary space */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (STRATDGRAPHMAP)) ": out of memory");
    *revaptr = 1;
    return;
  }
  memCpy (strtab, string, strnbr);                /* Copy string contents */
  strtab[strnbr] = '\0';                          /* Terminate string     */

  *revaptr = SCOTCH_stratDgraphMap (straptr, strtab); /* Call original routine */

  memFree (strtab);
}

/*
**
*/

SCOTCH_FORTRAN (                            \
STRATDGRAPHMAPBUILD, stratdgraphmapbuild, ( \
SCOTCH_Strat * const        straptr,        \
const SCOTCH_Num * const    flagval,        \
const SCOTCH_Num * const    procnbr,        \
const SCOTCH_Num * const    partnbr,        \
const double * const        kbalval,        \
int * const                 revaptr),       \
(straptr, flagval, procnbr, partnbr, kbalval, revaptr))
{
  *revaptr = SCOTCH_stratDgraphMapBuild (straptr, *flagval, *procnbr, *partnbr, *kbalval);
}

/*
**
*/

SCOTCH_FORTRAN (                                    \
STRATDGRAPHCLUSTERBUILD, stratdgraphclusterbuild, ( \
SCOTCH_Strat * const        straptr,                \
const SCOTCH_Num * const    flagval,                \
const SCOTCH_Num * const    procnbr,                \
const SCOTCH_Num * const    pwgtval,                \
const double * const        densval,                \
const double * const        bbalval,                \
int * const                 revaptr),               \
(straptr, flagval, procnbr, pwgtval, densval, bbalval, revaptr))
{
  *revaptr = SCOTCH_stratDgraphClusterBuild (straptr, *flagval, *procnbr, *pwgtval, *densval, *bbalval);
}
