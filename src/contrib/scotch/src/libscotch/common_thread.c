/* Copyright 2012-2015,2018,2019,2021,2022,2023,2025 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_thread.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module provides routines to ease   **/
/**                the use of Posix threads.               **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 04 jul 2012     **/
/**                                 to   : 27 apr 2015     **/
/**                # Version 7.0  : from : 03 jun 2018     **/
/**                                 to   : 28 sep 2025     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_COMMON_THREAD

#include "module.h"
#include "common.h"
#include "common_thread.h"
#include "common_thread_system.h"

/*****************************/
/*                           */
/* Thread handling routines. */
/*                           */
/*****************************/

/* This routine returns the number of threads
** of the given comtext.
** It returns:
** - (int)  : in all cases.
*/

int
threadContextNbr (
ThreadContext * const       contptr)
{
  return (contptr->thrdnbr);
}

/* This routine returns the pointer to the
** parameter of the parallel routine being
** currently executed in the given context.
** It returns:
** - (void *)  : in all cases.
*/

void *
threadContextParam (
ThreadContext * const       contptr)
{
  return ((void *) contptr->paraptr);
}

#ifdef COMMON_PTHREAD

/* This routine initializes a thread context
** structure using the given number of threads.
** It returns:
** - 0   : if thread context initialized.
** - !0  : on error.
*/

int
threadContextInit (
ThreadContext * const       contptr,
int                         thrdnbr,
const int * const           coretab)
{
  ThreadDescriptor *  desctab;
  int                 corenbr;
  int                 corenum;
  int                 thrdnum;

  threadProcessStateSave (contptr);               /* Save state of main thread        */
  corenbr = threadProcessCoreNbr (contptr);       /* Get number of assigned cores     */
  if (thrdnbr < 0)                                /* If unspecified number of threads */
    thrdnbr = corenbr;                            /* Take as many as available        */

  contptr->barrnbr = 0;
  contptr->bainnum = 0;
  contptr->funcptr = NULL;
  contptr->paraptr = NULL;
  contptr->thrdnbr = thrdnbr;

  if (thrdnbr == 1) {                             /* If no threads wanted       */
    contptr->statval = THREADCONTEXTSTATUSDWN;    /* Do not start thread system */
    return (0);
  }

  if ((desctab = memAlloc (thrdnbr * sizeof (ThreadDescriptor))) == NULL) {
    errorPrint ("threadContextInit: out of memory");
    return (1);
  }

  pthread_mutex_init (&contptr->lockdat, NULL);
  pthread_cond_init  (&contptr->conddat, NULL);
  contptr->statval = THREADCONTEXTSTATUSRDY;

  for (thrdnum = 1; thrdnum < thrdnbr; thrdnum ++) { /* Launch threads from 1 to (thrdnbr - 1) */
    desctab[thrdnum].contptr = contptr;
    desctab[thrdnum].thrdnum = thrdnum;
    corenum = (coretab != NULL) ? (coretab[thrdnum] % corenbr) : threadProcessCoreNum (contptr, thrdnum);

    if (threadCreate (&desctab[thrdnum], thrdnum, corenum) != 0) {
      errorPrint ("threadContextInit: cannot create thread (%d)", thrdnum);
      contptr->thrdnbr = thrdnum;                 /* Terminate all threads that have been launched to date */
      threadContextExit (contptr);
      return (1);
    }
  }
  desctab[0].contptr = contptr;
  desctab[0].thrdnum = 0;
  corenum = (coretab != NULL) ? (coretab[0] % corenbr) : threadProcessCoreNum (contptr, 0);
  threadCreate (&desctab[0], 0, corenum);         /* Set affinity of local thread (slaves use saved main thread mask) */

  threadContextBarrier (contptr);                 /* Ensure all slave threads have started before cleaning-up resources */

  memFree (desctab);

  return (0);
}

/* This routine frees the given thread context
** without restoring the thread affinity mask.
** It is used to free sub-contexts, such as
** those created by splitting an existing
** context.
** It returns:
** - void  : in all cases.
*/

