/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_extrude_.h"

void
neut_tess_extrude (struct TESS Tess, double d, struct TESS *pT)
{
  if (Tess.Dim != 2)
    ut_error_reportbug ();

  neut_tess_free (pT);

  neut_tess_extrude_gen (Tess, pT);

  neut_tess_extrude_cell (Tess, d, pT);

  neut_tess_extrude_ver (Tess, d, pT);

  neut_tess_extrude_edge (Tess, pT);

  neut_tess_extrude_face (Tess, d, pT);

  neut_tess_extrude_poly (Tess, pT);

  neut_tess_init_veredge (pT);
  neut_tess_init_edgeface (pT);
  neut_tess_init_facepoly (pT);

  neut_tess_extrude_domain (Tess, d, pT);

  return;
}
