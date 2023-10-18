/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tesr_op_.h"

void
neut_tesr_set_zero (struct TESR *pTesr)
{
  (*pTesr).Origin = NULL;
  (*pTesr).hasvoid = -1;

  (*pTesr).size = NULL;
  (*pTesr).vsize = NULL;
  (*pTesr).VoxCell = NULL;
  (*pTesr).VoxOri = NULL;
  (*pTesr).VoxOriDef = NULL;
  (*pTesr).CellQty = 0;
  (*pTesr).Dim = 0;
  (*pTesr).CellId = NULL;
  (*pTesr).CellOri = NULL;
  (*pTesr).CellOriDistrib = NULL;
  (*pTesr).CellOriDes = NULL;
  ut_string_string (NEUT_DEFAULT_ORIDESFULL, &(*pTesr).CellOriDes);
  (*pTesr).CellBBox = NULL;
  (*pTesr).CellCrySym = NULL;
  (*pTesr).CellGroup = NULL;
  (*pTesr).SeedCoo = NULL;
  (*pTesr).SeedWeight = NULL;

  (*pTesr).pSim = NULL;

  return;
}

void
neut_tesr_alloc (struct TESR *pTesr, int dim, int *size, double *vsize, char *wildcard)
{
  (*pTesr).Dim = dim;

  (*pTesr).Origin = ut_alloc_1d (3);

  if ((*pTesr).size == NULL || size != (*pTesr).size)
  {
    (*pTesr).size = ut_realloc_1d_int ((*pTesr).size, 3);
    ut_array_1d_int_set ((*pTesr).size, 3, 1);
    ut_array_1d_int_memcpy (size, dim, (*pTesr).size);
  }

  if (vsize != (*pTesr).vsize)
  {
    (*pTesr).vsize = ut_realloc_1d ((*pTesr).vsize, 3);
    ut_array_1d_set ((*pTesr).vsize, 3, 1);
    ut_array_1d_memcpy (vsize, dim, (*pTesr).vsize);
  }

  // FIXME for memory usage
  (*pTesr).VoxCell = ut_alloc_3d_int (size[0] + 2, size[1] + 2, size[2] + 2);

  if (wildcard && strstr (wildcard, "CellBBox"))
  {
    (*pTesr).CellBBox = ut_alloc_1d_ppint (1);
    (*pTesr).CellBBox[0] = NULL;
  }

  if (wildcard && strstr (wildcard, "VoxOri"))
    (*pTesr).VoxOri = ut_alloc_4d (size[0] + 1, size[1] + 1, size[2] + 1, 4);

  if (wildcard && strstr (wildcard, "VoxOriDef"))
  {
    (*pTesr).VoxOriDef = ut_alloc_3d_int (size[0] + 1, size[1] + 1, size[2] + 1);
    ut_array_3d_int_set ((*pTesr).VoxOriDef, size[0] + 1, size[1] + 1, size[2] + 1, 1);
  }

  return;
}

void
neut_tesr_free (struct TESR *pTesr)
{
  ut_free_1d (&(*pTesr).Origin);

  if ((*pTesr).size)
    ut_free_3d_int (&(*pTesr).VoxCell, (*pTesr).size[0] + 2,
                    (*pTesr).size[1] + 2);
  ut_free_1d (&(*pTesr).vsize);
  ut_free_1d_int (&(*pTesr).CellId);
  ut_free_2d (&(*pTesr).CellOri, (*pTesr).CellQty + 1);
  ut_free_2d_char (&(*pTesr).CellOriDistrib, (*pTesr).CellQty + 1);
  ut_free_1d_char (&(*pTesr).CellOriDes);
  ut_free_1d_char (&(*pTesr).CellCrySym);
  ut_free_1d_int (&(*pTesr).CellGroup);
  ut_free_2d (&(*pTesr).SeedCoo, (*pTesr).CellQty + 1);
  ut_free_1d (&(*pTesr).SeedWeight);
  ut_free_3d_int (&(*pTesr).CellBBox, (*pTesr).CellQty + 1, 3);
  if ((*pTesr).size)
  {
    ut_free_4d (&(*pTesr).VoxOri, (*pTesr).size[0] + 1, (*pTesr).size[1] + 1,
                (*pTesr).size[2] + 1);
    ut_free_3d_int (&(*pTesr).VoxOriDef, (*pTesr).size[0] + 1, (*pTesr).size[1] + 1);
  }
  ut_free_1d_int (&(*pTesr).size);

  return;
}

void
neut_tesr_reset (struct TESR *pTesr)
{
  neut_tesr_free (pTesr);

  neut_tesr_set_zero (pTesr);

  return;
}

void
neut_tesr_memcpy (struct TESR Tesr1, struct TESR *pTesr2)
{
  neut_tesr_free (pTesr2);
  neut_tesr_memcpy_parms (Tesr1, pTesr2);

  (*pTesr2).VoxCell =
    ut_alloc_3d_int ((*pTesr2).size[0] + 2, (*pTesr2).size[1] + 2,
                     (*pTesr2).size[2] + 2);
  ut_array_3d_int_memcpy (Tesr1.VoxCell, (*pTesr2).size[0] + 2,
                          (*pTesr2).size[1] + 2, (*pTesr2).size[2] + 2,
                          (*pTesr2).VoxCell);

  if (Tesr1.VoxOri)
  {
    (*pTesr2).VoxOri =
      ut_alloc_4d ((*pTesr2).size[0] + 1, (*pTesr2).size[1] + 1,
                   (*pTesr2).size[2] + 1, 4);
    ut_array_4d_memcpy (Tesr1.VoxOri, (*pTesr2).size[0] + 1,
                        (*pTesr2).size[1] + 1, (*pTesr2).size[2] + 1, 4,
                        (*pTesr2).VoxOri);
  }

  if (Tesr1.VoxOriDef)
  {
    (*pTesr2).VoxOriDef =
      ut_alloc_3d_int ((*pTesr2).size[0] + 1, (*pTesr2).size[1] + 1,
                   (*pTesr2).size[2] + 1);
    ut_array_3d_int_memcpy (Tesr1.VoxOriDef, (*pTesr2).size[0] + 1,
                            (*pTesr2).size[1] + 1, (*pTesr2).size[2] + 1,
                            (*pTesr2).VoxOriDef);
  }

  return;
}

void
neut_tesr_memcpy_parms (struct TESR Tesr1, struct TESR *pTesr2)
{
  neut_tesr_free (pTesr2);

  (*pTesr2).Dim = Tesr1.Dim;

  (*pTesr2).Origin = ut_alloc_1d (3);
  ut_array_1d_memcpy (Tesr1.Origin, 3, (*pTesr2).Origin);

  (*pTesr2).hasvoid = Tesr1.hasvoid;

  (*pTesr2).size = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy (Tesr1.size, 3, (*pTesr2).size);

  (*pTesr2).vsize = ut_alloc_1d (3);
  ut_array_1d_memcpy (Tesr1.vsize, 3, (*pTesr2).vsize);

  (*pTesr2).CellQty = Tesr1.CellQty;

  if (Tesr1.CellId)
  {
    (*pTesr2).CellId = ut_alloc_1d_int ((*pTesr2).CellQty + 1);
    ut_array_1d_int_memcpy (Tesr1.CellId + 1, (*pTesr2).CellQty,
                            (*pTesr2).CellId + 1);
  }

  if (Tesr1.CellOri)
  {
    (*pTesr2).CellOri = ut_alloc_2d ((*pTesr2).CellQty + 1, 4);
    ut_array_2d_memcpy (Tesr1.CellOri + 1, (*pTesr2).CellQty, 4,
                        (*pTesr2).CellOri + 1);
  }

  ut_string_string (Tesr1.CellOriDes, &(*pTesr2).CellOriDes);

  if (Tesr1.CellGroup)
  {
    (*pTesr2).CellGroup = ut_alloc_1d_int ((*pTesr2).CellQty + 1);
    ut_array_1d_int_memcpy (Tesr1.CellGroup + 1, (*pTesr2).CellQty,
                            (*pTesr2).CellGroup + 1);
  }

  if (Tesr1.CellCrySym)
    ut_string_string (Tesr1.CellCrySym, &(*pTesr2).CellCrySym);

  if (Tesr1.SeedCoo)
  {
    (*pTesr2).SeedCoo = ut_alloc_2d ((*pTesr2).CellQty + 1, 3);
    ut_array_2d_memcpy (Tesr1.SeedCoo + 1, (*pTesr2).CellQty, 3,
                        (*pTesr2).SeedCoo + 1);
  }

  if (Tesr1.SeedWeight)
  {
    (*pTesr2).SeedWeight = ut_alloc_1d ((*pTesr2).CellQty + 1);
    ut_array_1d_memcpy (Tesr1.SeedWeight + 1, (*pTesr2).CellQty,
                        (*pTesr2).SeedWeight + 1);
  }

  if (Tesr1.CellBBox)
  {
    (*pTesr2).CellBBox = ut_alloc_3d_int ((*pTesr2).CellQty + 1, 3, 2);
    ut_array_3d_int_memcpy (Tesr1.CellBBox + 1, (*pTesr2).CellQty, 3, 2,
                            (*pTesr2).CellBBox + 1);
  }

  return;
}

