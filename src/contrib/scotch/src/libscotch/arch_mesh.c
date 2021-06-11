/* Copyright 2004,2007,2008,2010,2011,2015,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_mesh.c                             **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module handles the mesh graph      **/
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
/**                # Version 3.1  : from : 22 jul 1996     **/
/**                                 to   : 23 jul 1996     **/
/**                # Version 3.2  : from : 16 oct 1996     **/
/**                                 to   : 14 may 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 4.0  : from : 09 jan 2004     **/
/**                                 to   : 10 mar 2005     **/
/**                # Version 5.1  : from : 21 jan 2008     **/
/**                                 to   : 11 aug 2010     **/
/**                # Version 6.0  : from : 14 feb 2011     **/
/**                                 to   : 15 may 2018     **/
/**                                                        **/
/**   NOTES      : # The vertices of the (dX,dY) mesh are  **/
/**                  numbered as terminals so that         **/
/**                  t(0,0) = 0, t(1,0) = 1,               **/
/**                  t(dX - 1, 0) = dX - 1, t(0,1) = dX,   **/
/**                  and t(x,y) = (y * dX) + x.            **/
/**                  The same holds for meshes of higher   **/
/**                  dimensions: index increases in higher **/
/**                  dimensions result in higher terminal  **/
/**                  domain numbers.                       **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH_MESH

#include "module.h"
#include "common.h"
#include "arch.h"
#include "arch_mesh.h"

/***********************************************/
/*                                             */
/* These are the 2-dimensional mesh routines. */
/*                                             */
/***********************************************/

/* This routine loads the
** bidimensional mesh architecture.
** It returns:
** - 0   : if the architecture has been successfully read.
** - !0  : on error.
*/

int
archMesh2ArchLoad (
ArchMesh2 * restrict const  archptr,
FILE * restrict const       stream)
{
#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMesh2)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMesh2Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMesh2ArchLoad: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((intLoad (stream, &archptr->c[0]) != 1) ||
      (intLoad (stream, &archptr->c[1]) != 1) ||
      (archptr->c[0] < 1) || (archptr->c[1] < 1)) {
    errorPrint ("archMesh2ArchLoad: bad input");
    return     (1);
  }
  archptr->dimnnbr = 2;

  return (0);
}

/* This routine saves the
** bidimensional mesh architecture.
** It returns:
** - 0   : if the architecture has been successfully written.
** - !0  : on error.
*/

int
archMesh2ArchSave (
const ArchMesh2 * const     archptr,
FILE * restrict const       stream)
{
#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMesh2)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMesh2Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMesh2ArchSave: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if (fprintf (stream, ANUMSTRING " " ANUMSTRING "\n",
               (Anum) archptr->c[0],
               (Anum) archptr->c[1]) == EOF) {
    errorPrint ("archMesh2ArchSave: bad output");
    return     (1);
  }

  return (0);
}

/* This function returns the smallest number
** of terminal domain included in the given
** domain.
*/

ArchDomNum
archMesh2DomNum (
const ArchMesh2 * const     archptr,
const ArchMesh2Dom * const  domptr)
{
  return ((domptr->c[1][0] * archptr->c[0]) + domptr->c[0][0]); /* Return vertex number */
}

/* This function returns the terminal domain associated
** with the given terminal number in the architecture.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

int
archMesh2DomTerm (
const ArchMesh2 * const     archptr,
ArchMesh2Dom * const        domptr,
const ArchDomNum            domnum)
{
  if (domnum < (archptr->c[0] * archptr->c[1])) { /* If valid label */
    domptr->c[0][0] =                             /* Set the domain */
    domptr->c[0][1] = domnum % archptr->c[0];
    domptr->c[1][0] =
    domptr->c[1][1] = domnum / archptr->c[0];

    return (0);
  }

  return (1);                                     /* Cannot set domain */
}

/* This function returns the number of
** elements in the rectangular domain.
*/

Anum
archMesh2DomSize (
const ArchMesh2 * const     archptr,
const ArchMesh2Dom * const  domptr)
{
  return ((domptr->c[0][1] - domptr->c[0][0] + 1) *
          (domptr->c[1][1] - domptr->c[1][0] + 1));
}

/* This function returns the average
** distance between two rectangular
** domains (in fact the distance between
** the centers of the domains).
*/

