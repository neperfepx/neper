/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTNSET_H
#define STRUCTNSET_H

  struct NSET
  {
    int qty;			// quantity of nsets (recorded 1 ... qty)

    char **names;		// names
    int *nodeqty;		// number of nodes
    int **nodes;		// 0 indexed
  };
  typedef struct NSET NSET;

#endif				/* STRUCTNSET_H */

#ifdef __cplusplus
}
#endif
