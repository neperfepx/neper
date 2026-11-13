/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_.h"

void
net_mtess_flatten (struct MTESS MTess, struct TESS *Tess, int TessId,
                   struct TESS *pFTess)
{
  int i, periodic;
  struct TESSE *TessE = NULL;
  struct FLATTEN Flatten;
  int *CTessIds = NULL, CTessQty;
  int *VTessIds = NULL, VTessQty = 0;
  struct TESS FTess;
  struct MTESS MTess2 = MTess;
  struct TESS *Tess2 = Tess;

  neut_tess_set_zero (&FTess);

  // periodic tessellation: adding virtual child tessellations
  periodic = Tess[TessId].Type && !strcmp (Tess[TessId].Type, "periodic");
  if (periodic)
    net_mtess_flatten_per_init (MTess, Tess, TessId, &MTess2, &Tess2,
                                &VTessIds, &VTessQty);

  TessE = calloc (MTess2.TessQty + 1, sizeof (struct TESSE));

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

  for (i = 0; i <= MTess2.TessQty; i++)
    neut_tesse_set_zero (TessE + i);

  net_mtess_flatten_gen (Tess2, TessId, &FTess, &Flatten);

  net_mtess_flatten_ver (MTess2, Tess2, TessId, CTessIds, CTessQty, VTessIds,
                         VTessQty, &FTess, TessE, &Flatten);

  net_mtess_flatten_edge (MTess2, Tess2, TessId, CTessIds, CTessQty,
                          VTessIds, VTessQty, &FTess, TessE, &Flatten);

  neut_flatten_init_domstuff_fromver (&Flatten, Tess2[TessId]);
  neut_flatten_init_domstuff_fromedge (&Flatten, Tess2[TessId]);

  net_mtess_flatten_face (MTess2, Tess2, TessId, CTessIds, CTessQty,
                          VTessIds, VTessQty, &FTess, &TessE, &Flatten);

  neut_flatten_init_domstuff_fromface (&Flatten);

  net_mtess_flatten_poly (MTess2, Tess2, TessId, CTessIds, CTessQty, &FTess,
                          &TessE, &Flatten);
  net_mtess_flatten_cell (Tess2, CTessIds, CTessQty, &FTess);

  net_mtess_flatten_dom (Tess2, TessId, &FTess, TessE);

  if (periodic)
    net_mtess_flatten_per (MTess2, Tess2, TessId, VTessIds, VTessQty, TessE,
                           &FTess);

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

  for (i = 0; i <= MTess2.TessQty; i++)
    neut_tesse_free (TessE + i);
  free (TessE);

  if (periodic)
    net_mtess_flatten_per_free (MTess, TessId, &MTess2, &Tess2, &VTessIds,
                                VTessQty);

  neut_tess_tess (FTess, pFTess);

  ut_free_1d_int (&CTessIds);
  neut_tess_free (&FTess);

  neut_flatten_free (&Flatten);

  return;
}
