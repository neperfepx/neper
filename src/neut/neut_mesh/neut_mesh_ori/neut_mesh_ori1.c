/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_ori_.h"

void
neut_mesh_elset_orianiso (struct NODES Nodes, struct MESH Mesh, double **EltOri,
                          char *crysym, int elset, double **evect, double *eval)
{
  struct OL_SET Set;

  neut_mesh_elset_olset (Nodes, Mesh, EltOri, crysym, elset, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso (Set, evect, eval);

  ol_set_free (Set);

  return;
}
