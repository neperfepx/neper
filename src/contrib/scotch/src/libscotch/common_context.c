/* Copyright 2019,2021-2023,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_context.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the execution       **/
/**                context management routines.            **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 07 may 2019     **/
/**                                 to   : 11 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "common_thread.h"
#include "common_thread_system.h"
#include "common_values.h"

/*
**  The static and global variables.
*/

static ValuesContext        valudat = { NULL, NULL, 0, 0, 0, 0, 0 };

/***********************************/
/*                                 */
/* These routines handle contexts. */
/*                                 */
/***********************************/

/* This routine initializes the given context.
** It returns:
** - void  : in all cases.
*/

void
contextInit (
Context * const             contptr)
{
  contptr->thrdptr = NULL;                        /* Thread context not initialized yet       */
  contptr->randptr = &intranddat;                 /* Use global random generator by default   */
  contptr->valuptr = NULL;                        /* Allow user library to provide its values */

  intRandInit (&intranddat);                      /* Make sure random context is initialized before cloning */
}

/* This routine frees a context structure.
** It returns:
** - void  : in all cases.
*/

void
contextExit (
Context * const             contptr)
{
  if (contptr->thrdptr != NULL) {                 /* If context has been commited */
    threadContextExit (contptr->thrdptr);
    memFree (contptr->thrdptr);
  }
  if (contptr->randptr != &intranddat)            /* If not global random generator */
    memFree (contptr->randptr);
  if (contptr->valuptr != &valudat) {             /* If not global values array           */
    if (contptr->valuptr->dataptr != contptr->valuptr->dainptr) /* If modified data array */
      memFree (contptr->valuptr->dataptr);
    memFree (contptr->valuptr);
  }

#ifdef SCOTCH_DEBUG_CONTEXT1
  contptr->thrdptr = NULL;
  contptr->randptr = NULL;
  contptr->valuptr = NULL;
#endif /* SCOTCH_DEBUG_CONTEXT1 */
}

/* This routine allocates the features of the
** given context if they have not been already.
** It returns:
** - 0   : if the initialization succeeded.
** - !0  : on error.
*/

int
contextCommit (
Context * const             contptr)
{
  int                 o;

  o = 0;
  if (contptr->thrdptr == NULL)                   /* If thread context not already initialized */
    o = contextThreadInit (contptr);

  if (contptr->valuptr == NULL)                   /* If no values provided by user library */
    contptr->valuptr = &valudat;                  /* Set default data to avoid any crash   */

  return (o);
}

/************************************/
/*                                  */
/* These routines handle the random */
/* generator features of contexts.  */
/*                                  */
/************************************/

/* This routine creates a clone of the default
** pseudo-random in its current state and places
** it in the given context.
** It returns:
** - 0   : if the cloning succeeded.
** - !0  : on error.
*/

int
contextRandomClone (
Context * const             contptr)
{
  IntRandContext *    randptr;

  if (contptr->randptr == &intranddat) {          /* If no clone yet, allocate space for it */
    if ((randptr = memAlloc (sizeof (IntRandContext))) == NULL) {
      errorPrint ("contextRandomClone: out of memory");
      return (1);
    }
    contptr->randptr = randptr;                   /* Context now uses its private clone */
  }
  else                                            /* Else re-use old clone */
    randptr = contptr->randptr;

  *randptr = intranddat;                          /* Clone default generator (always already initialized) */

  return (0);
}

/************************************/
/*                                  */
/* These routines handle the thread */
/* features of contexts.            */
/*                                  */
/************************************/

/* This routine initializes the thread context
** of an execution context.
** It returns:
** - 0   : if the thread context has been initialized.
** - !0  : on error.
*/

int
contextThreadInit2 (
Context * const             contptr,
const int                   thrdnbr,
const int * const           coretab)
{
  if (contptr->thrdptr != NULL) {
    errorPrint ("contextThreadInit2: thread context already allocated");
    return (1);
  }

  if ((contptr->thrdptr = memAlloc (sizeof (ThreadContext))) == NULL) {
    errorPrint ("contextThreadInit2: out of memory");
    return (1);
  }

  if (threadContextInit (contptr->thrdptr, thrdnbr, coretab) != 0) {
    memFree (contptr->thrdptr);
    contptr->thrdptr = NULL;
    return (1);
  }

  return (0);
}

