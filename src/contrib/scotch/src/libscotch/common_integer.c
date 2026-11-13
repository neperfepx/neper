/* Copyright 2004,2007-2012,2014-2016,2018,2019,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_integer.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module handles the generic integer **/
/**                type.                                   **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 07 sep 1998     **/
/**                                 to   : 22 sep 1998     **/
/**                # Version 0.1  : from : 07 jan 2002     **/
/**                                 to   : 17 jan 2003     **/
/**                # Version 1.0  : from : 23 aug 2005     **/
/**                                 to   : 19 dec 2006     **/
/**                # Version 2.0  : from : 26 feb 2008     **/
/**                                 to   : 26 feb 2008     **/
/**                # Version 5.1  : from : 09 nov 2008     **/
/**                                 to   : 16 jul 2010     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 03 jun 2018     **/
/**                # Version 7.0  : from : 03 jun 2018     **/
/**                                 to   : 10 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"

/********************************/
/*                              */
/* Basic routines for fast I/O. */
/*                              */
/********************************/

/* Fast read for INT values.
** It returns:
** - 1  : on success.
** - 0  : on error.
*/

int
intLoad (
FILE * const                stream,               /*+ Stream to read from     +*/
INT * const                 valptr)               /*+ Area where to put value +*/
{
  int                 sign;                       /* Sign flag      */
  int                 car;                        /* Character read */
  INT                 val;                        /* Value          */

  sign = 0;                                       /* Assume positive constant     */
  for ( ; ; ) {                                   /* Consume whitespaces and sign */
    car = getc (stream);
    if (isspace (car))
      continue;
    if ((car >= '0') && (car <= '9'))
      break;
    if (car == '-') {
      sign = 1;
      car  = getc (stream);
      break;
    }
    if (car == '+') {
      car = getc (stream);
      break;
    }
    return (0);
  }
  if ((car < '0') || (car > '9'))                 /* If first char is non numeric */
    return (0);                                   /* Then it is an error          */
  val = car - '0';                                /* Get first digit              */
  for ( ; ; ) {
#ifdef COMMON_DEBUG
    INT                 tmp;                      /* Saved value */
#endif /* COMMON_DEBUG */

    car = getc (stream);
    if ((car < '0') || (car > '9')) {
      ungetc (car, stream);
      break;
    }
#ifdef COMMON_DEBUG
    tmp = val;
#endif /* COMMON_DEBUG */
    val = val * 10 + (car - '0');                 /* Accumulate digits */
#ifdef COMMON_DEBUG
    if (val < tmp) {                              /* In case of overflow */
      errorPrint ("intLoad: overflow");
      return (0);
    }
#endif /* COMMON_DEBUG */
  }
  *valptr = (sign != 0) ? (- val) : val;          /* Set result */

  return (1);
}

/* Write routine for INT values.
** It returns:
** - 1  : on success.
** - 0  : on error.
*/

int
intSave (
FILE * const                stream,               /*+ Stream to write to +*/
const INT                   val)                  /*+ Value to write     +*/
{
  return ((fprintf (stream, INTSTRING, (INT) val) == EOF) ? 0 : 1);
}

/**********************************/
/*                                */
/* Permutation building routines. */
/*                                */
/**********************************/

/* This routine fills an array with
** consecutive INT values, in
** ascending order.
** It returns:
** - void  : in all cases.
*/

void
intAscn (
INT * const                 permtab,              /*+ Permutation array to build +*/
const INT                   permnbr,              /*+ Number of entries in array +*/
const INT                   baseval)              /*+ Base value                 +*/
{
  INT *               permtax;
  INT                 permnum;
  INT                 permnnd;

  for (permnum = baseval, permnnd = baseval + permnbr, permtax = permtab - baseval;
       permnum < permnnd; permnum ++)
    permtax[permnum] = permnum;
}

/* This routine computes a random permutation
** of an array of INT values.
** It returns:
** - void  : in all cases.
*/

void
intPerm (
INT * const                 permtab,              /*+ Permutation array to build +*/
const INT                   permnbr,              /*+ Number of entries in array +*/
Context * restrict const    contptr)
{
  INT *               permptr;
  UINT                permrmn;

  for (permptr = permtab, permrmn = (UINT) permnbr; /* Perform random permutation */
       permrmn > 0; permptr ++, permrmn --) {
    UINT                permnum;
    INT                 permtmp;

    permnum          = intRandVal (contptr->randptr, permrmn); /* Select index to swap */
    permtmp          = permptr[0];                /* Swap it with current index        */
    permptr[0]       = permptr[permnum];
    permptr[permnum] = permtmp;
  }
}

