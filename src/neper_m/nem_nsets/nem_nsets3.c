/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets_.h"

void
nem_nsets_custom_expr (struct TESS Tess, struct NODES Nodes,
                       struct MESH *Mesh, char *expr, int **pnodes,
                       int *pnodeqty)
{
  int i, qty;
  char *fct = NULL, *arg = NULL, **args = NULL;
  double *val = NULL, *coo = NULL, vmin, vmax, tol, dist, distmin;

  // whole-expression function: min(<expr>), max(<expr>) or
  // closest(<x>,<y>,<z>)
  if (expr[strlen (expr) - 1] == ')' && strchr (expr, '('))
  {
    fct = ut_alloc_1d_char (strlen (expr) + 1);
    arg = ut_alloc_1d_char (strlen (expr) + 1);
    strncpy (fct, expr, strchr (expr, '(') - expr);
    strcpy (arg, strchr (expr, '(') + 1);
    arg[strlen (arg) - 1] = '\0';

    // the opening parenthesis must match the last one
    if (ut_string_thischarqty (arg, '(') != ut_string_thischarqty (arg, ')'))
      fct[0] = '\0';
    else
    {
      int level = 0;
      for (i = 0; arg[i] != '\0'; i++)
      {
        level += (arg[i] == '(') - (arg[i] == ')');
        if (level < 0)
        {
          fct[0] = '\0';
          break;
        }
      }
    }
  }

  (*pnodeqty) = 0;
  ut_free_1d_int (pnodes);

  if (fct && (!strcmp (fct, "min") || !strcmp (fct, "max")))
  {
    val = ut_alloc_1d (Nodes.NodeQty + 1);

    neut_mesh_entity_expr_val (Nodes, Mesh, &Tess, NULL, NULL, NULL, NULL,
                               "node", arg, val, NULL);

    vmin = ut_array_1d_min (val + 1, Nodes.NodeQty);
    vmax = ut_array_1d_max (val + 1, Nodes.NodeQty);
    tol = 1e-6 * (vmax - vmin);

    (*pnodes) = ut_alloc_1d_int (Nodes.NodeQty);
    for (i = 1; i <= Nodes.NodeQty; i++)
      if (fabs (val[i] - (!strcmp (fct, "min") ? vmin : vmax)) <= tol)
        (*pnodes)[(*pnodeqty)++] = i;
    (*pnodes) = ut_realloc_1d_int (*pnodes, *pnodeqty);
  }

  else if (fct && !strcmp (fct, "closest"))
  {
    ut_list_break (arg, NEUT_SEP_NODEP, &args, &qty);
    if (qty != 3)
      ut_print_message (2, 3, "Failed to process expression `%s' (3 coordinates needed).\n", expr);

    coo = ut_alloc_1d (3);
    for (i = 0; i < 3; i++)
      if (ut_math_eval (args[i], 0, NULL, NULL, coo + i) == -1)
        ut_print_exprbug (args[i]);

    distmin = DBL_MAX;
    (*pnodes) = ut_alloc_1d_int (1);
    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      dist = ut_space_dist (Nodes.NodeCoo[i], coo);
      if (dist < distmin)
      {
        distmin = dist;
        (*pnodes)[0] = i;
      }
    }
    (*pnodeqty) = (Nodes.NodeQty > 0);

    ut_free_2d_char (&args, qty);
  }

  else
    neut_mesh_entity_expr_matches (&Tess, Nodes, Mesh, "node", expr, pnodes,
                                   pnodeqty);

  ut_free_1d_char (&fct);
  ut_free_1d_char (&arg);
  ut_free_1d (&val);
  ut_free_1d (&coo);

  return;
}