Anum
archMesh2DomDist (
const ArchMesh2 * const    archptr,
const ArchMesh2Dom * const dom0ptr,
const ArchMesh2Dom * const dom1ptr)
{
  return (((abs (dom0ptr->c[0][0] + dom0ptr->c[0][1] -
                 dom1ptr->c[0][0] - dom1ptr->c[0][1]) + 1) / 2) +
          ((abs (dom0ptr->c[1][0] + dom0ptr->c[1][1] -
                 dom1ptr->c[1][0] - dom1ptr->c[1][1]) + 1) / 2));
}

/* These functions try to split a rectangular
** domain into two subdomains.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

int
archMesh2DomBipart (
const ArchMesh2 * const       archptr,
const ArchMesh2Dom * const    domptr,
ArchMesh2Dom * restrict const dom0ptr,
ArchMesh2Dom * restrict const dom1ptr)
{
  Anum                dimsiz[2];
  int                 dimval;                     /* Dimension along which to split */

  dimsiz[0] = domptr->c[0][1] - domptr->c[0][0];
  dimsiz[1] = domptr->c[1][1] - domptr->c[1][0];

  if ((dimsiz[0] | dimsiz[1]) == 0)               /* Return if cannot bipartition more */
    return (1);

  dimval = 1;
  if ((dimsiz[0] > dimsiz[1]) ||                  /* Split domain in two along largest dimension */
      ((dimsiz[0] == dimsiz[1]) && (archptr->c[0] > archptr->c[1])))
    dimval = 0;

  if (dimval == 0) {                              /* Split across the X dimension */
    dom0ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = (domptr->c[0][0] + domptr->c[0][1]) / 2;
    dom1ptr->c[0][0] = dom0ptr->c[0][1] + 1;
    dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = dom1ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = dom1ptr->c[1][1] = domptr->c[1][1];
  }
  else {                                          /* Split across the Y dimension */
    dom0ptr->c[0][0] = dom1ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = (domptr->c[1][0] + domptr->c[1][1]) / 2;
    dom1ptr->c[1][0] = dom0ptr->c[1][1] + 1;
    dom1ptr->c[1][1] = domptr->c[1][1];
  }

  return (0);
}

int
archMesh2DomBipartO (
const ArchMesh2 * const       archptr,
const ArchMesh2Dom * const    domptr,
ArchMesh2Dom * restrict const dom0ptr,
ArchMesh2Dom * restrict const dom1ptr)
{
  if ((domptr->c[0][0] == domptr->c[0][1]) &&     /* Return if cannot bipartition more */
      (domptr->c[1][0] == domptr->c[1][1]))
    return (1);

  if (domptr->c[1][1] == domptr->c[1][0]) {       /* If the Y dimension cannot be cut */
    dom0ptr->c[0][0] = domptr->c[0][0];           /* Cut in the X dimension           */
    dom0ptr->c[0][1] = (domptr->c[0][0] + domptr->c[0][1]) / 2;
    dom1ptr->c[0][0] = dom0ptr->c[0][1] + 1;
    dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = dom1ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = dom1ptr->c[1][1] = domptr->c[1][1];
  }
  else {                                          /* If the Y dimension can be cut, cut it */
    dom0ptr->c[0][0] = dom1ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = (domptr->c[1][0] + domptr->c[1][1]) / 2;
    dom1ptr->c[1][0] = dom0ptr->c[1][1] + 1;
    dom1ptr->c[1][1] = domptr->c[1][1];
  }

  return (0);
}

int
archMesh2DomBipartU (
const ArchMesh2 * const       archptr,
const ArchMesh2Dom * const    domptr,
ArchMesh2Dom * restrict const dom0ptr,
ArchMesh2Dom * restrict const dom1ptr)
{
  if ((domptr->c[0][0] == domptr->c[0][1]) &&     /* Return if cannot bipartition more */
      (domptr->c[1][0] == domptr->c[1][1]))
    return (1);

  if ((domptr->c[0][1] - domptr->c[0][0]) >       /* Split domain unevenly along largest dimension */
      (domptr->c[1][1] - domptr->c[1][0])) {
    dom0ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = (domptr->c[0][0] + domptr->c[0][1] + domptr->c[0][1]) / 3;
    dom1ptr->c[0][0] = dom0ptr->c[0][1] + 1;
    dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = dom1ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = dom1ptr->c[1][1] = domptr->c[1][1];
  }
  else {
    dom0ptr->c[0][0] = dom1ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = dom1ptr->c[0][1] = domptr->c[0][1];
    dom0ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = (domptr->c[1][0] + domptr->c[1][1] + domptr->c[1][1]) / 3;
    dom1ptr->c[1][0] = dom0ptr->c[1][1] + 1;
    dom1ptr->c[1][1] = domptr->c[1][1];
  }

  return (0);
}