void
threadContextExit2 (
ThreadContext * const       contptr)
{
  int                 thrdnbr;
  int                 barrnbr;

  thrdnbr = contptr->thrdnbr;
  if (thrdnbr <= 1)                               /* If thread system not started, nothing to do */
    return;

  pthread_mutex_lock (&contptr->lockdat);
  contptr->statval = THREADCONTEXTSTATUSDWN;      /* Request shutdow       */
  pthread_cond_broadcast (&contptr->conddat);     /* Wake-up slave threads */
  pthread_mutex_unlock   (&contptr->lockdat);

  thrdnbr --;                                     /* Do not count ourselves in the spin-lock barrier                */
  do {                                            /* Spin-lock until all slave threads have exited critical section */
    pthread_mutex_lock (&contptr->lockdat);
    barrnbr = contptr->barrnbr;
    pthread_mutex_unlock (&contptr->lockdat);
  } while (barrnbr != thrdnbr);

  pthread_cond_destroy  (&contptr->conddat);      /* Destroy critical section features */
  pthread_mutex_destroy (&contptr->lockdat);
}

/* This routine frees the given thread context
** and restores the thread affinity mask.
** It returns:
** - void  : in all cases.
*/

void
threadContextExit (
ThreadContext * const       contptr)
{
  threadContextExit2 (contptr);                   /* Exit context and release threads */

  threadProcessStateRestore (contptr);            /* Restore state of main thread */
}

/* This routine performs a barrier on the given
** thread context. One could have used the
** standard pthread barrier routine, but it is
** not always available, and the cond and mutex
** objects already exist in the thread context,
** so it is more efficient to re-use them.
** It returns:
** - 0   : thread is not the last thread.
** - !0  : thread is the last thread.
*/

int
threadContextBarrier (
ThreadContext * const       contptr)
{
  int                 barrnbr;
  unsigned int        bainnum;
  int                 o;

  if (contptr->thrdnbr == 1)                      /* If thread system not started, return immediately */
    return (PTHREAD_BARRIER_SERIAL_THREAD);

  pthread_mutex_lock (&contptr->lockdat);

  barrnbr = contptr->barrnbr + 1;
  bainnum = contptr->bainnum;

  o = 0;                                          /* Assume thread will not be the last one */

  if (barrnbr == contptr->thrdnbr) {              /* If last thread         */
    contptr->barrnbr = 0;                         /* Reset barrier counters */
    contptr->bainnum = bainnum + 1;
    pthread_cond_broadcast (&contptr->conddat);   /* Wake-up all sleeping threads      */
    o = PTHREAD_BARRIER_SERIAL_THREAD;            /* Last thread returns special value */
  }
  else {                                          /* Not last thread         */
    contptr->barrnbr = barrnbr;                   /* One more thread blocked */
    do
      pthread_cond_wait (&contptr->conddat, &contptr->lockdat);
    while (contptr->bainnum == bainnum);
  }

  pthread_mutex_unlock (&contptr->lockdat);

  return (o);
}

/* This routine performs a specific barrier on
** the given thread context. It is called at the
** end of a round, to reset the statval flag to
** its wait state.
** It returns:
** - void  : in all cases.
*/

static inline
void
threadWaitBarrier (
ThreadContext * const       contptr)
{
  int                 barrnbr;
  unsigned int        bainnum;

  pthread_mutex_lock (&contptr->lockdat);

  barrnbr = contptr->barrnbr + 1;
  bainnum = contptr->bainnum;

  if (barrnbr == contptr->thrdnbr) {              /* If last thread                      */
    contptr->statval = THREADCONTEXTSTATUSRDY;    /* Round has completed for all threads */
#ifdef COMMON_DEBUG
    contptr->funcptr = NULL;
    contptr->paraptr = NULL;
#endif /* COMMON_DEBUG */
    contptr->barrnbr = 0;                         /* Reset barrier counters */
    contptr->bainnum = bainnum + 1;
    pthread_cond_broadcast (&contptr->conddat);   /* Wake-up all sleeping threads */
  }
  else {                                          /* Not last thread         */
    contptr->barrnbr = barrnbr;                   /* One more thread blocked */
    do
      pthread_cond_wait (&contptr->conddat, &contptr->lockdat);
    while (contptr->bainnum == bainnum);
  }

  pthread_mutex_unlock (&contptr->lockdat);
}

/* This routine is the wait loop for all slave
** threads of the thread context.
** It returns:
** - NULL  : in all cases.
*/

