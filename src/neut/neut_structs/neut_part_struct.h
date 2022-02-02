/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTPART_H
#define STRUCTPART_H

  struct PART
  {
    char *mode;			// partitioning or mapping
    char *mapping;		// mapping string or architecture file
    int qty;			// number of partitions

    int *node_parts;		/* new node numbers */
    int *elt_parts;		/* new elt  numbers */

    int *NodeQty;		/* quantities of nodes in the partitions (0-based) */
    int *EltQty;		/* quantities of elts  in the partitions (0-based) */
    int *targeteltqty;		/* target quantities of elts in the partitions
				   (0-based) */
  };
  typedef struct PART PART;

#endif				/* STRUCTPART_H */

#ifdef __cplusplus
}
#endif
