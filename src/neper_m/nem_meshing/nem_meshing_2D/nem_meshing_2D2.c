/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

int
nem_meshing_2D_face (struct IN_M In, struct MESHPARA MeshPara,
		     struct MULTIM *pMultim, struct timeval *pctrlc_t,
		     double *pallowed_t, double *pmax_elapsed_t,
		     struct TESS Tess, struct NODES RNodes,
		     struct MESH *RMesh,
                     struct NODES *pNodes,
		     struct MESH *Mesh,
                     struct NODES *pN, struct MESH *pM,
                     int face)
{
  int a;
  double elapsed_t, mOsize;
  struct NODES N2;
  struct MESH M2;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);
  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  (*pMultim).Oalgo[face] = -1;
  if (!MeshPara.face_op || !MeshPara.face_op[face])
    for (a = 0; a < (*pMultim).algoqty; a++)
    {
      // meshing
      nem_meshing_2D_face_algo (In, MeshPara, pMultim, a, pctrlc_t,
                                pallowed_t, pmax_elapsed_t, Tess,
                                RNodes, RMesh, *pNodes, Mesh, face,
                                &N2, &M2, &mOsize, &elapsed_t);

      // best-quality mesh, recording it
      if ((*pMultim).Oalgo[face] == a)
      {
        neut_nodes_memcpy (N2, pN);
        neut_mesh_memcpy (M2, pM);
      }

      // minimum quality reached; breaking
      if ((*pMultim).mO[face][a] > In.meshqualmin)
        break;
    }
  else
    neut_mesh_elset_mesh (RNodes, RMesh[2], face, pN, pM, NULL);

  if ((*pMultim).Oalgo[face] != -1)
#pragma omp critical
    (*pMultim).algohit[(*pMultim).Oalgo[face]]++;
  else
    ut_print_message (2, 3, "Meshing of face %d failed\n", face);

  neut_nodes_free (&N2);
  neut_mesh_free (&M2);

  return 0;
}

int
nem_meshing_2D_face_per (struct TESS Tess, struct NODES *N,
                         struct MESH *M, struct NODES *pN,
                         struct MESH *pM, int **pmaster_id,
                         int face)

{
  int master;

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  master = Tess.PerFaceMaster[face];
  neut_mesh_elset_mesh (N[master], M[master], 1, pN, pM, pmaster_id);
  neut_nodes_shift (pN,
                    Tess.PerFaceShift[face][0] * Tess.PeriodicDist[0],
                    Tess.PerFaceShift[face][1] * Tess.PeriodicDist[1],
                    Tess.PerFaceShift[face][2] * Tess.PeriodicDist[2]);
  if (Tess.PerFaceOri[face] == 1)
    neut_mesh_reversenodes (pM);

  return 0;
}

void
nem_meshing_2D_progress (struct MULTIM Multim, int face, int faceqty,
			 char *message)
{
  int a;
  int *pct = ut_alloc_1d_int (Multim.algoqty);
  char *tmp = ut_alloc_1d_char (128);
  char *format = ut_alloc_1d_char (128);

  sprintf (format, "%%3.0f%%%% (%.2g|%.2g/",
	   (face > 0) ? ut_array_1d_min (Multim.O + 1, face) : 0,
	   (face > 0) ? ut_array_1d_mean (Multim.O + 1, face) : 0);

  if (face > 0)
    ut_array_1d_int_percent (Multim.algohit, Multim.algoqty, pct);

  for (a = 0; a < Multim.algoqty; a++)
  {
    sprintf (tmp, "%s%2d%%%c", format, pct[a],
	     (a < Multim.algoqty - 1) ? '|' : ')');
    strcpy (format, tmp);
  }

  ut_print_progress (stdout, face, faceqty, format, message);

  ut_free_1d_int (pct);
  ut_free_1d_char (format);
  ut_free_1d_char (tmp);

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