static
void *
threadWait (
ThreadDescriptor * const    thrdptr)              /* Initial thread descriptor; may me destroyed afterwards */
{
  ThreadContextStatus statval;                    /* Status being read at wake-up time                                 */
  ThreadDescriptor    thrddat;                    /* Permanent descriptor passed to the routines of the current thread */

  thrddat = *thrdptr;                             /* Make local copy of thread descriptor before barrier */

  threadContextBarrier (thrddat.contptr);         /* Wait for all threads to complete initialization */

  while (1) {
    pthread_mutex_lock (&thrddat.contptr->lockdat);
    while ((statval = thrddat.contptr->statval) == THREADCONTEXTSTATUSRDY) /* As long as nothing to do, go on sleeping */
      pthread_cond_wait (&thrddat.contptr->conddat, &thrddat.contptr->lockdat);
    pthread_mutex_unlock (&thrddat.contptr->lockdat);

    if (statval != THREADCONTEXTSTATUSRUN)        /* Exit loop if not asked to run a routine */
      break;

    thrddat.contptr->funcptr (&thrddat, (void *) thrddat.contptr->paraptr); /* Call routine */

    threadWaitBarrier (thrddat.contptr);          /* Special barrier to reset to wait state */
  }

#ifdef COMMON_DEBUG
  if (statval != THREADCONTEXTSTATUSDWN)
    errorPrint ("threadWait: invalid status");
#endif /* COMMON_DEBUG */

  pthread_mutex_lock (&thrddat.contptr->lockdat); /* Acknowledge termination */
  thrddat.contptr->barrnbr ++;
  pthread_mutex_unlock (&thrddat.contptr->lockdat);

  return (NULL);
}

/* This routine, called by the master thread,
** launches a parallel task across the given
** thread context.
** It returns:
** - void  : in all cases.
*/

void
threadLaunch (
ThreadContext * const       contptr,
ThreadFunc const            funcptr,              /* Function to launch  */
void * const                paraptr)              /* Function parameters */
{
  ThreadDescriptor    thrddat;

  thrddat.contptr = contptr;                      /* Fill master descriptor */
  thrddat.thrdnum = 0;

  if (contptr->thrdnbr == 1) {                    /* If thread system not started, run function alone */
    funcptr (&thrddat, paraptr);
    return;
  }

  pthread_mutex_lock (&contptr->lockdat);         /* In case writes are not atomic */
  contptr->funcptr = funcptr;                     /* Set function parameters       */
  contptr->paraptr = paraptr;
  contptr->statval = THREADCONTEXTSTATUSRUN;      /* Allow other threads to run */
  pthread_cond_broadcast (&contptr->conddat);     /* Wake them up               */
  pthread_mutex_unlock (&contptr->lockdat);

  funcptr (&thrddat, paraptr);                    /* Run function along with other threads */

  threadWaitBarrier (contptr);                    /* Special barrier to reset to wait state */
}

/* This routine performs a synchronous
** reduction operation on the given block
** of threads. The routine is called only
** if the two threads in the reduction binary
** tree exist.
** A final, global barrier may be necessary for
** all threads to benefit from the result of the
** reduction operation.
** It returns:
** - void  : in all cases.
*/

