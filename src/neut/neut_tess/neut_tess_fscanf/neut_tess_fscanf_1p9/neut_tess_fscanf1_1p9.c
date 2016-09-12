/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fscanf_1p9_.h"

void
neut_tess_fscanf_1p9 (FILE * file, struct TESS *pTess)
{
  if ((*pTess).PolyQty > 0)
    neut_tess_free (pTess);

  neut_tess_fscanf_head_1p9 (pTess, file);
  neut_tess_fscanf_ver_1p9 (pTess, file);
  neut_tess_fscanf_edge_1p9 (pTess, file);
  neut_tess_fscanf_face_1p9 (pTess, file);
  neut_tess_fscanf_poly_1p9 (pTess, file);
  neut_tess_fscanf_foot_1p9 (file);

  neut_tess_init_edgelength (pTess);

  return;
}

void
neut_tess_name_fscanf_1p9 (char *name, struct TESS *pTess)
{
  FILE *file = ut_file_open (name, "r");
  neut_tess_fscanf_1p9 (file, pTess);
  ut_file_close (file, name, "r");

  return;
}