/*************************************/
/*                                   */
/* Pseudo-random generator routines. */
/*                                   */
/*************************************/

IntRandContext              intranddat = { 0, 0 }; /*+ Global context: not initialized, process number is 0 +*/

/* This routine sets the process number that is
** used to generate a different seed across all
** processes. In order for this number to be
** taken into account, it must be followed by
** a subsequent call to intRandInit(),
** intRandReset() or intRandSeed().
** It returns:
** - void  : in all cases.
*/

void
intRandProc (
IntRandContext * const      randptr,
const int                   procnum)
{
  randptr->procval = procnum;                     /* Set process number */
}

/* These routines initialize and/or reset the seed
** used by the given pseudo-random generator.
** It returns:
** - void  : in all cases.
*/

static
void
intRandSeed2 (
IntRandState * restrict     statptr,
UINT64                      randval)
{
  UINT64              seedval;

  UINT64 * restrict const randtab = statptr->randtab; /* Fast access */

  seedval = ((UINT64) randval) * 2U + 1U;         /* Never have a zero state */

  randtab[0] = seedval ^ (seedval << 15);
  randtab[1] = seedval ^ (seedval << 24);
}

void
intRandReset (
IntRandContext * const      randptr)
{
  UINT64              randval;

  randptr->flagval = 1;                           /* Generator has been initialized */

  randval = (((UINT64) randptr->seedval) * 2U + 1U) * (((UINT64) randptr->procval) * 2U + 1U); /* Account for process index */
  intRandSeed2 (&randptr->statdat, randval);      /* Initialize state vector from random seed                               */
}

void
intRandSeed (
IntRandContext * const      randptr,
UINT64                      seedval)
{
  randptr->seedval = seedval;                     /* Save new seed */

  intRandReset (randptr);                         /* Initialize pseudo-random seed */
}

/* This routine spawns a new pseudo-random
** generator from an existing one, so that
** sub-tasks can easily get a distinct stream
** of pseudo-random numbers.
** The building of the new generator does not
** change the state of the old one, so that
** spawning can be performed in parallel by
** the tasks in an asynchronous way.
** The routine itself is not thread-safe.
** It returns:
** - void  : in all cases.
*/

void
intRandSpawn (
IntRandContext * const      roldptr,
const int                   procnum,
IntRandContext * const      rnewptr)
{
  UINT64              seedval;

  rnewptr->procval = procnum;

  seedval = roldptr->statdat.randtab[0];          /* Get value from state of old generator */

  intRandSeed (rnewptr, (INT) seedval);           /* Set seed and initialize state */
}

/* This routine initializes the pseudo-random
** generator if necessary. In order for multi-sequential
** programs to have exactly the same behavior on any
** process, the random seed does not depend on process
** rank. This routine is not really thread-safe, so it
** should not be called concurrently when it has never
** been initialized before.
** It returns:
** - void  : in all cases.
*/

void
intRandInit (
IntRandContext * const      contptr)              /*+ Random context to initialize +*/
{
  if (contptr->flagval == 0) {                    /* Non thread-safe check */
#if ! ((defined COMMON_DEBUG) || (defined COMMON_RANDOM_FIXED_SEED))
    contptr->seedval = (INT) time (NULL);         /* Set random seed if needed */
#else /* ((defined COMMON_DEBUG) || (defined COMMON_RANDOM_FIXED_SEED)) */
    contptr->seedval = 1;
#endif /* ((defined COMMON_DEBUG) || (defined COMMON_RANDOM_FIXED_SEED)) */
    intRandReset (contptr);                       /* Initialize state vector from seed */
  }
}

/* This routine loads the random state.
** It returns:
** - 0  : on success.
** - 2  : on error.
*/

static
int
intRandLoad2 (
IntRandState * restrict const statptr,            /*+ Random state to load +*/
FILE * restrict const         stream)             /*+ Stream to read from  +*/
{
  if (fscanf (stream, "%" PRIu64 "%" PRIu64,
              &statptr->randtab[0],
              &statptr->randtab[1]) != 2) {
    errorPrint ("intRandLoad2: bad input");
    return (2);
  }

  return (0);
}

