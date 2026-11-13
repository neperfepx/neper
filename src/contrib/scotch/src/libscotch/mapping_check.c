/* Copyright 2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : mapping_check.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the mapping        **/
/**                checking function.                      **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 17 jul 2024     **/
/**                                 to   : 17 jul 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"

/***********************************/
/*                                 */
/* These routines handle mappings. */
/*                                 */
/***********************************/

/* This routine checks the consistency
** of the given mapping.
** It returns:
** - 0   : if mapping data are consistent.
** - !0  : on error.
*/

int
mapCheck (
const Mapping * const       mappptr)
{
  int                       mincval;              /* Flag set if mapping incomplete      */
  const Anum * restrict     parttax;
  const Arch * restrict     archptr;
  const ArchDom * restrict  domntab;
  ArchDom                   domnorg;              /* Initial domain for the architecture */
  Anum                      domnmax;
  Anum                      domnnbr;
  Anum                      domnnum;
  Gnum                      baseval;
  Gnum                      vertnnd;
  Gnum                      vertnum;

  if ((mappptr->grafptr == NULL) &&               /* If empty mapping */
      (mappptr->archptr == NULL))
    return (0);

  if ((mappptr->grafptr == NULL) ||               /* If incomplete mapping initialization */
      (mappptr->archptr == NULL)) {
    errorPrint ("mapCheck: inconsistent array data (1)");
    return (1);
  }

  archptr = mappptr->archptr;
  domnnbr = mappptr->domnnbr;
  domnmax = mappptr->domnmax;
  if ((domnmax < 0) ||
      (domnmax < domnnbr)) {
    errorPrint ("mapCheck: invalid domain numbers");
    return (1);
  }

  if (domnnbr == 0)                               /* If nothing to do */
    return (0);

  parttax = mappptr->parttax;
  domntab = mappptr->domntab;
  if (parttax == NULL) {                          /* Partition array should have been allocated */
    errorPrint ("mapCheck: inconsistent array data (2)");
    return (1);
  }
  if (domntab == NULL) {                          /* Domain array should have been allocated */
    errorPrint ("mapCheck: inconsistent array data (3)");
    return (1);
  }

  mincval = mappptr->flagval & MAPPINGINCOMPLETE; /* Check if mapping may be incomplete */
  baseval = mappptr->grafptr->baseval;
  vertnnd = mappptr->grafptr->vertnnd;

  for (vertnum = baseval; vertnum < vertnnd; vertnum ++) {
    Anum                domnnum;

    domnnum = parttax[vertnum];
    if ((domnnum >= domnnbr) ||                   /* If domain number is too big                */
        ((domnnum < 0) &&                         /* Or if it is too small and then             */
         ((domnnum != -1) || (mincval == 0)))) {  /* If not equal to -1 for incomplete mappings */
      errorPrint ("mapCheck: invalid part array");
      return (1);
    }
  }

  archDomFrst (archptr, &domnorg);                /* Get initial domain for the architecture */
  for (domnnum = 0; domnnum < domnnbr; domnnum ++) {
    if (archDomIncl (archptr, &domnorg, &domntab[domnnum]) != 1) {
      errorPrint ("mapCheck: invalid domain array");
      return (1);
    }
  }

  return (0);
}
