/* Copyright 2019 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_arch_dom_f.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This file contains the Fortran API      **/
/**                for the target architecture domains     **/
/**                handling routines of the libSCOTCH      **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 16 apr 2019     **/
/**                                 to   : 17 apr 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "scotch.h"

/***********************************/
/*                                 */
/* These routines are the Fortran  */
/* API for the architecture domain */
/* handling routines.              */
/*                                 */
/***********************************/

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHDOMNUM, archdomnum, (             \
SCOTCH_Arch * const         archptr,  \
SCOTCH_ArchDom * const      domnptr,  \
SCOTCH_Num * const          dnumptr), \
(archptr, domnptr, dnumptr))
{
  *dnumptr = SCOTCH_archDomNum (archptr, domnptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHDOMTERM, archdomterm, (           \
SCOTCH_Arch * const         archptr,  \
SCOTCH_ArchDom * const      domnptr,  \
const SCOTCH_Num * const    dnumptr,  \
int * const                 revaptr), \
(archptr, domnptr, dnumptr, revaptr))
{
  *revaptr = SCOTCH_archDomTerm (archptr, domnptr, *dnumptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
ARCHDOMSIZE, archdomsize, (             \
SCOTCH_Arch * const           archptr,  \
const SCOTCH_ArchDom * const  domnptr,  \
SCOTCH_Num * const            sizeptr), \
(archptr, domnptr, sizeptr))
{
  *sizeptr = SCOTCH_archDomSize (archptr, domnptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
ARCHDOMWGHT, archdomwght, (             \
SCOTCH_Arch * const           archptr,  \
const SCOTCH_ArchDom * const  domnptr,  \
SCOTCH_Num * const            wghtptr), \
(archptr, domnptr, wghtptr))
{
  *wghtptr = SCOTCH_archDomWght (archptr, domnptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
ARCHDOMDIST, archdomdist, (             \
SCOTCH_Arch * const           archptr,  \
const SCOTCH_ArchDom * const  dom0ptr,  \
const SCOTCH_ArchDom * const  dom1ptr,  \
SCOTCH_Num * const            distptr), \
(archptr, dom0ptr, dom1ptr, distptr))
{
  *distptr = SCOTCH_archDomDist (archptr, dom0ptr, dom1ptr);
}

/*
**
*/

SCOTCH_FORTRAN (                      \
ARCHDOMFRST, archdomfrst, (           \
SCOTCH_Arch * const         archptr,  \
SCOTCH_ArchDom * const      domnptr,  \
int * const                 revaptr), \
(archptr, domnptr, revaptr))
{
  *revaptr = SCOTCH_archDomFrst (archptr, domnptr);
}

/*
**
*/

SCOTCH_FORTRAN (                        \
ARCHDOMBIPART, archdombipart, (         \
SCOTCH_Arch * const           archptr,  \
const SCOTCH_ArchDom * const  domnptr,  \
SCOTCH_ArchDom * const        dom0ptr,  \
SCOTCH_ArchDom * const        dom1ptr,  \
int * const                   revaptr), \
(archptr, domnptr, dom0ptr, dom1ptr, revaptr))
{
  *revaptr = SCOTCH_archDomBipart (archptr, domnptr, dom0ptr, dom1ptr);
}
