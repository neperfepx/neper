/* Copyright 2021,2023,2025 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : context.c                               **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles contexts within     **/
/**                the libScotch routines.                 **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 03 oct 2021     **/
/**                                 to   : 14 sep 2025     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"

/*
**  The static and global variables.
*/

static struct ContextValuesData_ {
  INT                       vinttab[CONTEXTOPTIONNUMNBR];
  double                    vdbltab[CONTEXTOPTIONDBLNBR + 1]; /* TRICK: temporary hack: +1 since ISO C does not accept zero-sized arrays */
} contextvaluesdat = { {
#ifdef SCOTCH_DETERMINISTIC
                              1
#else /* SCOTCH_DETERMINISTIC */
                              0
#endif /* SCOTCH_DETERMINISTIC */
                              ,
#if ((defined SCOTCH_DETERMINISTIC) || (defined COMMON_RANDOM_FIXED_SEED))
                              1
#else /* ((defined SCOTCH_DETERMINISTIC) || (defined COMMON_RANDOM_FIXED_SEED)) */
                              0
#endif /* ((defined SCOTCH_DETERMINISTIC) || (defined COMMON_RANDOM_FIXED_SEED)) */
  }, { -1.0 } };                                  /* Temporary hack: dummy value since ISO C does not accept zero-sized arrays */

/***********************************/
/*                                 */
/* These routines handle contexts. */
/*                                 */
/***********************************/

/* This routine initializes the values of a context
** according to the needs of the libScotch.
** It returns:
** - 0  : in all cases.
*/

int
contextOptionsInit (
Context * const             contptr)
{
  int                 o;

  o = contextValuesInit (contptr, &contextvaluesdat, sizeof (contextvaluesdat),
                         CONTEXTOPTIONNUMNBR, (byte *) &contextvaluesdat.vinttab - (byte *) &contextvaluesdat,
                         CONTEXTOPTIONDBLNBR, (byte *) &contextvaluesdat.vdbltab - (byte *) &contextvaluesdat);

  if (o == 0) {                                   /* If values array allocated, update it with existing environment variables */
    INT                 deteval;
    INT                 rafival;

    contextValuesGetInt (contptr, CONTEXTOPTIONNUMDETERMINISTIC, &deteval);
    contextValuesSetInt (contptr, CONTEXTOPTIONNUMDETERMINISTIC, envGetInt ("SCOTCH_DETERMINISTIC", deteval));

    contextValuesGetInt (contptr, CONTEXTOPTIONNUMRANDOMFIXEDSEED, &rafival);
    contextValuesSetInt (contptr, CONTEXTOPTIONNUMRANDOMFIXEDSEED, envGetInt ("SCOTCH_RANDOM_FIXED_SEED", rafival));
  }

  return (o);
}