#ifdef COMMON_PTHREAD_REDUCE_CANONICAL
void
threadReduce (
const ThreadDescriptor * const  thrdptr,
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadReduceFunc const          redfptr,          /* Pointer to reduction routine  */
const int                       rootnum,          /* Root of reduction             */
const void * const              globptr)          /* Global data for reduction     */
{
  int                 thrdnsk;                    /* Rank of thread in skewed reduction tree */
  int                 thrdmsk;

  ThreadContext * const     contptr = thrdptr->contptr; /* Fast accesses */
  const int                 thrdnbr = contptr->thrdnbr;
  const int                 thrdnum = thrdptr->thrdnum;

#ifdef COMMON_DEBUG
  if ((rootnum < 0) || (rootnum >= thrdnbr)) {
    errorPrint ("threadReduce: invalid root number (1)");
    return;
  }
#endif /* COMMON_DEBUG */

  if (thrdnbr <= 1)                               /* If thread system not started, nothing to do */
    return;

  thrdnsk = (thrdnum + thrdnbr - rootnum) % thrdnbr;
  for (thrdmsk = 1; thrdmsk < thrdnbr; thrdmsk <<= 1) {
    int                 thrdesk;                  /* Skewed rank of end thread */

    threadContextBarrier (contptr);

    thrdesk = thrdnsk ^ thrdmsk;                  /* Get skewed rank of end thread */

    if (thrdesk < thrdnbr) {                      /* If end thread exists            */
      if (thrdesk > thrdnsk) {                    /* If we are on the receiving side */
        int                 thrdend;
        int                 thrddlt;

        thrdend = (thrdesk + rootnum) % thrdnbr;
        thrddlt = thrdend - thrdnum;
        redfptr (dataptr, (void *) ((byte *) dataptr + thrddlt * datasiz), globptr); /* Call reduction routine */
      }
      else                                        /* We are on the sending side       */
        thrdnsk += thrdnbr;                       /* Make sure we will no longer work */
    }
  }

  threadContextBarrier (contptr);
}
#else /* COMMON_PTHREAD_REDUCE_CANONICAL */
void
threadReduce (
const ThreadDescriptor * const  thrdptr,
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadReduceFunc const          redfptr,          /* Pointer to reduction routine  */
const int                       rootnum,          /* Root of reduction             */
const void * const              globptr)          /* Global data for reduction     */
{
  ThreadContext * const     contptr = thrdptr->contptr; /* Fast accesses */
  const int                 thrdnbr = contptr->thrdnbr;
  const int                 thrdnum = thrdptr->thrdnum;

#ifdef COMMON_DEBUG
  if ((rootnum < 0) || (rootnum >= thrdnbr)) {
    errorPrint ("threadReduce: invalid root number (1)");
    return;
  }
#endif /* COMMON_DEBUG */

  if (thrdnbr <= 1)                               /* If thread system not started, nothing to do */
    return;

  threadContextBarrier (contptr);

  if (thrdnum == rootnum) {                       /* If we are the root */
    int                 thrdtmp;

    for (thrdtmp = 1; thrdtmp < thrdnbr; thrdtmp ++) {
      int                 thrdesk;                /* Skewed rank of end thread */

      thrdesk = (rootnum + thrdtmp) % thrdnbr - rootnum;
      redfptr (dataptr, (void *) ((byte *) dataptr + thrdesk * datasiz), globptr); /* Call reduction routine */
    }
  }

  threadContextBarrier (contptr);
}
#endif /* COMMON_PTHREAD_REDUCE_CANONICAL */

/* This routine performs a synchronous
** scan operation on the given block of
** threads. It requires a dummy area for
** storing every other result, hence the
** phase number that is passed to the
** auxiliary routine.
** Both areas should be set with initial
** values, depending on the start phase
** number.
** It returns:
** - void  : in all cases.
*/

#ifdef COMMON_PTHREAD_SCAN_CANONICAL
void
threadScan (
const ThreadDescriptor * const  thrdptr,          /* Pointer to thread header      */
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadScanFunc const            scafptr,          /* Scan function                 */
const void * const              globptr)          /* Global data for reduction     */
{
  void *              cellptr;                    /* Pointer to local data block */
  int                 thrdmsk;                    /* Thread number skew mask     */
  int                 phasnum;                    /* Phase number                */

  ThreadContext * const     contptr = thrdptr->contptr; /* Fast accesses */
  const int                 thrdnbr = contptr->thrdnbr;
  const int                 thrdnum = thrdptr->thrdnum;

  if (thrdnbr <= 1)                               /* If thread system not started, nothing to do */
    return;

  cellptr = dataptr;

  for (thrdmsk = 1, phasnum = 0; thrdmsk < thrdnbr; thrdmsk <<= 1) {
    threadContextBarrier (contptr);               /* Barrier on all threads, even those which do not participate */

    if (cellptr != NULL) {                        /* If local thread is still active */
      int                 thrdend;

      thrdend = thrdnum - thrdmsk;                /* Get rank of end thread */
      if (thrdend >= 0) {                         /* If end slot exists     */
        scafptr (cellptr, (void *) ((byte *) cellptr - thrdmsk * datasiz), phasnum, phasnum ^ 1, globptr);
        phasnum ^= 1;                             /* Next destination will be the opposite */
      }
      else {                                      /* End slot does not exist                           */
        scafptr (cellptr, NULL, phasnum, phasnum ^ 1, globptr); /* Copy value for next steps to use it */
        phasnum = 0;                              /* No need to move it afterwards                     */
        cellptr = NULL;                           /* Do not consider this thread again                 */
      }
    }
  }

  if (phasnum != 0)                               /* If accumulated local value of last round not in place */
    scafptr (cellptr, NULL, phasnum, 0, globptr); /* Move value in place before returning                  */

  threadContextBarrier (contptr);
}
#else /* COMMON_PTHREAD_SCAN_CANONICAL */
void
threadScan (
const ThreadDescriptor * const  thrdptr,          /* Pointer to thread header      */
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadScanFunc const            scafptr,          /* Scan function                 */
const void * const              globptr)          /* Global data for reduction     */
{
  byte *              cellptr;                    /* Pointer to local data block */
  int                 thrdtmp;

  ThreadContext * const     contptr = thrdptr->contptr; /* Fast accesses */
  const int                 thrdnbr = contptr->thrdnbr;
  const int                 thrdnum = thrdptr->thrdnum;

  if (thrdnbr <= 1)                               /* If thread system not started, nothing to do */
    return;

  threadContextBarrier (contptr);

  if (thrdnum == 0) {
    for (thrdtmp = thrdnbr - 1, cellptr = (byte *) dataptr;
         thrdtmp > 0; thrdtmp --, cellptr += datasiz)
      scafptr ((void *) (cellptr + datasiz), (void *) cellptr, 0, 0, globptr);
  }

  threadContextBarrier (contptr);
}
#endif /* COMMON_PTHREAD_SCAN_CANONICAL */

