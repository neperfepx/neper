/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
    ut_print_neperbug ();

  return;
}

void
neut_tesr_entity_qty (struct TESR Tesr, char *entity, int *pqty)
{
  if (!strcmp (entity, "tesr"))
    (*pqty) = 1;
  else if (!strcmp (entity, "poly") || !strcmp (entity, "cell"))
    (*pqty) = Tesr.CellQty;
  else if (!strcmp (entity, "seed"))
    (*pqty) = Tesr.CellQty;
  else if (!strcmp (entity, "vox"))
    (*pqty) = ut_array_1d_int_prod (Tesr.size, 3);
  else if (!strcmp (entity, "group"))
    (*pqty) = Tesr.CellGroup ? ut_array_1d_int_max (Tesr.CellGroup + 1, Tesr.CellQty) : 0;
  else
    (*pqty) = -1;

  return;
}

int
neut_tesr_expr_val_int (struct TESR Tesr, char *entity, int id, char *expr,
                        int **pvals, int *pvalqty, char **ptype)
{
  int status;
  double *tmp = NULL;

  status = neut_tesr_expr_val (Tesr, entity, id, expr, &tmp, pvalqty, ptype);

  (*pvals) = ut_realloc_1d_int (*pvals, *pvalqty);
  ut_array_1d_round (tmp, *pvalqty, *pvals);

  ut_free_1d (&tmp);

  return status;
}