int
neut_tesr_expr_cells (struct TESR *pTesr, char *expr, int **pcells,
                      int *pcellqty)
{
  int i, id;
  FILE *file;
  (*pcellqty) = 0;

  if ((*pTesr).Dim != 3)
    ut_print_neperbug ();

  if (strcmp (expr, "all") == 0)
  {
    (*pcellqty) = (*pTesr).CellQty;
    (*pcells) = ut_alloc_1d_int (*pcellqty);
    for (i = 0; i < (*pTesr).CellQty; i++)
      (*pcells)[i] = i + 1;
  }

  else if (ut_string_isfilename (expr))
  {
    file = ut_file_open (expr, "r");
    while (fscanf (file, "%d", &id) != EOF)
    {
      (*pcellqty)++;
      (*pcells) = ut_realloc_1d_int (*pcells, *pcellqty);
      (*pcells)[(*pcellqty) - 1] = id;
    }
    ut_file_close (file, expr, "r");
  }
  /*
  else
  {
    int varqty = 1;
    char **vars = ut_alloc_2d_char (varqty, 15);
    double *vals = ut_alloc_1d (varqty);
    int status;
    double res;

    sprintf (vars[0], "id");

    for (i = 1; i <= Tesr.CellQty; i++)
    {
      vals[0] = i;

      int j;
      printf ("expr = %s\n", expr);
      for (j = 0; j < varqty; j++)
        printf ("vars[%d] = %s\n", j, vars[j]);

      abort ();

      status = ut_math_eval (expr, varqty, vars, vals, &res);
      if (status == -1)
        abort ();
      if (res == 1)
      {
        (*pcellqty)++;
        (*pcell) = ut_realloc_1d_int (*pcell, *pcellqty);
        (*pcell)[(*pcellqty) - 1] = i;
      }
    }

    ut_free_2d_char (&vars, varqty);
    ut_free_1d (&vals);
  }
  */
  else
  {
    int varqty;
    char **vars = NULL;
    double *vals = NULL;

    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    (*pcells) = 0;
    for (i = 1; i <= (*pTesr).CellQty; i++)
    {
      int j;
      for (j = 0; j < varqty; j++)
        neut_tesr_var_val_one (*pTesr, "cell", i, vars[j],
                               vals + j, NULL);

      double res;
      int status = ut_math_eval (expr, varqty, vars, vals, &res);
      if (status == -1)
        abort ();
      if (ut_num_equal (res, 1, 1e-6))
      {
        (*pcells) = ut_realloc_1d_int (*pcells, ++(*pcellqty));
        (*pcells)[(*pcellqty) - 1] = i;
      }
    }

    ut_free_2d_char (&vars, varqty);
    ut_free_1d (&vals);
  }

  return 0;
}

void
neut_tesr_scale (struct TESR *pTesr, double scale1, double scale2,
                 double scale3)
{
  int i;
  double *scale = ut_alloc_1d (3);

  ut_array_1d_set_3 (scale, scale1, scale2, scale3);

  for (i = 0; i < (*pTesr).Dim; i++)
    (*pTesr).vsize[i] *= scale[i];

  ut_free_1d (&scale);

  return;
}

void
neut_tesr_translate (struct TESR *pTesr, double s1, double s2, double s3)
{
  (*pTesr).Origin[0] += s1;
  (*pTesr).Origin[1] += s2;
  (*pTesr).Origin[2] += s3;

  return;
}

void
neut_tesr_rotate (struct TESR *pTesr, double **g)
{
  int i, j, k, posold[3];
  double c[3], coonew[3], cooold[3], tmp[3], tmp2[3], **ginv = ol_g_alloc ();
  struct TESR Tesr2;

  ol_g_inverse (g, ginv);

  neut_tesr_set_zero (&Tesr2);

  neut_tesr_memcpy_parms (*pTesr, &Tesr2);
  neut_tesr_alloc (&Tesr2, (*pTesr).Dim, (*pTesr).size, (*pTesr).vsize, NULL);

  neut_tesr_rastercentre (Tesr2, c);

#pragma omp parallel for private(i,j,k,tmp,tmp2,coonew,cooold,posold)
  for (k = 1; k <= Tesr2.size[2]; k++)
  {
    coonew[2] = (k - 0.5) * Tesr2.vsize[2];
    for (j = 1; j <= Tesr2.size[1]; j++)
    {
      coonew[1] = (j - 0.5) * Tesr2.vsize[1];
      for (i = 1; i <= Tesr2.size[0]; i++)
      {
        coonew[0] = (i - 0.5) * Tesr2.vsize[0];

        ut_array_1d_sub (c, coonew, 3, tmp);
        ol_g_vect_vect (g, tmp, tmp2);
        ut_array_1d_add (tmp2, c, 3, cooold);

        neut_tesr_coo_pos (*pTesr, cooold, 0, posold);
        if (neut_tesr_pos_valid (*pTesr, posold))
          Tesr2.VoxCell[i][j][k] =
            (*pTesr).VoxCell[posold[0]][posold[1]][posold[2]];
      }
    }
  }

  if ((*pTesr).CellOri)
  {
    double *q = ol_q_alloc ();
    ol_g_q (g, q);
    for (i = 1; i <= Tesr2.CellQty; i++)
      ol_q_q_q (Tesr2.CellOri[i], q, Tesr2.CellOri[i]);

    ol_q_free (q);
  }

  if ((*pTesr).CellBBox)
    neut_tesr_init_cellbbox (&Tesr2);

  if ((*pTesr).hasvoid != -1)
    neut_tesr_init_hasvoid (&Tesr2);

  if (Tesr2.SeedCoo)
    ut_free_2d (&Tesr2.SeedCoo, Tesr2.CellQty + 1);
  /*
     for (i = 1; i <= (*pTesr).CellQty; i++)
     ol_g_vect_vect (ginv, (*pTesr).SeedCoo[i], (*pTesr).SeedCoo[i]);
   */

  neut_tesr_free (pTesr);
  neut_tesr_memcpy (Tesr2, pTesr);
  neut_tesr_free (&Tesr2);
  ol_g_free (ginv);

  return;
}

