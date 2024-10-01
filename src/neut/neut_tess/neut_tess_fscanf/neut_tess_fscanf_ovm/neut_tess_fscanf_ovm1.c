/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_ovm_.h"

void
neut_tess_fscanf_ovm (FILE *file, struct TESS *pTess)
{
  (*pTess).Dim = 3;
  (*pTess).Level = 1;
  (*pTess).TessId = 1;
  ut_string_string ("standard", &(*pTess).Type);

  neut_tess_fscanf_ovm_header (file);

  neut_tess_fscanf_ovm_ver (file, pTess);

  neut_tess_fscanf_ovm_edge (file, pTess);

  neut_tess_fscanf_ovm_face (file, pTess);

  neut_tess_fscanf_ovm_poly (file, pTess);

  (*pTess).CellQty = (*pTess).PolyQty;

  neut_tess_init_domain (pTess);

  neut_tess_init_verdom (pTess);
  neut_tess_init_edgedom (pTess);
  neut_tess_init_facedom (pTess);

  return;
}

void
neut_tess_fnscanf_ovm (char *name, struct TESS *pTess)
{
  FILE *file = NULL;

  file = ut_file_open (name, "r");
  neut_tess_fscanf_ovm (file, pTess);
  ut_file_close (file, name, "r");

  return;
}
