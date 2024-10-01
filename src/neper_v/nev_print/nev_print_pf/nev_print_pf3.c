/* This id is part of the 'hermes' program. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf_file_data_ori (int input, struct PF *pPf, double **q,
                            double *weights, int qty, int *show,
                            double ***ppts, double **pptwgts,
                            int **pptcells, int *pptqty)
{
  int i, j;
  struct OL_SET Set;
  double *qref = ol_q_alloc ();

  ol_g_q ((*pPf).pfdir, qref);

  Set = ol_set_alloc (qty, (*pPf).crysym);

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

  if (!strcmp ((*pPf).space, "ipf") && !(*pPf).inputcellpole[input])
    nev_print_pf_file_data_ori_preipf (input, Set, (*pPf).pfpoles[1], pPf);

  (*ppts) = ut_alloc_2d (qty * (*pPf).pfpoleqty, 2);
  (*pptcells) = ut_alloc_1d_int (qty * (*pPf).pfpoleqty);
  (*pptwgts) = ut_alloc_1d (qty * (*pPf).pfpoleqty);

  (*pptqty) = 0;
  for (i = 0; i < (int) Set.size; i++)
    if (!show || show[i])
    {
      if (!strcmp ((*pPf).space, "pf"))
      {
        for (j = 1; j <= (*pPf).pfpoleqty; j++)
        {
	  if (! strcmp ((*pPf).projection, "stereographic"))
	    ol_q_pf_stprojxy (Set.q[i], (*pPf).pfpoles[j], (*ppts)[*pptqty]);
	  else if (! strcmp ((*pPf).projection, "equal-area"))
	    ol_q_pf_eaprojxy (Set.q[i], (*pPf).pfpoles[j], (*ppts)[*pptqty]);
	  else
	    abort();

          (*pptcells)[(*pptqty)] = i + 1;
          (*pptwgts)[(*pptqty)] = Set.weight[i];
          (*pptqty)++;
	}
      }

      else if (!strcmp ((*pPf).space, "ipf"))
      {
        int index, eqqty = ol_crysym_qty (Set.crysym);
        double **qs = ut_alloc_2d (eqqty + 1, 4);
        double *q = ol_q_alloc ();
        double *misori = ut_alloc_1d (eqqty + 1);

        // update reference orientation, (*pPf).inputcellori[input][i + 1]
        // by taking the closest (less-misoriented) orientation (no crystal sym)
        for (j = 1; j <= ol_crysym_qty (Set.crysym); j++)
        {
          ol_q_crysym (Set.q[i], Set.crysym, j, qs[j]);
          ol_q_q_misori (qs[j], (*pPf).inputcellori[input][i + 1], misori + j);
        }
        index = ut_array_1d_min_index (misori + 1, eqqty) + 1;
        ol_q_memcpy (qs[index], (*pPf).inputcellori[input][i + 1]);

        // taking the right equivalent
        ol_q_crysym (qs[index], Set.crysym, (*pPf).inputcellpole[input][i + 1], q);

        if (! strcmp ((*pPf).projection, "stereographic"))
          ol_q_ipf_stprojxy (q, (*pPf).pfpoles[1], (*ppts)[*pptqty]);
        else if (! strcmp ((*pPf).projection, "equal-area"))
          ol_q_ipf_eaprojxy (q, (*pPf).pfpoles[1], (*ppts)[*pptqty]);
        else
          abort();

        (*pptcells)[*pptqty] = i + 1;
        (*pptwgts)[*pptqty] = Set.weight[i];
        (*pptqty)++;

        ut_free_1d (&misori);
        ut_free_2d (&qs, eqqty + 1);
        ol_q_free (q);
      }

      else
        abort ();
    }

  ol_q_free (qref);

  return;
}

void
nev_print_pf_file_data_vect (int input, struct PF *pPf,
                             double **data, double *weights, int qty,
                             int *show, double ***ppts, double **pptwgts,
                             int **pptcells, int *pptqty)
{
  int i;
  double *vect = ut_alloc_1d (3);

  (*ppts) = ut_alloc_2d (qty, 2);
  (*pptcells) = ut_alloc_1d_int (qty);
  (*pptwgts) = ut_alloc_1d (qty);

  if (!strcmp ((*pPf).space, "ipf") && !(*pPf).inputcellpole[input])
    nev_print_pf_file_data_vect_preipf (input, data, qty, pPf);

  (*pptqty) = 0;
  for (i = 0; i < qty; i++)
    if (!show || show[i])
    {
      if (!strcmp ((*pPf).space, "pf"))
      {
        ol_g_vect_vect ((*pPf).pfdir, data[i], vect);
        ut_array_1d_scale (vect, 3, 1. / ut_array_1d_norm (vect, 3));

        if (! strcmp ((*pPf).projection, "stereographic"))
          ol_vect_stprojxy (vect, (*ppts)[(*pptqty)]);
        else if (! strcmp ((*pPf).projection, "equal-area"))
          ol_vect_eaprojxy (vect, (*ppts)[(*pptqty)]);
        else
          abort();

        (*pptcells)[(*pptqty)] = i + 1;
        (*pptwgts)[(*pptqty)] = weights ? weights[i] : 1;
        (*pptqty)++;
      }

      else if (!strcmp ((*pPf).space, "ipf"))
      {
        int eqqty = ol_crysym_qty ((*pPf).crysym);
        double **qs = ut_alloc_2d (eqqty + 1, 4);
        double *q = ol_q_alloc ();
        double *misori = ut_alloc_1d (eqqty + 1);

        /*
        // update reference orientation, (*pPf).inputcellori[input][i + 1]
        // by taking the closest (less-misoriented) orientation (no crystal sym)
        // int index;
        for (j = 1; j <= ol_crysym_qty (Set.crysym); j++)
        {
          ol_q_crysym (Set.q[i], Set.crysym, j, qs[j]);
          ol_q_q_misori (qs[j], (*pPf).inputcellori[input][i + 1], misori + j);
        }
        index = ut_array_1d_min_index (misori + 1, eqqty) + 1;
        ol_q_memcpy (qs[index], (*pPf).inputcellori[input][i + 1]);
        */

        double **I = ol_g_alloc ();
        double **gs = ol_g_alloc ();
        double *v = ol_vect_alloc ();

        ol_g_set_id (I);

        // taking the right equivalent
        // ol_q_crysym (qs[index], Set.crysym, (*pPf).inputcellpole[input][i + 1], q);
        ol_g_crysym (I, (*pPf).crysym, (*pPf).inputcellpole[input][i + 1], gs);
        ol_g_vect_vect (gs, data[i], v);
        ut_array_1d_normalize (v, 3);

        if (! strcmp ((*pPf).projection, "stereographic"))
          ol_vect_stprojxy (v, (*ppts)[*pptqty]);
        else if (! strcmp ((*pPf).projection, "equal-area"))
          ol_vect_eaprojxy (v, (*ppts)[*pptqty]);
        else
          abort();

        (*pptcells)[*pptqty] = i + 1;
        (*pptwgts)[*pptqty] = weights[i];
        (*pptqty)++;

        ut_free_1d (&misori);
        ut_free_2d (&qs, eqqty + 1);
        ol_q_free (q);
        ol_g_free (gs);
        ol_vect_free (v);
      }
    }

  /*
  (*pptqty) = 0;
  for (i = 0; i < (int) Set.size; i++)
    if (!show || show[i])
    {
      if (!strcmp ((*pPf).space, "pf"))
      {
        for (j = 0; j < (*pPf).pfpoleqty; j++)
        {
	  if (! strcmp (Pf.projection, "stereographic"))
	    ol_q_pf_stprojxy (Set.q[i], uvect[j], (*ppts)[*pptqty]);
	  else if (! strcmp (Pf.projection, "equal-area"))
	    ol_q_pf_eaprojxy (Set.q[i], uvect[j], (*ppts)[*pptqty]);
	  else
	    abort();

          (*pptcells)[(*pptqty)] = i + 1;
          (*pptwgts)[(*pptqty)] = Set.weight[i];
          (*pptqty)++;
	}
      }

      else
        abort ();
    }
    */

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
nev_print_pf_file_data_crysym (struct PF Pf, double ***ppts, double **pptwgts,
                           int **pptcells, int *pptqty)
{
  int i, j;
  int qty = *pptqty;
  double** I = ol_g_alloc ();
  double** gs = ol_g_alloc ();
  double* v0 = ol_r_alloc ();
  double* v = ol_r_alloc ();

  int eqqty = ol_crysym_qty (Pf.crysym);

  (*ppts) = ut_realloc_1d_pdouble ((*ppts), eqqty * qty);
  (*pptcells) = ut_realloc_1d_int ((*pptcells), eqqty * qty);
  (*pptwgts) = ut_realloc_1d ((*pptwgts), eqqty * qty);

  for (i = 2; i <= eqqty; i++)
  {
    ol_g_crysym (I, Pf.crysym, i, gs);

    for (j = 0; j < qty; j++)
    {
      (*ppts)[*pptqty] = ut_alloc_1d (3);

      if (!strcmp (Pf.projection, "stereographic"))
	ol_stprojxy_vect ((*ppts)[j], v0);
      else if (!strcmp (Pf.projection, "equal-area"))
	ol_eaprojxy_vect ((*ppts)[j], v0);

      ol_g_vect_vect (gs, v0, v);

      if (!strcmp (Pf.projection, "stereographic"))
	ol_vect_stprojxy (v, (*ppts)[*pptqty]);
      else if (!strcmp (Pf.projection, "equal-area"))
	ol_vect_eaprojxy (v, (*ppts)[*pptqty]);

      (*pptcells)[*pptqty] = (*pptcells)[j];
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
nev_print_pf_file_data_clustering (struct PF Pf, double ***ppts,
                                   double **pptwgts, int **pptcells, int *pptqty)
{
  int i, j, k, *pos = ut_alloc_1d_int (2);
  int qty_initial = *pptqty;
  double **grid_w = ut_alloc_2d (Pf.gridsize[0], Pf.gridsize[1]);
  int **grid_cell = ut_alloc_2d_int (Pf.gridsize[0], Pf.gridsize[1]);

  ut_print_message (0, 2, "Reducing data...\n");

  for (k = 0; k < (*pptqty); k++)
  {
    neut_pf_point_gridpos (Pf, (*ppts)[k], pos);

    grid_w[pos[0]][pos[1]] += (*pptwgts)[k];
    grid_cell[pos[0]][pos[1]] = (*pptcells)[k];
  }

  ut_free_2d (ppts, *pptqty);
  (*ppts) = ut_alloc_2d (1, 2);
  (*pptqty) = 0;
  (*pptwgts) = ut_realloc_1d (*pptwgts, 1);
  (*pptcells) = ut_realloc_1d_int (*pptcells, 1);
  for (i = 0; i < Pf.gridsize[0]; i++)
    for (j = 0; j < Pf.gridsize[1]; j++)
      if (grid_w[i][j] > 1e-12)
      {
        (*pptqty)++;
        (*ppts) = ut_realloc_2d_addline (*ppts, *pptqty, 2);
        ut_array_1d_memcpy (Pf.grid[i][j], 2, (*ppts)[(*pptqty) - 1]);
        (*pptwgts) = ut_realloc_1d (*pptwgts, *pptqty);
        (*pptwgts)[(*pptqty) - 1] = grid_w[i][j];
        (*pptcells) = ut_realloc_1d_int (*pptcells, *pptqty);
        (*pptcells)[(*pptqty) - 1] = grid_cell[i][j];
      }

  ut_print_message (0, 3, "Number of data reduced by %.1f% (to %d)\n", 100 *
      (double) (qty_initial - (*pptqty)) / qty_initial, !strcmp (Pf.refsym, "monoclinic") ? *pptqty : (*pptqty) / 4);

  ut_free_2d (&grid_w, Pf.gridsize[0]);
  ut_free_2d_int (&grid_cell, Pf.gridsize[0]);
  ut_free_1d_int (&pos);

  return;
}

void
nev_print_pf_ptsprint_symbols (struct IN_V In, int input, struct PF *pPf,
                               FILE *file, struct DATA Data, double **pts,
                               int *ptcells, int ptqty)
{
  int i, *plot = NULL, trs_disable;
  double* v = ut_alloc_1d (3);
  double* p110 = ol_p_alloc ();

  ut_array_1d_set_3 (v, 0, OL_IS2, OL_IS2);
  if (! strcmp ((*pPf).projection, "stereographic"))
    ol_vect_stprojxy (v, p110);
  else if (! strcmp ((*pPf).projection, "equal-area"))
    ol_vect_eaprojxy (v, p110);

  trs_disable = 0;

  // for each point, do we plot or not?
  plot = ut_alloc_1d_int (ptqty);

  if (!strcmp ((*pPf).space, "pf"))
    for (i = 0; i < ptqty; i++)
    {
      plot[i] = 0;
      if (!strcmp ((*pPf).shape, "full"))
        plot[i] = 1;
      else if (!strcmp ((*pPf).shape, "quarter"))
      {
        if (pts[i][0] >= 0 && pts[i][1] <= 0)
          plot[i] = 1;
      }
    }

  else if (!strcmp ((*pPf).space, "ipf"))
  {
    int cell;
    int *cell_plotted = ut_alloc_1d_int ((*pPf).inputcellqty[input] + 1);

    for (i = 0; i < ptqty; i++)
    {
      cell = ptcells[i];
      if (!cell_plotted[cell])
      {
        plot[i] = 1;
        cell_plotted[cell] = 1;
      }
    }

    ut_free_1d_int (&cell_plotted);
  }

  else
    abort ();

  // plotting
  int antipodal = 0;
  for (i = 0; i < ptqty; i++)
    if (plot[i])
    {
      int cell = ptcells[i];
      char *symbol = ut_alloc_1d_char (20);
      double *params = ut_alloc_1d (20);

      nev_print_symbol_expr_params (Data.Rad[cell], Data.Symbol[cell], symbol, params);

      if (strstr (In.imageformat, "png") && Data.Trs[cell] > 0)
        trs_disable = 1;

      fprintf (file, "%s (scale(scale/%f)*(%9.6f, %9.6f), %f, %f, rgb(%f,%f,%f), %f, rgb(%f,%f,%f), %f);\n",
               symbol, (!strcmp ((*pPf).space, "ipf") && !strcmp ((*pPf).crysym, "cubic")) ? p110[1] : 1,
               pts[i][0],
               pts[i][1],
               params[0] * 30, params[1],
               Data.Col[cell][0] / 255.,
               Data.Col[cell][1] / 255.,
               Data.Col[cell][2] / 255.,
               strstr (In.imageformat, "png") ? 0. : Data.Trs[cell],
               Data.BCol[0] / 255.,
               Data.BCol[1] / 255.,
               Data.BCol[2] / 255.,
               Data.BRad * 30);

      if (!strcmp ((*pPf).space, "pf") && !strcmp ((*pPf).shape, "full"))
      {
        if (ut_vector_norm_2d (pts[i]) > 0.999 && !antipodal)
        {
          ut_array_1d_scale (pts[i], 2, -1);
          antipodal = 1;
          i--;
        }
        else if (antipodal)
          antipodal = 0;
      }

      ut_free_1d_char (&symbol);
      ut_free_1d (&params);
    }

  if (trs_disable)
    ut_print_message (1, 3, "Transparency disabled (use pdf-only output).\n");

  ut_free_1d (&v);
  ut_free_1d (&p110);
  ut_free_1d_int (&plot);

  return;
}
