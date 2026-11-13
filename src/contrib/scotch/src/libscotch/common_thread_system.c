/* Copyright 2019,2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_thread_system.c                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module provides system, low-level  **/
/**                routines to ease the use of Posix       **/
/**                threads.                                **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 24 aug 2019     **/
/**                                 to   : 06 oct 2026     **/
/**                                                        **/
/**   NOTES      : # This code mainly derives from that    **/
/**                  of the Pastix solver.                 **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "common_thread_system.h"

/*
**  The static variables.
*/

#ifdef COMMON_PTHREAD

static pthread_mutex_t      threadsystemmutedat = PTHREAD_MUTEX_INITIALIZER;
static volatile int         threadsystemflagval = 0;

static volatile int         threadsystemcorenbr = 1;

#else /* COMMON_PTHREAD */

static const int            threadsystemcorenbr = 1;

#endif /* COMMON_PTHREAD */

/*****************************/
/*                           */
/* Thread handling routines. */
/*                           */
/*****************************/

#ifdef COMMON_PTHREAD

/* This routine returns the number of cores
** known to the system.
** It returns:
** - !0  : number of cores.
*/

int
threadSystemCoreNbr (void)
{
  int                 corenbr;

  pthread_mutex_lock (&threadsystemmutedat);

  if (threadsystemflagval == 0) {
#if (defined (COMMON_OS_MACOS))
    int                 maibtab[4];
    size_t              corelen;

    corelen    = sizeof (corenbr);                /* Request hw.ncpu from Management Information Base */
    maibtab[0] = CTL_HW;
    maibtab[1] = HW_AVAILCPU;

    sysctl (maibtab, 2, &corenbr, &corelen, NULL, 0); /* Get number of cores from the system */
    if (corenbr < 1) {
      maibtab[1] = HW_NCPU;
      sysctl (maibtab, 2, &corenbr, &corelen, NULL, 0);
      if (corenbr < 1)
	corenbr = 1;
    }
#elif (defined (COMMON_OS_WINDOWS))
    SYSTEM_INFO         sinfdat;

    GetSystemInfo (&sinfdat);
    corenbr = sinfdat.dwNumberOfProcessors;
#else /* (defined (COMMON_OS_LINUX) || defined (COMMON_OS_FREEBSD) || defined (COMMON_OS_AIX)) */
    corenbr = sysconf (_SC_NPROCESSORS_ONLN);
#endif

    threadsystemcorenbr = corenbr;
    threadsystemflagval = 1;
  }
  else
    corenbr = threadsystemcorenbr;

  pthread_mutex_unlock (&threadsystemmutedat);

  return (corenbr);
}

#endif /* COMMON_PTHREAD */

/**********************************/
/*                                */
/* Thread handling routine stubs. */
/*                                */
/**********************************/

#ifndef COMMON_PTHREAD

int
threadSystemCoreNbr ()
{
  return (threadsystemcorenbr);
}

#endif /* COMMON_PTHREAD */
