/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_.h"

void
nem_meshing_2d_face_mesh (struct IN_M In, struct MESHPARA MeshPara,
                          struct MULTIM *pMultim, int algo,
                          struct timeval *pctrlc_t, double *pallowed_t,
                          double *pmax_elapsed_t, struct TESS Tess,
                          struct NODES RNodes, struct MESH *RMesh,
                          struct NODES Nodes, struct MESH *Mesh, int face,
                          struct NODES *pN, struct MESH *pM, int **pbnodes,
                          int **plbnodes, int *pbnodeqty, double *pmOsize,
                          double *pelapsed_t)
{
  int iter, status = -1;
  double rnd;
  int var_qty = 2;
  char **vars = ut_alloc_2d_char (var_qty, 10);
  double *vals = ut_alloc_1d (var_qty);

  (*pbnodeqty) = 0;
  ut_free_1d_int (pbnodes);
  ut_free_1d_int (plbnodes);

  strcpy (vars[0], "Odis");
  strcpy (vars[1], "Osize");

  // mesh (iterate if meshing fails)
  for (iter = 0; iter < In.mesh2diter; iter++)
  {
    rnd = iter * 1.e-5;

    status =
      nem_meshing_2d_face_mesh_gmsh (Tess, RNodes, RMesh, face, MeshPara,
                                     Nodes, Mesh, MeshPara.face_cl[face],
                                     In.gmsh, In.tmp,
                                     (*pMultim).algos[algo][0], rnd,
                                     *pallowed_t, pN, pM, pbnodes, plbnodes,
                                     pbnodeqty, pmOsize, pctrlc_t,
                                     pelapsed_t);
    // success
    if (!status)
    {
#pragma omp critical
      (*pmax_elapsed_t) = ut_num_max ((*pmax_elapsed_t), *pelapsed_t);
#pragma omp critical
      (*pallowed_t) =
        ut_num_min (In.mesh2dmaxtime, In.mesh2drmaxtime * (*pmax_elapsed_t));
      break;
    }

    // meshing succeeded, but boundary mesh changed
    else if (status == 2)
      break;
  }

  // recording mesh-quality information in Multim
  if (!status)
  {
    neut_mesh_Odis (*pN, *pM, In.meshqualdisexpr,
                    &((*pMultim).mOdis[face][algo]));
    neut_mesh_Osize (*pN, *pM, MeshPara.face_cl[face],
                     &((*pMultim).mOsize[face][algo]));

    if (!strcmp (In.meshqualexpr, "Odis^0.8*Osize^0.2"))
      (*pMultim).mO[face][algo] =
        pow ((*pMultim).mOdis[face][algo],
             0.8) * pow ((*pMultim).mOsize[face][algo], 0.2);
    else
    {
      vals[0] = (*pMultim).mOdis[face][algo];
      vals[1] = (*pMultim).mOsize[face][algo];
      ut_math_eval (In.meshqualexpr, var_qty, vars, vals,
                    &((*pMultim).mO[face][algo]));
    }

    if ((*pMultim).Oalgo[face] == -1
        || ((*pMultim).mO[face][algo] >
            (*pMultim).mO[face][(*pMultim).Oalgo[face]]))
    {
      (*pMultim).Oalgo[face] = algo;
      (*pMultim).Odis[face] = (*pMultim).mOdis[face][(*pMultim).Oalgo[face]];
      (*pMultim).Osize[face] =
        (*pMultim).mOsize[face][(*pMultim).Oalgo[face]];
      (*pMultim).O[face] = (*pMultim).mO[face][(*pMultim).Oalgo[face]];
    }
  }

  else
  {
    (*pMultim).mOdis[face][algo] = 0;
    (*pMultim).mOsize[face][algo] = 0;
    (*pMultim).mO[face][algo] = 0;
  }

  ut_free_2d_char (&vars, var_qty);
  ut_free_1d (&vals);

  return;
}

void
nem_meshing_2d_face_record_nodes (struct TESS Tess, int face, struct NODES N,
                                  int *bnodes, int *lbnodes, int bnodeqty,
                                  int *master_id, int **N_global_id,
                                  struct NODES *pNodes)
{
  int i, bak, nb, tmp;

  // calculating the node numbers - some are already recorded ("1D"
  // nodes), the others are new.

  N_global_id[face] = ut_alloc_1d_int (N.NodeQty + 1);
  for (i = 0; i < bnodeqty; i++)
    N_global_id[face][lbnodes[i]] = bnodes[i];

  nb = (*pNodes).NodeQty;

  // new nodes: their node nbs are zero; filling with new values
  tmp = ut_array_1d_int_eltpos (N_global_id[face] + 1, N.NodeQty, 0);
  while (tmp != -1)
  {
    tmp++;
    N_global_id[face][tmp] = ++nb;
    tmp = ut_array_1d_int_eltpos (N_global_id[face] + 1, N.NodeQty, 0);
  }

  // adding body nodes to global nodes
  bak = (*pNodes).NodeQty;
  for (i = 1; i <= N.NodeQty; i++)
    if (N_global_id[face][i] > bak)
    {
      neut_nodes_addnode (pNodes, N.NodeCoo[i], -1);

      if (master_id)
        neut_nodes_markasslave (pNodes, (*pNodes).NodeQty,
                                N_global_id[Tess.
                                            PerFaceMaster[face]][master_id
                                                                 [i]],
                                Tess.PerFaceShift[face]);
    }

  return;
}

void
nem_meshing_2d_face_record_elts (int face, struct MESH M, int *N_global_id,
                                 struct MESH *Mesh)
{
  int i, j, *elt_nbs = NULL;

  /* renumbering mesh nodes to match global nodes */
  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 3; j++)
      M.EltNodes[i][j] = N_global_id[M.EltNodes[i][j]];

  /* renumbering mesh elts */
  elt_nbs = ut_alloc_1d_int (M.EltQty + 1);
  for (i = 1; i <= M.EltQty; i++)
    elt_nbs[i] = Mesh[2].EltQty + i;

  for (j = 1; j <= M.Elsets[1][0]; j++)
    M.Elsets[1][j] = elt_nbs[M.Elsets[1][j]];

  for (i = 1; i <= M.EltQty; i++)
    neut_mesh_addelt (Mesh + 2, M.EltNodes[i]);

  neut_mesh_addelset (Mesh + 2, M.Elsets[1] + 1, M.Elsets[1][0]);

  if (Mesh[2].ElsetQty != face)
    ut_print_neperbug ();

  ut_free_1d_int (&elt_nbs);

  return;
}
