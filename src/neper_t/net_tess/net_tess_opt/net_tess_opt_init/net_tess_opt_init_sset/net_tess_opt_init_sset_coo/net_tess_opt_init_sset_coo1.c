/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_coo_.h"

void
net_tess_opt_init_sset_coo (struct MTESS MTess, struct TESS *Tess, int dtess,
                            int dcell, struct TOPT *pTOpt, char *var, int pos,
                            char *cooexpr, double *rad, int *qty)
{
  int i, j, cell, from_file = 0;
  double *centre = ut_alloc_1d (3);
  struct POINT Point, Point2;
  char *prev = ut_alloc_1d_char (1000);
  double **coo = ut_alloc_2d ((*pTOpt).CellQty + 1, 3);
  char *mid = NULL;
  FILE *fp = NULL;
  struct TESS Tmp;

  neut_point_set_zero (&Point);
  neut_point_set_zero (&Point2);

  // loading all coos and using them one by one afterwards
  if (ut_string_isfilename (cooexpr))
  {
    from_file = 1;
    neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &mid);
    if (ut_file_testformat (cooexpr, "tess"))
    {
      neut_tess_set_zero (&Tmp);
      fp = ut_file_open (cooexpr, "R");
      neut_tess_fscanf (fp, &Tmp);
      ut_array_2d_memcpy (Tmp.SeedCoo + 1, (*pTOpt).CellQty, (*pTOpt).Dim,
                          coo + 1);
      ut_file_close (fp, cooexpr, "R");
      neut_tess_free (&Tmp);
    }
    else
      net_multiscale_arg_2d_fscanf (cooexpr, mid, coo + 1, (*pTOpt).CellQty,
                                    (*pTOpt).Dim);

    if ((*pTOpt).aspratio)
      for (i = 1; i <= (*pTOpt).CellQty; i++)
        for (j = 0; j < (*pTOpt).Dim; j++)
          coo[i][j] /= (*pTOpt).aspratio[j];
  }

  int *id = ut_alloc_1d_int ((*pTOpt).CellQty + 1);
  int multiseed = (ut_array_1d_int_max (qty + 1, (*pTOpt).CellQty) > 1);

  Point.Dim = (*pTOpt).SSet.Dim;

  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng *r2 = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, (*pTOpt).SSet.Random);
  gsl_rng_set (r2, 1);

  if (!strcmp (cooexpr, "packing"))
  {
    ut_array_1d_sort_des_index (rad + 1, (*pTOpt).CellQty, id + 1);
    ut_array_1d_int_addval (id + 1, (*pTOpt).CellQty, 1, id + 1);
  }
  else
    ut_array_1d_int_set_id (id, (*pTOpt).CellQty + 1);

  (*pTOpt).SSet.N = 0;
  for (i = 1; i <= (*pTOpt).CellQty; i++)
  {
    cell = id[i];

    if ((*pTOpt).tarqty > 0)
      ut_print_progress (stdout, i, (*pTOpt).CellQty, "%.0f%%", prev);

    net_tess_opt_init_sset_coo_cluster ((*pTOpt).Dim, (*pTOpt).activedim, r2,
                                        qty[cell], 0.5 * rad[cell], rad[cell],
                                        &Point2);

    if (from_file || !strncmp (cooexpr, "bcc", 3))
      ut_array_1d_memcpy (coo[cell], 3, centre);
    else
      net_tess_opt_init_sset_coo_centre (pTOpt, r, var, pos, cooexpr, Point,
                                         Point2, cell, centre);

    net_tess_opt_init_sset_coo_record (pTOpt, cell, &Point, centre, &Point2);
  }

  if (!strcmp (cooexpr, "LLLFP2011"))
  {
    if (multiseed)
      ut_print_message (2, 3,
                        "LLFP2011 method not available with multiseeding.\n");

    net_tess_opt_init_sset_coo_lllfp2011 (pTOpt);
  }

  if ((*pTOpt).tarqty == 0)
    printf ("\n");

  ut_free_1d (&centre);
  ut_free_2d (&coo, (*pTOpt).CellQty + 1);
  ut_free_1d_char (&prev);
  neut_point_free (&Point);
  neut_point_free (&Point2);
  gsl_rng_free (r);
  gsl_rng_free (r2);
  ut_free_1d_char (&mid);
  ut_free_1d_int (&id);

  return;
}