void
neut_tesr_rasterscale (struct TESR *pTesr, double scale1, double scale2,
                       double scale3)
{
  int i, j, k, l, m, n, qty, tmp;
  struct TESR Tesr2;
  int **pos = NULL;
  double *scale = NULL;
  double **q = NULL;
  int *id = NULL;

  if (ut_num_requal (scale1, 1, 1e-6) && ut_num_requal (scale2, 1, 1e-6)
      && ut_num_requal (scale3, 1, 1e-6))
    return;

  pos = ut_alloc_2d_int (3, 2);
  scale = ut_alloc_1d (3);

  srand48 (1);
  ut_array_1d_set_3 (scale, scale1, scale2, scale3);

  neut_tesr_set_zero (&Tesr2);
  neut_tesr_memcpy_parms (*pTesr, &Tesr2);

  Tesr2.Dim = (*pTesr).Dim;

  Tesr2.CellQty = (*pTesr).CellQty;

  for (i = 0; i < (*pTesr).Dim; i++)
  {
    Tesr2.size[i] = (*pTesr).size[i] * scale[i];
    scale[i] = (double) Tesr2.size[i] / (*pTesr).size[i];
  }

  for (i = 0; i < 3; i++)
    Tesr2.vsize[i] = (*pTesr).vsize[i] / scale[i];

  Tesr2.VoxCell =
    ut_alloc_3d_int (Tesr2.size[0] + 2, Tesr2.size[1] + 2, Tesr2.size[2] + 2);
  if ((*pTesr).VoxOri)
    Tesr2.VoxOri =
      ut_alloc_4d (Tesr2.size[0] + 1, Tesr2.size[1] + 1, Tesr2.size[2] + 1,
                   4);
  if ((*pTesr).VoxOriDef)
    Tesr2.VoxOriDef =
      ut_alloc_3d_int (Tesr2.size[0] + 1, Tesr2.size[1] + 1, Tesr2.size[2] + 1);

  for (k = 1; k <= Tesr2.size[2]; k++)
  {
    if (Tesr2.Dim == 3)
    {
      pos[2][0] = 1 + (double) (k - 1) / scale[2];
      pos[2][1] = 1 + (double) (k - 1e-6) / scale[2];
    }
    else
      ut_array_1d_int_set (pos[2], 2, 1);

    for (j = 1; j <= Tesr2.size[1]; j++)
    {
      if (Tesr2.Dim >= 2)
      {
        pos[1][0] = 1 + (double) (j - 1) / scale[1];
        pos[1][1] = 1 + (double) (j - 1e-6) / scale[1];
      }
      else
        ut_array_1d_int_set (pos[1], 2, 1);

      for (i = 1; i <= Tesr2.size[0]; i++)
      {
        pos[0][0] = 1 + (double) (i - 1) / scale[0];
        pos[0][1] = 1 + (double) (i - 1e-6) / scale[0];

        if (scale1 < 1 || scale2 < 1 || scale3 < 1)
        {
          qty =
            (pos[2][1] - pos[2][0] + 1) * (pos[1][1] - pos[1][0] +
                                           1) * (pos[0][1] - pos[0][0] + 1);
          int *array = ut_alloc_1d_int (qty);
          int *vals = NULL, *valqtys = NULL;
          int valqty, qty2;

          qty = 0;
          for (n = pos[2][0]; n <= pos[2][1]; n++)
            for (m = pos[1][0]; m <= pos[1][1]; m++)
              for (l = pos[0][0]; l <= pos[0][1]; l++)
                array[qty++] = (*pTesr).VoxCell[l][m][n];

          if ((*pTesr).VoxOri)
          {
            q = ut_alloc_2d (qty, 4);
            qty = 0;
            for (n = pos[2][0]; n <= pos[2][1]; n++)
              for (m = pos[1][0]; m <= pos[1][1]; m++)
                for (l = pos[0][0]; l <= pos[0][1]; l++)
                  ol_q_memcpy ((*pTesr).VoxOri[l][m][n], q[qty++]);
          }

          if ((*pTesr).VoxOriDef)
          {
            id = ut_alloc_1d_int (qty);
            qty = 0;
            for (n = pos[2][0]; n <= pos[2][1]; n++)
              for (m = pos[1][0]; m <= pos[1][1]; m++)
                for (l = pos[0][0]; l <= pos[0][1]; l++)
                  id[qty++] = (*pTesr).VoxOriDef[l][m][n];
          }

          ut_array_1d_int_valnbs (array, qty, &vals, &valqtys, &valqty);

          if (valqty == 0)
            ut_print_neperbug ();
          else if (valqty == 1)
          {
            Tesr2.VoxCell[i][j][k] = vals[0];
            if ((*pTesr).VoxOri)
              // FIXME we should compute an average orientation
              ol_q_memcpy (q[0], Tesr2.VoxOri[i][j][k]);
          }
          else
          {
            // qty2: number of equally represented cells
            qty2 = ut_array_1d_int_valnb (valqtys, valqty, valqtys[0]);

            if (qty2 == 1)
              Tesr2.VoxCell[i][j][k] = vals[0];
            else
              // FIXME
              Tesr2.VoxCell[i][j][k] = vals[0];
            // Tesr2.VoxCell[i][j][k] = val[0][(int) (drand48 () * qty2)];

            if ((*pTesr).VoxOri)
            {
              // FIXME we should compute an average orientation
              tmp = ut_array_1d_int_eltpos (array, qty, vals[0]);
              ol_q_memcpy (q[tmp], Tesr2.VoxOri[i][j][k]);
            }

            if ((*pTesr).VoxOriDef)
            {
              tmp = ut_array_1d_int_eltpos (array, qty, vals[0]);
              Tesr2.VoxOriDef[i][j][k] = id[tmp];
            }
          }

          if ((*pTesr).VoxOri)
            ut_free_2d (&q, qty);

          if ((*pTesr).VoxOriDef)
            ut_free_1d_int (&id);

          ut_free_1d_int (&array);
          ut_free_1d_int (&vals);
          ut_free_1d_int (&valqtys);
        }

        else
        {
          double *coo = ut_alloc_1d (3);
          int *pos = ut_alloc_1d_int (3);

          ut_array_1d_int_set_3 (pos, i, j, k);

          neut_tesr_pos_coo (Tesr2, pos, coo);
          neut_tesr_coo_pos (*pTesr, coo, 0, pos);

          Tesr2.VoxCell[i][j][k] = (*pTesr).VoxCell[pos[0]][pos[1]][pos[2]];

          ut_free_1d_int (&pos);
          ut_free_1d (&coo);
        }
      }
    }
  }

  neut_tesr_free (pTesr);
  neut_tesr_memcpy (Tesr2, pTesr);
  neut_tesr_free (&Tesr2);

  if ((*pTesr).CellBBox)
    neut_tesr_init_cellbbox (pTesr);

  ut_free_2d_int (&pos, 3);
  ut_free_1d (&scale);

  return;
}

void
neut_tesr_cells_voxqty (struct TESR Tesr, int **pvoxqty)
{
  int i, j, k;

  ut_free_1d_int (pvoxqty);

  (*pvoxqty) = ut_alloc_1d_int (Tesr.CellQty + 1);

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        (*pvoxqty)[Tesr.VoxCell[i][j][k]]++;

  return;
}

void
neut_tesr_cell_tesr (struct TESR Tesr, int cell, struct TESR *pCellTesr)
{
  int i, j, k;

  neut_tesr_memcpy_parms (Tesr, pCellTesr);

  (*pCellTesr).CellQty = 1;
  (*pCellTesr).VoxCell =
    ut_alloc_3d_int (Tesr.size[0] + 2, Tesr.size[1] + 2, Tesr.size[2] + 2);

  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
        if (Tesr.VoxCell[i][j][k] == cell)
          (*pCellTesr).VoxCell[i][j][k] = cell;

  return;
}

void
neut_tesr_cell_pos_neighpos (struct TESR Tesr, int cell, int *pos, int dim,
                             int ***ppos, int *pqty)
{
  int *array = ut_alloc_1d_int (1);
  array[0] = cell;

  neut_tesr_cells_pos_neighpos (Tesr, array, 1, pos, dim, ppos, pqty);

  ut_free_1d_int (&array);

  return;
}

// cell == NULL has a special behavior: it takes all cells
// cellqty = -1 takes all cells + void (VoxCell = 0)
void
neut_tesr_cells_pos_neighpos (struct TESR Tesr, int *cell, int cellqty,
                              int *pos, int dim, int ***ppos, int *pqty)
{
  if (Tesr.Dim == 3)
    neut_tesr_cells_pos_neighpos_3d (Tesr, cell, cellqty, pos, dim, ppos,
                                     pqty);
  else if (Tesr.Dim == 2)
    neut_tesr_cells_pos_neighpos_2d (Tesr, cell, cellqty, pos, dim, ppos,
                                     pqty);
  else
    ut_print_neperbug ();

  return;
}

void
neut_tesr_cells_pos_neighpos_3d (struct TESR Tesr, int *cell, int cellqty,
                                 int *pos, int dim, int ***ppos, int *pqty)
{
  int i, j, vqty;
  int **v = NULL;
  int *voxpos = ut_alloc_1d_int (3);

  if (Tesr.Dim != 3)
    ut_print_neperbug ();

  /*
     if (*ppos != NULL)
     ut_free_2d_int (ppos, *pqty);
   */

  // check memory leak
  (*pqty) = 0;
  (*ppos) = NULL;

  if (dim == 0)
  {
    vqty = 26;
    v = ut_alloc_2d_int (vqty, 3);
    ut_array_1d_int_set_3 (v[0], -1, -1, -1);
    ut_array_1d_int_set_3 (v[1], -1, -1, 0);
    ut_array_1d_int_set_3 (v[2], -1, -1, 1);
    ut_array_1d_int_set_3 (v[3], -1, 0, -1);
    ut_array_1d_int_set_3 (v[4], -1, 0, 0);
    ut_array_1d_int_set_3 (v[5], -1, 0, 1);
    ut_array_1d_int_set_3 (v[6], -1, 1, -1);
    ut_array_1d_int_set_3 (v[7], -1, 1, 0);
    ut_array_1d_int_set_3 (v[8], -1, 1, 1);
    ut_array_1d_int_set_3 (v[9], 0, -1, -1);
    ut_array_1d_int_set_3 (v[10], 0, -1, 0);
    ut_array_1d_int_set_3 (v[11], 0, -1, 1);
    ut_array_1d_int_set_3 (v[12], 0, 0, -1);
    ut_array_1d_int_set_3 (v[13], 0, 0, 1);
    ut_array_1d_int_set_3 (v[14], 0, 1, -1);
    ut_array_1d_int_set_3 (v[15], 0, 1, 0);
    ut_array_1d_int_set_3 (v[16], 0, 1, 1);
    ut_array_1d_int_set_3 (v[17], 1, -1, -1);
    ut_array_1d_int_set_3 (v[18], 1, -1, 0);
    ut_array_1d_int_set_3 (v[19], 1, -1, 1);
    ut_array_1d_int_set_3 (v[20], 1, 0, -1);
    ut_array_1d_int_set_3 (v[21], 1, 0, 0);
    ut_array_1d_int_set_3 (v[22], 1, 0, 1);
    ut_array_1d_int_set_3 (v[23], 1, 1, -1);
    ut_array_1d_int_set_3 (v[24], 1, 1, 0);
    ut_array_1d_int_set_3 (v[25], 1, 1, 1);
  }

  else if (dim == 1)
  {
    vqty = 18;
    v = ut_alloc_2d_int (vqty, 3);
    ut_array_1d_int_set_3 (v[0], -1, -1, 0);
    ut_array_1d_int_set_3 (v[1], -1, 0, -1);
    ut_array_1d_int_set_3 (v[2], -1, 0, 0);
    ut_array_1d_int_set_3 (v[3], -1, 0, 1);
    ut_array_1d_int_set_3 (v[4], -1, 1, 0);
    ut_array_1d_int_set_3 (v[5], 0, -1, -1);
    ut_array_1d_int_set_3 (v[6], 0, -1, 0);
    ut_array_1d_int_set_3 (v[7], 0, -1, 1);
    ut_array_1d_int_set_3 (v[8], 0, 0, -1);
    ut_array_1d_int_set_3 (v[9], 0, 0, 1);
    ut_array_1d_int_set_3 (v[10], 0, 1, -1);
    ut_array_1d_int_set_3 (v[11], 0, 1, 0);
    ut_array_1d_int_set_3 (v[12], 0, 1, 1);
    ut_array_1d_int_set_3 (v[13], 1, -1, 0);
    ut_array_1d_int_set_3 (v[14], 1, 0, -1);
    ut_array_1d_int_set_3 (v[15], 1, 0, 0);
    ut_array_1d_int_set_3 (v[16], 1, 0, 1);
    ut_array_1d_int_set_3 (v[17], 1, 1, 0);
  }

  else if (dim == 2)
  {
    vqty = 6;
    v = ut_alloc_2d_int (vqty, 3);
    ut_array_1d_int_set_3 (v[0], -1, 0, 0);
    ut_array_1d_int_set_3 (v[1], 1, 0, 0);
    ut_array_1d_int_set_3 (v[2], 0, -1, 0);
    ut_array_1d_int_set_3 (v[3], 0, 1, 0);
    ut_array_1d_int_set_3 (v[4], 0, 0, -1);
    ut_array_1d_int_set_3 (v[5], 0, 0, 1);
  }

  else
    abort ();

  for (i = 0; i < vqty; i++)
  {
    ut_array_1d_int_add (pos, v[i], 3, voxpos);

    int skip = 0;
    for (j = 0; !skip && j < 3; j++)
      if (voxpos[j] < 1 || voxpos[j] > Tesr.size[j])
        skip = 1;

    if (skip)
      continue;

    if ((cell == NULL && cellqty == -1)
        || (cell == NULL && cellqty == 0
            && Tesr.VoxCell[voxpos[0]][voxpos[1]][voxpos[2]] != 0)
        || ut_array_1d_int_eltpos (cell, cellqty,
                                   Tesr.
                                   VoxCell[voxpos[0]][voxpos[1]][voxpos[2]])
        != -1)
    {
      (*pqty)++;
      (*ppos) = ut_realloc_2d_int_addline (*ppos, *pqty, 3);
      ut_array_1d_int_memcpy (voxpos, 3, (*ppos)[(*pqty) - 1]);
    }
  }

  ut_free_2d_int (&v, vqty);
  ut_free_1d_int (&voxpos);

  return;
}

