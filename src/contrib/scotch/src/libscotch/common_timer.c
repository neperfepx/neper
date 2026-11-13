/* Copyright 2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_timer.c                          **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the timers.         **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 22 aug 2023     **/
/**                                 to   : 06 oct 2026     **/
/**                                                        **/
/************************************************************/

#include "module.h"
#include "common.h"

/*
**  The global variables.
*/

int                         timerNbr = 0;         /* Number of timers */
Clock *                     timerTab = NULL;      /* Array of timers  */

/*******************/
/*                 */
/* Timer routines. */
/*                 */
/*******************/

int
timerInit (
const int                   timenbr)
{
  int               timenum;

#ifdef COMMON_DEBUG
  if (timerTab != NULL) {
    errorPrint ("timerInit: timers already initialized");
    return (1);
  }
#endif /* COMMON_DEBUG */
  if ((timerTab = memAlloc (timenbr * sizeof (Clock))) == NULL) {
    errorPrint ("timerInit: out of memory");
    return (1);
  }

  timerNbr = timenbr;

  for (timenum = 0; timenum < timenbr; timenum ++)
    clockInit (&timerTab[timenum]);

  return (0);
}

void
timerExit (void)
{
#ifdef COMMON_DEBUG
  if (timerTab == NULL) {
    errorPrint ("timerExit: timers not initialized");
    return;
  }
#endif /* COMMON_DEBUG */

  memFree (timerTab);

  timerNbr = 0;
  timerTab = NULL;
}

void
timerStart (
const int                   timenum)
{
#ifdef COMMON_DEBUG
  if (timerTab == NULL) {
    errorPrint ("timerStart: timers not initialized");
    return;
  }
  if ((timenum < 0) || (timenum >= timerNbr)) {
    errorPrint ("timerStart: invalid timer number");
    return;
  }
#endif /* COMMON_DEBUG */

  clockStart (&timerTab[timenum]);
}

void
timerStop (
const int                   timenum)
{
#ifdef COMMON_DEBUG
  if (timerTab == NULL) {
    errorPrint ("timerStop: timers not initialized");
    return;
  }
  if ((timenum < 0) || (timenum >= timerNbr)) {
    errorPrint ("timerStop: invalid timer number");
    return;
  }
#endif /* COMMON_DEBUG */

  clockStop (&timerTab[timenum]);
}

double
timerVal (
const int                   timenum)
{
#ifdef COMMON_DEBUG
  if (timerTab == NULL) {
    errorPrint ("timerStop: timers not initialized");
    return (-1.0);
  }
  if ((timenum < 0) || (timenum >= timerNbr)) {
    errorPrint ("timerVal: invalid timer number");
    return (-1.0);
  }
#endif /* COMMON_DEBUG */

  return ((double) clockVal (&timerTab[timenum]));
}
