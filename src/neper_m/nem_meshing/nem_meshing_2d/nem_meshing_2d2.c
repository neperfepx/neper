/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2d_.h"

void
nem_meshing_2d_progress (struct MULTIM Multim, int face, int faceqty,
                         char *message)
{
  int i, a;
  int *pct = ut_alloc_1d_int (Multim.algoqty);
  double mean;
  char *tmp = ut_alloc_1d_char (128);
  char *format = ut_alloc_1d_char (128);

  if (face > 0)
  {
    mean = ut_array_1d_mean (Multim.O + 1, face);
    for (i = 1; i <= face; i++)
      if (Multim.O[i] == 0)
        Multim.O[i] = mean;
  }

  sprintf (format, "%%3.0f%%%% (%.2g|%.2g/",
           (face > 0) ? ut_array_1d_min (Multim.O + 1, face) : 0,
           (face > 0) ? ut_array_1d_mean (Multim.O + 1, face) : 0);

  if (face > 0)
    ut_stat_scaletopercent_int (Multim.algohit, Multim.algoqty, pct);

  for (a = 0; a < Multim.algoqty; a++)
  {
    sprintf (tmp, "%s%2d%%%c", format, pct[a],
             (a < Multim.algoqty - 1) ? '|' : ')');
    strcpy (format, tmp);
  }

  ut_print_progress (stdout, face, faceqty, format, message);

  ut_free_1d_int (&pct);
  ut_free_1d_char (&format);
  ut_free_1d_char (&tmp);

  return;
}

int
nem_meshing_2d_face (struct IN_M In, struct MESHPARA MeshPara,
                     struct MULTIM *pMultim, struct timeval *pctrlc_t,
                     double *pallowed_t, double *pmax_elapsed_t,
                     struct TESS Tess, struct NODES RNodes,
                     struct MESH *RMesh, struct NODES Nodes,
                     struct MESH *Mesh, struct NODES *pN, struct MESH *pM,
                     int **pbnodes, int **plbnodes, int *pbnodeqty, int face)
{
  int a, *bnodes2 = NULL, *lbnodes2 = NULL, bnodeqty2;
  double elapsed_t, mOsize;
  struct NODES N2;
  struct MESH M2;

  neut_nodes_set_zero (&N2);
  neut_mesh_set_zero (&M2);

  // if the face must be copied, we copy and that's it
  if (!strcmp (MeshPara.face_op[face], "copy"))
    neut_mesh_elset_mesh (RNodes, RMesh[2], face, pN, pM, NULL);

  // otherwise, running multimeshing
  else
  {
    (*pMultim).Oalgo[face] = -1;
    for (a = 0; a < (*pMultim).algoqty; a++)
    {
      // meshing
      nem_meshing_2d_face_mesh (In, MeshPara, pMultim, a, pctrlc_t,
                                pallowed_t, pmax_elapsed_t, Tess, RNodes,
                                RMesh, Nodes, Mesh, face, &N2, &M2, &bnodes2,
                                &lbnodes2, &bnodeqty2, &mOsize, &elapsed_t);

      // if best-quality mesh, then recording it
      if ((*pMultim).Oalgo[face] == a)
      {
        neut_nodes_memcpy (N2, pN);
        neut_mesh_memcpy (M2, pM);
        *pbnodeqty = bnodeqty2;
        (*pbnodes) = ut_realloc_1d_int (*pbnodes, *pbnodeqty);
        (*plbnodes) = ut_realloc_1d_int (*plbnodes, *pbnodeqty);
        ut_array_1d_int_memcpy (bnodes2, *pbnodeqty, *pbnodes);
        ut_array_1d_int_memcpy (lbnodes2, *pbnodeqty, *plbnodes);
      }

      // if mesh quality criterion reached, breaking
      if ((*pMultim).mO[face][a] > In.meshqualmin)
        break;
    }

    if ((*pMultim).Oalgo[face] != -1)
#pragma omp critical
      (*pMultim).algohit[(*pMultim).Oalgo[face]]++;
    else
      ut_print_message (2, 3, "Meshing of face %d failed\n", face);
  }

  neut_nodes_free (&N2);
  neut_mesh_free (&M2);
  ut_free_1d_int (&bnodes2);
  ut_free_1d_int (&lbnodes2);

  return 0;
}

int
nem_meshing_2d_face_per (struct TESS Tess, struct NODES Nodes,
                         struct NODES *N, struct MESH *M, struct NODES *pN,
                         struct MESH *pM, int **pmaster_id, int ***pbnodes,
                         int ***plbnodes, int **pbnodeqty, int face)
{
  int i, status, master = Tess.PerFaceMaster[face];

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  neut_nodes_memcpy (N[master], pN);
  neut_mesh_memcpy (M[master], pM);

  neut_nodes_shift (pN, Tess.PerFaceShift[face][0] * Tess.PeriodicDist[0],
                    Tess.PerFaceShift[face][1] * Tess.PeriodicDist[1],
                    Tess.PerFaceShift[face][2] * Tess.PeriodicDist[2]);

  (*pmaster_id) = ut_alloc_1d_int (N[master].NodeQty + 1);
  ut_array_1d_int_set_id (*pmaster_id, N[master].NodeQty + 1);

  (*pbnodeqty)[face] = (*pbnodeqty)[master];
  (*plbnodes)[face] =
    ut_realloc_1d_int ((*plbnodes)[face], (*pbnodeqty)[face]);
  (*pbnodes)[face] = ut_realloc_1d_int ((*pbnodes)[face], (*pbnodeqty)[face]);

  ut_array_1d_int_memcpy ((*plbnodes)[master], (*pbnodeqty)[face],
                          (*plbnodes)[face]);
  ut_array_1d_int_memcpy ((*pbnodes)[master], (*pbnodeqty)[face],
                          (*pbnodes)[face]);

  int *shift = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy (Tess.PerFaceShift[face], 3, shift);
  ut_array_1d_int_scale (shift, 3, 1);
  for (i = 0; i < (*pbnodeqty)[face]; i++)
  {
    status =
      neut_nodes_node_shift_pernode (Nodes, (*pbnodes)[master][i], shift,
                                     (*pbnodes)[face] + i);
    if (status)
      abort ();
  }
  ut_free_1d_int (&shift);

  if (Tess.PerFaceOri[face] == 1)
    neut_mesh_reversenodes (pM);

  return 0;
}

void
nem_meshing_2d_face_record (struct TESS Tess, int face, struct NODES N,
                            struct MESH M, int *bnodes, int *lbnodes,
                            int bnodeqty, int *master_id,
                            struct NODES *pNodes, int **N_global_id,
                            struct MESH *Mesh)
{
  nem_meshing_2d_face_record_nodes (Tess, face, N, bnodes, lbnodes, bnodeqty,
                                    master_id, N_global_id, pNodes);

  nem_meshing_2d_face_record_elts (face, M, N_global_id[face], Mesh);

  return;
}
