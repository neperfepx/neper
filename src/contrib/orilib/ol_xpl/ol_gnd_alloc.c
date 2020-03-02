/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_gnd_alloc.h"

struct OL_CRYS
ol_crys_alloc ()
{
  struct OL_CRYS Crys;

  Crys.structure = ut_alloc_1d_char (10);
  Crys.a = 0;
  Crys.nu = 0;

  Crys.rho_min = ut_alloc_1d_char (10);
  sprintf (Crys.rho_min, "sc");

  Crys.gnd_size = 0;
  Crys.gnd_s = NULL;
  Crys.gnd_t = NULL;

  return Crys;
}

void
ol_crys_free (struct OL_CRYS Crys)
{
  if (Crys.gnd_s != NULL)
    if (!strcmp (Crys.structure, "fcc"))
    {
      ut_free_2d (&Crys.gnd_s, 18);
      ut_free_2d (&Crys.gnd_t, 18);
    }

  ut_free_1d_char (&Crys.structure);
  ut_free_1d_char (&Crys.rho_min);

  return;
}

void
ol_crys_set_this (struct OL_CRYS *pCrys, char *structure, double a, double nu,
                  char *rho_min)
{
  (*pCrys).structure = ut_alloc_1d_char (strlen (structure) + 1);
  strcpy ((*pCrys).structure, structure);

  (*pCrys).a = a;
  (*pCrys).nu = nu;

  (*pCrys).rho_min = ut_alloc_1d_char (strlen (rho_min) + 1);
  strcpy ((*pCrys).rho_min, rho_min);

  return;
}