/* This function checks if dom1 is
** included in dom0.
** It returns:
** - 0  : if dom1 is not included in dom0.
** - 1  : if dom1 is included in dom0.
** - 2  : on error.
*/

int
archMesh2DomIncl (
const ArchMesh2 * const     archptr,
const ArchMesh2Dom * const  dom0ptr,
const ArchMesh2Dom * const  dom1ptr)
{
  if ((dom0ptr->c[0][0] <= dom1ptr->c[0][0]) &&
      (dom0ptr->c[0][1] >= dom1ptr->c[0][1]) &&
      (dom0ptr->c[1][0] <= dom1ptr->c[1][0]) &&
      (dom0ptr->c[1][1] >= dom1ptr->c[1][1]))
    return (1);

  return (0);
}

/* This function creates the MPI_Datatype for
** 2D mesh domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archMesh2DomMpiType (
const ArchMesh2 * const       archptr,
MPI_Datatype * const          typeptr)
{
  MPI_Type_contiguous (4, ANUM_MPI, typeptr);

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */

/***********************************************/
/*                                             */
/* These are the 3-dimensional mesh routines. */
/*                                             */
/***********************************************/

/* This routine loads the
** tridimensional mesh architecture.
** It returns:
** - 0   : if the architecture has been successfully read.
** - !0  : on error.
*/

int
archMesh3ArchLoad (
ArchMesh3 * restrict const  archptr,
FILE * restrict const       stream)
{
#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMesh3)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMesh3Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMesh3ArchLoad: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((intLoad (stream, &archptr->c[0]) != 1) ||
      (intLoad (stream, &archptr->c[1]) != 1) ||
      (intLoad (stream, &archptr->c[2]) != 1) ||
      (archptr->c[0] < 1) || (archptr->c[1] < 1) || (archptr->c[2] < 1)) {
    errorPrint ("archMesh3ArchLoad: bad input");
    return     (1);
  }
  archptr->dimnnbr = 3;

  return (0);
}

/* This routine saves the
** tridimensional mesh architecture.
** It returns:
** - 0   : if the architecture has been successfully written.
** - !0  : on error.
*/

int
archMesh3ArchSave (
const ArchMesh3 * const     archptr,
FILE * restrict const       stream)
{
#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMesh3)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMesh3Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMesh3ArchSave: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if (fprintf (stream, ANUMSTRING " " ANUMSTRING " " ANUMSTRING "\n",
               (Anum) archptr->c[0], (Anum) archptr->c[1], (Anum) archptr->c[2]) == EOF) {
    errorPrint ("archMesh3ArchSave: bad output");
    return     (1);
  }

  return (0);
}

/* This function returns the smallest number
** of terminal domain included in the given
** domain.
*/

ArchDomNum
archMesh3DomNum (
const ArchMesh3 * const     archptr,
const ArchMesh3Dom * const  domptr)
{
  return ((((domptr->c[2][0]  * archptr->c[1]) +  /* Return the vertex number */
             domptr->c[1][0]) * archptr->c[0]) +
             domptr->c[0][0]);
}

/* This function returns the terminal domain associated
** with the given terminal number in the architecture.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

int
archMesh3DomTerm (
const ArchMesh3 * const     archptr,
ArchMesh3Dom * const        domptr,
const ArchDomNum            domnum)
{
  if (domnum < (archptr->c[0] * archptr->c[1] * archptr->c[2])) { /* If valid label */
    domptr->c[0][0] =                             /* Set the domain                 */
    domptr->c[0][1] = domnum % archptr->c[0];
    domptr->c[1][0] =
    domptr->c[1][1] = (domnum / archptr->c[0]) % archptr->c[1];
    domptr->c[2][0] =
    domptr->c[2][1] = domnum / (archptr->c[0] * archptr->c[1]);

    return (0);
  }

  return (1);                                     /* Cannot set domain */
}

