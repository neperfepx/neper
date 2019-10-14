/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

/* Tessellation exportation: head */
void
neut_tesr_fscanf_head (struct TESR *pTesr, double *bounds, int **pvoxbounds,
                       char **pformat, FILE * file)
{
  int i, status;
  char *string = ut_alloc_1d_char (1000);

  (*pformat) = ut_alloc_1d_char (10);

  if (ut_file_string_scanncomp (file, "***tesr") != 0)
  {
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");
    abort ();
  }

  if (ut_file_string_scanncomp (file, "**format") != 0
      || ut_file_string_scanncomp (file, "2.0") != 0)
  {
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");
    abort ();
  }

  if (fscanf (file, "%s", *pformat) != 1
      || (!strcmp (*pformat, "ascii") && !strncmp (*pformat, "bin", 3)))
  {
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");
    abort ();
  }

  if (ut_file_string_scanncomp (file, "**general") != 0)
  {
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");
    abort ();
  }

  status = fscanf (file, "%d", &((*pTesr).Dim));

  if (status != 1)
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");

  (*pTesr).size = ut_alloc_1d_int (3);
  (*pTesr).Origin = ut_alloc_1d (3);
  ut_array_1d_int_set ((*pTesr).size, 3, 1);
  status = ut_array_1d_int_fscanf (file, (*pTesr).size, (*pTesr).Dim);

  if (status != 1)
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");

  (*pTesr).vsize = ut_alloc_1d (3);
  status = ut_array_1d_fscanf (file, (*pTesr).vsize, (*pTesr).Dim);

  while (ut_file_nextstring (file, string) == 1 && strncmp (string, "**", 2))
  {
    if (!strcmp (string, "*origin"))
    {
      ut_file_skip (file, 1);
      ut_array_1d_fscanf (file, (*pTesr).Origin, (*pTesr).Dim);
    }

    else if (!strcmp (string, "*hasvoid"))
    {
      ut_file_skip (file, 1);
      if (fscanf (file, "%d", &(*pTesr).hasvoid) != 1)
        ut_print_message (2, 0,
                          "Input file is not a valid raster tessellation file.\n");
    }

    else
      ut_print_message (2, 2, "Failed to read tesr file.\n");
  }

  if (bounds)
  {
    int *vox = ut_alloc_1d_int (3);
    double *coo = ut_alloc_1d (3);
    (*pvoxbounds) = ut_alloc_1d_int (6);

    for (i = 0; i <= 1; i++)
    {
      ut_array_1d_set_3 (coo, bounds[i] - (*pTesr).Origin[0],
                              bounds[i + 2] - (*pTesr).Origin[1],
                              bounds[i + 4] - (*pTesr).Origin[2]);
      neut_tesr_coo_pos (*pTesr, coo, i?-1:1, vox);
      (*pvoxbounds)[i] = vox[0];
      (*pvoxbounds)[i + 2] = vox[1];
      (*pvoxbounds)[i + 4] = vox[2];
    }

    ut_free_1d_int (vox);
    ut_free_1d (coo);
  }

  if (status != 1)
    ut_print_message (2, 0,
		      "Input file is not a valid raster tessellation file.\n");

  ut_free_1d_char (string);

  return;
}

