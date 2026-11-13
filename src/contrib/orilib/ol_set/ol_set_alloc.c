/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_alloc.h"

void
ol_set_zero (struct OL_SET *pOSet)
{
  (*pOSet).size = 0;
  (*pOSet).q = NULL;
  (*pOSet).weight = NULL;
  (*pOSet).theta = NULL;
  (*pOSet).theta3 = NULL;
  (*pOSet).id = NULL;
  (*pOSet).crysym = NULL;
  (*pOSet).nc = 0;

  return;
}

struct OL_SET
ol_set_alloc (size_t size, char *crysym)
{
  struct OL_SET Set;

  Set.size = size;
  Set.q = ut_alloc_2d (size, 4);
  Set.weight = ut_alloc_1d (size);
  ut_array_1d_set (Set.weight, size, 1);
  Set.theta = NULL;
  Set.theta3 = NULL;
  Set.id = ut_alloc_1d_int (size);
  ut_array_1d_int_set (Set.id, size, 1);
  Set.crysym = NULL;

  if (crysym)
  {
    Set.crysym = ut_alloc_1d_char (strlen (crysym) + 1);
    strcpy (Set.crysym, crysym);
    Set.nc = ol_crysym_qty (crysym);
  }
  else
  {
    Set.crysym = NULL;
    Set.nc = 1;
  }

  return Set;
}

void
ol_set_free (struct OL_SET *pOSet)
{
  ut_free_2d (&(*pOSet).q, (*pOSet).size);
  ut_free_1d (&(*pOSet).weight);
  ut_free_1d (&(*pOSet).theta);
  ut_free_2d (&(*pOSet).theta3, (*pOSet).size);
  ut_free_1d_int (&(*pOSet).id);
  ut_free_1d_char (&(*pOSet).crysym);
  (*pOSet).size = 0;

  return;
}

// REVISE to use net_ori_file
void
ol_set_subset (struct OL_SET Set1, double factor, struct OL_SET *pSet2)
{
  unsigned int qty, id;
  double factorinv;

  if (factor < 1e-6)
    return;

  factorinv = 1 / factor;

  qty = (unsigned int) ((double) Set1.size * factor) + 2;

  (*pSet2).size = 0;
  (*pSet2).q = ut_alloc_2d (qty, 4);
  (*pSet2).weight = ut_alloc_1d (qty);
  (*pSet2).id = ut_alloc_1d_int (qty);
  (*pSet2).theta = Set1.theta ? ut_alloc_1d (qty) : NULL;
  (*pSet2).theta3 = Set1.theta3 ? ut_alloc_1d_pdouble (qty) : NULL;

  id = (int) (factorinv * .5 - .5);
  while (id < Set1.size)
  {
    ol_q_memcpy (Set1.q[id], (*pSet2).q[(*pSet2).size]);
    (*pSet2).weight[(*pSet2).size] = Set1.weight[id];
    (*pSet2).id[(*pSet2).size] = Set1.id[id];
    if (Set1.theta)
      (*pSet2).theta[(*pSet2).size] = Set1.theta[id];
    if (Set1.theta3 && Set1.theta3[id])
    {
      (*pSet2).theta3[(*pSet2).size] = ut_alloc_1d (3);
      ut_array_1d_memcpy (Set1.theta3[id], 3, (*pSet2).theta3[(*pSet2).size]);
    }
    (*pSet2).size++;
    id = (int) (factorinv * ((*pSet2).size + 0.5) - 0.5);
  }

  return;
}

void
ol_set_subset_qty (struct OL_SET Set1, int qty, struct OL_SET *pSet2)
{
  ol_set_subset (Set1, qty / (double) Set1.size, pSet2);

  return;
}

void
ol_set_orthotropic (struct OL_SET Set1, struct OL_SET *pSet2)
{
  unsigned int i, j, id;
  double **qsym = ut_alloc_2d (3, 4);

  (*pSet2) = ol_set_alloc (Set1.size * 4, Set1.crysym);

  id = 0;
  for (i = 0; i < Set1.size; i++)
  {
    ol_q_refsym (Set1.q[i], 1, qsym[0]);
    ol_q_refsym (Set1.q[i], 2, qsym[1]);
    ol_q_refsym (qsym[0], 2, qsym[2]);

    for (j = 0; j < 4; j++)
    {
      ol_q_memcpy (j == 0 ? Set1.q[i] : qsym[j - 1], (*pSet2).q[id]);
      (*pSet2).weight[id] = Set1.weight[i];
      (*pSet2).id[id] = Set1.id[i];
      id++;
    }
  }

  ut_free_2d (&qsym, 3);

  return;
}

