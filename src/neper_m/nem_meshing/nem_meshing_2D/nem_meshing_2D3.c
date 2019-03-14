/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

void
nem_meshing_2D_face_algo (struct IN_M In, struct MESHPARA MeshPara,
			  struct MULTIM *pMultim, int algo,
			  struct timeval *pctrlc_t,
			  double *pallowed_t, double *pmax_elapsed_t,
			  struct TESS Tess, struct NODES RNodes,
			  struct MESH *RMesh, struct NODES Nodes,
			  struct MESH *Mesh, int face,
			  struct NODES *pN, struct MESH *pM,
			  double *pmOsize, double *pelapsed_t)
{
  int iter, status = -1;
  double rnd;
  int var_qty = 2;
  char **vars = ut_alloc_2d_char (var_qty, 10);
  double *vals = ut_alloc_1d (var_qty);
  double *face_eq = NULL;

  strcpy (vars[0], "Odis");
  strcpy (vars[1], "Osize");

  face_eq = MeshPara.face_eq ? MeshPara.face_eq[face] : NULL;
  /* mesh (iterate if meshing fails) */
  for (iter = 0; iter < In.mesh2diter; iter++)
  {
    rnd = iter * 1.e-5;

    status = nem_mesh_2d_gmsh (Tess, face, face_eq,
			       Nodes, Mesh, MeshPara.face_cl[face],
			       In.gmsh, In.tmp,
			       (*pMultim).algos[algo][0],
			       rnd, *pallowed_t, pN,
			       pM, pmOsize, pctrlc_t, pelapsed_t);

    if (status == 0)		// success
    {
#pragma omp critical
      (*pmax_elapsed_t) = ut_num_max ((*pmax_elapsed_t), *pelapsed_t);
#pragma omp critical
      (*pallowed_t) =
	ut_num_min (In.mesh2dmaxtime, In.mesh2drmaxtime * (*pmax_elapsed_t));
      break;
    }
    if (status == 2)		// meshing succeeded, but skin mesh changed
      break;
  }

  // back-projecting mesh
  if (MeshPara.face_eq)
    nem_meshing_2D_face_proj (Tess, RNodes, RMesh, face,
			      MeshPara.face_eq[face], pN);

  // recording meshquality information in Multim
  if (!status)
  {
    neut_mesh_Odis (*pN, *pM, In.meshqualdisexpr,
		    &((*pMultim).mOdis[face][algo]));
    neut_mesh_Osize (*pN, *pM, MeshPara.face_cl[face],
		     &((*pMultim).mOsize[face][algo]));

    if (!strcmp (In.meshqualexpr, "Odis^0.8*Osize^0.2"))
      (*pMultim).mO[face][algo] =
	pow ((*pMultim).mOdis[face][algo], 0.8)
	* pow ((*pMultim).mOsize[face][algo], 0.2);
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

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return;
}

void
nem_meshing_2D_face_record_nodes (struct TESS Tess, int face, struct NODES N,
                                  int *master_id, int **N_global_id,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  struct MESHPARA MeshPara)
{
  int i, j, bak, nb, tmp, bnodeqty, *bnodes = NULL;
  double *dist = NULL;
  double *bnodedists = NULL;

  /* calculating the node numbers - some are already recorded ("1D"
   * nodes), the others are new.  We use the node positions. */

  neut_mesh_elsets_nodes (Mesh[1], Tess.FaceEdgeNb[face] + 1,
			  Tess.FaceVerQty[face], &bnodes, &bnodeqty);

  bnodedists = ut_alloc_1d (bnodeqty);

  dist = ut_alloc_1d (bnodeqty);

  for (i = 0; i < bnodeqty; i++)
  {
    for (j = 0; j < bnodeqty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[bnodes[i]],
			       (*pNodes).NodeCoo[bnodes[j]]);

    dist[i] = DBL_MAX;
    bnodedists[i] = ut_array_1d_min (dist, bnodeqty);
  }

  ut_free_1d_ (&dist);

  N_global_id[face] = ut_alloc_1d_int (N.NodeQty + 1);

  /* there are bnodeqty nodes whose positions in Nodes are
   * recorded in bnodes.  The nodes are at random places in N.
   * Searching them and their numbers, in turn. */

  dist = ut_alloc_1d (N.NodeQty + 1);

  /* for every skin node (whose pos in Nodes / coo is known, looking
   * for it in N. */
  for (i = 0; i < bnodeqty; i++)
  {
    for (j = 1; j <= N.NodeQty; j++)
      dist[j] = ut_space_dist ((*pNodes).NodeCoo[bnodes[i]], N.NodeCoo[j]);

    tmp = 1 + ut_array_1d_min_index (dist + 1, N.NodeQty);

    if (dist[tmp] > 1e-3 * bnodedists[i] || N_global_id[face][tmp] != 0)
    {
      printf ("dist = %g > %g for parent node:", dist[tmp], 1e-3 * bnodedists[i]);
      ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[bnodes[i]], 3, "%f");
      printf ("projection along/onto:");
      ut_array_1d_fprintf (stdout, MeshPara.face_eq[face], 3, "%f");
      ut_array_1d_fprintf (stdout, Tess.FaceEq[face], 4, "%f");

      printf ("parent 1D nodes (%d):\n", bnodeqty);
      for (j = 0; j < bnodeqty; j++)
	ut_array_1d_fprintf (stdout, (*pNodes).NodeCoo[bnodes[j]], 3,
			     "%f");
      printf ("returned nodes (%d):\n", N.NodeQty);
      ut_array_2d_fprintf (stdout, N.NodeCoo + 1, N.NodeQty, 3, "%f");

      ut_error_reportbug ();
      abort ();
    }

    N_global_id[face][tmp] = bnodes[i];
  }

  nb = (*pNodes).NodeQty;

  /* new nodes: their node_nbs are zero; filling with new values */
  tmp = ut_array_1d_int_eltpos (N_global_id[face] + 1, N.NodeQty, 0);
  while (tmp != -1)
  {
    tmp++;
    N_global_id[face][tmp] = ++nb;
    tmp = ut_array_1d_int_eltpos (N_global_id[face] + 1, N.NodeQty, 0);
  }

  /* adding body nodes to global nodes */
  bak = (*pNodes).NodeQty;
  for (i = 1; i <= N.NodeQty; i++)
    if (N_global_id[face][i] > bak)
    {
      neut_nodes_addnode (pNodes, N.NodeCoo[i], -1);

      if (master_id)
        neut_nodes_markasslave (pNodes, (*pNodes).NodeQty,
                                N_global_id[Tess.PerFaceMaster[face]][master_id[i]],
                                Tess.PerFaceShift[face]);
    }

  ut_free_1d_int (bnodes);
  ut_free_1d (bnodedists);
  ut_free_1d (dist);

  return;
}

void
nem_meshing_2D_face_record_elts (int face, struct MESH M, int *node_nbs,
				 struct MESH *Mesh)
{
  int i, j;
  int *elt_nbs = NULL;

  /* renumbering mesh nodes to match global nodes */
  for (i = 1; i <= M.EltQty; i++)
    for (j = 0; j < 3; j++)
    {
      M.EltNodes[i][j] = node_nbs[M.EltNodes[i][j]];
      if (M.EltNodes[i][j] == 0)
	ut_error_reportbug ();
    }

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
    ut_error_reportbug ();

  ut_free_1d_int (elt_nbs);

  return;
}
