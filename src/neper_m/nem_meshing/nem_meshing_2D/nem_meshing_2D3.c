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

  /* mesh (iterate if meshing fails) */
  for (iter = 0; iter < In.mesh2diter; iter++)
  {
    rnd = iter * 1.e-5;
    face_eq = MeshPara.face_eq ? MeshPara.face_eq[face] : NULL;

    status = nem_mesh_2d_gmsh (Tess, face, face_eq,
			       Nodes, Mesh, MeshPara.face_cl[face],
			       In.gmsh, In.tmp,
			       (*pMultim).algos[algo][0],
			       rnd, *pallowed_t, pN,
			       pM, pmOsize, pctrlc_t, pelapsed_t);

    if (status == 0)		// success
    {
      (*pmax_elapsed_t) = ut_num_max ((*pmax_elapsed_t), *pelapsed_t);
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
nem_meshing_2D_face_record (struct TESS Tess, int face, struct NODES N,
			    struct MESH M, int *master_id,
			    struct NODES *pNodes,
			    struct MESH *Mesh, struct MESHPARA MeshPara)
{
  int *node_nbs = NULL;

  nem_meshing_2D_face_record_nodes (Tess, face, N, M, master_id,
				    &node_nbs, pNodes, Mesh, MeshPara);

  nem_meshing_2D_face_record_elts (face, M, node_nbs, Mesh);

  ut_free_1d_int (node_nbs);

  return;
}
