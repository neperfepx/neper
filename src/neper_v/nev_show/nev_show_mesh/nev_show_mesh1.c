/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_mesh_.h"

void
nev_show_mesh (char **argv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;
  ut_string_int (argv[(*pi)], &((*pPrint).showmesh));
  if ((*pPrint).showmesh < 0 || (*pPrint).showmesh > 1)
  {
    ut_print_message (2, 0, "-showmesh: bad value.\n");
    abort ();
  }

  return;
}

void
nev_show_mesh_slice (char **argv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;
  ut_string_int (argv[(*pi)], &((*pPrint).showslice));
  if ((*pPrint).showslice < 0 || (*pPrint).showslice > 1)
  {
    ut_print_message (2, 0, "-showslice: bad value.\n");
    abort ();
  }

  return;
}

void
nev_show_mesh_elt (char **argv, int *pi, struct TESS Tess,
		   struct NODES Nodes,
		   struct MESH *Mesh, struct PART Part, struct PRINT *pPrint)
{
  int i, j, status, varqty, qty;
  char **vars = NULL;
  double *vals = NULL;
  int **pshowarray = NULL;
  char *entity = ut_alloc_1d_char (10);

  if (!strcmp (argv[(*pi)], "-showelt0d"))
    strcpy (entity, "elt0d");
  else if (!strcmp (argv[(*pi)], "-showelt1d"))
    strcpy (entity, "elt1d");
  else if (!strcmp (argv[(*pi)], "-showelt2d"))
    strcpy (entity, "elt2d");
  else if (!strcmp (argv[(*pi)], "-showelt3d"))
    strcpy (entity, "elt3d");
  else if (!strcmp (argv[(*pi)], "-showelt"))
    sprintf (entity, "elt%dd", neut_mesh_array_dim (Mesh));
  else
    ut_error_reportbug ();

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

  (*pi)++;
  (*pshowarray) = ut_realloc_1d_int ((*pshowarray), qty + 1);
  ut_array_1d_int_zero ((*pshowarray), qty + 1);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pshowarray), qty);

  if (status != 0)
  {
    neut_mesh_var_list (entity, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= qty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
          neut_mesh_var_val_one (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                 Part, Tess, (*pPrint).showelt0d,
                                 (*pPrint).showelt1d, (*pPrint).showelt2d,
                                 (*pPrint).showelt3d, 0, entity, i, vars[j],
                                 vals + j, NULL);

      status = ut_math_eval_int (argv[(*pi)], varqty, vars, vals,
				 &((*pshowarray)[i]));
      if (status != 0)
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pshowarray)[0] = ut_array_1d_int_sum ((*pshowarray) + 1, qty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (entity);

  return;
}

void
nev_show_mesh_nodes (char **argv, int *pi, struct TESS Tess,
		     struct NODES Nodes,
		     struct MESH *Mesh, struct PART Part,
		     struct PRINT *pPrint)
{
  int i, j, status, varqty;
  char **vars = NULL;
  double *vals = NULL;

  (*pi)++;
  (*pPrint).shownode =
    ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);

  status = nev_show_genexpr (argv[(*pi)], (*pPrint).shownode, Nodes.NodeQty);

  if (status != 0)
  {
    neut_mesh_var_list ("node", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Nodes.NodeQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_mesh_var_val_one (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                 Part, Tess,
                                 (*pPrint).showelt0d, (*pPrint).showelt1d,
                                 (*pPrint).showelt2d, (*pPrint).showelt3d,
                                 0, "node", i, vars[j], vals + j, NULL);

      status = ut_math_eval_int (argv[(*pi)], varqty, vars, vals,
				 &((*pPrint).shownode[i]));
      if (status != 0)
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).shownode[0] = ut_array_1d_int_sum ((*pPrint).shownode + 1,
					       Nodes.NodeQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_mesh_elset (char **argv, int *pi, struct TESS Tess,
		     struct NODES Nodes, struct MESH *Mesh,
		     struct PART Part, struct PRINT *pPrint)
{
  int i, j, status, varqty;
  char **vars = NULL;
  double *vals = NULL;
  int *showelset3d = NULL;
  int elsetqty, eltqty;
  char *entity = ut_alloc_1d_char (10);
  int **elsets = NULL;
  int **pshowelt = NULL;

  if (!strcmp (argv[(*pi)], "-showelset0d"))
    strcpy (entity, "elset0d");
  else if (!strcmp (argv[(*pi)], "-showelset1d"))
    strcpy (entity, "elset1d");
  else if (!strcmp (argv[(*pi)], "-showelset2d"))
    strcpy (entity, "elset2d");
  else if (!strcmp (argv[(*pi)], "-showelset3d"))
    strcpy (entity, "elset3d");
  else if (!strcmp (argv[(*pi)], "-showelset"))
    sprintf (entity, "elset%dd", neut_mesh_array_dim (Mesh));
  else
    ut_error_reportbug ();

  if (!strcmp (entity, "elset0d"))
  {
    elsets = Mesh[0].Elsets;
    elsetqty = Mesh[0].ElsetQty;
    eltqty = Mesh[0].EltQty;
    pshowelt = &((*pPrint).showelt0d);
  }
  else if (!strcmp (entity, "elset1d"))
  {
    elsets = Mesh[1].Elsets;
    elsetqty = Mesh[1].ElsetQty;
    eltqty = Mesh[1].EltQty;
    pshowelt = &((*pPrint).showelt1d);
  }
  else if (!strcmp (entity, "elset2d"))
  {
    elsets = Mesh[2].Elsets;
    elsetqty = Mesh[2].ElsetQty;
    eltqty = Mesh[2].EltQty;
    pshowelt = &((*pPrint).showelt2d);
  }
  else if (!strcmp (entity, "elset3d"))
  {
    elsets = Mesh[3].Elsets;
    elsetqty = Mesh[3].ElsetQty;
    eltqty = Mesh[3].EltQty;
    pshowelt = &((*pPrint).showelt3d);
  }
  else
    abort ();

  (*pi)++;

  showelset3d = ut_alloc_1d_int (elsetqty + 1);
  (*pshowelt) = ut_realloc_1d_int ((*pshowelt), eltqty + 1);
  ut_array_1d_int_zero ((*pshowelt), eltqty + 1);

  status = nev_show_genexpr (argv[(*pi)], showelset3d, elsetqty);

  if (status != 0)
  {
    neut_mesh_var_list (entity, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= elsetqty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_mesh_var_val_one (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                 Part, Tess,
                                 (*pPrint).showelt0d, (*pPrint).showelt1d,
                                 (*pPrint).showelt2d, (*pPrint).showelt3d,
                                 0, entity, i, vars[j], vals + j, NULL);

      status = ut_math_eval_int (argv[(*pi)], varqty, vars, vals,
				 &(showelset3d[i]));
      if (status != 0)
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  for (i = 1; i <= elsetqty; i++)
    if (showelset3d[i])
      for (j = 1; j <= elsets[i][0]; j++)
	(*pshowelt)[elsets[i][j]] = 1;

  (*pshowelt)[0] = ut_array_1d_int_sum ((*pshowelt) + 1, eltqty);

  ut_free_1d_int (showelset3d);
  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (entity);

  return;
}