/* This function returns the number of
** elements in the cubic domain.
*/

Anum
archMesh3DomSize (
const ArchMesh3 * const     archptr,
const ArchMesh3Dom * const  domptr)
{
  return ((domptr->c[0][1] - domptr->c[0][0] + 1) *
          (domptr->c[1][1] - domptr->c[1][0] + 1) *
          (domptr->c[2][1] - domptr->c[2][0] + 1));
}

/* This function returns the average distance
** between two cubic domains (in fact the
** distance between the centers of the domains).
*/

Anum
archMesh3DomDist (
const ArchMesh3 * const     archptr,
const ArchMesh3Dom * const  dom0ptr,
const ArchMesh3Dom * const  dom1ptr)
{
  return (((abs (dom0ptr->c[0][0] + dom0ptr->c[0][1] -
                 dom1ptr->c[0][0] - dom1ptr->c[0][1]) + 1) / 2) +
          ((abs (dom0ptr->c[1][0] + dom0ptr->c[1][1] -
                 dom1ptr->c[1][0] - dom1ptr->c[1][1]) + 1) / 2) +
          ((abs (dom0ptr->c[2][0] + dom0ptr->c[2][1] -
                 dom1ptr->c[2][0] - dom1ptr->c[2][1]) + 1) / 2));
}

/* This function tries to split a cubic
** domain into two subdomains.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

int
archMesh3DomBipart (
const ArchMesh3 * const       archptr,
const ArchMesh3Dom * const    domptr,
ArchMesh3Dom * restrict const dom0ptr,
ArchMesh3Dom * restrict const dom1ptr)
{
  Anum                dimsiz[3];
  int                 dimtmp;
  int                 dimval;

  dimsiz[0] = domptr->c[0][1] - domptr->c[0][0];
  dimsiz[1] = domptr->c[1][1] - domptr->c[1][0];
  dimsiz[2] = domptr->c[2][1] - domptr->c[2][0];

  if ((dimsiz[0] | dimsiz[1] | dimsiz[2]) == 0)   /* Return if cannot bipartition more */
    return (1);

  dimval = (archptr->c[1] > archptr->c[0]) ? 1 : 0; /* Assume all subdomain dimensions are equal */
  if (archptr->c[2] > archptr->c[dimval])         /* Find priviledged dimension                  */
    dimval = 2;

  dimtmp = dimval;                                /* Find best dimension */
  if (dimsiz[(dimtmp + 1) % 3] > dimsiz[dimval])
    dimval = (dimtmp + 1) % 3;
  if (dimsiz[(dimtmp + 2) % 3] > dimsiz[dimval])
    dimval = (dimtmp + 2) % 3;

  if (dimval == 0) {                              /* Split domain in two along largest dimension */
    dom0ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = (domptr->c[0][0] + domptr->c[0][1]) / 2;
    dom1ptr->c[0][0] = dom0ptr->c[0][1] + 1;
    dom1ptr->c[0][1] = domptr->c[0][1];

    dom0ptr->c[1][0] = dom1ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = dom1ptr->c[1][1] = domptr->c[1][1];

    dom0ptr->c[2][0] = dom1ptr->c[2][0] = domptr->c[2][0];
    dom0ptr->c[2][1] = dom1ptr->c[2][1] = domptr->c[2][1];
  }
  else if (dimval == 1) {
    dom0ptr->c[0][0] = dom1ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = dom1ptr->c[0][1] = domptr->c[0][1];

    dom0ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = (domptr->c[1][0] + domptr->c[1][1]) / 2;
    dom1ptr->c[1][0] = dom0ptr->c[1][1] + 1;
    dom1ptr->c[1][1] = domptr->c[1][1];

    dom0ptr->c[2][0] = dom1ptr->c[2][0] = domptr->c[2][0];
    dom0ptr->c[2][1] = dom1ptr->c[2][1] = domptr->c[2][1];
  }
  else {
    dom0ptr->c[0][0] = dom1ptr->c[0][0] = domptr->c[0][0];
    dom0ptr->c[0][1] = dom1ptr->c[0][1] = domptr->c[0][1];

    dom0ptr->c[1][0] = dom1ptr->c[1][0] = domptr->c[1][0];
    dom0ptr->c[1][1] = dom1ptr->c[1][1] = domptr->c[1][1];

    dom0ptr->c[2][0] = domptr->c[2][0];
    dom0ptr->c[2][1] = (domptr->c[2][0] + domptr->c[2][1]) / 2;
    dom1ptr->c[2][0] = dom0ptr->c[2][1] + 1;
    dom1ptr->c[2][1] = domptr->c[2][1];
  }

  return (0);
}

