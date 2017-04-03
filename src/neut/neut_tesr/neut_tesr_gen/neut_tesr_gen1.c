/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesr_gen_.h"

void
neut_tesr_cell (struct TESR Tesr, char **pcell)
{
  if (Tesr.Dim == 0)
    ut_string_string ("ver", pcell);
  else if (Tesr.Dim == 1)
    ut_string_string ("edge", pcell);
  else if (Tesr.Dim == 2)
    ut_string_string ("face", pcell);
  else if (Tesr.Dim == 3)
    ut_string_string ("poly", pcell);
  else
    ut_error_reportbug ();

  return;
}

void
neut_tesr_entity_qty (struct TESR Tesr, char *entity, int *pqty)
{
  if (!strcmp (entity, "poly")
   || !strcmp (entity, "cell"))
    (*pqty) = Tesr.CellQty;
  else if (!strcmp (entity, "seed"))
    (*pqty) = Tesr.CellQty;
  else
    (*pqty) = -1;

  return;
}

void
neut_tesr_tesrqty (struct TESR Tesr, int *pqty)
{
  (*pqty) = ut_array_1d_int_prod (Tesr.size, Tesr.Dim);

  return;
}

int
neut_tesr_grid_area (struct TESR Tesr, double *parea)
{
  if (Tesr.Dim == 2)
  {
    (*parea) = Tesr.vsize[0] * Tesr.size[0] * Tesr.vsize[1] * Tesr.size[1];
    return 0;
  }
  else
  {
    (*parea) = 0;
    return -1;
  }
}

int
neut_tesr_area (struct TESR Tesr, double *parea)
{
  int i, j, qty;

  if (Tesr.Dim == 2)
  {
    qty = 0;
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	if (Tesr.VoxCell[i][j] != 0)
	  qty++;

    (*parea) = ut_array_1d_prod (Tesr.vsize, 2) * qty;
    return 0;
  }
  else
  {
    (*parea) = 0;
    return -1;
  }
}

int
neut_tesr_voxsize (struct TESR Tesr, double *pvol)
{
  if (Tesr.Dim == 2)
    return neut_tesr_voxarea (Tesr, pvol);
  else if (Tesr.Dim == 3)
    return neut_tesr_voxvolume (Tesr, pvol);
  else
    abort ();

  return 0;
}

int
neut_tesr_voxarea (struct TESR Tesr, double *pvol)
{
  (*pvol) = ut_array_1d_prod (Tesr.vsize, 2);

  return 0;
}

int
neut_tesr_voxvolume (struct TESR Tesr, double *pvol)
{
  (*pvol) = ut_array_1d_prod (Tesr.vsize, 3);

  return 0;
}

int
neut_tesr_volume (struct TESR Tesr, double *pvol)
{
  if (Tesr.Dim == 3)
  {
    (*pvol) = Tesr.vsize[0] * Tesr.size[0]
      * Tesr.vsize[1] * Tesr.size[1] * Tesr.vsize[2] * Tesr.size[2];
    return 0;
  }
  else
  {
    (*pvol) = 0;
    return -1;
  }
}

int
neut_tesr_grid_volume (struct TESR Tesr, double *pvol)
{
  int i, j, k, qty;

  if (Tesr.Dim == 3)
  {
    qty = 0;
    for (k = 1; k <= Tesr.size[1]; k++)
      for (j = 1; j <= Tesr.size[1]; j++)
	for (i = 1; i <= Tesr.size[0]; i++)
	  if (Tesr.VoxCell[i][j] != 0)
	    qty++;

    (*pvol) = ut_array_1d_prod (Tesr.vsize, 3) * qty;
    return 0;
  }
  else
  {
    (*pvol) = 0;
    return -1;
  }
}

int
neut_tesr_size (struct TESR Tesr, double *psize)
{
  if (Tesr.Dim == 2)
    neut_tesr_area (Tesr, psize);
  else if (Tesr.Dim == 3)
    neut_tesr_volume (Tesr, psize);
  else
    abort ();

  return 0;
}

