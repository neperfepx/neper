/* This id is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf_file_data_ori (double **q, double *weights, int qty, int *show,
                            struct PRINT Print,
                            double ***ppts, double **pptwgts, int **pcell,
                            int *pptqty)
{
  int i, j;
  struct OL_SET Set;
  double** uvect = NULL;
  double *qref = ol_q_alloc ();

  ol_g_q (Print.pfdir, qref);

  uvect = ut_alloc_2d (Print.pfpoleqty, 3);
  for (i = 0; i < Print.pfpoleqty; i++)
    ol_pole_vect (Print.pfpoles[i], uvect[i]);

  Set = ol_set_alloc (qty, NULL);

  for (i = 0; i < qty; i++)
  {
    ol_q_refcsys (q[i], qref, Set.q[i]);
    Set.weight[i] = weights ? weights[i] : 1;
  }

  /*
  else if (! strcmp (InPF.SamplingType, "rel"))
    ol_set_fscanf_sample (in, InPF.Sampling[id], &Set, NULL);
  else if (! strcmp (InPF.SamplingType, "abs"))
    ol_set_fscanf_sample_nb (in, InPF.Sampling[id], &Set, NULL);
  else
    abort ();
  */

  (*ppts) = ut_alloc_2d (qty * Print.pfpoleqty, 2);
  (*pcell) = ut_alloc_1d_int (qty * Print.pfpoleqty);
  (*pptwgts) = ut_alloc_1d (qty * Print.pfpoleqty);

  (*pptqty) = 0;
  for (i = 0; i < (int) Set.size; i++)
    if (!show || show[i])
    {
      for (j = 0; j < Print.pfpoleqty; j++)
      {
	if (!strcmp (Print.space, "ipf"))
	{
	  if (! strcmp (Print.pfprojection, "stereographic"))
	    ol_q_ipf_stprojxy (Set.q[i], uvect[j], (*ppts)[(*pptqty)]);
	  else if (! strcmp (Print.pfprojection, "equal-area"))
	    ol_q_ipf_eaprojxy (Set.q[i], uvect[j], (*ppts)[(*pptqty)]);
	  else
	    abort();
	}

        else
	{
	  if (! strcmp (Print.pfprojection, "stereographic"))
	    ol_q_pf_stprojxy (Set.q[i], uvect[j], (*ppts)[(*pptqty)]);
	  else if (! strcmp (Print.pfprojection, "equal-area"))
	    ol_q_pf_eaprojxy (Set.q[i], uvect[j], (*ppts)[(*pptqty)]);
	  else
	    abort();
	}

        (*pcell)[(*pptqty)] = i + 1;
	(*pptwgts)[(*pptqty)] = Set.weight[i];
	(*pptqty)++;
      }
    }

  ut_free_2d (&uvect, Print.pfpoleqty);
  ol_q_free (qref);

  return;
}

void
nev_print_pf_file_data_vect (double **data, double *weights, int qty, int *show,
                             struct PRINT Print,
                             double ***ppts, double **pptwgts, int **pcell,
                             int *pptqty)
{
  int i;
  double *vect = ut_alloc_1d (3);

  (*ppts) = ut_alloc_2d (qty, 2);
  (*pcell) = ut_alloc_1d_int (qty);
  (*pptwgts) = ut_alloc_1d (qty);

  (*pptqty) = 0;
  for (i = 0; i < qty; i++)
    if (!show || show[i])
    {
      ol_g_vect_vect (Print.pfdir, data[i], vect);
      ut_array_1d_scale (vect, 3, 1. / ut_array_1d_norm (vect, 3));

      if (! strcmp (Print.pfprojection, "stereographic"))
        ol_vect_stprojxy (vect, (*ppts)[(*pptqty)]);
      else if (! strcmp (Print.pfprojection, "equal-area"))
        ol_vect_eaprojxy (vect, (*ppts)[(*pptqty)]);
      else
        abort();

      (*pcell)[(*pptqty)] = i + 1;
      (*pptwgts)[(*pptqty)] = weights ? weights[i] : 1;
      (*pptqty)++;
    }

  ut_free_1d (&vect);

  return;
}