/* This function checks if dom1 is
** included in dom0.
** It returns:
** - 0  : if dom1 is not included in dom0.
** - 1  : if dom1 is included in dom0.
** - 2  : on error.
*/

int
archMesh3DomIncl (
const ArchMesh3 * const     archptr,
const ArchMesh3Dom * const  dom0ptr,
const ArchMesh3Dom * const  dom1ptr)
{
  if ((dom0ptr->c[0][0] <= dom1ptr->c[0][0]) &&
      (dom0ptr->c[0][1] >= dom1ptr->c[0][1]) &&
      (dom0ptr->c[1][0] <= dom1ptr->c[1][0]) &&
      (dom0ptr->c[1][1] >= dom1ptr->c[1][1]) &&
      (dom0ptr->c[2][0] <= dom1ptr->c[2][0]) &&
      (dom0ptr->c[2][1] >= dom1ptr->c[2][1]))
    return (1);

  return (0);
}

/* This function creates the MPI_Datatype for
** 3D mesh domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archMesh3DomMpiType (
const ArchMesh3 * const       archptr,
MPI_Datatype * const          typeptr)
{
  MPI_Type_contiguous (6, ANUM_MPI, typeptr);

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */

/**********************************************/
/*                                            */
/* These are the x-dimensional mesh routines. */
/*                                            */
/**********************************************/

/* This routine loads the X-dimensional mesh
** architecture.
** It returns:
** - 0   : if the architecture has been successfully read.
** - !0  : on error.
*/

int
archMeshXArchLoad (
ArchMeshX * restrict const  archptr,
FILE * restrict const       stream)
{
  Anum                dimnnum;

#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMeshX)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMeshXDom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMeshXArchLoad: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((intLoad (stream, &archptr->dimnnbr) != 1) ||
      (archptr->dimnnbr > ARCHMESHDIMNMAX)) {
    errorPrint ("archMeshXArchLoad: bad input (1)");
    return     (1);
  }

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    if ((intLoad (stream, &archptr->c[dimnnum]) != 1) ||
        (archptr->c[dimnnum] < 1)) {
      errorPrint ("archMeshXArchLoad: bad input (2)");
      return     (1);
    }
  }

  return (0);
}

/* This routine saves the
** tridimensional torus architecture.
** It returns:
** - 0   : if the architecture has been successfully written.
** - !0  : on error.
*/

int
archMeshXArchSave (
const ArchMeshX * const     archptr,
FILE * restrict const       stream)
{
  Anum                dimnnum;

#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchMeshX)    > sizeof (ArchDummy)) ||
      (sizeof (ArchMeshXDom) > sizeof (ArchDomDummy))) {
    errorPrint ("archMeshXArchSave: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if (fprintf (stream, ANUMSTRING " ",
               (Anum) archptr->dimnnbr) == EOF) {
    errorPrint ("archMeshXArchSave: bad output (1)");
    return     (1);
  }

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    if (fprintf (stream, ANUMSTRING " ",
               (Anum) archptr->c[dimnnum]) == EOF) {
      errorPrint ("archMeshXArchSave: bad output (2)");
      return     (1);
    }
  }

  if (fprintf (stream, "\n") == EOF) {
    errorPrint ("archMeshArchSave: bad output (3)");
    return     (1);
  }

  return (0);
}

/* This routine initializes the matching
** data structure according to the number
** of vertices to be managed.
** It returns:
** - 0   : if the data structure has been
**         successfully initialized.
** - !0  : on error.
*/

