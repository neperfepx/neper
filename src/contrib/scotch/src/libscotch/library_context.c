/* Copyright 2019,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_context.c                       **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the context  **/
/**                management routines of the libSCOTCH    **/
/**                library.                                **/
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
#include "context.h"
#include "library_context.h"
#include "scotch.h"

/****************************************/
/*                                      */
/* These routines are the C API for     */
/* execution context structure handling */
/* routines.                            */
/*                                      */
/****************************************/

/* This routine reserves a memory area
** of a size sufficient to store a
** context structure.
** It returns:
** - !NULL  : if the initialization succeeded.
** - NULL   : on error.
*/

SCOTCH_Context *
SCOTCH_contextAlloc (void)
{
  return ((SCOTCH_Context *) memAlloc (sizeof (SCOTCH_Context)));
}

/* This routine returns the size, in bytes,
** of a SCOTCH_Context structure.
** It returns:
** - > 0  : in all cases.
*/

int
SCOTCH_contextSizeof (void)
{
  return (sizeof (SCOTCH_Context));
}

/* This routine initializes the opaque
** graph structure used to handle graphs
** in the Scotch library.
** It returns:
** - 0   : if the initialization succeeded.
** - !0  : on error.
*/

int
SCOTCH_contextInit (
SCOTCH_Context * const      libcontptr)
{
  Context * const           contptr = (Context *) libcontptr;

  if (sizeof (SCOTCH_Context) < sizeof (Context)) {
    errorPrint (STRINGIFY (SCOTCH_contextInit) ": internal error");
    return (1);
  }

  contextInit (contptr);

  if (contextOptionsInit (contptr)) {
    errorPrint (STRINGIFY (SCOTCH_contextInit) ": cannot create option array");
    return (1);
  }

  return (0);
}

/* This routine frees the contents of the
** given opaque context structure.
** It returns:
** - void  : in all cases.
*/

void
SCOTCH_contextExit (
SCOTCH_Context * const      libcontptr)
{
  contextExit ((Context *) libcontptr);
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
SCOTCH_contextRandomClone (
SCOTCH_Context * const      libcontptr)
{
  return (contextRandomClone ((Context *) libcontptr));
}

/* This routine resets the random generator
** of the given context, to simulate a start
** from scratch.
** It returns:
** - void  : in all cases.
*/

void
SCOTCH_contextRandomReset (
SCOTCH_Context * const      libcontptr)
{
  intRandReset (((Context *) libcontptr)->randptr);
}

/* This routine sets the value of the
** random seed.
** It returns:
** - void  : in all cases.
*/

void
SCOTCH_contextRandomSeed (
SCOTCH_Context * const      libcontptr,
const SCOTCH_Num            seedval)
{
  intRandSeed (((Context *) libcontptr)->randptr, (UINT64) seedval);
}

/************************************/
/*                                  */
/* These routines handle the thread */
/* features of contexts.            */
/*                                  */
/************************************/

/* This routine, called by the master thread of
** a thread pool, initiates the environment to
** capture this thread pool within the given
** opaque context structure.
** It returns:
** - 0   : if thread context prepared.
** - !0  : on error.
*/

int
SCOTCH_contextThreadImport1 (
SCOTCH_Context * const      libcontptr,
const int                   thrdnbr)
{
  Context * restrict const  contptr = (Context *) libcontptr;

  if (contptr->thrdptr != NULL) {
    errorPrint (STRINGIFY (SCOTCH_contextThreadImport1) ": thread context already allocated");
    return (1);
  }

  if ((contptr->thrdptr = memAlloc (sizeof (ThreadContext))) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_contextThreadImport1) ": out of memory");
    return (1);
  }

  threadContextImport1 (contptr->thrdptr, thrdnbr);

  return (0);
}

/* This routine, called by all the threads of
** a thread pool, captures the slave threads
** and yields back control to the master thread.
** It returns:
** - 0   : if thread context finalized.
** - !0  : on error.
*/

int
SCOTCH_contextThreadImport2 (
SCOTCH_Context * const      libcontptr,
const int                   thrdnum)
{
  Context * restrict const  contptr = (Context *) libcontptr;

  if (contptr->thrdptr == NULL) {
    errorPrint (STRINGIFY (SCOTCH_contextThreadImport2) ": thread context not allocated");
    return (1);
  }

  threadContextImport2 (contptr->thrdptr, thrdnum);

  return (0);
}

