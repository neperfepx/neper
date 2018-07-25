/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_res_ori_.h"

extern void
net_res_ori (struct IN_T In, struct SEEDSET SSet)
{
  FILE *file = NULL;
  int i, j;
  char *des = ut_alloc_1d_char (10);
  char *format = ut_alloc_1d_char (10);

  file = ut_file_open (In.orif, "w");

  strcpy (des, In.orides);
  strcpy (format, In.oriformat);

  // plain format
  if (strcmp (format, "plain") == 0)
  {
    if (strcmp (des, "e") == 0)
    {
      double *e = ol_e_alloc ();
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_e (SSet.q[i], e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (des, "ek") == 0)
    {
      double *e = ol_e_alloc ();
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_e (SSet.q[i], e);
	ol_e_ek (e, e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (des, "er") == 0)
    {
      double *e = ol_e_alloc ();
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_e (SSet.q[i], e);
	ol_e_er (e, e);
	ol_e_fprintf (file, e, "%17.12f");
      }
      ol_e_free (e);
    }
    else if (strcmp (des, "g") == 0)
    {
      double **g = ol_g_alloc ();
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_g (SSet.q[i], g);
	ol_g_fprintf (file, g, "%17.12f");
      }
      ol_g_free (g);
    }
    else if (strcmp (des, "rtheta") == 0)
    {
      double *r = ol_r_alloc ();
      double theta;
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_rtheta (SSet.q[i], r, &theta);
	ol_rtheta_fprintf (file, r, theta, "%17.12f");
      }
      ol_r_free (r);
    }
    else if (strcmp (des, "theta") == 0)
    {
      double theta;
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_theta (SSet.q[i], &theta);
	ol_theta_fprintf (file, theta, "%17.12f");
      }
    }
    else if (strcmp (des, "R") == 0)
    {
      double *R = ol_R_alloc ();
      for (i = 1; i <= SSet.N; i++)
      {
	ol_q_R (SSet.q[i], R);
	ol_R_fprintf (file, R, "%17.12f");
      }
      ol_R_free (R);
    }
    else if (strcmp (des, "q") == 0)
    {
      for (i = 1; i <= SSet.N; i++)
	ol_q_fprintf (file, SSet.q[i], "%17.12f");
    }
    else if (strcmp (des, "Rcol") == 0)
    {
      double *R = ol_R_alloc ();
      int *rgb = ut_alloc_1d_int (3);
      double SQRT2 = 1.41421356237309504880;
      for (i = 1; i <= SSet.N; i++)
      {
	if (strcmp (In.oricrysym[In.levelqty], "cubic"))
	  ut_print_message (2, 1, "Rcol requires `-oricrysym cubic'.\n");

	ol_q_R (SSet.q[i], R);
	for (j = 0; j < 3; j++)
	  rgb[j] =
	    ut_num_d2ri (255 * (R[j] + (SQRT2 - 1)) / (2 * (SQRT2 - 1)));

	ut_array_1d_int_fprintf (file, rgb, 3, "%3d");
      }
      ol_R_free (R);
      ut_free_1d_int (rgb);
    }
    else
      ut_print_message (2, 1, "Format %s not available.\n", des);
  }

  // fepx format
  else if (strcmp (format, "fepx") == 0)
  {
    fprintf (file, "grain-orientations\n%d\n", SSet.N);
    double *e = ol_e_alloc ();
    for (i = 1; i <= SSet.N; i++)
    {
      ol_q_e (SSet.q[i], e);
      ol_e_ek (e, e);
      for (j = 0; j < 3; j++)
	fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "%d\n", i);
    }
    ol_e_free (e);
    fprintf (file, "EOF\n");
  }

  // geof format
  else if (strcmp (format, "geof") == 0)
  {
    double *e = ol_e_alloc ();
    for (i = 1; i <= SSet.N; i++)
    {
      fprintf (file, "**elset poly%d  *rotation ", i);
      ol_q_e (SSet.q[i], e);
      ol_e_e (e, e);
      for (j = 0; j < 3; j++)
	fprintf (file, "%17.12f ", e[j]);
      fprintf (file, "\n");
    }
    ol_e_free (e);
  }

  ut_file_close (file, In.orif, "w");

  ut_free_1d_char (des);
  ut_free_1d_char (format);

  return;
}