int
archMeshXMatchInit (
ArchMeshXMatch * restrict const   matcptr,
const ArchMeshX * restrict const  archptr)
{
  Anum                dimnnbr;
  Anum                dimnnum;
  Anum                multnbr;

  const Anum * restrict const dimntab = archptr->c;

  dimnnbr = archptr->dimnnbr;
  multnbr = (dimntab[0] + 1) >> 1;                /* In case dimension 0, which will be coarsened first, is odd */
  for (dimnnum = 1; dimnnum < dimnnbr; dimnnum ++)
    multnbr *= dimntab[dimnnum];

  if ((matcptr->multtab = memAlloc (multnbr * sizeof (ArchCoarsenMulti))) == NULL) {
    errorPrint ("archMeshXMatchInit: out of memory");
    return     (1);
  }

  matcptr->dimnnbr = dimnnbr;                     /* Set number of dimensions                       */
  matcptr->dimnnum = 0;                           /* Set first dimension to consider for coarsening */
  memCpy (matcptr->dimntab, dimntab, dimnnbr * sizeof (Anum));
  memSet (matcptr->passtab, 0,       dimnnbr * sizeof (int));

  return (0);
}

/* This routine frees the matching data
** structure.
** It returns:
** - void  : in all cases.
*/

void
archMeshXMatchExit (
ArchMeshXMatch * restrict const matcptr)
{
  memFree (matcptr->multtab);
}

/* This routine computes a matching from
** the current state of the matching structure.
** It returns:
** - >=0  : size of matching.
** - <0   : on error.
*/

Anum
archMeshXMatchMate (
ArchMeshXMatch * restrict const     matcptr,
ArchCoarsenMulti ** restrict const  multptr)
{
  ArchCoarsenMulti * restrict coarmulttab;
  Anum                        coarvertnum;
  Anum                        finevertnbr;
  Anum                        finevertnum;
  Anum                        finevertdlt;        /* Span of blocks of fine vertices that have same characteristics        */
  Anum                        fineverttmp;        /* Next fine vertex index that will result in change in dimension index  */
  Anum * restrict             dimntab;            /* Array of mesh dimensions                                              */
  Anum                        dimnnbr;
  Anum                        dimntmp;
  Anum                        dimnnum;            /* Number of dimension that will be coarsened                            */
  Anum                        indxnbr;            /* NUmber of fine indices in coarsened dimension                         */
  Anum                        indxnum;            /* Current fine vertex index in coarsened dimension                      */
  Anum                        indxval;            /* Value of dimension index that will hold single nodes                  */
  int                         passnum;            /* Flag that indicates whether even or odd dimension indices are matched */

  dimnnbr = matcptr->dimnnbr;
  dimntab = matcptr->dimntab;
  for (dimnnum = matcptr->dimnnum, dimntmp = (dimnnum + dimnnbr - 1) % dimnnbr; /* Search for a dimension to coarsen */
       dimntab[dimnnum] <= 1; dimnnum = (dimnnum + 1) % dimnnbr) {
    if (dimnnum == dimntmp)                       /* If no dimension can be coarsened, abort */
      return (-1);
  }
  matcptr->dimnnum = (dimnnum + 1) % dimnnbr;     /* Record next dimension to process */

  for (dimntmp = 0, finevertdlt = 1;              /* Compute stride of blocks of fine vertices to be coarsened according to selected dimension */
       dimntmp < dimnnum; dimntmp ++)
    finevertdlt *= dimntab[dimntmp];
  for (finevertnbr = finevertdlt;                 /* Compute number of fine vertices */
       dimntmp < dimnnbr; dimntmp ++)
    finevertnbr *= dimntab[dimntmp];

  indxnbr = dimntab[dimnnum];                     /* Get size of selected dimension                 */
  dimntab[dimnnum] = (indxnbr + 1) >> 1;          /* Record new size of selected dimension          */
  if ((indxnbr & 1) != 0) {                       /* If dimension to consider is odd                */
    passnum = matcptr->passtab[dimnnum] ^= 1;     /* Record that pass did matter for this dimension */
    indxval = (passnum != 0) ? 0 : indxnbr - 1;   /* Set index that will contain single nodes       */
  }
  else {
    passnum = 0;                                  /* Start multinodes from first index */
    indxval = -1;                                 /* No need to have single nodes      */
  }

  coarmulttab = matcptr->multtab;
  for (finevertnum = coarvertnum = indxnum = 0, fineverttmp = finevertdlt - 1;
       finevertnum < finevertnbr; finevertnum ++) {
    if (indxnum == indxval) {                     /* If dimension index is that of single nodes */
      coarmulttab[coarvertnum].vertnum[0] =       /* Create single node                         */
      coarmulttab[coarvertnum].vertnum[1] = finevertnum;
      coarvertnum ++;
    }
    else if ((indxnum & 1) == passnum) {          /* Else if dimension index is that of first mate */
      coarmulttab[coarvertnum].vertnum[0] = finevertnum; /* Create multinode                       */
      coarmulttab[coarvertnum].vertnum[1] = finevertnum + finevertdlt;
      coarvertnum ++;
    }

    if (finevertnum == fineverttmp) {             /* If dimension stride reached */
      indxnum = (indxnum + 1) % indxnbr;          /* Increase dimension index    */
      fineverttmp += finevertdlt;                 /* Compute next step to reach  */
    }
  }

#ifdef SCOTCH_DEBUG_ARCH2
  if (coarvertnum * indxnbr != finevertnbr * ((indxnbr + 1) >> 1)) {
    errorPrint ("archMeshXMatchMate: internal error");
    return     (-1);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */

  *multptr = coarmulttab;                         /* Always provide same mating array */

  return (coarvertnum);
}

/* This function returns the smallest number
** of terminal domain included in the given
** domain.
*/

ArchDomNum
archMeshXDomNum (
const ArchMeshX * const     archptr,
const ArchMeshXDom * const  domnptr)
{
  Anum                dimnnum;
  Anum                domnnum;

  for (dimnnum = archptr->dimnnbr - 2, domnnum = domnptr->c[archptr->dimnnbr - 1][0]; dimnnum >= 0; dimnnum --)
    domnnum = (domnnum * archptr->c[dimnnum]) + domnptr->c[dimnnum][0];

  return (domnnum);                               /* Return vertex number */
}

/* This function returns the terminal domain associated
** with the given terminal number in the architecture.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

int
archMeshXDomTerm (
const ArchMeshX * const     archptr,
ArchMeshXDom * const        domnptr,
const ArchDomNum            domnnum)
{
  Anum                dimnnum;
  Anum                domntmp;

  for (dimnnum = 0, domntmp = domnnum; dimnnum < archptr->dimnnbr; dimnnum ++) { /* Set the domain */
    domnptr->c[dimnnum][0] =
    domnptr->c[dimnnum][1] = domntmp % archptr->c[dimnnum];
    domntmp /= archptr->c[dimnnum];
  }

  if (domntmp > 0)                                /* If residual is not zero, terminal domain number is invalid since too high */
    return (1);

  return (0);
}

