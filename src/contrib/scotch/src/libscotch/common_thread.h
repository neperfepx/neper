/* Copyright 2018,2019,2021,2022,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_thread.h                         **/
/**                                                        **/
/**   AUTHORS    : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the internal data       **/
/**                declarations for the thread management  **/
/**                routines.                               **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 05 jun 2018     **/
/**                                 to   : 06 aug 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/*+ Barrier return flag. +*/

#ifndef PTHREAD_BARRIER_SERIAL_THREAD
#define PTHREAD_BARRIER_SERIAL_THREAD -1
#endif /* PTHREAD_BARRIER_SERIAL_THREAD */

/*
**  The type and structure definitions.
*/

/*+ Thread context status. +*/

typedef enum ThreadContextStatus_ {
  THREADCONTEXTSTATUSRDY,                         /*+ Ready to run +*/
  THREADCONTEXTSTATUSRUN,                         /*+ Task running +*/
  THREADCONTEXTSTATUSDWN                          /*+ Out of order +*/
} ThreadContextStatus;

/*+ Context in which parallel tasks can be launched. The abstract type is defined in "common.h". +*/

struct ThreadContext_ {
  int                           thrdnbr;          /*+ Number of threads                   +*/
  volatile ThreadContextStatus  statval;          /*+ Thread group status                 +*/
  volatile void *               paraptr;          /*+ Pointer to function parameter       +*/
#ifdef COMMON_PTHREAD
  volatile ThreadFunc           funcptr;          /*+ Function to call at run time        +*/
  volatile int                  barrnbr;          /*+ Number of threads currently blocked +*/
  volatile unsigned int         bainnum;          /*+ Number of barrier instance          +*/
  pthread_mutex_t               lockdat;          /*+ Lock for updating status            +*/
  pthread_cond_t                conddat;          /*+ Wakeup condition for slave threads  +*/
  union {                                         /*+ Context save area for main thread   +*/
    int                         dummval;          /*+ Dummy value if no affinity enabled  +*/
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
    cpu_set_t                   cpusdat;          /*+ Original thread mask of main thread +*/
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
  }                             savedat;          /*+ Save area for affinity mask         +*/
#endif /* COMMON_PTHREAD */
};

/*
**  The function prototypes.
*/

#ifdef SCOTCH_COMMON_THREAD
#ifdef COMMON_PTHREAD
static void                 threadWaitBarrier   (ThreadContext * const);
static void *               threadWait          (ThreadDescriptor * const);

static int                  threadCreate        (ThreadDescriptor * const, const int, const int);
static int                  threadProcessCoreNbr (ThreadContext * const);
static int                  threadProcessCoreNum (ThreadContext * const, int);
static void                 threadProcessStateRestore (ThreadContext * const);
static void                 threadProcessStateSave (ThreadContext * const);
#endif /* COMMON_PTHREAD */
#endif /* SCOTCH_COMMON_THREAD */
