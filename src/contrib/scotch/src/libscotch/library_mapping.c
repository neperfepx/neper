/* Copyright 2010,2021,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_mapping.c                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains miscellaneous      **/
/**                routines for handling centralized       **/
/**                graph mappings.                         **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 17 nov 2010     **/
/**                                 to   : 17 nov 2010     **/
/**                # Version 6.1  : from : 15 mar 2021     **/
/**                                 to   : 15 mar 2021     **/
/**                # Version 7.0  : from : 21 jan 2023     **/
/**                                 to   : 06 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "scotch.h"

/****************************************/
/*                                      */
/* These routines are the C API for     */
/* mapping structure handling routines. */
/*                                      */
/****************************************/

/* This routine reserves a memory area
** of a size sufficient to store a
** SCOTCH_Mapping structure.
** It returns:
** - !NULL  : if the allocation succeeded.
** - NULL   : on error.
*/

SCOTCH_Mapping *
SCOTCH_mapAlloc (void)
{
  return ((SCOTCH_Mapping *) memAlloc (sizeof (SCOTCH_Mapping)));
}

/* This routine returns the size, in bytes,
** of a SCOTCH_Mapping structure.
** It returns:
** - > 0  : in all cases.
*/

int
SCOTCH_mapSizeof (void)
{
  return (sizeof (SCOTCH_Mapping));
}
