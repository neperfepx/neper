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
  (*pOSet).id = NULL;
  (*pOSet).label = NULL;
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
  Set.id = ut_alloc_1d_int (size);
  ut_array_1d_int_set (Set.id, size, 1);
  Set.label = ut_alloc_1d_pchar (size);
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
  ut_free_1d_int (&(*pOSet).id);
  ut_free_2d_char (&(*pOSet).label, (*pOSet).size);
  ut_free_1d_char (&(*pOSet).crysym);
  (*pOSet).size = 0;

  return;
}

// REVISE to use net_ori_file
int
ol_set_fscanf (FILE * file, struct OL_SET *pSet, char *format)
{
  size_t i;
  double *e = ol_e_alloc ();
  int size, test, firstline_nbw, whole_nbw;
  char *format2 = ut_alloc_1d_char (100);

  // checking the number of string on the first line
  // can be 1, 3 or 4.
  ut_file_nextlinenbwords (file, &firstline_nbw);
  whole_nbw = ut_file_nbwords_pointer (file);

  /* required because size_t cannot be scanf (???) neither
     with %u nor with %zu with -pedantic (ISO C90) */
  /* adding 'z' is suggested on
     http://manpages.ubuntu.com/manpages/jaunty/man3/scanf.3.html */

  if (firstline_nbw == 2)
  {
    test = fscanf (file, "%d%s", &size, format2);

    (*pSet).size = (unsigned int) size;
    if (test != 2)
      return 0;
  }
  else if (firstline_nbw == 3)
  {
    (*pSet).size = whole_nbw / 3;
    strcpy (format2, "rodrigues");
  }
  else if (firstline_nbw == 4)
  {
    (*pSet).size = whole_nbw / 4;
    strcpy (format2, "e,w");
  }
  else if (firstline_nbw == 5)
  {
    (*pSet).size = whole_nbw / 5;
    strcpy (format2, "id,e,w");
  }
  else
    return 0;

  if (format)
    strcpy (format, format2);

  (*pSet) = ol_set_alloc ((*pSet).size, "cubic");

  for (i = 0; i < (*pSet).size; i++)
  {
    if (!strcmp (format2, "e"))
    {
      ol_e_fscanf (file, e);
      ol_e_q (e, (*pSet).q[i]);
    }
    else if (!strcmp (format2, "e,w"))
    {
      ol_e_fscanf (file, e);
      ol_e_q (e, (*pSet).q[i]);
      if (fscanf (file, "%lf", &((*pSet).weight[i])) != 1)
        return -1;
    }
    else if (!strcmp (format2, "id,e,w"))
    {
      if (fscanf (file, "%d", &((*pSet).id[i])) != 1)
        return -1;
      ol_e_fscanf (file, e);
      ol_e_q (e, (*pSet).q[i]);
      if (fscanf (file, "%lf", &((*pSet).weight[i])) != 1)
        return -1;
    }
    else if (!strcmp (format2, "id,e"))
    {
      if (fscanf (file, "%d", &((*pSet).id[i])) != 1)
        return -1;
      ol_e_fscanf (file, e);
      ol_e_q (e, (*pSet).q[i]);
    }
    else
      abort ();
  }

  ol_e_free (e);
  ut_free_1d_char (&format2);

  return 1;
}

int
ol_set_fscanf_sample (FILE * file, double factor, struct OL_SET *pSet,
                      char *format)
{
  /* needed for -O3 opti */
  struct OL_SET Setb = ol_set_alloc (1, NULL);

  ol_set_fscanf (file, &Setb, format);
  ol_set_sample (Setb, factor, pSet);
  ol_set_free (&Setb);

  return 1;
}

int
ol_set_fscanf_sample_nb (FILE * file, int nb, struct OL_SET *pSet,
                         char *format)
{
  /* needed for -O3 opti */
  struct OL_SET Setb = ol_set_alloc (1, NULL);

  ol_set_fscanf (file, &Setb, format);
  ol_set_sample_nb (Setb, nb, pSet);
  ol_set_free (&Setb);

  return 1;
}

