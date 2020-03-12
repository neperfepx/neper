/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTEST_H
#define STRUCTTEST_H

  struct TEST
  {
    int Qty;
    int *Id;
    char *ModPreCom;
    char **PreCom;
    char **Com;
    char **ComDep;
  };
  typedef struct TEST TEST;

#endif				/* STRUCTTEST_H */

#ifdef __cplusplus
}
#endif
