/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

/* Tessellation exportation: head */
void
neut_tesr_fscanf_head (struct TESR *pTesr, double *bounds, int **pvoxbounds,
                       char **pversion, char **pdataformat, char **poridataformat, FILE *file)
{
  int i, status;
  char *string = ut_alloc_1d_char (1000);

  (*pdataformat) = ut_alloc_1d_char (10);

  if (!ut_file_string_scanandtest (file, "***tesr"))
    ut_print_message (2, 0,
                      "Input file is not a valid raster tessellation file.\n");

  if (!ut_file_string_scanandtest (file, "**format"))
    ut_print_message (2, 0,
                      "Input file is not a valid raster tessellation file.\n");

  status = fscanf (file, "%s", string);
  ut_string_string (string, pversion);

  if (!strcmp (*pversion, "2.0"))
  {
    if (fscanf (file, "%s", *pdataformat) != 1
        || (!strcmp (*pdataformat, "ascii") && !strncmp (*pdataformat, "bin", 3)))
      ut_print_message (2, 0,
                        "Input file is not a valid raster tessellation file.\n");
    ut_string_string (*pdataformat, poridataformat);
  }
  else if (!strcmp (*pversion, "2.1") || !strcmp (*pversion, "2.2"))
  {
  }
  else
    ut_print_message (2, 0, "Unknown version.\n");

  if (!ut_file_string_scanandtest (file, "**general"))
    ut_print_message (2, 0,
                      "Input file is not a valid raster tessellation file.\n");

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

    // backward compatibility
    else if (!strcmp (string, "*hasvoid"))
      ut_file_skip (file, 2);

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
      neut_tesr_coo_pos (*pTesr, coo, i ? -1 : 1, vox);
      (*pvoxbounds)[i] = vox[0];
      (*pvoxbounds)[i + 2] = vox[1];
      (*pvoxbounds)[i + 4] = vox[2];
    }

    ut_free_1d_int (&vox);
    ut_free_1d (&coo);
  }

  if (status != 1)
    ut_print_message (2, 0,
                      "Input file is not a valid raster tessellation file.\n");

  ut_free_1d_char (&string);

  return;
}

void
neut_tesr_fscanf_cell (struct TESR *pTesr, char *version, FILE * file)
{
  int i, status, id;
  char *string = ut_alloc_1d_char (1000);
  double *des = ut_alloc_1d (9);

  ut_file_nextstring (file, string);

  if (!strcmp (string, "**cell"))
  {
    ut_file_skip (file, 1);

    (*pTesr).CellQty = 0;
    if (ut_file_nextstring (file, string) == 1 && string[0] != '*')
      if (fscanf (file, "%d", &(*pTesr).CellQty) != 1)
        abort ();

    while (ut_file_nextstring (file, string) == 1
           && strncmp (string, "**", 2))
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
        int major, minor;

        ut_string_version (version, &major, &minor, NULL);

        ut_file_skip (file, 1);
        (*pTesr).CellOri = ut_alloc_2d ((*pTesr).CellQty + 1, 4);
        (*pTesr).CellOriDes = ut_alloc_1d_char (20);
        if (fscanf (file, "%s", (*pTesr).CellOriDes) != 1)
          abort ();

        if (major <= 2 && minor <= 1)
        {
          ut_print_message (1, 3, "File version <= 2.1.  Fixing orientation convention...\n");
          neut_ori_des_fixconvention (&(*pTesr).CellOriDes);
        }

        neut_ori_fscanf (file, (*pTesr).CellOriDes, "ascii", (*pTesr).CellOri + 1,
                         NULL, (*pTesr).CellQty, NULL);
      }

      else if (!strcmp (string, "*orispread") || !strcmp (string, "*orispread"))
      {
        ut_file_skip (file, 1);
        char *tmp = ut_alloc_1d_char (1000);
        (*pTesr).CellOriDistrib = ut_alloc_1d_pchar ((*pTesr).CellQty + 1);
        (*pTesr).CellOriDistrib[0] = NULL;

        for (i = 1; i <= (*pTesr).CellQty; i++)
        {
          if (fscanf (file, "%s", tmp) != 1)
            abort ();
          ut_string_string (tmp, (*pTesr).CellOriDistrib + i);
        }

        ut_free_1d_char (&tmp);
      }

      else if (!strcmp (string, "*coo"))
        ut_file_skip (file, (*pTesr).CellQty * (*pTesr).Dim + 1);

      else if (!strcmp (string, "*vol") || !strcmp (string, "*convexity"))
        ut_file_skip (file, (*pTesr).CellQty + 1);

      else if (!strcmp (string, "*group"))
      {
        ut_file_skip (file, 1);
        (*pTesr).CellGroup = ut_alloc_1d_int ((*pTesr).CellQty + 1);
        ut_array_1d_int_fscanf (file, (*pTesr).CellGroup + 1,
                                (*pTesr).CellQty);
      }

      else if (!strcmp (string, "*crysym"))
      {
        ut_file_skip (file, 1);
        (*pTesr).CellCrySym = ut_alloc_1d_char (100);
        status = fscanf (file, "%s", (*pTesr).CellCrySym);
        if (status != 1)
          abort ();
        (*pTesr).CellCrySym =
          ut_realloc_1d_char ((*pTesr).CellCrySym,
                              strlen ((*pTesr).CellCrySym) + 1);
      }

      else
        ut_print_message (2, 2, "Failed to read tesr file.\n");
    }
  }

  ut_free_1d_char (&string);
  ut_free_1d (&des);

  return;
}