int
ol_set_fprintf (FILE * file, struct OL_SET Set, char *format)
{
  unsigned int i;
  int test;
  double *e = ol_e_alloc ();
  char *fmt = ut_alloc_1d_char (4 * strlen (format) + 10);
  int id, weight;

  test = -1;

  id = ut_array_1d_int_eltpos (Set.id, Set.size, 0) == -1 ? 0 : 1;

  weight = 0;
  for (i = 0; i < Set.size; i++)
    if (fabs (Set.weight[i] - 1) > 1e-6)
    {
      weight = 1;
      break;
    }

  if (id == 0 && weight == 0)
  {
    sprintf (fmt, "%s %s %s\n", format, format, format);
    for (i = 0; i < Set.size; i++)
    {
      ol_q_e (Set.q[i], e);
      test = fprintf (file, fmt, e[0], e[1], e[2]);
    }
  }
  else
  {
    sprintf (fmt, "%%d %s %s %s %s\n", format, format, format, format);
    for (i = 0; i < Set.size; i++)
    {
      ol_q_e (Set.q[i], e);
      test = fprintf (file, fmt, Set.id[i], e[0], e[1], e[2], Set.weight[i]);
    }
  }

  ol_e_free (e);
  ut_free_1d_char (&fmt);

  return test;
}

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

  id = (int) (factorinv * .5 - .5);
  while (id < Set1.size)
  {
    ol_q_memcpy (Set1.q[id], (*pSet2).q[(*pSet2).size]);
    (*pSet2).weight[(*pSet2).size] = Set1.weight[id];
    (*pSet2).id[(*pSet2).size] = Set1.id[id];
    (*pSet2).size++;
    id = (int) (factorinv * ((*pSet2).size + 0.5) - 0.5);
  }

  return;
}

void
ol_set_orthotropic (struct OL_SET Set1, struct OL_SET *pSet2)
{
  unsigned int i, j, id;
  double **qsym = ut_alloc_2d (3, 4);

  (*pSet2) = ol_set_alloc (Set1.size * 4, (*pSet2).crysym);

  id = 0;
  for (i = 0; i < Set1.size; i++)
  {
    ol_q_refsym (Set1.q[i], 1, qsym[0]);
    ol_q_refsym (Set1.q[i], 2, qsym[1]);
    ol_q_refsym (qsym[0], 2, qsym[2]);

    ol_q_memcpy (Set1.q[i], (*pSet2).q[id++]);
    for (j = 0; j < 3; j++)
    {
      ol_q_memcpy (qsym[j], (*pSet2).q[id]);
      (*pSet2).weight[id] = Set1.weight[id];
      (*pSet2).id[id] = Set1.id[id];
      id++;
    }
  }

  ut_free_2d (&qsym, 3);

  return;
}

void
ol_set_memcpy (struct OL_SET Set1, struct OL_SET *pSet2)
{
  (*pSet2).size = Set1.size;
  ut_array_2d_memcpy (Set1.q, Set1.size, 4, (*pSet2).q);
  ut_array_1d_memcpy (Set1.weight, Set1.size, (*pSet2).weight);
  ut_array_1d_int_memcpy (Set1.id, Set1.size, (*pSet2).id);

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

  id = 0;
  for (i = 0; i < SetQty; i++)
    for (j = 0; j < (int) OSets[i].size; j++)
    {
      ut_array_1d_memcpy (OSets[i].q[j], 4, (*pOSet).q[id]);
      (*pOSet).weight[id] = OSets[i].weight[j];
      if (OSets[i].theta)
        (*pOSet).theta[id] = OSets[i].theta[j];
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
  }

  gsl_rng_free (r);
  ut_free_1d_int (&id);
  ol_set_free (&OSetCpy);

  return;
}
