/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_pf_.h"

void
neut_pf_set_zero (struct PF *pPf)
{
  (*pPf).space = NULL;
  (*pPf).shape = NULL;
  (*pPf).projection = NULL;
  (*pPf).refsym = NULL;
  (*pPf).font = NULL;
  (*pPf).mode = NULL;
  (*pPf).clustering = 0;

  (*pPf).pfdir = NULL;
  (*pPf).pfkernelsig = 0;

  (*pPf).pfpolelabel = NULL;
  (*pPf).pfprojlabel = NULL;
  (*pPf).pfpolestring = NULL;
  (*pPf).pfpoleqty = 0;
  (*pPf).pfpoles = NULL;

  (*pPf).inputqty = 0;
  (*pPf).inputs = NULL;
  (*pPf).inputcellqty = NULL;
  (*pPf).inputcellpole = NULL;

  (*pPf).ipfptqty = 0;
  (*pPf).ipfpts = NULL;

  (*pPf).crysym = NULL;

  (*pPf).gridsize = NULL;
  (*pPf).grid = NULL;
  (*pPf).Pgrid = NULL;
  (*pPf).lgrid = NULL;

  return;
}

void
neut_pf_free (struct PF *pPf)
{
  ut_free_1d_char (&(*pPf).space);
  ut_free_1d_char (&(*pPf).shape);
  ut_free_1d_char (&(*pPf).projection);
  ut_free_1d_char (&(*pPf).refsym);
  ut_free_1d_char (&(*pPf).font);
  ut_free_1d_char (&(*pPf).mode);

  ut_free_2d (&(*pPf).pfdir, 3);
  ut_free_2d (&(*pPf).pfpoles, (*pPf).pfpoleqty + 1);

  ut_free_2d (&(*pPf).ipfpts, (*pPf).ipfptqty);

  ut_free_1d_char (&(*pPf).crysym);

  if ((*pPf).gridsize)
  {
    ut_free_3d (&(*pPf).grid, (*pPf).gridsize[0], (*pPf).gridsize[1]);
    ut_free_3d (&(*pPf).Pgrid, (*pPf).gridsize[0], (*pPf).gridsize[1]);
    ut_free_3d (&(*pPf).lgrid, (*pPf).gridsize[0] + 2, (*pPf).gridsize[1] + 2);

    ut_free_1d_int (&(*pPf).gridsize);
  }

  return;
}