int
neut_tesr_expr_val (struct TESR Tesr, char *entity, int id, char *expr,
                    double **pvals, int *pvalqty, char **ptype)
{
  int i, status, varqty;
  char **vars = NULL;
  double *vals = NULL;

  ut_math_vars (expr, &vars, &varqty);

  vals = ut_alloc_1d (varqty);

  if (varqty == 1 && !strcmp (expr, vars[0]))
    status = neut_tesr_var_val (Tesr, entity, id, expr, pvals, pvalqty, ptype);

  else
  {
    for (i = 0; i < varqty; i++)
      neut_tesr_var_val_one (Tesr, entity, id, vars[i], vals + i, NULL);

    (*pvalqty) = 1;
    (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
    status = ut_math_eval (expr, varqty, vars, vals, *pvals);

    if (ptype)
    {
      if (ut_math_eval_exprislogical (expr))
        ut_string_string ("%d", ptype);
      else
        ut_string_string ("%f", ptype);
    }
  }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

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
  ut_array_1d_round (tmp, *pvalqty, *pvals);

  return status;
}

int
neut_tesr_var_val (struct TESR Tesr, char *entity, int id, char *var,
                   double **pvals, int *pvalqty, char **ptype)
{
  int i, status, b, tmpint, tmpint3[3];
  double *c = NULL;
  char *typetmp = NULL;

  ut_string_string ("%f", &typetmp);

  (*pvals) = ut_realloc_1d (*pvals, 1);

  if (pvalqty)
    (*pvalqty) = 1;

  if (!strcmp (var, "all"))
  {
    (*pvals)[0] = 1;
    return 0;
  }
  else if (!strcmp (var, "none"))
  {
    (*pvals)[0] = 0;
    return 0;
  }

  // this is for a quick evaluation
  if (!strcmp (entity, "vox")
      && (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z")))
  {
    double *coo = ut_alloc_1d (3);
    neut_tesr_vox_coo (Tesr, id, coo);
    (*pvals)[0] = coo[var[0] - 'x'];
    if (ptype)
      ut_string_string ("%f", ptype);
    ut_free_1d (&coo);

    return 0;
  }

  c = ut_alloc_1d (3);

  // b = (Tess.CellQty > 0) ? Tess.CellBody[id] : 0;
  b = 0;

  if (strcmp (entity, "vox")
      && (!strcmp (var, "x") || !strcmp (var, "y") || !strcmp (var, "z")
          || !strcmp (var, "coo")))
    neut_tesr_cell_centre (Tesr, id, c);

  status = -1;
  if (!strcmp (entity, "tesr"))
  {
    status = 0;
    if (!strcmp (var, "dim"))
    {
      (*pvals)[0] = Tesr.Dim;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "voxnb"))
    {
      (*pvals)[0] = Tesr.size[0] * Tesr.size[1] * Tesr.size[2];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "voxnbx"))
    {
      (*pvals)[0] = Tesr.size[0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "voxnby"))
    {
      (*pvals)[0] = Tesr.size[1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "voxnbz"))
    {
      (*pvals)[0] = Tesr.size[2];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "voxsizex"))
      (*pvals)[0] = Tesr.vsize[0];
    else if (!strcmp (var, "voxsizey"))
      (*pvals)[0] = Tesr.vsize[1];
    else if (!strcmp (var, "voxsizez"))
      (*pvals)[0] = Tesr.vsize[2];
    else if (!strcmp (var, "rastersizex"))
      (*pvals)[0] = Tesr.vsize[0] * Tesr.size[0];
    else if (!strcmp (var, "rastersizey"))
      (*pvals)[0] = Tesr.vsize[1] * Tesr.size[1];
    else if (!strcmp (var, "rastersizez"))
      (*pvals)[0] = Tesr.vsize[2] * Tesr.size[2];
    else if (!strcmp (var, "rastersize"))
      neut_tesr_rastersize (Tesr, *pvals);
    else if (!strcmp (var, "area"))
      neut_tesr_area (Tesr, *pvals);
    else if (!strcmp (var, "vol"))
      neut_tesr_volume (Tesr, *pvals);
    else if (!strcmp (var, "size"))
      neut_tesr_size (Tesr, *pvals);
    else if (!strcmp (var, "x"))
      neut_tesr_centre_x (Tesr, *pvals);
    else if (!strcmp (var, "y"))
      neut_tesr_centre_y (Tesr, *pvals);
    else if (!strcmp (var, "z"))
      neut_tesr_centre_z (Tesr, *pvals);
    else if (!strcmp (var, "coo"))
    {
      neut_tesr_centre (Tesr, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var, "originx"))
      (*pvals)[0] = Tesr.Origin[0];
    else if (!strcmp (var, "originy"))
      (*pvals)[0] = Tesr.Origin[1];
    else if (!strcmp (var, "originz"))
      (*pvals)[0] = Tesr.Origin[2];
    else
      status = -1;
  }

  else if (!strcmp (entity, "cell") || !strcmp (entity, "poly")
        || !strcmp (entity, "face") || !strcmp (entity, "edge"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      if (strcmp (entity, "cell") != 0)
        (*pvals)[0] = id;
      else
        (*pvals)[0] = neut_tesr_cell_id (Tesr, id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "group"))
    {
      (*pvals)[0] = Tesr.CellGroup ? Tesr.CellGroup[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "body"))
    {
      (*pvals)[0] = b;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "x"))
      (*pvals)[0] = c[0];
    else if (!strcmp (var, "y"))
      (*pvals)[0] = c[1];
    else if (!strcmp (var, "z"))
      (*pvals)[0] = c[2];
    else if (!strcmp (var, "coo"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy (c, 3, *pvals);
    }
    else if (!strcmp (var, "vol"))
      neut_tesr_cell_volume (Tesr, id, *pvals);
    else if (!strcmp (var, "diameq"))
      neut_tesr_cell_diameq (Tesr, id, *pvals);
    else if (!strcmp (var, "radeq"))
      neut_tesr_cell_radeq (Tesr, id, *pvals);
    else if (!strcmp (var, "size"))
      neut_tesr_cell_size (Tesr, id, *pvals);
    else if (!strcmp (var, "convexity"))
      neut_tesr_cell_convexity (Tesr, id, *pvals);
    else if (!strcmp (var, "oridisanisoangles"))
    {
      double **evect = ut_alloc_2d (3, 3);
      neut_tesr_cell_orianiso (Tesr, id, evect, *pvals);
      (*pvalqty) = 3;
      ut_free_2d (&evect, 3);
    }
    else if (!strcmp (var, "oridisanisofact"))
    {
      double **evect = ut_alloc_2d (3, 3);
      double *eval = ut_alloc_1d (3);
      neut_tesr_cell_orianiso (Tesr, id, evect, eval);
      (*pvals)[0] = eval[0] / pow (eval[0] * eval[1] * eval[2], 1. / 3);
      ut_free_2d (&evect, 3);
      ut_free_1d (&eval);
    }
    else if (!strcmp (var, "oridisanisoaxes"))
    {
      double **evect = ut_alloc_2d (3, 3);
      double *eval = ut_alloc_1d (3);
      neut_tesr_cell_orianiso (Tesr, id, evect, eval);
      (*pvals) = ut_realloc_1d (*pvals, 9);
      ut_array_1d_memcpy (evect[0], 3, *pvals);
      ut_array_1d_memcpy (evect[1], 3, *pvals + 3);
      ut_array_1d_memcpy (evect[2], 3, *pvals + 6);
      (*pvalqty) = 9;
      ut_free_2d (&evect, 3);
      ut_free_1d (&eval);
    }
    else if (!strcmp (var, "oridisanisodeltas"))
    {
      double **evect = ut_alloc_2d (3, 3);
      neut_tesr_cell_orianiso_delta (Tesr, id, evect, *pvals);
      (*pvalqty) = 3;
      ut_free_2d (&evect, 3);
    }
    else if (!strcmp (var, "vxmin"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][0][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vymin"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][1][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vzmin"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][2][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vxmax"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][0][1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vymax"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][1][1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vzmax"))
    {
      (*pvals)[0] = Tesr.CellBBox[id][2][1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvxmin"))
    {
      (*pvals)[0] = 1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvymin"))
    {
      (*pvals)[0] = 1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvzmin"))
    {
      (*pvals)[0] = 1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvxmax"))
    {
      (*pvals)[0] = Tesr.size[0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvymax"))
    {
      (*pvals)[0] = Tesr.size[1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "domvzmax"))
    {
      (*pvals)[0] = Tesr.size[2];
      ut_string_string ("%d", &typetmp);
    }
    else if (neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);

      double *q = ol_q_alloc ();
      neut_tesr_cell_ori (Tesr, id, q);
      neut_ori_des_ori (q, var, *pvals);
      ol_q_free (q);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "seed"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "x"))
      (*pvals)[0] = Tesr.SeedCoo[id][0];
    else if (!strcmp (var, "y"))
      (*pvals)[0] = Tesr.SeedCoo[id][1];
    else if (!strcmp (var, "z"))
      (*pvals)[0] = Tesr.SeedCoo[id][2];
    else if (!strcmp (var, "w"))
      (*pvals)[0] = Tesr.SeedWeight[id];
    else if (!strcmp (var, "vol"))
      neut_tesr_cell_volume (Tesr, id, *pvals);
    else
      status = -1;
  }

  else if (!strcmp (entity, "vox"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "cell"))
    {
      neut_tesr_vox_cellid (Tesr, id, &tmpint);
      (*pvals)[0] = tmpint;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vx"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vy"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[1];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vz"))
    {
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      (*pvals)[0] = tmpint3[2];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vcoo"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_tesr_vox_pos (Tesr, id, tmpint3);
      for (i = 0; i < 3; i++)
        (*pvals)[i] = tmpint3[i];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "x"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[0];
    }
    else if (!strcmp (var, "y"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[1];
    }
    else if (!strcmp (var, "z"))
    {
      neut_tesr_vox_coo (Tesr, id, c);
      (*pvals)[0] = c[2];
    }
    else if (!strcmp (var, "coo"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_tesr_vox_coo (Tesr, id, *pvals);
    }
    else if (!strcmp (var, "oridef"))
    {
      neut_tesr_vox_oridef (Tesr, id, &tmpint);
      (*pvals)[0] = tmpint;
      ut_string_string ("%d", &typetmp);
    }
    else if (neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);

      double *q = ol_q_alloc ();
      neut_tesr_vox_ori (Tesr, id, q);
      neut_ori_des_ori (q, var, *pvals);
      ol_q_free (q);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "group"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vol"))
      neut_tesr_group_volume (Tesr, id, *pvals);
    else if (!strcmp (var, "volfrac"))
      neut_tesr_group_volfrac (Tesr, id, *pvals);
    else if (!strcmp (var, "area"))
      neut_tesr_group_area (Tesr, id, *pvals);
    else if (!strcmp (var, "areafrac"))
      neut_tesr_group_areafrac (Tesr, id, *pvals);
    else if (!strcmp (var, "size"))
      neut_tesr_group_size (Tesr, id, *pvals);
    else if (!strcmp (var, "sizefrac"))
      neut_tesr_group_sizefrac (Tesr, id, *pvals);
    else
      status = -1;
  }

  else
    ut_print_message (2, 0, "Failed to process expression `%s'.\n", entity);

  if (status == -1 && Tesr.pSim)
    status = neut_sim_entity_id_res_val (*(Tesr.pSim), entity, id, var, *pvals);

  if (ptype)
    ut_string_string (typetmp, ptype);

  ut_free_1d (&c);
  ut_free_1d_char (&typetmp);

  return status;
}

int
neut_tesr_var_val_one (struct TESR Tesr, char *entity, int id, char *var,
                       double *pval, char **ptype)
{
  int status, qty;
  double *tmp = NULL;

  status = neut_tesr_var_val (Tesr, entity, id, var, &tmp, &qty, ptype);

  *pval = tmp[0];

  ut_free_1d (&tmp);

  if (status)
    return status;
  else
    return qty == 1 ? 0 : -1;
}

int
neut_tesr_var_val_int_one (struct TESR Tesr, char *entity, int id, char *var,
                           int *pval, char **ptype)
{
  int qty, *tmp = NULL;

  neut_tesr_var_val_int (Tesr, entity, id, var, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d_int (&tmp);

  return 0;
}

int
neut_tesr_expr_val_int_one (struct TESR Tesr, char *entity, int id,
                            char *expr, int *pval, char **ptype)
{
  int qty, *tmp = NULL;

  neut_tesr_expr_val_int (Tesr, entity, id, expr, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d_int (&tmp);

  return 0;
}

void
neut_tesr_entity_expr_val (struct TESR Tesr, char *entity, char *expr,
                           double *val, char **ptype)
{
  int j, k, status, entityqty, varqty;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;
  char *type = NULL;

  if (ptype)
    ut_string_string ("%d", ptype);

  neut_tesr_entity_qty (Tesr, entity, &entityqty);
  ut_math_vars (expr, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  if (ut_string_isfilename (expr))
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
      {
        neut_tesr_var_val_one (Tesr, entity, j, vars[k], vals + k, &type);
        if (ptype && !strcmp (type, "%f"))
          ut_string_string ("%f", ptype);
      }

      status = ut_math_eval (expr, varqty, vars, vals, val + j);

      if (status == -1)
        abort ();
    }
  }

  ut_free_1d_char (&type);
  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

int
neut_tesr_expr_val_one (struct TESR Tesr, char *entity, int id, char *expr,
                        double *pval, char **ptype)
{
  int qty;
  double *tmp = NULL;

  neut_tesr_expr_val (Tesr, entity, id, expr, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d (&tmp);

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
  return (pos[0] >= 1 && pos[0] <= Tesr.size[0] && pos[1] >= 1
          && pos[1] <= Tesr.size[1] && pos[2] >= 1 && pos[2] <= Tesr.size[2]);
}

void
neut_tesr_sizestring (struct TESR Tesr, char **psizestring)
{
  (*psizestring) = ut_realloc_1d_char (*psizestring, 1000);

  if (Tesr.Dim == 2)
    sprintf ((*psizestring), "%d%s%d", Tesr.size[0], NEUT_SEP_DEP,
             Tesr.size[1]);
  else if (Tesr.Dim == 3)
    sprintf ((*psizestring), "%d%s%d%s%d", Tesr.size[0], NEUT_SEP_DEP,
             Tesr.size[1], NEUT_SEP_DEP, Tesr.size[2]);
  else
    abort ();

  (*psizestring) =
    ut_realloc_1d_char ((*psizestring), strlen (*psizestring) + 1);

  return;
}

void
neut_tesr_cell_olset (struct TESR Tesr, int cell, struct OL_SET *pOSet)
{
  int i, j, k, qty;

  (*pOSet) =
    ol_set_alloc ((Tesr.CellBBox[cell][2][1] - Tesr.CellBBox[cell][2][0] +
                   1) * (Tesr.CellBBox[cell][1][1] -
                         Tesr.CellBBox[cell][1][0] +
                         1) * (Tesr.CellBBox[cell][0][1] -
                               Tesr.CellBBox[cell][0][0] + 1),
                  Tesr.CellCrySym ? Tesr.CellCrySym : "triclinic");

  qty = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
        if (Tesr.VoxCell[i][j][k] == cell)
        {
          qty++;
          ol_q_memcpy (Tesr.VoxOri[i][j][k], (*pOSet).q[qty -1]);
          if (Tesr.VoxOriDef)
           (*pOSet).id[qty -1] = Tesr.VoxOriDef[i][j][k];
        }

  (*pOSet).size = (size_t) qty;

  return;
}

int
neut_tesr_isvoid (struct TESR Tesr)
{
  return (Tesr.CellQty == 0 && (!Tesr.size || Tesr.size[0] == 0));
}

void
neut_tesr_entity_expr_val_int (struct TESR Tesr, char *entity, char *expr,
                               int *val)
{
  int qty;
  double *tmp = NULL;

  neut_tesr_entity_qty (Tesr, entity, &qty);
  tmp = ut_alloc_1d (qty + 1);

  neut_tesr_entity_expr_val (Tesr, entity, expr, tmp, NULL);
  ut_array_1d_round (tmp + 1, qty, val + 1);

  ut_free_1d (&tmp);

  return;
}

int
neut_tesr_hascellorispread (struct TESR Tesr)
{
  int i;

  if (Tesr.CellOriDistrib)
    for (i = 1; i <= Tesr.CellQty; i++)
      if (strcmp (Tesr.CellOriDistrib[i], "none"))
        return 1;

  return 0;
}

int
neut_tesr_cellid_cell (struct TESR Tesr, int cellid, int *pcell)
{
  if (pcell)
    *pcell = -1;

  if (!Tesr.CellId)
  {
    if (pcell)
      (*pcell) = cellid;
    return 0;
  }

  else
  {
    int i;
    for (i = 1; i <= Tesr.CellQty; i++)
      if (Tesr.CellId[i] == cellid)
      {
        if (pcell)
          *pcell = i;
        return 0;
      }
  }

  return -1;
}

int
neut_tesr_cell_id (struct TESR Tesr, int cell)
{
  return Tesr.CellId ? Tesr.CellId[cell] : cell;
}

int
neut_tesr_cell_ori (struct TESR Tesr, int cell, double *q)
{
  int status = -1;

  if (Tesr.CellOri)
  {
    ol_q_memcpy (Tesr.CellOri[cell], q);
    status = 0;
  }

  else if (Tesr.pSim)
  {
    double *R = ol_R_alloc ();
    struct SIMRES SimRes;

    neut_simres_set_zero (&SimRes);

    neut_sim_simres (*(Tesr.pSim), "cell", "ori", &SimRes);

    if (SimRes.file)
    {
      double **qall = ut_alloc_2d (Tesr.CellQty + 1, 4);
      neut_ori_fnscanf (SimRes.file, (*(Tesr.pSim)).OriDes, "ascii", qall + 1, NULL, (*(Tesr.pSim)).CellQty, NULL, "R");

      ol_q_memcpy (qall[cell], q);

      ut_free_2d (&qall, Tesr.CellQty + 1);
    }

    neut_simres_free (&SimRes);
    ol_R_free (R);

    status = 0;
  }

  return status;
}

int
neut_tesr_cell_gos (struct TESR Tesr, int cell, double *pgos)
{
  int i, j, k, qty, status = -1;
  double theta, *q = ol_q_alloc ();

  neut_tesr_cell_ori (Tesr, cell, q);

  *pgos = 0;

  qty = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
        if (Tesr.VoxCell[i][j][k] == cell)
          if (!Tesr.VoxOriDef || Tesr.VoxOriDef[i][j][k])
          {
            ol_q_q_disori (q, Tesr.VoxOri[i][j][k], Tesr.CellCrySym, &theta);
            qty++;
            *pgos += theta;
          }

  *pgos = (qty > 0) ? *pgos / qty : -1;

  ol_q_free (q);

  return status;
}
