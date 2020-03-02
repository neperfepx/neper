/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_sample.h"

void
ol_set_sample (struct OL_SET Set1, double factor, struct OL_SET *pSet2)
{
  unsigned int qty, id;
  double factorinv = 1 / factor;

  qty = (unsigned int) ((double) Set1.size * factor) + 10;

  (*pSet2).size = 0;
  (*pSet2).q = ut_alloc_2d (qty, 4);
  (*pSet2).weight = ut_alloc_1d (qty);
  (*pSet2).id = ut_alloc_1d_int (qty);

  id = (unsigned int) (factorinv * .5 - .5);
  while (id < Set1.size)
  {
    ol_q_memcpy (Set1.q[id], (*pSet2).q[(*pSet2).size]);
    (*pSet2).weight[(*pSet2).size] = Set1.weight[id];
    (*pSet2).id[(*pSet2).size] = Set1.id[id];
    (*pSet2).size++;
    id = (unsigned int) (factorinv * ((*pSet2).size + 0.5) - 0.5);
  }

  return;
}

// RECODE ol_set_sample_nb to be the core routine and ol_set_sample to
// use it (not the inverse).
void
ol_set_sample_nb (struct OL_SET Set1, int nb, struct OL_SET *pSet2)
{
  unsigned int qty, id;
  double factor = nb / (double) Set1.size;
  double factorinv = 1. / factor;

  qty = (unsigned int) ((double) Set1.size * factor) + 10;

  (*pSet2).size = 0;
  (*pSet2).q = ut_alloc_2d (qty, 4);
  (*pSet2).weight = ut_alloc_1d (qty);
  (*pSet2).id = ut_alloc_1d_int (qty);

  id = (unsigned int) (factorinv * .5 - .5);
  while (id < Set1.size)
  {
    ol_q_memcpy (Set1.q[id], (*pSet2).q[(*pSet2).size]);
    (*pSet2).weight[(*pSet2).size] = Set1.weight[id];
    (*pSet2).id[(*pSet2).size] = Set1.id[id];
    (*pSet2).size++;
    id = (unsigned int) (factorinv * ((*pSet2).size + 0.5) - 0.5);
  }

  return;
}