int
intRandLoad (
IntRandContext * restrict const randptr,          /*+ Random context to load +*/
FILE * restrict const           stream)           /*+ Stream to read from    +*/
{
  INT                 versval;

  if (intLoad (stream, &versval) != 1) {          /* Read version number */
    errorPrint ("intRandLoad: bad input (1)");
    return (2);
  }
  if (versval != 1) {                             /* If version not one */
    errorPrint ("intRandLoad: invalid version number");
    return (2);
  }

  if (fscanf (stream, "%d%" PRIu64,
              &randptr->procval,
              &randptr->seedval) != 2) {
    errorPrint ("intRandLoad: bad input (2)");
    return (2);
  }

  randptr->flagval = 1;                           /* Assume state has been initialized */

  return (intRandLoad2 (&randptr->statdat, stream));
}

/* This routine saves the random state.
** It returns:
** - 0  : on success.
** - 1  : state cannot be saved.
** - 2  : on error.
*/

static
int
intRandSave2 (
IntRandState * restrict const statptr,            /*+ Random state to load +*/
FILE * restrict const         stream)             /*+ Stream to read from  +*/
{
  if (fprintf (stream, "%" PRIu64 "\t%" PRIu64 "\n",
               statptr->randtab[0],
               statptr->randtab[1]) < 0) {
    errorPrint ("intRandSave2: bad output");
    return (2);
  }

  return (0);
}

int
intRandSave (
IntRandContext * restrict const randptr,          /*+ Random state to load +*/
FILE * restrict const           stream)           /*+ Stream to read from  +*/
{
  if (randptr->flagval == 0) {
    errorPrint ("intRandSave: context not initialized");
    return (1);
  }

  if (fprintf (stream, "1\n%d\t%" PRIu64 "\n",
               randptr->procval,
               randptr->seedval) < 0) {
    errorPrint ("intRandSave: bad output");
    return (2);
  }

  return (intRandSave2 (&randptr->statdat, stream));
}

/* This routine computes a new pseudo-random
** INT value from the state that is passed to it.
** For speed and reproducibility reasons,
** this routine is not thread-safe. Providing
** a thread-safe routine would mean determinism
** could not be achieved in caller routines.
** It is the responsibility of application
** routines to call intRandVal() in a way that
** avoids concurrent execution and potentially
** enforces reproducibility.
** It returns:
** - x  : pseudo-random value.
*/

UINT64
intRandVal3 (
IntRandState * restrict const statptr)
{
  UINT64              x, y;                       /* State variables */

  UINT64 * restrict const randtab = statptr->randtab; /* Fast access */

  x = randtab[0];
  y = randtab[1];
  randtab[0] = y;
  x ^= x << 23;
  x  = x ^ y ^ (x >> 17) ^ (y >> 26);
  randtab[1] = x;

  return (x + y);
}

/* This routine returns a pseudo-random integer
** value in the range [0..randmax[. This routine
** is not thread-safe as it uses a global state
** variable.
** It returns:
** - x  : pseudo-random value.
*/

UINT
intRandVal (
IntRandContext * const      contptr,              /*+ Random context to load +*/
UINT                        randmax)
{
#ifdef COMMON_DEBUG
  if (contptr->flagval == 0) {
    errorPrint ("intRandVal: random generator not initialized");
    return (~0);
  }
#endif /* COMMON_DEBUG */

  return ((UINT) (intRandVal3 (&contptr->statdat) % ((UINT64) randmax)));
}

UINT64
intRandVal2 (
IntRandContext * const      contptr)              /*+ Random context to load +*/
{
  return ((UINT) intRandVal3 (&contptr->statdat));
}

/*********************/
/*                   */
/* Sorting routines. */
/*                   */
/*********************/

