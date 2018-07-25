/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
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
  else if (!strcmp (entity, "vox"))
    (*pqty) = ut_array_1d_int_prod (Tesr.size, 3);
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
neut_tesr_voxlengtheq (struct TESR Tesr, double *plengtheq)
{
  (*plengtheq) = ut_array_1d_gmean (Tesr.vsize, Tesr.Dim);

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
  if (!strcmp (entity, "general"))
  {
    (*pvarqty) = 6;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 10);
    strcpy ((*pvar)[0], "voxsizex");
    strcpy ((*pvar)[1], "voxsizey");
    strcpy ((*pvar)[2], "voxsizez");
    strcpy ((*pvar)[3], "domsizex");
    strcpy ((*pvar)[4], "domsizey");
    strcpy ((*pvar)[5], "domsizez");
  }
  else if (!strcmp (entity, "cell"))
  {
    (*pvarqty) = 11;
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
    strcpy ((*pvar)[9], "xyz");
    strcpy ((*pvar)[10], "e");
  }
  else if (!strcmp (entity, "vox"))
  {
    (*pvarqty) = 11;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 10);
    strcpy ((*pvar)[0], "id");
    strcpy ((*pvar)[1], "cell");
    strcpy ((*pvar)[2], "x");
    strcpy ((*pvar)[3], "y");
    strcpy ((*pvar)[4], "z");
    strcpy ((*pvar)[5], "vx");
    strcpy ((*pvar)[6], "vy");
    strcpy ((*pvar)[7], "vz");
    strcpy ((*pvar)[8], "e");
    strcpy ((*pvar)[9], "xyz");
    strcpy ((*pvar)[10], "vxyz");
  }
  else
    ut_error_reportbug ();

  return;
}

int
neut_tesr_expr_val_int (struct TESR Tesr, char *entity,
			int id, char *expr, int *val, int *pvalqty, char **ptype)
{
  int status;
  double tmp[10];

  status = neut_tesr_expr_val (Tesr, entity, id, expr, tmp, pvalqty, ptype);
  ut_array_1d_d2ri (tmp, *pvalqty, val);

  ut_free_1d (tmp);

  return status;
}