void
neut_tesr_var_list (char *entity, char ***pvar, int *pvarqty)
{
  if (!strcmp (entity, "cell"))
  {
    (*pvarqty) = 9;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 10);
    strcpy ((*pvar)[0], "id");
    strcpy ((*pvar)[1], "x");
    strcpy ((*pvar)[2], "y");
    strcpy ((*pvar)[3], "z");
    strcpy ((*pvar)[4], "vol");
    strcpy ((*pvar)[5], "diameq");
    strcpy ((*pvar)[6], "radeq");
    strcpy ((*pvar)[7], "size");
    strcpy ((*pvar)[8], "convexity");
  }
  else
    ut_error_reportbug ();

  return;
}

int
neut_tesr_expr_val_int (struct TESR Tesr, char *entity,
			int id, char *expr, int *pval, char **ptype)
{
  int status;
  double tmp;

  status = neut_tesr_expr_val (Tesr, entity, id, expr, &tmp, ptype);
  (*pval) = ut_num_d2ri (tmp);

  return status;
}

int
neut_tesr_expr_val (struct TESR Tesr, char *entity,
		    int id, char *expr, double *pval, char **ptype)
{
  int i, status, varqty;
  char **vars = NULL;
  double *vals = NULL;

  neut_tesr_var_list (entity, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  for (i = 0; i < varqty; i++)
    if (strstr (expr, vars[i]))
      neut_tesr_var_val (Tesr, entity, id, vars[i], vals + i, NULL);

  status = ut_math_eval (expr, varqty, vars, vals, pval);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  if (ptype)
    ut_string_string ("%f", ptype);

  return status;
}

int
neut_tesr_var_val_int (struct TESR Tesr, char *entity,
		       int id, char *var, int *pval, char **ptype)
{
  int status;
  double tmp;

  status = neut_tesr_var_val (Tesr, entity, id, var, &tmp, ptype);
  (*pval) = ut_num_d2ri (tmp);

  return status;
}

int
neut_tesr_var_val (struct TESR Tesr, char *entity,
		   int id, char *var, double *pval, char **ptype)
{
  int status, b;
  double vol;
  double *c = ut_alloc_1d (3);
  char *typetmp = ut_alloc_1d_char (10);

  // b = (Tess.CellQty > 0) ? Tess.CellBody[id] : 0;
  b = 0;

  if (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z"))
    neut_tesr_cell_centre (Tesr, id, c);

  status = -1;
  if (!strcmp (entity, "cell")
      || !strcmp (entity, "poly")
      || !strcmp (entity, "face") || !strcmp (entity, "edge"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      if (strcmp (entity, "cell") != 0)
	(*pval) = id;
      else
	(*pval) = Tesr.CellId ? Tesr.CellId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pval) = b;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      (*pval) = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      (*pval) = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      (*pval) = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "diameq"))
    {
      neut_tesr_cell_diameq (Tesr, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "radeq"))
    {
      neut_tesr_cell_radeq (Tesr, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "size"))
    {
      neut_tesr_cell_size (Tesr, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "convexity"))
    {
      neut_tesr_cell_convexity (Tesr, id, pval);
      strcpy (typetmp, "%f");
    }
  }
  else if (!strcmp (entity, "seed"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pval) = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      (*pval) = Tesr.SeedCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      (*pval) = Tesr.SeedCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      (*pval) = Tesr.SeedCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "w"))
    {
      (*pval) = Tesr.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, &vol);
      (*pval) = vol;
      strcpy (typetmp, "%f");
    }
  }
  else
    ut_print_message (2, 0, "Failed to process expression `%s'.\n", entity);

  if (ptype)
  {
    if (strlen (typetmp) == 0)
      abort ();
    ut_free_1d_char (*ptype);
    (*ptype) = ut_alloc_1d_char (strlen (typetmp) + 1);
    strcpy (*ptype, typetmp);
  }

  ut_free_1d (c);
  ut_free_1d_char (typetmp);

  return status;
}