void
neut_tesr_fscanf_cell (struct TESR *pTesr, FILE * file)
{
  int i, status, id;
  double theta;
  char *string = ut_alloc_1d_char (1000);
  double *des = ut_alloc_1d (9);

  ut_file_nextstring (file, string);

  if (!strcmp (string, "**cell"))
  {
    ut_file_skip (file, 1);
    if (fscanf (file, "%d", &(*pTesr).CellQty) != 1)
      abort ();

    while (ut_file_nextstring (file, string) == 1 && strncmp (string, "**", 2))
    {
      if (!strcmp (string, "*id"))
      {
	ut_file_skip (file, 1);
	(*pTesr).CellId = ut_alloc_1d_int ((*pTesr).CellQty + 1);

	ut_array_1d_int_fscanf (file, (*pTesr).CellId + 1, (*pTesr).CellQty);
      }

      else if (!strcmp (string, "*seed"))
      {
	ut_file_skip (file, 1);
	(*pTesr).SeedCoo = ut_alloc_2d ((*pTesr).CellQty + 1, 3);
	(*pTesr).SeedWeight = ut_alloc_1d ((*pTesr).CellQty + 1);

	for (i = 1; i <= (*pTesr).CellQty; i++)
	{
	  status = fscanf (file, " %d", &id);
	  if (id != i)
	    abort ();
	  ut_array_1d_fscanf (file, (*pTesr).SeedCoo[id], (*pTesr).Dim);
	  status = fscanf (file, " %lf", &((*pTesr).SeedWeight[id]));

	  if (status != 1)
	    abort ();
	}
      }

      else if (!strcmp (string, "*ori"))
      {
	if (!(*pTesr).CellId)
	  ut_print_message (2, 2, "Field *ori requires *id.\n");

	ut_file_skip (file, 1);
	(*pTesr).CellOri = ut_alloc_2d ((*pTesr).CellQty + 1, 4);

	if (fscanf (file, "%s", string) != 1)
	  abort ();

	if (!strcmp (string, "q"))
	  ut_array_2d_fscanf (file, (*pTesr).CellOri + 1, (*pTesr).CellQty, 4);
	else if (!strcmp (string, "e"))
	  for (i = 1; i <= (*pTesr).CellQty; i++)
	  {
	    ol_e_fscanf (file, des);
	    ol_e_q (des, (*pTesr).CellOri[i]);
	  }
	else if (!strcmp (string, "er"))
	  for (i = 1; i <= (*pTesr).CellQty; i++)
	  {
	    ol_e_fscanf (file, des);
	    ol_er_e (des, des);
	    ol_e_q (des, (*pTesr).CellOri[i]);
	  }
	else if (!strcmp (string, "ek"))
	  for (i = 1; i <= (*pTesr).CellQty; i++)
	  {
	    ol_e_fscanf (file, des);
	    ol_ek_e (des, des);
	    ol_e_q (des, (*pTesr).CellOri[i]);
	  }
	else if (!strcmp (string, "rtheta"))
	  for (i = 1; i <= (*pTesr).CellQty; i++)
	  {
	    ol_r_fscanf (file, des);
	    if (fscanf (file, "%lf", &theta) != 1)
	      abort ();
	    ol_rtheta_q (des, theta, (*pTesr).CellOri[i]);
	  }
	else if (!strcmp (string, "R"))
	  for (i = 1; i <= (*pTesr).CellQty; i++)
	  {
	    ol_R_fscanf (file, des);
	    ol_R_q (des, (*pTesr).CellOri[i]);
	  }
	else
	  ut_print_message (2, 2, "Could not process descriptor `%s'.\n",
			    string);
      }

      else if (!strcmp (string, "*coo"))
      {
	ut_file_skip (file, 1);
	(*pTesr).CellCoo = ut_alloc_2d ((*pTesr).CellQty + 1, 3);

	for (i = 1; i <= (*pTesr).CellQty; i++)
	  ut_array_1d_fscanf (file, (*pTesr).CellCoo[i], (*pTesr).Dim);
      }

      else if (!strcmp (string, "*vol"))
      {
	ut_file_skip (file, 1);
	(*pTesr).CellVol = ut_alloc_1d ((*pTesr).CellQty + 1);

	for (i = 1; i <= (*pTesr).CellQty; i++)
	{
	  status = fscanf (file, "%lf", &(*pTesr).CellVol[i]);
	  if (status != 1)
	    abort ();
	}
      }

      else if (!strcmp (string, "*convexity"))
      {
	ut_file_skip (file, 1);
	(*pTesr).CellConvexity = ut_alloc_1d ((*pTesr).CellQty + 1);

	for (i = 1; i <= (*pTesr).CellQty; i++)
	{
	  status = fscanf (file, "%lf", &(*pTesr).CellConvexity[i]);
	  if (status != 1)
	    abort ();
	}
      }

      else if (!strcmp (string, "*crysym"))
      {
	ut_file_skip (file, 1);
	(*pTesr).CellCrySym = ut_alloc_1d_char (100);
	status = fscanf (file, "%s", (*pTesr).CellCrySym);
        if (status != 1)
          abort ();
	(*pTesr).CellCrySym = ut_realloc_1d_char ((*pTesr).CellCrySym,
                                                  strlen ((*pTesr).CellCrySym) + 1);
      }

      else
	ut_print_message (2, 2, "Failed to read tesr file.\n");
    }
  }

  ut_free_1d_char (string);
  ut_free_1d (des);

  return;
}

