/* Copyright 2004,2007-2013,2015,2016,2018-2021,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch.c                                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module handles the generic target  **/
/**                architecture functions.                 **/
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
/**                                 to   : 16 aug 1995     **/
/**                # Version 3.1  : from : 02 may 1996     **/
/**                                 to   : 17 jul 1996     **/
/**                # Version 3.2  : from : 07 sep 1996     **/
/**                                 to   : 28 sep 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 3.4  : from : 08 nov 2001     **/
/**                                 to   : 08 nov 2001     **/
/**                # Version 4.0  : from : 04 nov 2003     **/
/**                                 to   : 09 jan 2004     **/
/**                # Version 5.1  : from : 11 dec 2007     **/
/**                                 to   : 25 jun 2010     **/
/**                # Version 6.0  : from : 14 feb 2011     **/
/**                                 to   : 24 aug 2020     **/
/**                # Version 6.1  : from : 05 apr 2021     **/
/**                                 to   : 05 apr 2021     **/
/**                # Version 7.0  : from : 18 feb 2018     **/
/**                                 to   : 07 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "arch_cmplt.h"
#include "arch_cmpltw.h"
#include "arch_deco.h"
#include "arch_deco2.h"
#include "arch_dist.h"
#include "arch_hcub.h"
#include "arch_mesh.h"
#include "arch_sub.h"
#include "arch_tleaf.h"
#include "arch_torus.h"
#include "arch_vcmplt.h"
#include "arch_vhcub.h"

/*
**  The static definitions.
*/

static const ArchClass      archClassTab[] = { ARCHCLASSBLOCK (Cmplt,   "cmplt",    ARCHPART),
                                               ARCHCLASSBLOCK (Cmpltw,  "cmpltw",   ARCHPART),
                                               ARCHCLASSBLOCK (Cmpltws, "cmpltws",  ARCHPART),
                                               ARCHCLASSBLOCK (Deco,    "deco",     ARCHNONE),
                                               ARCHCLASSBLOCK (Deco2,   "deco",     ARCHNONE), /* Hidden, type-2 decomposition-defined architecture */
                                               ARCHCLASSBLOCK (Dist,    "dist",     ARCHNONE),
                                               ARCHCLASSBLOCK (Hcub,    "hcub",     ARCHNONE),
                                               ARCHCLASSBLOCK (Tleaf,   "tleaf",    ARCHNONE),
                                               ARCHCLASSBLOCK (Ltleaf,  "ltleaf",   ARCHNONE),
                                               ARCHCLASSBLOCK (Mesh2,   "mesh2D",   ARCHNONE),
                                               ARCHCLASSBLOCK (Mesh3,   "mesh3D",   ARCHNONE),
                                               ARCHCLASSBLOCK (MeshX,   "meshXD",   ARCHNONE),
                                               ARCHCLASSBLOCK (Sub,     "sub",      ARCHNONE),
                                               ARCHCLASSBLOCK (Torus2,  "torus2D",  ARCHNONE),
                                               ARCHCLASSBLOCK (Torus3,  "torus3D",  ARCHNONE),
                                               ARCHCLASSBLOCK (TorusX,  "torusXD",  ARCHNONE),
                                               ARCHCLASSBLOCK (Vcmplt,  "varcmplt", ARCHPART | ARCHVAR),
                                               ARCHCLASSBLOCK (Vhcub,   "varhcub",  ARCHVAR),
                                               ARCHCLASSBLOCKNULL };

/**************************************/
/*                                    */
/* These are the entry points for the */
/* generic architecture routines.     */
/*                                    */
/**************************************/

/* This routine initializes an architecture structure.
** It zeroes the architecture body so that architecture
** specific routines can check if their pointers have
** been initialized or not.
** It returns:
** - !NULL  : pointer to the target architecture.
** - NULL   : on error.
*/

int
archInit (
Arch * restrict const       archptr)
{
  memSet (archptr, 0, sizeof (Arch));             /* Initialize architecture body (arch->clasptr = NULL) */

  return (0);
}

/* This routine deletes an architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archExit (
Arch * restrict const       archptr)
{
  return (archFree (archptr));                    /* Free architecture data */
}

/* This routine frees the architecture data.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archFree (
Arch * restrict const       archptr)
{
  int                 o;

  o = 0;                                          /* Assume everything will be all right */
  if ((archptr->clasptr           != NULL) &&
      (archptr->clasptr->archFree != NULL))       /* If there is a specific freeing routine */
    o = archptr->clasptr->archFree (&archptr->data); /* Call it                             */

#ifdef SCOTCH_DEBUG_GRAPH2
  memSet (archptr, ~0, sizeof (Arch));            /* Purge graph fields */
#endif /* SCOTCH_DEBUG_GRAPH2 */

  return (o);
}

/* This routine loads an architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archLoad (
Arch * restrict const       archptr,
FILE * const                stream)
{
  const ArchClass * restrict  clasptr;            /* Pointer to architecture class */
  char                        name[256];          /* Architecture name string      */

  if (fscanf (stream, "%255s", name) != 1) {      /* Read architecture name */
    errorPrint ("archLoad: cannot load architecture type");
    return (1);
  }
  name[255] = '\0';                               /* Set end of string */

  if ((clasptr = archClass (name)) == NULL) {     /* Get class from its name */
    errorPrint ("archLoad: invalid architecture type");
    return (1);
  }

  archptr->clasptr = clasptr;                     /* Set architecture class                                                         */
  archptr->flagval = archptr->clasptr->flagval | ARCHFREE; /* Copy architecture flag before it can be modified and set freeing flag */

  if (clasptr->archLoad != NULL) {                /* If class has loading function */
    if (clasptr->archLoad (&archptr->data, stream) != 0) { /* Load class data      */
      errorPrint ("archLoad: cannot load architecture data");
      if (clasptr->archFree != NULL)              /* If freeing function exists   */
        clasptr->archFree (&archptr->data);       /* Perform clean-up             */
      memSet (archptr, 0, sizeof (Arch));         /* Initialize architecture body */
      return (1);
    }
  }

  return (0);
}

