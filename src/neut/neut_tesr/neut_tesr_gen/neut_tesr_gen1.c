/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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
			int id, char *expr, int **pvals, int *pvalqty, char **ptype)
{
  int status;
  double *tmp = NULL;

  status = neut_tesr_expr_val (Tesr, entity, id, expr, &tmp, pvalqty, ptype);

  (*pvals) = ut_realloc_1d_int (*pvals, *pvalqty);
  ut_array_1d_d2ri (tmp, *pvalqty, *pvals);

  ut_free_1d (tmp);

  return status;
}

int
neut_tesr_expr_val (struct TESR Tesr, char *entity, int id, char *expr,
                    double **pvals, int *pvalqty, char **ptype)
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
      status = neut_tesr_var_val (Tesr, entity, id, expr, pvals, pvalqty, ptype);
      break;
    }

  if (!isvar)
  {
    for (i = 0; i < varqty; i++)
      if (strstr (expr, vars[i]))
        neut_tesr_var_val_one (Tesr, entity, id, vars[i], vals + i, NULL);

    (*pvalqty) = 1;
    (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
    status = ut_math_eval (expr, varqty, vars, vals, *pvals);

    if (ptype)
      ut_string_string ("%f", ptype);
  }

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return status;
}

int
neut_tesr_var_val_int (struct TESR Tesr, char *entity, int id, char *var,
                       int **pvals, int *pvalqty, char **ptype)
{
  int status;
  double *tmp = NULL;

  status = neut_tesr_var_val (Tesr, entity, id, var, &tmp, pvalqty, ptype);
  (*pvals) = ut_realloc_1d_int (*pvals, *pvalqty);
  ut_array_1d_d2ri (tmp, *pvalqty, *pvals);

  return status;
}

