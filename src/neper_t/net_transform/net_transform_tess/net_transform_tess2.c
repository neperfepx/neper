/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_.h"

void
net_transform_tess_ori (char *ori, struct TESS *pTess)
{
  int nblines, valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL, *filename = ut_alloc_1d_char (1000);
  struct OL_SET OSet;

  ut_string_function (ori, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "ori"))
    abort ();

  ut_string_string (ori, &filename);
  filename = ut_realloc_1d_char (filename, strlen (filename) + 2);
  ut_string_fnrs (filename, "ori(", "file(", 1);

  if (!ut_file_exist (vals[0]))
    ut_print_message (2, 4, "File `%s' does not exist.\n", vals[0]);
  nblines = ut_file_nblines (vals[0]);

  OSet = ol_set_alloc (nblines, (*pTess).CellCrySym);

  net_ori_file (filename, &OSet);

  if (nblines != (*pTess).CellQty)
    ut_print_message (2, 4, "Wrong number of entries in file `%s'.\n", vals[0]);

  ut_array_2d_memcpy (OSet.q, (*pTess).CellQty, 4, (*pTess).CellOri + 1);

  ol_set_free (&OSet);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);
  ut_free_1d_char (&filename);

  return;
}

void
net_transform_tess_crysym (char *ori, struct TESS *pTess)
{
  int valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (ori, &fct, &vars, &vals, &valqty);

  if (strcmp (fct, "crysym"))
    abort ();

  ut_string_string (vals[0], &(*pTess).CellCrySym);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  return;
}