/* Tessellation exportation: foot */
void
neut_tesr_fscanf_foot (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

void
neut_tesr_fscanf_data (struct TESR *pTesr, char *dirname, int *bounds,
                       double *scale, char *format, FILE * file)

{
  int i;
  char c;
  FILE *file2 = NULL;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (10);
  char *tmp2 = ut_alloc_1d_char (1000);
  fpos_t pos;

  if (bounds && scale)
    ut_error_reportbug ();

  if (!bounds)
    (*pTesr).VoxCell = ut_alloc_3d_int ((*pTesr).size[0] + 2,
					(*pTesr).size[1] + 2,
					(*pTesr).size[2] + 2);
  else
    (*pTesr).VoxCell = ut_alloc_3d_int (bounds[1] - bounds[0] + 3,
					bounds[3] - bounds[2] + 3,
					bounds[5] - bounds[4] + 3);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (strcmp (tmp, "**data") != 0)
    abort ();

  do
  {
    fgetpos (file, &pos);
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  }
  while (c == ' ' || c == '\n' || c == '\t');

  fsetpos (file, &pos);

  if (c == '*')
    if (fscanf (file, "%s", tmp) != 1)
      abort ();

  if (!strcmp (tmp, "*file"))
  {
    filename = ut_alloc_1d_char (1000);
    if (fscanf (file, "%s", tmp2) != 1)
      abort ();
    sprintf (filename, "%s/%s", dirname, tmp2);
    file2 = ut_file_open (filename, "r");
  }
  else
    file2 = file;

  if (!scale && !bounds)
    neut_tesr_fscanf_data_default (pTesr, format, file2);
  else if (bounds)
    neut_tesr_fscanf_data_bounds (pTesr, bounds, format, file2);
  else if (scale)
    neut_tesr_fscanf_data_scale (pTesr, scale, format, file2);

  if (!strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (filename);
  }

  if (bounds)
    for (i = 0; i < 3; i++)
      (*pTesr).size[i] = bounds[2 * i + 1] - bounds[2 * i] + 1;

  // renumber and set (*pTesr).CellQty
  if (!(*pTesr).CellId || bounds || scale)
    neut_tesr_renumber_continuous (pTesr);

  /*
     int status = 1;
     for (i = 8; status > 0 && i >= 1; i--)
     {
     printf ("filtering %d: ", i);
     status = neut_tesr_filter_noisereduction (pTesr, 0, i, INT_MAX);
     printf ("%d\n", status);
     }
   */

  ut_free_1d_char (tmp);
  ut_free_1d_char (tmp2);

  return;
}

void
neut_tesr_fscanf_oridata (struct TESR *pTesr, char *dirname, int *bounds,
                          double *scale, char *format, FILE * file)
{
  char c;
  FILE *file2 = NULL;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (10);
  char *tmp2 = ut_alloc_1d_char (1000);
  fpos_t pos;
  char des[10];

  if (bounds || scale)
    ut_error_reportbug ();

  (*pTesr).VoxOri = ut_alloc_4d ((*pTesr).size[0] + 1,
                                 (*pTesr).size[1] + 1,
                                 (*pTesr).size[2] + 1, 4);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (strcmp (tmp, "**oridata") != 0)
    abort ();

  if (fscanf (file, "%s", des) != 1)
    abort ();

  do
  {
    fgetpos (file, &pos);
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  }
  while (c == ' ' || c == '\n' || c == '\t');

  fsetpos (file, &pos);

  if (c == '*')
    if (fscanf (file, "%s", tmp) != 1)
      abort ();

  if (!strcmp (tmp, "*file"))
  {
    filename = ut_alloc_1d_char (1000);
    if (fscanf (file, "%s", tmp2) != 1)
      abort ();
    sprintf (filename, "%s/%s", dirname, tmp2);
    file2 = ut_file_open (filename, "r");
  }
  else
    file2 = file;

  neut_tesr_fscanf_oridata_default (pTesr, des, format, file2);

  if (!strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (filename);
  }

  ut_free_1d_char (tmp);
  ut_free_1d_char (tmp2);

  return;
}
