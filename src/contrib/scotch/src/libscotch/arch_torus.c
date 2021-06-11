/* Copyright 2004,2007,2008,2010,2011,2013 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_torus.c                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module handles the torus graph     **/
/**                target architectures.                   **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 01 dec 1992     **/
/**                                 to   : 24 mar 1993     **/
/**                # Version 1.2  : from : 04 feb 1994     **/
/**                                 to   : 11 feb 1994     **/
/**                # Version 1.3  : from : 20 apr 1994     **/
/**                                 to   : 20 apr 1994     **/
/**                # Version 2.0  : from : 06 jun 1994     **/
/**                                 to   : 23 dec 1994     **/
/**                # Version 2.1  : from : 07 apr 1995     **/
/**                                 to   : 29 jun 1995     **/
/**                # Version 3.0  : from : 01 jul 1995     **/
/**                                 to   : 08 sep 1995     **/
/**                # Version 3.1  : from : 07 may 1996     **/
/**                                 to   : 22 jul 1996     **/
/**                # Version 3.2  : from : 16 oct 1996     **/
/**                                 to   : 14 may 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 4.0  : from : 05 nov 2003     **/
/**                                 to   : 10 mar 2005     **/
/**                # Version 5.1  : from : 21 jan 2008     **/
/**                                 to   : 11 aug 2010     **/
/**                # Version 6.0  : from : 14 feb 2011     **/
/**                                 to   : 27 mar 2015     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH_TORUS

#include "module.h"
#include "common.h"
#include "arch.h"
#include "arch_mesh.h"
#include "arch_torus.h"

/***********************************************/
/*                                             */
/* These are the 2-dimensional torus routines. */
/*                                             */
/***********************************************/

/* This function returns the average
** distance between two rectangular
** domains (in fact the distance between
** the centers of the domains).
*/

Anum
archTorus2DomDist (
const ArchTorus2 * const    archptr,
const ArchTorus2Dom * const dom0ptr,
const ArchTorus2Dom * const dom1ptr)
{
  Anum               dc0, dc1;
  Anum               ds0, ds1;

  dc0 = abs (dom0ptr->c[0][0] + dom0ptr->c[0][1] -
             dom1ptr->c[0][0] - dom1ptr->c[0][1]);
  ds0 = (dc0 > archptr->c[0]) ? (2 * archptr->c[0] - dc0) : dc0;

  dc1 = abs (dom0ptr->c[1][0] + dom0ptr->c[1][1] -
             dom1ptr->c[1][0] - dom1ptr->c[1][1]);
  ds1 = (dc1 > archptr->c[1]) ? (2 * archptr->c[1] - dc1) : dc1;

  return ((ds0 + ds1) >> 1);
}

/***********************************************/
/*                                             */
/* These are the 3-dimensional torus routines. */
/*                                             */
/***********************************************/

/* This function returns the average distance
** between two cubic domains (in fact the
** distance between the centers of the domains).
*/

Anum
archTorus3DomDist (
const ArchTorus3 * const    archptr,
const ArchTorus3Dom * const dom0ptr,
const ArchTorus3Dom * const dom1ptr)
{
  Anum               dc0, dc1, dc2;
  Anum               ds0, ds1, ds2;

  dc0 = abs (dom0ptr->c[0][0] + dom0ptr->c[0][1] -
             dom1ptr->c[0][0] - dom1ptr->c[0][1]);
  ds0 = (dc0 > archptr->c[0]) ? (2 * archptr->c[0] - dc0) : dc0;

  dc1 = abs (dom0ptr->c[1][0] + dom0ptr->c[1][1] -
             dom1ptr->c[1][0] - dom1ptr->c[1][1]);
  ds1 = (dc1 > archptr->c[1]) ? (2 * archptr->c[1] - dc1) : dc1;

  dc2 = abs (dom0ptr->c[2][0] + dom0ptr->c[2][1] -
             dom1ptr->c[2][0] - dom1ptr->c[2][1]);
  ds2 = (dc2 > archptr->c[2]) ? (2 * archptr->c[2] - dc2) : dc2;

  return ((ds0 + ds1 + ds2) >> 1);
}

/***********************************************/
/*                                             */
/* These are the x-dimensional torus routines. */
/*                                             */
/***********************************************/

/* This function returns the average distance
** between two X-dimensional domains (that is,
** the Manhattan distance between the centers
** of the domains).
*/

Anum
archTorusXDomDist (
const ArchTorusX * const    archptr,
const ArchTorusXDom * const dom0ptr,
const ArchTorusXDom * const dom1ptr)
{
  Anum                dimnnum;
  Anum                distval;

  for (dimnnum = 0, distval = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    Anum                disttmp;

    disttmp = abs (dom0ptr->c[dimnnum][0] + dom0ptr->c[dimnnum][1] -
                   dom1ptr->c[dimnnum][0] - dom1ptr->c[dimnnum][1]);
    distval += (disttmp > archptr->c[dimnnum]) ? (2 * archptr->c[dimnnum] - disttmp) : disttmp;
  }

  return (distval >> 1);
}

/* This function creates the MPI_Datatype for
** xD torus domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archTorusXDomMpiType (
const ArchTorusX * const      archptr,
MPI_Datatype * const          typeptr)
{
  MPI_Type_contiguous (2 * archptr->dimmax, ANUM_MPI, typeptr);

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */
