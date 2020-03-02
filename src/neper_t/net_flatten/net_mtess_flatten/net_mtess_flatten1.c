/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_.h"

void
net_mtess_flatten (struct MTESS MTess, struct TESS *Tess, int TessId,
                   struct TESS *pFTess)
{
  int i;
  struct TESSE *TessE = calloc (MTess.TessQty + 1, sizeof (struct TESSE));
  struct FLATTEN Flatten;
  int *CTessIds = NULL, CTessQty;
  struct TESS FTess;

  neut_tess_set_zero (&FTess);

  neut_mtess_tess_poly_childtesses (MTess, Tess, TessId, &CTessIds,
                                    &CTessQty);

  neut_flatten_set_zero (&Flatten);

  /*
     FILE* file = NULL;
     char* filename = ut_alloc_1d_char (100);
     for (i = 1; i <= MTess.TessQty; i++)
     {
     sprintf (filename, "tess%d.geo", i);
     file = ut_file_open (filename, "w");
     neut_tess_fprintf_gmsh (file, Tess[i]);
     ut_file_close (file, filename, "w");
     }
   */

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tesse_set_zero (TessE + i);

  net_mtess_flatten_gen (Tess, TessId, &FTess, &Flatten);

  net_mtess_flatten_ver (MTess, Tess, TessId, CTessIds, CTessQty, &FTess,
                         TessE, &Flatten);

  net_mtess_flatten_edge (MTess, Tess, TessId, CTessIds, CTessQty, &FTess,
                          TessE, &Flatten);

  neut_flatten_init_domstuff_fromver (&Flatten, Tess[TessId]);
  neut_flatten_init_domstuff_fromedge (&Flatten, Tess[TessId]);

  net_mtess_flatten_face (MTess, Tess, TessId, CTessIds, CTessQty, &FTess,
                          &TessE, &Flatten);

  neut_flatten_init_domstuff_fromface (&Flatten);

  net_mtess_flatten_poly (MTess, Tess, TessId, CTessIds, CTessQty, &FTess,
                          &TessE, &Flatten);
  net_mtess_flatten_cell (Tess, CTessIds, CTessQty, &FTess);

  net_mtess_flatten_dom (Tess, TessId, &FTess, TessE);

  /*
     strcpy (filename, "tessf.geo");
     file = ut_file_open (filename, "w");
     neut_tess_fprintf_gmsh (file, *pFTess);
     ut_file_close (file, filename, "w");

     strcpy (filename, "tessf.tess");
     file = ut_file_open (filename, "w");
     neut_tess_fprintf (file, *pFTess);
     ut_file_close (file, filename, "w");
   */

#ifdef DEVEL_DEBUGGING_TEST
  if (neut_tess_test (FTess, 0) != 0)
    ut_print_message (2, 2, "The tessellation is not valid.\n");
#endif

  neut_tess_tess (FTess, pFTess);

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tesse_free (TessE + i);
  free (TessE);

  ut_free_1d_int (&CTessIds);
  neut_tess_free (&FTess);

  neut_flatten_free (&Flatten);

  return;
}
