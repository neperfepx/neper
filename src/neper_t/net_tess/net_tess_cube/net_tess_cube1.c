/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_cube_.h"

int
net_tess_cube (char *string, int dim, struct TESS PTess, int poly,
	       int TessId, struct TESS *pTess,
	       struct SEEDSET *pSSet)
{
  int status, i, id;
  int *N = ut_alloc_1d_int (3);
  char *tmpc = ut_alloc_1d_char (1000);
  char *fct = NULL;
  int varqty;
  char **vars = NULL, **vals = NULL;

  ut_string_function_separate (string, &fct, &vars, &vals, &varqty);

  if (!strcmp (fct, "cube"))
  {
    if (varqty == 1)
    {
      sscanf (vars[0], "%d", N);
      ut_array_1d_int_set (N, 3, N[0]);
    }
    else if (varqty == 3)
      for (i = 0; i < 3; i++)
	sscanf (vars[i], "%d", N + i);
    else
      abort ();
  }
  else if (!strcmp (fct, "square"))
  {
    if (varqty == 1)
    {
      sscanf (vars[0], "%d", N);
      ut_array_1d_int_set_3 (N, N[0], N[0], 1);
    }
    else if (varqty == 2)
    {
      for (i = 0; i < 2; i++)
	sscanf (vars[i], "%d", N + i);
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

  if (poly != 1)
    ut_error_reportbug ();

  net_tess_cube_ids (N, verid, edgeid, faceid, polyid);

  net_tess_cube_general (TessId, pTess);

  net_tess_cube_cells (N, pTess);
  net_tess_cube_vers (N, PTess, verid, pTess);
  net_tess_cube_edges (N, verid, edgeid, pTess);
  net_tess_cube_faces (N, verid, edgeid, faceid, pTess);
  net_tess_cube_polys (N, faceid, polyid, pTess);
  neut_tess_init_seeds_fromcell (pTess);
  net_tess_cube_sset (*pTess, pSSet);

  neut_tess_init_veredge (pTess);
  neut_tess_init_edgeface (pTess);
  net_tess_cube_facepoly (N, faceid, pTess);

  neut_tess_init_domain_poly (pTess, PTess, 1, NULL, NULL, NULL);

  if (dim == 2)
  {
    struct TESS T2;
    neut_tess_set_zero (&T2);

    status = neut_tess_domface_label_id (*pTess, "z0", &id);

    if (status != 0)
      ut_error_reportbug ();

    neut_tess_domface_tess (*pTess, id, &T2);
    neut_tess_tess (T2, pTess);
    neut_tess_free (&T2);
  }

  ut_free_3d_int (verid, N[0] + 1, N[1] + 1);
  ut_free_4d_int (edgeid, 3, N[0] + 1, N[1] + 1);
  ut_free_4d_int (faceid, 3, N[0] + 1, N[1] + 1);
  ut_free_3d_int (polyid, N[0], N[1]);
  ut_free_1d_int (N);
  ut_free_1d_char (tmpc);
  ut_free_1d_char (fct);
  ut_free_2d_char (vars, varqty);
  ut_free_2d_char (vals, varqty);

  return 0;
}
