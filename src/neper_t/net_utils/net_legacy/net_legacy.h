/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NET_LEGACY_H
#define NET_LEGACY_H

  extern int oneDIntEltPos (int *array, int beg, int end, int value);
  extern int oneDIntCompress (int *array, int beg, int end);
  extern int oneDIntDeleteNCompress (int *array, int beg, int end, int
				     value, int loop);
#endif				/* NET_LEGACY_H */

#ifdef __cplusplus
}
#endif
