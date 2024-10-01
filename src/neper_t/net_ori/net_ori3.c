/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_.h"

void
net_orispread_file (char *orispread, struct SEEDSET *pSSet)
{
  int i;
  char *tmp = ut_alloc_1d_char (1000);

  FILE *file = ut_file_open (orispread, "r");

  for (i = 1; i <= (*pSSet).N; i++)
  {
    if (fscanf (file, "%s", tmp) != 1)
      abort ();
    ut_string_string (tmp, (*pSSet).SeedOriDistrib + i);
  }

  ut_file_close (file, orispread, "r");

  ut_free_1d_char (&tmp);

  return;
}

void
net_ori_odf_pre (char *odf, struct OL_SET *pOSet, struct ODF *pOdf)
{
  int qty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (odf, &fct, &vars, &vals, &qty);

  ut_print_message (0, 2, "Crystal symmetry: %s\n", (*pOSet).crysym);

  ut_print_message (0, 2, "Reading odf...\n");

  neut_odf_fnscanf (odf, pOdf, "r");

  if (!strstr ((*pOdf).gridtype, (*pOSet).crysym))
    ut_print_message (2, 0, "Crystal symmetry (%s) and orientation space (%s) conflict.\n",
                      (*pOSet).crysym, (*pOdf).gridtype);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, qty);
  ut_free_2d_char (&vals, qty);

  return;
}

void
net_ori_addspread (char *spread, long random, struct OL_SET *pOSet)
{
  int i;
  struct OL_SET OSet2 = ol_set_alloc ((*pOSet).size, (*pOSet).crysym);

  ol_set_misorispread (spread, 3, random, &OSet2);

  for (i = 0; i < (int) (*pOSet).size; i++)
    ol_q_q_q_ref ((*pOSet).q[i], OSet2.q[i], (*pOSet).q[i]);

  ol_set_free (&OSet2);

  return;
}
