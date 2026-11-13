/* Copyright 2004,2007,2012,2014,2018,2019,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_random_f.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This file contains the Fortran API for  **/
/**                the random generator handling routines  **/
/**                of the libSCOTCH library.               **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 21 nov 2005     **/
/**                                 to   : 23 nov 2005     **/
/**                # Version 6.0  : from : 08 oct 2012     **/
/**                                 to   : 25 apr 2018     **/
/**                # Version 7.0  : from : 15 sep 2019     **/
/**                                 to   : 06 oct 2026     **/
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
/* for the random handling routines.  */
/*                                    */
/**************************************/

/*
**
*/

SCOTCH_FORTRAN (                      \
RANDOMLOAD, randomload, (             \
int * const                 fileptr,  \
int * const                 revaptr), \
(fileptr, revaptr))
{
  FILE *              stream;                     /* Stream to build from handle */
  int                 filenum;                    /* Duplicated handle           */
  int                 o;

  if ((filenum = dup (*fileptr)) < 0) {           /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (RANDOMLOAD)) ": cannot duplicate handle");
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((stream = fdopen (filenum, "r")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (RANDOMLOAD)) ": cannot open input stream");
    close      (filenum);
    *revaptr = 1;
    return;
  }
  setbuf (stream, NULL);                          /* Do not buffer on input */

  o = SCOTCH_randomLoad (stream);

  fclose (stream);                                /* This closes filenum too */

  *revaptr = o;
}

/*
**
*/

SCOTCH_FORTRAN (                      \
RANDOMSAVE, randomsave, (             \
int * const                 fileptr,  \
int * const                 revaptr), \
(fileptr, revaptr))
{
  FILE *              stream;                     /* Stream to build from handle */
  int                 filenum;                    /* Duplicated handle           */
  int                 o;

  if ((filenum = dup (*fileptr)) < 0) {           /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (RANDOMSAVE)) ": cannot duplicate handle");

    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((stream = fdopen (filenum, "w")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (RANDOMSAVE)) ": cannot open output stream");
    close      (filenum);
    *revaptr = 1;
    return;
  }

  o = SCOTCH_randomSave (stream);

  fclose (stream);                                /* This closes filenum too */

  *revaptr = o;
}

/*
**
*/

SCOTCH_FORTRAN (                      \
RANDOMPROC, randomproc, (             \
const int * const           procnum), \
(procnum))
{
  SCOTCH_randomProc (*procnum);
}

/*
**
*/

SCOTCH_FORTRAN (                  \
RANDOMRESET, randomreset, (void), \
())
{
  SCOTCH_randomReset ();
}

/*
**
*/

SCOTCH_FORTRAN (                    \
RANDOMSEED, randomseed, (           \
const SCOTCH_Num * const  seedptr), \
(seedptr))
{
  SCOTCH_randomSeed (*seedptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
RANDOMVAL, randomval, (               \
const SCOTCH_Num * const    randmax,  \
SCOTCH_Num * const          revaptr), \
(randmax, revaptr))
{
  *revaptr = SCOTCH_randomVal (*randmax);
}