int
ol_crys_set_mat (struct OL_CRYS *pCrys, char *mat)
{
  int status;

  double **fcc_s = ut_alloc_2d (12, 3);
  double **fcc_m = ut_alloc_2d (12, 3);
  double **fcc_t = ut_alloc_2d (12, 3);
  double **fcc_s_gnd = ut_alloc_2d (18, 3);
  double **fcc_t_gnd = ut_alloc_2d (18, 3);

  fcc_m[0][0] = 1;
  fcc_m[0][1] = 1;
  fcc_m[0][2] = 1;
  fcc_m[1][0] = 1;
  fcc_m[1][1] = 1;
  fcc_m[1][2] = 1;
  fcc_m[2][0] = 1;
  fcc_m[2][1] = 1;
  fcc_m[2][2] = 1;
  fcc_m[3][0] = 1;
  fcc_m[3][1] = 1;
  fcc_m[3][2] = -1;
  fcc_m[4][0] = 1;
  fcc_m[4][1] = 1;
  fcc_m[4][2] = -1;
  fcc_m[5][0] = 1;
  fcc_m[5][1] = 1;
  fcc_m[5][2] = -1;
  fcc_m[6][0] = 1;
  fcc_m[6][1] = -1;
  fcc_m[6][2] = 1;
  fcc_m[7][0] = 1;
  fcc_m[7][1] = -1;
  fcc_m[7][2] = 1;
  fcc_m[8][0] = 1;
  fcc_m[8][1] = -1;
  fcc_m[8][2] = 1;
  fcc_m[9][0] = 1;
  fcc_m[9][1] = -1;
  fcc_m[9][2] = -1;
  fcc_m[10][0] = 1;
  fcc_m[10][1] = -1;
  fcc_m[10][2] = -1;
  fcc_m[11][0] = 1;
  fcc_m[11][1] = -1;
  fcc_m[11][2] = -1;
  ut_array_2d_scale (fcc_m, 12, 3, OL_IS3);

  fcc_s[0][0] = 0;
  fcc_s[0][1] = 1;
  fcc_s[0][2] = -1;
  fcc_s[1][0] = 1;
  fcc_s[1][1] = 0;
  fcc_s[1][2] = -1;
  fcc_s[2][0] = 1;
  fcc_s[2][1] = -1;
  fcc_s[2][2] = 0;
  fcc_s[3][0] = 0;
  fcc_s[3][1] = 1;
  fcc_s[3][2] = 1;
  fcc_s[4][0] = 1;
  fcc_s[4][1] = 0;
  fcc_s[4][2] = 1;
  fcc_s[5][0] = 1;
  fcc_s[5][1] = -1;
  fcc_s[5][2] = 0;
  fcc_s[6][0] = 0;
  fcc_s[6][1] = 1;
  fcc_s[6][2] = 1;
  fcc_s[7][0] = 1;
  fcc_s[7][1] = 0;
  fcc_s[7][2] = -1;
  fcc_s[8][0] = 1;
  fcc_s[8][1] = 1;
  fcc_s[8][2] = 0;
  fcc_s[9][0] = 0;
  fcc_s[9][1] = 1;
  fcc_s[9][2] = -1;
  fcc_s[10][0] = 1;
  fcc_s[10][1] = 0;
  fcc_s[10][2] = 1;
  fcc_s[11][0] = 1;
  fcc_s[11][1] = 1;
  fcc_s[11][2] = 0;
  ut_array_2d_scale (fcc_s, 12, 3, OL_IS2);

  fcc_t[0][0] = -2;
  fcc_t[0][1] = 1;
  fcc_t[0][2] = 1;
  fcc_t[1][0] = -1;
  fcc_t[1][1] = 2;
  fcc_t[1][2] = -1;
  fcc_t[2][0] = 1;
  fcc_t[2][1] = 1;
  fcc_t[2][2] = -2;
  fcc_t[3][0] = 2;
  fcc_t[3][1] = -1;
  fcc_t[3][2] = 1;
  fcc_t[4][0] = 1;
  fcc_t[4][1] = -2;
  fcc_t[4][2] = -1;
  fcc_t[5][0] = -1;
  fcc_t[5][1] = -1;
  fcc_t[5][2] = -2;
  fcc_t[6][0] = -2;
  fcc_t[6][1] = -1;
  fcc_t[6][2] = 1;
  fcc_t[7][0] = 1;
  fcc_t[7][1] = 2;
  fcc_t[7][2] = 1;
  fcc_t[8][0] = -1;
  fcc_t[8][1] = 1;
  fcc_t[8][2] = 2;
  fcc_t[9][0] = 2;
  fcc_t[9][1] = 1;
  fcc_t[9][2] = 1;
  fcc_t[10][0] = -1;
  fcc_t[10][1] = -2;
  fcc_t[10][2] = 1;
  fcc_t[11][0] = 1;
  fcc_t[11][1] = -1;
  fcc_t[11][2] = 2;
  ut_array_2d_scale (fcc_t, 12, 3, OL_IS6);

  ut_array_2d_memcpy (fcc_s, 12, 3, fcc_s_gnd);
  fcc_s_gnd[12][0] = 1;
  fcc_s_gnd[12][1] = 1;
  fcc_s_gnd[12][2] = 0;
  fcc_s_gnd[13][0] = 1;
  fcc_s_gnd[13][1] = 0;
  fcc_s_gnd[13][2] = 1;
  fcc_s_gnd[14][0] = 0;
  fcc_s_gnd[14][1] = 1;
  fcc_s_gnd[14][2] = 1;
  fcc_s_gnd[15][0] = -1;
  fcc_s_gnd[15][1] = 1;
  fcc_s_gnd[15][2] = 0;
  fcc_s_gnd[16][0] = 1;
  fcc_s_gnd[16][1] = 0;
  fcc_s_gnd[16][2] = -1;
  fcc_s_gnd[17][0] = 0;
  fcc_s_gnd[17][1] = -1;
  fcc_s_gnd[17][2] = 1;
  ut_array_2d_scale (fcc_s_gnd + 12, 6, 3, OL_IS2);

  ut_array_2d_memcpy (fcc_t, 12, 3, fcc_t_gnd);
  ut_array_2d_memcpy (fcc_s_gnd, 6, 3, fcc_t_gnd + 12);

  status = 0;
  if (!strcmp (mat, "Al"))
    ol_crys_set_this (pCrys, "fcc", 0.405 * 1e-9, 0.3, "L2");
  else if (!strcmp (mat, "Ni"))
    ol_crys_set_this (pCrys, "fcc", 0.352 * 1e-9, 0.3, "L2");
  else if (!strcmp (mat, "Cu"))
    ol_crys_set_this (pCrys, "fcc", 0.361 * 1e-9, 0.3, "L2");
  else if (!strcmp (mat, "Fefcc"))
    ol_crys_set_this (pCrys, "fcc", 0.3515 * 1e-9, 0.3, "L2");
  else
    status = -1;

  if (!strcmp ((*pCrys).structure, "fcc"))
  {
    (*pCrys).N = 12;
    (*pCrys).s = ut_alloc_2d ((*pCrys).N, 3);
    (*pCrys).m = ut_alloc_2d ((*pCrys).N, 3);
    (*pCrys).t = ut_alloc_2d ((*pCrys).N, 3);

    ut_array_2d_memcpy (fcc_s, 12, 3, (*pCrys).s);
    ut_array_2d_memcpy (fcc_m, 12, 3, (*pCrys).m);
    ut_array_2d_memcpy (fcc_t, 12, 3, (*pCrys).t);

    (*pCrys).gnd_size = 18;
    (*pCrys).gnd_s = ut_alloc_2d ((*pCrys).gnd_size, 3);
    (*pCrys).gnd_t = ut_alloc_2d ((*pCrys).gnd_size, 3);

    ut_array_2d_memcpy (fcc_s_gnd, 18, 3, (*pCrys).gnd_s);
    ut_array_2d_memcpy (fcc_t_gnd, 18, 3, (*pCrys).gnd_t);
  }
  else
    abort ();

  ut_free_2d (&fcc_s, 12);
  ut_free_2d (&fcc_m, 12);
  ut_free_2d (&fcc_t, 12);
  ut_free_2d (&fcc_s_gnd, 12);
  ut_free_2d (&fcc_t_gnd, 12);

  return status;
}

