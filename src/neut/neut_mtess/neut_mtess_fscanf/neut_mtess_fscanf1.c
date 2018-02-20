/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fscanf_.h"

void
neut_mtess_fscanf (FILE * file, struct MTESS *pMTess, struct TESS **pTess)
{
  int i, j, tess, status;

  // status == 0 if mtess and 1 if tess
  status = neut_mtess_fscanf_head (pMTess, file);

  if ((*pMTess).LevelQty > 1)
    ut_print_message (0, 3, "multilevel tessellation (%d levels).\n",
		      (*pMTess).LevelQty);

  (*pTess) = calloc ((*pMTess).TessQty + 1, sizeof (struct TESS));

  for (i = status; i <= (*pMTess).TessQty; i++)
    neut_tess_fscanf (file, &((*pTess)[i]));

  // Setting TessId and Level in the Tessellations.  Should be read in
  // neut_tess_fscanf (i.e. written to the .tess) instead?
  for (i = status; i <= (*pMTess).TessQty; i++)
    (*pTess)[i].TessId = i;

  for (i = 1; i <= (*pMTess).LevelQty; i++)
    for (j = 1; j <= (*pMTess).LevelTessQty[i]; j++)
    {
      tess = (*pMTess).LevelTess[i][j];
      (*pTess)[tess].Level = i;
    }

  (*pMTess).TessDQty = ut_array_1d_int_sum ((*pMTess).LevelTessQty + 1,
					    (*pMTess).LevelQty - 1);

  return;
}

void
neut_mtess_name_fscanf (char *name, struct MTESS *pMTess, struct TESS **pTess)
{
  FILE *file = NULL;
  char **list = NULL;
  int i, j, qty, qty2;
  char **string = ut_alloc_2d_char (6, 1000);
  int *bounds = NULL;
  double *scale = NULL;

  ut_string_separate (name, NEUT_SEP_DEP, &list, &qty);

  file = ut_file_open (list[0], "r");
  neut_mtess_fscanf (file, pMTess, pTess);
  ut_file_close (file, list[0], "r");

  for (i = 1; i < qty; i++)
  {
    if (!strncmp (list[i], "scale", 5))
    {
      ut_string_fnr (list[i], ',', ' ');
      list[i][strlen (list[i]) - 1] = '\0';

      scale = ut_alloc_1d (3);
      qty2 = sscanf (list[i], "scale(%s%s%s", string[0], string[1], string[2]);

      if (qty2 == 1)
      {
	ut_math_eval (string[0], 0, NULL, NULL, &(scale[0]));
	ut_array_1d_set (scale, 3, scale[0]);
      }
      else
      {
	for (j = 0; j < qty2; j++)
	  ut_math_eval (string[j], 0, NULL, NULL, &(scale[j]));
	for (j = qty2; j < 3; j++)
	  scale[j] = 1;
      }

      neut_tess_scale ((*pTess) + 1, scale[0], scale[1], scale[2]);
    }
    else
      ut_print_message (2, 0, "Failed to process expression `%s'.\n", name);
  }

  ut_free_1d_int (bounds);
  ut_free_1d (scale);
  ut_free_2d_char (string, 6);
  ut_free_2d_char (list, qty);

  return;
}
