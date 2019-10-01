/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

void
neut_tesr_fscanf_data_default (struct TESR *pTesr, char *format, FILE * file)
{
  int i, j, k;

  for (k = 1; k <= (*pTesr).size[2]; k++)
    for (j = 1; j <= (*pTesr).size[1]; j++)
      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
	if (!strcmp (format, "ascii"))
	{
	  int data;
	  if (fscanf (file, "%d", &data) != 1)
	    abort ();

	  (*pTesr).VoxCell[i][j][k] = data;
	}
	else if (!strcmp (format, "binary8"))
	{
	  unsigned char data;
	  if (fread (&data, sizeof (unsigned char), 1, file) != 1)
	    abort ();

	  (*pTesr).VoxCell[i][j][k] = data;
	}
	else if ((!strcmp (format, "binary16") && !ut_sys_endian ())
	      || (!strcmp (format, "binary16_big") && ut_sys_endian ()))
	{
	  short data;
	  if (fread (&data, sizeof (short), 1, file) != 1)
	    abort ();

	  (*pTesr).VoxCell[i][j][k] = (int) data;
	}
	else if ((!strcmp (format, "binary16") && ut_sys_endian ())
	      || (!strcmp (format, "binary16_big") && !ut_sys_endian ()))
	{
	  short data;
	  short val2;
	  char *pval = NULL;
	  char *pval2 = (char *) &val2;

	  if (fread (&data, sizeof (short), 1, file) != 1)
	    abort ();

	  pval = (char *) &data;
	  pval2[1] = pval[0];
	  pval2[0] = pval[1];

	  (*pTesr).VoxCell[i][j][k] = (int) val2;
	}
	else if ((!strcmp (format, "binary32") && !ut_sys_endian ())
	      || (!strcmp (format, "binary32_big") && ut_sys_endian ()))
	{
	  int data;
	  if (fread (&data, sizeof (int), 1, file) != 1)
	    abort ();

	  (*pTesr).VoxCell[i][j][k] = data;
	}
	else if ((!strcmp (format, "binary32") && ut_sys_endian ())
	      || (!strcmp (format, "binary32_big") && !ut_sys_endian ()))
	{
	  int val2 = 0;
	  char *pval = NULL;
	  char *pval2 = (char *) &val2;

	  int data;

	  if (fread (&data, sizeof (int), 1, file) != 1)
	    abort ();

	  pval = (char *) &data;
	  pval2[3] = pval[0];
	  pval2[2] = pval[1];
	  pval2[1] = pval[2];
	  pval2[0] = pval[3];

	  (*pTesr).VoxCell[i][j][k] = (int) val2;
	}
	else
	  ut_print_message (2, 2, "Unknown format `%s'.\n", format);
      }

  return;
}

