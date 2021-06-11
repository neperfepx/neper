/* Copyright 2010,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_part_ovl_f.c              **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the Fortran API for the  **/
/**                graph partitioning routines with        **/
/**                overlap of the libSCOTCH library.       **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 29 may 2010     **/
/**                                 to   : 25 apr 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "scotch.h"

/**************************************/
/*                                    */
/* These routines are the Fortran API */
/* for the partitioning routines.     */
/*                                    */
/**************************************/

/*
**
*/

SCOTCH_FORTRAN (                      \
GRAPHPARTOVL, graphpartovl, (         \
SCOTCH_Graph * const        grafptr,  \
const SCOTCH_Num * const    partptr,  \
SCOTCH_Strat * const        straptr,  \
SCOTCH_Num * const          parttab,  \
int * const                 revaptr), \
(grafptr, partptr, straptr, parttab, revaptr))
{
  *revaptr = SCOTCH_graphPartOvl (grafptr, *partptr, straptr, parttab);
}

/* String lengths are passed at the very
** end of the argument list.
*/

SCOTCH_FORTRAN (                        \
STRATGRAPHPARTOVL, stratgraphpartovl, ( \
SCOTCH_Strat * const        straptr,    \
const char * const          string,     \
int * const                 revaptr,    \
const int                   strnbr),    \
(straptr, string, revaptr, strnbr))
{
  char * restrict     strtab;                     /* Pointer to null-terminated string */

  if ((strtab = (char *) memAlloc (strnbr + 1)) == NULL) { /* Allocate temporary space */
    errorPrint (STRINGIFY (SCOTCH_NAME_PUBLICFU (STRATGRAPHPARTOVL)) ": out of memory");
    *revaptr = 1;
    return;
  }
  memCpy (strtab, string, strnbr);                /* Copy string contents */
  strtab[strnbr] = '\0';                          /* Terminate string     */

  *revaptr = SCOTCH_stratGraphPartOvl (straptr, strtab); /* Call original routine */

  memFree (strtab);
}

/*
**
*/

SCOTCH_FORTRAN (                                  \
STRATGRAPHPARTOVLBUILD, stratgraphpartovlbuild, ( \
SCOTCH_Strat * const        straptr,              \
const SCOTCH_Num * const    flagval,              \
const SCOTCH_Num * const    partnbr,              \
const double * const        kbalval,              \
int * const                 revaptr),             \
(straptr, flagval, partnbr, kbalval, revaptr))
{
  *revaptr = SCOTCH_stratGraphPartOvlBuild (straptr, *flagval, *partnbr, *kbalval); /* Call original routine */
}
