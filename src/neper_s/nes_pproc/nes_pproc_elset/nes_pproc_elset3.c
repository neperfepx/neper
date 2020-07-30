/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_elset_.h"

void
nes_pproc_elset_compute (int elset, char *var, int step, char *out,
                         FILE * file)
{
  FILE *in = NULL;
  double rel;
  char *tmp = ut_alloc_1d_char (1000);
  char *tmp2 = NULL;
  struct OL_SET Set, Set2;

  ut_string_string (var, &tmp2);
  ut_string_fnr (tmp2, '(', ' ');
  ut_string_fnr (tmp2, ')', ' ');
  ut_string_fnr (tmp2, ',', ' ');

  if (!strncmp (var, "orf", 3))
  {
    sprintf (tmp, "%s/elsets/elset%d/ori/ori-step%d", out, elset, step);
    in = ut_file_open (tmp, "R");

    sscanf (tmp2, "%*s%lf", &rel);

    ol_set_fscanf (in, &Set, NULL);
    Set2 = ol_set_alloc (Set.size, Set.crysym);

    double **v = ol_g_alloc ();
    double *theta = ol_e_alloc ();
    ol_set_anifilter_iter (Set, 3, &Set2, NULL, v, theta);
    ol_set_fprintf (file, Set2, "%.12f");
    ol_g_free (v);
    ol_e_free (theta);

    ut_file_close (in, tmp, "R");
  }

  // mean orientation
  else if (!strcmp (var, "orm"))
  {
    if (ut_file_exist ("%s/elsets/elset%d/orf/orf-step%d", out, elset, step))
      sprintf (tmp, "%s/elsets/elset%d/orf/orf-step%d", out, elset, step);
    else
      if (ut_file_exist
          ("%s/elsets/elset%d/ori/ori-step%d", out, elset, step))
      sprintf (tmp, "%s/elsets/elset%d/ori/ori-step%d", out, elset, step);
    else
      abort ();

    in = ut_file_open (tmp, "R");
    ol_set_fscanf (in, &Set, NULL);
    double *qmean = ol_q_alloc ();
    double *emean = ol_e_alloc ();
    ol_set_mean_iter (Set, qmean);
    ol_q_e (qmean, emean);
    ol_e_fprintf (file, emean, "%.12f");
    ol_e_free (emean);
    ol_q_free (qmean);
    ut_file_close (in, tmp, "R");
  }

  // anisotropy properties
  else if (!strcmp (var, "ora"))
  {
    if (ut_file_exist ("%s/elsets/elset%d/orf/orf-step%d", out, elset, step))
      sprintf (tmp, "%s/elsets/elset%d/orf/orf-step%d", out, elset, step);
    else
      if (ut_file_exist
          ("%s/elsets/elset%d/ori/ori-step%d", out, elset, step))
      sprintf (tmp, "%s/elsets/elset%d/ori/ori-step%d", out, elset, step);
    else
      abort ();

    in = ut_file_open (tmp, "R");
    ol_set_fscanf (in, &Set, NULL);
    Set2 = ol_set_alloc (Set.size, Set.crysym);
    ol_set_disoriset (Set, &Set2);
    double **v = ol_g_alloc ();
    double *theta = ol_e_alloc ();
    ol_set_aniso (Set2, v, theta);
    ut_array_1d_fprintf (file, theta, 3, "%.12f");
    ut_array_2d_fprintf (file, v, 3, 3, "%.12f");
    ol_g_free (v);
    ol_e_free (theta);
    ut_file_close (in, tmp, "R");
  }

  ut_free_1d_char (&tmp);
  ol_set_free (Set);

  return;
}