struct OL_GNDMAP
ol_gndmap_alloc (struct OL_MAP Map, struct OL_CRYS Crys)
{
  struct OL_GNDMAP Gnd;

  Gnd.xsize = Map.xsize;
  Gnd.ysize = Map.ysize;
  Gnd.stepsize = Map.stepsize;

  Gnd.id = ut_alloc_2d_uint (Map.xsize, Map.ysize);
  Gnd.nye = ut_alloc_4d (Map.xsize, Map.ysize, 3, 3);

  int rho_size;

  if (!strcmp (Crys.structure, "fcc"))
    rho_size = 18;
  else
    abort ();

  Gnd.rho = ut_alloc_3d (Map.xsize, Map.ysize, rho_size);
  Gnd.rhosum = ut_alloc_2d (Map.xsize, Map.ysize);

  return Gnd;
}

void
ol_gndmap_free (struct OL_GNDMAP Gnd)
{
  ut_free_2d_uint (&Gnd.id, Gnd.xsize);
  ut_free_4d (&Gnd.nye, Gnd.xsize, Gnd.ysize, 3);
  ut_free_3d (&Gnd.rho, Gnd.xsize, Gnd.ysize);
  ut_free_2d (&Gnd.rhosum, Gnd.xsize);

  Gnd.xsize = 0;
  Gnd.ysize = 0;
  Gnd.stepsize = 0;

  return;
}

double **
ol_nye_alloc ()
{
  return ut_alloc_2d (3, 3);
}

void
ol_nye_free (double **nye)
{
  if (!nye)
    return;
  else
    ut_free_2d (&nye, 3);

  return;
}

void
ol_crys_ss_schmid (struct OL_CRYS Crys, double **g, double ***T)
{
  unsigned int i, j, k;
  double **ginv = ol_g_alloc ();

  ol_g_inverse (g, ginv);

  for (i = 0; i < Crys.N; i++)
  {
    ut_array_2d_zero (T[i], 3, 3);

    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
        T[i][j][k] = Crys.s[i][j] * Crys.m[i][k];

    ol_g_csys (T[i], ginv, T[i]);
  }

  ol_g_free (ginv);

  return;
}

void
ol_crys_ss_schmids (struct OL_CRYS Crys, double **g, double ***T)
{
  unsigned int i;

  ol_crys_ss_schmid (Crys, g, T);

  for (i = 0; i < Crys.N; i++)
    ut_mat_sym (T[i], 3, T[i]);

  return;
}