/* This routine saves an architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archSave (
const Arch * restrict const archptr,
FILE * restrict const       stream)
{
  int                 o;

  if (archptr->clasptr == NULL)                   /* If no architecture type defined */
    return (0);                                   /* Nothing to do                   */

  o = (fprintf (stream, "%s\n",                   /* Write architecture class */
                archptr->clasptr->archname) == EOF);
  if (archptr->clasptr->archSave != NULL)         /* If class has saving function        */
    o |= archptr->clasptr->archSave (&archptr->data, stream); /* Write architecture data */
  if (o != 0)
    errorPrint ("archSave: bad output");

  return (o);
}

/* This routine returns the pointer to
** the class of a given architecture
** name.
** It returns:
** - !NULL  : class pointer.
** - NULL   : on error.
*/

const ArchClass *
archClass (
const char * const          name)
{
  return (archClass2 (name, 0));                  /* Get first instance of class name */
}

const ArchClass *
archClass2 (
const char * const          name,
const int                   num)
{
  const ArchClass * restrict  clasptr;            /* Pointer to architecture class */

  for (clasptr = archClassTab; clasptr->archname != NULL; clasptr ++) { /* For all classes */
    if (strcasecmp (name, clasptr->archname) == 0) /* If class names matches               */
      return (clasptr + num);                     /* Return proper class                   */
  }

  return (NULL);                                  /* Class not found */
}

/* This routine returns the index of the
** given class.
** It returns:
** - -1  : class is NULL.
** - >0  : valid index.
*/

int
archClassNum (
const ArchClass * const     clasptr)
{
  return ((clasptr == NULL) ? -1 : (int) (clasptr - archClassTab));
}

/**************************************/
/*                                    */
/* These are the entry points for the */
/* generic domain routines. They are  */
/* used only in debugging mode, to    */
/* provide breakpoints for routines   */
/* which are else implemented as      */
/* macros for the sake of efficiency. */
/*                                    */
/**************************************/

/* This function returns the smallest number
** of terminal domain included within the
** given domain.
*/

#ifdef SCOTCH_DEBUG_ARCH2

ArchDomNum
archDomNum (
const Arch * const          archptr,
const ArchDom * const       domnptr)
{
  return (archDomNum2 (archptr, domnptr));        /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function computes the terminal domain
** associated with the given terminal number.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

int
archDomTerm (
const Arch * const          archptr,
ArchDom * restrict const    domnptr,
const ArchDomNum            domnnum)
{
  return (archDomTerm2 (archptr, domnptr, domnnum)); /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function returns the number
** of elements in the given domain.
** It returns:
** - >0  : size of the domain.
** - 0   : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

Anum
archDomSize (
const Arch * const          archptr,
const ArchDom * const       domnptr)
{
  return (archDomSize2 (archptr, domnptr));       /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function returns the weight
** of the given domain.
** It returns:
** - >0  : weight of the domain.
** - 0   : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

Anum
archDomWght (
const Arch * const          archptr,
const ArchDom * const       domnptr)
{
  return (archDomWght2 (archptr, domnptr));       /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function gives the average
** distance between two domains.
** It returns:
** - !-1  : distance between subdomains.
** - -1   : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

Anum
archDomDist (
const Arch * const          archptr,
const ArchDom * const       dom0ptr,
const ArchDom * const       dom1ptr)
{
  return (archDomDist2 (archptr, dom0ptr, dom1ptr)); /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function sets the biggest
** available domain for the given
** architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

int
archDomFrst (
const Arch * const          archptr,
ArchDom * const             domnptr)
{
  return (archDomFrst2 (archptr, domnptr));       /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This routine reads domain information
** from the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDomLoad (
const Arch * const          archptr,
ArchDom * const             domnptr,
FILE * const                stream)
{
  return (archptr->clasptr->domLoad (&archptr->data, &domnptr->data, stream)); /* Call proper routine */
}

/* This routine saves domain information
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDomSave (
const Arch * const          archptr,
const ArchDom * const       domnptr,
FILE * const                stream)
{
  return (archptr->clasptr->domSave (&archptr->data, &domnptr->data, stream)); /* Call proper routine */
}

/* This function tries to split a domain into
** two subdomains. The two subdomains are created
** so that subdomain 0 has same T_domNum as
** original domain.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

int
archDomBipart (
const Arch * const          archptr,
const ArchDom * const       domnptr,
ArchDom * const             dom0ptr,
ArchDom * const             dom1ptr)
{
  return (archDomBipart2 (archptr, domnptr, dom0ptr, dom1ptr)); /* Call proper routine */
}

#endif /* SCOTCH_DEBUG_ARCH2 */

/* This function checks if dom1 is
** included in dom0.
** It returns:
** - 0  : if dom1 is not included in dom0.
** - 1  : if dom1 is included in dom0.
** - 2  : on error.
*/

#ifdef SCOTCH_DEBUG_ARCH2

int
archDomIncl (
const Arch * const          archptr,
const ArchDom * const       dom0ptr,
const ArchDom * const       dom1ptr)
{
  return archDomIncl2 (archptr, dom0ptr, dom1ptr);
}

#endif /* SCOTCH_DEBUG_ARCH2 */
