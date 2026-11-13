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

void
net_transform_tess_renamedomface (char *expr, struct TESS *pTess)
{
  int i, valqty, domface = -1;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (expr, &fct, &vars, &vals, &valqty);

  if (valqty != 2)
    ut_print_message (2, 3, "Failed to parse expression `%s'.\n", expr);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if ((*pTess).DomFaceLabel && (*pTess).DomFaceLabel[i]
        && !strcmp ((*pTess).DomFaceLabel[i], vals[0]))
    {
      domface = i;
      break;
    }

  if (domface == -1)
    ut_print_message (2, 3, "Domain face `%s' not found.\n", vals[0]);

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (i != domface && !strcmp ((*pTess).DomFaceLabel[i], vals[1]))
      ut_print_message (2, 3, "Domain face `%s' already exists.\n", vals[1]);

  ut_string_string (vals[1], (*pTess).DomFaceLabel + domface);

  // updating the domain edge and vertex labels, which derive from the face labels
  if ((*pTess).Dim == 3)
    neut_tess_init_domain_label_3d_edgever (pTess);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  return;
}

// net_transform_tess_rmseed removes the seeds matching successive
// expressions and recomputes the tessellation after each of them.
void
net_transform_tess_rmseed (struct IN_T In, struct TESS Dom, char *expr,
                           struct TESS *pTess)
{
  int i, j, exprqty, qty, *rm = NULL, *old_new = NULL;
  int *periodic = ut_alloc_1d_int (3);
  char *fct = NULL, **exprs = NULL;
  struct SEEDSET SSet;
  struct TESS Old;

  if (Dom.CellQty == 0)
    ut_print_message (2, 3, "The domain could not be recovered from the tessellation.\n");
  // (ScaleCellId is the only multiscale information left with -loadtess)
  if (In.levelqty > 1 || (!strcmp (In.load, "none") ? 0 : (*pTess).ScaleCellId != NULL))
    ut_print_message (2, 3, "Not available for multiscale tessellations.\n");
  if ((*pTess).CellLamId)
    ut_print_message (2, 3, "Not available for lamellar tessellations.\n");

  // In.periodic is not set with -loadtess (In is a copy)
  if ((*pTess).Periodic)
    ut_array_1d_int_memcpy ((*pTess).Periodic, 3, periodic);
  In.periodic = periodic;

  ut_string_function_expr (expr, &fct, &exprs, &exprqty);

  for (j = 0; j < exprqty; j++)
  {
    rm = ut_alloc_1d_int ((*pTess).CellQty + 1);
    old_new = ut_alloc_1d_int ((*pTess).CellQty + 1);

    neut_tess_entity_expr_val_int (pTess, "seed", exprs[j], rm);

    qty = 0;
    for (i = 1; i <= (*pTess).CellQty; i++)
      old_new[i] = rm[i] ? 0 : ++qty;

    if (qty == 0)
      ut_print_message (2, 3, "Expression `%s' matches all seeds.\n", exprs[j]);

    ut_print_message (0, 3, "Removed %d seeds...\n", (*pTess).CellQty - qty);

    if (qty < (*pTess).CellQty)
    {
      neut_seedset_set_zero (&SSet);
      neut_tess_set_zero (&Old);

      net_transform_tess_rmseed_seedset (*pTess, old_new, Dom, &SSet);

      neut_tess_tess_gen (*pTess, &Old);
      neut_tess_tess_cell (*pTess, &Old);
      if ((*pTess).DomType)
        ut_string_string ((*pTess).DomType, &Old.DomType);

      net_transform_tess_rmseed_tess (In, Dom, SSet, pTess);

      net_transform_tess_rmseed_celldata (Old, old_new, pTess);

      // (needs the cell orientations)
      if ((*pTess).Dim == 3)
        net_domain_cut (Dom, pTess);

      neut_seedset_free (&SSet);
      neut_tess_free (&Old);
    }

    ut_free_1d_int (&rm);
    ut_free_1d_int (&old_new);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&exprs, exprqty);
  ut_free_1d_int (&periodic);

  return;
}