/* Tessellation exportation: foot */
void
neut_tesr_fscanf_foot (FILE * file)
{
  if (!ut_file_string_scanandtest (file, "***end"))
    abort ();

  return;
}

void
neut_tesr_fscanf_data (struct TESR *pTesr, char *dirname, int *bounds,
                       double *scale, char *version, char **pdataformat, FILE * file)
{
  int i;
  FILE *file2 = NULL;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (10);
  char *tmp2 = ut_alloc_1d_char (1000);

  if (bounds && scale)
    ut_print_neperbug ();

  if (!bounds && !(*pTesr).VoxCell)
    (*pTesr).VoxCell =
      ut_alloc_3d_int ((*pTesr).size[0] + 2, (*pTesr).size[1] + 2,
                       (*pTesr).size[2] + 2);
  else if (!(*pTesr).VoxCell)
    (*pTesr).VoxCell =
      ut_alloc_3d_int (bounds[1] - bounds[0] + 3, bounds[3] - bounds[2] + 3,
                       bounds[5] - bounds[4] + 3);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (strcmp (tmp, "**data") != 0)
    abort ();

  if (strcmp (version, "2.0"))
  {
    (*pdataformat) = ut_alloc_1d_char (10);

    if (fscanf (file, "%s", *pdataformat) != 1
        || (!strcmp (*pdataformat, "ascii") && !strncmp (*pdataformat, "bin", 3)))
      abort ();
  }

  ut_file_scan_file (file, dirname, &file2, &filename);

  if (!scale && !bounds)
    neut_tesr_fscanf_data_default (pTesr, *pdataformat, filename != NULL, file2, (*pTesr).VoxCell);
  else if (bounds)
    neut_tesr_fscanf_data_bounds (pTesr, bounds, *pdataformat, file2, (*pTesr).VoxCell);
  else if (scale)
    neut_tesr_fscanf_data_scale (pTesr, scale, *pdataformat, filename != NULL, file2, (*pTesr).VoxCell);

  if (!strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (&filename);
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

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);

  return;
}

void
neut_tesr_fscanf_oridata (struct TESR *pTesr, char *dirname, int *bounds,
                          double *scale, char *version, char **poridataformat,
                          FILE * file)
{
  FILE *file2 = NULL;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (10);
  char *tmp2 = ut_alloc_1d_char (1000);
  char *des = ut_alloc_1d_char (100);

  if (bounds || scale)
    ut_print_neperbug ();

  (*pTesr).VoxOri =
    ut_alloc_4d ((*pTesr).size[0] + 1, (*pTesr).size[1] + 1,
                 (*pTesr).size[2] + 1, 4);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (strcmp (tmp, "**oridata") != 0)
    abort ();

  if (fscanf (file, "%s", des) != 1)
    abort ();

  if (strcmp (version, "2.0"))
  {
    (*poridataformat) = ut_alloc_1d_char (10);

    if (fscanf (file, "%s", *poridataformat) != 1
        || (!strcmp (*poridataformat, "ascii") && !strncmp (*poridataformat, "bin", 3)))
      abort ();
  }

  ut_file_scan_file (file, dirname, &file2, &filename);

  neut_tesr_fscanf_oridata_default (pTesr, des, *poridataformat, 0, file2);

  if (!strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (&filename);
  }

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);
  ut_free_1d_char (&des);

  return;
}

void
neut_tesr_fscanf_oridef (struct TESR *pTesr, char *dirname, int *bounds,
                          double *scale, FILE * file)
{
  FILE *file2 = NULL;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (10);
  char *tmp2 = ut_alloc_1d_char (1000);
  char *des = ut_alloc_1d_char (100);
  char *format = ut_alloc_1d_char (10);

  if (bounds || scale)
    ut_print_neperbug ();

  (*pTesr).VoxOriDef =
    ut_alloc_3d_int ((*pTesr).size[0] + 1, (*pTesr).size[1] + 1,
                     (*pTesr).size[2] + 1);

  if (!ut_file_string_scanandtest (file, "**oridef"))
    ut_print_message (2, 0,
                      "Input file is not a valid raster tessellation file.\n");

  if (fscanf (file, "%s", format) != 1)
    abort ();

  ut_file_scan_file (file, dirname, &file2, &filename);

  if (!scale && !bounds)
    neut_tesr_fscanf_data_default (pTesr, format, filename != NULL, file2, (*pTesr).VoxOriDef);
  else if (bounds)
    neut_tesr_fscanf_data_bounds (pTesr, bounds, format, file2, (*pTesr).VoxOriDef);
  else if (scale)
    neut_tesr_fscanf_data_scale (pTesr, scale, format, filename != NULL, file2, (*pTesr).VoxOriDef);

  if (!strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (&filename);
  }

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);
  ut_free_1d_char (&des);
  ut_free_1d_char (&format);

  return;
}