void
nev_print_pf_file_data_orthotropic (double ***ppts, double **pptwgts,
                                    int **pptcells, int *pptqty)
{
  int i, qty = *pptqty;

  (*ppts) = ut_realloc_1d_pdouble (*ppts, 4 * qty);
  for (i = qty; i < 2 * qty; i++)
  {
    (*ppts)[i] = ut_alloc_1d (3);
    (*ppts)[i][0] = (*ppts)[i - qty][0];
    (*ppts)[i][1] = -(*ppts)[i - qty][1];
  }
  for (i = 2 * qty; i < 3 * qty; i++)
  {
    (*ppts)[i] = ut_alloc_1d (3);
    (*ppts)[i][0] = -(*ppts)[i - 2 * qty][0];
    (*ppts)[i][1] = -(*ppts)[i - 2 * qty][1];
  }
  for (i = 3 * qty; i < 4 * qty; i++)
  {
    (*ppts)[i] = ut_alloc_1d (3);
    (*ppts)[i][0] = -(*ppts)[i - 3 * qty][0];
    (*ppts)[i][1] =  (*ppts)[i - 3 * qty][1];
  }

  (*pptwgts) = ut_realloc_1d ((*pptwgts), 4 * qty);
  ut_array_1d_memcpy (*pptwgts, qty, (*pptwgts) + qty);
  ut_array_1d_memcpy (*pptwgts, qty, (*pptwgts) + 2 * qty);
  ut_array_1d_memcpy (*pptwgts, qty, (*pptwgts) + 3 * qty);

  (*pptcells) = ut_realloc_1d_int (*pptcells, 4 * qty);
  ut_array_1d_int_memcpy (*pptcells, qty, (*pptcells) + qty);
  ut_array_1d_int_memcpy (*pptcells, qty, (*pptcells) + 2 * qty);
  ut_array_1d_int_memcpy (*pptcells, qty, (*pptcells) + 3 * qty);

  (*pptqty) *= 4;

  return;
}

void
nev_print_pf_file_data_crysym (struct PRINT Print, double ***ppts, double **pptwgts,
                           int *pptqty)
{
  int i, j;
  int qty = *pptqty;
  double** I = ol_g_alloc ();
  double** gs = ol_g_alloc ();
  double* v0 = ol_r_alloc ();
  double* v = ol_r_alloc ();

  (*ppts) = ut_realloc_1d_pdouble ((*ppts), 24 * qty);
  (*pptwgts) = ut_realloc_1d ((*pptwgts), 24 * qty);
  for (i = 2; i <= 24; i++)
  {
    ol_g_crysym (I, "cubic", i, gs);

    for (j = 0; j < qty; j++)
    {
      (*ppts)[*pptqty] = ut_alloc_1d (3);

      if (!strcmp (Print.pfprojection, "stereographic"))
	ol_stprojxy_vect ((*ppts)[j], v0);
      else if (!strcmp (Print.pfprojection, "equal-area"))
	ol_eaprojxy_vect ((*ppts)[j], v0);

      ol_g_vect_vect (gs, v0, v);

      if (!strcmp (Print.pfprojection, "stereographic"))
	ol_vect_stprojxy (v, (*ppts)[*pptqty]);
      else if (!strcmp (Print.pfprojection, "equal-area"))
	ol_vect_eaprojxy (v, (*ppts)[*pptqty]);

      (*pptwgts)[*pptqty] = (*pptwgts)[j];
      (*pptqty)++;
    }
  }

  ol_g_free (I);
  ol_g_free (gs);
  ol_r_free (v0);
  ol_r_free (v);

  return;
}

