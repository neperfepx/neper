/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_ori_.h"

void
neut_mesh_elts_olset (struct NODES Nodes, struct MESH Mesh,
                      int *elts, int eltqty, struct OL_SET *pOSet)
{
  int i, elt;

  printf ("eltqty = %d\n", eltqty);
  printf ("Mesh.ElsetCrySym = %s\n", Mesh.ElsetCrySym);
  *pOSet = ol_set_alloc (eltqty, Mesh.ElsetCrySym);

  for (i = 0; i < eltqty; i++)
  {
    elt = elts ? elts[i] : i + 1;
    neut_mesh_elt_size (Nodes, Mesh, elt, (*pOSet).weight + i);
  }

  if (Mesh.SimEltOri)
    for (i = 0; i < eltqty; i++)
    {
      elt = elts ? elts[i] : i + 1;
      ol_q_memcpy (Mesh.SimEltOri[elt], (*pOSet).q[i]);
    }
  else if (Mesh.pSim)
  {
    neut_mesh_init_simeltori (&Mesh);

    for (i = 0; i < eltqty; i++)
    {
      elt = elts ? elts[i] : i + 1;
      ol_q_memcpy (Mesh.SimEltOri[elt], (*pOSet).q[i]);
    }
  }
  else if (Mesh.EltOri)
    for (i = 0; i < eltqty; i++)
    {
      elt = elts ? elts[i] : i + 1;
      ol_q_memcpy (Mesh.EltOri[elt], (*pOSet).q[i]);
    }
  else
    for (i = 0; i < eltqty; i++)
    {
      elt = elts ? elts[i] : i + 1;
      ol_q_memcpy (Mesh.ElsetOri[Mesh.EltElset[elt]], (*pOSet).q[i]);
    }

  if (Mesh.ElsetCrySym)
    ut_string_string (Mesh.ElsetCrySym, &(*pOSet).crysym);

  return;
}

void
neut_mesh_elsets_olset (struct NODES Nodes, struct MESH Mesh,
                        double **ElsetOri, int *elsets,
                        int elsetqty, struct OL_SET *pOSet)
{
  int i, elset;

  *pOSet = ol_set_alloc (elsetqty, Mesh.ElsetCrySym);

  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets ? elsets[i] : i + 1;
    neut_mesh_elset_size (Nodes, Mesh, elset, (*pOSet).weight + i);
    ol_q_memcpy (ElsetOri ? ElsetOri[elset] : Mesh.ElsetOri[elset], (*pOSet).q[i]);
  }

  if (Mesh.ElsetCrySym)
    ut_string_string (Mesh.ElsetCrySym, &(*pOSet).crysym);

  return;
}

void
neut_mesh_elts_orimean (struct NODES Nodes, struct MESH Mesh,
                         int *elts, int eltqty, double *mean)
{
  struct OL_SET OSet;

  neut_mesh_elts_olset (Nodes, Mesh, elts, eltqty, &OSet);

  ol_set_mean_iter (OSet, mean);

  ol_set_free (&OSet);

  return;
}

void
neut_mesh_elset_orianiso (struct NODES Nodes, struct MESH Mesh,
                          int elset,
                          double **evect, double *eval)
{
  neut_mesh_elts_orianiso (Nodes, Mesh,
                           Mesh.Elsets[elset] + 1,  Mesh.Elsets[elset][0],
                           evect, eval);

  return;
}

void
neut_mesh_elset_orianisogos (struct NODES Nodes, struct MESH Mesh,
                             int elset, double *pgos)
{
  double *eval = ut_alloc_1d (3);
  double **evect = ut_alloc_2d (3, 3);

  neut_mesh_elts_orianiso (Nodes, Mesh, Mesh.Elsets[elset] + 1,
                           Mesh.Elsets[elset][0], evect, eval);

  (*pgos) = sqrt (2 / M_PI) * ut_array_1d_lnorm (eval, 3, 1.58);

  ut_free_1d (&eval);
  ut_free_2d (&evect, 3);

  return;
}

void
neut_mesh_elts_orianiso (struct NODES Nodes, struct MESH Mesh,
                         int *elts, int eltqty,
                         double **evect, double *eval)
{
  struct OL_SET OSet;

  neut_mesh_elts_olset (Nodes, Mesh, elts, eltqty, &OSet);

  ol_set_disoriset (OSet, &OSet);

  ol_set_aniso (OSet, evect, eval);

  ol_set_free (&OSet);

  return;
}

void
neut_mesh_elset_gos (struct NODES Nodes, struct MESH Mesh,
                    int elset, double *pgos)
{
  neut_mesh_elts_gos (Nodes, Mesh, Mesh.Elsets[elset] + 1,
                      Mesh.Elsets[elset][0], pgos);

  return;
}

void
neut_mesh_elts_gos (struct NODES Nodes, struct MESH Mesh,
                    int *elts, int eltqty,
                    double *pgos)
{
  unsigned int i;
  double totvol, theta;
  struct OL_SET OSet;

  neut_mesh_elts_olset (Nodes, Mesh, elts, eltqty, &OSet);

  ol_set_disoriset (OSet, &OSet);

  (*pgos) = 0;
  totvol = 0;
  for (i = 0; i < OSet.size; i++)
  {
    ol_q_theta (OSet.q[i], &theta);
    *pgos += OSet.weight[i] * theta;
    totvol += OSet.weight[i];
  }
  (*pgos) /= totvol;

  ol_set_free (&OSet);

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

void
neut_mesh_elset_orianiso_delta (struct NODES Nodes, struct MESH Mesh, int elset,
                               double **evect_in, double *delta_in)
{
  neut_mesh_elts_orianiso_delta (Nodes, Mesh, Mesh.Elsets[elset] + 1, Mesh.Elsets[elset][0],
                                 evect_in, delta_in);

  return;
}

void
neut_mesh_elts_orianiso_delta (struct NODES Nodes, struct MESH Mesh, int *elts, int eltqty,
                               double **evect_in, double *delta_in)
{
  struct OL_SET Set;
  double **evect = ut_alloc_2d (3, 3);
  double *eval = ut_alloc_1d (3);
  double *delta = ut_alloc_1d (3);

  neut_mesh_elts_olset (Nodes, Mesh, elts, eltqty, &Set);

  ol_set_disoriset (Set, &Set);

  ol_set_aniso_delta (Set, evect, eval, delta);

  if (evect_in)
    ut_array_2d_memcpy (evect, 3, 3, evect_in);
  if (delta_in)
    ut_array_1d_memcpy (delta, 3, delta_in);

  ol_set_free (&Set);
  ut_free_2d (&evect, 3);
  ut_free_1d (&eval);
  ut_free_1d (&delta);

  return;
}
