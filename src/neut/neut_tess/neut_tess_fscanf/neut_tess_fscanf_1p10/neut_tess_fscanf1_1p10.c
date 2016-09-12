/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_1p10_.h"

void
neut_tess_fscanf_1p10 (FILE * file, struct TESS *pTess)
{
  neut_tess_fscanf_verbosity_1p10 (file, pTess, 0);
}

void
neut_tess_fscanf_verbosity_1p10 (FILE * file, struct TESS *pTess,
				 int verbosity)
{
  char *version = ut_alloc_1d_char (10);

  if ((*pTess).PolyQty > 0)
    neut_tess_free (pTess);

  neut_tess_fscanf_version_1p10 (file, version);
  neut_tess_fscanf_head_1p10 (pTess, file);
  neut_tess_fscanf_ver_1p10 (pTess, file);
  neut_tess_fscanf_edge_1p10 (pTess, file);
  neut_tess_fscanf_face_1p10 (pTess, file);
  neut_tess_fscanf_poly_1p10 (pTess, file);
  neut_tess_fscanf_domain_1p10 (pTess, file);
  neut_tess_init_domain_label (pTess);
  neut_tess_fscanf_foot_1p10 (file);
  neut_tess_init_edgelength (pTess);

  if (neut_tess_test (*pTess, verbosity) != 0)
  {
    ut_print_message (2, 0,
		      "Tessellation checking failed - tessellation is not valid.\n");
    if (!verbosity)
      ut_print_message (2, 0, "Debug with option -checktess of neper -T.\n");

    abort ();
  }
  else if (verbosity)
    ut_print_message (2, 0, "Tessellation checking succeeded.\n");

  ut_free_1d_char (version);

  return;
}

void
neut_tess_name_fscanf_1p10 (char *name, struct TESS *pTess)
{
  FILE *file = ut_file_open (name, "r");
  neut_tess_fscanf_1p10 (file, pTess);
  ut_file_close (file, name, "r");

  return;
}
