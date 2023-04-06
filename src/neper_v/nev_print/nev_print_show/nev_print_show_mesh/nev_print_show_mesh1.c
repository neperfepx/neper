/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_show_mesh_.h"

void
nev_print_show_mesh_elt (char *entity_in, char *expr, struct TESS *pTess,
                         struct NODES Nodes, struct MESH *Mesh,
                         struct PRINT *pPrint)
{
  int i, j, status, varqty = 0, qty;
  char **vars = NULL;
  double *vals = NULL;
  int **pshowarray = NULL;
  char *entity = ut_alloc_1d_char (10);

  if (!strcmp (entity_in, "elt0d"))
    strcpy (entity, "elt0d");
  else if (!strcmp (entity_in, "elt1d"))
    strcpy (entity, "elt1d");
  else if (!strcmp (entity_in, "elt2d"))
    strcpy (entity, "elt2d");
  else if (!strcmp (entity_in, "elt3d"))
    strcpy (entity, "elt3d");
  else if (!strcmp (entity_in, "elt"))
    sprintf (entity, "elt%dd", neut_mesh_array_dim (Mesh));
  else
  {
    printf ("entity_in = %s\n", entity_in);
    ut_print_neperbug ();
  }

  if (!strcmp (entity, "elt0d"))
  {
    qty = Mesh[0].EltQty;
    pshowarray = &((*pPrint).showelt0d);
  }
  else if (!strcmp (entity, "elt1d"))
  {
    qty = Mesh[1].EltQty;
    pshowarray = &((*pPrint).showelt1d);
  }
  else if (!strcmp (entity, "elt2d"))
  {
    qty = Mesh[2].EltQty;
    pshowarray = &((*pPrint).showelt2d);
  }
  else if (!strcmp (entity, "elt3d"))
  {
    qty = Mesh[3].EltQty;
    pshowarray = &((*pPrint).showelt3d);
  }
  else
    abort ();

  (*pshowarray) = ut_realloc_1d_int ((*pshowarray), qty + 1);
  ut_array_1d_int_zero ((*pshowarray), qty + 1);

  // checking for a general expression
  status = nev_print_show_genexpr (expr, *pshowarray, qty);

  if (status != 0)
  {
    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= qty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_mesh_var_val_one (Nodes, Mesh, pTess,
                                        (*pPrint).showelt0d,
                                        (*pPrint).showelt1d,
                                        (*pPrint).showelt2d,
                                        (*pPrint).showelt3d, 0, entity, i,
                                        vars[j], vals + j, NULL);
        if (status)
          ut_print_exprbug (vars[j]);
      }

      status =
        ut_math_eval_int (expr, varqty, vars, vals,
                          &((*pshowarray)[i]));
      if (status)
        ut_print_exprbug (expr);
    }
  }

  (*pshowarray)[0] = ut_array_1d_int_sum ((*pshowarray) + 1, qty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&entity);

  return;
}

void
nev_print_show_mesh_nodes (char *expr, struct TESS *pTess,
                     struct NODES Nodes, struct MESH *Mesh,
                     struct PRINT *pPrint)
{
  int i, j, status, varqty = 0;
  char **vars = NULL;
  double *vals = NULL;

  (*pPrint).shownode =
    ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);

  status = nev_print_show_genexpr (expr, (*pPrint).shownode, Nodes.NodeQty);

  if (status != 0)
  {
    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_mesh_var_val_one (Nodes, Mesh, pTess,
                                        (*pPrint).showelt0d,
                                        (*pPrint).showelt1d,
                                        (*pPrint).showelt2d,
                                        (*pPrint).showelt3d, 0, "node", i,
                                        vars[j], vals + j, NULL);
        if (status)
          ut_print_exprbug (vars[j]);
      }

      status =
        ut_math_eval_int (expr, varqty, vars, vals,
                          &((*pPrint).shownode[i]));
      if (status)
        ut_print_exprbug (expr);
    }
  }

  (*pPrint).shownode[0] =
    ut_array_1d_int_sum ((*pPrint).shownode + 1, Nodes.NodeQty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
nev_print_show_mesh_elset (char *entity_in, char *expr, struct TESS *pTess,
                     struct NODES Nodes, struct MESH *Mesh,
                     struct PRINT *pPrint)
{
  int i, j, status, varqty;
  char **vars = NULL;
  double *vals = NULL;
  int elsetqty, EltQty;
  char *entity = ut_alloc_1d_char (10);
  int **elsets = NULL;
  int **pshowelt = NULL, **pshowelset = NULL;

  if (!strcmp (entity_in, "elset0d"))
    strcpy (entity, "elset0d");
  else if (!strcmp (entity_in, "elset1d"))
    strcpy (entity, "elset1d");
  else if (!strcmp (entity_in, "elset2d"))
    strcpy (entity, "elset2d");
  else if (!strcmp (entity_in, "elset3d"))
    strcpy (entity, "elset3d");
  else if (!strcmp (entity_in, "elset"))
    sprintf (entity, "elset%dd", neut_mesh_array_dim (Mesh));
  else
    ut_print_neperbug ();

  if (!strcmp (entity, "elset0d"))
  {
    elsets = Mesh[0].Elsets;
    elsetqty = Mesh[0].ElsetQty;
    EltQty = Mesh[0].EltQty;
    pshowelset = &((*pPrint).showelset0d);
    pshowelt = &((*pPrint).showelt0d);
  }
  else if (!strcmp (entity, "elset1d"))
  {
    elsets = Mesh[1].Elsets;
    elsetqty = Mesh[1].ElsetQty;
    EltQty = Mesh[1].EltQty;
    pshowelset = &((*pPrint).showelset1d);
    pshowelt = &((*pPrint).showelt1d);
  }
  else if (!strcmp (entity, "elset2d"))
  {
    elsets = Mesh[2].Elsets;
    elsetqty = Mesh[2].ElsetQty;
    EltQty = Mesh[2].EltQty;
    pshowelset = &((*pPrint).showelset2d);
    pshowelt = &((*pPrint).showelt2d);
  }
  else if (!strcmp (entity, "elset3d"))
  {
    elsets = Mesh[3].Elsets;
    elsetqty = Mesh[3].ElsetQty;
    EltQty = Mesh[3].EltQty;
    pshowelset = &((*pPrint).showelset3d);
    pshowelt = &((*pPrint).showelt3d);
  }
  else
    abort ();

  (*pshowelset) = ut_alloc_1d_int (elsetqty + 1);
  (*pshowelt) = ut_realloc_1d_int ((*pshowelt), EltQty + 1);
  ut_array_1d_int_zero ((*pshowelt), EltQty + 1);

  status = nev_print_show_genexpr (expr, (*pshowelset), elsetqty);

  if (status != 0)
  {
    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= elsetqty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_mesh_var_val_one (Nodes, Mesh, pTess,
                                        (*pPrint).showelt0d,
                                        (*pPrint).showelt1d,
                                        (*pPrint).showelt2d,
                                        (*pPrint).showelt3d, 0, entity, i,
                                        vars[j], vals + j, NULL);
        if (status)
          ut_print_exprbug (expr);
      }

      status =
        ut_math_eval_int (expr, varqty, vars, vals, &((*pshowelset)[i]));
      if (status)
        ut_print_exprbug (expr);
    }
  }

  for (i = 1; i <= elsetqty; i++)
    if ((*pshowelset)[i])
      for (j = 1; j <= elsets[i][0]; j++)
        (*pshowelt)[elsets[i][j]] = 1;

  (*pshowelt)[0] = ut_array_1d_int_sum ((*pshowelt) + 1, EltQty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&entity);

  return;
}