/* This function returns the number of
** elements in the cubic domain.
*/

Anum
archMeshXDomSize (
const ArchMeshX * const     archptr,
const ArchMeshXDom * const  domnptr)
{
  Anum                dimnnum;
  Anum                domnsiz;

  for (dimnnum = 0, domnsiz = 1; dimnnum < archptr->dimnnbr; dimnnum ++)
    domnsiz *= domnptr->c[dimnnum][1] - domnptr->c[dimnnum][0] + 1;

  return (domnsiz);
}

/* This function returns the average distance
** between two cubic domains (in fact the
** distance between the centers of the domains).
*/

Anum
archMeshXDomDist (
const ArchMeshX * const     archptr,
const ArchMeshXDom * const  dom0ptr,
const ArchMeshXDom * const  dom1ptr)
{
  Anum                dimnnum;
  Anum                distval;
  Anum                disttmp;

  for (dimnnum = 0, distval = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    disttmp = abs (dom0ptr->c[dimnnum][0] + dom0ptr->c[dimnnum][1] -
                   dom1ptr->c[dimnnum][0] - dom1ptr->c[dimnnum][1]);
    distval += (disttmp > archptr->c[dimnnum]) ? (2 * archptr->c[dimnnum] - disttmp) : disttmp;
  }

  return (distval >> 1);
}

