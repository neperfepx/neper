/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

void
neut_tesr_name_fscanf (char *name, struct TESR *pTesr)
{
  FILE *file = NULL;
  char **list = NULL;
  int i, j, qty, qty2;
  char **string = ut_alloc_2d_char (6, 1000);
  double *bounds = NULL;
  double *scale = NULL;

  ut_string_separate (name, NEUT_SEP_DEP, &list, &qty);

  for (i = 1; i < qty; i++)
  {
    if (!strncmp (list[i], "crop", 4))
    {
      ut_string_fnr (list[i], ',', ' ');
      list[i][strlen (list[i]) - 1] = '\0';

      bounds = ut_alloc_1d (6);
      qty2 = sscanf (list[i], "crop(%s%s%s%s%s%s",
		    string[0], string[1], string[2],
		    string[3], string[4], string[5]);

      for (j = 0; j < qty2; j++)
	ut_math_eval (string[j], 0, NULL, NULL, &(bounds[j]));
      for (j = qty2; j < 6; j++)
	bounds[j] = -1;
    }
    else if (!strncmp (list[i], "rasterscale", 11))
    {
      ut_string_fnr (list[i], ',', ' ');
      list[i][strlen (list[i]) - 1] = '\0';

      scale = ut_alloc_1d (3);
      qty2 = sscanf (list[i], "rasterscale(%s%s%s", string[0], string[1], string[2]);

      if (qty2 == 1)
      {
	ut_math_eval (string[0], 0, NULL, NULL, scale);
	ut_array_1d_set (scale, 3, scale[0]);
      }
      else
      {
	for (j = 0; j < qty2; j++)
	  ut_math_eval (string[j], 0, NULL, NULL, scale + j);
	for (j = qty2; j < 3; j++)
	  scale[j] = 1;
      }
    }
    else
      ut_print_message (2, 0, "Failed to process expression `%s'.\n", name);
  }

  file = ut_file_open (list[0], "r");
  neut_tesr_fscanf (file, dirname (list[0]), bounds, scale, pTesr);
  ut_file_close (file, list[0], "r");

  ut_free_1d (bounds);
  ut_free_1d (scale);
  ut_free_2d_char (string, 6);
  ut_free_2d_char (list, qty);

  return;
}

void
neut_tesr_fscanf (FILE * file, char *dirname, double *bounds, double *scale, struct TESR *pTesr)
{
  char *format = NULL, string[1000];
  int *voxbounds = NULL;

  neut_tesr_free (pTesr);

  neut_tesr_fscanf_head (pTesr, bounds, &voxbounds, &format, file);
  neut_tesr_fscanf_cell (pTesr, file);
  neut_tesr_fscanf_data (pTesr, dirname, voxbounds, scale, format, file);

  ut_file_nextstring (file, string);
  if (!strcmp (string, "**oridata"))
    neut_tesr_fscanf_oridata (pTesr, dirname, voxbounds, scale, format, file);
  neut_tesr_fscanf_foot (file);

  neut_tesr_init_cellbbox (pTesr);

  if (bounds || scale)
    if ((*pTesr).size[2] == 1)
      (*pTesr).Dim = 2;

  ut_free_1d_char (format);
  ut_free_1d_int (voxbounds);

  return;
}