void
ol_set_memcpy (struct OL_SET Set1, struct OL_SET *pSet2)
{
  int i;

  ol_set_free (pSet2);
  (*pSet2) = ol_set_alloc (Set1.size, Set1.crysym);
  ut_array_2d_memcpy (Set1.q, Set1.size, 4, (*pSet2).q);

  if (Set1.theta)
  {
    (*pSet2).theta = ut_alloc_1d ((*pSet2).size);
    ut_array_1d_memcpy (Set1.theta, Set1.size, (*pSet2).theta);
  }

  if (Set1.theta3)
  {
    (*pSet2).theta3 = ut_alloc_1d_pdouble ((*pSet2).size);
    for (i = 0; i < (int) (*pSet2).size; i++)
      if (Set1.theta3[i])
      {
        (*pSet2).theta3[i] = ut_alloc_1d (3);
        ut_array_1d_memcpy (Set1.theta3[i], 3, (*pSet2).theta3[i]);
      }
  }

  if (Set1.weight)
  {
    (*pSet2).weight = ut_alloc_1d ((*pSet2).size);
    ut_array_1d_memcpy (Set1.weight, Set1.size, (*pSet2).weight);
  }

  if (Set1.id)
  {
    (*pSet2).id = ut_alloc_1d_int ((*pSet2).size);
    ut_array_1d_int_memcpy (Set1.id, Set1.size, (*pSet2).id);
  }

  return;
}

void
ol_set_cat (struct OL_SET* OSets, int SetQty, struct OL_SET *pOSet)
{
  int i, j, id, size;

  size = 0;
  for (i = 0; i < SetQty; i++)
    size += OSets[i].size;

  (*pOSet) = ol_set_alloc (size, OSets[0].crysym);
  for (i = 0; i < SetQty; i++)
    if (OSets[i].theta)
    {
      (*pOSet).theta = ut_alloc_1d (size);
      break;
    }
  for (i = 0; i < SetQty; i++)
    if (OSets[i].theta3)
    {
      (*pOSet).theta3 = ut_alloc_1d_pdouble (size);
      break;
    }

  id = 0;
  for (i = 0; i < SetQty; i++)
    for (j = 0; j < (int) OSets[i].size; j++)
    {
      ut_array_1d_memcpy (OSets[i].q[j], 4, (*pOSet).q[id]);
      (*pOSet).weight[id] = OSets[i].weight[j];
      if (OSets[i].theta)
        (*pOSet).theta[id] = OSets[i].theta[j];
      if (OSets[i].theta3 && OSets[i].theta3[j])
      {
        (*pOSet).theta3[id] = ut_alloc_1d (3);
        ut_array_1d_memcpy (OSets[i].theta3[j], 3, (*pOSet).theta3[id]);
      }
      (*pOSet).id[id] = OSets[i].id[j];
      id++;
    }

  return;
}

void
ol_set_shuf (struct OL_SET *pOSet, int random)
{
  int i, *id = ut_alloc_1d_int ((*pOSet).size);
  struct OL_SET OSetCpy;

  OSetCpy = ol_set_alloc ((*pOSet).size, (*pOSet).crysym);

  ol_set_memcpy (*pOSet, &OSetCpy);

  ut_array_1d_int_set_id (id, (*pOSet).size);

  gsl_rng *r = NULL;
  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, random - 1);

  gsl_ran_shuffle (r, id, (*pOSet).size, sizeof (int));

  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    ol_q_memcpy (OSetCpy.q[id[i]], (*pOSet).q[i]);
    (*pOSet).weight[i] = OSetCpy.weight[id[i]];
    if (OSetCpy.id)
      (*pOSet).id[i] = OSetCpy.id[id[i]];
    if (OSetCpy.theta)
      (*pOSet).theta[i] = OSetCpy.theta[id[i]];
    if (OSetCpy.theta3)
    {
      ut_free_1d (&(*pOSet).theta3[i]);
      if (OSetCpy.theta3[id[i]])
      {
        (*pOSet).theta3[i] = ut_alloc_1d (3);
        ut_array_1d_memcpy (OSetCpy.theta3[id[i]], 3, (*pOSet).theta3[i]);
      }
    }
  }

  gsl_rng_free (r);
  ut_free_1d_int (&id);
  ol_set_free (&OSetCpy);

  return;
}
