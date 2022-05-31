/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_ori_.h"

void
neut_mesh_elts_olset (struct NODES Nodes, struct MESH Mesh,
                      double **EltOri, char *crysym, int *elts, int eltqty, struct OL_SET *pOSet)
{
  int i, elt;

  *pOSet = ol_set_alloc (eltqty, crysym);

  for (i = 0; i < eltqty; i++)
  {
    elt = elts[i];
    neut_mesh_elt_size (Nodes, Mesh, elt, (*pOSet).weight + i);
    ol_q_memcpy (EltOri ? EltOri[elt] : Mesh.EltOri[elt], (*pOSet).q[i]);
  }

  if (crysym)
    ut_string_string (crysym, &(*pOSet).crysym);

  return;
}

void
neut_mesh_elsets_olset (struct NODES Nodes, struct MESH Mesh,
                        double **ElsetOri, char *crysym, int *elsets,
                        int elsetqty, struct OL_SET *pOSet)
{
  int i, elset;

  *pOSet = ol_set_alloc (elsetqty, crysym);

  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets ? elsets[i] : i + 1;
    neut_mesh_elset_size (Nodes, Mesh, elset, (*pOSet).weight + i);
    ol_q_memcpy (ElsetOri ? ElsetOri[elset] : Mesh.ElsetOri[elset], (*pOSet).q[i]);
  }

  if (crysym)
    ut_string_string (crysym, &(*pOSet).crysym);

  return;
}

void
neut_mesh_elts_orimean (struct NODES Nodes, struct MESH Mesh,
                         int *elts, int eltqty, double **EltOri,
                         char *crysym, double *mean)
{
  struct OL_SET OSet;

  neut_mesh_elts_olset (Nodes, Mesh, EltOri, crysym, elts, eltqty, &OSet);

  ol_set_mean_iter (OSet, mean);

  ol_set_free (OSet);

  return;
}

void
neut_mesh_elts_orianiso (struct NODES Nodes, struct MESH Mesh,
                         int *elts, int eltqty, double **EltOri,
                         char *crysym, double **evect, double *eval)
{
  struct OL_SET OSet;

  neut_mesh_elts_olset (Nodes, Mesh, EltOri, crysym, elts, eltqty, &OSet);

  ol_set_disoriset (OSet, &OSet);

  ol_set_aniso (OSet, evect, eval);

  ol_set_free (OSet);

  return;
}

void
neut_mesh_elt_ori (struct MESH Mesh, int id, double *ori)
{
  if (Mesh.EltOri)
    ol_q_q (Mesh.EltOri[id], ori);

  else
    ol_q_q (Mesh.ElsetOri[Mesh.EltElset[id]], ori);

  return;
}