/* This function sets the biggest
** domain available for this
** architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archMeshXDomFrst (
const ArchMeshX * const       archptr,
ArchMeshXDom * restrict const domptr)
{
  Anum                dimnnum;

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    domptr->c[dimnnum][0] = 0;
    domptr->c[dimnnum][1] = archptr->c[dimnnum] - 1;
  }

  return (0);
}

/* This routine reads domain information
** from the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archMeshXDomLoad (
const ArchMeshX * const       archptr,
ArchMeshXDom * restrict const domptr,
FILE * restrict const         stream)
{
  Anum                dimnnum;

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    if ((intLoad (stream, &domptr->c[dimnnum][0]) != 1) ||
        (intLoad (stream, &domptr->c[dimnnum][1]) != 1) ||
        (domptr->c[dimnnum][0] > domptr->c[dimnnum][1])  ||
        (domptr->c[dimnnum][0] < 0)) {
      errorPrint ("archMeshXDomLoad: bad input");
      return     (1);
    }
  }

  return (0);
}

/* This routine saves domain information
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archMeshXDomSave (
const ArchMeshX * const     archptr,
const ArchMeshXDom * const  domptr,
FILE * restrict const       stream)
{
  Anum                dimnnum;

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    if (fprintf (stream, ANUMSTRING " " ANUMSTRING " ",
                 (Anum) domptr->c[dimnnum][0],
                 (Anum) domptr->c[dimnnum][1]) == EOF) {
      errorPrint ("archMeshXDomSave: bad output");
      return     (1);
    }
  }

  return (0);
}

/* This function tries to split a cubic
** domain into two subdomains.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

int
archMeshXDomBipart (
const ArchMeshX * const       archptr,
const ArchMeshXDom * const    domptr,
ArchMeshXDom * restrict const dom0ptr,
ArchMeshXDom * restrict const dom1ptr)
{
  Anum                archdimsizmax;              /* Maximum span on largest architecture dimension */
  Anum                domndimsizmax;              /* Maximum span on largest domain dimension       */
  Anum                domndimval;                 /* Dimension to be split                          */
  Anum                domndimflg;                 /* Flag set if subdomain can be bipartitioned     */
  Anum                domndimtmp;
  Anum                dimnnum;

  for (dimnnum = domndimval = archptr->dimnnbr - 1, archdimsizmax = domndimflg = 0, domndimsizmax = -1;
       dimnnum >= 0; dimnnum --) {
    Anum                archdimsiz;
    Anum                domndimsiz;
    Anum                domndim0;
    Anum                domndim1;

    dom0ptr->c[dimnnum][0] =                       /* Set up subdomain data as copy of original domain data */
    dom1ptr->c[dimnnum][0] = domndim0 = domptr->c[dimnnum][0];
    dom0ptr->c[dimnnum][1] =
    dom1ptr->c[dimnnum][1] = domndim1 = domptr->c[dimnnum][1];

    domndimsiz  = domndim1 - domndim0;            /* Span on current dimension            */
    domndimflg |= domndimsiz;                     /* Flag set if at least one is not zero */

    if (domndimsiz < domndimsizmax)               /* If dimension is too small, skip it */
      continue;
    archdimsiz = archptr->c[dimnnum];
    if ((domndimsiz == domndimsizmax) &&          /* If dimension to split is not priviledged, skip it */
        (archdimsiz <= archdimsizmax))
      continue;

    archdimsizmax = archdimsiz;                   /* Record dimension to split */
    domndimsizmax = domndimsiz;
    domndimval    = dimnnum;
  }

  if (domndimflg == 0)                            /* Return if cannot bipartition more */
    return (1);

  domndimtmp = (domptr->c[domndimval][0] + domptr->c[domndimval][1]) / 2;
  dom0ptr->c[domndimval][1] = domndimtmp;
  dom1ptr->c[domndimval][0] = domndimtmp + 1;

  return (0);
}

/* This function checks if dom1 is
** included in dom0.
** It returns:
** - 0  : if dom1 is not included in dom0.
** - 1  : if dom1 is included in dom0.
** - 2  : on error.
*/

int
archMeshXDomIncl (
const ArchMeshX * const     archptr,
const ArchMeshXDom * const  dom0ptr,
const ArchMeshXDom * const  dom1ptr)
{
  Anum                dimnnum;

  for (dimnnum = 0; dimnnum < archptr->dimnnbr; dimnnum ++) {
    if ((dom1ptr->c[dimnnum][0] < dom0ptr->c[dimnnum][0]) ||
        (dom1ptr->c[dimnnum][1] > dom0ptr->c[dimnnum][1]))
      return (0);
  }

  return (1);
}

/* This function creates the MPI_Datatype for
** xD torus domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archMeshXDomMpiType (
const ArchMeshX * const     archptr,
MPI_Datatype * const        typeptr)
{
  MPI_Type_contiguous (2 * archptr->dimnnbr, ANUM_MPI, typeptr);

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */
