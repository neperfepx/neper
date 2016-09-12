/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include"ut.h"

double
ut_time_subtract (struct timeval *x, struct timeval *y)
{
  double t;
  struct timeval result;
  struct timeval end;
  struct timeval *pend = &end;

  if (y)
    pend = y;
  else
    gettimeofday (pend, NULL);

  /* Perform the carry for the later subtraction by updating y.
   * */
  if (x->tv_usec < pend->tv_usec)
  {
    int nsec = (pend->tv_usec - x->tv_usec) / 1000000 + 1;
    pend->tv_usec -= 1000000 * nsec;
    pend->tv_sec += nsec;
  }
  if (x->tv_usec - pend->tv_usec > 1000000)
  {
    int nsec = (pend->tv_usec - x->tv_usec) / 1000000;
    pend->tv_usec += 1000000 * nsec;
    pend->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
   *          *      tv_usec is certainly
   *          positive. */
  result.tv_sec = x->tv_sec - pend->tv_sec;
  result.tv_usec = x->tv_usec - pend->tv_usec;

  t = -((double) result.tv_sec + (double) result.tv_usec / 1000000);

  // this avoids negative zeros
  if (t == 0)
    t = 0;

  return t;
}
