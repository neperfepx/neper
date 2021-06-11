/* Copyright 2004,2007,2010,2011,2015,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_arch_f.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This file contains the Fortran API for  **/
/**                the architecture handling routines of   **/
/**                the libSCOTCH library.                  **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 02 dec 1999     **/
/**                                 to   : 15 nov 2001     **/
/**                # Version 4.0  : from : 13 jan 2004     **/
/**                                 to   : 13 jan 2004     **/
/**                # Version 5.1  : from : 27 mar 2010     **/
/**                                 to   : 13 feb 2011     **/
/**                # Version 6.0  : from : 16 apr 2015     **/
/**                                 to   : 31 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "scotch.h"

/*************************************/
/*                                   */
/* These routines are the Fortran    */
/* API for the architecture handling */
/* routines.                         */
/*                                   */
/*************************************/

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHINIT, archinit, (                 \
SCOTCH_Arch * const         archptr,  \
int * const                 revaptr), \
(archptr, revaptr))
{
  *revaptr = SCOTCH_archInit (archptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHEXIT, archexit, (                 \
SCOTCH_Arch * const         archptr), \
(archptr))
{
  SCOTCH_archExit (archptr);
}

/* When an input stream is built from the given
** file handle, it is set as unbuffered, so as to
** allow for multiple stream reads from the same
** file handle. If it were buffered, too many
** input characters would be read on the first
** block read.
*/

SCOTCH_FORTRAN (                      \
ARCHLOAD, archload, (                 \
SCOTCH_Arch * const         archptr,  \
int * const                 fileptr,  \
int * const                 revaptr), \
(archptr, fileptr, revaptr))
{
  FILE *              stream;                     /* Stream to build from handle */
  int                 filenum;                    /* Duplicated handle           */
  int                 o;

  if ((filenum = dup (*fileptr)) < 0) {           /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (ARCHLOAD)) ": cannot duplicate handle");
    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((stream = fdopen (filenum, "r")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (ARCHLOAD)) ": cannot open input stream");
    close      (filenum);
    *revaptr = 1;
    return;
  }
  setbuf (stream, NULL);                          /* Do not buffer on input */

  o = SCOTCH_archLoad (archptr, stream);

  fclose (stream);                                /* This closes filenum too */

  *revaptr = o;
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHSAVE, archsave, (                 \
SCOTCH_Arch * const         archptr,  \
int * const                 fileptr,  \
int * const                 revaptr), \
(archptr, fileptr, revaptr))
{
  FILE *              stream;                     /* Stream to build from handle */
  int                 filenum;                    /* Duplicated handle           */
  int                 o;

  if ((filenum = dup (*fileptr)) < 0) {           /* If cannot duplicate file descriptor */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (ARCHSAVE)) ": cannot duplicate handle");

    *revaptr = 1;                                 /* Indicate error */
    return;
  }
  if ((stream = fdopen (filenum, "w")) == NULL) { /* Build stream from handle */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (ARCHSAVE)) ": cannot open output stream");
    close      (filenum);
    *revaptr = 1;
    return;
  }

  o = SCOTCH_archSave (archptr, stream);

  fclose (stream);                                /* This closes filenum too */

  *revaptr = o;
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHNAME, archname, (                 \
SCOTCH_Arch * const         archptr,  \
char * const                chartab,  \
int * const                 charptr), \
(archptr, chartab, charptr))
{
  char *              nameptr;                    /* Name string */

  nameptr = SCOTCH_archName (archptr);            /* Get architecture name  */
  strncpy (chartab, nameptr, *charptr);           /* Copy architecture name */
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHSIZE, archsize, (                 \
SCOTCH_Arch * const         archptr,  \
int * const                 sizeptr), \
(archptr, sizeptr))
{
  *sizeptr = SCOTCH_archSize (archptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHVAR, archvar, (                   \
SCOTCH_Arch * const         archptr,  \
int * const                 flagptr), \
(archptr, flagptr))
{
  *flagptr = SCOTCH_archVar (archptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHCMPLT, archcmplt, (               \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    vertnbr,  \
int * const                 revaptr), \
(archptr, vertnbr, revaptr))
{
  *revaptr = SCOTCH_archCmplt (archptr, *vertnbr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHCMPLTW, archcmpltw, (             \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    vertnbr,  \
const SCOTCH_Num * const    velotab,  \
int * const                 revaptr), \
(archptr, vertnbr, velotab, revaptr))
{
  *revaptr = SCOTCH_archCmpltw (archptr, *vertnbr, velotab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHHCUB, archhcub, (                 \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimnmax,  \
int * const                 revaptr), \
(archptr, dimnmax, revaptr))
{
  *revaptr = SCOTCH_archHcub (archptr, *dimnmax);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHMESH2, archmesh2, (               \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimxval,  \
const SCOTCH_Num * const    dimyval,  \
int * const                 revaptr), \
(archptr, dimxval, dimyval, revaptr))
{
  *revaptr = SCOTCH_archMesh2 (archptr, *dimxval, *dimyval);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHMESH3, archmesh3, (               \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimxval,  \
const SCOTCH_Num * const    dimyval,  \
const SCOTCH_Num * const    dimzval,  \
int * const                 revaptr), \
(archptr, dimxval, dimyval, dimzval, revaptr))
{
  *revaptr = SCOTCH_archMesh3 (archptr, *dimxval, *dimyval, *dimzval);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHMESHX, archmeshx, (               \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimnnbr,  \
const SCOTCH_Num * const    dimntab,  \
int * const                 revaptr), \
(archptr, dimnnbr, dimntab, revaptr))
{
  *revaptr = SCOTCH_archMeshX (archptr, *dimnnbr, dimntab);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
ARCHSUB, archsub, (                     \
SCOTCH_Arch * const         subarchptr, \
SCOTCH_Arch * const         orgarchptr, \
const SCOTCH_Num * const    vnumnbr,    \
const SCOTCH_Num * const    vnumtab,    \
int * const                 revaptr),   \
(subarchptr, orgarchptr, vnumnbr, vnumtab, revaptr))
{
  *revaptr = SCOTCH_archSub (subarchptr, orgarchptr, *vnumnbr, vnumtab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHTLEAF, archtleaf, (               \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    levlnbr,  \
const SCOTCH_Num * const    sizetab,  \
const SCOTCH_Num * const    linktab,  \
int * const                 revaptr), \
(archptr, levlnbr, sizetab, linktab, revaptr))
{
  *revaptr = SCOTCH_archTleaf (archptr, *levlnbr, sizetab, linktab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHTORUS2, archtorus2, (             \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimxval,  \
const SCOTCH_Num * const    dimyval,  \
int * const                 revaptr), \
(archptr, dimxval, dimyval, revaptr))
{
  *revaptr = SCOTCH_archTorus2 (archptr, *dimxval, *dimyval);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHTORUS3, archtorus3, (             \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimxval,  \
const SCOTCH_Num * const    dimyval,  \
const SCOTCH_Num * const    dimzval,  \
int * const                 revaptr), \
(archptr, dimxval, dimyval, dimzval, revaptr))
{
  *revaptr = SCOTCH_archTorus3 (archptr, *dimxval, *dimyval, *dimzval);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHTORUSX, archtorusx, (             \
SCOTCH_Arch * const         archptr,  \
const SCOTCH_Num * const    dimnnbr,  \
const SCOTCH_Num * const    dimntab,  \
int * const                 revaptr), \
(archptr, dimnnbr, dimntab, revaptr))
{
  *revaptr = SCOTCH_archTorusX (archptr, *dimnnbr, dimntab);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHVCMPLT, archvcmplt, (             \
SCOTCH_Arch * const         archptr,  \
int * const                 revaptr), \
(archptr, revaptr))
{
  *revaptr = SCOTCH_archVcmplt (archptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHVHCUB, archvhcub, (               \
SCOTCH_Arch * const         archptr,  \
int * const                 revaptr), \
(archptr, revaptr))
{
  *revaptr = SCOTCH_archVhcub (archptr);
}