// cell == NULL has a special behavior: it takes all cells
// cellqty = -1 takes all cells + void (VoxCell = 0)
void
neut_tesr_cells_pos_neighpos_2d (struct TESR Tesr, int *cell, int cellqty,
                                 int *pos, int dim, int ***ppos, int *pqty)
{
  int i, j, vqty;
  int **v = NULL;
  int *voxpos = ut_alloc_1d_int (3);

  if (Tesr.Dim != 2)
    ut_print_neperbug ();

  // check memory leak
  (*pqty) = 0;
  (*ppos) = NULL;

  if (dim == 0)
  {
    vqty = 8;
    v = ut_alloc_2d_int (vqty, 2);
    ut_array_1d_int_set_2 (v[0], -1, -1);
    ut_array_1d_int_set_2 (v[1], -1, 0);
    ut_array_1d_int_set_2 (v[2], -1, 1);
    ut_array_1d_int_set_2 (v[3], 0, -1);
    ut_array_1d_int_set_2 (v[4], 0, 1);
    ut_array_1d_int_set_2 (v[5], 1, -1);
    ut_array_1d_int_set_2 (v[6], 1, 0);
    ut_array_1d_int_set_2 (v[7], 1, 1);
  }

  else if (dim == 1)
  {
    vqty = 4;
    v = ut_alloc_2d_int (vqty, 2);
    ut_array_1d_int_set_2 (v[0], -1, 0);
    ut_array_1d_int_set_2 (v[1], 1, 0);
    ut_array_1d_int_set_2 (v[2], 0, -1);
    ut_array_1d_int_set_2 (v[3], 0, 1);
  }

  else
    abort ();

  for (i = 0; i < vqty; i++)
  {
    ut_array_1d_int_add (pos, v[i], 2, voxpos);
    voxpos[2] = 1;

    int skip = 0;
    for (j = 0; !skip && j < 2; j++)
      if (voxpos[j] < 1 || voxpos[j] > Tesr.size[j])
        skip = 1;

    if (skip)
      continue;

    if ((cell == NULL && cellqty == -1)
        || (cell == NULL && cellqty == 0
            && Tesr.VoxCell[voxpos[0]][voxpos[1]][1] != 0)
        || ut_array_1d_int_eltpos (cell, cellqty,
                                   Tesr.VoxCell[voxpos[0]][voxpos[1]][1]) !=
        -1)
    {
      (*pqty)++;
      (*ppos) = ut_realloc_2d_int_addline (*ppos, *pqty, 3);
      ut_array_1d_int_memcpy (voxpos, 3, (*ppos)[(*pqty) - 1]);
    }
  }

  ut_free_2d_int (&v, vqty);
  ut_free_1d_int (&voxpos);

  return;
}

void
neut_tesr_cell_tesrpos (struct TESR Tesr, int cell, int *pvoxqty,
                        int ***pvoxpos)
{
  int i, j, k;

  (*pvoxqty) = 0;
  (*pvoxpos) = NULL;

  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
        if (Tesr.VoxCell[i][j][k] == cell)
        {
          (*pvoxpos) =
            ut_realloc_2d_int_addline ((*pvoxpos), ++(*pvoxqty), 3);
          ut_array_1d_int_set_3 ((*pvoxpos)[(*pvoxqty) - 1], i, j, k);
        }

  return;
}

int
neut_tesr_renumber_continuous (struct TESR *pTesr)
{
  int i, j, k, l, cellidmax, oldcellidmax;
  int *cellvoxqty = NULL;
  int CellQty_old = 0;
  int *CellId_old = NULL;
  int *CellId_old_inv = NULL;
  int CellId_old_max;
  double **CellOri_old = NULL;
  int *CellGroup_old = NULL;
  double **SeedCoo_old = NULL;
  double *SeedWeight_old = NULL;
  int *CellId_inv = NULL;
  int CellId_max;
  int pos;

  // Pre-processing --------------------------------------------------------------

  // If (*pTesr).CellQty and (*pTesr).CellId defined and one of the cells has an id equal to zero:
  // - removing cell by setting its voxcell and bbox to zero
  for (l = 1; l <= (*pTesr).CellQty; l++)
    if ((*pTesr).CellId && (*pTesr).CellId[l] == 0)
    {
      for (k = (*pTesr).CellBBox[l][2][0]; k <= (*pTesr).CellBBox[l][2][1]; k++)
        for (j = (*pTesr).CellBBox[l][1][0]; j <= (*pTesr).CellBBox[l][1][1]; j++)
          for (i = (*pTesr).CellBBox[l][0][0]; i <= (*pTesr).CellBBox[l][0][1]; i++)
            if ((*pTesr).VoxCell[i][j][k] == l)
              (*pTesr).VoxCell[i][j][k] = 0;

      ut_array_2d_int_zero ((*pTesr).CellBBox[l], 3, 2);
    }

  // Preparing renumbering per se -------------------------------------------------

  // From (*pTesr).VoxCell:
  // - determining the max cell id as cellidmax
  // - recording the number of voxels of each cell in cellvoxqty (from 1 to cellidmax)
  cellidmax = 0;
  cellvoxqty = ut_alloc_1d_int (1);
  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
        if ((*pTesr).VoxCell[i][j][k] > cellidmax)
        {
          oldcellidmax = cellidmax;
          cellidmax = (*pTesr).VoxCell[i][j][k];
          cellvoxqty = ut_realloc_1d_int_zero (cellvoxqty, cellidmax + 1, cellidmax - oldcellidmax);
        }

        cellvoxqty[(*pTesr).VoxCell[i][j][k]]++;
      }

  // If (*pTesr).CellId undefined but (*pTesr).CellQty defined:
  // - setting it to 1, 2, ...
  if ((*pTesr).CellQty && !(*pTesr).CellId)
  {
    (*pTesr).CellId = ut_alloc_1d_int ((*pTesr).CellQty + 1);
    ut_array_1d_int_set_id ((*pTesr).CellId, (*pTesr).CellQty + 1);
  }

  // If CellQty defined, recording the existing variables as *_old, before
  // renumbering;

  if ((*pTesr).CellQty)
  {
    CellQty_old = (*pTesr).CellQty;

    if ((*pTesr).CellId)
    {
      CellId_old = ut_alloc_1d_int (CellQty_old + 1);
      ut_array_1d_int_memcpy ((*pTesr).CellId + 1, CellQty_old, CellId_old + 1);
      ut_array_1d_int_inv (CellId_old + 1, CellQty_old, &CellId_old_inv,
                           &CellId_old_max);
      ut_array_1d_int_addval (CellId_old_inv, CellId_old_max, 1,
                              CellId_old_inv);
    }

    if ((*pTesr).CellOri)
    {
      CellOri_old = ut_alloc_2d (CellQty_old + 1, 4);
      ut_array_2d_memcpy ((*pTesr).CellOri + 1, CellQty_old, 4,
                          CellOri_old + 1);
    }

    if ((*pTesr).CellGroup)
    {
      CellGroup_old = ut_alloc_1d_int (CellQty_old + 1);
      ut_array_1d_int_memcpy ((*pTesr).CellGroup + 1, CellQty_old,
                              CellGroup_old + 1);
    }

    if ((*pTesr).SeedCoo)
    {
      SeedCoo_old = ut_alloc_2d (CellQty_old + 1, 3);
      ut_array_2d_memcpy ((*pTesr).SeedCoo + 1, CellQty_old, 3,
                          SeedCoo_old + 1);
    }

    if ((*pTesr).SeedWeight)
    {
      SeedWeight_old = ut_alloc_1d (CellQty_old + 1);
      ut_array_1d_memcpy ((*pTesr).SeedWeight + 1, CellQty_old,
                          SeedWeight_old + 1);
    }
  }

  // Starting renumbering --------------------------------------------------------

  // Calculating (*pTesr).CellQty and (*pTesr).CellId

  (*pTesr).CellQty = 0;
  for (i = 1; i <= cellidmax; i++)
    if (cellvoxqty[i] > 0)
      (*pTesr).CellQty++;

  (*pTesr).CellId = ut_alloc_1d_int ((*pTesr).CellQty + 1);

  pos = 0;
  for (i = 1; i <= cellidmax; i++)
    if (cellvoxqty[i] > 0)
      (*pTesr).CellId[++pos] = i;

  ut_array_1d_int_inv ((*pTesr).CellId, (*pTesr).CellQty + 1, &CellId_inv,
                       &CellId_max);

  // renumbering VoxCell
  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
        (*pTesr).VoxCell[i][j][k] = CellId_inv[(*pTesr).VoxCell[i][j][k]];

  if (CellId_old)
    for (i = 1; i <= (*pTesr).CellQty; i++)
      (*pTesr).CellId[i] = CellId_old[(*pTesr).CellId[i]];

  if (CellOri_old)
    for (i = 1; i <= (*pTesr).CellQty; i++)
      ut_array_1d_memcpy (CellOri_old[CellId_old_inv[(*pTesr).CellId[i]]], 4,
                          (*pTesr).CellOri[i]);

  if (CellGroup_old)
    for (i = 1; i <= (*pTesr).CellQty; i++)
      (*pTesr).CellGroup[i] = CellGroup_old[CellId_old_inv[(*pTesr).CellId[i]]];

  if (SeedCoo_old)
    for (i = 1; i <= (*pTesr).CellQty; i++)
      ut_array_1d_memcpy (SeedCoo_old[CellId_old_inv[(*pTesr).CellId[i]]], 3,
                          (*pTesr).SeedCoo[i]);

  if (SeedWeight_old)
    for (i = 1; i <= (*pTesr).CellQty; i++)
      (*pTesr).SeedWeight[i] =
        SeedWeight_old[CellId_old_inv[(*pTesr).CellId[i]]];

  ut_free_1d_int (&cellvoxqty);
  ut_free_1d_int (&CellId_inv);
  ut_free_1d_int (&CellId_old);
  ut_free_1d_int (&CellId_old_inv);
  ut_free_1d_int (&CellGroup_old);
  if (CellOri_old)
    ut_free_2d (&CellOri_old, CellQty_old);
  if (SeedCoo_old)
    ut_free_2d (&SeedCoo_old, CellQty_old);
  ut_free_1d (&SeedWeight_old);

  int useful = 0;
  for (i = 0; i <= (*pTesr).CellQty; i++)
    if ((*pTesr).CellId[i] != i)
    {
      useful = 1;
      break;
    }

  if (!useful)
    ut_free_1d_int (&(*pTesr).CellId);

  return 0;
}