void
neut_tesr_fscanf_oridata_default (struct TESR *pTesr, char *des, char *format,
                                  FILE * file)
{
  int i, j, k, l, endian = ut_sys_endian ();

  if (!strcmp (des, "g"))
  {
    double **g = ol_g_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_g_fscanf (file, g);
          else
            for (l = 0; l < 9; l++)
            {
              if (fread (g[l / 3] + l % 3, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                g[l / 3][l % 3] = ut_num_reverseendian (g[l / 3][l % 3]);
            }

          ol_g_q (g, (*pTesr).VoxOri[i][j][k]);
        }
    ol_g_free (g);
  }

  else if (!strcmp (des, "rtheta"))
  {
    double theta, *r = ol_r_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_rtheta_fscanf (file, r, &theta);
          else
          {
            for (l = 0; l < 3; l++)
            {
              if (fread (r + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                r[l] = ut_num_reverseendian (r[l]);
            }
            if (fread (&theta, sizeof (double), 1, file) != 1)
              abort ();
            if (endian)
              theta = ut_num_reverseendian (theta);
          }

          ol_rtheta_q (r, theta, (*pTesr).VoxOri[i][j][k]);
        }
    ol_r_free (r);
  }

  else if (!strcmp (des, "R"))
  {
    double *R = ol_R_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_R_fscanf (file, R);
          else
            for (l = 0; l < 3; l++)
            {
              if (fread (R + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                R[l] = ut_num_reverseendian (R[l]);
            }

          ol_R_q (R, (*pTesr).VoxOri[i][j][k]);
        }
    ol_R_free (R);
  }

  else if (!strcmp (des, "q"))
  {
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
          if (!strcmp (format, "ascii"))
            ol_q_fscanf (file, (*pTesr).VoxOri[i][j][k]);
          else
            for (l = 0; l < 4; l++)
            {
              if (fread ((*pTesr).VoxOri[i][j][k] + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                (*pTesr).VoxOri[i][j][k][l]
                  = ut_num_reverseendian ((*pTesr).VoxOri[i][j][k][l]);
            }
  }

  else if (!strcmp (des, "e"))
  {
    double *e = ol_e_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_e_fscanf (file, e);
          else
            for (l = 0; l < 3; l++)
            {
              if (fread (e + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                e[l] = ut_num_reverseendian (e[l]);
            }

          ol_e_q (e, (*pTesr).VoxOri[i][j][k]);
        }
    ol_e_free (e);
  }

  else if (!strcmp (des, "ek"))
  {
    double *e = ol_e_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_e_fscanf (file, e);
          else
            for (l = 0; l < 3; l++)
            {
              if (fread (e + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                e[l] = ut_num_reverseendian (e[l]);
            }

          ol_ek_e (e, e);
          ol_e_q (e, (*pTesr).VoxOri[i][j][k]);
        }
    ol_e_free (e);
  }

  else if (!strcmp (des, "er"))
  {
    double *e = ol_e_alloc ();
    for (k = 1; k <= (*pTesr).size[2]; k++)
      for (j = 1; j <= (*pTesr).size[1]; j++)
        for (i = 1; i <= (*pTesr).size[0]; i++)
        {
          if (!strcmp (format, "ascii"))
            ol_e_fscanf (file, e);
          else
            for (l = 0; l < 3; l++)
            {
              if (fread (e + l, sizeof (double), 1, file) != 1)
                abort ();
              if (endian)
                e[l] = ut_num_reverseendian (e[l]);
            }

          ol_er_e (e, e);
          ol_e_q (e, (*pTesr).VoxOri[i][j][k]);
        }
    ol_e_free (e);
  }

  else
    ut_error_expression (des);

  return;
}

void
neut_tesr_fscanf_data_bounds (struct TESR *pTesr, int *bounds, char *format,
			      FILE * file)
{
  int i, j, k, ii, jj, kk;

  for (k = 1; k <= (*pTesr).size[2]; k++)
  {
    kk = k - bounds[4] + 1;

    for (j = 1; j <= (*pTesr).size[1]; j++)
    {
      jj = j - bounds[2] + 1;

      for (i = 1; i <= (*pTesr).size[0]; i++)
      {
	ii = i - bounds[0] + 1;

	if (!strcmp (format, "ascii"))
	{
	  int data;
	  if (fscanf (file, "%d", &data) != 1)
	    abort ();

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = data;
	}
	else if (!strcmp (format, "binary8"))
	{
	  unsigned char data;
	  if (fread (&data, sizeof (unsigned char), 1, file) != 1)
	    abort ();

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = data;
	}
	else if (!strcmp (format, "binary16"))
	{
	  short data;
	  if (fread (&data, sizeof (short), 1, file) != 1)
	    abort ();

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = (int) data;
	}
	else if (!strcmp (format, "binary16*"))
	{
	  short data;
	  short val2;
	  char *pval = NULL;
	  char *pval2 = (char *) &val2;

	  if (fread (&data, sizeof (short), 1, file) != 1)
	    abort ();

	  pval = (char *) &data;
	  pval2[1] = pval[0];
	  pval2[0] = pval[1];

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = (int) val2;
	}
	else if (!strcmp (format, "binary32"))
	{
	  int data;
	  if (fread (&data, sizeof (int), 1, file) != 1)
	    abort ();

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = data;
	}
	else if (!strcmp (format, "binary32*"))
	{
	  int val2 = 0;
	  char *pval = NULL;
	  char *pval2 = (char *) &val2;

	  int data;

	  if (fread (&data, sizeof (int), 1, file) != 1)
	    abort ();

	  pval = (char *) &data;
	  pval2[3] = pval[0];
	  pval2[2] = pval[1];
	  pval2[1] = pval[2];
	  pval2[0] = pval[3];

	  if (k >= bounds[4] && k <= bounds[5]
	      && j >= bounds[2] && j <= bounds[3]
	      && i >= bounds[0] && i <= bounds[1])
	    (*pTesr).VoxCell[ii][jj][kk] = (int) val2;
	}
	else
	  abort ();
      }
    }
  }

  return;
}

void
neut_tesr_fscanf_data_scale (struct TESR *pTesr, double *scale, char *format,
			     FILE * file)
{
  neut_tesr_fscanf_data_default (pTesr, format, file);

  neut_tesr_rasterscale (pTesr, scale[0], scale[1], scale[2]);

  return;
}
