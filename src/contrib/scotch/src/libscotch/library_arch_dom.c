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
/**   NAME       : library_arch_dom.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Amaury JACQUES (v6.0)                   **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the target   **/
/**                architecture domain handling routines   **/
/**                of the libSCOTCH library.               **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 16 apr 2019     **/
/**                                 to   : 25 apr 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "scotch.h"

/***************************************/
/*                                     */
/* These routines are the C API for    */
/* the architecture domain handling    */
/* routines.                           */
/*                                     */
/***************************************/

/*+ This function returns the smallest number
*** of terminal domain included within the
*** given domain.
*** It returns:
*** - SCOTCH_Num  : in all cases.
+*/

SCOTCH_Num
SCOTCH_archDomNum (
SCOTCH_Arch * const           archptr,
const SCOTCH_ArchDom * const  domnptr)
{
  return (archDomNum ((Arch * const) archptr, (const ArchDom * const) domnptr));
}

/*+ This function computes the terminal domain
*** associated with the given terminal number.
*** It returns:
*** - 0  : if label is valid and domain has been updated.
*** - 1  : if label is invalid.
*** - 2  : on error.
+*/

int
SCOTCH_archDomTerm (
SCOTCH_Arch * const         archptr,
SCOTCH_ArchDom *  const     domnptr,
const SCOTCH_Num            domnnum)
{
  return (archDomTerm ((Arch * const) archptr, (ArchDom * const) domnptr, domnnum));
}

/*+ This function returns the number
*** of elements in the given domain.
*** It returns:
*** - >0  : size of the domain.
*** - 0   : on error.
+*/

SCOTCH_Num
SCOTCH_archDomSize (
SCOTCH_Arch * const           archptr,
const SCOTCH_ArchDom * const  domnptr)
{
  return (archDomSize ((Arch * const) archptr, (const ArchDom * const) domnptr));
}

/*+ This function returns the weight
*** of the given domain.
*** It returns:
*** - >0  : weight of the domain.
*** - 0   : on error.
+*/

SCOTCH_Num
SCOTCH_archDomWght (
SCOTCH_Arch * const           archptr,
const SCOTCH_ArchDom * const  domnptr)
{
  return (archDomWght ((Arch * const) archptr, (const ArchDom * const) domnptr));
}

/*+ This function yields the average
*** distance between two domains.
*** It returns:
*** - !-1  : distance between subdomains.
*** - -1   : on error.
+*/

SCOTCH_Num
SCOTCH_archDomDist (
SCOTCH_Arch * const           archptr,
const SCOTCH_ArchDom * const  dom0ptr,
const SCOTCH_ArchDom * const  dom1ptr)
{
  return (archDomDist ((Arch * const) archptr, (const ArchDom * const) dom0ptr, (const ArchDom * const) dom1ptr));
}

/*+ This routine fills the contents of the
*** given domain with the biggest domain
*** of the given target architecture.
*** It returns:
*** - 0   : if domain could be created.
*** - !0  : on error.
+*/

int
SCOTCH_archDomFrst (
SCOTCH_Arch * const         archptr,
SCOTCH_ArchDom * const      domnptr)
{
  return (archDomFrst ((Arch * const) archptr, (ArchDom * const) domnptr));
}

/*+ This function tries to split a domain into
*** two subdomains. The two subdomains are created
*** so that subdomain 0 has same T_domNum as
*** original domain.
*** It returns:
*** - 0  : if bipartitioning succeeded.
*** - 1  : if bipartitioning could not be performed.
*** - 2  : on error.
+*/

int
SCOTCH_archDomBipart (
SCOTCH_Arch * const           archptr,
const SCOTCH_ArchDom * const  domnptr,
SCOTCH_ArchDom * const        dom0ptr,
SCOTCH_ArchDom * const        dom1ptr)
{
  return (archDomBipart ((Arch * const) archptr, (const ArchDom * const) domnptr, (ArchDom * const) dom0ptr, (ArchDom * const) dom1ptr));
}