int
neut_tesr_cell_switch (struct TESR *pTesr, int cell1, int cell2)
{
  int i, j, k;

  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
        if ((*pTesr).VoxCell[i][j][k] == cell1)
          (*pTesr).VoxCell[i][j][k] = cell2;
        else if ((*pTesr).VoxCell[i][j][k] == cell2)
          (*pTesr).VoxCell[i][j][k] = cell1;

  if ((*pTesr).CellId)
    ut_array_1d_int_switch ((*pTesr).CellId, cell1, cell2);
  if ((*pTesr).CellOri)
    ut_array_2d_switchlines ((*pTesr).CellOri, 4, cell1, cell2);
  if ((*pTesr).CellBBox)
    ut_array_3d_int_switcharrays ((*pTesr).CellBBox, 3, 2, cell1, cell2);
  if ((*pTesr).SeedCoo)
    ut_array_2d_switchlines ((*pTesr).SeedCoo, 3, cell1, cell2);
  if ((*pTesr).SeedWeight)
    ut_array_1d_switch ((*pTesr).SeedWeight, cell1, cell2);

  return 0;
}

void
neut_tesr_init_tesrsize (struct TESR *pTesr, struct TESS Domain, int dim,
                         char *tesrsizestring)
{
  int i, qty, val;
  double avdsize;
  char **list = NULL;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Domain, bbox);

  ut_list_break (tesrsizestring, NEUT_SEP_DEP, &list, &qty);

  (*pTesr).Dim = dim;

  (*pTesr).size = ut_alloc_1d_int (3);
  ut_array_1d_int_set ((*pTesr).size, 3, 1);

  if (qty == 1)
  {
    ut_math_eval_int (list[0], 0, NULL, NULL, &val);

    if ((*pTesr).Dim == 1)
      avdsize = bbox[0][1] - bbox[0][0];
    else if ((*pTesr).Dim == 2)
      avdsize =
        pow ((bbox[0][1] - bbox[0][0]) * (bbox[1][1] - bbox[1][0]), 0.5);
    else if ((*pTesr).Dim == 3)
      avdsize =
        pow ((bbox[0][1] - bbox[0][0]) * (bbox[1][1] -
                                          bbox[1][0]) * (bbox[2][1] -
                                                         bbox[2][0]),
             0.3333333333333333);
    else
    {
      ut_print_neperbug ();
      abort ();
    }

    for (i = 0; i < (*pTesr).Dim; i++)
      (*pTesr).size[i] =
        ut_num_d2ri (val * (bbox[i][1] - bbox[i][0]) / avdsize);
  }
  else if (qty == (*pTesr).Dim)
    for (i = 0; i < (*pTesr).Dim; i++)
      ut_math_eval_int (list[i], 0, NULL, NULL, &((*pTesr).size[i]));
  else
    abort ();

  (*pTesr).vsize = ut_alloc_1d (3);

  for (i = 0; i < (*pTesr).Dim; i++)
    (*pTesr).vsize[i] = (bbox[i][1] - bbox[i][0]) / (*pTesr).size[i];

  ut_free_2d (&bbox, 3);
  ut_free_2d_char (&list, qty);

  return;
}

void
neut_tesr_crop (struct TESR *pTesr, char *crop)
{
  int i, j, k, id;
  double d;
  double *coo = ut_alloc_1d (3);
  double *coo2 = ut_alloc_1d (3);
  double *cube = ut_alloc_1d (6);
  int *pt = ut_alloc_1d_int (3);
  int qty = 0;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  char *shape = ut_alloc_1d_char (strlen (crop) + 1);

  if (!strncmp (crop, "crop(", 5))
  {
    sscanf (crop + 5, "%s", shape);
    shape[strlen (shape) - 1] = '\0';
  }
  else if (!strncmp (crop, "crop:", 5))
    sscanf (crop + 5, "%s", shape);
  else
    strcpy (shape, crop);

  ut_string_function (shape, &fct, &vars, &vals, &qty);

  if (!strcmp (fct, "cylinder") || !strcmp (fct, "circle"))
  {
    if (qty != 3)
      ut_print_message (2, 3, "Failed to parse `%s'.\n", shape);

    neut_tesr_expr_val_one (*pTesr, "tesr", 0, vals[0], coo, NULL);
    neut_tesr_expr_val_one (*pTesr, "tesr", 0, vals[1], coo + 1, NULL);
    neut_tesr_expr_val_one (*pTesr, "tesr", 0, vals[2], &d, NULL);

    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
        ut_array_1d_int_set_3 (pt, i, j, 1);
        neut_tesr_pos_coo (*pTesr, pt, coo2);

        if (ut_space_dist_2d (coo, coo2) > .5 * d)
        {
          ut_array_1d_int_set ((*pTesr).VoxCell[i][j] + 1, (*pTesr).size[2],
                               0);
          if ((*pTesr).VoxOriDef)
            ut_array_1d_int_set ((*pTesr).VoxOriDef[i][j] + 1, (*pTesr).size[2],
                                 0);
        }
      }
  }
  else if (!strcmp (fct, "cube"))
  {
    if (qty != 6)
      ut_print_message (2, 3, "Failed to parse `%s'.\n", shape);

    for (i = 0; i < 6; i++)
      neut_tesr_expr_val_one (*pTesr, "tesr", 0, vals[i], cube + i, NULL);

    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
          if ((*pTesr).VoxCell[i][j][k])
          {
            ut_array_1d_int_set_3 (pt, i, j, k);
            neut_tesr_pos_coo (*pTesr, pt, coo2);

            for (id = 0; id < 3; id++)
              if (coo2[id] < cube[2 * id] || coo2[id] > cube[2 * id + 1])
              {
                (*pTesr).VoxCell[i][j][k] = 0;
                if ((*pTesr).VoxOriDef)
                  (*pTesr).VoxOriDef[i][j][k] = 0;
                break;
              }
          }
  }
  else if (!strcmp (fct, "square"))
  {
    if (qty != 4)
      ut_print_message (2, 3, "Failed to parse `%s'.\n", shape);

    if ((*pTesr).Dim == 3)
      ut_print_message (2, 3, "`square' available for 2D tessellation only.\n");

    for (i = 0; i < 4; i++)
      neut_tesr_expr_val_one (*pTesr, "tesr", 0, vals[i], cube + i, NULL);

    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
        if ((*pTesr).VoxCell[i][j][1])
        {
          ut_array_1d_int_set_3 (pt, i, j, 1);
          neut_tesr_pos_coo (*pTesr, pt, coo2);

          for (id = 0; id < 2; id++)
            if (coo2[id] < cube[2 * id] || coo2[id] > cube[2 * id + 1])
            {
              (*pTesr).VoxCell[i][j][1] = 0;
              if ((*pTesr).VoxOriDef)
                (*pTesr).VoxOriDef[i][j][1] = 0;
              break;
            }
        }
  }
  else
    ut_print_message (2, 2, "Unknown shape `%s'.\n", shape);

  neut_tesr_renumber_continuous (pTesr);

  if ((*pTesr).CellBBox)
    neut_tesr_init_cellbbox (pTesr);
  if ((*pTesr).hasvoid != -1)
    neut_tesr_init_hasvoid (pTesr);

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, qty);
  ut_free_2d_char (&vals, qty);
  ut_free_1d (&coo);
  ut_free_1d (&coo2);
  ut_free_1d_int (&pt);
  ut_free_1d (&cube);

  return;
}

