/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_obj_.h"

void
neut_tess_fscanf_obj (FILE *file, struct TESS *pTess)
{
  int i, nblines, line_count;
  char *entity = NULL, *line = ut_alloc_1d_char (10000);
  char *prefix = ut_alloc_1d_char (10);
  int groupqty = 0;
  char **groups = ut_alloc_1d_pchar (1);
  int SFaceVerQty = 0;
  int **SFaceVerNb = ut_alloc_1d_pint (1);
  char *prev = ut_alloc_1d_char (100);

  neut_tess_fscanf_obj_pre (pTess);

  nblines = ut_file_nblines_pointer (file);

  ut_print_message (0, 3, "Parsing file... ");

  line_count = 0;
  ut_print_progress (stdout, line_count, nblines, "%.0f%%", prev);

  while (fgets(line, 10000, file))
  {
    line_count++;

    for (i = 0; i < (int) strlen (line); i++)
      if (line[i] == '\n')
        line[i] = '\0';

    if (ut_string_nbwords (line) > 0)
    {
      sscanf (line, "%s", prefix);

      if (!strcmp (prefix, "#")) {}
      else if (!strcmp (prefix, "mtllib")) {}
      else if (!strcmp (prefix, "usemtl")) {}
      else if (!strcmp (prefix, "vt")) {}
      else if (!strcmp (prefix, "vn")) {}

      else if (!strcmp (prefix, "g"))
        neut_tess_fscanf_obj_scangroup (line, &groups, &groupqty, pTess);

      else if (!strcmp (prefix, "v"))
        neut_tess_fscanf_obj_scanver (line, pTess);

      else if (!strcmp (prefix, "f"))
        neut_tess_fscanf_obj_scanface (line, &SFaceVerNb, &SFaceVerQty, pTess);

      else
        ut_print_message (2, 2, "Failed to process `%s'\n", line);
    }

    ut_print_progress (stdout, line_count, nblines, "%.0f%%", prev);
  }

  if ((*pTess).PolyQty)
  {
    // declaring tessellation as 2D or 3D
    if (ut_array_1d_int_min ((*pTess).PolyFaceQty + 1, (*pTess).PolyQty) > 1)
    {
      (*pTess).Dim = 3;
      (*pTess).CellQty = (*pTess).PolyQty;
    }
    else
    {
      (*pTess).Dim = 2;
      (*pTess).CellQty = (*pTess).FaceQty;

      ut_free_2d_int (&(*pTess).PolyFaceNb, (*pTess).PolyQty + 1);
      ut_free_2d_int (&(*pTess).PolyFaceOri, (*pTess).PolyQty + 1);
      ut_free_1d_int (&(*pTess).PolyFaceQty);
      ut_free_2d_int (&(*pTess).FacePoly, (*pTess).FaceQty + 1);
      (*pTess).PolyQty = 0;
    }
  }

  // working on the vertices, faces and cells
  ut_print_message (0, 3, "Recovering edges... ");
  neut_tess_init_edges_fromversandfaces (pTess);

  if ((*pTess).PolyQty > 0)
  {
    ut_print_message (0, 3, "Recovering face geometry... ");
    neut_tess_init_polyfaceori (pTess);
  }

  ut_free_1d_char (&entity);
  ut_free_1d_char (&line);
  ut_free_1d_char (&prefix);
  ut_free_2d_char (&groups, groupqty);
  ut_free_2d_int (&SFaceVerNb, SFaceVerQty);
  ut_free_1d_char (&prev);

  (*pTess).Dim = 3;

  return;
}

void
neut_tess_fnscanf_obj (char *name, struct TESS *pTess)
{
  FILE *file = NULL;

  file = ut_file_open (name, "r");
  neut_tess_fscanf_obj (file, pTess);
  ut_file_close (file, name, "r");

  return;
}