/* This routine, to be called only by the master
** thread of an outside threading environment,
** creates a Scotch context to be populated by
** its fellow threads.
** It returns:
** - void  : in all cases.
*/

void
threadContextImport1 (
ThreadContext * const       contptr,
const int                   thrdnbr)
{
  contptr->thrdnbr = thrdnbr;
  contptr->paraptr = NULL;
  contptr->funcptr = NULL;
  contptr->barrnbr = 0;
  contptr->bainnum = 0;

  if (thrdnbr == 1) {                             /* If no threads wanted       */
    contptr->statval = THREADCONTEXTSTATUSDWN;    /* Do not start thread system */
    return;
  }

  pthread_mutex_init (&contptr->lockdat, NULL);
  pthread_cond_init  (&contptr->conddat, NULL);
  contptr->statval = THREADCONTEXTSTATUSRDY;
}

/* This routine, to be called by all threads of
** an outside threading environment once the master
** has successfully returned from threadContextImport1(),
** populates the given Scotch context by the slave
** threads which are put to sleep and gives back
** control to the master thread.
** It returns:
** - void  : in all cases.
*/

void
threadContextImport2 (
ThreadContext * const       contptr,
const int                   thrdnum)              /*+ Number of thread in its outside environment +*/
{
  if (thrdnum != 0) {
    ThreadDescriptor        thrddat;

    thrddat.contptr = contptr;
    thrddat.thrdnum = thrdnum;

    threadWait (&thrddat);                        /* Lock slave threads */
  }
  else
    threadContextBarrier (contptr);
}

#endif /* COMMON_PTHREAD */

/**********************************/
/*                                */
/* Thread handling routine stubs. */
/*                                */
/**********************************/

#ifndef COMMON_PTHREAD

int
threadContextInit (
ThreadContext * const       contptr,
const int                   thrdnbr,
const int * const           coretab)
{
  contptr->thrdnbr = 1;                           /* Only main thread will be active */
  contptr->statval = THREADCONTEXTSTATUSDWN;      /* Thread system is not functional */

  return (0);
}

/*
**
*/

void
threadContextExit2 (
ThreadContext * restrict const  contptr)
{
}

/*
**
*/

void
threadContextExit (
ThreadContext * restrict const  contptr)
{
}

/*
**
*/

int
threadContextBarrier (
ThreadContext * const       contptr)
{
  return (PTHREAD_BARRIER_SERIAL_THREAD);         /* Last and only thread returns special value */
}

/*
**
*/

void
threadLaunch (
ThreadContext * const       contptr,
ThreadFunc const            funcptr,              /* Function to launch  */
void * const                paraptr)              /* Function parameters */
{
  ThreadDescriptor    thrddat;

  thrddat.contptr = contptr;                      /* Fill master descriptor */
  thrddat.thrdnum = 0;

  funcptr (&thrddat, paraptr);                    /* Run function alone */
}

/*
**
*/

void
threadReduce (
const ThreadDescriptor * const  thrdptr,
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadReduceFunc const          redfptr,          /* Pointer to reduction routine  */
const int                       rootnum,          /* Root of reduction             */
const void * const              globptr)          /* Global data for reduction     */
{
#ifdef COMMON_DEBUG
  if (rootnum != 0)
    errorPrint ("threadReduce: invalid root number (2)");
#endif /* COMMON_DEBUG */
}