/* This routine creates a thread context with
** the given number of threads, possibly mapped
** onto the given set of cores.
** It returns:
** - 0   : if thread context created.
** - !0  : on error.
*/

int
SCOTCH_contextThreadSpawn (
SCOTCH_Context * const      libcontptr,
const int                   thrdnbr,
const int * const           coretab)
{
  return (contextThreadInit2 ((Context *) libcontptr, thrdnbr, ((void *) coretab == (void *) &thrdnbr) ? NULL : coretab));
}

/*********************************/
/*                               */
/* These routines handle context */
/* options.                      */
/*                               */
/*********************************/

/* This routine gets an interger option value
** from the given context.
** It returns:
** - 0   : if the value was obtained.
** - !0  : invalid option number.
*/

int
SCOTCH_contextOptionGetNum (
SCOTCH_Context * const      libcontptr,
const int                   optinum,
SCOTCH_Num * const          optiptr)
{
  return (contextValuesGetInt ((Context *) libcontptr, optinum, optiptr));
}

/* This routine sets an interger option value
** in the given context.
** It returns:
** - 0   : if the value was set.
** - !0  : on error.
*/

int
SCOTCH_contextOptionSetNum (
SCOTCH_Context * const      libcontptr,
const int                   optinum,
const SCOTCH_Num            optival)
{
  SCOTCH_Num          optitmp;                    /* Working option value */
  int                 o;

  optitmp = optival;                              /* Set working value */
  o = 0;                                          /* Assume no error   */

  switch (optinum) {
    case CONTEXTOPTIONNUMRANDOMFIXEDSEED :
      if (optitmp != 0)
        optitmp = 1;                              /* Only two values available */
      break;
    case CONTEXTOPTIONNUMDETERMINISTIC :
      if (optitmp != 0) {
        optitmp = 1;                              /* Only two values available */

        o = contextValuesSetInt ((Context *) libcontptr, CONTEXTOPTIONNUMRANDOMFIXEDSEED, 1); /* If deterministic behavior wanted, use fixed random seed */
      }
      break;
    default :
      errorPrint (STRINGIFY (SCOTCH_contextOptionSetNum) ": invalid option name");
      return (1);
  }

  return (o || contextValuesSetInt ((Context *) libcontptr, optinum, optitmp));
}

/* This routine parses a context option
** string and sets the option values of
** the given context accordingly.
** It returns:
** - 0   : if the string was properly parsed.
** - !0  : on error.
*/

static ContextOptionArg     contextOptionTab[] = {
  { 'd',  CONTEXTOPTIONNUMDETERMINISTIC,   1 },
  { 'u',  CONTEXTOPTIONNUMDETERMINISTIC,   0 },
  { 'f',  CONTEXTOPTIONNUMRANDOMFIXEDSEED, 1 },
  { 'r',  CONTEXTOPTIONNUMRANDOMFIXEDSEED, 0 },
  { '\0', -1, -1 } };

int
SCOTCH_contextOptionParse (
SCOTCH_Context * const      libcontptr,
const char *                parastr)
{
  const char *        paraptr;
  int                 o;

  o = 0;                                          /* Assume everything will be all right */
  for (paraptr = parastr; *paraptr != '\0'; ) {
    char                        paraval;
    ContextOptionArg * restrict argsptr;

    while (isspace (paraval = *paraptr))          /* Advance to next selector */
      paraptr ++;
    if (! isalpha (paraval)) {
      errorPrint (STRINGIFY (SCOTCH_contextOptionParse) ": invalid syntax before \"%s\"", paraptr);
      return (1);
    }

    for (argsptr = contextOptionTab; argsptr->nameval != '\0'; argsptr ++) {
      if (argsptr->nameval == paraval)            /* If parameter name found */
	break;
    }
    if (argsptr->nameval == '\0') {               /* If parameter name not found */
      errorPrint (STRINGIFY (SCOTCH_contextOptionParse) ": invalid parameter name before \"%s\"", paraptr);
      return (1);
    }

    o |= SCOTCH_contextOptionSetNum (libcontptr, argsptr->vtypidx, argsptr->vtypval); /* Set options value */

    paraptr ++;                                   /* Skip parameter name      */
    while (isspace (paraval = *paraptr))          /* Advance to next selector */
      paraptr ++;
    if (paraval == '\0')                          /* If string completed, exit loop */
      break;
    if (paraval == ',')                           /* Skip comma separator */
      paraptr ++;
  }

  return (o);
}