void
neut_pf_init_ipfborder (struct PF *pPf)
{
  int i;
  double dtheta, *r = ol_r_alloc ();
  double **g = ol_g_alloc ();
  double* v = ut_alloc_1d (3);

  if (!strcmp ((*pPf).crysym, "cubic"))
  {
    double* v110 = ut_alloc_1d (3);
    ut_array_1d_set_3 (v110, 0, OL_IS2, OL_IS2);

    ol_r_set_this (r, 0, OL_IS2, -OL_IS2);

    if (!(*pPf).ipfptqty)
      (*pPf).ipfptqty = 37;

    dtheta = 35.26439 / ((*pPf).ipfptqty - 2);
    ol_rtheta_g (r, dtheta, g);

    (*pPf).ipfpts = ut_alloc_2d ((*pPf).ipfptqty, 2);
    for (i = 0; i < (*pPf).ipfptqty - 1; i++)
    {
      ol_rtheta_g (r, i * dtheta, g);
      ol_g_vect_vect (g, v110, v);
      if (!strcmp ((*pPf).projection, "stereographic"))
        ol_vect_stprojxy (v, (*pPf).ipfpts[i + 1]);
      else if (! strcmp ((*pPf).projection, "equal-area"))
        ol_vect_eaprojxy (v, (*pPf).ipfpts[i + 1]);
      else
        abort ();
    }
    for (i = 0; i < (*pPf).ipfptqty; i++)
    {
      ut_array_1d_switch ((*pPf).ipfpts[i], 0, 1);
      (*pPf).ipfpts[i][1] *= -1;
    }

    ut_free_1d (&v110);
  }

  else if (!strcmp ((*pPf).crysym, "hexagonal"))
  {
    int *hpole = ut_alloc_1d_int (4);
    double* v2110 = ut_alloc_1d (3);
    ut_array_1d_int_set_4 (hpole, -1, 2, -1, 0);
    ol_hpole_vect (hpole, v2110);

    ol_r_set_this (r, 0, 0, -1);

    if (!(*pPf).ipfptqty)
      (*pPf).ipfptqty = 32;

    dtheta = 30. / ((*pPf).ipfptqty - 2);
    ol_rtheta_g (r, dtheta, g);

    (*pPf).ipfpts = ut_alloc_2d ((*pPf).ipfptqty, 2);
    for (i = 0; i < (*pPf).ipfptqty - 1; i++)
    {
      ol_rtheta_g (r, i * dtheta, g);
      ol_g_vect_vect (g, v2110, v);
      if (!strcmp ((*pPf).projection, "stereographic"))
        ol_vect_stprojxy (v, (*pPf).ipfpts[i + 1]);
      else if (! strcmp ((*pPf).projection, "equal-area"))
        ol_vect_eaprojxy (v, (*pPf).ipfpts[i + 1]);
      else
        abort ();
    }
    for (i = 0; i < (*pPf).ipfptqty; i++)
    {
      ut_array_1d_switch ((*pPf).ipfpts[i], 0, 1);
      (*pPf).ipfpts[i][1] *= -1;
    }

    ut_free_1d (&v2110);
    ut_free_1d_int (&hpole);
  }

  // expanding triangle for in-domain tests
  int j;
  double *delta = ut_alloc_1d (2);
  delta[0] = (*pPf).ipfpts[1][0] * 1e-3;
  delta[1] = (*pPf).ipfpts[(*pPf).ipfptqty - 1][1] * 1e-3;

  (*pPf).domainptqty = (*pPf).ipfptqty;
  (*pPf).domainpts = ut_alloc_2d ((*pPf).domainptqty, 2);
  ut_array_2d_memcpy ((*pPf).ipfpts, (*pPf).ipfptqty, 2, (*pPf).domainpts);
  ut_array_2d_scale ((*pPf).domainpts, (*pPf).domainptqty, 2, 1 + 1e-3);
  for (i = 0; i < (*pPf).domainptqty; i++)
    for (j = 0; j < 2; j++)
      (*pPf).domainpts[i][j] -= delta[j] / 2;

  ut_free_1d (&delta);

  ol_r_free (r);
  ol_g_free (g);
  ut_free_1d (&v);

  return;
}

