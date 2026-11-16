/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include <stdlib.h>
#include "ut_compat.h"

#ifdef _WIN32

// POSIX 48-bit linear congruential generator: X(n+1) = (a X(n) + c) mod 2^48,
// with a = 0x5DEECE66D and c = 0xB (as drand48, erand48 and srand48)

static unsigned short ut_compat_state[3] = { 0x330E, 0xABCD, 0x1234 };

static double
ut_compat_next (unsigned short x[3])
{
  unsigned long long X = ((unsigned long long) x[2] << 32)
    | ((unsigned long long) x[1] << 16) | x[0];

  X = (0x5DEECE66DULL * X + 0xBULL) & 0xFFFFFFFFFFFFULL;

  x[0] = (unsigned short) (X & 0xFFFF);
  x[1] = (unsigned short) ((X >> 16) & 0xFFFF);
  x[2] = (unsigned short) ((X >> 32) & 0xFFFF);

  return (double) X / 281474976710656.0;        // 2^48
}

double
erand48 (unsigned short xsubi[3])
{
  return ut_compat_next (xsubi);
}

double
drand48 (void)
{
  return ut_compat_next (ut_compat_state);
}

void
srand48 (long seed)
{
  ut_compat_state[0] = 0x330E;
  ut_compat_state[1] = (unsigned short) (seed & 0xFFFF);
  ut_compat_state[2] = (unsigned short) ((seed >> 16) & 0xFFFF);
}

ssize_t
getline (char **plineptr, size_t *pn, FILE *stream)
{
  size_t len = 0;
  int c;

  if (!plineptr || !pn || !stream)
    return -1;

  if (!*plineptr || *pn == 0)
  {
    *pn = 128;
    *plineptr = (char *) malloc (*pn);
    if (!*plineptr)
      return -1;
  }

  while ((c = fgetc (stream)) != EOF)
  {
    if (len + 2 > *pn)
    {
      char *tmp = (char *) realloc (*plineptr, 2 * (*pn));
      if (!tmp)
        return -1;
      *plineptr = tmp;
      *pn *= 2;
    }
    (*plineptr)[len++] = (char) c;
    if (c == '\n')
      break;
  }

  if (len == 0)
    return -1;

  (*plineptr)[len] = '\0';

  return (ssize_t) len;
}

#endif