int
neut_tesr_expr_val (struct TESR Tesr, char *entity,
		    int id, char *expr, double *val, int *pvalqty, char **ptype)
{
  int i, status, varqty, isvar;
  char **vars = NULL;
  double *vals = NULL;

  neut_tesr_var_list (entity, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  isvar = 0;
  for (i = 0; i < varqty; i++)
    if (!strcmp (expr, vars[i]))
    {
      isvar = 1;
      status = neut_tesr_var_val (Tesr, entity, id, expr, val, pvalqty, ptype);
      break;
    }

  if (!isvar)
  {
    for (i = 0; i < varqty; i++)
      if (strstr (expr, vars[i]))
        neut_tesr_var_val (Tesr, entity, id, vars[i], vals + i, pvalqty, NULL);

    status = ut_math_eval (expr, varqty, vars, vals, val);
    (*pvalqty) = 1;

    if (ptype)
      ut_string_string ("%f", ptype);
  }

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return status;
}

int
neut_tesr_var_val_int (struct TESR Tesr, char *entity,
		       int id, char *var, int *val, int *pvalqty, char **ptype)
{
  int status;
  double tmp[10];

  status = neut_tesr_var_val (Tesr, entity, id, var, tmp, pvalqty, ptype);
  ut_array_1d_d2ri (tmp, *pvalqty, val);

  return status;
}

int
neut_tesr_var_val (struct TESR Tesr, char *entity,
		   int id, char *var, double *val, int *pvalqty, char **ptype)
{
  int i, status, b, tmpint, tmpint3[3];
  double vol;
  double *c = ut_alloc_1d (3);
  char *typetmp = ut_alloc_1d_char (10);

  // b = (Tess.CellQty > 0) ? Tess.CellBody[id] : 0;
  b = 0;

  if (strcmp (entity, "vox") && (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z")
      || !strcmp (var, "xyz")))
    neut_tesr_cell_centre (Tesr, id, c);

  if (pvalqty)
    (*pvalqty) = 1;

  status = -1;
  if (!strcmp (entity, "general")
      || !strcmp (entity, "cell")
      || !strcmp (entity, "poly")
      || !strcmp (entity, "face")
      || !strcmp (entity, "edge"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      if (strcmp (entity, "cell") != 0)
	val[0] = id;
      else
	val[0] = Tesr.CellId ? Tesr.CellId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      val[0] = b;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      val[0] = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      val[0] = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      val[0] = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "xyz"))
    {
      ut_array_1d_memcpy (val, 3, c);
      strcpy (typetmp, "%f");
      (*pvalqty) = 3;
    }
    else if (!strcmp (var, "e"))
    {
      ol_q_e (Tesr.CellOri[id], val);
      strcpy (typetmp, "%f");
      (*pvalqty) = 3;
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, val);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "diameq"))
    {
      neut_tesr_cell_diameq (Tesr, id, val);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "radeq"))
    {
      neut_tesr_cell_radeq (Tesr, id, val);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "size"))
    {
      neut_tesr_cell_size (Tesr, id, val);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "convexity"))
    {
      neut_tesr_cell_convexity (Tesr, id, val);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizex"))
    {
      val[0] = Tesr.vsize[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizey"))
    {
      val[0] = Tesr.vsize[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizez"))
    {
      val[0] = Tesr.vsize[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizex"))
    {
      val[0] = Tesr.vsize[0] * Tesr.size[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizey"))
    {
      val[0] = Tesr.vsize[1] * Tesr.size[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizez"))
    {
      val[0] = Tesr.vsize[2] * Tesr.size[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "e"))
    {
      ol_q_e (Tesr.CellOri[id], val);
      strcpy (typetmp, "%f");
      if (pvalqty)
        (*pvalqty) = 3;
    }
  }
  else if (!strcmp (entity, "seed"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      val[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      val[0] = Tesr.SeedCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      val[0] = Tesr.SeedCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      val[0] = Tesr.SeedCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "w"))
    {
      val[0] = Tesr.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, &vol);
      val[0] = vol;
      strcpy (typetmp, "%f");
    }
  }

  else if (!strcmp (entity, "vox"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      val[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "cell"))
    {
      neut_tesr_vox_cell (Tesr, id, &tmpint);
      val[0] = tmpint;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vx"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      val[0] = tmpint3[0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vy"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      val[0] = tmpint3[1];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vz"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      val[0] = tmpint3[2];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vxyz"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      for (i = 0; i < 3; i++)
        val[i] = tmpint3[i];
      (*pvalqty) = 3;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      val[0] = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      val[0] = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      val[0] = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "xyz"))
    {
      neut_tesr_vox_coo (Tesr, id, val);
      (*pvalqty) = 3;
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "e"))
    {
      int cell;
      neut_tesr_vox_cell (Tesr, id, &cell);
      ol_q_e (Tesr.CellOri[cell], val);
      strcpy (typetmp, "%f");
      if (pvalqty)
        (*pvalqty) = 3;
    }
    else
      status = -1;
  }

  else
    ut_print_message (2, 0, "Failed to process expression `%s'.\n", entity);

  if (ptype)
  {
    if (strlen (typetmp) == 0)
      abort ();
    ut_string_string (typetmp, ptype);
  }

  ut_free_1d (c);
  ut_free_1d_char (typetmp);

  return status;
}

int
neut_tesr_var_val_all (struct TESR Tesr, char *entity, char *var, double **val,
                       int *valqty, char **ptype)
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

  neut_tesr_entity_qty (Tesr, entity, &qty);

  for (i = 1; i <= qty; i++)
    neut_tesr_var_val (Tesr, entity, i, var2[0], val[i], valqty + i, ptype);

  if (var2qty == 2)
  {
    if (!strcmp (var2[1], "rel"))
    {
      mean = ut_array_2d_col_mean (val + 1, 0, qty);
      factor = 1. / mean;
    }
    else if (!strcmp (var2[1], "uc"))
    {
      neut_tesr_size (Tesr, &size);
      size /= Tesr.CellQty;

      size = pow (size, 1. / Tesr.Dim);
      neut_tesr_var_dim (Tesr.Dim, entity, var2[0], &dim);
      factor = pow (1. / size, dim);
    }
    else
      abort ();

    ut_array_2d_scale (val + 1, qty, 1, factor);
  }

  ut_free_2d_char (var2, var2qty);
  ut_free_1d_char (entity2);

  return 0;
}

int
neut_tesr_expr_val_all (struct TESR Tesr, char *entity, char *var,
                        double **val, int *valqty, char **ptype)
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

  neut_tesr_entity_qty (Tesr, entity, &qty);

  for (i = 1; i <= qty; i++)
    neut_tesr_expr_val (Tesr, entity, i, var2[0], val[i], valqty + i, ptype);

  if (var2qty == 2)
  {
    if (!strcmp (var2[1], "rel"))
    {
      mean = ut_array_2d_col_mean (val + 1, 0, qty);
      factor = 1. / mean;
    }
    else if (!strcmp (var2[1], "uc"))
    {
      neut_tesr_size (Tesr, &size);
      size /= Tesr.CellQty;

      size = pow (size, 1. / Tesr.Dim);
      neut_tesr_var_dim (Tesr.Dim, entity, var2[0], &dim);
      factor = pow (1. / size, dim);
    }
    else
      abort ();

    ut_array_2d_scale (val + 1, qty, 1, factor);
  }

  ut_free_2d_char (var2, var2qty);
  ut_free_1d_char (entity);

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
	    neut_tesr_var_val (Tesr, entity, j, vars[k], vals + k, NULL, NULL);

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

int
neut_tesr_pos_valid (struct TESR Tesr, int *pos)
{
  return (pos[0] >= 1 && pos[0] <= Tesr.size[0] &&
          pos[1] >= 1 && pos[1] <= Tesr.size[1] &&
          pos[2] >= 1 && pos[2] <= Tesr.size[2]);
}

void
neut_tesr_sizestring (struct TESR Tesr, char **psizestring)
{
  (*psizestring) = ut_realloc_1d_char (*psizestring, 1000);

  if (Tesr.Dim == 2)
    sprintf ((*psizestring), "%d%s%d", Tesr.size[0], NEUT_SEP_DEP, Tesr.size[1]);
  else if (Tesr.Dim == 3)
    sprintf ((*psizestring), "%d%s%d%s%d", Tesr.size[0], NEUT_SEP_DEP,
                                           Tesr.size[1], NEUT_SEP_DEP, Tesr.size[2]);
  else
    abort ();

  (*psizestring) = ut_realloc_1d_char ((*psizestring), strlen (*psizestring) + 1);

  return;
}