void
neut_pf_init_grid (struct PF *pPf)
{
  int i, j;

  (*pPf).grid = ut_alloc_3d ((*pPf).gridsize[0], (*pPf).gridsize[1], 2);
  (*pPf).lgrid = ut_alloc_3d ((*pPf).gridsize[0] + 2, (*pPf).gridsize[1] + 2, 2);
  (*pPf).Pgrid = ut_alloc_3d ((*pPf).gridsize[0], (*pPf).gridsize[1], 3);

  if (!strcmp ((*pPf).space, "pf"))
  {
    if (!strcmp ((*pPf).shape, "full"))
    {
      for (i = 0; i < (*pPf).gridsize[0]; i++)
        for (j = 0; j < (*pPf).gridsize[1]; j++)
        {
          (*pPf).grid[i][j][0] = 2 * (i + 0.5) / (*pPf).gridsize[0] - 1;
          (*pPf).grid[i][j][1] = 2 * (j + 0.5) / (*pPf).gridsize[1] - 1;
          if (ut_array_1d_norm ((*pPf).grid[i][j], 2) <= 1)
          {
            if (!strcmp ((*pPf).projection, "stereographic"))
              ol_stprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
            else if (!strcmp ((*pPf).projection, "equal-area"))
              ol_eaprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
            else
              abort();
          }
        }

      for (i = 0; i < (*pPf).gridsize[0] + 2; i++)
        for (j = 0; j < (*pPf).gridsize[1] + 2; j++)
        {
          (*pPf).lgrid[i][j][0] = 2 * (i - 0.5) / (*pPf).gridsize[0] - 1;
          (*pPf).lgrid[i][j][1] = 2 * (j - 0.5) / (*pPf).gridsize[1] - 1;
        }
    }

    else if (!strcmp ((*pPf).shape, "quarter"))
    {
      for (i = 0; i < (*pPf).gridsize[0]; i++)
        for (j = 0; j < (*pPf).gridsize[1]; j++)
        {
          (*pPf).grid[i][j][0] = (i + 0.5) / (*pPf).gridsize[0];
          (*pPf).grid[i][j][1] = (j + 0.5) / (*pPf).gridsize[1];
          if (ut_array_1d_norm ((*pPf).grid[i][j], 2) <= 1)
          {
            if (!strcmp ((*pPf).projection, "stereographic"))
              ol_stprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
            else if (!strcmp ((*pPf).projection, "equal-area"))
              ol_eaprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
            else
              abort();
          }
        }

      for (i = 0; i < (*pPf).gridsize[0] + 2; i++)
        for (j = 0; j < (*pPf).gridsize[1] + 2; j++)
        {
          (*pPf).lgrid[i][j][0] = (i - 0.5) / (*pPf).gridsize[0];
          (*pPf).lgrid[i][j][1] = (j - 0.5) / (*pPf).gridsize[1];
        }
    }
  }

  else if (!strcmp ((*pPf).space, "ipf"))
  {
    for (i = 0; i < (*pPf).gridsize[0]; i++)
      for (j = 0; j < (*pPf).gridsize[1]; j++)
      {
	(*pPf).grid[i][j][0] = (*pPf).ipfpts[1][0] * (i + 0.5) / (*pPf).gridsize[0];
	(*pPf).grid[i][j][1] = (*pPf).ipfpts[(*pPf).ipfptqty - 1][1] * (j + 0.5) / (*pPf).gridsize[1];
	if (!strcmp ((*pPf).projection, "stereographic"))
	  ol_stprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
	else if (!strcmp ((*pPf).projection, "equal-area"))
	    ol_eaprojxy_vect ((*pPf).grid[i][j], (*pPf).Pgrid[i][j]);
	else
	  abort();
      }

    for (i = 0; i < (*pPf).gridsize[0] + 2; i++)
      for (j = 0; j < (*pPf).gridsize[1] + 2; j++)
      {
	(*pPf).lgrid[i][j][0] = (*pPf).ipfpts[1][0] * (i - 0.5) / (*pPf).gridsize[0];
	(*pPf).lgrid[i][j][1] = (*pPf).ipfpts[(*pPf).ipfptqty - 1][1] * (j - 0.5) / (*pPf).gridsize[1];
      }
  }

  else
    abort ();

  return;
}

void
neut_pf_point_gridpos (struct PF Pf, double *coo, int *pos)
{
  int i;

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      for (i = 0; i < 2; i++)
        pos[i] = ut_num_d2ri (0.5 * ((coo[i] + 1) * Pf.gridsize[i] - 1));
    }

    // may return negative values since the grid should be larger than the reduced pf
    else if (!strcmp (Pf.shape, "quarter"))
    {
      for (i = 0; i < 2; i++)
        pos[i] = ut_num_d2ri (coo[i] * Pf.gridsize[i] - 0.5);
    }
  }

  // may return negative values since the grid should be larger than the std triangle
  else if (!strcmp (Pf.space, "ipf"))
  {
    pos[0] = ut_num_d2ri (coo[0] * Pf.gridsize[0] / Pf.ipfpts[1][0] - 0.5);
    pos[1] = ut_num_d2ri (coo[1] * Pf.gridsize[1] / Pf.ipfpts[Pf.ipfptqty - 1][1] - 0.5);
  }

  else
    abort ();

  return;
}