void
nev_print_pf_file_data_clustering (struct PRINT Print, double ***ppts,
                                   double **pptwgts, int **pptcells, int *pptqty)
{
  int i, j, k, x_id, y_id, id, qty;
  double **grid_p = NULL;
  double  *grid_w = NULL;
  int  *grid_cell = NULL;
  double** tmp = NULL;
  double* tmp2 = NULL;
  int* tmp3 = NULL;

  ut_print_message (0, 2, "Reducing data...\n");

  qty = (*pptqty);
  tmp = ut_alloc_2d (*pptqty, 2);
  ut_array_2d_memcpy (*ppts, *pptqty, 2, tmp);
  tmp2 = ut_alloc_1d (*pptqty);
  ut_array_1d_memcpy (*pptwgts, *pptqty, tmp2);
  tmp3 = ut_alloc_1d_int (*pptqty);
  ut_array_1d_int_memcpy (*pptcells, *pptqty, tmp3);

  grid_p = ut_alloc_2d (Print.pfgridsize * Print.pfgridsize, 2);
  grid_w = ut_alloc_1d (Print.pfgridsize * Print.pfgridsize);
  grid_cell = ut_alloc_1d_int (Print.pfgridsize * Print.pfgridsize);

  id = 0;
  for (j = 0; j < Print.pfgridsize; j++)
    for (i = 0; i < Print.pfgridsize; i++)
    {
      grid_p[id][0] = 2 * (double) i / (double) (Print.pfgridsize - 1) - 1;
      grid_p[id][1] = 2 * (double) j / (double) (Print.pfgridsize - 1) - 1;
      id++;
    }

  for (k = 0; k < qty; k++)
  {
    x_id = ut_num_d2ri ((tmp[k][0] + 1) * (Print.pfgridsize - 1) / 2.);
    y_id = ut_num_d2ri ((tmp[k][1] + 1) * (Print.pfgridsize - 1) / 2.);
    id   = y_id * Print.pfgridsize + x_id;
    grid_w[id] += tmp2[k];
    grid_cell[id] = tmp3[k];
  }

  ut_free_2d (ppts, *pptqty);
  (*ppts) = ut_alloc_2d (1, 2);
  (*pptqty) = 0;
  (*pptwgts) = ut_realloc_1d (*pptwgts, 1);
  (*pptcells) = ut_realloc_1d_int (*pptcells, 1);
  for (i = 0; i < Print.pfgridsize * Print.pfgridsize; i++)
    if (grid_w[i] > 1e-12)
    {
      (*pptqty)++;
      (*ppts) = ut_realloc_2d_addline ((*ppts), (*pptqty), 2);
      ut_array_1d_memcpy (grid_p[i], 2, (*ppts)[(*pptqty) - 1]);
      (*pptwgts) = ut_realloc_1d ((*pptwgts), (*pptqty));
      (*pptwgts)[(*pptqty) - 1] = grid_w[i];
      (*pptcells) = ut_realloc_1d_int ((*pptcells), (*pptqty));
      (*pptcells)[(*pptqty) - 1] = grid_cell[i];
    }

  ut_print_message (0, 3, "Number of data reduced by %.1f% (to %d)\n", 100 *
      (double) (qty - (*pptqty)) / qty, !strcmp (Print.pfsym, "monoclinic") ? (*pptqty) : (*pptqty) / 4);

  ut_free_2d (&grid_p, Print.pfgridsize * Print.pfgridsize);
  ut_free_1d (&grid_w);
  ut_free_1d_int (&grid_cell);
  ut_free_2d (&tmp, *pptqty);
  ut_free_1d (&tmp2);
  ut_free_1d_int (&tmp3);

  return;
}

void
nev_print_pf_ptsprint_symbols (FILE *file, struct PRINT Print, struct DATA Data,
                  double **pipfpts, int ipfptqty, double **pts, int *ptcells,
                  int ptqty)
{
  int i, plot, trs_disable;
  double* v = ut_alloc_1d (3);
  double* p110 = ol_p_alloc ();

  ut_array_1d_set_3 (v, 0, OL_IS2, OL_IS2);
  if (! strcmp (Print.pfprojection, "stereographic"))
    ol_vect_stprojxy (v, p110);
  else if (! strcmp (Print.pfprojection, "equal-area"))
    ol_vect_eaprojxy (v, p110);

  trs_disable = 0;

  for (i = 0; i < ptqty; i++)
  {
    plot = 0;
    if (!strcmp (Print.pfshape, "full"))
      plot = 1;
    else if (!strcmp (Print.pfshape, "quarter"))
    {
      if (pts[i][0] >= 0 && pts[i][1] <= 0)
	plot = 1;
    }
    else if (!strcmp (Print.space, "ipf"))
      plot = ut_space_point_indomain_2d (pts[i], pipfpts, ipfptqty);
    else
      abort ();

    if (plot)
    {
      char *symbol = ut_alloc_1d_char (20);
      double *params = ut_alloc_1d (20);

      nev_print_symbol_expr_params (Data.Rad[ptcells[i]], Data.Symbol[ptcells[i]], symbol, params);

      if (strstr (Print.imageformat, "png") && Data.Trs[ptcells[i]] > 0)
        trs_disable = 1;

      fprintf (file, "%s (scale(scale/%f)*(%9.6f, %9.6f), %f, %f, rgb(%f,%f,%f), %f, rgb(%f,%f,%f), %f);\n",
               symbol, !strcmp (Print.space, "ipf") ? p110[1] : 1,
               pts[i][0],
               pts[i][1],
               params[0] * 30, params[1],
               Data.Col[ptcells[i]][0] / 255.,
               Data.Col[ptcells[i]][1] / 255.,
               Data.Col[ptcells[i]][2] / 255.,
               strstr (Print.imageformat, "png") ? 0. : Data.Trs[ptcells[i]],
               Data.BCol[0] / 255.,
               Data.BCol[1] / 255.,
               Data.BCol[2] / 255.,
               Data.BRad * 30);

      ut_free_1d_char (&symbol);
      ut_free_1d (&params);
    }
  }

  if (trs_disable)
    ut_print_message (1, 3, "Transparency disabled (use pdf-only output).\n");

  ut_free_1d (&v);
  ut_free_1d (&p110);

  return;
}