int
neut_tesr_2d (struct TESR *pTesr)
{
  if ((*pTesr).Dim == 3 && (*pTesr).size[2] == 1)
  {
    (*pTesr).Dim = 2;
    return 0;
  }
  else
    return -1;
}

void
neut_tesr_bounds (struct TESR *pTesr, int **bounds)
{
  int i, j, k;

  // z0
  for (k = 1; k <= (*pTesr).size[2] && !bounds[2][0]; k++)
    for (j = 1; j <= (*pTesr).size[1] && !bounds[2][0]; j++)
      for (i = 1; i <= (*pTesr).size[0] && !bounds[2][0]; i++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[2][0] = k;

  // z1
  for (k = (*pTesr).size[2]; k >= 1 && !bounds[2][1]; k--)
    for (j = 1; j <= (*pTesr).size[1] && !bounds[2][1]; j++)
      for (i = 1; i <= (*pTesr).size[0] && !bounds[2][1]; i++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[2][1] = k;

  // y0
  for (j = 1; j <= (*pTesr).size[1] && !bounds[1][0]; j++)
    for (k = 1; k <= (*pTesr).size[2] && !bounds[1][0]; k++)
      for (i = 1; i <= (*pTesr).size[0] && !bounds[1][0]; i++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[1][0] = j;

  // y1
  for (j = (*pTesr).size[1]; j >= 1 && !bounds[1][1]; j--)
    for (k = 1; k <= (*pTesr).size[2] && !bounds[1][1]; k++)
      for (i = 1; i <= (*pTesr).size[0] && !bounds[1][1]; i++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[1][1] = j;

  // x0
  for (i = 1; i <= (*pTesr).size[0] && !bounds[0][0]; i++)
    for (j = 1; j <= (*pTesr).size[1] && !bounds[0][0]; j++)
      for (k = 1; k <= (*pTesr).size[2] && !bounds[0][0]; k++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[0][0] = i;

  // x1
  for (i = (*pTesr).size[0]; i >= 1 && !bounds[0][1]; i--)
    for (j = 1; j <= (*pTesr).size[1] && !bounds[0][1]; j++)
      for (k = 1; k <= (*pTesr).size[2] && !bounds[0][1]; k++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
          bounds[0][1] = i;

  return;
}

void
neut_tesr_autocrop (struct TESR *pTesr)
{
  int i, j, k;
  int **bounds = ut_alloc_2d_int (3, 2);
  int *size = ut_alloc_1d_int (3);
  struct TESR Tesr2;

  neut_tesr_set_zero (&Tesr2);

  neut_tesr_bounds (pTesr, bounds);
  for (i = 0; i < 3; i++)
    size[i] = bounds[i][1] - bounds[i][0] + 1;

  neut_tesr_memcpy_parms (*pTesr, &Tesr2);
  neut_tesr_alloc (&Tesr2, (*pTesr).Dim, size, (*pTesr).vsize, NULL);

  for (i = 1; i <= size[0]; i++)
    for (j = 1; j <= size[1]; j++)
      for (k = 1; k <= size[2]; k++)
        Tesr2.VoxCell[i][j][k] =
          (*pTesr).VoxCell[i + bounds[0][0] - 1][j + bounds[1][0] - 1][k +
                                                                       bounds
                                                                       [2][0]
                                                                       - 1];

  if ((*pTesr).VoxOri)
  {
    Tesr2.VoxOri = ut_alloc_4d (Tesr2.size[0] + 1, Tesr2.size[1] + 1, Tesr2.size[2] + 1, 4);
    for (i = 1; i <= size[0]; i++)
      for (j = 1; j <= size[1]; j++)
        for (k = 1; k <= size[2]; k++)
          ol_q_memcpy ((*pTesr).VoxOri[i + bounds[0][0] - 1]
                                      [j + bounds[1][0] - 1]
                                      [k + bounds[2][0] - 1],
                       Tesr2.VoxOri[i][j][k]);
  }

  if ((*pTesr).VoxOriDef)
  {
    Tesr2.VoxOriDef = ut_alloc_3d_int (Tesr2.size[0] + 1, Tesr2.size[1] + 1, Tesr2.size[2] + 1);
    for (i = 1; i <= size[0]; i++)
      for (j = 1; j <= size[1]; j++)
        for (k = 1; k <= size[2]; k++)
          Tesr2.VoxOriDef[i][j][k] = (*pTesr).VoxOriDef[i + bounds[0][0] - 1]
            [j + bounds[1][0] - 1] [k + bounds[2][0] - 1];

  }

  for (i = 0; i < 3; i++)
    Tesr2.Origin[i] = (*pTesr).Origin[i] + (bounds[i][0] - 1) * Tesr2.vsize[i];

  for (i = 1; i <= (*pTesr).CellQty; i++)
    for (j = 0; j < 3; j++)
    {
      Tesr2.CellBBox[i][j][0] -= bounds[j][0] + 1;
      Tesr2.CellBBox[i][j][1] -= bounds[j][0] + 1;
    }

  neut_tesr_free (pTesr);
  neut_tesr_memcpy (Tesr2, pTesr);

  ut_free_2d_int (&bounds, 3);
  ut_free_1d_int (&size);

  if ((*pTesr).CellBBox)
    neut_tesr_init_cellbbox (pTesr);
  if ((*pTesr).hasvoid != -1)
    neut_tesr_init_hasvoid (pTesr);

  return;
}

void
neut_tesr_init_cellbbox (struct TESR *pTesr)
{
  int i, j, k, l, cell, *pos = NULL;

  if (!(*pTesr).CellQty)
    return;

  pos = ut_alloc_1d_int (3);

  if ((*pTesr).CellBBox)
    ut_free_3d_int (&(*pTesr).CellBBox, (*pTesr).CellQty + 1, 3);

  (*pTesr).CellBBox = ut_alloc_3d_int ((*pTesr).CellQty + 1, 3, 2);

  for (i = 1; i <= (*pTesr).CellQty; i++)
    for (j = 0; j < 3; j++)
    {
      (*pTesr).CellBBox[i][j][0] = INT_MAX;
      (*pTesr).CellBBox[i][j][1] = INT_MIN;
    }

  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
        ut_array_1d_int_set_3 (pos, i, j, k);

        cell = (*pTesr).VoxCell[i][j][k];

        for (l = 0; l < 3; l++)
        {
          (*pTesr).CellBBox[cell][l][0] =
            ut_num_min ((*pTesr).CellBBox[cell][l][0], pos[l]);
          (*pTesr).CellBBox[cell][l][1] =
            ut_num_max ((*pTesr).CellBBox[cell][l][1], pos[l]);
        }
      }

  ut_free_1d_int (&pos);

  return;
}

void
neut_tesr_init_hasvoid (struct TESR *pTesr)
{
  int i, j, k = 1;

  if (!(*pTesr).VoxCell)
    return;

  (*pTesr).hasvoid = 0;
  for (i = 1; i <= (*pTesr).size[0]; i++)
  {
    for (j = 1; j <= (*pTesr).size[1]; j++)
    {
      for (k = 1; k <= (*pTesr).size[2]; k++)
        if ((*pTesr).VoxCell[i][j][k] == 0)
        {
          (*pTesr).hasvoid = 1;
          break;
        }

      if ((*pTesr).VoxCell[i][j][k] == 0)
        break;
    }

    if ((*pTesr).VoxCell[i][j][k] == 0)
      break;
  }

  return;
}

void
neut_tesr_rmsat (struct TESR *pTesr, char *rmsat, int verbosity)
{
  int status, dim;
  int voxqty = 0, **voxpos = NULL;

  status = sscanf (rmsat, "rmsat(%d)", &dim);

  if (status != 1)
    dim = (*pTesr).Dim - 1;

  neut_tesr_rmsat_cell_find (*pTesr, dim, &voxqty, &voxpos);

  if (voxqty > 0)
    neut_tesr_rmsat_cell_remove (pTesr, dim, voxqty, voxpos);

  if (verbosity)
    ut_print_message (0, verbosity, "%d voxel%s removed.\n", voxqty,
                      (voxqty > 1) ? "s" : "");

  ut_free_2d_int (&voxpos, voxqty);

  return;
}

void
neut_tesr_grow (struct TESR *pTesr, char *grow, int verbosity)
{
  int i, voxqty;

  (void) grow;

  // for (i = pow (3, (*pTesr).Dim) - 1; i >= 1; i--)
  for (i = 1; i >= 1; i--)
  {
    int iterqty;
    voxqty = neut_tesr_grow_neigh (pTesr, 0, i, INT_MAX, &iterqty);

    if (voxqty >= 0)
    {
      if (verbosity)
        ut_print_message (0, verbosity,
                          "%d voxel%s added in %d iterations.\n",
                          voxqty, (voxqty > 1) ? "s" : "", iterqty);
    }
    else
    {
      voxqty = 0;
      if (verbosity)
        ut_print_message (0, verbosity,
                          "%d voxel%s added in %d iterations.\n", voxqty,
                          (voxqty > 1) ? "s" : "", iterqty);
      break;
    }
  }

  neut_tesr_init_hasvoid (pTesr);

  return;
}

void
neut_tesr_tessinter (struct TESR *pTesr, char *crop, int verbosity)
{
  int qty = 0, i;
  struct TESS Tess;
  char *tess = ut_alloc_1d_char (strlen (crop) + 1);
  int ***found = ut_alloc_3d_int ((*pTesr).size[0] + 1,
                                  (*pTesr).size[1] + 1,
                                  (*pTesr).size[2] + 1);

  neut_tess_set_zero (&Tess);
  sscanf (crop, "tessinter(%s", tess);
  tess[strlen (tess) - 1] = '\0';
  neut_tess_fnscanf (tess, &Tess);

#pragma omp parallel for private(i)
  for (i = 1; i <= Tess.CellQty; i++)
  {
    int j, x, y, z;
    double **bbox = ut_alloc_2d (3, 2);
    double *coo = ut_alloc_1d (3);
    int *posmin = ut_alloc_1d_int (3);
    int *posmax = ut_alloc_1d_int (3);
    int *pos = ut_alloc_1d_int (3);

    neut_tess_cell_bbox (Tess, i, bbox);

    for (j = 0; j < 3; j++)
      coo[j] = bbox[j][0];
    neut_tesr_coo_pos (*pTesr, coo, -1, posmin);

    for (j = 0; j < 3; j++)
      coo[j] = bbox[j][1];
    neut_tesr_coo_pos (*pTesr, coo, 1, posmax);

    for (z = posmin[2]; z <= posmax[2]; z++)
      for (y = posmin[1]; y <= posmax[1]; y++)
        for (x = posmin[0]; x <= posmax[0]; x++)
          if ((*pTesr).VoxCell[x][y][z])
          {
            ut_array_1d_int_set_3 (pos, x, y, z);
            neut_tesr_pos_coo (*pTesr, pos, coo);

            if (neut_tess_point_incell (Tess, coo, i))
            {
              if (found[x][y][z])
                abort ();
              found[x][y][z] = i;
            }
          }

    ut_free_2d (&bbox, 3);
    ut_free_1d (&coo);
    ut_free_1d_int (&posmin);
    ut_free_1d_int (&posmax);
    ut_free_1d_int (&pos);
  }

  int z;

#pragma omp parallel for private(z)
  for (z = 1; z <= (*pTesr).size[2]; z++)
  {
    int x, y;
    for (y = 1; y <= (*pTesr).size[1]; y++)
      for (x = 1; x <= (*pTesr).size[0]; x++)
        if ((*pTesr).VoxCell[x][y][z] && !found[x][y][z])
        {
          (*pTesr).VoxCell[x][y][z] = 0;
          qty++;
        }
  }

  if (verbosity)
    ut_print_message (0, verbosity, "%d voxels removed.\n", qty);

  if ((*pTesr).hasvoid != -1)
    neut_tesr_init_hasvoid (pTesr);

  ut_free_1d_char (&tess);
  neut_tess_free (&Tess);
  ut_free_3d_int (&found, (*pTesr).size[0] + 1, (*pTesr).size[1] + 1);

  return;
}

void
neut_tesr_addbuffer_string (struct TESR *pTesr, char *string)
{
  int i, valqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  int *buff = ut_alloc_1d_int (6);

  ut_string_function (string, &fct, &vars, &vals, &valqty);

  if (valqty == 6)
    for (i = 0; i < 6; i++)
      buff[i] = atoi (vals[i]);

  else if (valqty == 3)
    for (i = 0; i < 3; i++)
    {
      buff[2 * i] = atoi (vals[i]);
      buff[2 * i + 1] = atoi (vals[i]);
    }
  else
    abort ();

  neut_tesr_addbuffer (pTesr, buff);

  ut_free_1d_int (&buff);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, valqty);
  ut_free_2d_char (&vals, valqty);

  return;
}

void
neut_tesr_addbuffer (struct TESR *pTesr, int *buff)
{
  int i, j, k, ii, jj, kk;
  int *sizecpy = ut_alloc_1d_int (3);

  // general

  for (i = 0; i < 3; i++)
    (*pTesr).Origin[i] -= buff[2 * i] * (*pTesr).vsize[i];

  if (ut_array_1d_int_sum (buff, 6) > 0)
    (*pTesr).hasvoid = 1;

  ut_array_1d_int_memcpy ((*pTesr).size, 3, sizecpy);

  for (i = 0; i < 3; i++)
    (*pTesr).size[i] += buff[2 * i] + buff[2 * i + 1];

  for (i = 1; i <= (*pTesr).CellQty; i++)
    for (j = 0; j < 3; j++)
      ut_array_1d_int_addval ((*pTesr).CellBBox[i][j], 2, buff[2 * j], (*pTesr).CellBBox[i][j]);

  // VoxCell

  if ((*pTesr).VoxCell)
  {
    int ***VoxCellCpy = ut_alloc_3d_int (sizecpy[0] + 2,
                                         sizecpy[1] + 2,
                                         sizecpy[2] + 2);

    ut_array_3d_int_memcpy ((*pTesr).VoxCell, sizecpy[0] + 2,
                            sizecpy[1] + 2, sizecpy[2] + 2,
                            VoxCellCpy);

    ut_free_3d_int (&(*pTesr).VoxCell, sizecpy[0] + 2,
                    sizecpy[1] + 2);

    (*pTesr).VoxCell =
      ut_alloc_3d_int ((*pTesr).size[0] + 2, (*pTesr).size[1] + 2, (*pTesr).size[2] + 2);

    for (k = 1; k <= sizecpy[2]; k++)
    {
      kk = k + buff[4];
      for (j = 1; j <= sizecpy[1]; j++)
      {
        jj = j + buff[2];
        for (i = 1; i <= sizecpy[0]; i++)
        {
          ii = i + buff[0];
          (*pTesr).VoxCell[ii][jj][kk] = VoxCellCpy[i][j][k];
        }
      }
    }

    ut_free_3d_int (&VoxCellCpy, sizecpy[0] + 2, sizecpy[1] + 2);
  }

  // VoxOri

  if ((*pTesr).VoxOri)
  {
    double ****VoxOriCpy = ut_alloc_4d (sizecpy[0] + 1,
                                        sizecpy[1] + 1,
                                        sizecpy[2] + 1, 4);

    ut_array_4d_memcpy ((*pTesr).VoxOri, sizecpy[0] + 1, sizecpy[1] + 1,
                        sizecpy[2] + 1, 4, VoxOriCpy);

    ut_free_4d (&(*pTesr).VoxOri, sizecpy[0] + 1, sizecpy[1] + 1, sizecpy[2] + 1);

    (*pTesr).VoxOri =
      ut_alloc_4d ((*pTesr).size[0] + 1, (*pTesr).size[1] + 1, (*pTesr).size[2] + 1, 4);

    for (k = 1; k <= sizecpy[2]; k++)
    {
      kk = k + buff[4];
      for (j = 1; j <= sizecpy[1]; j++)
      {
        jj = j + buff[2];
        for (i = 1; i <= sizecpy[0]; i++)
        {
          ii = i + buff[0];
          ut_array_1d_memcpy (VoxOriCpy[i][j][k], 4, (*pTesr).VoxOri[ii][jj][kk]);
        }
      }
    }

    ut_free_4d (&VoxOriCpy, sizecpy[0] + 1, sizecpy[1] + 1, sizecpy[2] + 1);
  }

  // VoxOriDef

  if ((*pTesr).VoxOriDef)
  {
    int ***VoxOriDefCpy = ut_alloc_3d_int (sizecpy[0] + 1,
                                           sizecpy[1] + 1,
                                           sizecpy[2] + 1);

    ut_array_3d_int_memcpy ((*pTesr).VoxOriDef, sizecpy[0] + 1, sizecpy[1] + 1,
                        sizecpy[2] + 1, VoxOriDefCpy);

    ut_free_3d_int (&(*pTesr).VoxOriDef, sizecpy[0] + 1, sizecpy[1] + 1);

    (*pTesr).VoxOriDef =
      ut_alloc_3d_int ((*pTesr).size[0] + 1, (*pTesr).size[1] + 1, (*pTesr).size[2] + 1);

    for (k = 1; k <= sizecpy[2]; k++)
    {
      kk = k + buff[4];
      for (j = 1; j <= sizecpy[1]; j++)
      {
        jj = j + buff[2];
        for (i = 1; i <= sizecpy[0]; i++)
        {
          ii = i + buff[0];
          (*pTesr).VoxOriDef[ii][jj][kk] = VoxOriDefCpy[i][j][k];
        }
      }
    }

    ut_free_3d_int (&VoxOriDefCpy, sizecpy[0] + 1, sizecpy[1] + 1);
  }

  ut_free_1d_int (&sizecpy);

  return;
}

void
neut_tesr_init_voxori (struct TESR *pTesr)
{
  int i, j, k;

  if (!(*pTesr).CellOri)
    return;

  (*pTesr).VoxOri =
    ut_alloc_4d ((*pTesr).size[0] + 1, (*pTesr).size[1] + 1,
                 (*pTesr).size[2] + 1, 4);

  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
        ut_array_1d_memcpy ((*pTesr).CellOri[(*pTesr).VoxCell[i][j][k]], 4,
                            (*pTesr).VoxOri[i][j][k]);

  return;
}

void
neut_tesr_oriaverage (struct TESR *pTesr)
{
  int i;

  if (!(*pTesr).CellOri)
    (*pTesr).CellOri = ut_alloc_2d ((*pTesr).CellQty + 1, 4);

  if (!(*pTesr).VoxOri)
    return;

  for (i = 1; i <= (*pTesr).CellQty; i++)
    neut_tesr_cell_oriaverage (pTesr, i);

  return;
}

void
neut_tesr_cell_oriaverage (struct TESR *pTesr, int cell)
{
  struct OL_SET OSet;

  if (!(*pTesr).VoxOri)
    return;

  neut_tesr_cell_olset (*pTesr, cell, &OSet);
  if (OSet.size > 0)
    ol_set_mean_iter (OSet, (*pTesr).CellOri[cell]);

  ol_set_free (OSet);

  return;
}

void
neut_tesr_unindex (struct TESR *pTesr)
{
  int i, j, k;

  if (!(*pTesr).VoxOriDef)
  {
    (*pTesr).VoxOriDef = ut_alloc_3d_int ((*pTesr).size[0] + 1,
                                       (*pTesr).size[1] + 1,
                                       (*pTesr).size[2] + 1);

    for (i = 1; i <= (*pTesr).size[0]; i++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (k = 1; k <= (*pTesr).size[2]; k++)
          if ((*pTesr).VoxCell[i][j][k] != 0)
            (*pTesr).VoxOriDef[i][j][k] = 1;
  }

  for (i = 1; i <= (*pTesr).size[0]; i++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (k = 1; k <= (*pTesr).size[2]; k++)
        if ((*pTesr).VoxCell[i][j][k] != 0)
        {
          if (ut_num_equal ((*pTesr).VoxOri[i][j][k][0], 1, 1e-6)
           || ut_num_equal ((*pTesr).VoxOri[i][j][k][0], -1, 1e-6))
          {
            (*pTesr).VoxCell[i][j][k] = 0;
            (*pTesr).VoxOriDef[i][j][k] = 0;
          }
        }

  return;
}

void
neut_tesr_cell_orirotate (struct TESR *pTesr, int cell, double *r, double theta, char *csys)
{
  int i, j, k;
  double *q = ol_q_alloc ();

  ol_rtheta_q (r, theta, q);

  if ((*pTesr).CellOri)
  {
    if (!csys || !strcmp (csys, "ref"))
      ol_q_q_q_ref ((*pTesr).CellOri[cell], q, (*pTesr).CellOri[cell]);
    else
      ol_q_q_q_cur ((*pTesr).CellOri[cell], q, (*pTesr).CellOri[cell]);
  }

  if ((*pTesr).VoxOri)
    for (k = (*pTesr).CellBBox[cell][2][0]; k <= (*pTesr).CellBBox[cell][2][1]; k++)
      for (j = (*pTesr).CellBBox[cell][1][0]; j <= (*pTesr).CellBBox[cell][1][1]; j++)
        for (i = (*pTesr).CellBBox[cell][0][0]; i <= (*pTesr).CellBBox[cell][0][1]; i++)
          if ((*pTesr).VoxCell[i][j][k] == cell)
          {
            if (!csys || !strcmp (csys, "ref"))
              ol_q_q_q_ref ((*pTesr).VoxOri[i][j][k], q, (*pTesr).VoxOri[i][j][k]);
            else
              ol_q_q_q_cur ((*pTesr).VoxOri[i][j][k], q, (*pTesr).VoxOri[i][j][k]);
          }

  ol_q_free (q);

  return;
}

int
neut_tesr_cellexpr_merge (struct TESR *pTesr, char *cellexpr)
{
  int i, j, k, l, mergecell, cell, cellqty, *cells = NULL;

  neut_tesr_expr_cells (pTesr, cellexpr, &cells, &cellqty);

  mergecell = cells[0];

  for (i = 1; i < cellqty; i++)
  {
    cell = cells[i];

    for (l = (*pTesr).CellBBox[cell][2][0]; l <= (*pTesr).CellBBox[cell][2][1]; l++)
      for (k = (*pTesr).CellBBox[cell][1][0]; k <= (*pTesr).CellBBox[cell][1][1]; k++)
        for (j = (*pTesr).CellBBox[cell][0][0]; j <= (*pTesr).CellBBox[cell][0][1]; j++)
          if ((*pTesr).VoxCell[j][k][l] == cell)
            (*pTesr).VoxCell[j][k][l] = mergecell;

    if ((*pTesr).CellOri)
      ol_q_set_zero ((*pTesr).CellOri[cell]);
    if ((*pTesr).CellId)
      (*pTesr).CellId[cell] = 0;
  }

  ut_free_1d_int (&cells);

  return cellqty;
}

void
neut_tesr_addcell (struct TESR *pTesr)
{
  (*pTesr).CellQty++;

  if ((*pTesr).CellId)
    (*pTesr).CellId = ut_realloc_1d_int ((*pTesr).CellId, (*pTesr).CellQty + 1);

  if ((*pTesr).CellOri)
  {
    (*pTesr).CellOri = ut_realloc_2d_addline ((*pTesr).CellOri, (*pTesr).CellQty + 1, 4);
    if ((*pTesr).CellQty == 1)
      (*pTesr).CellOri[0] = ut_alloc_1d (4);
  }

  if ((*pTesr).CellOriDistrib)
  {
    (*pTesr).CellOriDistrib = ut_realloc_1d_pchar ((*pTesr).CellOriDistrib, (*pTesr).CellQty + 1);
    if ((*pTesr).CellQty == 1)
      (*pTesr).CellOriDistrib[0] = NULL;
  }

  if ((*pTesr).CellBBox)
  {
    (*pTesr).CellBBox = ut_realloc_1d_ppint ((*pTesr).CellBBox, (*pTesr).CellQty + 1);
    (*pTesr).CellBBox[(*pTesr).CellQty] = ut_alloc_2d_int (3, 2);
    if ((*pTesr).CellQty == 1)
      (*pTesr).CellBBox[0] = ut_alloc_2d_int (3, 2);
  }

  if ((*pTesr).CellGroup)
    (*pTesr).CellGroup = ut_realloc_1d_int ((*pTesr).CellGroup, (*pTesr).CellQty + 1);

  if ((*pTesr).SeedCoo)
  {
    (*pTesr).SeedCoo = ut_realloc_2d_addline ((*pTesr).SeedCoo, (*pTesr).CellQty + 1, 3);
    if ((*pTesr).CellQty == 1)
      (*pTesr).SeedCoo[0] = NULL;
  }

  if ((*pTesr).SeedWeight)
    (*pTesr).SeedWeight = ut_realloc_1d ((*pTesr).SeedWeight, (*pTesr).CellQty + 1);

  return;
}

void
neut_tesr_resetorigin (struct TESR *pTesr)
{
  ut_array_1d_zero ((*pTesr).Origin, 3);

  return;
}

void
neut_tesr_resetcellid (struct TESR *pTesr)
{
  ut_free_1d_int (&(*pTesr).CellId);

  return;
}

int
neut_tesr_cellexpr_remove (struct TESR *pTesr, char *expr)
{
  int i, *cells = NULL, cellqty = 0;

  if ((*pTesr).Dim == 2)
    abort ();

  neut_tesr_expr_cells (pTesr, expr, &cells, &cellqty);

  if (!(*pTesr).CellId)
  {
    (*pTesr).CellId = ut_alloc_1d_int ((*pTesr).CellQty + 1);
    ut_array_1d_int_set_id ((*pTesr).CellId, (*pTesr).CellQty + 1);
  }

  for (i = 0; i < cellqty; i++)
    (*pTesr).CellId[cells[i]] = 0;

  if (cellqty)
    neut_tesr_renumber_continuous (pTesr);

  ut_free_1d_int (&cells);

  return cellqty;
}

void
neut_tesr_bbox_posbbox (struct TESR Tesr, double **bbox, int **posbbox)
{
  int i;
  double *coo = ut_alloc_1d (3);
  int *posmin = ut_alloc_1d_int (3);
  int *posmax = ut_alloc_1d_int (3);

  for (i = 0; i < 3; i++)
    coo[i] = bbox[i][0];
  neut_tesr_coo_pos (Tesr, coo, -1, posmin);

  for (i = 0; i < 3; i++)
    coo[i] = bbox[i][1];
  neut_tesr_coo_pos (Tesr, coo, 1, posmax);

  for (i = 0; i < 3; i++)
  {
    posbbox[i][0] = posmin[i];
    posbbox[i][1] = posmax[i];
  }

  ut_free_1d (&coo);
  ut_free_1d_int (&posmin);
  ut_free_1d_int (&posmax);

  return;
}
