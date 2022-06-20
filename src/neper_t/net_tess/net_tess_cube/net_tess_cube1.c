/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_cube_.h"

int
net_tess_cube (struct IN_T In, int level, char *morpho, struct MTESS *pMTess,
               struct TESS *Tess, int dtess, int dcell, int TessId,
               struct SEEDSET *SSet)
{
  int status, i, id;
  int *N = ut_alloc_1d_int (3);
  char *tmpc = ut_alloc_1d_char (1000);
  char *fct = NULL;
  int varqty;
  char **vals = NULL;
  int dim = (In.levelqty > 1) ? 3 : In.dim;
  struct TESS Dom;

  neut_tess_set_zero (&Dom);

  neut_tess_poly_tess (Tess[dtess], dcell, &Dom);

  ut_print_message (0, 2, "Running tessellation...\n");

  ut_string_function (morpho, &fct, NULL, &vals, &varqty);

  if (!strcmp (fct, "cube"))
  {
    if (varqty == 1)
    {
      sscanf (vals[0], "%d", N);
      ut_array_1d_int_set (N, 3, N[0]);
    }
    else if (varqty == 3)
      for (i = 0; i < 3; i++)
        sscanf (vals[i], "%d", N + i);
    else
      abort ();
  }
  else if (!strcmp (fct, "square"))
  {
    if (varqty == 1)
    {
      sscanf (vals[0], "%d", N);
      ut_array_1d_int_set_3 (N, N[0], N[0], 1);
    }
    else if (varqty == 2)
    {
      for (i = 0; i < 2; i++)
        sscanf (vals[i], "%d", N + i);
      N[2] = 1;
    }
    else
      abort ();
  }
  else
    abort ();

  int ***verid = ut_alloc_3d_int (N[0] + 1, N[1] + 1, N[2] + 1);
  int ****edgeid = ut_alloc_4d_int (3, N[0] + 1, N[1] + 1, N[2] + 1);
  int ****faceid = ut_alloc_4d_int (3, N[0] + 1, N[1] + 1, N[2] + 1);
  int ***polyid = ut_alloc_3d_int (N[0], N[1], N[2]);
  double **bbox = ut_alloc_2d (3, 2);
  neut_tess_bbox (Dom, bbox);

  if (level != 1 || dtess != 0 || dcell != 1)
    ut_print_neperbug ();

  net_tess_cube_ids (N, verid, edgeid, faceid, polyid);

  net_tess_cube_general (TessId, Tess + 1);

  net_tess_cube_cells (N, Tess + 1);
  net_tess_cube_vers (N, bbox, verid, Tess + 1);
  net_tess_cube_edges (N, verid, edgeid, Tess + 1);
  net_tess_cube_faces (N, verid, edgeid, faceid, Tess + 1);
  net_tess_cube_polys (N, faceid, polyid, Tess + 1);
  neut_tess_init_seeds_fromcell (Tess + 1);
  net_tess_cube_sset (Tess[1], SSet + 1);

  neut_tess_init_veredge (Tess + 1);
  neut_tess_init_edgeface (Tess + 1);
  net_tess_cube_facepoly (N, faceid, Tess + 1);

  neut_tess_init_domain_poly (Tess + 1, Dom, 1, NULL, NULL, NULL);

  if (dim == 2)
  {
    struct TESS T2;
    neut_tess_set_zero (&T2);

    status = neut_tess_domface_label_id (Tess[1], "z0", &id);

    if (status != 0)
      ut_print_neperbug ();

    neut_tess_domface_tess (Tess[1], id, &T2);
    neut_tess_tess (T2, Tess + 1);
    neut_tess_free (&T2);
  }

  ut_print_message (0, 2, "Generating crystal orientations...\n");
  net_ori (In, 1, *pMTess, Tess, SSet, 0, 1, SSet + 1, 3);

  ut_string_string (SSet[1].crysym, &(Tess[1].CellCrySym));

  ut_free_3d_int (&verid, N[0] + 1, N[1] + 1);
  ut_free_4d_int (&edgeid, 3, N[0] + 1, N[1] + 1);
  ut_free_4d_int (&faceid, 3, N[0] + 1, N[1] + 1);
  ut_free_3d_int (&polyid, N[0], N[1]);
  ut_free_1d_int (&N);
  ut_free_1d_char (&tmpc);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d (&bbox, 3);
  neut_tess_free (&Dom);

  return 0;
}
