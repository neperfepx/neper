/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_.h"

int
nem_meshing_2D_face (struct IN_M In, struct MESHPARA MeshPara,
		     struct MULTIM *pMultim, struct timeval *pctrlc_t,
		     double *pallowed_t, double *pmax_elapsed_t,
		     struct TESS Tess, struct NODES RNodes,
		     struct MESH *RMesh, struct NODES *pNodes,
		     struct MESH *Mesh, int face)
{
  int a, master;
  double elapsed_t, mOsize;
  struct NODES N, N2;
  struct MESH M, M2;
  int *master_id = NULL;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  if (Tess.Dim == 2 || strncmp (Tess.Type, "periodic", 8) != 0
      || Tess.PerFaceMaster[face] == 0)
  {
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
	  neut_nodes_memcpy (N2, &N);
	  neut_mesh_memcpy (M2, &M);
	}

	// minimum quality reached; breaking
	if ((*pMultim).mO[face][a] > In.meshqualmin)
	  break;
      }
    else
      neut_mesh_elset_mesh (RNodes, RMesh[2], face, &N, &M, NULL);
  }

  else
  {
    master = Tess.PerFaceMaster[face];
    neut_mesh_elset_mesh (*pNodes, Mesh[2], master, &N, &M, &master_id);
    neut_nodes_shift (&N,
		      Tess.PerFaceShift[face][0] * Tess.PeriodicDist[0],
		      Tess.PerFaceShift[face][1] * Tess.PeriodicDist[1],
		      Tess.PerFaceShift[face][2] * Tess.PeriodicDist[2]);
    if (Tess.PerFaceOri[face] == 1)
      neut_mesh_reversenodes (&M);
  }


  if ((*pMultim).Oalgo[face] != -1)
    (*pMultim).algohit[(*pMultim).Oalgo[face]]++;
  else
    ut_print_message (2, 3, "Meshing of face %d failed\n", face);

  // Recording face mesh in global mesh
  nem_meshing_2D_face_record (Tess, face, N, M, master_id, pNodes, Mesh,
			      MeshPara);

  neut_nodes_free (&N);
  neut_nodes_free (&N2);
  neut_mesh_free (&M);
  neut_mesh_free (&M2);
  ut_free_1d_int (master_id);

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