int
neut_tesr_var_val (struct TESR Tesr, char *entity,
                   int id, char *var, double **pvals, int *pvalqty,
                   char **ptype)
{
  int i, status, b, tmpint, tmpint3[3];
  double vol;
  double *c = NULL;
  char *typetmp = NULL;

  (*pvals) = ut_realloc_1d (*pvals, 1);

  if (pvalqty)
    (*pvalqty) = 1;

  // this is for a quick evaluation
  if (!strcmp (entity, "vox") && (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z")))
  {
    double *coo = ut_alloc_1d (3);
    neut_tesr_vox_coo (Tesr, id, coo);
    (*pvals)[0] = coo[var[0] - 'x'];
    ut_free_1d (coo);

    return 1;
  }

  c = ut_alloc_1d (3);
  typetmp = ut_alloc_1d_char (10);

  // b = (Tess.CellQty > 0) ? Tess.CellBody[id] : 0;
  b = 0;

  if (strcmp (entity, "vox") && (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z")
      || !strcmp (var, "xyz")))
    neut_tesr_cell_centre (Tesr, id, c);

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
	(*pvals)[0] = id;
      else
	(*pvals)[0] = Tesr.CellId ? Tesr.CellId[id] : id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = b;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      (*pvals)[0] = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      (*pvals)[0] = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      (*pvals)[0] = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "xyz"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy (*pvals, 3, c);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "e"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ol_q_e (Tesr.CellOri[id], *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "diameq"))
    {
      neut_tesr_cell_diameq (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "radeq"))
    {
      neut_tesr_cell_radeq (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "size"))
    {
      neut_tesr_cell_size (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "convexity"))
    {
      neut_tesr_cell_convexity (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizex"))
    {
      (*pvals)[0] = Tesr.vsize[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizey"))
    {
      (*pvals)[0] = Tesr.vsize[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "voxsizez"))
    {
      (*pvals)[0] = Tesr.vsize[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizex"))
    {
      (*pvals)[0] = Tesr.vsize[0] * Tesr.size[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizey"))
    {
      (*pvals)[0] = Tesr.vsize[1] * Tesr.size[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "domsizez"))
    {
      (*pvals)[0] = Tesr.vsize[2] * Tesr.size[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "e"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ol_q_e (Tesr.CellOri[id], *pvals);
      strcpy (typetmp, "%f");
    }
  }
  else if (!strcmp (entity, "seed"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      (*pvals)[0] = Tesr.SeedCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      (*pvals)[0] = Tesr.SeedCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      (*pvals)[0] = Tesr.SeedCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "w"))
    {
      (*pvals)[0] = Tesr.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "vol"))
    {
      neut_tesr_cell_volume (Tesr, id, &vol);
      (*pvals)[0] = vol;
      strcpy (typetmp, "%f");
    }
  }

  else if (!strcmp (entity, "vox"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "cell"))
    {
      neut_tesr_vox_cell (Tesr, id, &tmpint);
      (*pvals)[0] = tmpint;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vx"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vy"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[1];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vz"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[2];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "vxyz"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      for (i = 0; i < 3; i++)
        (*pvals)[i] = tmpint3[i];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var, "x"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "y"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "z"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "xyz"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_tesr_vox_coo (Tesr, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var, "e"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);

      if (Tesr.VoxOri)
      {
        neut_tesr_vox_pos (Tesr, id, tmpint3);
        ol_q_e (Tesr.VoxOri[tmpint3[0]][tmpint3[1]][tmpint3[2]], *pvals);
      }
      else
      {
        neut_tesr_vox_cell (Tesr, id, &tmpint);
        ol_q_e (Tesr.CellOri[tmpint], *pvals);
      }
      strcpy (typetmp, "%f");
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
neut_tesr_var_val_one (struct TESR Tesr, char *entity,
		       int id, char *var, double *pval,
                       char **ptype)
{
  int qty;
  double *tmp = NULL;

  neut_tesr_var_val (Tesr, entity, id, var, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d (tmp);

  return 0;
}

int
neut_tesr_var_val_int_one (struct TESR Tesr, char *entity,
                           int id, char *var, int *pval,
                           char **ptype)
{
  int qty, *tmp = NULL;

  neut_tesr_var_val_int (Tesr, entity, id, var, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d_int (tmp);

  return 0;
}

int
neut_tesr_expr_val_int_one (struct TESR Tesr, char *entity,
                           int id, char *expr, int *pval,
                           char **ptype)
{
  int qty, *tmp = NULL;

  neut_tesr_expr_val_int (Tesr, entity, id, expr, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d_int (tmp);

  return 0;
}

void
neut_tesr_entity_expr_val (struct TESR Tesr, char *entity,
			   char *expr, double *val)
{
  int j, k, status, entityqty, varqty;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;

  neut_tesr_entity_qty (Tesr, entity, &entityqty);
  neut_tesr_var_list (entity, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  if (ut_string_filename (expr))
  {
    file = ut_file_open (expr, "R");
    ut_array_1d_fscanf (file, val + 1, entityqty);
    ut_file_close (file, expr, "R");
  }
  else
  {
    for (j = 1; j <= entityqty; j++)
    {
      for (k = 0; k < varqty; k++)
        if (strstr (expr, vars[k]))
          neut_tesr_var_val_one (Tesr, entity, j, vars[k], vals + k, NULL);

      status = ut_math_eval (expr, varqty, vars, vals, val + j);

      if (status == -1)
        abort ();
    }
  }

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

int
neut_tesr_expr_val_one (struct TESR Tesr, char *entity,
		       int id, char *expr, double *pval,
                       char **ptype)
{
  int qty;
  double *tmp = NULL;

  neut_tesr_expr_val (Tesr, entity, id, expr, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d (tmp);

  return 0;
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

void
neut_tesr_cell_olset (struct TESR Tesr, int cell, struct OL_SET *pOSet)
{
  int i, j, k, qty;

  (*pOSet) = ol_set_alloc ((Tesr.CellBBox[cell][2][1] - Tesr.CellBBox[cell][2][0] + 1)
                         * (Tesr.CellBBox[cell][1][1] - Tesr.CellBBox[cell][1][0] + 1)
                         * (Tesr.CellBBox[cell][0][1] - Tesr.CellBBox[cell][0][0] + 1),
                         Tesr.CellCrySym? Tesr.CellCrySym : "triclinic");

  qty = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
        if (Tesr.VoxCell[i][j][k] == cell)
          ol_q_memcpy (Tesr.VoxOri[i][j][k], (*pOSet).q[qty++]);

  (*pOSet).size = (size_t) qty;

  return;
}