/*
**
*/

void
threadScan (
const ThreadDescriptor * const  thrdptr,          /* Pointer to thread header      */
void * const                    dataptr,          /* Local data object             */
const size_t                    datasiz,          /* Size of per-thread data block */
ThreadScanFunc const            scafptr,          /* Scan function                 */
const void * const              globptr)          /* Global data for reduction     */
{
}

/*
**
*/

void
threadContextImport1 (
ThreadContext * const       contptr,
const int                   thrdnbr)
{
  contptr->thrdnbr = 1;                           /* Only main thread will be active */
  contptr->statval = THREADCONTEXTSTATUSDWN;      /* Thread system is not functional */
}

/*
**
*/

void
threadContextImport2 (
ThreadContext * const       contptr,
const int                   thrdnum)              /*+ Number of thread in its outside environment +*/
{
}

#endif /* COMMON_PTHREAD */

/*****************************/
/*                           */
/* Thread affinity routines. */
/*                           */
/*****************************/

#ifdef COMMON_PTHREAD

static
int
threadCreate (
ThreadDescriptor * const    descptr,
const int                   thrdnum,
const int                   corenum)
{
  pthread_t           thidval;
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  cpu_set_t           cpusdat;
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */

  if (thrdnum > 0) {                              /* Do not create master thread */
    if (pthread_create (&thidval, NULL, (void * (*) (void *)) threadWait, (void *) descptr) != 0) {
      errorPrint ("threadCreate: cannot launch thread (%d)", thrdnum);
      return (1);
    }
    pthread_detach (thidval);                     /* Nobody will wait for us to join */
  }
  else
    thidval = pthread_self ();

#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  if ((corenum >= 0) && (corenum < CPU_SETSIZE)) {
    CPU_ZERO (&cpusdat);
    CPU_SET  (corenum, &cpusdat);
    pthread_setaffinity_np (thidval, sizeof (cpu_set_t), &cpusdat);
  }
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */

  return (0);
}

/* This routine returns the number of available threads
** to run the process: either the number of threads assigned
** to the process by the environment (e.g., an MPI launcher)
** or the system number of threads.
** It returns:
** - >0  : number of threads, in all cases.
*/

static
int
threadProcessCoreNbr (
ThreadContext * const       contptr)
{
  int                 corenbr;
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  corenbr = CPU_COUNT (&contptr->savedat.cpusdat);
#else /* COMMON_PTHREAD_AFFINITY_LINUX */
  corenbr = threadSystemCoreNbr ();
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */

  return (corenbr);
}

/* This routine returns the number of the core to be
** associated with the given thread number.
** It returns:
** - x  : number of the core associated with the thread.
*/

static
int
threadProcessCoreNum (
ThreadContext * const       contptr,
int                         thrdnum)
{
  int                 corenum;
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  int                 corenbr;

  corenbr  = CPU_COUNT (&contptr->savedat.cpusdat);
  thrdnum %= corenbr;                             /* Round-robin on thread allocation */

  for (corenum = 0; thrdnum >= 0; corenum ++) {   /* For all potential cores       */
    if (CPU_ISSET (corenum, &contptr->savedat.cpusdat)) { /* If core is available  */
      if (thrdnum <= 0)                           /* And it is the one we want     */
        break;                                    /* We have found our core number */
      thrdnum --;                                 /* One less available core       */
    }
  }
#else /* COMMON_PTHREAD_AFFINITY_LINUX */
  corenum = thrdnum;
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */

  return (corenum);
}

/* This routine saves the thread context of
** the master thread.
** It returns:
** - void  : in all cases.
*/

static
void
threadProcessStateSave (
ThreadContext * const       contptr)
{
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  pthread_getaffinity_np (pthread_self (), sizeof (cpu_set_t), &contptr->savedat.cpusdat);
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
}

/* This routine restores the thread context
** of the master thread.
** It returns:
** - void  : in all cases.
*/

static
void
threadProcessStateRestore (
ThreadContext * const       contptr)
{
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
  pthread_setaffinity_np (pthread_self (), sizeof (cpu_set_t), &contptr->savedat.cpusdat);
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
}

#endif /* COMMON_PTHREAD */