int
neut_tesr_var_val_all (struct TESR Tesr,
		       char *entity, char *var, double *val, char **ptype)
{
  int i, qty, dim;
  double size, factor, mean;
  char **var2 = NULL;
  int var2qty;
  char *entity2 = NULL;

  if (strcmp (entity, "cell") != 0)
    ut_string_string (entity, &entity2);
  else
    ut_string_string ("poly", &entity2);

  ut_string_separate (var, NEUT_SEP_DEP, &var2, &var2qty);

  neut_tesr_entity_qty (Tesr, entity2, &qty);

  for (i = 1; i <= qty; i++)
    neut_tesr_var_val (Tesr, entity2, i, var2[0], val + i, ptype);

  if (var2qty == 2)
  {
    if (!strcmp (var2[1], "rel"))
    {
      mean = ut_array_1d_mean (val + 1, qty);
      factor = 1. / mean;
    }
    else if (!strcmp (var2[1], "uc"))
    {
      neut_tesr_size (Tesr, &size);
      size /= Tesr.CellQty;

      size = pow (size, 1. / Tesr.Dim);
      neut_tesr_var_dim (Tesr.Dim, entity2, var2[0], &dim);
      factor = pow (1. / size, dim);
    }
    else
      abort ();

    ut_array_1d_scale (val + 1, qty, factor);
  }

  ut_free_2d_char (var2, var2qty);
  ut_free_1d_char (entity2);

  return 0;
}

int
neut_tesr_expr_val_all (struct TESR Tesr,
		        char *entity, char *var, double *val, char **ptype)
{
  int i, qty, dim;
  double size, factor, mean;
  char **var2 = NULL;
  int var2qty;
  char *entity2 = NULL;

  if (strcmp (entity, "cell") != 0)
    ut_string_string (entity, &entity2);
  else
    ut_string_string ("poly", &entity2);

  ut_string_separate (var, NEUT_SEP_DEP, &var2, &var2qty);

  neut_tesr_entity_qty (Tesr, entity2, &qty);

  for (i = 1; i <= qty; i++)
    neut_tesr_expr_val (Tesr, entity2, i, var2[0], val + i, ptype);

  if (var2qty == 2)
  {
    if (!strcmp (var2[1], "rel"))
    {
      mean = ut_array_1d_mean (val + 1, qty);
      factor = 1. / mean;
    }
    else if (!strcmp (var2[1], "uc"))
    {
      neut_tesr_size (Tesr, &size);
      size /= Tesr.CellQty;

      size = pow (size, 1. / Tesr.Dim);
      neut_tesr_var_dim (Tesr.Dim, entity2, var2[0], &dim);
      factor = pow (1. / size, dim);
    }
    else
      abort ();

    ut_array_1d_scale (val + 1, qty, factor);
  }

  ut_free_2d_char (var2, var2qty);
  ut_free_1d_char (entity2);

  return 0;
}

void
neut_tesr_entity_expr_val (struct TESR Tesr, char *entity,
			   char *expr, double *val)
{
  int i, j, k, status;
  int qty, *qty2 = NULL;
  char ***list = NULL;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;
  int varqty;
  int entityqty;
  double tmp, tmp2;

  entityqty = Tesr.CellQty;
  ut_string_separate2 (expr, NEUT_SEP_NODEP, NEUT_SEP_DEP, &list, &qty2,
		       &qty);

  // patching list
  for (i = 0; i < qty; i++)
    if (qty2[i] == 2)
      continue;
    else if (qty2[i] == 1)
    {
      list[i] = ut_realloc_1d_pchar (list[i], 2);
      list[i][1] = NULL;
      ut_string_string (list[i][0], &(list[i][1]));
      strcpy (list[i][0], "1");
      qty2[i] = 2;
    }
    else
      abort ();

  neut_tesr_var_list (entity, &vars, &varqty);

  vals = ut_alloc_1d (varqty);

  for (i = 0; i < qty; i++)
  {
    if (ut_string_filename (list[i][1]))
    {
      file = ut_file_open (list[i][1], "R");
      ut_array_1d_fscanf (file, val + 1, entityqty);
      ut_file_close (file, list[i][1], "R");
    }
    else
    {
      ut_string_real (list[i][1], &tmp);

      if (!strcmp (list[i][0], "1"))
	ut_array_1d_set (val + 1, entityqty, tmp);
      else
	for (j = 1; j <= entityqty; j++)
	{
	  for (k = 0; k < varqty; k++)
	    neut_tesr_var_val (Tesr, entity, j, vars[k], &(vals[k]), NULL);

	  status = ut_math_eval (list[i][0], varqty, vars, vals, &tmp2);

	  if (status == -1)
	    abort ();

	  if (ut_num_equal (tmp2, 1, 1e-6))
	    val[j] = tmp;
	}
    }
  }

  return;
}

void
neut_tesr_var_dim (int dim, char *entity, char *var, int *pdim)
{
  neut_tess_var_dim (dim, entity, var, pdim);

  return;
}