int
contextThreadInit (
Context * const             contptr)
{
  int                 thrdnbr;

#ifdef SCOTCH_PTHREAD
#ifdef SCOTCH_PTHREAD_NUMBER
  thrdnbr = SCOTCH_PTHREAD_NUMBER;                /* If prescribed number defined at compile time, use it as default */
#else /* SCOTCH_PTHREAD_NUMBER */
  thrdnbr = -1;                                   /* Else take the number of cores at run time */
#endif /* SCOTCH_PTHREAD_NUMBER */
  thrdnbr = envGetInt ("SCOTCH_PTHREAD_NUMBER", thrdnbr);
#else /* SCOTCH_PTHREAD */
  thrdnbr = 1;                                    /* No threads allowed */
#endif /* SCOTCH_PTHREAD */

  return (contextThreadInit2 (contptr, thrdnbr, NULL));
}

/* This routine, to be called only by the leader thread
** of the current threading environment, splits this
** context into two sub-contexts, each of them inheriting
** of (almost) half of the threads of the initial context.
** A new, independent, pseudo-random generator is created
** for the second sub-context. The two sub-contexts are
** initialized as if a contextInit() were called on each
** of them, after which each sub-context leader runs the
** user-provided function within its sub-context.
** It returns:
** - 0  : if sub-contexts could be created.
** - 1  : if initial context is too small.
*/

static
void
contextThreadLaunchSplit2 (
ThreadDescriptor * restrict const descptr,        /*+ Thread descriptor in initial context +*/
ContextSplit * restrict const     spltptr)        /*+ Data structure for splitting context +*/
{
  const int           thrdnbr = threadNbr (descptr);
  const int           thrdnum = threadNum (descptr);
  const int           thrdmed = (thrdnbr + 1) / 2; /* Median thread number */

  if (thrdnum < thrdmed) {                        /* If thread belongs to first sub-context */
    threadContextImport2 (spltptr->conttab[0].thrdptr, thrdnum); /* Lock all worker threads */

    if (thrdnum == 0) {                           /* If leader thread of sub-context 0 */
      spltptr->funcptr   (&spltptr->conttab[0], 0, spltptr->paraptr);
      threadContextExit2 (spltptr->conttab[0].thrdptr);
    }
  }
  else {                                          /* Thread belongs to second sub-context             */
    threadContextImport2 (spltptr->conttab[1].thrdptr, thrdnum - thrdmed); /* Lock all worker threads */

    if (thrdnum == thrdmed) {                     /* If leader thread of sub-context 1 */
      spltptr->funcptr   (&spltptr->conttab[1], 1, spltptr->paraptr);
      threadContextExit2 (spltptr->conttab[1].thrdptr);
    }
  }
}

int
contextThreadLaunchSplit (
Context * const             contptr,
ContextSplitFunc const      funcptr,              /* Function to launch  */
void * const                paraptr)              /* Function parameters */
{
  ContextSplit              spltdat;              /* Data structure for passing arguments         */
  ThreadContext             thrdtab[2];           /* Thread contexts for both sub-contexts        */
  IntRandContext            randdat;              /* Pseudo-random context for second sub-context */
  const int                 thrdnbr = contextThreadNbr (contptr);

  if (thrdnbr <= 1)                               /* If current context too small or inactive, nothing to do */
    return (1);

  spltdat.conttab[0].thrdptr = &thrdtab[0];
  spltdat.conttab[0].randptr = contptr->randptr;  /* Re-use pseudo-random generator of initial context in sub-context 0 */
  spltdat.conttab[0].valuptr = contptr->valuptr;
  spltdat.conttab[1].thrdptr = &thrdtab[1];
  spltdat.conttab[1].randptr = &randdat;          /* Set independent pseudo-random generator for sub-context 1 */
  spltdat.conttab[1].valuptr = contptr->valuptr;
  spltdat.funcptr = funcptr;
  spltdat.paraptr = paraptr;

  threadContextImport1 (&thrdtab[0], (thrdnbr + 1) / 2); /* Prepare sub-contexts to host threads */
  threadContextImport1 (&thrdtab[1],  thrdnbr      / 2);

  intRandProc (&randdat, (int) intRandVal2 (contptr->randptr)); /* Initialize new generator from existing one */
  intRandSeed (&randdat,       intRandVal2 (contptr->randptr));

  threadLaunch (contptr->thrdptr, (ThreadFunc) contextThreadLaunchSplit2, (void *) &spltdat); /* Launch all threads of initial context */

  return (0);
}