/* This routine sorts an array of
** INT values in ascending order
** by their first value, used as key.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intSort1asc1
#define INTSORTSIZE                 (sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t; t = *((INT *) (p)); *((INT *) (p)) = *((INT *) (q)); *((INT *) (q)) = t; } while (0)
#define INTSORTCMP(p,q)             (*((INT *) (p)) < *((INT *) (q)))
#include "common_sort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/* This routine sorts an array of pairs of
** INT values in ascending order by their
** first value, used as key.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intSort2asc1
#define INTSORTSIZE                 (2 * sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t, u; t = *((INT *) (p)); u = *((INT *) (p) + 1); *((INT *) (p)) = *((INT *) (q)); *((INT *) (p) + 1) = *((INT *) (q) + 1); *((INT *) (q)) = t; *((INT *) (q) + 1) = u; } while (0)
#define INTSORTCMP(p,q)             (*((INT *) (p)) < *((INT *) (q)))
#include "common_sort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/* This routine sorts an array of pairs of
** INT values in ascending order by both
** of their values, used as primary and
** secondary keys.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intSort2asc2
#define INTSORTSIZE                 (2 * sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t, u; t = *((INT *) (p)); u = *((INT *) (p) + 1); *((INT *) (p)) = *((INT *) (q)); *((INT *) (p) + 1) = *((INT *) (q) + 1); *((INT *) (q)) = t; *((INT *) (q) + 1) = u; } while (0)
#define INTSORTCMP(p,q)             ((*((INT *) (p)) < *((INT *) (q))) || ((*((INT *) (p)) == *((INT *) (q))) && (*((INT *) (p) + 1) < *((INT *) (q) + 1))))
#include "common_sort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/* This routine sorts an array of 3-uples of
** INT values in ascending order by their
** first value, used as key.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intSort3asc1
#define INTSORTSIZE                 (3 * sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t, u, v; t = *((INT *) (p)); u = *((INT *) (p) + 1); v = *((INT *) (p) + 2); *((INT *) (p)) = *((INT *) (q)); *((INT *) (p) + 1) = *((INT *) (q) + 1); *((INT *) (p) + 2) = *((INT *) (q) + 2); *((INT *) (q)) = t; *((INT *) (q) + 1) = u; *((INT *) (q) + 2) = v; } while (0)
#define INTSORTCMP(p,q)             (*((INT *) (p)) < *((INT *) (q)))
#include "common_sort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/* This routine sorts an array of 3-uples of
** INT values in ascending order by their
** first and second values, used as primary
** and secondary keys.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intSort3asc2
#define INTSORTSIZE                 (3 * sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t, u, v; t = *((INT *) (p)); u = *((INT *) (p) + 1); v = *((INT *) (p) + 2); *((INT *) (p)) = *((INT *) (q)); *((INT *) (p) + 1) = *((INT *) (q) + 1); *((INT *) (p) + 2) = *((INT *) (q) + 2); *((INT *) (q)) = t; *((INT *) (q) + 1) = u; *((INT *) (q) + 2) = v; } while (0)
#define INTSORTCMP(p,q)             ((*((INT *) (p)) < *((INT *) (q))) || ((*((INT *) (p)) == *((INT *) (q))) && (*((INT *) (p) + 1) < *((INT *) (q) + 1))))
#include "common_sort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP

/*****************************/
/*                           */
/* Partial sorting routines. */
/*                           */
/*****************************/

/* This routine partially sorts an array of
** pairs of INT values in ascending order by
** their first value, used as key.
** It returns:
** - void  : in all cases.
*/

#define INTSORTNAME                 intPsort2asc1
#define INTSORTSIZE                 (2 * sizeof (INT))
#define INTSORTSWAP(p,q)            do { INT t, u; t = *((INT *) (p)); u = *((INT *) (p) + 1); *((INT *) (p)) = *((INT *) (q)); *((INT *) (p) + 1) = *((INT *) (q) + 1); *((INT *) (q)) = t; *((INT *) (q) + 1) = u; } while (0)
#define INTSORTCMP(p,q)             (*((INT *) (p)) < *((INT *) (q)))
#include "common_psort.c"
#undef INTSORTNAME
#undef INTSORTSIZE
#undef INTSORTSWAP
#undef INTSORTCMP



/* This routine computes the greatest common
** divisor of two non-negative integers u and v.
** It returns:
** - x  : the GCD of u and v.
*/

INT
intGcd (
INT                         u,
INT                         v)
{
  INT                 t;

  if (v < u) {                                    /* u should always be the biggest */
    t = u;
    u = v;
    v = t;
  }

  while (v != 0) {
    t = v;
    v = u % v;
    u = t;
  }

  return (u);
}
